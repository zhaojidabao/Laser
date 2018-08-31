// MarkMulDocDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "HLDoc.h"
#include "MarkMulDocDlg.h"
#include "SetRunDlg.h"
#include <mmsystem.h>
#include "MainFrm.h"
#include "DlgMotorMoveWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLOCKFREQ_AND_TOTTICKS 100	//VXD
#define W32IF_PASS_EVENT 110
 
/////////////////////////////////////////////////////////////////////////////
// CMarkMulDocDlg dialog


CMarkMulDocDlg::CMarkMulDocDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CMarkMulDocDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkMulDocDlg)
	m_bManual = FALSE;
	m_nMarkType = 0;
	m_bLoopMark =  FALSE;
	m_nIntervalTime = 100;
	//}}AFX_DATA_INIT
	m_nCount = -1;
	m_dwTotal = 0;
	m_dwTotalStart = 0;
	m_dwTotalEnd = 0;
	m_dwSingle = 0;
	m_nCur = 0;
	m_bPrevFrame = FALSE;
}


void CMarkMulDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkMulDocDlg)
	DDX_Control(pDX, IDC_PROGRESS_MARK, m_progressMark);
	DDX_Control(pDX, IDC_LIST_DOCNAME, m_listDocName);
	DDX_Check(pDX, IDC_CHECK_MANUAL, m_bManual);
	DDX_Radio(pDX, IDC_RADIO_LASER, m_nMarkType);
	DDX_Check(pDX, IDC_CHECK_LOOPMULMARK, m_bLoopMark);
	DDX_Text(pDX, IDC_EDIT_INTERVALTIME, m_nIntervalTime);
	DDV_MinMaxInt(pDX, m_nIntervalTime, 0, 9999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkMulDocDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkMulDocDlg)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_CHECK_MANUAL, OnCheckManual)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_MOTORHOME, OnButtonMotorhome)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MARK_END,OnMarkEnd)
	ON_MESSAGE(WM_MARK_START,OnMarkStart)
	ON_MESSAGE(WM_MARK_STOP,OnMarkStop)
	ON_MESSAGE(WM_OUTPUT_MARK_MSG,OnOutPutMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkMulDocDlg message handlers
BOOL CMarkMulDocDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//取得各打开文档的指针
	CHLApp *pApp = (CHLApp *)AfxGetApp();
	CMultiDocTemplate *pDocTem = NULL;
	CHLDoc *pDoc = NULL;
	
	POSITION posTem = pApp->GetFirstDocTemplatePosition();
	while( posTem  !=  NULL )
	{
		pDocTem = (CMultiDocTemplate *)pApp->GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc  !=  NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			if ( pDoc == NULL )
				continue;

			pDoc->PreprocessMark();
			if ( pDoc->OutOfLimit() )
			{
				AfxMessageBox(IDS_ERROT_OUTAREA);
				CDialog::OnCancel();
				return FALSE;
			}
			m_arrayDoc.Add(pDoc);
			m_listDocName.AddString(pDoc->GetTitle());
		}
	}
	m_listDocName.SetSel(0);
	if ( m_listDocName.GetCount() == 0 )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	m_pMarkMgr = new CMarkMgr();
	if ( !m_pMarkMgr )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	m_pMarkMgr->RunThread((LPVOID)GetSafeHwnd());
	
	m_progressMark.SetRange32(0,10);
	m_progressMark.SetStep(1);
	
	m_bFirst = TRUE;
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	UpdateView();
	
	UpdateTime();
	UpdateCount();
	
	if ( FALSE == g_DAT.m_btExtendInfo.FunData.bMotorMark )
		GetDlgItem(IDC_BUTTON_MOTORHOME)->ShowWindow(FALSE);
	
	g_DAT.OutStatus(O_ENT_MARK_ENV,O_ENT_MARK_ENV);
	g_DAT.m_hMarkWnd = GetSafeHwnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarkMulDocDlg::OnButtonDown() 
{
	int nCount = m_listDocName.GetCount();
	if ( 0 == nCount )
		return;
	
	int nSel = m_listDocName.GetCurSel();
	if ( nSel == LB_ERR || nSel == (nCount-1) )
		return;

	m_listDocName.SetSel(nSel);
	m_listDocName.SelItemRange(FALSE,0,nCount);

	CString str;
	m_listDocName.GetText(nSel,str);
	m_listDocName.DeleteString(nSel);
	m_listDocName.InsertString((nSel+1),str);
	m_listDocName.SetSel(nSel+1);
	UpdateData(FALSE);	
	
	CHLDoc *pDoc = (CHLDoc *)m_arrayDoc.GetAt(nSel);
	m_arrayDoc.RemoveAt(nSel);
	m_arrayDoc.InsertAt(nSel+1,pDoc);
}



void CMarkMulDocDlg::OnButtonUp()
{
	int nCount = m_listDocName.GetCount();
	if ( 0 == nCount )
		return;
	
	int nSel = m_listDocName.GetCurSel();
	if ( nSel == LB_ERR || nSel < 1 )
		return;
	
	m_listDocName.SetSel(nSel);
	m_listDocName.SelItemRange(FALSE,0,nCount);
	
	CString str;
	m_listDocName.GetText(nSel,str);
	m_listDocName.DeleteString(nSel);
	m_listDocName.InsertString((nSel-1),str);
	m_listDocName.SetSel(nSel-1);
	UpdateData(FALSE);
	
	CHLDoc *pDoc = (CHLDoc *)m_arrayDoc.GetAt(nSel);
	m_arrayDoc.RemoveAt(nSel);
	m_arrayDoc.InsertAt(nSel-1,pDoc);
}

void CMarkMulDocDlg::OnButtonDel() 
{
	//multiple-document;
	int nCount = m_listDocName.GetSelCount();
	if ( 0 == nCount )
		return;
	
	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_listDocName.GetSelItems(nCount, aryListBoxSel.GetData());

	int nSel;
	for(int i = 0;i<nCount;i++)
	{
		nSel = aryListBoxSel.GetAt(i);
		if ( nSel == LB_ERR ) 
			return;

		m_listDocName.DeleteString(nSel-i);
		m_arrayDoc.RemoveAt(nSel-i);
	}

	UpdateData(FALSE);
}

void CMarkMulDocDlg::OnCancel()
{
	m_pMarkMgr->SetChain(NULL);
	m_pMarkMgr->StopThread();

	delete m_pMarkMgr;
	m_pMarkMgr = NULL;

	g_DAT.m_hMarkWnd = NULL;


	CDialog::OnCancel();
}




void CMarkMulDocDlg::OnButtonMark() 
{
	UpdateData(TRUE);
	
	if ( 0 == m_arrayDoc.GetSize() ) 
		return;
	
	Mark();
}


void CMarkMulDocDlg::OnButtonStop() 
{
	//设置停止事件，打标线程检测到该事件后退出打标
	m_nCur  = 0;
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	m_bLoopMark = FALSE;
	m_pMarkMgr->StopMark();
	UpdateView();
}

void CMarkMulDocDlg::UpdateView()
{
	CString str;
	if ( m_bMark )
	{
		str.LoadString(IDS_MARK_ON);
	}
	else
	{
		str.LoadString(IDS_MARK_PAUSE);
	}
	SetDlgItemText(IDC_BUTTON_MARK,str);
	
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bMarkStop);
	GetDlgItem(IDCANCEL)->EnableWindow(m_bMarkExit);
	
	GetDlgItem(IDC_RADIO_LASER)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_RED)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_FRAME)->EnableWindow(m_bMarkExit);	
	GetDlgItem(IDC_BUTTON_MOTORHOME)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_CHECK_MANUAL)->EnableWindow(m_bMarkExit);
	
	UpdateData(FALSE);
}



LRESULT CMarkMulDocDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd();
		break;
	case MARK_SEND:
		m_pDoc->SetModifiedFlag();
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}

	return 0;
}



void CMarkMulDocDlg::MarkEnd()
{
	m_pDoc->SetModifiedFlag();
	MarkSEnd();
	m_bFirst = TRUE;
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	
	Run();	//工作台移动
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));

	int nCount = m_listDocName.GetCount();
	m_listDocName.SelItemRange(FALSE,0,nCount);
	m_listDocName.SetSel(m_nCur);

 	UpdateView();
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));
	
	if ( m_bManual )
	{
		if ( m_nCur == 0 )
		{
			m_dwTotalEnd = timeGetTime();
			m_dwTotal = m_dwTotalEnd - m_dwTotalStart;
			UpdateCount();
			m_dwTotalStart = 0;
		}
	}
	else
	{
		if ( m_nCur )
		{
			PostMessage(WM_MARK_START);
		}
		else
		{
			// 消息循环
			if ( m_bLoopMark )
			{
				Sleep(m_nIntervalTime);
				PostMessage(WM_MARK_START);
			}
			else
			{
				m_dwTotalEnd = timeGetTime();
				m_dwTotal  = m_dwTotalEnd -m_dwTotalStart;
				UpdateCount();
				m_dwTotalStart = 0;
			}
		}
	}
}



//
LRESULT CMarkMulDocDlg::OnOutPutMsg( WPARAM wParam,LPARAM lParam )
{
	TCHAR szMsg[255] = {0};
	if ( HLERROR == g_DAT.GetErrInfo(wParam, szMsg, 255) )
		return 1;
	
	CString strCurMsg;
	GetWindowText(strCurMsg);
	if ( _tcslen(szMsg) <= 0 || 0 == strCurMsg.CompareNoCase(szMsg) )
		return 1;
	
	SetWindowText(szMsg);
	
	if ( !m_bMarkStop )
		return 1;
	
	UINT nType    = (UINT)((lParam & 0xFFFF0000) >> 16);
	UINT nValue   = (UINT)(lParam  & 0xFFFF);
	switch( nType )
	{
	case 1:
		{
			switch(nValue)
			{
			case 0:
				break;
			case 1:
				OnButtonStop();
				break;
			case 2:
				Mark();
				GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(FALSE);
				break;
			case 3:
				if ( 1 == g_SysCfg.m_nSafeDoorRemark )
					Mark();
				
				GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(TRUE);
				break;
			default:
				break;
			}
			break;
		}
	default:
		break;
	}
	
	return 0;
}


void CMarkMulDocDlg::MarkSEnd()
{
	m_progressMark.SetPos(0);
	m_dwSingle = m_pMarkMgr->GetSingleMarkTime();
	
	CMainFrame *pMFrame = (CMainFrame*)AfxGetMainWnd();
	if ( pMFrame  !=  NULL )
		pMFrame->SetMarkTime(m_dwSingle);
	
	UpdateTime();
}




LRESULT CMarkMulDocDlg::OnMarkStop(WPARAM wParam,LPARAM lParam)
{
	OnButtonStop();
	
	return 0;
}



LRESULT CMarkMulDocDlg::OnMarkStart(WPARAM wParam,LPARAM lParam)
{
	Mark();
	
	return 0;
}


void CMarkMulDocDlg::OnCheckManual()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


BOOL CMarkMulDocDlg::GetNext()
{
	m_nCur = m_listDocName.GetCurSel();
	m_pDoc = (CHLDoc *)m_arrayDoc.GetAt(m_nCur++);
	
	if ( m_nCur  >=  m_arrayDoc.GetSize() )
	{
		m_nCur = 0;
	}
	
	//打标参数更新
	m_pMarkMgr->SetLay( m_pDoc->GetLay() );	
	m_pMarkMgr->SetChain( &(m_pDoc->m_chain) );
	m_pMarkMgr->m_pChain3d = m_pDoc->m_b3D ? &(m_pDoc->m_chain3d) : NULL;
	
	return TRUE;
}

void CMarkMulDocDlg::OnButtonSet() 
{
	int nCount = m_listDocName.GetSelCount();
	if ( nCount == 0 )
		return;
	
	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_listDocName.GetSelItems(nCount, aryListBoxSel.GetData());
	
	int nSel = aryListBoxSel.GetAt(0);
	if ( nSel == LB_ERR )
		return;
	
	CHLDoc *pDoc = (CHLDoc *)m_arrayDoc.GetAt(nSel);
	MOTOR motor = pDoc->m_motor;
	
	CSetRunDlg dlg;
	dlg.m_nAxisNo    = motor.nAxisNo;
	dlg.m_nMoveModel = motor.nMoveModel;
	dlg.m_nWorkType  = motor.nMoveType;
	
	if ( g_Motion.GetAxisType(motor.nAxisNo) == 0 )
	{
		dlg.m_dbLineValue = motor.dbMoveValue;
	}
	else
	{
		dlg.m_dbRotateValue = motor.dbMoveValue;
	}

	if ( dlg.DoModal() == IDOK )
	{
		motor.nAxisNo    = dlg.m_nAxisNo;
		motor.nMoveModel = dlg.m_nMoveModel;
		motor.nMoveType  = dlg.m_nWorkType;
		
		if ( g_Motion.GetAxisType(dlg.m_nAxisNo) == 0 )
		{
			motor.dbMoveValue = dlg.m_dbLineValue;
		}
		else
		{
			motor.dbMoveValue = dlg.m_dbRotateValue;
		}
		
		pDoc->m_motor = motor;
	}
}


//
void CMarkMulDocDlg::OnButtonSave()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CHLDoc *pDoc = NULL;
	int count = m_arrayDoc.GetSize();
	for( int i = 0; i<count; i++ )
	{
		pDoc = (CHLDoc *)m_arrayDoc.GetAt(i);
		if ( pDoc->GetPathName().IsEmpty() )
		{
			AfxMessageBox(IDS_MULDOC_NOPATH,MB_ICONSTOP);
			return;
		}
	}
	
	//保存多文档数据
	static TCHAR szFilter[] =  _T( "Muldoc File(*.mul)|*.mul||" );
	CFileDialog dlg( FALSE,_T("mul"),NULL,OFN_HIDEREADONLY,szFilter );
	
	CString str;
	str.LoadString(IDS_MULDOC_SAVE);
	dlg.m_ofn.lpstrTitle = str;
	if ( dlg.DoModal() == IDOK )
	{
		str = dlg.GetPathName();
		BOOL bIsExit = IsFileExist(str);
		if ( bIsExit ) 
		{
			if ( IDNO == AfxMessageBox(IDS_YH_DULNAME, MB_OKCANCEL) ) 
				return;
		}
		
		MULDOCFILE stMulDoc;
		for( int i = 0; i<count; i++ )
		{
			pDoc = (CHLDoc *)m_arrayDoc.GetAt(i);
			CString strPath = pDoc->GetPathName();
			
#ifdef _UNICODE
			char szPath[MAX_PATH] = {0};
			TCHAR* pChar = strPath.GetBuffer(MAX_PATH);
			WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pChar, -1, szPath, MAX_PATH, NULL, NULL);
			strPath.ReleaseBuffer();
			stMulDoc.arName.Add(szPath);
#else
			stMulDoc.arName.Add(strPath);
#endif
			stMulDoc.arOption.Add(pDoc->m_motor);
		}
		stMulDoc.Serialize(str, FALSE);
	}
}



void CMarkMulDocDlg::Run()
{
	//工作台转动
	MOTOR motor = m_pDoc->m_motor;
	if ( motor.nMoveModel == 0  )
		g_Motion.AMove(motor.nAxisNo,motor.dbMoveValue,motor.nMoveType);
	else
		g_Motion.RMove(motor.nAxisNo,motor.dbMoveValue,motor.nMoveType);
}



void CMarkMulDocDlg::UpdateTime()
{
	CString str;
	int day,hour,min,ms;
	double sec;
	
	ms = m_dwSingle%1000;
	m_dwSingle /= 1000;
	
	sec = m_dwSingle%60;
	m_dwSingle /= 60;
	sec += (ms/1000.0);
	
	min = m_dwSingle%60;
	m_dwSingle /= 60;
	
	hour = m_dwSingle%24;
	day = m_dwSingle/24;
	
	str.Format(_T("%d:%2d:%2d:%6.3f"),day,hour,min,sec);
	SetDlgItemText(IDC_SINGLE_TIME,str);
	
	if ( !g_DAT.GetRedPreview() )
		m_nCount++;
	
	SetDlgItemInt(IDC_NUM,m_nCount);
}



void CMarkMulDocDlg::UpdateCount()
{
	CString str;
	int day,hour,min,ms;
	double sec;
	
	ms = m_dwTotal%1000;
	m_dwTotal /= 1000;
	
	sec = m_dwTotal%60;
	m_dwTotal /= 60;
	sec += (ms/1000.0);
	
	min = m_dwTotal%60;
	m_dwTotal /= 60;
	
	hour = m_dwTotal%24;
	day = m_dwTotal/24;
	
	str.Format(_T("%d:%2d:%2d:%6.3f"),day,hour,min,sec);
	SetDlgItemText(IDC_TOTAL_TIME,str);
}


void CMarkMulDocDlg::Mark()
{
	UpdateData(TRUE);
 	m_pMarkMgr->SetMarkType(m_nMarkType);
	
	//得到本次要进行标记的文档
	if ( m_bMark && m_bFirst )
	{
		if ( !GetNext() )
			return;
	}
	
	//界面改变
	m_bMark = !m_bMark;
	m_bMarkStop = TRUE;
	m_bMarkExit = FALSE;
	UpdateView();
	
	if ( m_dwTotalStart == 0 )
		m_dwTotalStart = timeGetTime();
	
	if ( !m_bMark )
	{
		if (m_bFirst)
		{
			//使缺省按钮失效
			m_bFirst = FALSE;
			m_pMarkMgr->StartMark();
		}
		else
		{
			m_pMarkMgr->ReMark();
		}
	}
	else
	{
		m_pMarkMgr->PauseMark();
	}
}

BOOL CMarkMulDocDlg::IsFileExist(CString &strFileName)
{
	CFileFind filefind;
	////////
	BOOL bFind = filefind.FindFile(strFileName);
	filefind.Close();
	
	return bFind;
}


void CMarkMulDocDlg::OnButtonMotorhome() 
{
	// TODO: Add your control notification handler code here
	CDlgMotorMoveWait dlg;

	dlg.SetData(3, 0, 0, 0);
	dlg.DoModal();
}
