#if !defined(AFX_VTRLLISTCTRL_H__9F8BE06E_8C6B_44FF_B13A_8D6C21FB082A__INCLUDED_)
#define AFX_VTRLLISTCTRL_H__9F8BE06E_8C6B_44FF_B13A_8D6C21FB082A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VtrlListCtrl.h : header file
//
#include <afxtempl.h>

#define WM_ITEM_DRAG WM_USER+310
/////////////////////////////////////////////////////////////////////////////
// CVtrlListCtrl window
typedef struct VtrlItem
{
	int   iImage;
	int   iKey;
	DWORD dwData;
	CString strObjName;
}VtrlItem;

enum switchType
{
	eTypeNormal, //普通
	eTypeKey,	 //正确关键字
	eTypeErrKey, //错误关键字
	eTypeBlock   //关键字区间表达式
};

class AFX_EXT_CLASS CVtrlListCtrl : public CListCtrl
{
// Construction
public:
	CVtrlListCtrl();

private:
	CArray<VtrlItem, VtrlItem&> m_Items;
	CMap<int, int, CString, const CString&> m_keyStrings;
	DWORD   m_dwItemCount;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVtrlListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateListCtrl();
	BOOL DeleteAllItems();
	BOOL DeleteItem(int nItem);
	BOOL SetItemData(int nItem,DWORD dwData);	
	int  InsertItem(int nItem, int iKey, int iImage);
	int  InsertItem(int nItem, int iKey, int iImage, CString strObjName);
	void ScrollToItem(int index);
	BOOL GetStringAt(int iKey, CString &str);
	void SetStringAt(int iKey, const CString &str);
	virtual ~CVtrlListCtrl();

	//
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect);
	
	//
	COLORREF crWindowText,crWindow,crHighLight,crHighLightText;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CVtrlListCtrl)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL m_bDraging;
	int  m_nDownItem;
	int  m_nUpItem;
	void DrawLineInvert(int x0,int y0,int x1,int y1);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VTRLLISTCTRL_H__9F8BE06E_8C6B_44FF_B13A_8D6C21FB082A__INCLUDED_)
