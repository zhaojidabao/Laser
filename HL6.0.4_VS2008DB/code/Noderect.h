// NodeRect.h: interface for the CNodeRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODERECT_H__9E292CE2_E0D9_11D3_845D_444553540001__INCLUDED_)
#define AFX_NODERECT_H__9E292CE2_E0D9_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"

class CNodeRect : public CNodeV  
{
public:
	CNodeRect();
	virtual ~CNodeRect();

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeRect();
		return pNode;
	};

	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);	
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual BOOL Create(CDot pt1,CDot pt2);
	BOOL CreateStroke(CDot dt1,CDot dt2,int nMode = 0,double dbValue = 1.0);
	void AddCircleStroke(CStroke *pStroke, CDot dotCenter, CDot dotStart, CDot dotEnd);
	
	//
	CGArc m_GArc;
};

#endif // !defined(AFX_NODERECT_H__9E292CE2_E0D9_11D3_845D_444553540001__INCLUDED_)
