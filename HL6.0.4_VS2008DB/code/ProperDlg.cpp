// ProperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ProperDlg.h"
#include "HLView.h"
#include "STYLESAVEDLG.H"
#include "LocNukeForHL.h"
#include "MainFrm.h"
#include "baseMacro.h"
#include "DlgLaserDot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProperDlg dialog
#define MINSPEED 0.01
#define FPK_ADVPARA

#include "resource.h"
CProperDlg::CProperDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CProperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProperDlg)
	m_bSelMark = FALSE;
	m_bDefault = FALSE;
	m_nItem = 0;
	//}}AFX_DATA_INIT
}

void CProperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProperDlg)
	DDX_Control(pDX, IDC_TAB_MARKPARA, m_tabMarkPara);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_style);
	DDX_Control(pDX, IDC_LIST_PARA,  m_listPara);
    DDX_Control(pDX, IDC_LIST_LAYER, m_listLayer);
    DDX_Control(pDX, IDC_BUTTON_LAYER1, m_layerColorBt0);
    DDX_Control(pDX, IDC_BUTTON_LAYER2, m_layerColorBt1);
    DDX_Control(pDX, IDC_BUTTON_LAYER3, m_layerColorBt2);
    DDX_Control(pDX, IDC_BUTTON_LAYER4, m_layerColorBt3);
    DDX_Control(pDX, IDC_BUTTON_LAYER5, m_layerColorBt4);
    DDX_Control(pDX, IDC_BUTTON_LAYER6, m_layerColorBt5);
    DDX_Control(pDX, IDC_BUTTON_LAYER7, m_layerColorBt6);
	DDX_Check(pDX, IDC_CHECK_SEL_MARK, m_bSelMark);
	DDX_Check(pDX, IDC_CHECK_DEFAULT, m_bDefault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProperDlg, CDialog)
	//{{AFX_MSG_MAP(CProperDlg)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LAYER, OnRclickListLayer)
	ON_COMMAND(ID_MOD_LAYER_COLOR, OnModLayerColor)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LAYER, OnClickListLayer)
	ON_BN_CLICKED(IDC_BUTTON_LAYER1, OnButtonLayer1)
	ON_BN_CLICKED(IDC_BUTTON_LAYER2, OnButtonLayer2)
	ON_BN_CLICKED(IDC_BUTTON_LAYER3, OnButtonLayer3)
	ON_BN_CLICKED(IDC_BUTTON_LAYER4, OnButtonLayer4)
	ON_BN_CLICKED(IDC_BUTTON_LAYER5, OnButtonLayer5)
	ON_BN_CLICKED(IDC_BUTTON_LAYER6, OnButtonLayer6)
	ON_BN_CLICKED(IDC_BUTTON_LAYER7, OnButtonLayer7)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER1, OnDoubleclickedButtonLayer1)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER2, OnDoubleclickedButtonLayer2)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER4, OnDoubleclickedButtonLayer4)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER3, OnDoubleclickedButtonLayer3)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER5, OnDoubleclickedButtonLayer5)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER6, OnDoubleclickedButtonLayer6)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER7, OnDoubleclickedButtonLayer7)
	ON_BN_CLICKED(IDC_CHECK_SEL_MARK, OnCheckSelMark)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT, OnCheckDefault)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnButtonErase)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnSelchangeComboStyle)
	ON_COMMAND(ID_LOCK_LAYER,  OnLockLayer)
	ON_COMMAND(ID_UNLOCK_LAYER,  OnUNLockLayer)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MARKPARA, OnSelchangeTabMarkpara)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_COMMAND(ID_SELECT_LAYER, OnSelectLayer)
	ON_WM_KEYDOWN()
	ON_WM_CANCELMODE()
	ON_COMMAND(ID_UNSELECT_LAYER, OnUnselectLayer)
	//}}AFX_MSG_MAP
	ON_NOTIFY(PROPERTY_CHANGE,IDC_LIST_PARA,OnParaChange)
    ON_NOTIFY(SEL_VALUE_CHANGE,IDC_LIST_PARA,OnParaListChange)
	ON_LBN_SELCHANGE(IDC_LIST_PARA, &CProperDlg::OnLbnSelchangeListPara)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LAYER, &CProperDlg::OnLvnItemchangedListLayer)
	ON_BN_CLICKED(IDC_BUTTON_LASERDOT, &CProperDlg::OnBnClickedButtonLaserdot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProperDlg message handlers

BOOL CProperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	InitLayer();
	InitMarkPara();
	for( int iPage = 0; iPage < g_DAT.m_arrParaCfg.GetSize(); ++iPage )
	{
		PARA_PAGE& page = g_DAT.m_arrParaCfg.ElementAt(iPage);
		TC_ITEM tabItem;	
		tabItem.mask = TCIF_TEXT;
		tabItem.pszText = (LPTSTR)(LPCTSTR)page.m_strName;
		m_tabMarkPara.InsertItem(iPage, &tabItem);
	}
	
	SetParaListShow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CProperDlg::InitLayer()
{
    m_listLayer.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_SHAREIMAGELISTS|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	
	CString strNum,strColor,strSpeed;
	CString strQ,strQRls,strHead;
	strNum.LoadString(IDS_MARKSET_NUM);
	strColor.LoadString(IDS_LAYER_COLOR);
	strSpeed.LoadString(IDS_SPEED);
	strQ.LoadString(IDS_Q_FREQ);
	strQRls.LoadString(IDS_Q_RLS);
	//
	strHead.Format(_T("%s,15;%s,40;%s,40;%s,44;%s,48"),strNum,strColor,strSpeed,strQ,strQRls);
	m_listLayer.SetHeadings(strHead);
	
	m_layerColorBt0.Attach( RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128) );
	m_layerColorBt1.Attach( RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128) );
	m_layerColorBt2.Attach( RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128) );
	m_layerColorBt3.Attach( RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128) );
	m_layerColorBt4.Attach( RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128) );
	m_layerColorBt5.Attach( RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128) );
	m_layerColorBt6.Attach( RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128) );
	
	HICON hIcon[2];
    m_ImageList.Create(12, 12, ILC_MASK, 12, 0);
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON_LOCK);    // 未锁定
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON_UNLOCK);// 锁定
	
	m_ImageList.Add(hIcon[0]);
	m_ImageList.Add(hIcon[1]);

	m_listLayer.SetImageList(&m_ImageList, LVSIL_SMALL);

	return TRUE;
}



BOOL CProperDlg::OnNewDocument(CLay &lay)
{
	m_listLayer.DeleteAllItems();
	int nLayCount = lay.GetSize();
	for( int j = 0; j < g_SysCfg.m_nLayerCnt; j++ )
	{
		CLayer layer  = lay.GetLayer(j);
		TCHAR szSpeed[20] = { 0 };
		TCHAR szQF[16] = { 0 };
		TCHAR szNum[8] = { 0 };
		TCHAR szW[16] = { 0 };
		if ( EMCC == g_nCardType )
		{
			double dbSpeed = layer.m_para.emcc.dbMarkV;
			_stprintf_s(szSpeed,	20,_T("%0.0f"),  dbSpeed);
			_stprintf_s(szQF,16,		_T("%0.0f"),  layer.m_para.emcc.dbQFreq);
			_stprintf_s(szW,16,		_T("%0.0f"),  layer.m_para.emcc.dbQRls);
		}
		
		_stprintf_s( szNum, 8,_T("%d"), j );
		m_listLayer.AddItem( szNum, _T(""), szSpeed, szQF, szW );
	}
	
	for( int i = 0; i < g_SysCfg.m_nLayerCnt; i++ )
	{
		CLayer layer = lay.GetLayer(i);
		m_listLayer.SetItemColor(i, 1, 0, layer.m_color);
	    switch(i)
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, TRUE);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color);
			break;
		default:
			break;
		}
	}
	
	SetFillObjMarkSet(lay);
	
	m_style.ResetContent();
	CString searchFile;
	if ( EMCC == g_nCardType )
	{
		searchFile.Format(_T("%s*.%s"), g_sys.GetInstallPath(), g_DAT.m_strEMCCLaser);
	}
	else
	{
		searchFile = m_strPath + _T("*.yh");
	}
	
	CString yh("");
	CFileFind finder;
	BOOL bWorking = finder.FindFile(searchFile);
	while( bWorking )
	{
		bWorking = finder.FindNextFile();
		yh = finder.GetFileName();
		yh = yh.Left(yh.ReverseFind('.'));
		m_style.AddString(yh);
	}
	
	m_style.SetCurSel(-1);
	m_bDefault = FALSE;
	m_listLayer.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_listLayer.SetCurSel(0);
	m_nItem = 0;

	// modify by zhq,2014-07-10
	//NMHDR  hDr;
	//OnLayerSelChange(&hDr,NULL,FALSE);

	OnLayerSelChange(NULL,NULL,FALSE);
	Invalidate();
	
	return TRUE;
}



//
BOOL CProperDlg::UpdateProperty()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return FALSE;
	
	m_bSelMark = pDoc->m_chain.GetMarkSel();
	UpdateData(FALSE);

	CString searchFile;
	m_style.ResetContent();
	if ( EMCC == g_nCardType ) 
	{
		searchFile.Format(_T("%s*.%s"), g_sys.GetInstallPath(), g_DAT.m_strEMCCLaser);
	}
	else
	{
		searchFile = m_strPath+"*.yh";
	}
	
	CString yh;
	CFileFind finder;
    BOOL bWorking = finder.FindFile(searchFile);
    while ( bWorking )
    {
        bWorking = finder.FindNextFile();
		yh = finder.GetFileName();
		yh = yh.Left(yh.ReverseFind('.'));
		m_style.AddString(yh);
	}
	
	m_style.SetCurSel(-1);
	
	m_bDefault = FALSE;
	// modify by zhq,2014-07-10
	//NMHDR  hDr;
	//OnLayerSelChange(&hDr,NULL,FALSE);
	OnLayerSelChange(NULL,NULL,FALSE);
	SetParaListShow();
	
    GetDlgItem(IDC_LIST_LAYER)->EnableWindow(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET);
	GetDlgItem(IDC_LIST_PARA)->EnableWindow(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET);
	GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET);
	GetDlgItem(IDC_BUTTON_ERASE)->EnableWindow(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET);
	GetDlgItem(IDC_BUTTON_DEFAULT)->EnableWindow(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET);
	GetDlgItem(IDC_CHECK_DEFAULT)->EnableWindow(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET);
	
	return TRUE;
}


void CProperDlg::OnRclickListLayer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	//快速排序时不允许对层编辑
	if ( pDoc->m_bPathOptimize )
		return;
	
	HitTestEx(&m_listLayer, pNMHDR, m_nItem, m_nSubItem);
	BOOL bIsDrawMark = FALSE;
	CLay lay = pDoc->GetLay();
	int nIndex = m_listLayer.GetTopIndex();
	for( int i = nIndex; i < nIndex + 7; i++ )
	{
		if ( m_nItem == i )
		{
			bIsDrawMark = TRUE;
		}
		else
		{
			bIsDrawMark = FALSE;
		}
		
		CLayer layer = lay.GetLayer(i);
		int nSetIndex = i - nIndex;
	    switch( nSetIndex )
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		default:
			break;
		}
	}
	
	int index = pDoc->GetMenuIndex();
	CMenu menu,*pMenu = NULL;
	menu.LoadMenu(IDR_POP);
	pMenu = menu.GetSubMenu(5);
	if ( pMenu )
	{
		NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	    POINT point = pNMListView->ptAction;
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	
	g_sys.m_bEnableUpdate = FALSE;
	SetFocus();
	
	*pResult = 0;
}




void CProperDlg::SetButtonColor( int nIndex, COLORREF color ) 
{
	int nTopIndex = m_listLayer.GetTopIndex();
	int nSetIndex = nIndex - nTopIndex;
	switch( nSetIndex )
	{
	case 0:
		m_layerColorBt0.SetBGColor(color, TRUE);
		break;
	case 1:
		m_layerColorBt1.SetBGColor(color, TRUE);
		break;
	case 2:
		m_layerColorBt2.SetBGColor(color, TRUE);
		break;
	case 3:
		m_layerColorBt3.SetBGColor(color, TRUE);
		break;
	case 4:
		m_layerColorBt4.SetBGColor(color, TRUE);
		break;
	case 5:
		m_layerColorBt5.SetBGColor(color, TRUE);
		break;
	case 6:
		m_layerColorBt6.SetBGColor(color, TRUE);
		break;
	default:
		break;
	}
}


void CProperDlg::OnModLayerColor() 
{
	CColorDialog colorDlg;
	if ( IDOK == colorDlg.DoModal() )
	{
		COLORREF selColor  = colorDlg.GetColor();
		
		CHLDoc *pDoc = (CHLDoc*)GetCurActiveDocument();
		if ( NULL == pDoc )
			return;
		
		pDoc->SetLayerColor(m_nItem, selColor);
		m_listLayer.SetItemColor(m_nItem,1,0,selColor);
		int nIndex = m_listLayer.GetTopIndex();
		int nNum   = m_nItem - nIndex;
		
		ClickColorButton(nNum);
		OnLayerColorChange(m_nItem, selColor);
	}
}

void CProperDlg::OnLayerColorChange( int nIndex, COLORREF color )
{
	CHLDoc *pDoc = (CHLDoc*)GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	//层更新
	CLayer ly = pDoc->GetLayer(nIndex);
	ly.m_color = color;
	
	//更新文档中相应层及其图形对象
	pDoc->LayChange(PTY_LAYER, ly);
	pDoc->UpdateAllViews(NULL);
}


BOOL CProperDlg::HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem, int &nSubItem)
{
    if ( pCtrl  == NULL ) return FALSE;
	if ( pNMHDR == NULL ) return FALSE;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	nItem    = pCtrl->HitTest( point, NULL);
	nSubItem = -1;
	m_listLayer.SetCurSel(nItem);
	
	return TRUE;
}


//
void CProperDlg::OnClickListLayer( NMHDR* pNMHDR, LRESULT* pResult )
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) )
		return;
	
	CHLDoc *pDoc = (CHLDoc*)GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	HitTestEx( &m_listLayer, pNMHDR, m_nItem, m_nSubItem );
	BOOL bIsDrawMark = FALSE;
	int  nIndex = m_listLayer.GetTopIndex();
	for( int i = nIndex; i < nIndex + 7; i++ )
	{
		if ( m_nItem == i )
		{
			bIsDrawMark = TRUE;
		}
		else
		{
			bIsDrawMark = FALSE;
		}
		
        CLayer layer = pDoc->GetLayer(i);
		int nSetIndex = i - nIndex;
	    switch( nSetIndex )
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		default:
			break;
		}
	}
	
	OnLayerSelChange(pNMHDR, pResult);
	
	*pResult = 0;
}

void CProperDlg::ClickColorButton(int nNum) 
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	CLay lay = pDoc->GetLay();
	int nIndex = m_listLayer.GetTopIndex();
	
	m_nItem = nIndex + nNum;	
	m_listLayer.SetItemState(m_nItem, LVIS_SELECTED, LVIS_SELECTED);
	
	if ( m_listLayer.GetCurSel() == m_nItem )
		return;
	
	m_listLayer.SetCurSel(m_nItem);
	CLayer layer = lay.GetLayer(m_nItem);
	BOOL bIsDrawMark = FALSE;
	for( int i = nIndex; i < nIndex + 7; i++ ) 
	{
		if ( nIndex + nNum == i )
		{
			bIsDrawMark = TRUE;
		}
		else
		{
			bIsDrawMark = FALSE;
		}
		
		CLayer layer = lay.GetLayer(i);
		int nSetIndex = i - nIndex;
	    switch( nSetIndex )
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		default:
			break;
		}
	}

	OnLayerSelChange(NULL, NULL);
}

//
void CProperDlg::OnDoubleclickedColorButton() 
{	
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) )
		return;

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	if ( pDoc->GetStatus() == CChain::selectNull )
	{
		AfxMessageBox(IDS_NOOBJECT,MB_ICONWARNING);
		return;
	}
	
	BOOL bIsColorToFill = ((CButton*)GetDlgItem(IDC_CHECK_FILL_COLOR))->GetCheck();
	CLayer layer = pDoc->GetLayer(m_nItem);
	if ( bIsColorToFill )
	{
		pDoc->LayChange(PTY_FILL, pDoc->GetLayer(m_nItem));
	}
	else
	{
	    pDoc->LayChange(PTY_OUTLINE, pDoc->GetLayer(m_nItem));
	}
	
	((CButton*)GetDlgItem(IDC_CHECK_FILL_COLOR))->SetCheck(FALSE);
	
	pDoc->UpdateAllViews(NULL);
}


void CProperDlg::OnButtonLayer1() 
{
	ClickColorButton(0);
}


void CProperDlg::OnDoubleclickedButtonLayer1() 
{
	OnDoubleclickedColorButton();
}

void CProperDlg::OnButtonLayer2() 
{
	ClickColorButton(1);
}

void CProperDlg::OnDoubleclickedButtonLayer2() 
{
	OnDoubleclickedColorButton();
}

void CProperDlg::OnButtonLayer3() 
{
	ClickColorButton(2);
}

void CProperDlg::OnDoubleclickedButtonLayer3() 
{
	OnDoubleclickedColorButton();
}

void CProperDlg::OnButtonLayer4() 
{
	ClickColorButton(3);
}

void CProperDlg::OnDoubleclickedButtonLayer4() 
{
	OnDoubleclickedColorButton();
}

void CProperDlg::OnButtonLayer5() 
{
	ClickColorButton(4);
}

void CProperDlg::OnDoubleclickedButtonLayer5() 
{
	OnDoubleclickedColorButton();
}


void CProperDlg::OnButtonLayer6() 
{
	ClickColorButton(5);
}

void CProperDlg::OnDoubleclickedButtonLayer6() 
{
	OnDoubleclickedColorButton();
}

void CProperDlg::OnButtonLayer7() 
{
	ClickColorButton(6);
}

void CProperDlg::OnDoubleclickedButtonLayer7() 
{
	OnDoubleclickedColorButton();
}

void CProperDlg::OnCheckSelMark() 
{
	UpdateData(TRUE);

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	pDoc->SetSelectedObjbeMarked(m_bSelMark);
}

void CProperDlg::OnCheckDefault() 
{
	UpdateData(TRUE);

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	int index = m_listLayer.GetCurSel();
	if ( index == LB_ERR )
		return;
	
	CLayer ly = pDoc->GetLayer(index);
	ly.m_bDefault = m_bDefault;
	if ( ly.m_bDefault )
	{
		CLayer tmp = pDoc->GetLayer();
		ly.m_para = tmp.m_para;
	}
	//更新文档中相应层
	pDoc->LayChange(PTY_PARA,ly);
	
	NMHDR  hDr;
	OnLayerSelChange(&hDr,NULL);
}

void CProperDlg::OnLayerSelChange(NMHDR* pNMHDR, LRESULT* pRes,BOOL  bUnDo)
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) )
		return;
	
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	int index = m_listLayer.GetCurSel();
	if ( index == LB_ERR )
		return;
	
	CLayer ly = pDoc->GetLayer(index);
	
	//第0层的处理
	if ( index == 0 )
	{
		GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_HIDE);
		m_bDefault = FALSE;
	}
	else
	{
		GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_SHOW);
		m_bDefault = ly.m_bDefault;
	}
	
	if ( pNMHDR )
	{
		//非内部更新,加入命令处理器
		ly.m_nLayer = index;
		if ( bUnDo )
			pDoc->LayChange(PTY_CHANGE,ly);
 		else
 			pDoc->SetLayCmd(PTY_CHANGE,ly);
	}

	
	//更新打标参数列表
	SetPara(ly.m_para);
}

void CProperDlg::SetPara(PARA &para)
{
	int nCount = 0;
	CString str;
	switch( g_nCardType )
	{
	case EMCC:
		{
			for( int iPage = 0; iPage < m_arEmccPara.GetSize(); ++iPage )
			{
				PAGE_ITEM* parItem = m_arEmccPara.ElementAt(iPage);
				for( int iPara = 0; iPara < parItem->GetSize(); ++iPara ) 
				{
					CPropertyItem *pItem = parItem->ElementAt(iPara);
					if (_T("CNT") == pItem->m_strAlias)
					{
						str.Format(_T("%d"),para.emcc.nCount);
					}
					else if (_T("ASV") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbMarkV);					
					}
					else if (_T("ESV") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbJumpV);
					}
					else if (_T("QSF") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbQFreq);
					}
					else if (_T("QRT") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbQRls);
					}
					else if (_T("WAT") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbCurrent);
					}
					else if (_T("FPT") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nFpkTime);
					}
					else if (_T("FIP") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbFPKInitRate);
					}
					else if (_T("FAP") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbFPKLenRate);
					}
					else if (_T("FMP") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbFPKMaxPRate);
					}
					else if (_T("FCR") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbFPKCurRate);
					}
					else if (_T("MQR") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nQuality);
					}
					else if (_T("LDT") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nLayerDelay);
					}
					else if (_T("ODL") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nLaserOnDelay);
					}
					else if (_T("FDL") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nLaserOffDelay);
					}
					else if (_T("MLT") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nMoveLineDelay);
					}
					else if (_T("JDL") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nJumpDelay);
					}
					else if (_T("MDL") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nRoundDelay);
					}
					else if (_T("CUR") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbCurrentR);
					}
					else if (_T("SMC") == pItem->m_strAlias || _T("SMP") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbSimmerCur);
					}
					else if (_T("WAF") == pItem->m_strAlias || _T("PLS") == pItem->m_strAlias) {
						str.Format(_T("%d"),para.emcc.nWaveForm);
					}
					else if (_T("POW") == pItem->m_strAlias) {
						str.Format(_T("%.3f"),para.emcc.dbPowerR);
					}
					
					pItem->m_curValue = str;
				}
			}

			break;
		}
	default:
		{
			break;
		}
	}

	TCHAR szSpeed[20] = {0};
	TCHAR szQF[16] = {0};
	TCHAR szW[16] = {0};
	if ( EMCC == g_nCardType )
	{
		double dbSpeed = para.emcc.dbMarkV;
		_stprintf_s(szSpeed,20,	_T("%0.0f"),  dbSpeed);
		_stprintf_s(szQF,16,		_T("%0.1f"),  para.emcc.dbQFreq);
		_stprintf_s(szW,16,		_T("%0.1f"),  para.emcc.dbQRls);
	}

	int index = m_listLayer.GetCurSel();
	m_listLayer.SetItemText(index, 2, szSpeed);
	m_listLayer.SetItemText(index, 3, szQF);
	m_listLayer.SetItemText(index, 4, szW);
	
	UpdateView();
}



void CProperDlg::UpdateView()
{
	m_listPara.Invalidate();
	
	if ( m_bDefault )
	{
		m_listPara.SetCurSel(-1);
	}

	GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(!m_bDefault);
	GetDlgItem(IDC_LIST_PARA)->EnableWindow(!m_bDefault);
	UpdateData(FALSE);
}



void CProperDlg::GetPara(PARA &para)
{
	CString   str;
	int   nCount = 0;
	////////////////////////
	if ( EMCC == g_nCardType )
	{
		for( int iPage = 0; iPage < m_arEmccPara.GetSize(); ++iPage ) 
		{
			PAGE_ITEM* parItem = m_arEmccPara.ElementAt(iPage);
			for( int iPara = 0; iPara < parItem->GetSize(); ++iPara ) 
			{
				CPropertyItem *pItem = parItem->ElementAt(iPara);
				if ( NULL == pItem )
					continue;
				
				str = pItem->m_curValue;
				if ( _T("CNT") == pItem->m_strAlias ) {
					para.emcc.nCount = _ttoi(str);
				}else if ( _T("ASV") == pItem->m_strAlias ) {
					para.emcc.dbMarkV = _tstof(str);					
				}else if ( _T("ESV") == pItem->m_strAlias ) {
					para.emcc.dbJumpV = _tstof(str);
				}else if ( _T("QSF") == pItem->m_strAlias ) {
					para.emcc.dbQFreq = _tstof(str);
				}else if ( _T("QRT") == pItem->m_strAlias ) {
					para.emcc.dbQRls = _tstof(str);
				}else if ( _T("WAT") == pItem->m_strAlias ) {
					para.emcc.dbCurrent = _tstof(str);
				}else if ( _T("FPT") == pItem->m_strAlias ) {
					para.emcc.nFpkTime = _ttoi(str);
				}else if ( _T("FIP") == pItem->m_strAlias ) {
					para.emcc.dbFPKInitRate = _tstof(str);
				}else if ( _T("FAP") == pItem->m_strAlias ) {
					para.emcc.dbFPKLenRate = _tstof(str);
				}else if ( _T("FMP") == pItem->m_strAlias ) {
					para.emcc.dbFPKMaxPRate = _tstof(str);
				}else if ( _T("FCR") == pItem->m_strAlias ) {
					para.emcc.dbFPKCurRate = _tstof(str);
				}else if ( _T("MQR") == pItem->m_strAlias ) {
					para.emcc.nQuality = _ttoi(str);
				}else if ( _T("LDT") == pItem->m_strAlias ) {
					para.emcc.nLayerDelay = _ttoi(str);
				}else if ( _T("ODL") == pItem->m_strAlias ) {
					para.emcc.nLaserOnDelay = _ttoi(str);
				}else if ( _T("FDL") == pItem->m_strAlias ) {
					para.emcc.nLaserOffDelay = _ttoi(str);
				}else if ( _T("MLT") == pItem->m_strAlias ) {
					para.emcc.nMoveLineDelay = _ttoi(str);
				}else if ( _T("JDL") == pItem->m_strAlias ) {
					para.emcc.nJumpDelay = _ttoi(str);
				}else if ( _T("MDL") == pItem->m_strAlias ) {
					para.emcc.nRoundDelay = _ttoi(str);
				}else if ( _T("CUR") == pItem->m_strAlias ) {
					para.emcc.dbCurrentR = _tstof(str);
				}else if ( _T("SMC") == pItem->m_strAlias || _T("SMP") == pItem->m_strAlias) {
					para.emcc.dbSimmerCur = _tstof(str);
				}else if ( _T("WAF") == pItem->m_strAlias || _T("PLS") == pItem->m_strAlias ) {
					para.emcc.nWaveForm = _ttoi(str);
				}else if ( _T("POW") == pItem->m_strAlias ) {
					para.emcc.dbPowerR = _tstof(str);
				}			
			}
		}
	}
}

void CProperDlg::OnButtonSave() 
{
	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CStyleSaveDlg dlg;
	if ( dlg.DoModal() == IDOK )
	{
		int index = m_style.FindStringExact(-1,dlg.m_strStyle);
		if ( index != CB_ERR )
		{	
			if ( AfxMessageBox(IDS_YH_DULNAME,MB_YESNO)  !=  IDYES )
				return;
		}

		CString strStyle = dlg.m_strStyle;
		CString filename;

		if ( EMCC == g_nCardType ) 
			filename.Format(_T("%s%s.%s"), g_sys.GetInstallPath(), strStyle, g_DAT.m_strEMCCLaser);
		else
			filename = m_strPath + strStyle + _T(".yh");

		FILE *fp = NULL;
		if ( ( fp = _tfopen(filename,_T("wb")) ) == NULL )
		{	
			AfxMessageBox(IDS_YH_ADD);
		}
		else
		{
			if ( index == CB_ERR )
			{
				m_style.AddString(dlg.m_strStyle);
			}
			m_style.SelectString(-1, dlg.m_strStyle);

			CHLDoc *pDoc = GetCurActiveDocument();
			CLay& lay = pDoc->GetLay();
			lay.Serialize(fp, TRUE, g_nCardType,TRUE);
			fclose(fp);
		}
	}
}

void CProperDlg::OnButtonErase() 
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) )
		return;

	int index = m_style.GetCurSel();
	if ( index == CB_ERR )
	{
		AfxMessageBox(IDS_YH_NOSEL);
		return;
	}

	CString filename;
	m_style.GetLBText(index,filename);
	
	if ( EMCC == g_nCardType )
		filename = g_sys.GetInstallPath() + filename +_T(".") + g_DAT.m_strEMCCLaser;
	else
		filename = m_strPath + filename + _T(".yh");
	
	if ( !DeleteFile(filename) )
	{
		AfxMessageBox(IDS_YH_DELETE);
	}
	else
	{
		m_style.DeleteString(index);
		m_style.SetCurSel(-1);
	}
}

void CProperDlg::OnButtonDefault()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) )
		return;
	
	if ( IDYES  !=  AfxMessageBox(IDS_YH_PROMPT,MB_YESNO|MB_ICONQUESTION))
		return;

	CString filename;
	FILE *fp = NULL;

	if ( EMCC == g_nCardType ) 
		filename.Format(_T("%sdefault.%s"), g_sys.GetInstallPath(), g_DAT.m_strEMCCLaser);
	else
		filename = m_strPath+_T("default.yh");

	SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL); 

	if ( ( fp = _tfopen(filename,_T("wb")) ) == NULL )
	{	
		AfxMessageBox(IDS_YH_DEFAULT);
	}
	else
	{
		CHLDoc *pDoc = GetCurActiveDocument();
		CLay& lay = pDoc->GetLay();
		lay.Serialize(fp, TRUE, g_nCardType,TRUE);
		fclose(fp);
	}

	int index = m_style.FindStringExact(-1, _T("default"));
	if ( index == CB_ERR )
		index = m_style.AddString(_T("default"));

	m_style.SetCurSel(index);
	
	SetFileAttributes(filename,FILE_ATTRIBUTE_READONLY); 	
}

void CProperDlg::OnSelchangeComboStyle() 
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) ){
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	int index = m_style.GetCurSel();
	if ( index  !=  CB_ERR )
	{	
		CString filename;
		m_style.GetLBText(index,filename);	

		if ( EMCC == g_nCardType ) 
			filename = g_sys.GetInstallPath() + filename + _T(".") + g_DAT.m_strEMCCLaser;
		else
			filename = m_strPath+filename+_T(".yh");
	
		BOOL bUnicode = CStdioFileEx::IsFileUnicode(filename);
		FILE *fp = _tfopen(filename,( bUnicode ? _T("rb") : _T("rt") ) );
		if ( fp == NULL )
		{	
			AfxMessageBox(IDS_YH_CHANGE);
		}
		else
		{
			CLay lay;
			lay.Serialize(fp, FALSE, g_nCardType,bUnicode);
			fclose(fp);

			CHLDoc *pDoc = GetCurActiveDocument();
			CLay& doclay  = pDoc->GetLay();
			doclay = lay;

			int nCurSel = m_listLayer.GetCurSel();
			CLayer ly = pDoc->GetLayer(nCurSel);
			SetPara(ly.m_para);
			OnParaListChange(NULL, NULL);
		}
	}
}

void CProperDlg::OnParaListChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	OnParaChange(pNMHDR, pRes);
	
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	int nLayerCount = pDoc->GetLaySize();
	for (int i = 0; i<nLayerCount && i<m_listLayer.GetItemCount(); i++)
	{
		CLayer layer   = pDoc->GetLayer(i);
		TCHAR szSpeed[64] = { 0 };
		TCHAR szQF[16] = { 0 };
		TCHAR szW[16] = { 0 };
		if ( EMCC == g_nCardType )
		{
			double duSpeed = layer.m_para.emcc.dbMarkV;
			_stprintf_s(szSpeed,	64,_T("%0.0f"),  duSpeed);
			_stprintf_s(szQF,	16,	_T("%0.1f"),  layer.m_para.emcc.dbQFreq);
			_stprintf_s(szW,16,		_T("%0.1f"),  layer.m_para.emcc.dbQRls);
		}
		
		TCHAR szNum[64] = { 0 };
		BOOL bLock = pDoc->GetLockFlg(i);
		if ( bLock )
		{
			_stprintf_s(szNum,64, _T("%d  %c"), i, TCHAR(64));
		}
		else
		{
			_stprintf_s(szNum,64, _T("%d"), i);
		}
		
		m_listLayer.SetItemText(i, 0, szNum);
		m_listLayer.SetItemText(i, 2, szSpeed);
		m_listLayer.SetItemText(i, 3, szQF);
		m_listLayer.SetItemText(i, 4, szW);
	}
}




void CProperDlg::OnParaChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	int index = m_listLayer.GetCurSel();
	if ( index  !=  LB_ERR )
	{
		//更新数据
		CLayer ly = pDoc->GetLayer(index);
		double dbOldCur =  0 ;
		if ( EMCC == g_nCardType )
			dbOldCur = ly.m_para.emcc.dbCurrent;
		
		PARA OldPara = ly.m_para;
		GetPara(ly.m_para);
		if ( EMCC == g_nCardType )
		{
			if ( OldPara.emcc.Equal(ly.m_para.emcc) )
			{
				UpdateView();
				return;
			}
		}
		
		pDoc->LayChange(PTY_PARA,ly);
		SetPara(ly.m_para);
		
		if ( fabs(dbOldCur - ly.m_para.emcc.dbCurrent) > 0.1 )
			g_DAT.SetCurrent(ly.m_para.emcc.dbCurrent);
		
		//更新文档中相应层
		UpdateView();
	}
}




BOOL CProperDlg::PreTranslateMessage(MSG* pMsg) 
{
// 	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
// 		&& (GetKeyState(VK_F10) & 0x8000))
// 	{
// 		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL , NULL);
// 		return TRUE;
// 	}
// 	
// 	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
// 		&& (GetKeyState(VK_F7) & 0x8000))
// 	{
// 		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
// 		return TRUE;
// 	}
// 	
// 	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
// 		&& (GetKeyState(VK_F11) & 0x8000))
// 	{
// 		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_DWONLOAD_MARKDATA , NULL);
// 		return TRUE;
// 	}
// 
// 	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_RETURN) & 0x8000))
// 	{
// 		return TRUE;
// 	}
// 	
// 	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_ESCAPE) & 0x8000))
// 	{
// 		return TRUE;
// 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}




void CProperDlg::OnLockLayer()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	TCHAR szValue[64] = {0};
	_stprintf_s(szValue, 64,_T("%d  %c"), m_nItem, TCHAR(64));
	m_listLayer.SetItemText(m_nItem, 0, szValue);

	pDoc->OnLockLayer(m_nItem);
	pDoc->UpdateAllViews(NULL,NULL);
}


//
void CProperDlg::OnUNLockLayer()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	if ( pDoc->OnUnLockLayer(m_nItem) )
	{
		TCHAR szValue[20] = {0};
		_stprintf_s(szValue, 20, _T("%d"), m_nItem);
		m_listLayer.SetItemText(m_nItem, 0, szValue);
		pDoc->UpdateAllViews(NULL,NULL);
	}
}

//

void CProperDlg::SelMarkParaPage(int nIndex)
{
	if (g_CurUserInfo.dwRight1&USER_RIGHT_LAYERSET && nIndex >= 0 && nIndex<m_arEmccPara.GetSize()) 
	{
		if( nIndex != 1 || g_CurUserInfo.dwRight1&USER_RIGHT_ADVANCE )
		{
			PAGE_ITEM* parItem = m_arEmccPara.ElementAt(nIndex);
			for (int iPara = 0; iPara < parItem->GetSize(); ++iPara ) 
			{
				CPropertyItem *pItem = parItem->ElementAt(iPara);
				if ( NULL == pItem )
					continue;
				if ( pItem->m_bShow )
					m_listPara.AddPropItem(pItem);
			}
		}
	}

	m_tabMarkPara.SetCurSel(nIndex);
	m_listPara.Invalidate();
	UpdateView();
}

//
void CProperDlg::InitMarkPara() 
{	
	for (int iPage = 0; iPage < g_DAT.m_arrParaCfg.GetSize(); ++iPage) 
	{
		PAGE_ITEM* parItem = new PAGE_ITEM;

		PARA_PAGE& page = g_DAT.m_arrParaCfg.ElementAt(iPage);
		for (int iPara = 0; iPara < page.m_arrCfg.GetSize(); ++iPara ) 
		{
			PARA_CFG& paraCfg = page.m_arrCfg[iPara];

			CPropertyItem *pItem = new CPropertyItem(  paraCfg.m_propName,
							paraCfg.m_curValue, 
							PIT_EDIT, 
							_T(""), 
							TRUE,
							paraCfg.m_strAlias, 	  
							paraCfg.m_nValueType, 
							paraCfg.m_dbMin,
							paraCfg.m_dbMax
						  );
			parItem->Add(pItem);
		}

		m_arEmccPara.Add(parItem);
	}
}

void CProperDlg::OnDestroy() 
{
	for (int iPage = 0; iPage < m_arEmccPara.GetSize(); ++iPage) 
	{
		PAGE_ITEM* parItem = m_arEmccPara.ElementAt(iPage);
		for (int iPara = 0; iPara < parItem->GetSize(); ++iPara ) 
		{
			CPropertyItem *pItem = parItem->ElementAt(iPara);
			if ( pItem )
			{
				delete pItem;
				pItem = NULL;
			}
		}
		parItem->RemoveAll();
		delete parItem;
	}

	m_arEmccPara.RemoveAll();

	CDialog::OnDestroy();
}

void CProperDlg::OnSelchangeTabMarkpara(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_listPara.Clear();
	int nIndex = m_tabMarkPara.GetCurSel();
	if (-1 == nIndex)
	{
		*pResult = 0;
		return;
	}
	m_listPara.ShowWindow(SW_SHOW);
	SelMarkParaPage(nIndex);
	
	*pResult = 0;
}

void CProperDlg::SetParaListShow()
{
	m_listPara.Clear();
	
	if ( g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET ) 
	{
		m_tabMarkPara.ShowWindow(SW_SHOW);
		SelMarkParaPage(0);
		m_tabMarkPara.SetCurSel(0);
	}
	else
	{
		m_tabMarkPara.ShowWindow(SW_HIDE);
	}
}

void CProperDlg::OnMouseMove( UINT nFlags, CPoint point ) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMouseMove(nFlags, point);
}

void CProperDlg::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialog::OnCaptureChanged(pWnd);
}


void CProperDlg::OnDocChange()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	int nLayerCount = pDoc->GetLaySize();
	if ( nLayerCount > g_SysCfg.m_nLayerCnt ) 
	{
		nLayerCount = g_SysCfg.m_nLayerCnt;
	}
	else if ( nLayerCount < g_SysCfg.m_nLayerCnt )
	{
		int nCount = g_SysCfg.m_nLayerCnt - nLayerCount;
		OnAddLayer(nLayerCount, nCount, pDoc);
	}
	
	for( int i = 0; i < nLayerCount; i++ )
	{
		TCHAR szSpeed[20] = {0};
		TCHAR szQF[16] = {0};
		TCHAR szW[16] = {0};
		
		CLayer layer = pDoc->GetLayer(i);
		if ( EMCC == g_nCardType )
		{
			double duSpeed = layer.m_para.emcc.dbMarkV;
			_stprintf_s(szSpeed, 20, _T("%0.0f"),  duSpeed);
			_stprintf_s(szQF, 16,     _T("%0.1f"),  layer.m_para.emcc.dbQFreq);
			_stprintf_s(szW,  16,     _T("%0.1f"),  layer.m_para.emcc.dbQRls);
		}
		
		TCHAR szNum[20] = {0};
		BOOL bLock = pDoc->GetLockFlg(i);
		if ( bLock )
		{
			_stprintf_s( szNum, 20,  _T("%d  %c"), i, TCHAR(64) );
		}
		else
		{
			_stprintf_s( szNum, 20, _T("%d"), i );
		}
		m_listLayer.SetItemText(i, 0, szNum);
		m_listLayer.SetItemText(i, 2, szSpeed);
		m_listLayer.SetItemText(i, 3, szQF);
		m_listLayer.SetItemText(i, 4, szW);
	}
	
	if ( m_nItem  >=  nLayerCount ) 
	{
		m_nItem = 0;
	}
	
	BOOL bSelNum = FALSE;
	int nIndex = m_listLayer.GetTopIndex();
	for( int j = nIndex; j < nLayerCount; j++ ) 
	{
		CLayer layer = pDoc->GetLayer(j);
		m_listLayer.SetItemColor(j, 1, 0, layer.m_color);
		if ( m_nItem == j )
		{
			bSelNum = TRUE;
		}
		
	    switch( j - nIndex )
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bSelNum);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bSelNum);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bSelNum);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bSelNum);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bSelNum);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bSelNum);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bSelNum);
			break;
		default:
			break;
		}
		bSelNum = FALSE;
	}
	
	m_listLayer.SetItemState( m_nItem, LVIS_SELECTED, LVIS_SELECTED );
	m_listLayer.SetCurSel(m_nItem);
	pDoc->SetCurLayer(m_nItem);
}

//
void CProperDlg::OnAddLayer( int nStart, int nCount, CHLDoc *pDoc )
{
	for( int j = nStart; j < nStart + nCount; j++ )
	{
		CLayer layer   = pDoc->GetLayer(j);
		TCHAR szSpeed[20];
		TCHAR szQF[16];
		TCHAR szW[16];
		if ( EMCC == g_nCardType )
		{
			double duSpeed = layer.m_para.emcc.dbMarkV;
			_stprintf_s(szSpeed,  20,_T("%0.0f"),  duSpeed);
			_stprintf_s(szQF, 16,     _T("%0.0f"), layer.m_para.emcc.dbQFreq);
			_stprintf_s(szW,  16,     _T("%0.0f"),  layer.m_para.emcc.dbQRls);
		}
		
		TCHAR szNum[8];
		_stprintf_s( szNum, 8, _T("%d"), j );
		m_listLayer.AddItem( szNum, _T(""), szSpeed, szQF, szW );
	}
	
	for( int i = nStart; i < nStart + nCount; i++ )
	{
		CLayer layer = pDoc->GetLayer(i);
		m_listLayer.SetItemColor(i, 1, 0, layer.m_color);
	    switch(i)
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, TRUE);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color);
			break;
		default:
			break;
		}
	}
	
	CLay &lay = pDoc->GetLay();
	SetFillObjMarkSet(lay);
}

void CProperDlg::SetFillObjMarkSet(CLay &lay)
{
	CMainFrame *pFWnd = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if ( NULL == pFWnd )
		return;
	
	pFWnd->m_FillEditDlg.m_FillObjMarkSet.SetLayerCount(lay);
}


void CProperDlg::DoUndoLayerPara()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	int  index = m_listLayer.GetCurSel();
	if ( index == LB_ERR )
		return;

	CLayer ly = pDoc->GetLayer(index);

	//第0层的处理
	if ( index == 0 )
	{
		GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_HIDE);
		m_bDefault = FALSE;
	}
	else
	{
		GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_SHOW);
		m_bDefault = ly.m_bDefault;
	}

	ly.m_bDefault = m_bDefault;
	ly.m_nLayer   = index;
	pDoc->SetLayCmd(PTY_PARA,ly);

	//更新打标参数列表
	SetPara(ly.m_para);

	int nLayerCount = pDoc->GetLaySize();
	for( int i = 0; i < nLayerCount; i++ )
	{
		CLayer layer = pDoc->GetLayer(i);
		TCHAR szSpeed[20] = {0};
		TCHAR szQF[16] = {0};
		TCHAR szW[16] = {0};
		if ( EMCC == g_nCardType ) 
		{
			double duSpeed = layer.m_para.emcc.dbMarkV;
			_stprintf_s(szSpeed, 20,_T("%0.0f"),	duSpeed);
			_stprintf_s(szQF,	16,	 _T("%0.1f"),	layer.m_para.emcc.dbQFreq);
			_stprintf_s(szW,	16,	 _T("%0.1f"),	layer.m_para.emcc.dbQRls);
		}

		TCHAR szNum[20] = {0};
		BOOL bLock = pDoc->GetLockFlg(i);
		if ( bLock )
		{
			_stprintf_s(szNum, 20, _T("%d  %c"), i, TCHAR(64));
		}
		else
		{
			_stprintf_s(szNum, 20, _T("%d"), i);
		}

		m_listLayer.SetItemText(i, 0, szNum);
		m_listLayer.SetItemText(i, 2, szSpeed);
		m_listLayer.SetItemText(i, 3, szQF);
		m_listLayer.SetItemText(i, 4, szW);
	}
}

//
void CProperDlg::OnSelectLayer() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;

	pDoc->OnSelectLayer(m_nItem);
}


//
void CProperDlg::OnUnselectLayer() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;

	pDoc->OnUnselectLayer(m_nItem);
}

BOOL CProperDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CProperDlg::OnLbnSelchangeListPara()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CProperDlg::OnLvnItemchangedListLayer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CProperDlg::OnBnClickedButtonLaserdot()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;

	int index = m_listLayer.GetCurSel();
	if ( index  !=  LB_ERR )
	{
		//更新数据
		CLayer ly;
		GetPara(ly.m_para);

		CDlgLaserDot dlg;
		dlg.m_dbSpeed = ly.m_para.emcc.dbMarkV;
		dlg.m_dbFreq = ly.m_para.emcc.dbQFreq*1000;
		if( IDOK == dlg.DoModal() && (fabs(ly.m_para.emcc.dbMarkV - dlg.m_dbSpeed) > EPSILON || fabs(dlg.m_dbFreq - dlg.m_dbFreq) > EPSILON ) )
		{
			// update mark parameter from the dlg
			ly.m_para.emcc.dbMarkV = dlg.m_dbSpeed;
			ly.m_para.emcc.dbQFreq = dlg.m_dbFreq/1000;
			pDoc->LayChange(PTY_PARA,ly);
			SetPara(ly.m_para);
			UpdateView();
		}
	}
}
