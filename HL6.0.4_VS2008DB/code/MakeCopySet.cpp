// MakeCopySet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MakeCopySet.h"
#include "HLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeCopySet dialog


CMakeCopySet::CMakeCopySet(CWnd* pParent  /*= NULL*/)
	: CDialog(CMakeCopySet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeCopySet)
	m_nScanCount = 1;
	//}}AFX_DATA_INIT
}


void CMakeCopySet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeCopySet)
	DDX_Text(pDX, IDC_EDIT_SCAN_COUNT, m_nScanCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMakeCopySet, CDialog)
	//{{AFX_MSG_MAP(CMakeCopySet)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMakeCopySet::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeCopySet message handlers

void CMakeCopySet::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CMakeCopySet::OnCancel() 
{
	CDialog::OnCancel();
}

BOOL CMakeCopySet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMakeCopySet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
