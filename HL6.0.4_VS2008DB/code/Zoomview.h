#if !defined(AFX_ZOOMVIEW_H__E36DD80E_3D6E_4D53_906D_582128D59DF7__INCLUDED_)
#define AFX_ZOOMVIEW_H__E36DD80E_3D6E_4D53_906D_582128D59DF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZoomView view

#include "XDC.h"
#include "Quad.h"
#include "MeasureLine.h"
#include "ZoomViewB.h"

class CZoomView : public CZoomViewB
{

protected:
	CZoomView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CZoomView)

// Operations
public:
	XDC *PrepareDC(CDC *pDC);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CZoomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
protected:
	//{{AFX_MSG(CZoomView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
	void DrawSelQuadList();
	void UpdateSelQuad();
	void OnArrayPro(CPoint &point);

	// ’Û¡–‘ˆº”
	void SetArrayDlg(CWnd *pWnd);
	BOOL Track(CPoint &point, CRect &rect, CWnd *pWnd);

public:
	CQuad m_SelQuad;
	CWnd  *m_pArrayWnd;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMVIEW_H__E36DD80E_3D6E_4D53_906D_582128D59DF7__INCLUDED_)
