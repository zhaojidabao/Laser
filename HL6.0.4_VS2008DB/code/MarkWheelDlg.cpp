// MarkWheelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MarkWheelDlg.h"
#include <mmsystem.h>
#include "DlgProWait.h"
#include "MainFrm.h"
#include "DlgMotorMoveWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkWheelDlg dialog

CMarkWheelDlg::CMarkWheelDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CMarkWheelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkWheelDlg)
	m_bUpdate = FALSE;
	m_nMarkType = 0;
	m_bLoopMark = FALSE;
	m_nIntervalTime = 100;
	m_nProcessType = 0;
	m_bHome = FALSE;
	//}}AFX_DATA_INIT
	m_pMarkMgr = NULL;
	m_pChainWheel = NULL;
	m_nPrevType = -1;
	m_bShowInfoItem = FALSE;
	m_bCreateWheelStroke = FALSE;
}


void CMarkWheelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkWheelDlg)
	DDX_Control(pDX, IDC_LIST_INFOLIST, m_InfoList);
	DDX_Control(pDX, IDC_STATIC_MARKINFO, m_MarkInfo);
	DDX_Control(pDX, IDC_PROGRESS_MARK, m_progressMark);
	DDX_Check(pDX, IDC_CHECK_UPDATE, m_bUpdate);
	DDX_Radio(pDX, IDC_RADIO_LASER, m_nMarkType);
	DDX_Check(pDX, IDC_CHECK_LOOPMARK, m_bLoopMark);
	DDX_Text(pDX, IDC_EDIT_INTERVALTIME, m_nIntervalTime);
	DDV_MinMaxInt(pDX, m_nIntervalTime, 0, 9999999);
	DDX_Radio(pDX, IDC_RADIO_PROCESS, m_nProcessType);
	DDX_Check(pDX, IDC_CHECK_HOME, m_bHome);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_MARK, m_btnStartMark);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStopMark);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_btnView);
	DDX_Control(pDX, IDC_CHECK_RED, m_chkRedEnable);
}



BEGIN_MESSAGE_MAP(CMarkWheelDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkWheelDlg)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_UPDATE, OnCheckUpdate)
	ON_BN_CLICKED(IDC_RADIO_LASER, OnRadioLaser)
	ON_BN_CLICKED(IDC_RADIO_RED, OnRadioRed)
	ON_BN_CLICKED(IDC_CHECK_LOOPMARK, OnCheckLoopmark)
	ON_BN_CLICKED(IDC_CHECK_RED, OnCheckRed)
	ON_BN_CLICKED(IDC_CHECK_HOME, OnCheckHome)
	ON_BN_CLICKED(IDC_RADIO_PROCESS, OnRadioProcess)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_BUTTON_MOTORHOME, OnButtonMotorhome)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MARK_END,OnMarkEnd)
	ON_MESSAGE(WM_MARK_START,OnMarkStart)
	ON_MESSAGE(WM_MARK_STOP,OnMarkStop)
	ON_MESSAGE(WM_OUTPUT_MARK_MSG,OnOutPutMsg)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMarkWheelDlg message handlers
void CMarkWheelDlg::OnButtonMark()
{
	m_MarkInfo.SetWindowText(_T(""));
	Mark();
}

//
void CMarkWheelDlg::OnButtonStop()
{
	if ( WaitForSingleObject(hMarkSts, 0)  !=  WAIT_OBJECT_0 )
		return;
	
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(FALSE);
	
	m_bLoopMark = FALSE;
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
	m_pMarkMgr->StopMark();
	
	for( int i = 0; i<100; i++ )
	{
		if ( WAIT_TIMEOUT == WaitForSingleObject(hMarkSts, 20)  )
			break;
	}
	
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(TRUE);
	UpdateView();
}



//
BOOL CMarkWheelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//控件浮动
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_MARK), IDC_BUTTON_MARK); 
	
	m_pChainWheel = new CChain();
	m_pMarkMgr = new CMarkMgr();
	if ( !m_pChainWheel || !m_pMarkMgr )
	{
		if ( m_pChainWheel )
			delete m_pChainWheel;
		if ( m_pMarkMgr )
			delete m_pMarkMgr;

		CDialog::OnCancel();
		return FALSE;
	}

	m_pMarkMgr->RunThread((LPVOID)GetSafeHwnd());
	m_pMarkMgr->SetLay(m_pDoc->GetLay());	
	m_pMarkMgr->SetChain(m_pChainWheel);
	
	m_progressMark.SetRange32(0,10);
	m_progressMark.SetStep(1);
	
	m_bFirst = TRUE;
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	UpdateView();
	
	//定义计时器
	SetTimer(0,1000,NULL);
	m_timeStart = CTime::GetCurrentTime();
	m_tsWork = 0;
	m_dwSingle = 0;
	m_dwWork = 0;
	m_nCount = 0;
	m_nTotal = 0;
	UpdateTime(TRUE);
	
	m_MarkInfo.SetTextColor(RGB(255,0,0), FALSE);
	m_MarkInfo.SetBold(TRUE, FALSE);
	
	//
	m_btnStartMark.SetFlat(FALSE);
	m_btnStopMark.SetFlat(FALSE);
	m_btnExit.SetFlat(FALSE);
	
	m_btnView.SetIcon(IDI_ICON_WHELLVIEW);
	m_btnView.DrawBorder(FALSE);

	m_chkRedEnable.SetFlat(FALSE);
 	m_chkRedEnable.SetIcon(IDI_ICON_RED, IDI_ICON_FALSE);
	m_chkRedEnable.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	
	//
	g_DAT.m_hMarkWnd = GetSafeHwnd();
	ShowInfoItem();
	
	g_DAT.OutStatus(O_ENT_MARK_ENV,O_ENT_MARK_ENV);
	//
	m_bHome = g_sys.GetProfileInt(_T("WheelSet"),_T("WheelHomeEnable"),0);
	m_nProcessType = g_sys.GetProfileInt(_T("WheelSet"),_T("WheelProcessType"),0);
	UpdateData(FALSE);
	
	return TRUE;
}


//
void CMarkWheelDlg::UpdateView()
{
	CString str1,str2;
	if ( m_bMark )
	{
		str1.LoadString(IDS_MARK_ON);
		m_btnStartMark.SetIcon(IDI_ICON_NONE);
	}
	else
	{
		str1.Empty();
		m_btnStartMark.SetIcon(IDI_ICON_LASER);
	}
	
	if ( m_bMarkStop )
		str2.Empty();
	else
		str2.LoadString(IDS_MARK_STOP);
	
	SetDlgItemText(IDC_BUTTON_MARK,str1);
	SetDlgItemText(IDC_BUTTON_STOP,str2);
	
	GetDlgItem(IDC_CHECK_RED)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bMarkStop);
	GetDlgItem(IDCANCEL)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_LASER)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_RED)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_MOTORHOME)->EnableWindow(m_bMarkExit);
	
	m_btnStopMark.SetIcon(m_bMarkStop ? IDI_ICON_LASERSTOP : IDI_ICON_NONE);
}


//
LRESULT CMarkWheelDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd();
		break;
	case MARK_SEND:
		MarkSEnd();
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}

	return 0;
}

//
void CMarkWheelDlg::OnMarkEndL(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd();
		break;
	case MARK_SEND:
		MarkSEnd();
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}
}


LRESULT CMarkWheelDlg::OnMarkStart(WPARAM wParam,LPARAM lParam)
{
	Mark();
	return 0;
}


LRESULT CMarkWheelDlg::OnMarkStop(WPARAM wParam,LPARAM lParam)
{
	OnButtonStop();
	return 0;
}

void CMarkWheelDlg::UpdateTime(BOOL flag)
{
	m_dwSingle = m_pMarkMgr->GetSingleMarkTime();
	
	m_timeEnd = CTime::GetCurrentTime();
	m_tsTotal  = m_timeEnd-m_timeStart;
	if ( flag )
	{
		m_dwWork  +=  m_dwSingle;
		m_tsWork  = (m_dwWork/1000);
	}
	m_tsIdle = m_tsTotal-m_tsWork;
	
	SetDlgItemInt(IDC_STATIC_COUNT,m_nCount);
	SetDlgItemInt(IDC_STATIC_NUM,m_nTotal);
	SetDlgItemText(IDC_STATIC_TOTAL,m_tsTotal.Format(_T("%D:%H:%M:%S")));
	SetDlgItemText(IDC_STATIC_WORK,m_tsWork.Format(_T("%D:%H:%M:%S")));
	SetDlgItemText(IDC_STATIC_IDLE,m_tsIdle.Format(_T("%D:%H:%M:%S")));

	//单个打标时间显示
	if ( flag )
	{
		CString str;
		int day,hour,min,ms;
		double sec;
		
		ms = m_dwSingle % 1000;
		m_dwSingle /= 1000;

		sec = m_dwSingle%60;
		m_dwSingle /= 60;
		sec  +=  (ms/1000.0);

		min = m_dwSingle%60;
		m_dwSingle /= 60;
		
		hour = m_dwSingle%24;
		day  = m_dwSingle/24;
		
		str.Format(_T("%d:%2d:%2d:%6.3f"),day,hour,min,sec);
		SetDlgItemText(IDC_STATIC_SINGLE,str);
	}
}

void CMarkWheelDlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 0 )
	{
		UpdateTime();
	}
	
	CDialog::OnTimer(nIDEvent);
}


void CMarkWheelDlg::OnCancel()
{
	if ( WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
		return;
	
	if ( NULL == m_pMarkMgr )
		return;
	
	m_pMarkMgr->SetLoopMarkInfo(FALSE, m_nIntervalTime);
	m_pMarkMgr->SetChain(NULL);
	m_pMarkMgr->StopThread();
	
	delete m_pMarkMgr;
	m_pMarkMgr = NULL;
	

	if ( m_pChainWheel )
	{
		delete m_pChainWheel;
		m_pChainWheel = NULL;
	}
	
	//统计打标参数
	DWORD dwMarkNum  = g_sys.GetProfileInt(_T("MARKNUMBER"), _T("MARKSTAT"), 0);
	DWORD dwMarkTime = g_sys.GetProfileInt(_T("MARKTIME"), _T("MARKSTAT"), 0);
	dwMarkNum    +=   m_nCount;
	dwMarkTime   +=  (m_dwWork+500)/1000;
	g_sys.WriteProfileInt(_T("MARKNUMBER"), _T("MARKSTAT"), dwMarkNum);
	g_sys.WriteProfileInt(_T("MARKTIME"), _T("MARKSTAT"), dwMarkTime);
	
	//
	g_sys.WriteProfileInt(_T("WheelSet"),_T("WheelHomeEnable"),m_bHome);
	g_sys.WriteProfileInt(_T("WheelSet"),_T("WheelProcessType"),m_nProcessType);
	g_DAT.m_hMarkWnd = NULL;
	
	CDialog::OnCancel();
}






BOOL CMarkWheelDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( !m_bMarkExit )
	{
		if ( (WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_RETURN) & 0x8000) )
			return TRUE;
	}
	
	if ( ((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		 && (GetKeyState(VK_F2) & 0x8000))
	{
		ShowInfoItem();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}



void CMarkWheelDlg::OnCheckUpdate()
{
	UpdateData(TRUE);
}

//
void CMarkWheelDlg::Mark()
{
	UpdateData(TRUE);
	
	m_pMarkMgr->SetMarkType(m_nMarkType);
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
	
	if ( m_bMark && m_bFirst )
	{
		//处于不打标状态时,即下一个状态是打标状态时
		if ( m_pMarkMgr->MarkIsOver() == TRUE )
			return;
	}
	m_bMark = !m_bMark;
	m_bMarkStop = TRUE;
	m_bMarkExit = FALSE;
	
 	if ( !m_bMark )
	{	
		if ( m_nPrevType  !=  m_nProcessType )//类型变化
			m_bCreateWheelStroke = FALSE;
			
		//检查旋转打标数据是否已准备就绪
		if ( !m_bCreateWheelStroke  )
		{	
			if ( !CreateWheelStroke() )
			{
				m_bMark = !m_bMark;
				return;
			}
			
			m_bCreateWheelStroke = TRUE;
		}
		
		if ( m_bFirst )
		{
			//使缺省按钮失效
			m_bFirst = FALSE;
			m_pMarkMgr->StartMark();
		}
		else
		{
			if ( HLOK  !=  m_pMarkMgr->ReMark() )
			{
				m_bMark = !m_bMark;
				return;
			}
		}
	}
	else
	{
		m_pMarkMgr->PauseMark();
	}
	
	UpdateView();
	GetDlgItem(IDC_BUTTON_MARK)->SetFocus();
}





//
void CMarkWheelDlg::MarkEnd()
{
	MarkSEnd();
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	m_bFirst = TRUE;
	
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));
	if ( m_bUpdate && m_pDoc )
	{
		m_pDoc->UpdateAllViews(NULL);
		m_pDoc->SetTrackerQuad(m_pChain->GetLimit());
	}
	
	if ( !m_pMarkMgr->GetRedPreview() && m_dwSingle > 0 )
		m_nCount++;
	
	UpdateTime(TRUE);
	UpdateView();
	
	GetDlgItem(IDC_BUTTON_MARK)->SetFocus();
	
	//
	m_pChain->PreprocessMark(0x00);
	if ( m_pChain->PreprocessMark(0x06) == TRUE )
	{
		m_bCreateWheelStroke = FALSE;
		m_pMarkMgr->m_bPreProc = m_pChain->PreprocessMark(0x01);
	}
	
	CMainFrame *pMFrame = (CMainFrame*)AfxGetMainWnd();
	if ( pMFrame  !=  NULL )
		pMFrame->SetMarkTime(g_DAT.GetMarkTime());
}



//
void CMarkWheelDlg::MarkSEnd()
{
	m_pDoc->SetModifiedFlag();
	m_progressMark.SetPos(0);
	
	//电机回零
	if (m_bHome == TRUE)
	{
		g_Motion.AMove(0,0,2);
	}
	
	UpdateTime();
	if ( m_dwSingle > 0 )
		m_nTotal = m_nTotal+1;
}


//
void CMarkWheelDlg::OnRadioLaser() 
{
	UpdateData(TRUE);
	UpdateView();
}

//
void CMarkWheelDlg::OnRadioRed()
{
	UpdateData(TRUE);
	UpdateView();
}

//
void CMarkWheelDlg::OnCheckLoopmark() 
{
	UpdateData(TRUE);
	
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
}

void CMarkWheelDlg::ShowInfoItem()
{
	CWnd *pWnd = NULL;
	RECT rect1;
	RECT rect2;
	
	GetWindowRect(&rect1);
	
	if ( m_bShowInfoItem )
	{
		pWnd = GetDlgItem(IDC_STATIC_RECTW);		
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_RECTH);
	}
	
	if ( pWnd == NULL)
		return;
	
	pWnd->GetWindowRect(&rect2);
	rect1.right  = rect2.right;
	rect1.bottom = rect2.bottom;
	MoveWindow(&rect1);
	
	m_bShowInfoItem = !m_bShowInfoItem;
}

//
LRESULT CMarkWheelDlg::OnOutPutMsg(WPARAM wParam,LPARAM lParam)
{
	TCHAR szMsg[255] = {0};
	if ( HLERROR == g_DAT.GetErrInfo(wParam, szMsg, 255)) 
		return 1;
	
	CString strCurMsg;
	m_MarkInfo.GetWindowText(strCurMsg);
	if ( _tcslen(szMsg) <= 0 || 0 == strCurMsg.CompareNoCase(szMsg) )
		return 1;

	if ( (wParam < 10) && (wParam  !=  0) )
		m_MarkInfo.SetTextColor(RGB(255,0,0), TRUE);
	else
		m_MarkInfo.SetTextColor(RGB(0,0,255), TRUE);
	
	m_MarkInfo.SetWindowText(szMsg);
	m_MarkInfo.Invalidate();
	m_InfoList.AddString(szMsg);
	
	if ( m_InfoList.GetCount() > 100 )
		m_InfoList.DeleteString(0);

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


void CMarkWheelDlg::OnCheckRed()
{
	// TODO: Add your control notification handler code here
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_RED))->GetCheck();
	if ( bRet )
	{
		g_DAT.OutStatus(O_RED, O_RED);
	}
	else
	{
		g_DAT.OutStatus(O_RED, 0);
	}
}

//
BOOL CMarkWheelDlg::CreateWheelStroke()
{
	if ( m_pChain == NULL )
		return FALSE;
	
	m_pChainWheel->DeleteAll();
	m_pChainWheel->m_bWheelProStop = FALSE;
	//
	CDlgProWait dlg;
	dlg.m_nProcessType = m_nProcessType;
	dlg.SetChain(m_pChainWheel, m_pChain);
	dlg.DoModal();
	
	if ( dlg.m_nProState == 1 )
	{
		m_pChainWheel->DeleteAll();
		return FALSE;
	}
	else if ( dlg.m_nProState == 2 )
	{
		CString strMsg;
		strMsg.LoadString(IDS_ERROR_SEGMENT);
		MessageBox(strMsg,_T("Tip"),MB_OK|MB_ICONQUESTION);
	}
	//
	m_nPrevType = m_nProcessType;//记录当前处理的类型	
	m_pMarkMgr->SetChain(m_pChainWheel);
	if ( m_nProcessType == 0 )
		m_progressMark.SetRange32(0,m_pMarkMgr->m_dwMarkCount);
	else
		m_progressMark.SetRange32(0,19);
	
	return TRUE;
}

void CMarkWheelDlg::OnCheckHome()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CMarkWheelDlg::OnRadioProcess()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CMarkWheelDlg::OnRadio2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CMarkWheelDlg::OnButtonView()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_MARK)->SetFocus();
}

void CMarkWheelDlg::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CMarkWheelDlg::OnButtonMotorhome() 
{
	// TODO: Add your control notification handler code here
//	g_Motion.GoSoftHome();

	CDlgMotorMoveWait dlg;
	dlg.SetData(3, 0, 0, 0);
	dlg.DoModal();
}
