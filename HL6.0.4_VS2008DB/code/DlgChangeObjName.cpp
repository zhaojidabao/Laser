// DlgChangeObjName.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgChangeObjName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChangeObjName dialog


CDlgChangeObjName::CDlgChangeObjName(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgChangeObjName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChangeObjName)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgChangeObjName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChangeObjName)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChangeObjName, CDialog)
	//{{AFX_MSG_MAP(CDlgChangeObjName)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//CDlgChangeObjName message handlers
