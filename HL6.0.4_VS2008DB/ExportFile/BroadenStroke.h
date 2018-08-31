#if !defined(AFX_DOTINTERFILTER_H__8F6F44FB_365A_405f_950A_553BDDBC4ABF__INCLUDED_)
#define AFX_DOTINTERFILTER_H__8F6F44FB_365A_405f_950A_553BDDBC4ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrokeB.h"

//  write by zhq,2014-05-27 
//  ��������Ӵֹ���
class AFX_EXT_CLASS CBroadenStroke
{
public:
	CBroadenStroke();
	~CBroadenStroke();
	CBroadenStroke(double r,double d, int nType);
	void SetBroadenPara(double r,double d,double nType);
	void SetCirclePara(double precision,double nSampleCount);

	// ��pListIn�����������Ӵ֣�������ص�pListOut��
	BOOL BroadenStrokeList(StrokeList* pListIn,StrokeList* pListOut);
	// ��pStroke�Ӵ֣����ؽ����pListOut��
	BOOL BroadenStroke(StrokeList* pListOut,CStrokeB* pStroke);

	int BroadenDot(const CDot& ptS,const CDot& ptM,const CDot& ptE, CArray<CDot,CDot>& dotArray,double r);

	// ȥ���ظ���
	void RemoveSameDot(CArray<CDot, CDot> &dotArray);

	// ȥ�����ߵ�
	void RemoveMidDotInLine(CArray<CDot, CDot> &dotArray);

	double GetDistance(CDot &sDot, CDot &eDot);
	// �����Ƿ���
	inline BOOL IsLine(const CDot &dotLeft, const CDot &dotMid, const CDot &dotRight);
	BOOL GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot);
	void FreeStrokeList(StrokeList* pList);
public:
	double m_dbBroadenR;	// �Ӵְ뾶
	double m_dbBroadenD;	// �Ӵּ��
	int    m_nType;			// �Ӵַ�ʽ

	double m_dbPrecision;
	int    m_nSampleCount;

};

#endif // !defined(AFX_DOTINTERFILTER_H__8F6F44FB_365A_405f_950A_553BDDBC4ABF__INCLUDED_)