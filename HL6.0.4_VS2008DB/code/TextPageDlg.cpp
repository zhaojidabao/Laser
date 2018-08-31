// TextPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextPageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextPageDlg property page
IMPLEMENT_DYNCREATE(CTextPageDlg, CPropertyPage)

CTextPageDlg::CTextPageDlg() : CPropertyPage(CTextPageDlg::IDD)
{
	//{{AFX_DATA_INIT(CTextPageDlg)
	//}}AFX_DATA_INIT
}

CTextPageDlg::~CTextPageDlg()
{
	Delete();
}

void CTextPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextPageDlg)
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Control(pDX, IDC_TAB_TEXT, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextPageDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTextPageDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TEXT, OnSelchangeTabText)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_TEXT, OnSelchangingTabText)
	ON_BN_CLICKED(IDC_BUT_ADD, OnButAdd)
	ON_BN_CLICKED(IDC_BUT_DEL, OnButDel)
	ON_BN_CLICKED(IDC_BUT_MODIFY, OnButModify)
	ON_LBN_SELCHANGE(IDC_LIST_DATA, OnSelchangeListData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextPageDlg message handlers

BOOL CTextPageDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString str;
	//生成标签
	TC_ITEM tabItem;
	tabItem.mask = TCIF_PARAM|TCIF_TEXT;
	tabItem.pszText = new TCHAR[255];
	///////
	if ( tabItem.pszText == NULL )
		return FALSE;
	
	tabItem.lParam = (LPARAM)&m_PageText;
	VERIFY(m_PageText.Create(CTextPageTextDlg::IDD,&m_tab));
	str.LoadString(IDS_TEXTTITLE);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(0,&tabItem);
	m_PageText.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageText.ShowWindow(SW_SHOW);
	m_PageText.EnableWindow(TRUE);
	
	tabItem.lParam = (LPARAM)&m_PageFnt;
	VERIFY(m_PageFnt.Create(CTextPageFntDlg::IDD,&m_tab));
	str.LoadString(IDS_FNTTITLE);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(1,&tabItem);
	m_PageFnt.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageFnt.ShowWindow(SW_HIDE);
	m_PageFnt.EnableWindow(FALSE);
		
	tabItem.lParam = (LPARAM)&m_PageRound;
	VERIFY(m_PageRound.Create(CTextPageRoundDlg::IDD,&m_tab));
	str.LoadString(IDS_ROUNDTITLE);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(2,&tabItem);
	m_PageRound.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageRound.ShowWindow(SW_HIDE);
	m_PageRound.EnableWindow(FALSE);

	delete tabItem.pszText;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextPageDlg::OnSelchangeTabText(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
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

void CTextPageDlg::OnSelchangingTabText(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
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

void CTextPageDlg::OnButAdd()
{
	CText* pNew = new CText();
	if ( NULL == pNew )  
		return;

	pNew->FlashText(0x00);
	m_list.AddTail(pNew);

	int nSel = m_listData.AddString(_T(""));
	m_listData.SetCurSel(nSel);

	Update_Data(TRUE);
	ListFlash();
}

void CTextPageDlg::Delete()
{
	POSITION pos = m_list.GetHeadPosition();
	while ( pos  !=  NULL )
	{
		CText *pText = m_list.GetNext(pos);
		if ( pText  !=  NULL )
		{
			delete pText;
			pText = NULL;
		}
	}

	m_list.RemoveAll();
}

void CTextPageDlg::OnButDel()
{
	if  ( m_listData.GetCount()  <=  1 )
		return;

	int nIndex = m_listData.GetCurSel();
	if (  LB_ERR == nIndex) 
		return;

	m_listData.DeleteString(nIndex);
	POSITION pos = m_list.FindIndex(nIndex);
	if ( pos  !=  NULL )
	{
		CText *pText = m_list.GetAt(pos);
		if ( pText  !=  NULL )
		{
			delete  pText;
			pText = NULL;
		}
		m_list.RemoveAt(pos);
	}
}

void CTextPageDlg::OnButModify()
{
	Update_Data(TRUE);
	ListFlash();
}

void CTextPageDlg::OnSelchangeListData() 
{
	Update_Data(FALSE);
}

void CTextPageDlg::ListFlash()
{
	m_listData.ResetContent();

	CText* pText = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pText = m_list.GetNext(pos);
		if ( NULL == pText )
			continue;

		m_listData.AddString(pText->GetInfo());
	}
}

//
void CTextPageDlg::Update_Data (BOOL bSaveAndValidate)
{
	UINT nIndex = m_listData.GetCurSel();
	if ( LB_ERR == nIndex) 
		return;
	
	CText* pText = m_list.GetAt(m_list.FindIndex(nIndex));
	if ( NULL == pText) 
		return;

	if ( TRUE == bSaveAndValidate )
	{
		m_PageText.UpdateData(TRUE);
		pText->m_nText			 =  m_PageText.m_nText;
		pText->m_strFix			 =  m_PageText.m_strFix;
		pText->m_strDate		 =  m_PageText.m_strDate;
		pText->m_nJumpStart	 =  m_PageText.m_nJumpStart;
		pText->m_nJumpEnd		 =  m_PageText.m_nJumpEnd;
		pText->m_strLeft			 =  m_PageText.m_strLeft;
		pText->m_strRight		 =  m_PageText.m_strRight;
		pText->m_nJumpLen		 =  m_PageText.m_nJumpLen;
		pText->m_strPrefix		 =  m_PageText.m_strPrefix;
		pText->m_nJumpStep		 =  m_PageText.m_nJumpStep;
		//循环跳号
		pText->m_bJumpLoop       =  m_PageText.m_bJumpLoop;
		pText->m_nJumpDis		 =  m_PageText.m_nJumpInterval;
		pText->m_bFill				 =  m_PageText.m_bFill;
		pText->m_nFromFile		 =  m_PageText.m_nFromFile;
		pText->m_strFromFile	 =  m_PageText.m_strFromFile;
		pText->m_strBinText	 =  m_PageText.m_strBinText;
		pText->m_strLocal		 =  m_PageText.m_strLocal;
	}
	else
	{
		m_PageText.m_nText		 =  pText->m_nText;
		m_PageText.m_strFix		 =  pText->m_strFix;
		m_PageText.m_strDate		 =  pText->m_strDate;
		m_PageText.m_nJumpStart =  pText->m_nJumpStart;
		m_PageText.m_nJumpEnd	 =  pText->m_nJumpEnd;
		m_PageText.m_strLeft		 =  pText->m_strLeft;
		m_PageText.m_strRight		 =  pText->m_strRight;
		m_PageText.m_nJumpLen	 =  pText->m_nJumpLen;
		m_PageText.m_strPrefix		 =  pText->m_strPrefix;
		m_PageText.m_nJumpStep	 =  pText->m_nJumpStep;
		//循环跳号
		m_PageText.m_bJumpLoop   =  pText->m_bJumpLoop;
		m_PageText.m_nJumpInterval = pText->m_nJumpDis;
		m_PageText.m_bFill			 =  pText->m_bFill;
		m_PageText.m_nFromFile	 =  pText->m_nFromFile;
		m_PageText.m_strFromFile	 =  pText->m_strFromFile;
		m_PageText.m_pSerial		 =  &(pText->m_saSerial);
		m_PageText.m_pCur			 =  &(pText->m_nMarkTimes);
		m_PageText.m_strBinText	 =  pText->m_strBinText;	
		m_PageText.m_strLocal		 =  pText->m_strLocal;
		
		m_PageText.UpdateData(FALSE);
		m_PageText.UpdateDataView();
	}
}


