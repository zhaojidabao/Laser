// ColorButton.cpp : implementation file
//


#include "stdafx.h"
#include "ColorButton.h"
#include "propertylist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
}

CColorButton::~CColorButton()
{
	m_CBox.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers
BOOL CColorButton::Create(DWORD dwStyle, const RECT& rect,
						  CWnd* pParentWnd, UINT nID)
{
	if ( pParentWnd == NULL )
		return FALSE;
	
	if ( !CButton::Create(_T("..."),dwStyle,rect,pParentWnd,nID) )
		return FALSE;

	 return m_CBox.CreateColorBox(0,0,110,230,this);
}

int CColorButton::GetColorBox()
{
	return m_CBox.m_nCurBox;
}

void CColorButton::ShowWin()
{
	CRect rect;
	GetWindowRect(&rect);

	int left = rect.left;
	int bottom = rect.bottom;
	int MaxR = GetSystemMetrics(SM_CXSCREEN);
	int MaxB = GetSystemMetrics(SM_CYSCREEN);

	//���ұ߽紦��
	if ( left < 5 )
		left = 5;
	if ( left > MaxR-110 )
		left = MaxR-110;

	//���±߽紦��
	if ( bottom < 5 )
		bottom = 5;
	if ( bottom > MaxB-260 )
		bottom = MaxB-260;

	m_CBox.MoveWindow(left,bottom,110,230);
	m_CBox.ShowWin();
}

void CColorButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDown(nFlags,point);
	ShowWin();
}

COLORREF CColorButton::GetColor()
{
	return (m_CBox.GetColor(m_CBox.m_nCurBox));
}

void CColorButton::PostMes()
{
	CWnd* pWnd = GetParent();
	CPropertyList* pList = (CPropertyList*)pWnd;
	pList->OnSelectOk();
}
