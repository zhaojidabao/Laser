#if !defined(AFX_DOTINTERFILTER_H__8F6F44FB_365A_405f_950A_553BDDBC4ABF__INCLUDED_)
#define AFX_DOTINTERFILTER_H__8F6F44FB_365A_405f_950A_553BDDBC4ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrokeB.h"

//  write by zhq,2014-05-27 
//  线条及点加粗功能
class AFX_EXT_CLASS CBroadenStroke
{
public:
	CBroadenStroke();
	~CBroadenStroke();
	CBroadenStroke(double r,double d, int nType);
	void SetBroadenPara(double r,double d,double nType);
	void SetCirclePara(double precision,double nSampleCount);

	// 把pListIn中所有线条加粗，结果返回到pListOut中
	BOOL BroadenStrokeList(StrokeList* pListIn,StrokeList* pListOut);
	// 把pStroke加粗，返回结果到pListOut中
	BOOL BroadenStroke(StrokeList* pListOut,CStrokeB* pStroke);

	int BroadenDot(const CDot& ptS,const CDot& ptM,const CDot& ptE, CArray<CDot,CDot>& dotArray,double r);

	// 去掉重复点
	void RemoveSameDot(CArray<CDot, CDot> &dotArray);

	// 去掉共线点
	void RemoveMidDotInLine(CArray<CDot, CDot> &dotArray);

	double GetDistance(CDot &sDot, CDot &eDot);
	// 三点是否共线
	inline BOOL IsLine(const CDot &dotLeft, const CDot &dotMid, const CDot &dotRight);
	BOOL GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot);
	void FreeStrokeList(StrokeList* pList);
public:
	double m_dbBroadenR;	// 加粗半径
	double m_dbBroadenD;	// 加粗间距
	int    m_nType;			// 加粗方式

	double m_dbPrecision;
	int    m_nSampleCount;

};

#endif // !defined(AFX_DOTINTERFILTER_H__8F6F44FB_365A_405f_950A_553BDDBC4ABF__INCLUDED_)