// ChainB.h: interface for the CChainB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAINB_H__8175D22A_6BFE_4F68_8031_A0921734F570__INCLUDED_)
#define AFX_CHAINB_H__8175D22A_6BFE_4F68_8031_A0921734F570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataBase.h"
#include "TreeB.h"
#include "GroupB.h"

class AFX_EXT_CLASS CChainB  : public CDataBase
{
public:
	CChainB();
	virtual ~CChainB();
	
public:
//	TREELIST m_list;	//所有树的头节点列表
	CTypedPtrList<CObList,CGroupB *> m_listGroup;
	int m_nMarkUnit;
	BOOL m_bSelMark;
	
public:
	void StoreChain_T(const CChainB* pChain);
	void ReStoreChain_T(const CChainB* pChain);
	
	double Distance(CDot &sDot, CDot &eDot);
	void SortAllObj(CArray<SORTNODE,SORTNODE> &Sort);
	void AutoSortAllObj(CArray<ASNODE,ASNODE> &Sort, DWORD dwStartGroup = 0);
	void GSortAllObj(CArray<GNODE,GNODE> &Sort);
	void DelTree(CGroupB *pGroup);
	
	void CalcMarkDot(CArray<SORTNODE,SORTNODE> &Sort);
};

#endif // !defined(AFX_CHAINB_H__8175D22A_6BFE_4F68_8031_A0921734F570__INCLUDED_)
