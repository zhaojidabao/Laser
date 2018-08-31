// TextPageFntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextPageFntDlg.h"
#include "FntHzk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextPageFntDlg property page

IMPLEMENT_DYNCREATE(CTextPageFntDlg, CPropertyPage)

CTextPageFntDlg::CTextPageFntDlg() : CPropertyPage(CTextPageFntDlg::IDD)
, m_bWeld(FALSE)
, m_bInterFilter(FALSE)
, m_dbInterSpace(0.1)
, m_bBroaden(FALSE)
, m_dbLineWidth(0.1)
, m_dbBroadenD(0.05)
, m_dbMatrixR(0)
, m_dbMatrixFillD(0.05)
{
	//{{AFX_DATA_INIT(CTextPageFntDlg)
	m_nFont = 1;
	m_dbCharSpace = 0.3;
	m_dbWidthFactor = 0.0;
	m_dbHeight = 0.0;
	m_dbLineSpace = 0.0;
	m_strFontCHS = _T("");
	m_strFontEN = _T("");
	m_strFontNUM = _T("");
	m_strFontUser = _T("");
	m_strPrecisK = _T("100");
	m_dbFixSpace = 8.0;
	m_bFixSpace = FALSE;
	m_strHzkType = FONT_HZK_12;
	m_strAscType = FONT_ASC_8_16;
	//}}AFX_DATA_INIT
}



CTextPageFntDlg::~CTextPageFntDlg()
{
}



void CTextPageFntDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextPageFntDlg)
	DDX_Control(pDX, IDC_COMBO_MATRIXCHS, m_cbxHzkType);
	DDX_Control(pDX, IDC_COMBO_MATRIXENN, m_cbxAscType);
	DDX_Control(pDX, IDC_COMBO_USER, m_comboUser);
	DDX_Radio(pDX, IDC_RADIO_FONT_TTF, m_nFont);
	DDX_Control(pDX, IDC_COMBO_NUM, m_comboNUM);
	DDX_Control(pDX, IDC_COMBO_EN, m_comboEN);
	DDX_Control(pDX, IDC_COMBO_CHS, m_comboCHS);
	DDX_Text(pDX, IDC_EDIT_CHARSPACE, m_dbCharSpace);
	DDX_Text(pDX, IDC_EDIT_WIDTHFACTOR, m_dbWidthFactor);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dbHeight);
	DDX_Text(pDX, IDC_EDIT_LINESPACE, m_dbLineSpace);
	DDX_CBString(pDX, IDC_COMBO_CHS, m_strFontCHS);
	DDX_CBString(pDX, IDC_COMBO_EN, m_strFontEN);
	DDX_CBString(pDX, IDC_COMBO_NUM, m_strFontNUM);
	DDX_CBString(pDX,IDC_COMBO_USER,m_strFontUser);
	DDX_CBString(pDX, IDC_COMBO_PRECIS_K, m_strPrecisK);
	DDX_Text(pDX, IDC_EDIT_FIXSPACE, m_dbFixSpace);
	DDX_Check(pDX, IDC_CHECK_FIXSPACE, m_bFixSpace);
	DDX_CBString(pDX, IDC_COMBO_MATRIXCHS, m_strHzkType);
	DDX_CBString(pDX, IDC_COMBO_MATRIXENN, m_strAscType);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_WELD, m_bWeld);
	DDX_Check(pDX, IDC_CHECK_InterFilter, m_bInterFilter);
	DDX_Text(pDX, IDC_EDIT_INTERSPACE, m_dbInterSpace);
	DDX_Check(pDX, IDC_CHECK_BROADEN, m_bBroaden);
	DDX_Text(pDX, IDC_EDIT_BROADENR, m_dbLineWidth);
	DDX_Text(pDX, IDC_EDIT_BROADEND, m_dbBroadenD);
	DDV_MinMaxDouble(pDX, m_dbBroadenD, 0.001, 100);
	DDV_MinMaxDouble(pDX, m_dbLineWidth, 0.001, 1000);
	DDX_Text(pDX, IDC_EDIT_MATRIX_RADIUS, m_dbMatrixR);
	DDV_MinMaxDouble(pDX, m_dbMatrixR, 0, 100);
	DDX_Text(pDX, IDC_EDIT_FILL_RADIUS, m_dbMatrixFillD);
}

BEGIN_MESSAGE_MAP(CTextPageFntDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTextPageFntDlg)
	ON_BN_CLICKED(IDC_RADIO_FONT_SHX, OnRadioFontShx)
	ON_BN_CLICKED(IDC_RADIO_FONT_TTF, OnRadioFontTtf)
	ON_BN_CLICKED(IDC_BUTTON_TTF, OnButtonTtf)
	ON_BN_CLICKED(IDC_RADIO_FONT_USER, OnRadioFontUser)
	ON_BN_CLICKED(IDC_CHECK_FIXSPACE, OnCheckFixspace)
	ON_BN_CLICKED(IDC_RADIO_FONT_MATRIX, OnRadioFontMatrix)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_PRECIS_K, &CTextPageFntDlg::OnCbnSelchangeComboPrecisK)
	ON_BN_CLICKED(IDC_CHECK_InterFilter, &CTextPageFntDlg::OnBnClickedCheckInterfilter)
	ON_BN_CLICKED(IDC_CHECK_BROADEN, &CTextPageFntDlg::OnBnClickedCheckBroaden)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextPageFntDlg message handlers

void CTextPageFntDlg::UpdateDataView()
{	
	GetDlgItem(IDC_BUTTON_TTF)->ShowWindow(0 == m_nFont);
	GetDlgItem(IDC_COMBO_PRECIS_K)->ShowWindow(0 == m_nFont);
	GetDlgItem(IDC_STATIC_PRECIS_K)->ShowWindow(0 == m_nFont);
	GetDlgItem(IDC_STATIC_CHS)->ShowWindow(1 == m_nFont);
	GetDlgItem(IDC_COMBO_CHS)->ShowWindow(1 == m_nFont);
	GetDlgItem(IDC_STATIC_EN)->ShowWindow(1 == m_nFont);
	GetDlgItem(IDC_COMBO_EN)->ShowWindow(1 == m_nFont);
	GetDlgItem(IDC_STATIC_NUM)->ShowWindow(1 == m_nFont);
	GetDlgItem(IDC_COMBO_NUM)->ShowWindow(1 == m_nFont);
	GetDlgItem(IDC_STATIC_USER)->ShowWindow(2 == m_nFont);
	GetDlgItem(IDC_COMBO_USER)->ShowWindow(2 == m_nFont);
	GetDlgItem(IDC_STATIC_CHSM)->ShowWindow(3 == m_nFont);
	GetDlgItem(IDC_COMBO_MATRIXCHS)->ShowWindow(3 == m_nFont);
	GetDlgItem(IDC_STATIC_ENNM)->ShowWindow(3 == m_nFont);
	GetDlgItem(IDC_COMBO_MATRIXENN)->ShowWindow(3 == m_nFont);
	//
	GetDlgItem(IDC_STATIC_MATRIX_RADIUS)->ShowWindow(3 == m_nFont);
	GetDlgItem(IDC_EDIT_MATRIX_RADIUS)->ShowWindow(3 == m_nFont);
	GetDlgItem(IDC_STATIC_MATRIX_FILLR)->ShowWindow(3 == m_nFont);
	GetDlgItem(IDC_EDIT_FILL_RADIUS)->ShowWindow(3 == m_nFont);

	GetDlgItem(IDC_EDIT_INTERSPACE)->EnableWindow(m_bInterFilter);
	GetDlgItem(IDC_EDIT_BROADENR)->EnableWindow(m_bBroaden);
	GetDlgItem(IDC_EDIT_BROADEND)->EnableWindow(m_bBroaden);

	switch(m_nFont) 
	{
		case 0:		//TTF字体
		case 1:		//SHX字体
		case 3:		//点阵
			GetDlgItem(IDC_EDIT_CHARSPACE)->EnableWindow(!m_bFixSpace);
			GetDlgItem(IDC_EDIT_WIDTHFACTOR)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_LINESPACE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_FIXSPACE)->EnableWindow(m_bFixSpace);
			break;
		case 2:
			//大族字体
			GetDlgItem(IDC_EDIT_CHARSPACE)->EnableWindow(!m_bFixSpace);
			GetDlgItem(IDC_EDIT_WIDTHFACTOR)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_LINESPACE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_FIXSPACE)->EnableWindow(TRUE);
	}
}

//
void CTextPageFntDlg::OnRadioFontShx() 
{
	UpdateData();
	UpdateDataView();
}

void CTextPageFntDlg::OnRadioFontTtf() 
{
	UpdateData();
	UpdateDataView();
}

void CTextPageFntDlg::OnRadioFontUser() 
{
	UpdateData();
	UpdateDataView();
}


void CTextPageFntDlg::OnRadioFontMatrix() 
{
	UpdateData();
	UpdateDataView();	
}


void CTextPageFntDlg::OnCheckFixspace()
{
	UpdateData();
	UpdateDataView();
}

void CTextPageFntDlg::Layout()
{
	CRect rect, rt;
	int offx(0), offy(0);
	GetDlgItem(IDC_STATIC_WINDOW)->GetClientRect(rt);
	MoveWindow(rt);	

	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_TTF)->GetWindowRect(rt);
	offx = rect.left-rt.left;
	offy = rect.top-rt.top;
	GetDlgItem(IDC_BUTTON_TTF)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_BUTTON_TTF)->MoveWindow(rt);
	GetDlgItem(IDC_STATIC_PRECIS_K)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_PRECIS_K)->MoveWindow(rt);
	GetDlgItem(IDC_COMBO_PRECIS_K)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_COMBO_PRECIS_K)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_SHX)->GetWindowRect(rt);
	offx = rect.left-rt.left;
	offy = rect.top-rt.top;
	GetDlgItem(IDC_STATIC_CHS)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_CHS)->MoveWindow(rt);
	GetDlgItem(IDC_COMBO_CHS)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_COMBO_CHS)->MoveWindow(rt);
	GetDlgItem(IDC_STATIC_EN)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_EN)->MoveWindow(rt);
	GetDlgItem(IDC_COMBO_EN)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_COMBO_EN)->MoveWindow(rt);
	GetDlgItem(IDC_STATIC_NUM)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_NUM)->MoveWindow(rt);
	GetDlgItem(IDC_COMBO_NUM)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_COMBO_NUM)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_CUSTOM)->GetWindowRect(rt);
	offx = rect.left-rt.left;
	offy = rect.top-rt.top;
	GetDlgItem(IDC_STATIC_USER)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_USER)->MoveWindow(rt);
	GetDlgItem(IDC_COMBO_USER)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_COMBO_USER)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_MATRIX)->GetWindowRect(rt);
	offx = rect.left-rt.left;
	offy = rect.top-rt.top;
	GetDlgItem(IDC_STATIC_CHSM)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_CHSM)->MoveWindow(rt);
	GetDlgItem(IDC_COMBO_MATRIXCHS)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_COMBO_MATRIXCHS)->MoveWindow(rt);
	GetDlgItem(IDC_STATIC_ENNM)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_ENNM)->MoveWindow(rt);
	GetDlgItem(IDC_COMBO_MATRIXENN)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_COMBO_MATRIXENN)->MoveWindow(rt);

	//
	GetDlgItem(IDC_STATIC_MATRIX_RADIUS)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_MATRIX_RADIUS)->MoveWindow(rt);
	GetDlgItem(IDC_EDIT_MATRIX_RADIUS)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_EDIT_MATRIX_RADIUS)->MoveWindow(rt);
	GetDlgItem(IDC_STATIC_MATRIX_FILLR)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_STATIC_MATRIX_FILLR)->MoveWindow(rt);
	GetDlgItem(IDC_EDIT_FILL_RADIUS)->GetWindowRect(rt);
	rt.OffsetRect(offx, offy);
	ScreenToClient(rt);
	GetDlgItem(IDC_EDIT_FILL_RADIUS)->MoveWindow(rt);
}


BOOL CTextPageFntDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	Layout();
	
	//初始化字体信息
	CString path = g_sys.GetInstallPath();
	
	CString searchFile;
	CString font;
	CFileFind finder;
    BOOL bWorking;
	
	searchFile = path+_T("\\pfonts\\*.shx");
	bWorking = finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-4);
		m_comboCHS.AddString(font);
	}
	m_comboCHS.SelectString(-1,m_strFontCHS);
	
	searchFile = path+_T("\\efonts\\*.shx");
	bWorking = finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-4);
		m_comboEN.AddString(font);
	}
	m_comboEN.SelectString(-1,m_strFontEN);	
	
	searchFile = path+_T("\\efonts\\*.shx");
	bWorking = finder.FindFile(searchFile);
    while(bWorking)
	{
		bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-4);
		m_comboNUM.AddString(font);
	}
	m_comboNUM.SelectString(-1,m_strFontNUM);	
	
	//大族字体
	searchFile = path+_T("\\Font\\*.font");
	bWorking = finder.FindFile(searchFile);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-5);
		m_comboUser.AddString(font);
	}
	m_comboUser.SelectString(-1,m_strFontUser);	

	//
	m_cbxHzkType.AddString(FONT_HZK_12);
	m_cbxHzkType.AddString(FONT_HZK_14);
	m_cbxHzkType.AddString(FONT_HZK_16);
	m_cbxAscType.AddString(FONT_ASC_5_7);
	m_cbxAscType.AddString(FONT_ASC_8_16);
	
	UpdateDataView();
	
	return TRUE;
}





//
void CTextPageFntDlg::OnButtonTtf()
{
	CFontDialog dlg(&m_lf);
	dlg.m_cf.Flags &= ~CF_EFFECTS;
	
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&m_lf);
	}
}




void CTextPageFntDlg::OnCbnSelchangeComboPrecisK()
{
	// TODO: 在此添加控件通知处理程序代码
}





void CTextPageFntDlg::OnBnClickedCheckInterfilter()
{
	UpdateData();
	UpdateDataView();
}

void CTextPageFntDlg::OnBnClickedCheckBroaden()
{
	UpdateData();
	UpdateDataView();
}

