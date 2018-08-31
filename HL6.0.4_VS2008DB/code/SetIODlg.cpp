//SetIODlg.cpp : implementation file
#include "stdafx.h"
#include "hl.h"
#include "SetIODlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetIODlg dialog


CSetIODlg::CSetIODlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CSetIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetIODlg)
	m_strIO = _T("");
	//}}AFX_DATA_INIT
}


void CSetIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetIODlg)
	DDX_Text(pDX, IDC_EDIT_IO, m_strIO);
	DDV_MaxChars(pDX, m_strIO, 5);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetIODlg, CDialog)
	//{{AFX_MSG_MAP(CSetIODlg)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	ON_EN_CHANGE(IDC_EDIT_IO, OnChangeEditIo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetIODlg message handlers

void CSetIODlg::OnButtonGet() 
{
	// TODO: Add your control notification handler code here
	m_strIO = g_DAT.GetInputStr();
	UpdateData(FALSE);
}

void CSetIODlg::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_strIO.GetLength() < g_sys.m_nIOSelCount )
	{
		int nSub = g_sys.m_nIOSelCount - m_strIO.GetLength();
		for ( int i = 0; i < nSub; i++ )
			m_strIO.Insert(0,_T("0"));
	}
	else 
		m_strIO = m_strIO.Left(::g_sys.m_nIOSelCount);

	UpdateData(FALSE);

	CDialog::OnOK();
}



void CSetIODlg::OnChangeEditIo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 
	//
	TCHAR ch;
	int  i = 0;
	for(; i<m_strIO.GetLength(); i++)
	{
		ch = m_strIO.GetAt(i);
		if ( ch == _T('0') || ch == _T('1') )
			continue;
		else
			break;
	}
	
	if ( i < m_strIO.GetLength())
	{
		AfxMessageBox(_T("Invalid input, the character only used 0 or 1!") );
	}
}



