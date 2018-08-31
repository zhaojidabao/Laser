// FastMarkStopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "FastMarkStopDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern  CHLApp	theApp;
/////////////////////////////////////////////////////////////////////////////
// CFastMarkStopDlg dialog

CFastMarkStopDlg::CFastMarkStopDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CFastMarkStopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastMarkStopDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsDownLoad = FALSE;
}


void CFastMarkStopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastMarkStopDlg)
	DDX_Control(pDX, IDC_PROGRESS_DWONLOAD, m_progressMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastMarkStopDlg, CDialog)
	//{{AFX_MSG_MAP(CFastMarkStopDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FASTMARK_END, OnFastMarkEnd)
	ON_MESSAGE(WM_FASTMARK_PRO,OnMarkPro)
	ON_BN_CLICKED(IDOK, &CFastMarkStopDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastMarkStopDlg message handlers

BOOL CFastMarkStopDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_progressMark.SetRange32(0,10);
	m_progressMark.SetStep(1);
	
	// TODO: Add extra initialization here
	OnFastmark();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFastMarkStopDlg::OnOK() 
{
	theApp.m_FastMark.StopFastMark();
}

//
void CFastMarkStopDlg::OnFastmark()
{
	if ( g_bIsSelMark )
	{
		theApp.m_SelMark.StopSelMark();
		Sleep(50);
	}
	
	if ( g_bIsRedPreview ) 
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
		Sleep(100);
	}
	
	if ( g_bIsFastMark ) 
	{
		theApp.m_FastMark.StopFastMark();
	}
	else
	{
		g_bIsFastMark = TRUE;
		theApp.m_FastMark.StartFastMark(this->m_hWnd, m_bIsDownLoad);
	}
}




//
LRESULT CFastMarkStopDlg::OnFastMarkEnd( WPARAM wParam,LPARAM lParam )
{
	double dbSingleTime = g_DAT.GetMarkTime();
	CMainFrame *pMFrame = (CMainFrame*)theApp.m_pMainWnd;
	if ( pMFrame  !=  NULL )
		pMFrame->SetMarkTime(dbSingleTime);
	
	g_bIsFastMark = FALSE;
	CDialog::OnOK();
	
	if ( m_bIsDownLoad && wParam >0 ) 
	{
		CString strInfo, str;
		str.LoadString(IDS_DOWNNORMALOK);
		strInfo.Format( str, (UINT)wParam );
		AfxMessageBox(strInfo);
	}
	
	return 0;
}





//
void CFastMarkStopDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	return ;
}





LRESULT CFastMarkStopDlg::OnMarkPro(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}

	return 0;
}

void CFastMarkStopDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
