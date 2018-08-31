// MgrPathOptimize.h: interface for the CMgrPathOptimize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MGRPATHOPTIMIZE_H__F00E2022_4BFC_42ED_9584_8D754323F066__INCLUDED_)
#define AFX_MGRPATHOPTIMIZE_H__F00E2022_4BFC_42ED_9584_8D754323F066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuadB.h"
#include "PathOptimize.h"
#include "StrokeB.h"


class AFX_EXT_CLASS CMgrPathOptimize  
{
public:	
	CMgrPathOptimize();
	virtual ~CMgrPathOptimize();
	
	//
	void OptimizeProcess(StrokeList* plist);
	void OptimizeProcess(CArray<CStrokeB*,CStrokeB*> &arrayStroke);

	void SetStrokeList();
	void SetStrokeList(StrokeList* plist);
	void SetStrokeList(CArray<CStrokeB*,CStrokeB*> &arrayStroke);
	
	void CalLimit();
	void BreakQuad();
	void BreakQuad(double dbQuadLength);
	void AddStrokeToQuad(CPathOptimize* pPathOptimize);
	void Optimize();	
	void LoadStrokelist(StrokeList *pListStroke);
	void OptimizeQuad();
	void OptimizeQuad001();
	void OptimizeQuad002();
	void GetTheMinDot(DotList *plist,CDot dotStart, double &dbLengthMin,int &nPos);
	POSITION GetPathOptimizeAt(int nPos);
	void CreateOtherQuad();
	
	void NormalPath(StrokeList *plist);
	void NormalPath(CArray<CStrokeB*,CStrokeB*> &arrayStroke);
	
public:
	CQuadB m_quadLimit;
	StrokeList m_strokelist;
	CTypedPtrList<CObList,CPathOptimize*>m_listPathOptimize;
	CTypedPtrList<CObList,CPathOptimize*>m_listPathOptimizeTemp;

	//
	int  m_nColCount;
	int  m_nRowCount;
	CPathOptimize *m_pPathOptimize;
	double m_dbQuadLength;
	double m_dbRealQuadLengthX;
	double m_dbRealQuadLengthY;

protected:
	CDataMng m_dataMng;
};

#endif // !defined(AFX_MGRPATHOPTIMIZE_H__F00E2022_4BFC_42ED_9584_8D754323F066__INCLUDED_)
