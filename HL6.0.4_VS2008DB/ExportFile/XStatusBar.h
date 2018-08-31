// XStatusBar.h: interface for the CXStatusBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSTATUSBAR_H__BBEC742A_E03F_49BC_8A89_11FF0A793B34__INCLUDED_)
#define AFX_XSTATUSBAR_H__BBEC742A_E03F_49BC_8A89_11FF0A793B34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ENUMXSTATUSBAR
{
	SBACTF_NORMAL         = 0x0000, 
	SBACTF_AUTOFIT        = 0x0001,
	SBACTF_COMMAND	      = 0x0002,
	SBACTF_HANDCURSOR     = 0x0004,
	
	SBACTF_DOUBLECLICK	  = 0x0100,
	SBACTF_SINGLECLICK	  = 0x0200, 
	
	SBACTF_LEFTBUTTON     = 0x1000,
	SBACTF_RIGHTBUTTON	  = 0x2000, 
	SBACTF_MIDDLEBUTTON	  = 0x4000, 
	
	SBACTF_STYLEFLAGMASK  = 0x00FF,
	SBACTF_CLICKFLAGMASK  = 0x0F00,
	SBACTF_BUTTONFLAGMASK = 0xF000,
};



class AFX_EXT_CLASS CXStatusBar : public CStatusBar  
{
public:
	CXStatusBar();
	virtual ~CXStatusBar();

public:
	int GetPanesCount() const{
		return m_nCount;
	}
	
	void SetPaneWidth(int nIndex, int nWidth)
	{
		_STATUSBAR_PANE_ pane;
		PaneInfoGet(nIndex, &pane);
		pane.cxText = nWidth;
		PaneInfoSet(nIndex, &pane);
	}
	
	BOOL AddPane(
		UINT nID,	// ID of the  pane
		int nIndex	// index of the pane
		);

	BOOL RemovePane(
		UINT nID	// ID of the pane
		);

	BOOL AddPaneControl(CWnd* pWnd, UINT nID, BOOL bAutoDestroy)
	{
		return AddPaneControl( pWnd->GetSafeHwnd(), nID, bAutoDestroy);
	}
	
	BOOL AddPaneControl(HWND hWnd, UINT nID, BOOL bAutoDestroy);
	
	void DisableControl( int nIndex, BOOL bDisable=TRUE)
	{
		UINT uItemID = GetItemID(nIndex);
		for ( int i = 0; i < m_arrPaneControls.GetSize(); i++ ){
			if( uItemID == m_arrPaneControls[i]->nID ){
				if( m_arrPaneControls[i]->hWnd && ::IsWindow(m_arrPaneControls[i]->hWnd) ) {
					::EnableWindow(m_arrPaneControls[i]->hWnd, bDisable); 
				}
			}
		}
	}

	void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth)
	{
		CStatusBar::SetPaneInfo(nIndex, nID, nStyle, cxWidth);
		BOOL bDisabled = ((nStyle&SBPS_DISABLED) == 0);
		DisableControl(nIndex, bDisabled);
	}

	void SetPaneStyle(int nIndex, UINT nStyle)
	{
		CStatusBar::SetPaneStyle(nIndex, nStyle);
		BOOL bDisabled = ((nStyle&SBPS_DISABLED) == 0);
		DisableControl(nIndex, bDisabled);
	}
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtStatusControlBar)
	//}}AFX_VIRTUAL

// Implementation

protected:

	struct _STATUSBAR_PANE_
	{
		_STATUSBAR_PANE_(){
			nID = cxText = nStyle = nFlags = 0;
		}
		
		UINT    nID;        // IDC of indicator: 0 => normal text area
		int     cxText;     // width of string area in pixels
		//   on both sides there is a 3 pixel gap and
		//   a one pixel border, making a pane 6 pixels wider
		UINT    nStyle;     // style flags (SBPS_*)
		UINT    nFlags;     // state flags (SBPF_*)
		CString strText;    // text in the pane
	};
	
	struct _STATUSBAR_PANE_CTRL_
	{
		HWND hWnd;
		UINT nID;
		BOOL bAutoDestroy;		
	};
	
	CArray < _STATUSBAR_PANE_CTRL_*, _STATUSBAR_PANE_CTRL_* > m_arrPaneControls; 
	
	_STATUSBAR_PANE_* GetPanePtr(int nIndex) const
	{
		ASSERT((nIndex >= 0 && nIndex < m_nCount) || m_nCount == 0);
		return ((_STATUSBAR_PANE_*)m_pData) + nIndex;
	}
	
	BOOL PaneInfoGet(int nIndex, _STATUSBAR_PANE_* pPane);
	BOOL PaneInfoSet(int nIndex, _STATUSBAR_PANE_* pPane);
	
	void RepositionControls();
	
public:
	void AutoFitPane(int nIndex);
	int HitTest(CPoint point);
	BOOL SetPaneCursorIndex(int nIndex, UINT nCursorID);
	BOOL SetPaneCursorIndex(int nIndex, HCURSOR hCursor);
	HCURSOR GetPaneCursorIndex(int nIndex);
	DWORD GetPaneFlagsIndex(int nIndex);
	void SetHandCursor(HCURSOR hCursor, BOOL bTryDefault = TRUE);
	void SetITCursor(DWORD dwCursorID);
	BOOL SetHandCursor(LPCTSTR lpszCursorID, BOOL bTryDefault = TRUE);
	BOOL SetHandCursor(UINT nCursorID, BOOL bTryDefault = TRUE);
	void SendPaneCommand(CPoint point, UINT message);
	BOOL SetPaneFlagsIndex(int nIndex, DWORD dwFlags = SBACTF_NORMAL);
	void UpdateTooltipRects();
	BOOL SetPaneTooltipIndex(int nIndex, LPCTSTR lpszText = NULL);
	BOOL SetPaneTooltipIndex(int nIndex, UINT nTipID);
	
public:
	BOOL m_bTryDefaultHandCursor;
	HCURSOR m_hCursorHand;
	CArray<HCURSOR, HCURSOR> m_adwCursors;
	CArray<DWORD, DWORD> m_adwFlags;
	CToolTipCtrl m_tooltip;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusBarACT)
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CExtStatusControlBar)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

};

#endif // !defined(AFX_XSTATUSBAR_H__BBEC742A_E03F_49BC_8A89_11FF0A793B34__INCLUDED_)
