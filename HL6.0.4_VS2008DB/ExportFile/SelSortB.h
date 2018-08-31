// SelSortB.h: interface for the CSelSortB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELSORTB_H__91312440_30B0_4728_AE42_57D9057B0B13__INCLUDED_)
#define AFX_SELSORTB_H__91312440_30B0_4728_AE42_57D9057B0B13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrokeB.h"

class AFX_EXT_CLASS CSelSortB  
{
public:
	CSelSortB();
	virtual ~CSelSortB();

public:
	DWORD  GetSort(CArray<GNODE,GNODE> &ObjList, CStrokeB *pStroke);
	BOOL   GetRectCrossDot(GNODE &gNode, CDot &dtS, CDot &dtE, CArray<GNODE,GNODE> &ObjList);
	BOOL   GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot);
	BOOL   IsCross(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 );
	double Distance(CDot &sDot, CDot &eDot);
	void   Adjust(MSG msg, CPoint pointOld,CPoint &pointNew);

};

#endif // !defined(AFX_SELSORTB_H__91312440_30B0_4728_AE42_57D9057B0B13__INCLUDED_)
