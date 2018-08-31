
// DataMatrixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DataMatrixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMatrixDlg dialog
CDataMatrixDlg::CDataMatrixDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CDataMatrixDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataMatrixDlg)
	//}}AFX_DATA_INIT
	m_nErrLev = 5;
	m_nShape = 0;
	m_nSymbol = 0;
}


void CDataMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMatrixDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_ERR, m_comboErr);
	DDX_Control(pDX, IDC_COMBO_SYMBOL, m_comboSymbol);
	DDX_Control(pDX, IDC_COMBO_SHAPE, m_comboShape);
}

BEGIN_MESSAGE_MAP(CDataMatrixDlg, CDialog)
	//{{AFX_MSG_MAP(CDataMatrixDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDataMatrixDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMatrixDlg message handlers


BOOL CDataMatrixDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_comboErr.AddString(_T("ECC200"));
	m_comboErr.AddString(_T("ECC140"));
	m_comboErr.AddString(_T("ECC100"));
	m_comboErr.AddString(_T("ECC080"));
	m_comboErr.AddString(_T("ECC050"));
	m_comboErr.AddString(_T("ECC000"));
	m_comboErr.SetCurSel(0);
	/////////////
	m_comboSymbol.AddString(_T("Auto"));
	m_comboSymbol.AddString(_T("ASCII"));
	m_comboSymbol.AddString(_T("C40"));
	m_comboSymbol.AddString(_T("TEXT"));
	m_comboSymbol.AddString(_T("X12"));
	m_comboSymbol.AddString(_T("EDIFACT"));
	m_comboSymbol.AddString(_T("BASE256"));
	m_comboSymbol.SetCurSel(m_nSymbol);
	/////////////////
	m_comboShape.AddString(_T("Auto Shape"));
	m_comboShape.AddString(_T("10x10"));
	m_comboShape.AddString(_T("12x12"));
	m_comboShape.AddString(_T("14x14"));
	m_comboShape.AddString(_T("16x16"));
	m_comboShape.AddString(_T("18x18"));
	m_comboShape.AddString(_T("20x20"));
	m_comboShape.AddString(_T("22x22"));
	m_comboShape.AddString(_T("24x24"));
	m_comboShape.AddString(_T("26x26"));
	m_comboShape.AddString(_T("32x32"));
	m_comboShape.AddString(_T("36x36"));
	m_comboShape.AddString(_T("40x40"));
	m_comboShape.AddString(_T("44x44"));
	m_comboShape.AddString(_T("48x48"));
	m_comboShape.AddString(_T("52x52"));
	m_comboShape.AddString(_T("64x64"));
	m_comboShape.AddString(_T("72x72"));
	m_comboShape.AddString(_T("80x80"));
	m_comboShape.AddString(_T("88x88"));
	m_comboShape.AddString(_T("96x96"));
	m_comboShape.AddString(_T("104x104"));
	m_comboShape.AddString(_T("120x120"));
	m_comboShape.AddString(_T("132x132"));
	m_comboShape.AddString(_T("144x144"));
	m_comboShape.AddString(_T(" 8x18 "));
	m_comboShape.AddString(_T(" 8x32 "));
	m_comboShape.AddString(_T("12x26"));
	m_comboShape.AddString(_T("12x36"));
	m_comboShape.AddString(_T("16x36"));
	m_comboShape.AddString(_T("16x48"));
	m_comboShape.SetCurSel(m_nShape);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDataMatrixDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_nErrLev = m_comboErr.GetCurSel();
	m_nSymbol =  m_comboSymbol.GetCurSel();
	m_nShape = m_comboShape.GetCurSel();

	OnOK();
}
