// GSpline.h: interface for the CGSpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GSPLINE_H__76DFA306_124E_4C68_ACBE_3B4280507CB1__INCLUDED_)
#define AFX_GSPLINE_H__76DFA306_124E_4C68_ACBE_3B4280507CB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dot.h"
#include "StrokeB.h"


class AFX_EXT_CLASS CGSpline
{
public:
	CGSpline();
	virtual ~CGSpline();
		
	//
	double m_dbPrecision;
	int m_nSampleCount;
	
	//
	CDot UnitVec(CDot dotStart, CDot dotEnd);
	double CalDistance(CDot dot2,CDot dot1);
	void CreateSPline(CArray<CDot,CDot> &dotArray, CStrokeB *pStroke);	
	void CreatePrevSPline(CArray<CDot,CDot> &dotSourthArray, CStrokeB *pStroke, CDot dot);
	void MakeStroke(CDot *pFitPoint,double* pWeight,int nFPNum,int nDegree,CDot T0,CDot Tn,CStrokeB *pStroke,BOOL bLast = FALSE);
	void CalCtrlPoint(CDot *pFitPoint,double* pKnot,int nFPNum,CDot T0,CDot Tn,CDot * ctrlPoint);
	void CalKnotValue(CDot *pFitPoint, int nFPNum, int nDegree, double* pKnot);	
	void CalCoeffMatrix(double* pKnot, int nFPNum, double a[]);	
	void CalRightVector(double* pKnot, CDot* pFitPoint, int nFPNum, CDot T0, CDot Tn, CDot b[]);
	void ludcmp(double a[],int n,int indx[], int& d);
	void lubksb(double a[],int n,int indx[],double b[]);
	BOOL CreateDotList(double* pCPx,double* pCPy,double* pKnot,double* pWeight,int nDegree,int nCPNum,double dbPrecision,CStrokeB *pStroke);
	double CalLength(double* pPvalX,double* pPvalY,double* pKnot,double* pWeight,int startKnot,int nCount);
	void CalSPlineX(double* pPval,double* pKnot,double* pWeight,int startKnot,double val[],int nCount);	
	double CalSplineCoord(double* pPval,double* pKnot,double* pWeight,int startKnot,double t);
};

#endif // !defined(AFX_GSPLINE_H__76DFA306_124E_4C68_ACBE_3B4280507CB1__INCLUDED_)
