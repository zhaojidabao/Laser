// GArc.h: interface for the CGArc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GARC_H__370C756A_A6A7_493D_BE22_3CA016F6E392__INCLUDED_)
#define AFX_GARC_H__370C756A_A6A7_493D_BE22_3CA016F6E392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dot.h"
#include "StrokeB.h"



class AFX_EXT_CLASS CGArc
{
public:
	CGArc();
	virtual ~CGArc();
	
	//
	CDot UnitVec(CDot dotStart, CDot dotEnd);
	CDot UnitVec(CDot dot);
	double CalDistance(CDot dot2,CDot dot1);
	CDot RotateVector(CDot vec, double rotA);
	double GetCenterAngle(CDot dot1, CDot dot2, CDot dot3, CDot dotCenter, int nType);
	CDot AjustDot(CDot dot1, CDot dot2, CDot dot3);
	int GetQuadrantNo(CDot dot);
	double GetVectorAngleX(CDot dot);
	BOOL GetCenter(CDot dot1, CDot dot2, CDot dot3, CDot &dotCenter);
	
	//
	void CreateArcline(CDot dot1, CDot dot2, CDot dot3, CStrokeB *pStroke, int nType);
	CDot *_CalCtrlPoint(CDot dotStart, CDot dotCenter, CDot dotEnd, double centerAngle);
	double* _CalWeightValue(double centerA);
	double* _CalKnotValue(double centerA);
	
	//
	double m_dbPrecision;
	int m_nSampleCount;
};

#endif // !defined(AFX_GARC_H__370C756A_A6A7_493D_BE22_3CA016F6E392__INCLUDED_)
