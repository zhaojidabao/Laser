// PathOptimize.h: interface for the CPathOptimize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHOPTIMIZE_H__3261E626_5E3B_4D4F_9484_22D73BB2FB78__INCLUDED_)
#define AFX_PATHOPTIMIZE_H__3261E626_5E3B_4D4F_9484_22D73BB2FB78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuadB.h"
#include "StrokeB.h"
#include "DataMng.h"

class  AFX_EXT_CLASS CPathOptimize : public CObject  
{
DECLARE_SERIAL(CPathOptimize)
public:
	CPathOptimize();
	virtual ~CPathOptimize();

public:
	CQuadB m_quadLimit;
	StrokeList m_strokelist;
    CDot m_dotHead;
	CDot m_dotTail;
	
	//
	BOOL CheckStrokeIsInQuad(CStrokeB *pStorke);
	void AddStroke(CStrokeB *pStorke);
	void Optimize(int nType = 0);
	void Optimize001();
	void Optimize002();
	
	void InverseStrokeArray(StrokeList *plist);
	
	void GetTheMinDot(DotList *plist,CDot dot, double &dbLengthMin,int &nPos);
	POSITION GetStrokeAt(StrokeList *plist,int nPos);
	void NormalPath();

protected:
	CDataMng m_dataMng;
};

#endif // !defined(AFX_PATHOPTIMIZE_H__3261E626_5E3B_4D4F_9484_22D73BB2FB78__INCLUDED_)
