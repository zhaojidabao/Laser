// DlgHelpPath.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgHelpPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHelpPath dialog


CDlgHelpPath::CDlgHelpPath(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgHelpPath::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHelpPath)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHelpPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHelpPath)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHelpPath, CDialog)
	//{{AFX_MSG_MAP(CDlgHelpPath)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHelpPath message handlers
