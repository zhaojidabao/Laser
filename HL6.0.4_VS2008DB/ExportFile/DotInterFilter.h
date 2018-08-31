// DotInterFilter.h: interface for the PointOfIntersectionFilterForHl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTINTERFILTER_H__0A556413_1671_4382_9158_50B3576CE5DB__INCLUDED_)
#define AFX_DOTINTERFILTER_H__0A556413_1671_4382_9158_50B3576CE5DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrokeB.h"
#include "DotIntersection.h"


class AFX_EXT_CLASS CDotInterFilter : public CDotIntersection  
{
public:
	CDotInterFilter();
	virtual ~CDotInterFilter();
	
protected:
	CArray<InterDot,InterDot>m_dotArray;//合成线条点队列
	CArray<InterDot,InterDot>m_tmpArray;//交点排序队列
	
public:
	void AddInterPoint(InterDot dot);
	void ReCreateStrokeList(CStrokeB *pStroke,StrokeList *pList);
	BOOL GenerateNewPoint(LINE Line);
	BOOL GetLineInterPoint(LINE L1,LINE L2, InterDot & point);
	int  ScanFilterInterDot(void *param1,void *param2=NULL);

protected:
	void SortInterPointArray(LINE Line);
};

#endif // !defined(AFX_DOTINTERFILTER_H__0A556413_1671_4382_9158_50B3576CE5DB__INCLUDED_)
