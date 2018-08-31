// SelSort.h: interface for the CSelSort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELSORT_H__6079E525_9C6F_47BB_946A_DB1FE9C0D494__INCLUDED_)
#define AFX_SELSORT_H__6079E525_9C6F_47BB_946A_DB1FE9C0D494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SelSortB.h"

class CSelSort : public CSelSortB
{
public:
	CSelSort();
	virtual ~CSelSort();

public:
	BOOL Track(CWnd* pWnd, CArray<GNODE,GNODE> &ObjList);
};

#endif // !defined(AFX_SELSORT_H__6079E525_9C6F_47BB_946A_DB1FE9C0D494__INCLUDED_)
