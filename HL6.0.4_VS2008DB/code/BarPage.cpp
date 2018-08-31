// BarPage.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "BarPage.h"
#include "LangCodePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CBarPage property page

IMPLEMENT_DYNCREATE(CBarPage, CPropertyPage)

CBarPage::CBarPage() : CPropertyPage(CBarPage::IDD)
, m_bParity(FALSE)
, m_dbDotRadius(0.05)
, m_bHollow(FALSE)
, m_dbHollowWidth(0)
, m_dbHollowHeight(0)
, m_bLangEnable(FALSE)
{
	//{{AFX_DATA_INIT(CBarPage)
	m_bRev = FALSE;
	m_bGoBack  = FALSE;
	m_bConnect = FALSE;
	m_dbHeight = 0.0;
	m_dbNarrow = 0.0;
	m_dbShrink = 0.0;
	m_dbWidth = 0.0;
	m_dbSpace = 0.0;
	m_dbAngle = 0.0;
	m_nSingleLineMarkCount = 1;
	m_nNarrowLineNum = 2;
	m_bMatrix = FALSE;
	m_dbHeightRev = 0.0;
	m_bAutoFill = FALSE;
	m_bParity = FALSE;

	m_nCodePage = -1;
	//}}AFX_DATA_INIT
}

CBarPage::~CBarPage()
{
}

void CBarPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarPage)
	DDX_Check(pDX, IDC_CHECK_REV, m_bRev);
	DDX_Check(pDX, IDC_CHECK_GO_BACK, m_bGoBack);
	DDX_Check(pDX, IDC_CHECK_LINE_CONNECT, m_bConnect);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dbHeight);
	DDX_Text(pDX, IDC_EDIT_NARROW, m_dbNarrow);
	DDX_Text(pDX, IDC_EDIT_SHRINK, m_dbShrink);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_dbWidth);
	DDX_Text(pDX, IDC_EDIT_SPACE, m_dbSpace);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_dbAngle);
	DDX_Text(pDX, IDC_EDIT_SINGLELINE_MARKCOUNT, m_nSingleLineMarkCount);
	DDV_MinMaxDWord(pDX, m_nSingleLineMarkCount, 1, 9999999);
	DDX_Text(pDX, IDC_EDIT_NARROW_LINENUM, m_nNarrowLineNum);
	DDX_Check(pDX, IDC_CHECK_MATRIX, m_bMatrix);
	DDX_Text(pDX, IDC_EDIT_HEIGHREV, m_dbHeightRev);
	DDX_Check(pDX, IDC_CHECK_AUTOFILL, m_bAutoFill);
	DDX_Check(pDX, IDC_CHECK_PARITY, m_bParity);
	DDX_Text(pDX, IDC_EDIT_DOTRADIUS, m_dbDotRadius);
	DDX_Check(pDX, IDC_CHECK_HOLLOW, m_bHollow);
	DDX_Text(pDX, IDC_EDIT_HOLLOW_WIDTH, m_dbHollowWidth);
	DDX_Text(pDX, IDC_EDIT_HOLLOW_HEIGHT, m_dbHollowHeight);
	DDX_Control(pDX, IDC_COMBO_CODEPAGE, m_cbCodePage);
	DDX_Check(pDX, IDC_CHECK_LANG, m_bLangEnable);
}


BEGIN_MESSAGE_MAP(CBarPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBarPage)
	ON_BN_CLICKED(IDC_CHECK_REV, OnCheckRev)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_HOLLOW, &CBarPage::OnCheckHollow)
	ON_CBN_SELCHANGE(IDC_COMBO_CODEPAGE, &CBarPage::OnCbnSelchangeComboCodepage)
	ON_BN_CLICKED(IDC_BUTTON_DEFLNG, &CBarPage::OnBnClickedButtonDeflng)
	ON_BN_CLICKED(IDC_CHECK_LANG, &CBarPage::OnBnClickedCheckLang)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarPage message handlers

BOOL CBarPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(m_bRev);
	GetDlgItem(IDC_EDIT_HEIGHREV)->EnableWindow(m_bRev);

	GetDlgItem(IDC_EDIT_HOLLOW_WIDTH)->EnableWindow(m_bHollow);
	GetDlgItem(IDC_EDIT_HOLLOW_HEIGHT)->EnableWindow(m_bHollow);

	int nSel = -1;
	UINT nDefCodePage = GetACP();
	if( m_nCodePage == -1 )
		m_nCodePage = nDefCodePage;
	if( g_ArrayCodePage.GetCount() > 0 )
	{
		for(int i = 0; i<g_ArrayCodePage.GetCount(); i++)
		{
			SCodeLang sl = g_ArrayCodePage.GetAt(i);
			m_cbCodePage.InsertString(i,sl.CodePageName);
			if( sl.nCodePage == m_nCodePage )
				nSel = i;
		}
		
		if( nSel >= 0)
			m_cbCodePage.SetCurSel(nSel);
	}

	m_cbCodePage.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEFLNG)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBarPage::OnCheckRev() 
{
	UpdateData();
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(m_bRev);
	GetDlgItem(IDC_EDIT_HEIGHREV)->EnableWindow(m_bRev);
}

void CBarPage::OnCheckHollow()
{
	UpdateData();
	GetDlgItem(IDC_EDIT_HOLLOW_WIDTH)->EnableWindow(m_bHollow);
	GetDlgItem(IDC_EDIT_HOLLOW_HEIGHT)->EnableWindow(m_bHollow);
}

void CBarPage::OnCbnSelchangeComboCodepage()
{
	UpdateData(TRUE);
	int nSel = m_cbCodePage.GetCurSel();
	if( nSel >= 0 && nSel < g_ArrayCodePage.GetCount() )
	{
		m_nCodePage = g_ArrayCodePage.GetAt(nSel).nCodePage;
	}
}

void CBarPage::OnBnClickedButtonDeflng()
{
	UINT nDefCodePage = GetACP();
	if( g_ArrayCodePage.GetCount() > 0 )
	{
		for(int i = 0; i<g_ArrayCodePage.GetCount(); i++)
		{
			SCodeLang sl = g_ArrayCodePage.GetAt(i);
			if( sl.nCodePage == nDefCodePage )
			{
				m_nCodePage = nDefCodePage;
				m_cbCodePage.SetCurSel(i);
				UpdateData(TRUE);
				break;
			}
		}
	}
}

void CBarPage::OnBnClickedCheckLang()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_cbCodePage.EnableWindow(m_bLangEnable);
	GetDlgItem(IDC_BUTTON_DEFLNG)->EnableWindow(m_bLangEnable);

}
