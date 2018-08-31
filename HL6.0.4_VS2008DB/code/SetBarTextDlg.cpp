// SetBarTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetBarTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBarTextDlg dialog


CSetBarTextDlg::CSetBarTextDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CSetBarTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetBarTextDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSetBarTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBarTextDlg)
	DDX_Control(pDX, IDC_TAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetBarTextDlg, CDialog)
	//{{AFX_MSG_MAP(CSetBarTextDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBarTextDlg message handlers

BOOL CSetBarTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Éú³É±êÇ©
	TC_ITEM tabItem;
	tabItem.mask = TCIF_PARAM|TCIF_TEXT;
	tabItem.pszText = new TCHAR[255];
	///////
	if ( tabItem.pszText == NULL )
	{
		CDialog::OnCancel();
		return FALSE;
	}
	
	tabItem.lParam = (LPARAM)&m_PageFnt;
	VERIFY(m_PageFnt.Create(CTextPageFntDlg::IDD,&m_tab));

	CString str;
	str.LoadString(IDS_FNTTITLE);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(0,&tabItem);
	m_PageFnt.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageFnt.ShowWindow(SW_SHOW);
	m_PageFnt.EnableWindow(TRUE);
	
	tabItem.lParam = (LPARAM)&m_PageRound;
	VERIFY(m_PageRound.Create(CTextPageRoundDlg::IDD,&m_tab));
	str.LoadString(IDS_ROUNDTITLE);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(1,&tabItem);
	m_PageRound.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageRound.ShowWindow(SW_HIDE);
	m_PageRound.EnableWindow(FALSE);
	
	delete tabItem.pszText;

	SetDlgItemText(IDC_INFO,m_strInfo);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetBarTextDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab = m_tab.GetCurSel();
	TC_ITEM tci;
	
	tci.mask = TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(TRUE);

	*pResult = 0;
}

void CSetBarTextDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab = m_tab.GetCurSel();
	TC_ITEM tci;
	
	tci.mask = TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);
	pWnd->EnableWindow(FALSE);
	
	*pResult = 0;
}

void CSetBarTextDlg::OnOK() 
{
	m_PageFnt.UpdateData(TRUE);
	m_PageRound.UpdateData(TRUE);
	CDialog::OnOK();
}
