// NodeDot.h: interface for the CNodeDot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEDOT_H__F04DEF82_258F_4300_A8AD_5396863AB9E7__INCLUDED_)
#define AFX_NODEDOT_H__F04DEF82_258F_4300_A8AD_5396863AB9E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeV.h"

class CNodeDot : public CNodeV  
{
public:
	CNodeDot();
	virtual ~CNodeDot();

	BOOL Create(const CPoint& point);
	virtual CQuad CalLimit();
	virtual void Serialize (FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual void AddToPlt(CNode *pNodePlt);
	virtual void Draw(XDC *pDC);

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeDot();
		return pNode;
	};
};

#endif // !defined(AFX_NODEDOT_H__F04DEF82_258F_4300_A8AD_5396863AB9E7__INCLUDED_)
