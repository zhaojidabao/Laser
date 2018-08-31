// XTracker.h: interface for the XTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTRACKER_H__D814A30B_F9F7_4FF1_BC2D_CA3850434BCA__INCLUDED_)
#define AFX_XTRACKER_H__D814A30B_F9F7_4FF1_BC2D_CA3850434BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Stroke.h"
#include "QuadB.h"	// Added by ClassView
#include "XTrackerB.h"
#include "QuadRangle.h"

class XTracker: public XTrackerB
{
public:
	XTracker();
	~XTracker();

public:
	int   GetSelCnt();
	void GetSelDot(double& dbX, double& dbY, double& dbZ);
	void ModifyDot(double dbX, double dbY, double dbZ);
	void MoveDot(double MoveX,double MoveY);
	BOOL IsSameNode(CStroke *pStroke1,CStroke *pStroke2);
	void JoinPoint();
	void SplitPoint(double dbPoint);
	void ClearAll();
	BOOL HitValidPoint(CPoint &point,CWnd *pWnd);
	void ModifyStrokeList(CStroke *pStroke);
	void DelDot();
	void AddPoint(CPoint &point,CWnd *pWnd);
	void AddRect(CWnd *pWnd);
	void Edit(CQuad &quad,CWnd *pWnd);
	void SetFlag(BOOL bflag);
	void Edit(CPoint &point,CWnd *pWnd);
	BOOL IsOnDot(CPoint &point,CWnd *pWnd);
	void DelRect(CWnd *pWnd);
	void ClearAndStore(CQuad &quad,CWnd *pWnd);
	BOOL HasDot(CQuad &quad);
	void ClearAndStore(CPoint &point,CWnd *pWnd);
	DWORD FillRect(CWnd *pWnd);
	void SearchDot(CPoint &point,CWnd *pWnd);
	void SearchDot(CQuad &quad);
	void GetDot(CQuad &quad);
	void GetDot(CPoint &point,CWnd *pWnd);
	BOOL HaveObject();
	BOOL HitOtherDot(CPoint &point,CWnd *pWnd);
	BOOL HitSelectDot(CPoint &point,CWnd *pWnd);
	void ClearStrokeList();
	void DrawNode(CPoint &pt1,CPoint &pt2,CWnd *pWnd);
	void SingleModify(CPoint &point,CWnd *pWnd);
	void SetPhysicQuad(CWnd *pWnd,CQuad &quad);
	void GetTrueRect(CRect &rect);
	void InverDraw(CQuad &quad,CWnd *pWnd);
	void Draw(CQuad &quad,CWnd*pWnd);
	void DrawDistortion(XDC *pDC,CQuad quad,CWnd *pWnd);	
	void DrawTracker(XDC *pDC,CQuad &Quad,CWnd *pWnd,BOOL bLock);
	void DrawFrame(XDC *pDC,CQuadRangle &Quad,CWnd *pWnd,BOOL bLock);
	int  GetResult(CPoint &point ,CQuad &quad,CWnd *pWnd);
	int  GetResult(CPoint &point ,CQuadRangle &quad,CWnd *pWnd);
	int  GetDistortionPos(CPoint point ,CQuad &quad,CWnd *pWnd);
	void Handle(int nResult,double &ratiox,double &ratioy,double &MoveX,double &MoveY,CPoint dot,CPoint &point,CQuad &quad,CWnd *pWnd,BOOL bCtrlKey = FALSE);
	BOOL GetDragPara(int &nResult,CQuad &quad,CDot &dot,double &ratioX,double &ratioY,double &MoveX,double &MoveY,CWnd *pWnd);
	BOOL GetDragFrame(int nResult,CQuadRangle &frame,CDot &dot,double &ratioX,double &ratioY,double &MoveX,double &MoveY,CWnd *pWnd);
	BOOL GetDistortion(int nRst,CQuadRangle quad,CDot &dot,double &dbRotateA,double &dbShearXA,double &dbShearYA,CWnd *pWnd);
	
	int  PtHitQuad(CPoint &pt,CQuad &quad,CWnd *pWnd);
	int  PtHitQuad(CPoint &pt,CQuadRangle &frame,CWnd *pWnd);
	
	void AddDotHead(CStroke *pStroke,CDot &dotIndex,CWnd *pWnd);
	void FromHandle(int nResult,double &ratioX,double &ratioY,double &MoveX,double &MoveY, CPoint pointOrg,CPoint &point,CQuad &quad,CDot &dot,CWnd *pWnd, BOOL bCtrlKey = FALSE);
	
	BOOL IsPtInRect(int i,CPoint &point,CTree *pTree,CWnd *pWnd);
	BOOL CtrlPoint(int i,CPoint &point,CTree* pTree,CWnd *pWnd);
	int  HitPoint(CTree* pTree,CPoint &point,CWnd *pWnd);
	BOOL InRect(CPoint &point,CTree *pTree,CWnd *pWnd);
	void UpdateRectCtrl();
	void InverDrawDot(CWnd *pWnd);

	// 查找选中列表中第一个点的序号
	int FindSelListFirstDotNum(int &nNum);
	
	CStroke* FindSelListFirstDot(int &nNum, int &nCount);
	// 通过一个选中的点选中整个笔画
	void SelectStrokeBySelPt(CWnd *pWnd);
	void SetDotToStart(CWnd *pWnd);
	void FilterPoint(CWnd *pWnd, double dbPresic, double dbAngle);
	void KillAngle(CWnd *pWnd);
	void ReverseStroke(CWnd *pWnd);

	void DrawSelTraker(XDC *pDC, CQuad &Quad,CWnd *pWnd, CArray<SELQUAD, SELQUAD> &QuadList,BOOL bLock);
	void DrawSelObjTracker(XDC *pDC, CQuad &Quad,BOOL bFillQuad,CWnd *pWnd);

	CQuad GetQuadSave()
	{
		return m_quadSave;
	}
	void SetQuadSave(CQuad &quad)
	{
		m_quadSave = quad;
	}

	CQuad m_quadOrg;			
	CQuad m_quad[8];
	StrokeList m_StrokeList;//用来放置笔划

private:
	void ModifyDot(int nResult,CDot &dot,CQuad &quad);
	void ModifyDot(int nResult,CDot &dot,CQuadRangle& frame);
	void GetDot(int nResult,CQuadRangle &frame,CDot &dot);
	void DrawQuad(int nResult,CDot &dot,double &ratioX,double &ratioY,double &MoveX,double &MoveY,CQuad &quad1,CQuad &quad2,CWnd *pWnd);
	//8个控点位置 按0，1，2，3为四周控点，4，5，6，7为边上控点(clock-wise)
	

private:
	CQuad m_NewQuad;
	CQuad m_OldQuad;
	CQuad m_quadSave;

public:
	void DrawQuadInvert(CQuadRangle quad, XDC *pXDC);
};

#endif // !defined(AFX_XTRACKER_H__D814A30B_F9F7_4FF1_BC2D_CA3850434BCA__INCLUDED_)
