// OptStrokeList.h: interface for the COptStrokeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTSTROKELIST_H__4EDC974A_D229_4945_9E83_71547B898772__INCLUDED_)
#define AFX_OPTSTROKELIST_H__4EDC974A_D229_4945_9E83_71547B898772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"

class COptStrokeList  
{
public:
	////�����Ż�����
	void PartOptInsertStroke(CStroke* pStroke);
	void PartOptEmptyStrokeList(BOOL bDestroy = FALSE);

	////��ȫ�Ż�����
	void FullOpt();

	////
	void Detach();
	void Attach(StrokeList* pListSrc,double dbOptDist = 0.001,BOOL bFull = TRUE);
	COptStrokeList();
	virtual ~COptStrokeList();

private:
	void InsertStrokeIntoList(CStroke* pStroke);
	void InsertStrokeIntoList2(CStroke* pStroke);

	////
	BOOL m_bOptFull;//��ʾ�Ƿ���ȫ�Ż�(��ȫ�Ż������ʾ��m_pListSrc�����������Ż�����)
	double m_dbOptDist;//�Ż����
	StrokeList* m_pListSrc;//���Ż���������
};

#endif // !defined(AFX_OPTSTROKELIST_H__4EDC974A_D229_4945_9E83_71547B898772__INCLUDED_)
