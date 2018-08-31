// NodeCondition.h: interface for the CNodeCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECONDITION_H__256C02EF_2F90_42DA_8ADD_D4460C5991CA__INCLUDED_)
#define AFX_NODECONDITION_H__256C02EF_2F90_42DA_8ADD_D4460C5991CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeInput.h"

class CNodeConditionS : public CNodeInput  
{
public:
	CNodeConditionS();
	virtual ~CNodeConditionS();

	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeConditionS();
		return pNode;
	};
};

class CNodeConditionE : public CNodeControl  
{
public:
	CNodeConditionE();
	virtual ~CNodeConditionE();

	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeConditionE();
		return pNode;
	};
};

#endif // !defined(AFX_NODECONDITION_H__256C02EF_2F90_42DA_8ADD_D4460C5991CA__INCLUDED_)
