
// PropertyList.cpp : implementation file
//

#include "stdafx.h"
//#include "hl.h"
#include "PropertyList.h"
//#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyList

CPropertyList::CPropertyList()
{
	m_bComBoxChange = FALSE;
	m_bEditChange   = FALSE;
}

CPropertyList::~CPropertyList()
{
}


BEGIN_MESSAGE_MAP(CPropertyList, CListBox)
	//{{AFX_MSG_MAP(CPropertyList)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_CBN_KILLFOCUS(IDC_PROPCMBBOX, OnKillfocusCmbBox)
	ON_CBN_SELCHANGE(IDC_PROPCMBBOX, OnSelchangeCmbBox)
	ON_EN_KILLFOCUS(IDC_PROPEDITBOX, OnKillfocusEditBox)
	ON_EN_CHANGE(IDC_PROPEDITBOX, OnChangeEditBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyList message handlers

BOOL CPropertyList::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CListBox::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style  &=  ~(LBS_OWNERDRAWVARIABLE | LBS_SORT);
	cs.style  |=  LBS_OWNERDRAWFIXED;
	
	m_bTracking = FALSE;
	m_nDivider = 0;
	m_bDivIsSet = FALSE;
	
	return TRUE;
}

void CPropertyList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = 20; //pixels
}


void CPropertyList::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC dc;
	dc.Attach(lpDIS->hDC);
	CRect rectFull = lpDIS->rcItem;
	CRect rect = rectFull;
	if ( m_nDivider == 0 )
		m_nDivider = (int)( 0.7* rect.Width() );

	rect.left = m_nDivider;
	CRect rect2 = rectFull;
	rect2.right   = rect.left - 1;
	UINT nIndex = lpDIS->itemID;

	if ( nIndex  !=  (UINT) -1 )
	{
		BOOL flag = FALSE;
		
		//如为选择状态，则高亮显示左边区域
		if ( lpDIS->itemState & ODS_SELECTED )
		{
			dc.Rectangle(rect2.left+1,rect2.top,rect2.right-2,rect2.bottom);
			dc.FillSolidRect(rect2.left+2,rect2.top+1,rect2.Width()-5,rect2.Height()-2,::GetSysColor(COLOR_HIGHLIGHT));

			flag = TRUE;
		}
		else
		{	
			dc.Rectangle(rect2.left+1,rect2.top,rect2.right-2,rect2.bottom-2);
			dc.FillSolidRect(rect2.left+2,rect2.top+1,rect2.Width()-5,rect2.Height()-4,::GetSysColor(COLOR_3DFACE));//COLOR_SCROLLBAR));
		}
	
		CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(nIndex);
		if ( pItem->m_nItemType == PIT_DISABLE )
			dc.FillSolidRect(rect,RGB(192,192,192));

		//显示分隔边框
		dc.DrawEdge(rect2,EDGE_SUNKEN,BF_BOTTOMRIGHT);
		dc.DrawEdge(rect,EDGE_SUNKEN,BF_BOTTOM);

		dc.SetBkMode(TRANSPARENT);
		//显示项目值
		if (pItem->m_nItemType == PIT_COLOR)
		{
			COLORREF color;
			CString currClr = pItem->m_curValue;
			//parse the property's current color value
			if (currClr.Find(_T("RGB")) > -1)
			{
				int j = currClr.Find(_T(','),3);
				CString bufr = currClr.Mid(4,j-4);
				int RVal = _ttoi(bufr);
				int j2 = currClr.Find(_T(','),j+1);
				bufr = currClr.Mid(j+1,j2-(j+1));
				int GVal = _ttoi(bufr);
				int j3 = currClr.Find(_T(')'),j2+1);
				bufr = currClr.Mid(j2+1,j3-(j2+1));
				int BVal = _ttoi(bufr);
				color = RGB(RVal,GVal,BVal);
			}
			else
				color = 0;
			dc.Rectangle(rect.left+1,rect.top,rect.right,rect.bottom-2);
			dc.FillSolidRect(rect.left+2,rect.top+1,rect.Width()-3,rect.Height()-4,color);
		}
		else
		{
			dc.Rectangle(rect.left+1,rect.top,rect.right,rect.bottom-2);
			if (pItem->m_nItemType == PIT_DISABLE)
			    dc.FillSolidRect(rect.left+2,rect.top+1,rect.Width()-3,rect.Height()-4,RGB(192,192,192));
			dc.DrawText(pItem->m_curValue,CRect(rect.left+3,rect.top+3,
				rect.right+3,rect.bottom+3),DT_LEFT | DT_SINGLELINE);
		}

		//显示项目标题
		if ( flag )
		{	
			COLORREF oldCLR = dc.GetTextColor(); 
			dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			dc.DrawText(pItem->m_propName,CRect(rect2.left+3,rect2.top+3,
								rect2.right-3,rect2.bottom+3),DT_LEFT | DT_SINGLELINE);
			dc.SetTextColor(oldCLR);
		}
		else
			dc.DrawText(pItem->m_propName,CRect(rect2.left+3,rect2.top+3,
							rect2.right-3,rect2.bottom+3),DT_LEFT | DT_SINGLELINE);

	}
	dc.Detach();
}

int CPropertyList::AddItem(CString txt)
{
	int nIndex = AddString(txt);
	return nIndex;
}

int CPropertyList::AddPropItem(CPropertyItem* pItem)
{
	int nIndex = AddString(_T(""));
	SetItemDataPtr(nIndex,pItem);
	return nIndex;
}

int CPropertyList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bDivIsSet = FALSE;
	m_nDivider = 0;
	m_bTracking = FALSE;

	m_hCursorSize = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	m_SSerif8Font.CreatePointFont(80,_T("MS Sans Serif"));

	return 0;
}

void CPropertyList::OnSelchange() 
{
 	CRect rect;
	CString lBoxSelText;

	GetItemRect(m_curSel,rect);
	rect.left = m_nDivider;

	if ( LB_ERR == m_curSel) 
		return;

	NotifyChange();
	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);
	
	if ( pItem == NULL )
		return;

	if ( m_BtnCtrl )
		m_BtnCtrl.ShowWindow(SW_HIDE);
	if ( m_advBtn )
		m_advBtn.ShowWindow(SW_HIDE);

	switch(pItem->m_nItemType)
	{
	case PIT_COMBO:
		{
			//display the combo box.  If the combo box has already been
			//created then simply move it to the new location, else create it
			m_nLastBox = 0;
			if ( m_cmbBox )
				m_cmbBox.MoveWindow(rect);
			else
			{	
				rect.bottom  +=  100;
				m_cmbBox.Create(CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT | WS_VISIBLE | WS_CHILD | WS_BORDER,
							rect,this,IDC_PROPCMBBOX);
				m_cmbBox.SetFont(&m_SSerif8Font);
			}

			//add the choices for this particular property
			CString cmbItems = pItem->m_cmbItems;
			lBoxSelText = pItem->m_curValue;
		
			m_cmbBox.ResetContent();

			int i = 0, i2 = 0;
			while ((i2 = cmbItems.Find(_T('|'),i))  !=  -1)
			{
				m_cmbBox.AddString(cmbItems.Mid(i,i2-i));
				i = i2+1;
			}

			m_cmbBox.ShowWindow(SW_SHOW);
			m_cmbBox.SetFocus();

			//jump to the property's current value in the combo box
			int j = m_cmbBox.FindStringExact(0,lBoxSelText);
			if (j  !=  CB_ERR)
				m_cmbBox.SetCurSel(j);
			else
				m_cmbBox.SetCurSel(0);
			break;
		}
	case PIT_EDIT:
		{
			//display edit box
			m_nLastBox = 1;
			m_prevSel = m_curSel;
			rect.bottom  -=  2;
			if (m_editBox)
				m_editBox.MoveWindow(rect);
			else
			{
				m_editBox.Create(ES_LEFT | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD | WS_BORDER,
								rect,this,IDC_PROPEDITBOX);
				m_editBox.SetFont(&m_SSerif8Font);
			}
	
			lBoxSelText = pItem->m_curValue;
	
			m_editBox.SetReadOnly(FALSE);
			m_editBox.ShowWindow(SW_SHOW);
			m_editBox.SetFocus();
			//set the text in the edit box to the property's current value
			m_editBox.SetWindowText(lBoxSelText);
			m_editBox.SetSel(0, lBoxSelText.GetLength());
			break;
		}
	case PIT_DISABLE:
			break;
	case PIT_COLOR:
		{
			//当为第0层且为颜色项时，不能进行编辑，即为缺省层
			if (m_curSel != 0)
				DisplayButton(rect);
		}
	case PIT_ADV:
		DisplayAdvCtrl(rect);
		break;
	}
	
	NotifyChange(SEL_CHANGE);
}

void CPropertyList::DisplayButton(CRect region)
{
	m_nLastBox = 2;
	m_prevSel = m_curSel;

	if (region.Width() > 20)
		region.left = region.right - 20;
	region.bottom -= 2;

	if (m_BtnCtrl){
		m_BtnCtrl.MoveWindow(region);
	}
	else
	{	
		m_BtnCtrl.Create(BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
							region,this,IDC_PROPBTNCTRL);
		m_BtnCtrl.SetFont(&m_SSerif8Font);
	}
	m_BtnCtrl.ShowWindow(SW_SHOW);
}

void CPropertyList::OnKillfocusCmbBox() 
{
	if ( m_curSel == LB_ERR )
		return;

	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);
	if ( pItem == NULL  || PIT_COMBO  !=  pItem->m_nItemType )
		return;

	m_cmbBox.ShowWindow(SW_HIDE);

	Invalidate();

	NotifyChange();
}

void CPropertyList::OnKillfocusEditBox()
{
	if ( m_curSel == LB_ERR )
		return;

	CString newStr;
	m_editBox.GetWindowText(newStr);
	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);

	if ( pItem == NULL /*|| PIT_COMBO  !=  pItem->m_nItemType*/ )
		return;

	double dbInput = _tstof((LPCTSTR)newStr);
	if ( dbInput > pItem->m_dbMax )
	{
		switch(pItem->m_nValueType)
		{
			case PIT_INT:
				newStr.Format(_T("%d") , (int)pItem->m_dbMax);
				break;
			case PIT_DOUBLE:
				newStr.Format(_T("%0.3f") , pItem->m_dbMax);
				break;
			default:
				newStr.Format(_T("%d") , (int)pItem->m_dbMax);
				break;
		}		
	}
	else if ( dbInput < pItem->m_dbMin ) 
	{
		dbInput = pItem->m_dbMin;
		switch(pItem->m_nValueType) 
		{
			case PIT_INT:
				newStr.Format(_T("%d") , (int)pItem->m_dbMin);
				break;
			case PIT_DOUBLE:
				newStr.Format(_T("%0.3f") , pItem->m_dbMin);
				break;
			default:
				newStr.Format(_T("%d") , (int)pItem->m_dbMin);
				break;
		}
	}

	pItem->m_curValue = newStr;

	m_editBox.ShowWindow(SW_HIDE);
	Invalidate();

	NotifyChange();

	if ( (1 == m_curSel) || (8 == m_curSel) || (10 == m_curSel) )
	{
		NotifyChange(SEL_VALUE_CHANGE);
	}
}

void CPropertyList::OnKillfocusEditNoNotify()
{
	if ( m_curSel == LB_ERR )
		return;

	CString newStr;
	m_editBox.GetWindowText(newStr);
	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);

	if ( pItem == NULL || PIT_COMBO  !=  pItem->m_nItemType )
		return;

	pItem->m_curValue = newStr;

	m_editBox.ShowWindow(SW_HIDE);
	Invalidate();
}

void CPropertyList::OnSelchangeCmbBox()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	CString selStr;
	if ( m_cmbBox )
	{
		m_cmbBox.GetLBText(m_cmbBox.GetCurSel(),selStr);
		CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);
		pItem->m_curValue    = selStr;

		NotifyChange();
	}
}

void CPropertyList::OnChangeEditBox()
{
	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_LAYERSET))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CString newStr;
	m_editBox.GetWindowText(newStr);
	
	CPropertyItem* pItem = (CPropertyItem*)GetItemDataPtr(m_curSel);
	pItem->m_curValue = newStr;

	m_bEditChange = TRUE;

	return;
}

void CPropertyList::OnSelectOk()
{
	if ( m_curSel == LB_ERR )
		return;

	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);

	if ( m_BtnCtrl )
		m_BtnCtrl.ShowWindow(SW_HIDE);

	if ( pItem == NULL )
		return;

	COLORREF selClr = m_BtnCtrl.GetColor();
	CString clrStr;
	clrStr.Format(_T("RGB(%d,%d,%d)"),  GetRValue(selClr),
						GetGValue(selClr), GetBValue(selClr));
	pItem->m_curValue = clrStr;

	Invalidate();
	NotifyChange();
}

void CPropertyList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		//if columns were being resized then this indicates
		//that mouse is up so resizing is done.  Need to redraw
		//columns to reflect their new widths.
		
		m_bTracking = FALSE;
		//if mouse was captured then release it
		if ( GetCapture() == this )
			::ReleaseCapture();

		::ClipCursor(NULL);

		CClientDC dc(this);
		InvertLine(&dc,CPoint(point.x,m_nDivTop),CPoint(point.x,m_nDivBtm));
		//set the divider position to the new value
		m_nDivider = point.x;

		//redraw
		Invalidate();
	}
	else
	{
		BOOL loc(FALSE);
		m_curSel = ItemFromPoint(point,loc);
		CListBox::OnLButtonUp(nFlags, point);
	}
}

void CPropertyList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((point.x >= m_nDivider-5) && (point.x <= m_nDivider+5))
	{
		//if mouse clicked on divider line, then start resizing
		::SetCursor(m_hCursorSize);

		CRect windowRect;
		GetWindowRect(windowRect);
		windowRect.left  +=  10; windowRect.right  -=  10;
		//do not let mouse leave the list box boundary
		::ClipCursor(windowRect);
		
		if (m_cmbBox)
			m_cmbBox.ShowWindow(SW_HIDE);
		if (m_editBox)
			m_editBox.ShowWindow(SW_HIDE);
		
		CRect clientRect;
		GetClientRect(clientRect);
		
		m_bTracking = TRUE;
		m_nDivTop = clientRect.top;
		m_nDivBtm = clientRect.bottom;
		m_nOldDivX = point.x;

		CClientDC dc(this);
		InvertLine(&dc,CPoint(m_nOldDivX,m_nDivTop),CPoint(m_nOldDivX,m_nDivBtm));

		//capture the mouse
		SetCapture();
	}
	else
	{
		m_bTracking = FALSE;
		CListBox::OnLButtonDown(nFlags, point);
	}
}

void CPropertyList::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (m_bTracking)
	{
		//move divider line to the mouse pos. if columns are
		//currently being resized
		CClientDC dc(this);
		//remove old divider line
		InvertLine(&dc,CPoint(m_nOldDivX,m_nDivTop),CPoint(m_nOldDivX,m_nDivBtm));
		//draw new divider line
		InvertLine(&dc,CPoint(point.x,m_nDivTop),CPoint(point.x,m_nDivBtm));
		m_nOldDivX = point.x;
	}
	else if ((point.x  >=  m_nDivider-5) && (point.x  <=  m_nDivider+5))
		//set the cursor to a sizing cursor if the cursor is over the row divider
		::SetCursor(m_hCursorSize);
	else
		CListBox::OnMouseMove(nFlags, point);
//	SetFocus();
}

void CPropertyList::InvertLine(CDC* pDC,CPoint ptFrom,CPoint ptTo)
{
	int nOldMode = pDC->SetROP2(R2_NOT);
	
	pDC->MoveTo(ptFrom);
	pDC->LineTo(ptTo);

	pDC->SetROP2(nOldMode);
}

void CPropertyList::PreSubclassWindow() 
{
	m_bDivIsSet	 =  FALSE;
	m_nDivider		 =  0;
	m_bTracking	 =  FALSE;
	m_curSel		 =  1;

	m_hCursorSize		 =  AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorArrow	 =  AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	m_SSerif8Font.CreatePointFont(80,_T("MS Sans Serif"));
}



void CPropertyList::NotifyChange(UINT code)
{
	NMHDR nm;
	nm.hwndFrom = m_hWnd;
	nm.idFrom = NULL;
	nm.code = code;
	CWnd* pWnd = GetOwner();

	if ( pWnd )
	{
		pWnd->SendMessage(WM_NOTIFY, nm.idFrom, (LPARAM)&nm);
	}
}



void CPropertyList::DeleteItem( LPDELETEITEMSTRUCT lpDeleteItemStruct )
{
	if ( m_cmbBox )
		m_cmbBox.ShowWindow(SW_HIDE);
	if ( m_editBox )
		m_editBox.ShowWindow(SW_HIDE);
	if ( m_BtnCtrl )
		m_BtnCtrl.ShowWindow(SW_HIDE);
	
	CPropertyItem* pItem = (CPropertyItem*)(lpDeleteItemStruct->itemData);
	//delete pItem;
	if ( pItem &&  PIT_COLOR == pItem->m_nItemType) 
	{
		delete pItem;
		pItem = NULL;
	}
}



BOOL CPropertyList::PreTranslateMessage(MSG* pMsg) 
{
	if (     ( WM_KEYDOWN  == pMsg->message ) 
		&& ((GetKeyState(VK_RETURN) & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)))
	{
		m_curSel    =  GetCurSel();
		
		OnKillfocusCmbBox();
		OnKillfocusEditBox();
		
		int nCount  = GetCount();
		int nSelNum = ((m_curSel + 1) % nCount);
		SetCurSel(nSelNum);
		m_curSel = nSelNum;
		OnSelchange();
		
		return TRUE;
	}
	
	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_UP) & 0x8000))
	{
		m_curSel   = GetCurSel();
		
		OnKillfocusCmbBox();
		OnKillfocusEditBox();
		
		int nCount   = GetCount();
		int nSelNum = ((m_curSel - 1) % nCount);
		if ( 0  >=  nSelNum )
		{
			nSelNum = 0;
		}
		SetCurSel(nSelNum);
		m_curSel = nSelNum;
		OnSelchange();
		
		return TRUE;
	}
	
	return CListBox::PreTranslateMessage(pMsg);
}



//
void CPropertyList::Clear()
{	
	ResetContent();
	m_curSel = -1;
	if ( m_cmbBox )
		m_cmbBox.DestroyWindow();
	if ( m_editBox )
		m_editBox.DestroyWindow();
	if ( m_BtnCtrl )
		m_BtnCtrl.DestroyWindow();
	if ( m_advBtn )
		m_advBtn.DestroyWindow();
}


//
void CPropertyList::DisplayAdvCtrl(CRect rect)
{
	CRect rtBtn = rect;
	
	if ( m_advBtn )
	{
		m_advBtn.MoveWindow(rtBtn);
	}
	else
	{	
		CString str;
		str.LoadString(IDS_MARKTYPESET);
		m_advBtn.Create(str, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
										rtBtn,this,IDC_PROPADVBTN);
		m_advBtn.SetFont(&m_SSerif8Font);
	}
	///////
	m_advBtn.ShowWindow(SW_SHOW);
}


void CPropertyList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	Invalidate();
	
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}
