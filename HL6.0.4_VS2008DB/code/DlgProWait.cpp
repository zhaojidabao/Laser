// DlgProWait.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgProWait.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProWait dialog

UINT Wheel_Process(LPVOID lpPara)
{	
	CDlgProWait *pDlg = (CDlgProWait *)lpPara;
	HWND hWnd = pDlg->GetSafeHwnd();
	
	while(TRUE)
	{		
		pDlg->Process();
		break;
	}
	PostMessage(hWnd,WM_WHEELPROC_EXIT,0,0);
	
	return 0;
}


CDlgProWait::CDlgProWait(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgProWait::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProWait)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pChain = NULL;
	m_pChainSource = NULL;
	m_nProcessType = 0;
	m_nProState = 0;
	m_bExit = TRUE;
}



void CDlgProWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProWait)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgProWait, CDialog)
	//{{AFX_MSG_MAP(CDlgProWait)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WHEELPROC_EXIT,OnWheelProExit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProWait message handlers
BOOL CDlgProWait::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AfxBeginThread(Wheel_Process,(LPVOID)this,THREAD_PRIORITY_NORMAL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProWait::OnButtonExit()
{
	// TODO: Add your control notification handler code here
	if ( m_bExit == FALSE )
	{
		m_mgrWheelStroke.ProStop(TRUE);
		m_pChain->m_bWheelProStop = TRUE;
	}
}


BOOL CDlgProWait::Process()
{
	m_bExit = FALSE;
	m_pChain->ReStoreSelChain(*m_pChainSource);
	m_nProState = m_pChain->CreateWheelStroke(m_nProcessType,g_SysCfg.m_dbDiameter/2,g_SysCfg.m_dbDivAngle,g_SysCfg.m_nFillNum);
	m_bExit = TRUE;
	
	return TRUE;
}


void CDlgProWait::SetChain(CChain *pChain, CChain *pChainSource)
{
	m_pChain = pChain;
	m_pChainSource = pChainSource;
}


LRESULT CDlgProWait::OnWheelProExit(WPARAM wParam,LPARAM lParam)
{
	Sleep(10);
	CDialog::OnCancel();

	return 0;
}

