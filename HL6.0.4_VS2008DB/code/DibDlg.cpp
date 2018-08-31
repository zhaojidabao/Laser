
// DibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DibDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAXDOTTIME	65535	//最大单点时间

/////////////////////////////////////////////////////////////////////////////
// CDibDlg dialog

CDibDlg::CDibDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CDibDlg::IDD, pParent)
	, m_nMinFDL(50)
	, m_nMaxFDL(815)
{
	//{{AFX_DATA_INIT(CDibDlg)
	m_strBmpName = _T("");
	m_bOppsitioncol = FALSE;
	m_dbSpaceX = 0.05;
	m_dbSpaceY = 0.05;
	m_dbDotTime = MAXDOTTIME;
	m_bLoop = FALSE;
	m_nDibType  = 0;
	m_dbQFre = 0.0;
	m_dbLoopCompate = 0.0f;
	m_nAccTime = 0;
	m_dbBrightness = 0.0;
	m_dbContrast = 0.0;
	m_nComSel = 0;
	m_dbQFre = 0.0;
	//}}AFX_DATA_INIT
}

void CDibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDibDlg)
	DDX_Text(pDX, IDC_FILE_NAME, m_strBmpName);
	DDX_Check(pDX, IDC_OPPOSITION_COL, m_bOppsitioncol);
	DDX_Text(pDX, IDC_SPACE_X, m_dbSpaceX);
	DDV_MinMaxDouble(pDX, m_dbSpaceX,1.e-003,1.);
	DDX_Text(pDX, IDC_SPACE_Y, m_dbSpaceY);	
	DDV_MinMaxDouble(pDX, m_dbSpaceY,1.e-003,1.);
	DDX_Text(pDX, IDC_EDIT_DOTTIME, m_dbDotTime);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bLoop);
	DDX_Radio(pDX, IDC_RADIO_GRAY, m_nDibType);
	DDV_MinMaxInt(pDX, m_nDibType, 0, 2);
	DDX_Text(pDX, IDC_EDIT_QFre, m_dbQFre);
	DDX_Text(pDX, IDC_EDIT_LoopCompate, m_dbLoopCompate);
	DDV_MinMaxDouble(pDX, m_dbLoopCompate, -50., 50.);
	DDX_Text(pDX, IDC_EDIT_ACCTime, m_nAccTime);
	DDV_MinMaxInt(pDX, m_nAccTime, 0, 50);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_dbBrightness);
	DDV_MinMaxDouble(pDX, m_dbBrightness, -1., 1.);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_dbContrast);
	DDV_MinMaxDouble(pDX, m_dbContrast, -1., 1.);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_DELAY_MIN, m_nMinFDL);
	DDV_MinMaxInt(pDX, m_nMinFDL, 0, 1000);
	DDX_Text(pDX, IDC_DELAY_MAX, m_nMaxFDL);
	DDV_MinMaxInt(pDX, m_nMaxFDL, 0, 1000);
}

BEGIN_MESSAGE_MAP(CDibDlg, CDialog)
	//{{AFX_MSG_MAP(CDibDlg)
	ON_EN_KILLFOCUS(IDC_SPACE_X, OnKillfocusSpaceX)
	ON_BN_CLICKED(IDC_CHECK_LOOP, OnCheckLoop)
	ON_EN_KILLFOCUS(IDC_EDIT_DOTTIME, OnKillfocusEditDottime)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_GRAY, OnBnClickedRadioGray)
	ON_BN_CLICKED(IDC_RADIO_DIFF, OnBnClickedRadioDiff)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_DOTMATRIX, &CDibDlg::OnBnClickedRadioDotmatrix)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibDlg message handlers

void CDibDlg::OnBnClickedRadioGray()
{
	UpdateData();
	GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(m_nDibType == 1?TRUE:FALSE);
	GetDlgItem(IDC_BUTTON_GRAYCURRENT)->EnableWindow(m_nDibType == 1?FALSE:TRUE);
	GetDlgItem(IDC_DELAY_MIN)->EnableWindow(m_nDibType == 2 );
	GetDlgItem(IDC_DELAY_MAX)->EnableWindow(m_nDibType == 2 );
	GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(m_nDibType == 0);
	if (m_nDibType == 1)
	{
		//GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
	}	
	else
	{
		//GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop);
	}
}

void CDibDlg::OnBnClickedRadioDiff()
{
	UpdateData();
//	GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(m_nDibType == 1?TRUE:FALSE);
	GetDlgItem(IDC_BUTTON_GRAYCURRENT)->EnableWindow(m_nDibType == 1?FALSE:TRUE);
	GetDlgItem(IDC_DELAY_MIN)->EnableWindow(m_nDibType == 2 );
	GetDlgItem(IDC_DELAY_MAX)->EnableWindow(m_nDibType == 2 );
	GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(m_nDibType == 0);
	if (m_nDibType == 1)
	{
//		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
	}	
	else
	{
//		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop);
	}
}

BOOL CDibDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_DELAY_MIN)->EnableWindow(m_nDibType == 2);
	GetDlgItem(IDC_DELAY_MAX)->EnableWindow(m_nDibType == 2);
	GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(m_nDibType == 0);
	if (m_nDibType == 1)
	{
	//	GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
	}	
	else
	{
	//	GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop);
	}
	
	
	m_dbQFre = m_dbMarkSpeed / m_dbSpaceX * 1000;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDibDlg::OnKillfocusSpaceX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if( m_dbSpaceX < 1.e-003 )
		m_dbSpaceX = 0.001;
	else if( m_dbSpaceX > 1 )
		m_dbSpaceX = 1;

	m_dbQFre = m_dbMarkSpeed/m_dbSpaceX * 1000;
	UpdateData(FALSE);
}

void CDibDlg::OnCheckLoop() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nDibType == 0)
	{
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop?TRUE:FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	
}

void CDibDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if( m_dbSpaceX < 1.e-003 )
		m_dbSpaceX = 0.001;
	else if( m_dbSpaceX > 1 )
		m_dbSpaceX = 1;

	if( m_dbSpaceY < 1.e-003 )
		m_dbSpaceY = 0.001;
	else if( m_dbSpaceY > 1 )
		m_dbSpaceY = 1;
	if( m_dbLoopCompate < -50 )
		m_dbLoopCompate = -50;
	else if( m_dbLoopCompate > 50 )
		m_dbLoopCompate = 50;

	if( m_nAccTime < 0 )
		m_nAccTime = 0;
	else if( m_nAccTime > 50 )
		m_nAccTime = 50;
	if( m_dbBrightness < - 1 )
		m_dbBrightness = -1;
	else if( m_dbBrightness > 1 )
		m_dbBrightness = 1;

	if( m_dbContrast < -1 )
		m_dbContrast = -1;
	else if( m_dbContrast > 1 )
		m_dbContrast = 1;

	UpdateData(FALSE);
	CDialog::OnOK();
}

void CDibDlg::OnKillfocusEditDottime() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_dbDotTime > MAXDOTTIME) 
	{
		m_dbDotTime = MAXDOTTIME;
	}
	UpdateData(FALSE);
	
}

void CDibDlg::OnClose()
{
	CDialog::OnClose();
}

void CDibDlg::OnBnClickedRadioDotmatrix()
{
	UpdateData();
	GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(m_nDibType == 1?TRUE:FALSE);
	GetDlgItem(IDC_BUTTON_GRAYCURRENT)->EnableWindow(m_nDibType == 1?FALSE:TRUE);
	GetDlgItem(IDC_DELAY_MIN)->EnableWindow(m_nDibType == 2);
	GetDlgItem(IDC_DELAY_MAX)->EnableWindow(m_nDibType == 2);
	GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(m_nDibType == 0);
	if (m_nDibType == 1)
	{
		//GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
	}	
	else
	{
		//GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop);
	}
}
