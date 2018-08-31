// EllipSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "EllipSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEllipSetDlg dialog


CEllipSetDlg::CEllipSetDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CEllipSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEllipSetDlg)
	m_dbRuler = 0.5;
	//}}AFX_DATA_INIT
}


void CEllipSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEllipSetDlg)
	DDX_Text(pDX, IDC_RULER, m_dbRuler);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEllipSetDlg, CDialog)
	//{{AFX_MSG_MAP(CEllipSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEllipSetDlg message handlers

void CEllipSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	///////////
	if ( m_dbRuler  <=  0 )
	{
		AfxMessageBox( _T("Invalid data input...") );
		return;
	}

	g_SysCfg.m_dbDrawEllipSet = m_dbRuler;
	g_sys.WriteSysCfg();
	
	CDialog::OnOK();
}
