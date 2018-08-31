

#ifndef FENCER_FILL_H_20060902
#define FENCER_FILL_H_20060902

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Quad.h"
#include "Stroke.h"
#include "FillB.h"

#define FILL_BACK	1     //�������
#define FILL_CROSS	2     //�������
#define FILL_EVEN	4     //�߼��������
#define FILL_MORD	8	  //���˳���ȴ�������ٴ��߿�
#define FILL_PTP	16    //��դ
#define FILL_CIRCLE	32    //����
#define FILL_BETTER	64    //�Ż�


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
