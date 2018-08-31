
// SetQRCodebar.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetQRCodebar.h"
// #include "qrlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetQRCodebar dialog

CSetQRCodebar::CSetQRCodebar(CWnd* pParent  /*= NULL*/)
	: CDialog(CSetQRCodebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetQRCodebar)
	m_nErrorValue = 0;
	m_nSize = 0;
	m_nCodeType = 5;
	//}}AFX_DATA_INIT

	m_bMicroQR = FALSE;
}

void CSetQRCodebar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetQRCodebar)
	DDX_CBIndex(pDX, IDC_COMBO_ERRORLEVEL, m_nErrorValue);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_nSize);
	DDX_CBIndex(pDX, IDC_COMBO_CODETYPE, m_nCodeType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ctrBarSize);
	DDX_Control(pDX, IDC_COMBO_ERRORLEVEL, m_ctrErrLevel);
	DDX_Control(pDX, IDC_COMBO_CODETYPE, m_ctrBarType);
}


BEGIN_MESSAGE_MAP(CSetQRCodebar, CDialog)
	//{{AFX_MSG_MAP(CSetQRCodebar)
	//}}AFX_MSG_MAP
	ON_CBN_KILLFOCUS(IDC_COMBO_SIZE, &CSetQRCodebar::OnCbnKillfocusComboSize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetQRCodebar message handlers

BOOL CSetQRCodebar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( m_bMicroQR )
	{
		InitMicroQRSizeInfo();
	}
	else
	{
		//2014-06-30
		m_ctrBarType.ResetContent();
		CString str;
		str.LoadString(IDS_MICROQR_CODETYPE_NUMBER);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_ALP);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_BINARY);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_KANJI);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_CH);
		m_ctrBarType.AddString(str);		
		str.LoadString(IDS_MICROQR_CODETYPE_MIXCODE);
		m_ctrBarType.AddString(str);
		m_ctrBarType.SetCurSel(m_nCodeType);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CSetQRCodebar::InitMicroQRSizeInfo()
{
	if (FALSE == m_bMicroQR)
		return;
	
	CString str("");
	m_ctrBarSize.ResetContent();

	str = "11x11";
	m_ctrBarSize.AddString(str);
	str = "13x13";
	m_ctrBarSize.AddString(str);
	str = "15x15";
	m_ctrBarSize.AddString(str);
	str = "17x17";
	m_ctrBarSize.AddString(str);

	m_ctrBarSize.SetCurSel(m_nSize);
	UpdateMicroQRInfo();
}




void CSetQRCodebar::UpdateMicroQRInfo()
{
	if (FALSE == m_bMicroQR)
		return;
	
	m_ctrErrLevel.ResetContent();
	m_ctrBarType.ResetContent();
	
	CString str = "";
	if ( 0 == m_nSize )
	{
		str = "L(7%)";
		m_ctrErrLevel.AddString(str);

		str.LoadString(IDS_MICROQR_CODETYPE_NUMBER);
		m_ctrBarType.AddString(str);

		m_nErrorValue = 0;
		m_nCodeType = 0;
	}
	else if ( 1 == m_nSize )
	{
		str = "L(7%)";
		m_ctrErrLevel.AddString(str);
		str = "M(15%)";
		m_ctrErrLevel.AddString(str);
		
		str.LoadString(IDS_MICROQR_CODETYPE_NUMBER);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_ALP);
		m_ctrBarType.AddString(str);

		if (m_nErrorValue > 2)
			m_nErrorValue = 1;

		if (m_nCodeType > 2)
			m_nCodeType = 1;
	}
	else if ( 2 == m_nSize )
	{
		str = "L(7%)";
		m_ctrErrLevel.AddString(str);
		str = "M(15%)";
		m_ctrErrLevel.AddString(str);

		str.LoadString(IDS_MICROQR_CODETYPE_NUMBER);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_ALP);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_BINARY);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_KANJI);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_CH);
		m_ctrBarType.AddString(str);		
// 		str.LoadString(IDS_MICROQR_CODETYPE_MIXCODE);
// 		m_ctrBarType.AddString(str);
		
		if ( m_nErrorValue > 2 )
			m_nErrorValue = 1;
	}
	else if ( 3 == m_nSize )
	{
		str = "L(7%)";
		m_ctrErrLevel.AddString(str);
		str = "M(15%)";
		m_ctrErrLevel.AddString(str);
		str = "Q(25%)";
		m_ctrErrLevel.AddString(str);

		str.LoadString(IDS_MICROQR_CODETYPE_NUMBER);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_ALP);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_BINARY);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_KANJI);
		m_ctrBarType.AddString(str);
		str.LoadString(IDS_MICROQR_CODETYPE_CH);
		m_ctrBarType.AddString(str);
// 		str.LoadString(IDS_MICROQR_CODETYPE_MIXCODE);
// 		m_ctrBarType.AddString(str);
		
		if (m_nErrorValue > 3)
			m_nErrorValue = 2;
	}

	m_ctrErrLevel.SetCurSel(m_nErrorValue);
	m_ctrBarType.SetCurSel(m_nCodeType);
}
void CSetQRCodebar::OnCbnKillfocusComboSize()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	UpdateMicroQRInfo();
}
