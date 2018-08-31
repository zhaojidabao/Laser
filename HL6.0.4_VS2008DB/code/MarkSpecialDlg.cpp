// MarkSpecialDlg.cpp : implementation file
#include "stdafx.h"
#include "hl.h"
#include "HLDoc.h"
#include "MarkSpecialDlg.h"
#include "SetRunDlg.h"
#include <mmsystem.h>
#include "resource.h"
#include "SetIoDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMarkSpecialDlg dialog
  
CMarkSpecialDlg::CMarkSpecialDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CMarkSpecialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkSpecialDlg)
	m_bDebug = FALSE;
	m_nSignalType = 1;
	m_nMarkType = 0;
	//}}AFX_DATA_INIT
	m_nCount = -1;
	m_dwTotal = 0;
	m_dwTotalStart = 0;
	m_dwTotalEnd = 0;
	m_dwSingle = 0;
	m_bPrevFrame = FALSE;
}


void CMarkSpecialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkSpecialDlg)
	DDX_Control(pDX, IDC_PROGRESS_MARK, m_progressMark);
	DDX_Control(pDX, IDC_LIST_DOCNAME, m_listDocName);
	DDX_Check(pDX, IDC_CHECK_DEBUG, m_bDebug);
	DDX_Radio(pDX, IDC_RADIO_SIGNALTYPE, m_nSignalType);
	DDX_Radio(pDX, IDC_RADIO_LASER, m_nMarkType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkSpecialDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkSpecialDlg)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_GETIO, OnButtonGetio)
	ON_BN_CLICKED(IDC_CHECK_DEBUG, OnCheckDebug)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MARK_END,OnMarkEnd)
	ON_MESSAGE(WM_MARK_START,OnMarkStart)
	ON_MESSAGE(WM_MARK_PAUSE,OnMarkPause)
	ON_MESSAGE(WM_OUTPUT_MARK_MSG,OnOutPutMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkSpecialDlg message handlers
BOOL CMarkSpecialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//取得各打开文档的指针
	CHLApp *pApp = (CHLApp *)AfxGetApp();

	CMultiDocTemplate *pDocTem = NULL;
	CHLDoc *pDoc = NULL;
	POSITION posTem = pApp->GetFirstDocTemplatePosition();
	while(posTem  !=  NULL)
	{
		pDocTem = (CMultiDocTemplate *)pApp->GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc  !=  NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			if ( !pDoc )
				continue;

			pDoc->PreprocessMark();
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
	if ( !m_pMarkMgr ){
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
	
	g_DAT.m_hMarkWnd = GetSafeHwnd();
	g_DAT.OutStatus(O_ENT_MARK_ENV,O_ENT_MARK_ENV);
	
	SetDlgItemText(IDC_STATIC_IO,g_DAT.GetInputStr(TRUE));
	
	return TRUE;
}


void CMarkSpecialDlg::OnButtonDown() 
{
	int nCount = m_listDocName.GetCount();
	if ( nCount == 0 )
		return;

	int nSel = m_listDocName.GetCurSel();
	m_listDocName.SetSel(nSel);
	m_listDocName.SelItemRange(FALSE,0,nCount);

	if ( nSel == LB_ERR || nSel == (nCount-1) ) 
		return;
	
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



void CMarkSpecialDlg::OnButtonUp() 
{
	int nCount = m_listDocName.GetCount();
	if ( nCount == 0 )
		return;

	int nSel = m_listDocName.GetCurSel();
	m_listDocName.SetSel(nSel);
	m_listDocName.SelItemRange(FALSE,0,nCount);
	if ( nSel == LB_ERR || nSel<1 )
		return;

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



void CMarkSpecialDlg::OnButtonDel() 
{
	int nCount = m_listDocName.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_listDocName.GetSelItems(nCount, aryListBoxSel.GetData());

	for(int i = 0; i < nCount; i++ )
	{
		int nSel = aryListBoxSel.GetAt(i);
		if ( nSel == LB_ERR ) 
			return;

		m_listDocName.DeleteString(nSel-i);
		m_arrayDoc.RemoveAt(nSel-i);
	}

	UpdateData(FALSE);
}

void CMarkSpecialDlg::OnCancel() 
{
	m_pMarkMgr->StopThread();	
	m_pMarkMgr->SetChain(NULL);

	delete m_pMarkMgr;
	m_pMarkMgr = NULL;

	g_DAT.m_hMarkWnd = NULL;

	CDialog::OnCancel();
}

void CMarkSpecialDlg::OnButtonMark() 
{
	//得到本次要进行标记的文档
	if ( m_bMark && m_bFirst )
	{
		if ( !GetNext() )
			return;
	}
	Mark();
}

void CMarkSpecialDlg::OnButtonStop() 
{
	//设置停止事件，打标线程检测到该事件后退出打标
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	m_pMarkMgr->StopMark();
	UpdateView();
}

void CMarkSpecialDlg::UpdateView()
{
	CString str;
	if (m_bMark){
		str.LoadString(IDS_MARK_ON);
		if (m_bFirst)
		{
			GetDlgItem(IDC_RADIO_LASER)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_RED)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_FRAME)->EnableWindow(TRUE);
		}
	}
	else{
		str.LoadString(IDS_MARK_PAUSE);
		if (m_bFirst)
		{
			GetDlgItem(IDC_RADIO_LASER)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_RED)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_FRAME)->EnableWindow(FALSE);
		}
	}
	SetDlgItemText(IDC_BUTTON_MARK,str);
	
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bMarkStop);
	GetDlgItem(IDCANCEL)->EnableWindow(m_bMarkExit);
	
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(m_bMarkExit);	
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(m_bMarkExit);
}

LRESULT CMarkSpecialDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
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

void CMarkSpecialDlg::MarkEnd()
{
	m_pDoc->SetModifiedFlag();
	MarkSEnd();
	m_bFirst = TRUE;
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));
	UpdateView();
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));

	m_dwTotalEnd = timeGetTime();
	m_dwTotal  = m_dwTotalEnd -m_dwTotalStart;
	UpdateCount();
	m_dwTotalStart = 0;
	
	CMainFrame *pMFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if ( pMFrame  !=  NULL )
		pMFrame->SetMarkTime(g_DAT.GetMarkTime());
}

void CMarkSpecialDlg::MarkSEnd()
{
	m_progressMark.SetPos(0);
	m_dwSingle = m_pMarkMgr->GetSingleMarkTime();
	UpdateTime();
}

//
LRESULT CMarkSpecialDlg::OnOutPutMsg(WPARAM wParam,LPARAM lParam)
{
	TCHAR szMsg[255] = { 0 };
	if ( HLERROR == g_DAT.GetErrInfo(wParam, szMsg, 255)) 
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

LRESULT CMarkSpecialDlg::OnMarkStart(WPARAM wParam,LPARAM lParam)
{
	//得到本次要进行标记的文档
	if ( m_bMark && m_bFirst )
	{
		if ( !GetNext() )
		{
			SetEvent(hStopAfterTouch);
			return 1;
		}
	}
	
	Mark();

	return 0;
}

BOOL CMarkSpecialDlg::GetNext()
{
	CString strIO;

	m_pDoc = NULL;
	////////////////
	if ( m_bDebug )
	{
		m_pDoc = (CHLDoc*)m_arrayDoc.GetAt(m_listDocName.GetCurSel());
	}
	else
	{
		if ( m_nSignalType>0 )
		{  //脚踏触发后
			strIO = g_DAT.GetInputStr();
		}
		else
		{	//脚踏触发前
			strIO = g_DAT.GetInputStr();
		}

		//////////////
		CHLDoc *pDoc = NULL;
		for (int i = 0; i<m_listDocName.GetCount(); i++)
		{
			pDoc = (CHLDoc *)m_arrayDoc.GetAt(i);

#ifdef _UNICODE
			char szIO[100] = {0};
			TCHAR* pChar = strIO.GetBuffer(100);
			WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pChar, -1, szIO, 100, NULL, NULL);
			strIO.ReleaseBuffer();
			if ( pDoc->m_motor.strIO.CompareNoCase(szIO) == 0 )
#else
			if ( pDoc->m_motor.strIO.CompareNoCase(strIO) == 0 )
#endif
			{
				m_pDoc = pDoc;
				break;
			}
		}
	}

	if ( m_pDoc == NULL )
	{
		CString	str;
		str.LoadString(IDS_DISMATCH_DOC);
		SetDlgItemText(IDC_STATIC_DOC,str);
		return FALSE;
	}
	SetDlgItemText(IDC_STATIC_DOC,m_pDoc->GetTitle());
	
	//打标参数更新
	m_pMarkMgr->SetLay(m_pDoc->GetLay());
	m_pMarkMgr->SetChain(&(m_pDoc->m_chain));
	m_pMarkMgr->m_pChain3d = m_pDoc->m_b3D?&(m_pDoc->m_chain3d):NULL;

	return TRUE;
}


void CMarkSpecialDlg::OnButtonSet()
{
	int nCount = m_listDocName.GetSelCount();
	if ( nCount<1 )	return;
	
	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_listDocName.GetSelItems(nCount, aryListBoxSel.GetData());
	
	int nSel = aryListBoxSel.GetAt(0);
	if ( nSel == LB_ERR ) return;
	
	CHLDoc *pDoc = (CHLDoc *)m_arrayDoc.GetAt(nSel);
	MOTOR  motor = pDoc->m_motor;
	
	CSetIODlg dlg;
	dlg.m_strIO	 =  motor.strIO;
	if ( dlg.DoModal() == IDOK )
	{
		motor.strIO = dlg.m_strIO;
		for( int i = 0; i<nCount; i++ )
		{
			nSel = aryListBoxSel.GetAt(i);
			if (nSel == LB_ERR) return;
			pDoc = (CHLDoc *)m_arrayDoc.GetAt(nSel);
			pDoc->m_motor = motor;
		}
	}
}




void CMarkSpecialDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CString str;
	CHLDoc *pDoc = NULL;
	int count = m_arrayDoc.GetSize();
	//
	for(int i = 0; i<count; i++)
	{
		pDoc = (CHLDoc *)m_arrayDoc.GetAt(i);
		if (pDoc->GetPathName().IsEmpty())
		{
			AfxMessageBox(IDS_MULDOC_NOPATH,MB_ICONSTOP);
			return;
		}
	}

	//保存多文档数据
	static TCHAR szFilter[] = _T("Muldoc File(*.mul)|*.mul||");
	CFileDialog dlg( FALSE,_T("Mul"), NULL, OFN_HIDEREADONLY, szFilter );
	
	str.LoadString(IDS_MULDOC_SAVE);
	dlg.m_ofn.lpstrTitle = str;
	if ( dlg.DoModal() == IDOK )
	{
		str = dlg.GetPathName();
		
		MULDOCFILE stMulDoc;
		for (int i = 0; i<count; i++)
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


void CMarkSpecialDlg::UpdateTime()
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
	

	if ( 0 == m_nMarkType )	
		m_nCount++;

	SetDlgItemInt(IDC_NUM,m_nCount);
}

void CMarkSpecialDlg::UpdateCount()
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



void CMarkSpecialDlg::Mark() 
{
	UpdateData(TRUE);
	m_pMarkMgr->SetMarkType(m_nMarkType);
	
	//界面改变
	m_bMark = !m_bMark;
	m_bMarkStop = TRUE;
	m_bMarkExit = FALSE;
	UpdateView();

	if (m_dwTotalStart == 0)
		m_dwTotalStart = timeGetTime();
	
	if (!m_bMark)
	{
		if (m_bFirst)
		{
			//使缺省按钮失效
			m_bFirst = FALSE;
			m_pMarkMgr->StartMark();
		}
		else
		{
			KillTimer(10);			
			m_pMarkMgr->ReMark();
		}
	}
	else
	{
		m_pMarkMgr->PauseMark();
	}
}



void CMarkSpecialDlg::OnButtonGetio() 
{
	SetDlgItemText(IDC_STATIC_IO,g_DAT.GetInputStr());
}

void CMarkSpecialDlg::OnCheckDebug() 
{
	UpdateData(TRUE);
}


LRESULT CMarkSpecialDlg::OnMarkPause(WPARAM wParam,LPARAM lParam)
{
	m_bMark = TRUE;

	UpdateView();

	return 0;
}

