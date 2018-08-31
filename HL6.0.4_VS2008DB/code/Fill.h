

#ifndef FENCER_FILL_H_20060902
#define FENCER_FILL_H_20060902

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Quad.h"
#include "Stroke.h"
#include "FillB.h"

#define FILL_BACK	1     //往复填充
#define FILL_CROSS	2     //交叉填充
#define FILL_EVEN	4     //线间距自修正
#define FILL_MORD	8	  //打标顺序，先打填充线再打标边框
#define FILL_PTP	16    //光栅
#define FILL_CIRCLE	32    //连接
#define FILL_BETTER	64    //优化


class CFill : public CFillB
{
public:
	CFill(double space,double dbBorderSpace,double dbAngle
		,UINT style,CProperty property,double dbPtp, double dbCircle,double dbHeadKillLen,double dbTailKillLen);
	CFill(NEW_FILL_DATA newFillData, CProperty property);
	virtual ~CFill();
	
public:
	CQuad m_quadLimit;
public:
	void Proc(StrokeList* plistData,StrokeList& listFillData);
	void Proc(StrokeList *plistData,StrokeList &listFillData, double dbFillAngle, BOOL bFillFirst);
	void CirclProc(StrokeList *pList, StrokeList *pRetList, double dbLen);
	void CirclLinePro(CArray<StrokeList*, StrokeList*> &RetList, StrokeList& listFillData);
	CQuad CalLimit();
};

#endif
