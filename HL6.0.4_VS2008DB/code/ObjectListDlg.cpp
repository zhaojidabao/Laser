// ObjectListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ObjectListDlg.h"
#include "HLView.h"
#include "DlgSetDelay.h"
#include "DlgSetInput.h"
#include "DlgSetOutput.h"
#include "NodeDelay.h"
#include "NodeInput.h"
#include "NodeOutput.h"
#include "DlgChangeObjName.h"
#include "baseMacro.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectListDlg dialog

CObjectListDlg::CObjectListDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CObjectListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_listInfo.RemoveAll();
	m_nListCurItem = 0;
	m_nCurItem = 0;

	m_bBlockItem = FALSE;
}


void CObjectListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectListDlg)
	DDX_Control(pDX, IDC_LIST_DOC_OBJECT, m_DocObjList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectListDlg, CDialog)
	//{{AFX_MSG_MAP(CObjectListDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DOC_OBJECT, OnClickListDocObject)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DOC_OBJECT, OnRclickListDocObject)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_DOC_OBJECT, OnBeginlabeleditListDocObject)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_DOC_OBJECT, OnRdblclkListDocObject)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOC_OBJECT, OnDblclkListDocObject)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MOVE_PREV, OnMovePrev)
	ON_COMMAND(ID_MOVE_NEXT, OnMoveNext)
	ON_COMMAND(ID_MOVETO_LAST, OnMovetoLast)
	ON_COMMAND(ID_MOVETO_FIRST, OnMovetoFirst)
	ON_COMMAND(ID_OBJ_CHANGENAME, OnObjChangename)
	ON_COMMAND(ID_OBJLIST_DEL, ObjDel)
	ON_COMMAND(ID_LOAD_OBJLIST, ObjListLoad)
	ON_COMMAND(ID_UNLOAD_OBJLIST, ObjListUnLoad)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_DOC_OBJECT, OnKeydownListDocObject)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ITEM_DRAG, OnItemDrag)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOC_OBJECT, &CObjectListDlg::OnLvnItemchangedListDocObject)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectListDlg message handlers

void CObjectListDlg::OnClickListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( pNMHDR == NULL ) return;
	
	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	int nCount = m_listInfo.GetSize();
	int nItem  = m_DocObjList.HitTest( point, NULL);
	if ((-1 == nItem) || (nCount  <=  0) || (nItem  >=  nCount))
	{
		*pResult = 0;
		return;
	}
	
	m_nListCurItem = nItem;
	m_nCurItem	 =  m_nListCurItem;
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	int nSelCount = m_DocObjList.GetSelectedCount();
	if ( nSelCount  <=  0 )
		return;
	
	pDoc->SetSelect(FALSE);
	
	POSITION pos = m_DocObjList.GetFirstSelectedItemPosition();
	while( pos  !=  NULL )
	{
		int nIndex = m_DocObjList.GetNextSelectedItem(pos);
		CGroupObj *pGroupObj = m_listInfo.GetAt(nIndex);
		if ( NULL == pGroupObj )
			continue;
		
		pDoc->SetGroupObjSel(pGroupObj);
	}
	
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if ( pView )
	{
		pView->DispSelObjProp();
	}
	
	*pResult = 0;
}


BOOL CObjectListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_DocObjList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CString str("");
	str.LoadString(IDS_NAME);
	m_DocObjList.InsertColumn(0,(LPCTSTR)str,LVCFMT_LEFT,100,50);
	str.LoadString(IDS_TYPE);
	m_DocObjList.InsertColumn(1,(LPCTSTR)str,LVCFMT_LEFT,70,50);
	
	HICON hIcon[29];
	m_ImageList.Create(16,16,ILC_COLORDDB|ILC_MASK|ILC_COLOR24|ILC_COLOR32, 8, 8); // 32, 32 for large icons
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON_UNKNOW);    // 未知节点
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON_POLYLINE);  // 曲线 
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON_RECTANGLE); // 矩形
	hIcon[3] = AfxGetApp()->LoadIcon(IDI_ICON_TEXT);      // 文本
	hIcon[4] = AfxGetApp()->LoadIcon(IDI_ICON_TEXT);      // 跳号
	hIcon[5] = AfxGetApp()->LoadIcon(IDI_ICON_BMP);       // BMP
	hIcon[6] = AfxGetApp()->LoadIcon(IDI_ICON_ELLIPSE);   // 圆
	hIcon[7] = AfxGetApp()->LoadIcon(IDI_ICON_PLT);       // PLT
	hIcon[8] = AfxGetApp()->LoadIcon(IDI_ICON_GRID);      // 网格
	hIcon[9] = AfxGetApp()->LoadIcon(IDI_ICON_POLYGON);   // 多边形
	hIcon[10] = AfxGetApp()->LoadIcon(IDI_ICON_DXF);      // DXF
	hIcon[11] = AfxGetApp()->LoadIcon(IDI_ICON_CODEBAR);  // 二维条码
    hIcon[12] = AfxGetApp()->LoadIcon(IDI_ICON_COMBINOBJ);// 组合对象
	hIcon[13] = AfxGetApp()->LoadIcon(IDI_ICON_DELAY);	  // 延时器对象
	hIcon[14] = AfxGetApp()->LoadIcon(IDI_ICON_INPUT);	  // 输入信号对象
	hIcon[15] = AfxGetApp()->LoadIcon(IDI_ICON_OUTPUT);	  // 输出信号对象
	hIcon[16] = AfxGetApp()->LoadIcon(IDI_ICON_SPLINE);   //样条曲线
	hIcon[17] = AfxGetApp()->LoadIcon(IDI_ICON_ARC);		//圆弧
	hIcon[18] = AfxGetApp()->LoadIcon(IDI_ICON_CIRCLE0);	//圆
	hIcon[19] = AfxGetApp()->LoadIcon(IDI_ICON_DOT);		//点
	hIcon[20] = AfxGetApp()->LoadIcon(IDI_ICON_UNION);		//群组对象
	hIcon[21] = AfxGetApp()->LoadIcon(IDI_ICON_MOTORMOVE);		//运动控制对象
	hIcon[22] = AfxGetApp()->LoadIcon(IDI_ICON_MOTORPOS);		//运动位置设定对象
	hIcon[23] = AfxGetApp()->LoadIcon(IDI_ICON_CONDITIONS);		//条件开始对象
	hIcon[24] = AfxGetApp()->LoadIcon(IDI_ICON_CONDITIONE);		//条件结束对象
	hIcon[25] = AfxGetApp()->LoadIcon(IDI_ICON_POLYGON);		//nC
	hIcon[26] = AfxGetApp()->LoadIcon(IDI_ICON_BUFFER_LASER);		//激光控制
	hIcon[27] = AfxGetApp()->LoadIcon(IDI_ICON_CCD);		//CCD对象
	hIcon[28] = AfxGetApp()->LoadIcon(IDI_ICON_POSCALI);		//位置校正对象

	for (int i = 0; i < 29; i++)
	{
		m_ImageList.Add(hIcon[i]);
	}
	
	str.LoadString(IDS_UNKNOW);
	m_DocObjList.SetStringAt(NODE_BASE, str);
	
	str.LoadString(IDS_LINES);
	m_DocObjList.SetStringAt(NODE_LINE, str);
	
	str.LoadString(IDS_RECT);
	m_DocObjList.SetStringAt(NODE_RECT, str);
	
	str.LoadString(IDS_TEXT);
	m_DocObjList.SetStringAt(NODE_HLTEXT, str);
	
	str.LoadString(IDS_JUMPSTRING);
	m_DocObjList.SetStringAt(NODE_SERIAL, str);
	
	str.LoadString(IDS_BMP);
	m_DocObjList.SetStringAt(NODE_BMP, str);
	
	str.LoadString(IDS_ELLIPSE);
	m_DocObjList.SetStringAt(NODE_ELLIPSE, str);
	
	str = _T("PLT");
	m_DocObjList.SetStringAt(NODE_PLT, str);
	
	str.LoadString(IDS_GRID);
	m_DocObjList.SetStringAt(NODE_GRID, str);
	
	str.LoadString(IDS_POLYGON);
	m_DocObjList.SetStringAt(NODE_POLYGON, str);
	
	str = _T("DXF");
	m_DocObjList.SetStringAt(NODE_DXF, str);
	
	str.LoadString(IDS_CODEBAR);
	m_DocObjList.SetStringAt(NODE_CODEBAR, str);
	
	str.LoadString(IDS_GROUP);
	m_DocObjList.SetStringAt(NODE_COMBIN, str);
	
	str.LoadString(IDS_DELAY);
	m_DocObjList.SetStringAt(NODE_DELAY, str);
	
	str.LoadString(IDS_INPUT);
	m_DocObjList.SetStringAt(NODE_INPUT, str);
	
	str.LoadString(IDS_OUTPUT);
	m_DocObjList.SetStringAt(NODE_OUTPUT, str);
	
	str.LoadString(IDS_SPLINE);
	m_DocObjList.SetStringAt(NODE_SPLINE, str);
	
	str.LoadString(IDS_ARC);
	m_DocObjList.SetStringAt(NODE_ARC, str);
	
	str.LoadString(IDS_CIRCLE);
	m_DocObjList.SetStringAt(NODE_CIRCLE, str);

	str.LoadString(IDS_DOT);
	m_DocObjList.SetStringAt(NODE_DOT, str);

	str.LoadString(IDS_UNION);
	m_DocObjList.SetStringAt(NODE_UNION, str);
	
	str.LoadString(IDS_MOTORMOVE);
	m_DocObjList.SetStringAt(NODE_MOTORMOVE, str);

	str.LoadString(IDS_MOTORPOS);
	m_DocObjList.SetStringAt(NODE_MOTORPOS, str);

	str.LoadString(IDS_CONDITIONS);
	m_DocObjList.SetStringAt(NODE_CONDITIONS, str);

	str.LoadString(IDS_CONDITIONE);
	m_DocObjList.SetStringAt(NODE_CONDITIONE, str);

	str.LoadString(IDS_OBJINFO_BUFFERLASER);
	m_DocObjList.SetStringAt(NODE_BUFFERLASER, str);

	str.LoadString(IDS_OBJINFO_CCD);
	m_DocObjList.SetStringAt(NODE_CCD, str);

	str.LoadString(IDS_OBJINFO_POSCALI);
	m_DocObjList.SetStringAt(NODE_POSCALI, str);

	m_DocObjList.SetImageList(&m_ImageList, LVSIL_SMALL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
BOOL CObjectListDlg::AddClmItem(CGroupObj *pGroupObj)
{	
	if ( NULL == pGroupObj )
		return FALSE;
	
	ENU_OBJTYPE nObjType = (ENU_OBJTYPE)pGroupObj->GetType();
	
	char szMsg[32] = { 0 };
	int nCount = m_listInfo.GetSize();
	BOOL bAddEnable = TRUE;
	switchType eShow = eTypeNormal;
	
	switch(nObjType)
	{
	case NODE_BASE:
	case NODE_LINE:
	case NODE_RECT:
	case NODE_HLTEXT:
	case NODE_SERIAL:
	case NODE_BMP:
	case NODE_ELLIPSE:
	case NODE_PLT:
	case NODE_GRID:
	case NODE_POLYGON:
	case NODE_DXF:
	case NODE_CODEBAR:
	case NODE_COMBIN:
	case NODE_SPLINE:
	case NODE_ARC:
	case NODE_CIRCLE:
	case NODE_UNION:
	case NODE_DOT:
		if ( nCount > 1000 )
			bAddEnable = FALSE;
		
		eShow = m_bBlockItem ?  eTypeBlock : eTypeNormal;
		break;
	case NODE_DELAY:
	case NODE_INPUT:
	case NODE_OUTPUT:
	case NODE_MOTORMOVE:
	case NODE_MOTORPOS:
	case NODE_BUFFERLASER:
	case NODE_CCD:
	case NODE_POSCALI:
		eShow = m_bBlockItem?eTypeBlock:eTypeNormal;
		break;
	case NODE_CONDITIONS:
		if (!m_bBlockItem) 
		{
			m_bBlockItem = TRUE;
			eShow = eTypeKey;
		}
		else
		{
			eShow = eTypeErrKey;
		}
		break;
	case NODE_CONDITIONE:
		if (m_bBlockItem) 
		{
			m_bBlockItem = FALSE;
			eShow = eTypeKey;
		}
		else
		{
			eShow = eTypeErrKey;
		}
		break;
	default:
		nObjType = NODE_BASE;
		eShow = m_bBlockItem ? eTypeBlock : eTypeNormal;
		break;
	}
	
	if ( bAddEnable == TRUE )
	{
		m_DocObjList.InsertItem(nCount , nObjType, nObjType, pGroupObj->m_strObjName);
		m_DocObjList.SetItemData(nCount, eShow);
		m_listInfo.Add(pGroupObj);
	}
	
	return TRUE;
}



BOOL CObjectListDlg::DelAllClmItem()
{
	m_listInfo.RemoveAll();
	m_DocObjList.DeleteAllItems();
	
	m_bBlockItem = FALSE;
	return TRUE;
}


BOOL CObjectListDlg::SetHeadings( const CString& strHeadings )
{
	int nNumColumns  = 0;
	int nStart = 0;
	while( TRUE )
	{
		const int iComma = strHeadings.Find( _T(','), nStart );
		if ( iComma < 0 )
			break;
		
		const CString strHeading = strHeadings.Mid( nStart, iComma - nStart );
		nStart = iComma + 1;
		int iSemiColon = strHeadings.Find( _T(';'), nStart );
		if ( iSemiColon < 0 )
		{
			iSemiColon = strHeadings.GetLength();
		}
		
		const int nWidth = _ttoi( strHeadings.Mid( nStart, iSemiColon - nStart ) );
		nStart = iSemiColon + 1;
		
		switch(nNumColumns)
		{
		case 0:
			{
				if (m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_LEFT, nWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		case 1:
			{
				if ( m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_LEFT, nWidth ) == -1 )
				{
					return FALSE;
				}
				break;
			}
		case 2:
			{
				if ( m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_CENTER, nWidth ) == -1 )
				{
					return FALSE;
				}
				break;
			}
		default:
			{
				if ( m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_CENTER, nWidth ) == -1 )
				{
					return FALSE;
				}
				break;
			}
		}

		nNumColumns++;
	}

	return TRUE;
}

void CObjectListDlg::OnRclickListDocObject(NMHDR* pNMHDR, LRESULT* pResult)
{
	if ( pNMHDR == NULL )
		return;
	
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CPoint point = pNMListView->ptAction;
	
	int nCount = m_listInfo.GetSize();
	int nItem  = m_DocObjList.HitTest( point, NULL);
	if ( (-1 == nItem) || (nCount  <=  0) || (nItem  >=  nCount) )
	{
		*pResult = 0;
		return;
	}
	
	m_nListCurItem = nItem;
	m_nCurItem	   = m_nListCurItem;
	CGroupObj *pGroupObj   = m_listInfo.GetAt(m_nListCurItem);
	int nObjType   = pGroupObj->GetType();
	
	CMenu menu;
	menu.LoadMenu(IDR_POP);
	CMenu* pMenu = menu.GetSubMenu(6);
	if ( pMenu )
	{
		point = pNMListView->ptAction;
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	SetFocus();
	
	*pResult = 0;
}




BOOL CObjectListDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( WM_SYSKEYDOWN  == pMsg->message || WM_KEYDOWN  == pMsg->message )
	{
		if ( GetKeyState(VK_F10) & 0x8000 )
		{
			AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL, NULL);
			return TRUE;
		}
		else if ( GetKeyState(VK_F7) & 0x8000 )
		{
			AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
			return TRUE;
		}
		else if ( GetKeyState(VK_F11) & 0x8000 )
		{
			AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_DWONLOAD_MARKDATA , NULL);
			return TRUE;
		}
		else if ( GetKeyState(VK_DELETE) & 0x8000 )
		{
			ObjDel();
			return TRUE;
		}
		else if ( GetKeyState(VK_RETURN) & 0x8000 )
			return TRUE;
		else if ( GetKeyState(VK_SPACE) & 0x8000 )
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}




void CObjectListDlg::ObjDel()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;

	CHLDoc *pDoc = (CHLDoc*)GetCurActiveDocument();
	if ( pDoc == NULL )
		return;

	pDoc->Delete();
	
	SetObjUnSel();
	ObjListLoad();
}




//
void CObjectListDlg::ObjListLoad()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	ObjListUnLoad();
	pDoc->ObjListLoad();
}




void CObjectListDlg::ObjListUnLoad()
{
	DelAllClmItem();
}



void CObjectListDlg::OnBeginlabeleditListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( pNMHDR == NULL )
		return;

	LV_DISPINFO* pDispInfo  = (LV_DISPINFO*)pNMHDR;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CPoint point = pNMListView->ptAction;
	
	int nCount = m_listInfo.GetSize();
	int nItem  = m_DocObjList.HitTest( point, NULL);
	if ( (-1 == nItem) || (nCount  <=  0) || (nItem  >=  nCount) )
	{
		*pResult = 0;
		return;
	}
	
	m_nListCurItem = nItem;
	m_nCurItem = m_nListCurItem;
	*pResult = 0;
}





void CObjectListDlg::OnRdblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( pNMHDR == NULL ) 
		return;
	
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CPoint point = pNMListView->ptAction;
	
	int nCount = m_listInfo.GetSize();
	int nItem  = m_DocObjList.HitTest( point, NULL);
	if ( (-1 == nItem) || (nCount  <=  0) || (nItem  >=  nCount) )
	{
		*pResult = 0;
		return;
	}
	
	m_nListCurItem = nItem;
	m_nCurItem = m_nListCurItem;
	*pResult = 0;
}


void CObjectListDlg::OnDblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;
	
	if ( pNMHDR == NULL ) 
		return;
	
	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	int nCount = m_listInfo.GetSize();
	int nItem  = m_DocObjList.HitTest( point, NULL);
	if ( (-1 == nItem) || (nCount  <=  0) || (nItem  >=  nCount) )
	{
		*pResult = 0;
		return;
	}
	
	//
	CGroupObj *pGoupObj = m_listInfo.GetAt(m_nListCurItem);
	CDlgChangeObjName dlg;
	dlg.m_strName = pGoupObj->m_strObjName;
	if ( dlg.DoModal() == IDOK )
	{
		pGoupObj->m_strObjName = dlg.m_strName;		
		CHLDoc *pDoc = GetCurActiveDocument();
		if ( NULL == pDoc )
			return;
		
		ObjListUnLoad();
		pDoc->ObjListLoad();
		m_DocObjList.UpdateListCtrl();
	}
	
	*pResult = 0;
}


void CObjectListDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMouseMove(nFlags, point);
}



int CObjectListDlg::SetSelByGroupObj( CGroupObj *pGroupObj )
{
	int nIndex = -1;
	int nCount = m_listInfo.GetSize();
	for( int i = 0; i < nCount; i++ )
	{
		CGroupObj *pGroupObjTemp = m_listInfo.GetAt(i);
		if ( pGroupObjTemp  !=  NULL )
		{
			if ( pGroupObjTemp == pGroupObj )
			{
				if ( nIndex < 0 ) 
					nIndex = i;
				
				m_DocObjList.SetItemState( i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
				break;
			}
		}
	}
	
	return nIndex;
}



void CObjectListDlg::SetObjUnSel()
{
	int nCount = m_DocObjList.GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		m_DocObjList.SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
	}
}




void CObjectListDlg::OnMovePrev() 
{
	// TODO: Add your command handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;

	m_nListCurItem = m_nCurItem; 
	if (  m_nListCurItem < 1 || m_nListCurItem  >=  m_DocObjList.GetItemCount() )
		return;
	
	int  nItem = m_nListCurItem;
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	SortTreeObject(m_nListCurItem, m_nListCurItem-1);
}


void CObjectListDlg::OnMoveNext()
{
	// TODO: Add your command handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;

	m_nListCurItem = m_nCurItem;
	int  nItem = m_nListCurItem;
	//////
	if (  nItem < 0 || nItem  >=  ( m_DocObjList.GetItemCount() - 1 ) )
		return;

	SortTreeObject(m_nListCurItem, m_nListCurItem+1);
}



void CObjectListDlg::OnMovetoLast()
{
	// TODO: Add your command handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return; 

	m_nListCurItem = m_nCurItem; 
	int  nItem = m_nListCurItem;
	/////
	if (  nItem < 0 || nItem  >=  ( m_DocObjList.GetItemCount() -1 ) )
		return;
	
	SortTreeObject(m_nListCurItem, m_DocObjList.GetItemCount()-1);
}

void CObjectListDlg::OnMovetoFirst()
{
	// TODO: Add your command handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;
	
	m_nListCurItem = m_nCurItem;
	int nItem = m_nListCurItem;
	//
	if (  nItem < 1 || nItem  >=  m_DocObjList.GetItemCount() )
		return;
	
	SortTreeObject(m_nListCurItem, 0);
}


//
void CObjectListDlg::SortTreeObject(int nFrom, int nTo)
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	pDoc->AdjustObjectOrder(nFrom, nTo);
	
	ObjListUnLoad();
	pDoc->ObjListLoad();
	
	m_DocObjList.UpdateListCtrl();
}

LRESULT CObjectListDlg::OnItemDrag(WPARAM wParam, LPARAM lParam)
{
	SortTreeObject((int)wParam, (int)lParam);
	return 0;
}


void CObjectListDlg::OnObjChangename()
{
	// TODO: Add your command handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;

	CGroupObj *pGoupObj = m_listInfo.GetAt(m_nListCurItem);
	CDlgChangeObjName dlg;
	dlg.m_strName = pGoupObj->m_strObjName;
	if ( dlg.DoModal() == IDOK )
	{
		pGoupObj->m_strObjName = dlg.m_strName;

		CHLDoc *pDoc = GetCurActiveDocument();
		if ( NULL == pDoc )
			return;
		
		ObjListUnLoad();
		pDoc->ObjListLoad();
		pDoc->SetModifiedFlag();
		m_DocObjList.UpdateListCtrl();
	}
}

void CObjectListDlg::OnKeydownListDocObject( NMHDR* pNMHDR, LRESULT* pResult ) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nItem  = m_nCurItem;
	if (pLVKeyDow->wVKey == VK_UP)
	{
		nItem--;
	}
	else if (pLVKeyDow->wVKey == VK_DOWN)
	{
		nItem++;
	}
	else
	{
		*pResult = 1;
		return;
	}
	
	int nCount = m_listInfo.GetSize();
	if ((-1 == nItem) || (nCount  <=  0) || (nItem  >=  nCount))
	{
		*pResult = 0;
		return;
	}
	
	int nSelCount = m_DocObjList.GetSelectedCount();
	if ( nSelCount > 1 )
	{
		*pResult = 1;
		return;
	}

	m_nListCurItem = nItem;
	m_nCurItem	 =  m_nListCurItem;
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
		
	pDoc->SetSelect(FALSE);
	CGroupObj *pGroupObj = m_listInfo.GetAt(m_nCurItem);
	pDoc->SetGroupObjSel(pGroupObj);

	*pResult = 0;
}



void CObjectListDlg::OnLvnItemchangedListDocObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
