// DlgAddFillSet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgAddFillSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddFillSet dialog


CDlgAddFillSet::CDlgAddFillSet(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgAddFillSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddFillSet)
	m_nNum = 1;
	m_dbAddDis = 0;
	m_dbAddAngle = 0;
	m_bSaveCountour = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgAddFillSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddFillSet)
	DDX_Text(pDX, IDC_EDIT_NUM, m_nNum);
	DDX_Text(pDX, IDC_EDIT_AddDis, m_dbAddDis);
	DDX_Text(pDX, IDC_EDIT_AddAngle, m_dbAddAngle);
	DDX_Check(pDX, IDC_CHECK_SAVECONTOUR, m_bSaveCountour);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddFillSet, CDialog)
	//{{AFX_MSG_MAP(CDlgAddFillSet)
	ON_BN_CLICKED(IDC_CHECK_SAVECONTOUR, OnCheckSavecontour)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddFillSet message handlers

void CDlgAddFillSet::OnCheckSavecontour() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
