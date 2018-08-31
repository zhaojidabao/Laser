// SetLangRegionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Hl.h"
#include "SetLangRegionDlg.h"


// CSetLangRegionDlg 对话框

IMPLEMENT_DYNAMIC(CSetLangRegionDlg, CDialog)

CSetLangRegionDlg::CSetLangRegionDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CSetLangRegionDlg::IDD, pParent)
{
	m_nLangId = 0;  //default - Chinese(Simplified)
	strcpy(m_szLang,"chs");
}

CSetLangRegionDlg::~CSetLangRegionDlg()
{
}

void CSetLangRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LANG, m_combLang);
}


BEGIN_MESSAGE_MAP(CSetLangRegionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetLangRegionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetLangRegionDlg 消息处理程序
BOOL CSetLangRegionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_combLang.AddString(_T("Chinese (simplified)"));
	m_combLang.AddString(_T("Chinese (traditional)"));
	m_combLang.AddString(_T("Japanese"));
	m_combLang.AddString(_T("Korean"));
	m_combLang.AddString(_T("Czech"));
	m_combLang.AddString(_T("Danish"));
	m_combLang.AddString(_T("Dutch (default)"));
	m_combLang.AddString(_T("Dutch (Belgium)"));
	m_combLang.AddString(_T("English (default)"));
	m_combLang.AddString(_T("English (Australia)"));
	m_combLang.AddString(_T("English (Canada)"));
	m_combLang.AddString(_T("English (New Zealand)"));
	m_combLang.AddString(_T("English (United Kingdom)"));
	m_combLang.AddString(_T("English (United States)"));
	m_combLang.AddString(_T("Finnish"));
	m_combLang.AddString(_T("French (default)"));
	m_combLang.AddString(_T("French (Belgium)"));
	m_combLang.AddString(_T("French (Canada)"));
	m_combLang.AddString(_T("French (Switzerland)"));
	m_combLang.AddString(_T("German (default)"));
	m_combLang.AddString(_T("German (Austria)"));
	m_combLang.AddString(_T("German (Switzerland)"));
	m_combLang.AddString(_T("Greek"));
	m_combLang.AddString(_T("Hungarian"));
	m_combLang.AddString(_T("Icelandic"));
	m_combLang.AddString(_T("Italian (default)"));
	m_combLang.AddString(_T("Italian (Switzerland)"));
	m_combLang.AddString(_T("Norwegian (default)"));
	m_combLang.AddString(_T("Norwegian (Bokmal)"));
	m_combLang.AddString(_T("Norwegian (Nynorsk)"));
	m_combLang.AddString(_T("Polish"));
	m_combLang.AddString(_T("Portuguese (default)"));
	m_combLang.AddString(_T("Portuguese (Brazil)"));
	m_combLang.AddString(_T("Russian (default)"));
	m_combLang.AddString(_T("Slovak"));
	m_combLang.AddString(_T("Spanish (default)"));
	m_combLang.AddString(_T("Spanish (Mexico)"));
	m_combLang.AddString(_T("Spanish (Modern)"));
	m_combLang.AddString(_T("Swedish"));
	m_combLang.AddString(_T("Turkish"));
	
	m_nLangId = ::g_sys.GetProfileInt(_T("CHARSET"),_T("REGION_ID"),0);
	m_combLang.SetCurSel(m_nLangId);
	UpdateData(FALSE);

	return TRUE;
}

void CSetLangRegionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	static char szLang[][10] = {
								"chs",
								"cht",
								"jpn",
								"kor",
								"csy",
								"dan",
								"nld",
								 "nlb",
								"english",
								"ena",
								"enc",
								"enz",
								"eng",
								"enu",
								"fin", 
								"fra",
								"frb",
								"frc",
								"frs",
								"deu",
								"dea",
								"des",
								"ell",
								"hun",
								"isl",
								"ita",
								"its",
								"norwegian",
								"nor", 
								"non",
								"plk",
								"ptg",
								"ptb",
								"rus",
								"sky",
								"esp",
								"esm",
								"esn",
								"sve",
								"trk" 
							};

	UpdateData(TRUE);
	int nSel = m_combLang.GetCurSel();
	if ( nSel == LB_ERR )
		return;
	
	strcpy(m_szLang,szLang[nSel]);
	m_nLangId = nSel;

	CString strLang(m_szLang);
	::g_sys.WriteProfileInt(_T("CHARSET"),_T("REGION_ID"),m_nLangId);
	::g_sys.WriteIniString(_T("CHARSET"),_T("REGION_LANG"),strLang.GetBuffer());
	strLang.ReleaseBuffer();
	OnOK();
}
