// FillEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "FillEditDlg.h"
#include "HLView.h"
#include "FastMarkStopDlg.h"
#include "baseMacro.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFillEditDlg dialog

CFillEditDlg::CFillEditDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CFillEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFillEditDlg)
	m_nTabIndex = 0;
	//}}AFX_DATA_INIT
	m_nTabPrev = -1;
	m_nObjPrev = -1;
}


void CFillEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFillEditDlg)
	DDX_Control(pDX, IDC_TAB_OBJEDIT, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillEditDlg, CDialog)
	//{{AFX_MSG_MAP(CFillEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLYFILL, OnButtonApply)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_OBJEDIT, OnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OBJEDIT, OnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_FASTMARK, OnButtonFastmark)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_OUTLINEDATA, OnButtonDownOfflinedata)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillEditDlg message handlers

void CFillEditDlg::OnButtonApply() 
{	
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;

	if ( g_bIsSelMark  || g_bIsRedPreview || g_bIsFastMark )
		return;

	CHLView* pView = GetCurActiveView();
	if ( pView == NULL )
		return;

	CHLDoc *pDoc = pView->GetDocument();
	if ( pDoc == NULL )
		return;
	
	GetDlgItem(IDC_BUTTON_APPLYFILL)->EnableWindow(FALSE);
	AfxGetApp()->BeginWaitCursor();	
	
	m_BarEditDlg.Apply();
	m_TextEditDlg.Apply();
	m_ArcInfo.Apply();
	m_CircleInfo.Apply();
	
	m_DelayEditDlg.Apply();
	m_InputEditDlg.Apply();
	m_OutputEditDlg.Apply();
	m_MotorMove.Apply();
	m_MotorPos.Apply();
	m_BufferLaser.Apply();
	m_CCD.Apply();
	m_PosCali.Apply();

	m_nTabPrev = m_tab.GetCurSel();
	
	if ( m_tab.GetItemCount() > 1 && m_nTabPrev == 1 )
	{	
		FILLDATA fillNew;
		m_ObjFillEdit.GetNewFillData(fillNew);
		pDoc->UpdateFill(fillNew);
	}
	else if ( 0 == m_nTabPrev ) 
	{	
		m_FillObjMarkSet.Apply();
	}
	
	GetDlgItem(IDC_BUTTON_APPLYFILL)->EnableWindow(TRUE);
	AfxGetApp()->EndWaitCursor();
	
	pView->DispSelObjProp();
	pDoc->SetTrackerQuad(pDoc->GetLimit());
}

//
BOOL CFillEditDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	
	VERIFY(m_FillObjMarkSet.Create(CFillObjmMarkSetDlg::IDD,&m_tab));
	VERIFY(m_ObjFillEdit.Create(CObjecteditfill::IDD,&m_tab));
	VERIFY(m_TextEditDlg.Create(CTextEditDlg::IDD, &m_tab));
	VERIFY(m_BarEditDlg.Create(CDlgBarEdit::IDD, &m_tab));	
	VERIFY(m_DelayEditDlg.Create(CDlgSetDelay::IDD, &m_tab));
	VERIFY(m_InputEditDlg.Create(CDlgSetInput::IDD, &m_tab));
	VERIFY(m_OutputEditDlg.Create(CDlgSetOutput::IDD, &m_tab));
	VERIFY(m_ArcInfo.Create(CDlgObjEditArcInfo::IDD, &m_tab));
	VERIFY(m_CircleInfo.Create(CDlgObjInfoCircle::IDD, &m_tab));
	VERIFY(m_MotorMove.Create(CDlgObjInfoMotorMove::IDD, &m_tab));
	VERIFY(m_MotorPos.Create(CDlgObjInfoMotorPos::IDD, &m_tab));
	VERIFY(m_BufferLaser.Create(CDlgObjInfoBufferLaser::IDD, &m_tab));	
	VERIFY(m_CCD.Create(CDlgObjInfoCCD::IDD, &m_tab));
	VERIFY(m_PosCali.Create(CDlgObjPosCali::IDD, &m_tab));
	
	m_FillObjMarkSet.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_ObjFillEdit.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_TextEditDlg.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BarEditDlg.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_DelayEditDlg.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_InputEditDlg.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_OutputEditDlg.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_ArcInfo.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_CircleInfo.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_MotorMove.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_MotorPos.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BufferLaser.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_CCD.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PosCali.SetWindowPos(NULL,0,18,0,0,SWP_NOSIZE|SWP_NOZORDER);
	
#ifdef _SOFTMODE_EDITOR_
	GetDlgItem(IDC_BUTTON_FASTMARK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN_OUTLINEDATA)->EnableWindow(FALSE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFillEditDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab = m_tab.GetCurSel();
	if (  iTab < 0 )
		return;
	
	m_nTabIndex = iTab;

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	if ( pWnd  )
	{
		pWnd->ShowWindow(SW_HIDE);
		pWnd->EnableWindow(FALSE);
	}
	
	*pResult = 0;
}

//
void CFillEditDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab = m_tab.GetCurSel();
	if ( iTab < 0  )
		return;
	
	m_nTabIndex = iTab;

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	if ( pWnd )
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->EnableWindow(TRUE);
	}
	
	*pResult = 0;
}

//
void CFillEditDlg::SetTable(int nItem)
{	
	int nCnt = m_tab.GetItemCount();
	if ( nItem  >=  nCnt )
		return;

	int iTab = m_tab.GetCurSel();
	if (  iTab  < 0 )
		return;

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	//隐藏原有的标签
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);

	CWnd *pWnd = (CWnd *)tci.lParam;
	if ( pWnd )
	{
		pWnd->ShowWindow(SW_HIDE);
		pWnd->EnableWindow(FALSE);
	}

	//显示选定的标签
	m_tab.SetCurSel(nItem);
	m_tab.GetItem(nItem,&tci);
	ASSERT(tci.lParam);

	pWnd = (CWnd *)tci.lParam;
	if ( pWnd )
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->EnableWindow(TRUE);
	}
}

//
void CFillEditDlg::CheckFastmark()
{
	CFastMarkStopDlg fastMark;
	/////
	if ( g_DAT.EnterMarkEnv() )
	{
		fastMark.DoModal();	
		g_DAT.LeaveMarkEnv();
	}
}



//
void CFillEditDlg::OnButtonFastmark()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;

	if ( !pDoc->IsSelectedObj() )
		return;
	
	g_DAT.SetLay(pDoc->GetLay());
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	CFastMarkStopDlg FastMark;
	if ( g_DAT.EnterMarkEnv() )
	{
		FastMark.DoModal();
		g_DAT.LeaveMarkEnv();
	}
}



//
void CFillEditDlg::DownOfflinedata()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	if ( !pDoc->IsSelectedObj() )
		return;
	
	int   nStatus = -1;
    UINT nRet = g_DAT.GetWorkStatus(&nStatus);
	if ( nRet  !=  HLOK )
		return;
	
	if ( 0 == nStatus )
	{
		if ( IDNO == AfxMessageBox(IDS_OFFLINESTATUS_ISREDOWN, MB_YESNO) )
			return;
		g_DAT.SetWorkStatus(1);
	}
	
	CFastMarkStopDlg FastMark;
	FastMark.m_bIsDownLoad = TRUE;
	
	g_DAT.SetLay( pDoc->GetLay() );
	if ( g_DAT.EnterMarkEnv() )
	{
		FastMark.DoModal();
		g_DAT.LeaveMarkEnv();
	}
}

//
void CFillEditDlg::OnButtonDownOfflinedata()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_DWONLOAD_MARKDATA , NULL);
}

//
void CFillEditDlg::GeneralPage(CNode *pNode)
{
	m_tab.DeleteAllItems();
	
	m_ObjFillEdit.ShowWindow(SW_HIDE);
	m_FillObjMarkSet.ShowWindow(SW_HIDE);
	
	m_TextEditDlg.ShowWindow(SW_HIDE);
	m_BarEditDlg.ShowWindow(SW_HIDE);
	m_ArcInfo.ShowWindow(SW_HIDE);
	m_CircleInfo.ShowWindow(SW_HIDE);
	
	m_DelayEditDlg.ShowWindow(SW_HIDE);
	m_InputEditDlg.ShowWindow(SW_HIDE);
	m_OutputEditDlg.ShowWindow(SW_HIDE);
	m_MotorMove.ShowWindow(SW_HIDE);
	m_MotorPos.ShowWindow(SW_HIDE);
	m_BufferLaser.ShowWindow(SW_HIDE);
	m_CCD.ShowWindow(SW_HIDE);
	m_PosCali.ShowWindow(SW_HIDE);
	
	m_tab.ShowWindow(SW_HIDE);
	
	//
	m_TextEditDlg.m_pNodeText = NULL;
	m_BarEditDlg.m_pNodeBar = NULL;
	m_ArcInfo.m_pNodeArc3Pt = NULL;
	m_CircleInfo.m_pNodeCircle = NULL;
	
	m_DelayEditDlg.m_pNode = NULL;
	m_InputEditDlg.m_pNode = NULL;
	m_OutputEditDlg.m_pNode = NULL;
	m_MotorMove.m_pNode = NULL;
	m_MotorPos.m_pNode = NULL;
	m_BufferLaser.m_pNode = NULL;
	m_CCD.m_pNode = NULL;
	m_PosCali.m_pNode = NULL;
	
	if ( NULL == pNode || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) ) 
		return;
	
	//
	if ( pNode->GetType() == NODE_BMP || pNode->GetType() == NODE_CONDITIONE  )
		return;
	
	m_tab.ShowWindow(SW_SHOW);
	
	CString str;
	TC_ITEM tabItem;
	tabItem.mask = TCIF_PARAM|TCIF_TEXT;
	tabItem.pszText = new TCHAR[255];
	if ( tabItem.pszText == NULL )
		return;

	memset(tabItem.pszText,0,sizeof(TCHAR)*255);
	switch( pNode->GetType() )
	{
		case NODE_MOTORMOVE:
			{
				str.LoadString(IDS_MOTORMOVE);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_MotorMove;
				m_tab.InsertItem(0,&tabItem);
				m_MotorMove.ShowWindow(SW_SHOW);			
				m_MotorMove.SetInfo(pNode);
			}
			break;
		case NODE_MOTORPOS:
			{
				str.LoadString(IDS_MOTORPOS);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_MotorPos;
				m_tab.InsertItem(0,&tabItem);
				m_MotorPos.ShowWindow(SW_SHOW);			
				m_MotorPos.SetInfo(pNode);
			}
			break;
		case NODE_BUFFERLASER:
			{
				str.LoadString(IDS_OBJINFO_BUFFERLASER);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_OutputEditDlg;
				m_tab.InsertItem(0,&tabItem);
				m_BufferLaser.ShowWindow(SW_SHOW);			
				m_BufferLaser.SetInfo(pNode);
			}
			break;
		case NODE_DELAY:
			{
				str.LoadString(IDS_DELAY);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_DelayEditDlg;
				m_tab.InsertItem(0,&tabItem);
				m_DelayEditDlg.ShowWindow(SW_SHOW);			
				m_DelayEditDlg.SetInfo(pNode);
			}
			break;
		case NODE_CCD:
			{
				str.LoadString(IDS_OBJINFO_CCD);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_OutputEditDlg;
				m_tab.InsertItem(0,&tabItem);
				m_CCD.ShowWindow(SW_SHOW);			
				m_CCD.SetInfo(pNode);
			}
			break;
		case NODE_POSCALI:
			{
				str.LoadString(IDS_OBJINFO_POSCALI);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_OutputEditDlg;
				m_tab.InsertItem(0,&tabItem);
				m_PosCali.ShowWindow(SW_SHOW);			
				m_PosCali.SetInfo(pNode);
			}
			break;

		case NODE_INPUT:
		case NODE_CONDITIONS:
			{
				str.LoadString(IDS_INPUT);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_InputEditDlg;
				m_tab.InsertItem(0,&tabItem);
				m_InputEditDlg.ShowWindow(SW_SHOW);
				m_InputEditDlg.SetInfo(pNode);
			}
			break;
			
		case NODE_CONDITIONE:
			break;

		case NODE_OUTPUT:
			{
				str.LoadString(IDS_OUTPUT);
				_tcscpy_s(tabItem.pszText,255,str);
				tabItem.lParam = (LPARAM)&m_OutputEditDlg;
				m_tab.InsertItem(0,&tabItem);
				m_OutputEditDlg.ShowWindow(SW_SHOW);			
				m_OutputEditDlg.SetInfo(pNode);
			}
			break;
			
		case NODE_ARC:
			{
				GeneralBasePage(FALSE);
				str.LoadString(IDS_OBJINFO_ARC);
				
				int nIndex = m_tab.GetItemCount();
				tabItem.lParam = (LPARAM)&m_ArcInfo;
				
				_tcscpy_s(tabItem.pszText,255,str);
				m_tab.InsertItem(nIndex,&tabItem);
				m_ArcInfo.ShowWindow(SW_HIDE);
				
				if ( m_nTabPrev < 0 || m_nObjPrev  !=  NODE_ARC )
					m_nTabPrev = nIndex;
				
				SetTable(m_nTabPrev);
				m_nTabIndex = m_nTabPrev;
				
				m_ArcInfo.GetArcInfo(pNode);
			}
			break;
			
		case NODE_CIRCLE:
			{
				GeneralBasePage(FALSE);
				//////////
				str.LoadString(IDS_OBJINFO_CIRCLE);
				tabItem.lParam = (LPARAM)&m_CircleInfo;
				_tcscpy_s(tabItem.pszText,255,str);
				
				int nIndex = m_tab.GetItemCount();
				m_tab.InsertItem(nIndex,&tabItem);
				m_CircleInfo.ShowWindow(SW_HIDE);

				if ( m_nTabPrev < 0  || m_nObjPrev  !=  NODE_CIRCLE  )
					m_nTabPrev = nIndex;
				
				SetTable(m_nTabPrev);
				m_nTabIndex = m_nTabPrev;
				m_CircleInfo.GetObjInfo(pNode);
			}
			break;

		case NODE_HLTEXT:
			{
				GeneralBasePage(FALSE);
				//
				tabItem.lParam = (LPARAM)&m_TextEditDlg;
				str.LoadString(IDS_TEXTTITLE);
				_tcscpy_s(tabItem.pszText,255,str);
				
				int nIndex = m_tab.GetItemCount();
				m_tab.InsertItem(nIndex,&tabItem);
				m_TextEditDlg.ShowWindow(SW_HIDE);
				
				if ( m_nTabPrev < 0  || m_nObjPrev  !=  NODE_HLTEXT )
					m_nTabPrev = nIndex;
				
				SetTable(m_nTabPrev);
				m_nTabIndex = m_nTabPrev;

				m_TextEditDlg.SetTextInfo(pNode);
			}
			break;
			
		case NODE_CODEBAR:
			{
				tabItem.lParam = (LPARAM)&m_FillObjMarkSet;
				str.LoadString(IDS_MARKTYPESET);
				_tcscpy_s(tabItem.pszText,255,str);
				m_tab.InsertItem(0,&tabItem);
				
				int nIndex = m_tab.GetItemCount();
				tabItem.lParam = (LPARAM)&m_BarEditDlg;
				
				str.LoadString(IDS_BARTITLE);
				_tcscpy_s(tabItem.pszText,255, str);
				m_tab.InsertItem(nIndex, &tabItem);
				m_BarEditDlg.ShowWindow(SW_HIDE);
				
				if ( m_nTabPrev < 0  || m_nObjPrev  !=  NODE_CODEBAR )
					m_nTabPrev = nIndex;
				
				SetTable(m_nTabPrev);
				m_nTabIndex = m_nTabPrev;
				
				m_BarEditDlg.SetTextInfo(pNode);
			}
			break;
		default:
			{
				GeneralBasePage(TRUE);
				break;
			}
	}
	m_nObjPrev = pNode->GetType();
	
	delete tabItem.pszText;
}

void CFillEditDlg::GeneralBasePage(BOOL bSelPage)
{
	TC_ITEM tabItem;
	tabItem.mask   = TCIF_PARAM|TCIF_TEXT;
	tabItem.pszText = new TCHAR[255];
	if ( tabItem.pszText == NULL )
		return;

	tabItem.lParam  = (LPARAM)&m_FillObjMarkSet;
	memset(tabItem.pszText,0,sizeof(TCHAR)*255);
	
	CString str;
	str.LoadString(IDS_MARKTYPESET);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(0,&tabItem);
	
	tabItem.lParam = (LPARAM)&m_ObjFillEdit;
	str.LoadString(IDS_MARKFILLEDIT);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(1,&tabItem);
	
	if ( bSelPage )
	{
		if ( m_nTabPrev < 0 || m_nTabPrev > 1 )
			m_nTabPrev = 1;
		
		SetTable(m_nTabPrev);
		m_nTabIndex = m_nTabPrev;
	}
	
	delete tabItem.pszText;
}
