#if !defined(__XCBAR_H__)
#define __XCBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Sizecbar.h"
/////////////////////////////////////////////////////////////////////////
// XBtn (button info) helper class

/////////////////////////////////////////////////////////////////////////
// XControlBar control bar

#ifndef baseSCB_Cool
#define baseSCB_Cool CSizingControlBar
#endif

class AFX_EXT_CLASS XControlBar : public baseSCB_Cool
{
	class XBtn
	{
	public:
		XBtn();

		void Move(CPoint ptTo) {ptOrg = ptTo; };
		CRect GetRect() { return CRect(ptOrg, CSize(11, 11)); };
		void Paint(CDC* pDC);

		BOOL    bPushed;
		BOOL    bRaised;

	protected:
		CPoint  ptOrg;
	};

    DECLARE_DYNAMIC(XControlBar);

// Construction
public:
    XControlBar();

// Attributes
public:
    virtual BOOL HasGripper() const;

// Operations
public:

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
    virtual BOOL CreateEx(LPCTSTR lpszWindowName, CWnd* pParentWnd,UINT nID,
				CSize sizeDefault=CSize(200,200),UINT nMode=0, 
				DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(XControlBar)
    //}}AFX_VIRTUAL

// Implementation
public:
	void SetSize(CSize size);
	void SetChildWnd(CWnd* pWnd);
    virtual ~XControlBar();
    
protected:
    // implementation helpers
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
	void NcPaintMode0(CDC* pDC, CRect rcClient);
	void NcPaintMode1(CDC* pDC, CRect rcClient);
	void NcPaintMode2(CDC* pDC, CRect rcClient);
    virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

protected:
    int     m_cyGripper;
    XBtn m_biHide;
	CString m_strCaption;
	UINT m_nMode;
    BOOL    m_bActive; // a child has focus

	CWnd* m_pWndChild;

// Generated message map functions
protected:
    //{{AFX_MSG(XControlBar)
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
	void DrawCaption(CDC* pDC,CRect rt);
};

#endif

