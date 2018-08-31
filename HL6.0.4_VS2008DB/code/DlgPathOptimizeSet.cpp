// DlgPathOptimizeSet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgPathOptimizeSet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPathOptimizeSet dialog


CDlgPathOptimizeSet::CDlgPathOptimizeSet(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgPathOptimizeSet::IDD, pParent)
{	
	//{{AFX_DATA_INIT(CDlgPathOptimizeSet)
	m_nOptimizeType = 0;
	//}}AFX_DATA_INIT
}


void CDlgPathOptimizeSet::DoDataExchange(CDataExchange* pDX)
{	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPathOptimizeSet)
	DDX_Control(pDX, IDC_STATIC_WARN, m_staticWarn);
	DDX_Radio(pDX, IDC_RADIO1, m_nOptimizeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPathOptimizeSet, CDialog)
	//{{AFX_MSG_MAP(CDlgPathOptimizeSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPathOptimizeSet message handlers


BOOL CDlgPathOptimizeSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_staticWarn.SetTextColor(RGB(255,0,0));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
