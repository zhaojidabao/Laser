// ArrayposDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ArrayposDlg.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrayPosDlg dialog

CArrayPosDlg::CArrayPosDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CArrayPosDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrayPosDlg)
	m_dbX = 0.0;
	m_dbY = 0.0;
	m_dbR = 0.0;
	//}}AFX_DATA_INIT
}


void CArrayPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrayPosDlg)
	DDX_Text(pDX, IDC_EDIT_X, m_dbX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dbY);
	DDX_Text(pDX, IDC_EDIT_R, m_dbR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrayPosDlg, CDialog)
	//{{AFX_MSG_MAP(CArrayPosDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrayPosDlg message handlers
