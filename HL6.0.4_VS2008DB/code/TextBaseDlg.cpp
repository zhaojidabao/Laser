// TextBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextBaseDlg.h"
#include "baseMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextBaseDlg dialog
CTextBaseDlg::CTextBaseDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CTextBaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextBaseDlg)
	//}}AFX_DATA_INIT
	m_pNode = NULL;
}

void CTextBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextBaseDlg)
	DDX_Control(pDX, IDC_TAB_BASE, m_ctrlTabBase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextBaseDlg, CDialog)
	//{{AFX_MSG_MAP(CTextBaseDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BASE, OnSelchangeTabBase)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_BASE, OnSelchangingTabBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextBaseDlg message handlers

BOOL CTextBaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if ( NULL == m_pNode )
	{
		CDialog::OnCancel();
		return TRUE;
	}
	
	//生成标签
	CString str;
	TC_ITEM tabItem;
	tabItem.mask = TCIF_PARAM|TCIF_TEXT;
	tabItem.pszText = new TCHAR[255];
	///////
	if ( tabItem.pszText == NULL )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	str.LoadString(IDS_TEXTTITLE);
	_tcscpy_s(tabItem.pszText,255,str);	
	tabItem.lParam = (LPARAM)&m_TextPage;
	VERIFY(m_TextPage.Create(CTextPageDlg::IDD,&m_ctrlTabBase));
	m_ctrlTabBase.InsertItem(0,&tabItem);
	m_TextPage.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_TextPage.ShowWindow(SW_SHOW);
	m_TextPage.EnableWindow(TRUE);
	m_TextPage.UpdateData(FALSE);

	delete tabItem.pszText;

	Update_Data(FALSE);
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextBaseDlg::OnSelchangeTabBase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iTab = m_ctrlTabBase.GetCurSel();
	/////
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_ctrlTabBase.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	if ( pWnd )
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->EnableWindow(TRUE);
	}
	
	*pResult = 0;
}

void CTextBaseDlg::OnSelchangingTabBase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	TC_ITEM tci;

	int iTab = m_ctrlTabBase.GetCurSel();
	tci.mask = TCIF_PARAM;
	m_ctrlTabBase.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	if ( pWnd )
	{
		pWnd->ShowWindow(SW_HIDE);
		pWnd->EnableWindow(FALSE);
	}
	
	*pResult = 0;
}

void CTextBaseDlg::OnOK()
{
	Update_Data();
	///
	CDialog::OnOK();
}

void CTextBaseDlg::OnCancel()
{
	m_TextPage.Delete();
	////
	CDialog::OnCancel();
}

void CTextBaseDlg::Update_Data(BOOL bSaveAndValidate)
{
	if ( m_pNode == NULL )
		return;

	m_TextPage.Update_Data(bSaveAndValidate);

	if ( bSaveAndValidate )
	{
		int  i = 0;
		int  nOldNum  = m_pNode->GetTextCount();
		int  nNewNum = m_TextPage.m_list.GetCount();
		if ( nOldNum > nNewNum )
		{
			CUIntArray  arryID;
			for ( i = nNewNum; i < nOldNum; i++ )
			{
				CTextB *pOldText = m_pNode->GetText(i);
				if ( pOldText )
					arryID.Add(pOldText->GetTextID());
			}
			for ( i = 0; i < arryID.GetCount(); i++ )
				m_pNode->DelTextByID(arryID.GetAt(i));
			//////
			nOldNum  = nNewNum;
		}
		i = 0;
		POSITION pos = m_TextPage.m_list.GetHeadPosition();
		while ( pos )
		{
			CText *pText = m_TextPage.m_list.GetNext(pos);
			if ( pText == NULL )
				continue;
			//////
			if ( i < nOldNum )//原有Text内容替换
			{
				CTextB *pTextOld = m_pNode->GetText(i);
				if ( pTextOld )
					pTextOld->CopyExceptID(pText);
			}
			else//增加新Text
			{
				CText *pNew = new CText();
				if ( pNew  !=  NULL )
				{
					pNew->CopyExceptID(pText);
					m_pNode->AddTextAll_NewID(pNew);//向MgrText和Nodecodebar中同时添加TEXT,重新分配ID
				}
			}
			i++;
		}

		//字体设置部分
		m_TextPage.m_PageFnt.UpdateData(TRUE);
		m_pNode->m_nFont				 =  m_TextPage.m_PageFnt.m_nFont;			
		m_pNode->m_dbCharSpace	 =  m_TextPage.m_PageFnt.m_dbCharSpace;	
		m_pNode->m_dbWidthFactor	 =  m_TextPage.m_PageFnt.m_dbWidthFactor/100;	
		m_pNode->m_dbHeight			 =  m_TextPage.m_PageFnt.m_dbHeight;		
		m_pNode->m_dbLineSpace		 =  m_TextPage.m_PageFnt.m_dbLineSpace;	
		m_pNode->m_strFontCHS		 =  m_TextPage.m_PageFnt.m_strFontCHS;		
		m_pNode->m_strFontEN		 =  m_TextPage.m_PageFnt.m_strFontEN;		
		m_pNode->m_strFontNUM		 =  m_TextPage.m_PageFnt.m_strFontNUM;		
		m_pNode->m_strFontUser		 =  m_TextPage.m_PageFnt.m_strFontUser;	
		m_pNode->m_bFixSpace		 =  m_TextPage.m_PageFnt.m_bFixSpace;	
		m_pNode->m_dbFixSpace		 =  m_TextPage.m_PageFnt.m_dbFixSpace;	
		m_pNode->m_lf					 =  m_TextPage.m_PageFnt.m_lf;				
		m_pNode->m_strPrecisK		 =  m_TextPage.m_PageFnt.m_strPrecisK;	
		m_pNode->m_strHzkType		 =  m_TextPage.m_PageFnt.m_strHzkType;	
		m_pNode->m_strAscType		 =  m_TextPage.m_PageFnt.m_strAscType;	

		// add by zhq,增加字体自动焊接和过滤交点选项
		m_pNode->m_bWeld				 = 	m_TextPage.m_PageFnt.m_bWeld;
		m_pNode->m_bInterFilter		 = 	m_TextPage.m_PageFnt.m_bInterFilter;
		m_pNode->m_dbInterSpace	 = 	m_TextPage.m_PageFnt.m_dbInterSpace;

		// 加粗
		m_pNode->m_bBroaden			= 	m_TextPage.m_PageFnt.m_bBroaden;
		m_pNode->m_dbLineWidth		= 	m_TextPage.m_PageFnt.m_dbLineWidth;
		m_pNode->m_dbBroadenD		= 	m_TextPage.m_PageFnt.m_dbBroadenD;

		//排布设置部分
		m_TextPage.m_PageRound.UpdateData(TRUE);
		m_pNode->m_nPath				 =  m_TextPage.m_PageRound.m_nPath;		
		m_pNode->m_dbRadius			 =  m_TextPage.m_PageRound.m_dbRadius;		
		m_pNode->m_dbStart			 =  m_TextPage.m_PageRound.m_dbStart;		
		m_pNode->m_bCCW				 =  m_TextPage.m_PageRound.m_bCCW;		
		m_pNode->m_bInside			 =  m_TextPage.m_PageRound.m_bInside;		
		m_pNode->m_nAlign				 =  m_TextPage.m_PageRound.m_nAlign;		
		m_pNode->m_dbDirection		 =  m_TextPage.m_PageRound.m_dbDirection;	
		m_pNode->m_bCustomFormat	 =  m_TextPage.m_PageRound.m_bCustomFormat;	
		m_pNode->m_strFormat		 =  m_TextPage.m_PageRound.m_strFormat;	
		m_pNode->m_bPrintInverse	 =  m_TextPage.m_PageRound.m_bPrintInverse;	

		m_pNode->m_dbMatrixR		 = 	m_TextPage.m_PageFnt.m_dbMatrixR;
		m_pNode->m_dbMatrixFillD	 = 	m_TextPage.m_PageFnt.m_dbMatrixFillD;
	}
	else
	{
		//文本应用
		m_TextPage.Delete();
		for (UINT iText = 0; iText < m_pNode->GetTextCount(); iText++) 
		{
			CTextB *pText = (CTextB*)m_pNode->GetText(iText);
			if ( NULL == pText )
				continue;

			CText *pNew = new CText();
			if ( NULL == pNew ) 
				continue;
	
			pNew->CopyAll(pText);
			m_TextPage.m_list.AddTail(pNew);
		}
		m_TextPage.ListFlash();
		m_TextPage.m_listData.SetCurSel(0);
		m_TextPage.Update_Data(FALSE);

		//字体设置部分
		m_TextPage.m_PageFnt.m_nFont				 =  m_pNode->m_nFont;
		m_TextPage.m_PageFnt.m_dbCharSpace		 =  m_pNode->m_dbCharSpace;
		m_TextPage.m_PageFnt.m_dbWidthFactor	 =  m_pNode->m_dbWidthFactor*100;
		m_TextPage.m_PageFnt.m_dbHeight			 =  m_pNode->m_dbHeight;
		m_TextPage.m_PageFnt.m_dbLineSpace		 =  m_pNode->m_dbLineSpace;
		m_TextPage.m_PageFnt.m_strFontCHS			 =  m_pNode->m_strFontCHS;		
		m_TextPage.m_PageFnt.m_strFontEN			 =  m_pNode->m_strFontEN;	
		m_TextPage.m_PageFnt.m_strFontNUM		 =  m_pNode->m_strFontNUM;
		m_TextPage.m_PageFnt.m_strFontUser		 =  m_pNode->m_strFontUser;
		m_TextPage.m_PageFnt.m_bFixSpace			 =  m_pNode->m_bFixSpace;
		m_TextPage.m_PageFnt.m_dbFixSpace			 =  m_pNode->m_dbFixSpace;
		m_TextPage.m_PageFnt.m_lf						 =  m_pNode->m_lf;
		m_TextPage.m_PageFnt.m_strPrecisK			 =  m_pNode->m_strPrecisK;
		m_TextPage.m_PageFnt.m_strHzkType			 =  m_pNode->m_strHzkType;
		m_TextPage.m_PageFnt.m_strAscType			 =  m_pNode->m_strAscType;	
		//add by zhq,增加字体自动焊接和过滤交点选项
		m_TextPage.m_PageFnt.m_bWeld				 =  m_pNode->m_bWeld;
		m_TextPage.m_PageFnt.m_bInterFilter			 =  m_pNode->m_bInterFilter;
		m_TextPage.m_PageFnt.m_dbInterSpace			 =  m_pNode->m_dbInterSpace;

		// 加粗
		m_TextPage.m_PageFnt.m_bBroaden				= m_pNode->m_bBroaden;
		m_TextPage.m_PageFnt.m_dbLineWidth			= m_pNode->m_dbLineWidth;
		m_TextPage.m_PageFnt.m_dbBroadenD			= m_pNode->m_dbBroadenD;

		m_TextPage.m_PageFnt.m_dbMatrixR			 =  m_pNode->m_dbMatrixR;
		m_TextPage.m_PageFnt.m_dbMatrixFillD		 =  m_pNode->m_dbMatrixFillD;

		m_TextPage.m_PageFnt.UpdateData(FALSE);
		m_TextPage.m_PageFnt.UpdateDataView();
		//排布设置部分
		m_TextPage.m_PageRound.m_nPath				 =  m_pNode->m_nPath;
		m_TextPage.m_PageRound.m_dbRadius			 =  m_pNode->m_dbRadius;
		m_TextPage.m_PageRound.m_dbStart			 =  m_pNode->m_dbStart;
		m_TextPage.m_PageRound.m_bCCW			 =  m_pNode->m_bCCW;
		m_TextPage.m_PageRound.m_bInside			 =  m_pNode->m_bInside;
		m_TextPage.m_PageRound.m_nAlign			=  m_pNode->m_nAlign;
		m_TextPage.m_PageRound.m_dbDirection	=  m_pNode->m_dbDirection;
		m_TextPage.m_PageRound.m_bCustomFormat	=  m_pNode->m_bCustomFormat;
		m_TextPage.m_PageRound.m_strFormat		=  m_pNode->m_strFormat;
		m_TextPage.m_PageRound.m_bPrintInverse	=  m_pNode->m_bPrintInverse;
		m_TextPage.m_PageRound.UpdateData(FALSE);
		m_TextPage.m_PageRound.UpdateDataView();
	}
}

