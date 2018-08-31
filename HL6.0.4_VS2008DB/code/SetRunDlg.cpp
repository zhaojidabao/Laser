
// SetRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetRunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRunDlg dialog

 
CSetRunDlg::CSetRunDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CSetRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRunDlg)
	m_nAxisNo = 0;
	m_dbLineValue = 0.0;
	m_dbRotateValue = 0.0;
	m_nMoveModel = 0;
	m_nWorkType = 0;
	//}}AFX_DATA_INIT
}


void CSetRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRunDlg)
	DDX_CBIndex(pDX, IDC_COMBO_AxisNo, m_nAxisNo);
	DDX_Text(pDX, IDC_EDIT_VALUE_LINE, m_dbLineValue);
	DDX_Text(pDX, IDC_EDIT_VALUE_ROTATE, m_dbRotateValue);
	DDX_Radio(pDX, IDC_RADIO_MOVEMODEL, m_nMoveModel);
	DDX_Radio(pDX, IDC_RADIO_WORKTYPE, m_nWorkType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRunDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRunDlg)
		ON_CBN_SELCHANGE(IDC_COMBO_AxisNo, OnSelchangeCOMBOAxisNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRunDlg message handlers



void CSetRunDlg::OnSelchangeCOMBOAxisNo()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_VALUE_ROTATE)->EnableWindow( g_Motion.GetAxisType(m_nAxisNo) == 1 && TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark );
	GetDlgItem(IDC_EDIT_VALUE_LINE)->EnableWindow( g_Motion.GetAxisType(m_nAxisNo) == 0 && TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark );
}



void CSetRunDlg::OnOK()
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}



BOOL CSetRunDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT_VALUE_ROTATE)->EnableWindow( g_Motion.GetAxisType(m_nAxisNo) == 1 && TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark );
	GetDlgItem(IDC_EDIT_VALUE_LINE)->EnableWindow( g_Motion.GetAxisType(m_nAxisNo) == 0 && TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark );
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




