// Stroke.h: interface for the CStroke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STROKE_H__61FEE585_4852_11D4_A483_00105A615525__INCLUDED_)
#define AFX_STROKE_H__61FEE585_4852_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Quad.h"	// Added by ClassView
#include "Dot.h"	// Added by ClassView
#include "Property.h"
#include "Layer.h"
#include "XDC.h"
#include "StrokeB.h"

typedef CArray<CDot,CDot> DotList;

class CStroke : public CStrokeB  
{
	DECLARE_SERIAL(CStroke)

public:
	CStroke(CProperty pty);	
	CStroke();
	virtual ~CStroke();
	
    CStroke& operator  = (const CStroke &stroke);
	CStroke& operator  = (const CStrokeB &stroke);

	void AddRect(CWnd *pWnd);
	BOOL Click(CPoint &point,CWnd *pWnd);
	void DelRect(CWnd *pWnd);
	virtual BOOL IsOnDot(CPoint &point,CWnd*pWnd);
	BOOL AddPoint(CPoint &point,CWnd *pWnd);
	BOOL DotRight(CDot &dot,CWnd *pWnd);
	void SplitDot(int nBegin,int nEnd,CDot &dt1,CDot &dt2);
	void AddStroke(CStroke *pStroke);
	void ConnectHeadToHead(CStroke *pStroke);
	void ConnectTailToTail(CStroke *pStroke);
	
	void Draw(XDC *pDC,int nFont = 0, BOOL bSort = FALSE);
	int  GetMarkUnit();
	
	void MarkPointPreview(CDC *pDC, const int &nPreviewSpeed);
	void FilterExcursion(StrokeList& listData);
	
	BOOL ISOutLineStroke(StrokeList *plistData);
	void FilterUnAvlityDot(DotExList& dotlist,StrokeList& listdata,int& nCount);
	void SetDotToStart(CDot &dt);
	virtual CStrokeB* Copy();
	CQuad CalLimit();
	CQuad CalLimit(CDot dot,double dbXScale = 1,double dbYScale = 1);
	void CopyStroke(const CStroke *pStroke);
	void CopyStrokeB(const CStrokeB *pStroke);
	
public:
	CQuad m_quadLimit;
	BOOL  m_bCharEnd;
	BOOL  m_bFillFirst;
};

#endif // !defined(AFX_STROKE_H__61FEE585_4852_11D4_A483_00105A615525__INCLUDED_)
