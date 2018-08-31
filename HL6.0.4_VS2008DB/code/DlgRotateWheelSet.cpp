// DlgRotateWheelSet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgRotateWheelSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRotateWheelSet dialog


CDlgRotateWheelSet::CDlgRotateWheelSet(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgRotateWheelSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRotateWheelSet)
	m_dbDiameter = 80.0;
	m_dbDivAngle = 1.0;
	m_nFillNum = 3;
	//}}AFX_DATA_INIT
}


void CDlgRotateWheelSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRotateWheelSet)
	DDX_Text(pDX, IDC_EDIT_DIAMETER, m_dbDiameter);
	DDX_Text(pDX, IDC_EDIT_DIVANGLE, m_dbDivAngle);
	DDX_Text(pDX, IDC_EDIT_FILLNO, m_nFillNum);
	DDV_MinMaxInt(pDX, m_nFillNum, 1, 99999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRotateWheelSet, CDialog)
	//{{AFX_MSG_MAP(CDlgRotateWheelSet)
	ON_EN_KILLFOCUS(IDC_EDIT_FILLNO, OnKillfocusEditFillno)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRotateWheelSet message handlers

BOOL CDlgRotateWheelSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dbDiameter = g_SysCfg.m_dbDiameter;
	m_dbDivAngle = g_SysCfg.m_dbDivAngle;
	m_nFillNum   = g_SysCfg.m_nFillNum;
	
	UpdateData(FALSE);
	
	return TRUE;
}


void CDlgRotateWheelSet::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if ( m_nFillNum %2 == 0 )
	{
		m_nFillNum  +=  1;
		UpdateData(FALSE);
	}
	
	g_SysCfg.m_dbDiameter = m_dbDiameter;
	g_SysCfg.m_dbDivAngle = m_dbDivAngle;
	g_SysCfg.m_nFillNum   = m_nFillNum;
	g_sys.WriteSysCfg();
	
	CDialog::OnOK();
}

void CDlgRotateWheelSet::OnKillfocusEditFillno() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if ( m_nFillNum%2 == 0 )
	{
		m_nFillNum  +=  1;
		
		UpdateData(FALSE);
	}
}
