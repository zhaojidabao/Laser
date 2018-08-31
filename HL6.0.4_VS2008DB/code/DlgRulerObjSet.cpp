// DlgRulerObjSet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgRulerObjSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRulerObjSet dialog


CDlgRulerObjSet::CDlgRulerObjSet(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgRulerObjSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRulerObjSet)
	m_bViewNumber = FALSE;
	m_nJumpCount = 0;
	m_nDecimalDigits = 0;
	//}}AFX_DATA_INIT
}


void CDlgRulerObjSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRulerObjSet)
	DDX_Check(pDX, IDC_CHECK_VIEW, m_bViewNumber);
	DDX_Text(pDX, IDC_EDIT_JUMPCOUNT, m_nJumpCount);
	DDV_MinMaxInt(pDX, m_nJumpCount, 0, 999999);
	DDX_Text(pDX, IDC_EDIT_NUMBERCOUNT, m_nDecimalDigits);
	DDV_MinMaxInt(pDX, m_nDecimalDigits, 0, 99999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRulerObjSet, CDialog)
	//{{AFX_MSG_MAP(CDlgRulerObjSet)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRulerObjSet message handlers
