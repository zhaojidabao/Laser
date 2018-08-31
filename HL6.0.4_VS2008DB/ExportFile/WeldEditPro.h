#pragma once

#include "DataMng.h"

// CWeldEditPro ÃüÁîÄ¿±ê

class AFX_EXT_CLASS CWeldEditPro : public CObject
{
public:
	CWeldEditPro();
	virtual ~CWeldEditPro();
	
	void Weld( CArray<StrokeList*, StrokeList*> &arrayListSource, StrokeList &listRet );
	void Weld( StrokeList &listSource );

private:
	void FindSingStroke( StrokeList &listSource, StrokeList &listSingle, StrokeList &listWork );
	BOOL IsSingStroke( CStrokeB *pStroke, StrokeList &listSource );

	void CreateTouchArrayList( CArray<StrokeList*, StrokeList*> &arrayListSource, StrokeList &listSource );
	BOOL CreateTouchList( StrokeList &listSource, StrokeList &listTouch );
	BOOL CreateTouchList( StrokeList &listCheck, StrokeList &listSource, StrokeList &listTouch );
	BOOL CreateTouchList( CStrokeB *pStrokeCheck, StrokeList &listSource, StrokeList &listTouch );
	void ReMoveWithStyle( StrokeList &listSource, int nStyle );
		
	void DoWeld( StrokeList &listStroke );
	void DoWeld( CStrokeB *pStrokeWeld, StrokeList &listStrokeSource, StrokeList &listStrokeNew, int nPosWeld );
	int DotOnPolygon( CDot dotCenter, CStrokeB *pStroke, StrokeList &listStroke, int nPosWeld );
	BOOL DotInPolygon( CDot dotCenter, CStrokeB *pStroke, StrokeList &listStroke, int nPosWeld );
	
	void CalCrossList( StrokeList &listStroke );
	void CalCrossList( CStrokeB *pStroke, StrokeList &listStroke, StrokeList &listStrokeNew );
	void CalCrossList( CStrokeB *pStroke, int nPos, StrokeList &listStroke, CArray<CDot,CDot> &dotArray );
	void ArrangeDotArray( CDot dotStart, CArray<CDot,CDot> &dotArray );

private:
	CDataMng m_dataMng;
};


