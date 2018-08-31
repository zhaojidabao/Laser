// HLView.h : interface of the CHLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLVIEW_H__B8BC1851_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_HLVIEW_H__B8BC1851_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Zoomview.h"
#include "HLDoc.h"
#include "XDC.h"
#include "XTracker.h"
#include "Quad.h"	// Added by ClassView
#include "VectorMgr.h"

class CHLView : public CZoomView
{
protected: // create from serialization only
	CHLView();
	DECLARE_DYNCREATE(CHLView)

// Attributes
public:
	CHLDoc* GetDocument();
// Operations
public:
	virtual void NotifyZoom(void);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void JoinPoint();
	void SplitPoint(double dbPoint);
	void AddPoint(CPoint &point);
	void OnChangeconvert();
	void DrawRect();
	BOOL HitOtherObject(CPoint &point);
	void DelDot();
	void ReplaceDot();
	void DrawRotateTrack(XDC *pDC);
	BOOL IsBitSigned(unsigned short n, unsigned int nBit);
	void DrawIcon(CDC *pDC, int x, int y, unsigned short *pIconMap);
	void RotatePreviewStrokeList(StrokeList *pStrokeList,CDot CenterPoint, double dbAngle);
	void EmptyStroke(StrokeList *pStrokeList);
	void PreviewStroke(StrokeList *pStrokeList);
	void InitIcon();
	void PreviewRotateTracker(CQuad rect);
	double CalAngle(double cx, double cy);
	void TrackRotate(CPoint OrgPoint,BOOL bShift,BOOL bControl);
	void DrawBox(XDC *pDC);

	void DrawMark(CDC* pDC);
	void SortPro();
	void AutoSortPro();
	void AutoSortDraw(CDC *pDc);

	void SetGroupNum(int nNum);
	void ClearMark();

	void AlignBottomCmd();
	void AlignTopCmd();
	void AlignRightCmd();
	void AlignLeftCmd();
	void DistanceYCmd();
	void DistanceXCmd();
	void EqualWhCmd();
	void EqualHeightCmd();
	void EqualWidthCmd();
	
	void SetCurCursorClick(){m_hCursorCur = m_hCursorClick;};

	virtual ~CHLView();
	int m_nResult;
	XTracker m_track;
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL OpenSelectFile(CHLDoc *pDoc,TCHAR* pszFilter,CString &strPath, CString& strFileExt,BOOL &bCenter,BOOL &bOptimize);

protected:
	//{{AFX_MSG(CHLView)
	afx_msg void OnUpdatePos(CCmdUI *pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomSelect();
	afx_msg void OnZoomAll();
	afx_msg void OnZoomPage();
	afx_msg void OnZoomoper();
	afx_msg void OnZoomoff();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint( ); 
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomoper(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomoff(CCmdUI* pCmdUI);
	afx_msg void OnDrawClick();
	afx_msg void OnUpdateDrawClick(CCmdUI* pCmdUI);
	afx_msg void OnDrawHand();
	afx_msg void OnUpdateDrawHand(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawRectangle();
	afx_msg void OnUpdateDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDrawText();
	afx_msg void OnUpdateDrawText(CCmdUI* pCmdUI);
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFileImport();
	afx_msg void OnFileImportVector();
	afx_msg void OnFileImportBmp();
	afx_msg void OnFileImportNc();
    afx_msg void OnLoadObjList();
	afx_msg void OnDrawGrid();
	afx_msg void OnUpdateDrawGrid(CCmdUI* pCmdUI);
	afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateDrawPolygon(CCmdUI* pCmdUI);
	afx_msg void OnDrawCodebar();
	afx_msg void OnUpdateDrawCodebar(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHelppath();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDrawDrag();
	afx_msg void OnUpdateDrawDrag(CCmdUI* pCmdUI);
	afx_msg void OnMoveaccu();
	afx_msg void OnConvert();
	afx_msg void OnDeldot();
	afx_msg void OnJoindot();
	afx_msg void OnSplitdot();
    afx_msg void OnSelAllStroke();
    afx_msg void OnSetCurDotToStart();
    afx_msg void OnFilterPoint();
    afx_msg void OnReverseStroke();
    afx_msg void OnFilterCurDocPoint();
    afx_msg void OnKillAngle();
    afx_msg void OnSortObj();
    afx_msg void OnUpdateSortObj(CCmdUI* pCmdUI);
	afx_msg void OnAutoSortObj();
    afx_msg void OnUpdateAutoSortObj(CCmdUI* pCmdUI);
	afx_msg void OnSelSortObj();
    afx_msg void OnUpdateSelSortObj(CCmdUI* pCmdUI);
	afx_msg void OnAlignBottom();
	afx_msg void OnAlignLeft();
	afx_msg void OnAlignRight();
	afx_msg void OnAlignTop();
	afx_msg void OnDistanceX();
	afx_msg void OnDistanceY();
	afx_msg void OnEqualHeight();
	afx_msg void OnEqualWidth();
	afx_msg void OnEqualWh();
	afx_msg void OnAlignX();
	afx_msg void OnAlignY();
	afx_msg void OnJointstrokeCurdoc();
	afx_msg void OnSetUserfont();
	afx_msg void OnDrawSpline();
	afx_msg void OnSplineEdit();
	afx_msg void OnDrawArc3pt();
	afx_msg void OnUpdateDrawArc3pt(CCmdUI* pCmdUI);
	afx_msg void OnDrawArcCenter();
	afx_msg void OnUpdateDrawArcCenter(CCmdUI* pCmdUI);
	afx_msg void OnDrawCircle();
	afx_msg void OnUpdateDrawCircle(CCmdUI* pCmdUI);
	afx_msg void OnArcEdit();
	afx_msg void OnCircleEdit();
	afx_msg void OnPrecisionSet();
	afx_msg void OnUpdateDrawSpline(CCmdUI* pCmdUI);
	afx_msg void OnDrawDot();
	afx_msg void OnUpdateDrawDot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawTimer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawInput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawOutput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawFill(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawBmp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEqualWidth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEqualWh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEqualHeight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignTop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignX(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignY(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignBottom(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDistanceX(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDistanceY(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConvert(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilterCurdocPoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePathNormal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetUserfont(CCmdUI* pCmdUI);
	afx_msg void OnDrawMotormove();
	afx_msg void OnUpdateDrawMotormove(CCmdUI* pCmdUI);
	afx_msg void OnDrawSetpos();
	afx_msg void OnUpdateDrawSetpos(CCmdUI* pCmdUI);
	afx_msg void OnDrawCondition();
	afx_msg void OnUpdateDrawCondition(CCmdUI* pCmdUI);
	afx_msg void OnDrawRange(UINT nID);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
	
public:
	void InvalidateSort();
	void DispSelObjProp();
	CString m_strAutoSortInfo;
	UINT m_nDrawTool;			//当前的绘图工具
	
private:
	void DrawTracker(XDC *pDC);
	void DrawFrame(XDC *pDC);
	void DrawDistortion(XDC *pDC);

	HCURSOR m_hCursorCur;         // 当前光标
	HCURSOR m_hCursorClick;	      // 点取光标
	HCURSOR m_hCursorShape;		  // 结点光标
	HCURSOR m_hCursorCross;		  // 十字线光标
	HCURSOR m_hCursorCrossRot;	  //
	HCURSOR m_hCursorHorDrag;	  // 水平拖动光标
	HCURSOR m_hCursorVerDrag;	  // 垂直拖动光标
	HCURSOR m_hCursorHVDrag1;	  // 斜面拖动光标
	HCURSOR m_hCursorHVDrag2;	  // 斜面拖动光标
	HCURSOR m_hCursorDoxMove;     // 画布移动
	HCURSOR m_hCursorZoomIn;      // 局部放大
    HCURSOR m_hCursorMLineH;      // 局部放大
    HCURSOR m_hCursorMLineV;      // 局部放大
	HCURSOR m_hCursorRotateDrag;
	HCURSOR m_hCursorTBDrag;
	HCURSOR m_hCursorLRDrag;
	HCURSOR m_hCursorText;
	HCURSOR m_hCursorBar;      
	HCURSOR m_hCursorEllipse;  
	HCURSOR m_hCursorPoly;     
	HCURSOR m_hCursorRect;     
	HCURSOR m_hCursorGrid;     
	HCURSOR m_hCursorDot;      
	HCURSOR m_hCursorSpline;
	HCURSOR m_hCursorArc;
	HCURSOR m_hCursorViewRotate;
	
   	int			m_nRotateDragType;
	BOOL		m_bRotate;
	CPoint	m_ptCur;			  // 当前位置
	HCURSOR m_hDrag;

    CStroke	m_RotateRectStroke; 
	unsigned short m_DragRotateIcon[8][14]; 
	CPoint	m_ptSave;

	void Track3D(CPoint point);

private:
	CQuad m_quadDown;
	BOOL  m_bLBDown;
	BOOL  m_bActive;
	CDC*  m_pDC;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnDrawBufferlaser();
	afx_msg void OnUpdateDrawBufferlaser(CCmdUI *pCmdUI);
	afx_msg void OnDrawCcd();
	afx_msg void OnUpdateDrawCcd(CCmdUI *pCmdUI);
	afx_msg void OnDrawPoscali();
	afx_msg void OnUpdateDrawPoscali(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in HLView.cpp
inline CHLDoc* CHLView::GetDocument()
   { return (CHLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HLVIEW_H__B8BC1851_DE24_11D3_845D_444553540001__INCLUDED_)
