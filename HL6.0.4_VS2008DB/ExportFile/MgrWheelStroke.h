// MgrWheelStroke.h: interface for the CMgrWheelStroke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MGRWHEELSTROKE_H__896EDCD2_D60C_419B_B6F4_8FD5884F8A13__INCLUDED_)
#define AFX_MGRWHEELSTROKE_H__896EDCD2_D60C_419B_B6F4_8FD5884F8A13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrokeB.h"
#include "DataMng.h"

class AFX_EXT_CLASS CMgrWheelStroke  
{
public:
	CMgrWheelStroke();
	virtual ~CMgrWheelStroke();
	
	int CreateWheelObj( double dbRadius, CArray<SORTOBJ,SORTOBJ> &arraySortSour, int &nSize);
	int CreateWheelStroke( CArray<StrokeList*, StrokeList*> &arrayList, double dbDivAngle, double dbDirect, CArray<StrokeList*, StrokeList*> &arrayListSource, int nProcessType, int nProNu=0);	
	void ProcessStroke(StrokeList &listStroke, BOOL bFlag, double dbCenter,double dbRadius);
	void SortStrokeList(CArray<StrokeList*, StrokeList*> &arrayListSource);
	void ProStop(BOOL bFlag);
	void InitParamete();
	
private:
	BOOL CreateNode(CArray<StrokeList*, StrokeList*> &arrayList, CArray<StrokeList*, StrokeList*> &arrayListSource);
	BOOL VerticalDivide(double dbScanLine, double dbScanLineNext, StrokeList &listStroke);
	void ProcessStroke1(StrokeList &listStroke);
	BOOL DivideFillStroke(CArray<StrokeList*, StrokeList*> &arrayList, StrokeList *pListStroke, int nProNo);
	
	int CreateWheelStroke1( CArray<StrokeList*, StrokeList*> &arrayList, double dbDirect, CArray<StrokeList*, StrokeList*> &arrayListSource);
	int CreateWheelStroke2( CArray<StrokeList*, StrokeList*> &arrayList, double dbDivAngle, double dbDirect, CArray<StrokeList*, StrokeList*> &arrayListSource);
	int CreateWheelStroke3( CArray<StrokeList*, StrokeList*> &arrayList, double dbDivAngle, double dbRadius, CArray<StrokeList*, StrokeList*> &arrayListSource, int nProNu);
	
	static BOOL m_bStop;
	CDataMng m_dataMng;
};

#endif // !defined(AFX_MGRWHEELSTROKE_H__896EDCD2_D60C_419B_B6F4_8FD5884F8A13__INCLUDED_)
