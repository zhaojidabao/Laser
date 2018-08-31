#if !defined(AFX_ZOOMVIEWB_H__05B3A639_E53E_47F5_9450_F14E25D7E9A0__INCLUDED_)
#define AFX_ZOOMVIEWB_H__05B3A639_E53E_47F5_9450_F14E25D7E9A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomViewB.h : header file
//
#include "MeasureLine.h"
#include "..\ExportFile\XDC.h"
#include "..\ExportFile\QuadB.h"

enum MODESELECT{MODE_ZOOMIN ,MODE_ZOOMOUT,MODE_ZOOMOPER,MODE_ZOOMSEL,MODE_ZOOMOFF};

/////////////////////////////////////////////////////////////////////////////
// CZoomViewB view

class AFX_EXT_CLASS CZoomViewB : public CView
{
protected:
	CZoomViewB();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CZoomViewB)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomViewB)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CZoomViewB();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CZoomViewB)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void DrawRuler(CDC *pDC);
	void DrawMark(CDC *pDC);
	void DrawCurPos(CDC *pDC,POINT pt);

	void DrawLine(CDC *pDC,int x,int y,int x2,int y2);
	void DrawText(CDC *pDC,int x,int y,double angle,TCHAR *str);

	UINT GetZoomMode();
	void SetZoomMode(UINT mode);
	void ZoomIn();
	void ZoomOut();
	void Drag();	
	void NoDrag();

	void ViewDPtoLP (LPPOINT lpPoints, int nCount = 1);
	void ViewLPtoDP (LPPOINT lpPoints, int nCount = 1);
	void DoZoomIn(double x,double y,double CentX=0,double CentY=0);

    // 点转换
	void PTLogicToPhysic(POINT* pt);
	void PTPhysicToLogic(POINT* pt);
    //
	void LogicToPhysic(QUAD &quad);
	void PhysicToLogic(QUAD &quad);
	// 坐标位置
	void LogicToPhysic(double& x,double& y);
	void PhysicToLogic(double& x,double& y);
	
	void LenLogicToPhysic(double* pdbVal,int nCount=1);
	void LenPhysicToLogic(double* pdbVal,int nCount=1);

	void ActualLen(double &dbx);
	void SetViewScale(double dbScale);
	void DoMeasureLine();
	void DrawMeasureLine(CDC *pDc);
	void MoveMeasureLine(CPoint ptDown);
	BOOL DrawMeasureLineTemp(LONG lOldPos, BOOL &bHasOldPos, LONG lNewPos, UINT nLineType, CPoint point);

	void AlignMeasureLineCenter(CPoint &point, CPoint ptDown, CQuadB quad);
	void AlignMeasureLineTopBottom(CPoint &point);
	void AlignMeasureLineLeftRight(CPoint &point);
	void AlignMeasureLine(int nResult, CPoint &point, CPoint ptDown, CQuadB quadTemp);

	void LastBrush(CWnd *pWnd, CPoint &ptf, CPoint &ptl);
	void DrawRect(CWnd *pWnd,CPoint &ptf,CPoint &ptl,CPoint &point);
	void FlashCurPos(POINT pt);
	
public:
	BOOL	m_bDrag;
	BOOL	m_bLBDowm;
	POINT	m_ptPrev;
	POINT	m_ptNow;
	POINT	m_ptResev;
	POINT	m_ptCurPos;
	int		m_nRulerWidth;
	double	m_dbViewScale;		//在ZOOMIN/OUT时用的系数
	MODESELECT m_ModeSelect;
	BOOL	m_bRButDown;	 	//鼠标右键按下

	CRect	m_rect;
	XDC		m_DC;

	CPoint	m_ptSPrev;
	CPoint	m_ptSNow;
	CMeasureLine* m_arrayMeasureLine;						//测量线条
	int		m_nMeasureLineCount;
	BOOL	m_bCanMoveMeasureLine;							//可以移动线条
	BOOL	m_bCanSetLinePos;								//可以设置线条位置
	int		m_nMeasureLineIndex;							//线条索引
	BOOL	m_bMeasureLineX;
	BOOL	m_bMeasureLineY;
	double	m_dbMeasureLineX;
	double	m_dbMeasureLineY;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMVIEWB_H__05B3A639_E53E_47F5_9450_F14E25D7E9A0__INCLUDED_)
