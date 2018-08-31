// DcpMsg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DcpMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDcpMsg dialog

CDcpMsg::CDcpMsg(CWnd* pParent  /*= NULL*/)
	: CDialog(CDcpMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDcpMsg)
	m_strDcpMsg = _T("");
	m_bRedOpen = FALSE;
	//}}AFX_DATA_INIT
}


void CDcpMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDcpMsg)
	DDX_Text(pDX, IDC_STATIC_DCPMSGDISP, m_strDcpMsg);
	DDX_Check(pDX, IDC_CHECK_RED, m_bRedOpen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDcpMsg, CDialog)
	//{{AFX_MSG_MAP(CDcpMsg)
	ON_BN_CLICKED(IDC_CHECK_LASERSTATUS, OnCheckLaserstatus)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_CHECK_RED, OnCheckRed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDcpMsg message handlers

void CDcpMsg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDcpMsg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CDcpMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UINT uintOut = 0x0000;
	g_DAT.GetOutput(&uintOut);
	m_bRedOpen = (uintOut & O_RED) ? TRUE : FALSE;
	UpdateData(FALSE);

	SetLaserstatus(m_bLaserSatus);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDcpMsg::SetLaserstatus(BOOL bDcpStatus) 
{
	if (bDcpStatus)
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ERRORSTATUS))->SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_DCPON)));
	}
	else
	{
		((CButton*)GetDlgItem(IDC_BUTTON_ERRORSTATUS))->SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_DCPOFF)));
	}
}


void CDcpMsg::SetDispMsg(CString &strDcpMsg)
{
	m_strDcpMsg = strDcpMsg;
	GetDlgItem(IDC_STATIC_DCPMSGDISP)->SetWindowText(m_strDcpMsg);
}



void CDcpMsg::OnCheckLaserstatus() 
{
	UINT nMsk  = -1;
	UINT nSts  = -1;
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_LASERSTATUS))->GetCheck();
	if (bRet) 
	{
		nMsk   = 1 << 11;
		nSts   = 1 << 11;
	}
	else
	{
		nMsk   = 1 << 11;
		nSts   = 0 << 11;
	}

//	g_DAT.OutStatus(nMsk, nSts);
}

void CDcpMsg::OnCheckRed() 
{
	UpdateData(TRUE);

	if (m_bRedOpen) 
	{
		g_DAT.OutStatus(O_RED, O_RED);
	}
	else
	{
		g_DAT.OutStatus(O_RED, 0);
	}

}

