// TextPageRoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextPageRoundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextPageRoundDlg property page

IMPLEMENT_DYNCREATE(CTextPageRoundDlg, CPropertyPage)

CTextPageRoundDlg::CTextPageRoundDlg() : CPropertyPage(CTextPageRoundDlg::IDD)
, m_bPrintInverse(FALSE)
{
	//{{AFX_DATA_INIT(CTextPageRoundDlg)
	m_bCCW = FALSE;
	m_dbDirection = 0.0;
	m_dbRadius = 0.0;
	m_dbStart = 0.0;
	m_nAlign = 0;
	m_nPath = -1;
	m_bInside = FALSE;
	m_strFormat = _T("");
	m_bCustomFormat = FALSE;
	//}}AFX_DATA_INIT
}

CTextPageRoundDlg::~CTextPageRoundDlg()
{
}

void CTextPageRoundDlg::DoDataExchange(CDataExchange* pDX)
{	
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextPageRoundDlg)
	DDX_Check(pDX, IDC_CHECK_CCW, m_bCCW);
	DDX_Text(pDX, IDC_EDIT_DIRECTION, m_dbDirection);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dbRadius);
	DDX_Text(pDX, IDC_EDIT_START, m_dbStart);
	DDX_Radio(pDX, IDC_RADIO_ALIGN_LEFT, m_nAlign);
	DDX_Radio(pDX, IDC_RADIO_PATH_LINE, m_nPath);
	DDX_Check(pDX, IDC_CHECK_INSIDE, m_bInside);
	DDX_Text(pDX, IDC_EDIT_FORMAT, m_strFormat);
	DDX_Check(pDX, IDC_CHECK_CUSTOM, m_bCustomFormat);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_PRINTINVERSE, m_bPrintInverse);
}

BEGIN_MESSAGE_MAP(CTextPageRoundDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTextPageRoundDlg)
	ON_BN_CLICKED(IDC_RADIO_PATH_LINE, OnRadioPathLine)
	ON_BN_CLICKED(IDC_RADIO_PATH_ROUND, OnRadioPathRound)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM, OnCheckCustom)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_PRINTINVERSE, &CTextPageRoundDlg::OnBnClickedCheckPrintinverse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextPageRoundDlg message handlers

void CTextPageRoundDlg::OnRadioPathLine() 
{
	UpdateData();
	UpdateDataView();
}

void CTextPageRoundDlg::OnRadioPathRound() 
{
	UpdateData();
	UpdateDataView();
}

void CTextPageRoundDlg::UpdateDataView()
{	
	BOOL bFlag;
	switch(m_nPath) 
	{	
	case 0:
		bFlag = (m_nPath == 0);
		GetDlgItem(IDC_EDIT_DIRECTION)->EnableWindow(bFlag);
		GetDlgItem(IDC_RADIO_ALIGN_LEFT)->EnableWindow(bFlag);
		GetDlgItem(IDC_RADIO_ALIGN_RIGHT)->EnableWindow(bFlag);
		GetDlgItem(IDC_RADIO_ALIGN_MID)->EnableWindow(bFlag);
		GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(!bFlag);
		GetDlgItem(IDC_EDIT_START)->EnableWindow(!bFlag);
		GetDlgItem(IDC_CHECK_CCW)->EnableWindow(!bFlag);
		GetDlgItem(IDC_CHECK_INSIDE)->EnableWindow(!bFlag);
		GetDlgItem(IDC_CHECK_PRINTINVERSE)->EnableWindow(!bFlag);
		break;

	case 1:
		bFlag = (m_nPath == 1);
		GetDlgItem(IDC_EDIT_DIRECTION)->EnableWindow(!bFlag);
		GetDlgItem(IDC_RADIO_ALIGN_LEFT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_RADIO_ALIGN_RIGHT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_RADIO_ALIGN_MID)->EnableWindow(!bFlag);
		GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(bFlag);
		GetDlgItem(IDC_EDIT_START)->EnableWindow(bFlag);
		GetDlgItem(IDC_CHECK_CCW)->EnableWindow(bFlag);
		GetDlgItem(IDC_CHECK_INSIDE)->EnableWindow(bFlag);
		GetDlgItem(IDC_CHECK_PRINTINVERSE)->EnableWindow(bFlag);
		break;
	}

	GetDlgItem(IDC_EDIT_FORMAT)->EnableWindow(m_bCustomFormat);
}





//
BOOL CTextPageRoundDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	UpdateDataView();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextPageRoundDlg::OnCheckCustom() 
{
	UpdateData();
	GetDlgItem(IDC_EDIT_FORMAT)->EnableWindow(m_bCustomFormat);
}

void CTextPageRoundDlg::OnBnClickedCheckPrintinverse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}
