// MarkNormalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MarkNormalDlg.h"
#include <mmsystem.h>
#include "MainFrm.h"
#include "DlgMotorMoveWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkNormalDlg dialog

CMarkNormalDlg::CMarkNormalDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CMarkNormalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkNormalDlg)
	m_bUpdate = FALSE;
	m_nMarkType = 0;
	m_bLoopMark = FALSE;
	m_nIntervalTime = 100;
	m_bRedMark = FALSE;
	//}}AFX_DATA_INIT
	m_pMarkMgr = NULL;
	m_bShowInfoItem = FALSE;
}

void CMarkNormalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkNormalDlg)
	DDX_Control(pDX, IDC_LIST_INFOLIST, m_InfoList);
	DDX_Control(pDX, IDC_STATIC_MARKINFO, m_MarkInfo);
	DDX_Control(pDX, IDC_PROGRESS_MARK, m_progressMark);
	DDX_Check(pDX, IDC_CHECK_UPDATE, m_bUpdate);
	DDX_Radio(pDX, IDC_RADIO_LASER, m_nMarkType);
	DDX_Check(pDX, IDC_CHECK_LOOPMARK, m_bLoopMark);
	DDX_Text(pDX, IDC_EDIT_INTERVALTIME, m_nIntervalTime);
	DDV_MinMaxInt(pDX, m_nIntervalTime, 0, 999999);
	DDX_Check(pDX, IDC_CHECK_RED, m_bRedMark);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_BUTTON_MARK, m_btnStartMark);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStopMark);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_CHECK_RED, m_chkRedEnable);
}

BEGIN_MESSAGE_MAP(CMarkNormalDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkNormalDlg)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_UPDATE, OnCheckUpdate)
	ON_BN_CLICKED(IDC_RADIO_LASER, OnRadioLaser)
	ON_BN_CLICKED(IDC_RADIO_RED, OnRadioRed)
	ON_BN_CLICKED(IDC_RADIO_FRAME, OnRadioFrame)
	ON_BN_CLICKED(IDC_CHECK_LOOPMARK, OnCheckLoopmark)
	ON_BN_CLICKED(IDC_CHECK_RED, OnCheckRed)
	ON_BN_CLICKED(IDC_BUTTON_MOTORHOME, OnButtonMotorhome)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MARK_END,OnMarkEnd)
	ON_MESSAGE(WM_MARK_START,OnMarkStart)
	ON_MESSAGE(WM_MARK_STOP,OnMarkStop)
	ON_MESSAGE(WM_OUTPUT_MARK_MSG,OnOutPutMsg)
	ON_BN_CLICKED(IDCANCEL, &CMarkNormalDlg::OnBnClickedCancel)

	ON_MESSAGE(WM_SOCKETS,OnWmSockets)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkNormalDlg message handlers

void CMarkNormalDlg::OnButtonMark() 
{
 	m_MarkInfo.SetWindowText(_T(""));
 	Mark();
}

//
void CMarkNormalDlg::OnButtonStop() 
{
	if ( WaitForSingleObject(hMarkSts, 0)  !=  WAIT_OBJECT_0 )
		return;
	
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(FALSE);
	
	m_bLoopMark = FALSE;
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
	m_pMarkMgr->StopMark();
	
	for( int i = 0; i<100; i++ )
	{
		if ( WAIT_TIMEOUT == WaitForSingleObject(hMarkSts, 20) )
			break;
	}
	
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(TRUE);
	UpdateView();
}

//
BOOL CMarkNormalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//控件浮动
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_MARK), IDC_BUTTON_MARK); 
	
	m_pMarkMgr = new CMarkMgr();
	if ( !m_pMarkMgr )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	m_pMarkMgr->RunThread((LPVOID)GetSafeHwnd());
	m_pMarkMgr->SetLay(m_pDoc->GetLay());	
	m_pMarkMgr->SetChain(m_pChain);
	m_pMarkMgr->m_pChain3d = m_pChain3d;
	
	m_progressMark.SetRange32(0,10);
	m_progressMark.SetStep(1);
	
	m_bFirst = TRUE;
	m_bMark = m_bMarkExit = TRUE;
	m_bMarkStop = FALSE;
	UpdateView();
	
	//定义计时器
	if ( !SetTimer(0,1000,NULL) )
	{
		AfxMessageBox(IDS_ERROR_TIMER);
	}

	m_tsWork = 0;
	m_dwSingle = 0;
	m_dwWork = 0;
	m_nCount = 0;
	m_nTotal = 0;	
	m_timeStart = CTime::GetCurrentTime();
	UpdateTime(TRUE);
	
	m_MarkInfo.SetTextColor(RGB(255,0,0), FALSE);
	m_MarkInfo.SetBold(TRUE, FALSE);
	//
	m_btnStartMark.SetFlat(FALSE);
	m_btnStopMark.SetFlat(FALSE);
	m_btnExit.SetFlat(FALSE);
	
	m_chkRedEnable.SetFlat(FALSE);
	m_chkRedEnable.SetIcon(IDI_ICON_RED, IDI_ICON_FALSE);
	m_chkRedEnable.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	
	//
	g_DAT.m_hMarkWnd = GetSafeHwnd();
	ShowInfoItem();
	
	//
	UINT uintOut;
	g_DAT.GetOutput(&uintOut);
	m_bRedMark = (uintOut & O_RED) ? TRUE : FALSE;
	UpdateData(FALSE);
	
	if ( !g_DAT.m_btExtendInfo.FunData.bMotorMark )
		GetDlgItem(IDC_BUTTON_MOTORHOME)->ShowWindow(FALSE);
	
	g_DAT.OutStatus(O_ENT_MARK_ENV,O_ENT_MARK_ENV);

	//////////////////////////////////////////////////////////////////////////
	StartNet();

	return TRUE;
}

//
void CMarkNormalDlg::UpdateView()
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
	GetDlgItem(IDC_RADIO_FRAME)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_MOTORHOME)->EnableWindow(m_bMarkExit);
	
	m_btnStopMark.SetIcon( m_bMarkStop ? IDI_ICON_LASERSTOP : IDI_ICON_NONE );
}


//
LRESULT CMarkNormalDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd(lParam);
		break;
	case MARK_SEND:
		MarkSEnd(lParam);
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}
	
	return 0;
}


//
void CMarkNormalDlg::OnMarkEndL(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd(lParam);
		break;
	case MARK_SEND:
		MarkSEnd(lParam);
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}
}


LRESULT CMarkNormalDlg::OnMarkStart(WPARAM wParam,LPARAM lParam)
{
	Mark();
	return 0;
}


LRESULT CMarkNormalDlg::OnMarkStop(WPARAM wParam,LPARAM lParam)
{
	OnButtonStop();
	return 0;
}


void CMarkNormalDlg::UpdateTime(BOOL flag)
{
	m_dwSingle = m_pMarkMgr->GetSingleMarkTime();

	m_timeEnd = CTime::GetCurrentTime();
	m_tsTotal = m_timeEnd-m_timeStart;
	if ( flag )
	{
		m_dwWork += m_dwSingle;
		m_tsWork = (m_dwWork/1000);
	}
	m_tsIdle = m_tsTotal-m_tsWork;
	
	SetDlgItemInt(IDC_STATIC_COUNT,m_nCount);
	SetDlgItemInt(IDC_STATIC_NUM,m_nTotal);
	SetDlgItemText(IDC_STATIC_TOTAL,m_tsTotal.Format("%D:%H:%M:%S"));
	SetDlgItemText(IDC_STATIC_WORK,m_tsWork.Format("%D:%H:%M:%S"));
	SetDlgItemText(IDC_STATIC_IDLE,m_tsIdle.Format("%D:%H:%M:%S"));
	
	//单个打标时间显示
	if ( flag )
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
		SetDlgItemText(IDC_STATIC_SINGLE,str);
	}
}

void CMarkNormalDlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 0 )
	{
		UpdateTime();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMarkNormalDlg::OnCancel() 
{
	if ( WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
		return;
	
	g_DAT.m_hMarkWnd = NULL;

	//统计打标参数
	DWORD dwMarkNum  = ::g_sys.GetProfileInt(_T("MARKNUMBER"), _T("MARKSTAT"), 0);
	DWORD dwMarkTime = ::g_sys.GetProfileInt(_T("MARKTIME"), _T("MARKSTAT"), 0);
	dwMarkNum   +=   m_nCount;
	dwMarkTime  +=  (m_dwWork+500)/1000;

	::g_sys.WriteProfileInt(_T("MARKNUMBER"), _T("MARKSTAT"), dwMarkNum);
	::g_sys.WriteProfileInt(_T("MARKTIME"), _T("MARKSTAT"), dwMarkTime);
	
	if ( NULL == m_pMarkMgr )
		return;
	
	m_pMarkMgr->SetLoopMarkInfo(FALSE, m_nIntervalTime);
	m_pMarkMgr->SetChain(NULL);
	m_pMarkMgr->StopThread();

	delete m_pMarkMgr;
	m_pMarkMgr = NULL;		
	
	CDialog::OnCancel();
}

BOOL CMarkNormalDlg::PreTranslateMessage(MSG* pMsg) 
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


void CMarkNormalDlg::OnCheckUpdate() 
{
	UpdateData(TRUE);
}


//
void CMarkNormalDlg::Mark()
{
	UpdateData(TRUE);
	
	m_pMarkMgr->SetMarkType(m_nMarkType);
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
	
	if ( m_bMark && m_bFirst )
	{
		//处于不打标状态时,即下一个状态是打标状态时
		if ( m_pMarkMgr->MarkIsOver() )
			return;
	}
	m_bMark = !m_bMark;
	m_bMarkStop = TRUE;
	m_bMarkExit = FALSE;
	
 	if ( !m_bMark )
	{
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
void CMarkNormalDlg::MarkEnd(int  nSts)
{
	MarkSEnd(nSts);

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
	if ( g_SysCfg.m_bCloseRedIfMark )
	{
		UINT uintOut;
		g_DAT.GetOutput(&uintOut);
		m_bRedMark = (uintOut & O_RED) ? TRUE : FALSE;
		UpdateData(FALSE);
	}
	
	CMainFrame *pMFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if ( pMFrame  !=  NULL )
		pMFrame->SetMarkTime( g_DAT.GetMarkTime() );
}


//
void CMarkNormalDlg::MarkSEnd(int nSts)
{
	m_pDoc->SetModifiedFlag();
	m_progressMark.SetPos(0);
	
	UpdateTime();
	
	if ( m_dwSingle > 0 )
		m_nTotal = m_nTotal+1;
}

//
void CMarkNormalDlg::OnRadioLaser()
{
	UpdateData(TRUE);
	UpdateView();
}

//
void CMarkNormalDlg::OnRadioRed()
{
	UpdateData(TRUE);
	UpdateView();
}

//
void CMarkNormalDlg::OnRadioFrame() 
{
	UpdateData(TRUE);
	UpdateView();
}

void CMarkNormalDlg::OnCheckLoopmark()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
}

void CMarkNormalDlg::ShowInfoItem()
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
	
	if ( pWnd == NULL )
		return;
	
	pWnd->GetWindowRect(&rect2);
	rect1.right  = rect2.right;
	rect1.bottom = rect2.bottom;
	MoveWindow(&rect1);
	
	m_bShowInfoItem = !m_bShowInfoItem;
}


//
LRESULT CMarkNormalDlg::OnOutPutMsg( WPARAM wParam, LPARAM lParam )
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
	UINT nValue   = (UINT) (lParam  & 0xFFFF);
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



void CMarkNormalDlg::OnCheckRed()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if ( m_bRedMark )
	{
		g_DAT.OutStatus(O_RED, O_RED);
	}
	else
	{
		g_DAT.OutStatus(O_RED, 0);
	}
}


void CMarkNormalDlg::OnButtonMotorhome()
{
	// TODO: Add your control notification handler code here
	CDlgMotorMoveWait dlg;
	dlg.SetData(3, 0, 0, 0);
	dlg.DoModal();
}



void CMarkNormalDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

LRESULT CMarkNormalDlg::OnWmSockets(WPARAM wpara,LPARAM lpara)
{
	CString StrRec;
	CString strIp;
	if(HIWORD(wpara)==m_nMainID)
	{
		switch( LOWORD(wpara) )
		{
		case IDWM_SOCKET_ACCEPT:
			m_nListenID=LOWORD(lpara);
			DWORD dwMainIP;
			DWORD dwListenIP;
			m_sock.GetAssociatsSocketIP(m_nMainID,m_nListenID,&dwMainIP,&dwListenIP);
			break;
		case IDWM_SOCKET_RECEIVE:
			OnNetRec( );
			break;
		case IDWM_SOCKET_CLOSE:
			/*if(!m_bServer)*/ m_sock.CloseSock(m_nMainID,m_nListenID);
			break;
		}
	}
	return 0;
}

void CMarkNormalDlg::OnNetRec()
{
	char buff[8193];
	::ZeroMemory((void*) buff,sizeof(char)*8192);	
	if(m_bServer)
		m_sock.ReadData(buff,8192,m_nMainID,0,m_nListenID);
	else
		m_sock.ReadData(buff,8192,m_nMainID);

	CString strRev=buff;

	////-----检测数据包是否符合规则----////
	CString strFlagB,strFlagE;
	strFlagB.Format(_T("%c"),0x02);
	strFlagE.Format(_T("%c"),0x03);
	int nFlagB= strRev.Find(strFlagB);
	int nFlagE= strRev.Find(strFlagE);
	int nLen= strRev.GetLength();
	if (nFlagB>=0 && nFlagE>0)
	{
		OutputDebugString(_T("数据格式符合规则！"));
		strRev= strRev.Mid(nFlagB+1, nFlagE-nFlagB-1);
		nLen  = strRev.GetLength();
	} 
	else
	{
		OutputDebugString(_T("数据格式不正确！\n"));
		::AfxMessageBox(_T("数据格式不正确"));
		return;
	}
	///////////////////////////////////////////////
	CString strErr;
	CString strFeedback;
	int nIdx = strRev.Find(_T("$"),0);
	if (nIdx<0)
	{
		strErr = _T("数据格式不正确！");
		::AfxMessageBox(strErr);
		strFeedback = _T("Receive_Error");
		//SendSocketMsg(strFeedback);
		return;
	}
	int nPos  = strRev.Find('_');
	if (nPos<0)  
	{
		strErr = _T("数据格式不正确！");
		::AfxMessageBox(strErr);
		strFeedback = _T("Receive_Error");
		//SendSocketMsg(strFeedback);
		return;
	}
	//

	CString strCmd = strRev.Mid(nIdx+1, nPos-nIdx-1);
	CString strPara = strRev.Mid(nPos+1);
	if (0 == strCmd.CompareNoCase(_T("Initialize")))   //初始化指令
	{
		CString strHsFile = _T("D:\\HSFILE\\") + strPara + _T(".hs");
		if (!PathFileExists(strHsFile))
		{
			strErr = _T("模板不存在！");
			::AfxMessageBox(strErr,MB_TOPMOST|MB_OKCANCEL);
			strFeedback = _T("Initialize_NG");	   //模板不存在
			SendSocketMsg(strFeedback);
			return;
		}

		CHLApp* pHLApp = (CHLApp*) AfxGetApp();
		CHLDoc* pDoc;
		pDoc = (CHLDoc*) pHLApp->OpenDocumentFile(strHsFile);
		if (NULL == pDoc)
		{
			CString strErr = _T("打开模板失败！");
			AfxMessageBox(strErr, MB_TOPMOST|MB_OKCANCEL);
			strFeedback = _T("Initialize_NG");	   //打开模板失败
			SendSocketMsg(strFeedback);
			return;
		} 
		else
		{
			strFeedback = _T("Initialize_OK");
			SendSocketMsg(strFeedback);
			m_pDoc = pDoc;
			m_pChain = &(m_pDoc->m_chain);
			m_pMarkMgr->SetLay(m_pDoc->GetLay());
			m_pMarkMgr->SetChain(m_pChain);
		}
	}
	else if(0 == strCmd.CompareNoCase(_T("Data")))  //数据指令
	{
		CString strTmp;
		int i=0;
		while (AfxExtractSubString(strTmp,(LPCTSTR)strPara,i,_T(',')))
		{
			CString strModelName;
			strModelName.Format(_T("%d"), i);
			if (!m_pChain->TextReplace(strTmp, strModelName))
			{
				strFeedback =  _T("Receive_NG");
				SendSocketMsg(strFeedback);
				strErr.Format(_T("替换变量[%s]失败"),strModelName);
				::AfxMessageBox(strErr);
				return;
			}
			i++;
		}
		strFeedback = _T("Receive_OK");
		SendSocketMsg(strFeedback);
		m_pDoc->UpdateAllViews(NULL);
	}
	else if (0 == strCmd.CompareNoCase(_T("MarkStart"))) //打标指令
	{
		::SendMessage(GetSafeHwnd(),WM_MARK_START,0,0);
		strFeedback = _T("MarkStart_OK");
		SendSocketMsg(strFeedback);
	}
	else
	{
		strFeedback = _T("MarkStart_NG");
		SendSocketMsg(strFeedback);
	}
	return;
}

void CMarkNormalDlg::SendSocketMsg(CString strMsg) 
{
	char szData[1024]={0};
	WideCharToMultiByte(CP_ACP,0,strMsg,-1,szData,1024,NULL,NULL);

	m_sock.SendData(szData,strlen(szData),m_nMainID,NULL,m_nListenID);

}

void CMarkNormalDlg::StartNet()
{
	m_strIP		=g_sys.GetIniString(_T("ComInfo"),_T("IP"),_T("127.0.0.1"));
	m_nAddr		=g_sys.GetProfileInt(_T("ComInfo"),_T("Address"),9001);
	m_bServer  	=g_sys.GetProfileInt(_T("ComInfo"),_T("Server"),0);
	m_bSameIP	=g_sys.GetProfileInt(_T("ComInfo"),_T("SameIP"),0);

	char szIP[20]={0};
	WideCharToMultiByte(CP_ACP,0,m_strIP,-1,szIP,20,NULL,NULL);
	BOOL nInit = m_sock.InitSock(GetSafeHwnd(),m_bServer,m_nMainID,szIP,m_nAddr,m_bSameIP);
}