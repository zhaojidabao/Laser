// DlgSplash.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash dialog


CDlgSplash::CDlgSplash(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSplash)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSplash, CDialog)
	//{{AFX_MSG_MAP(CDlgSplash)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash message handlers

BOOL CDlgSplash::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSplash::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}
