// NodeDelay.h: interface for the CNodeDelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NodeDelay_H__0B102651_4AEA_441B_88CB_8D4230586621__INCLUDED_)
#define AFX_NodeDelay_H__0B102651_4AEA_441B_88CB_8D4230586621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeControl.h"

class CNodeDelay : public CNodeControl
{	
public:
	CNodeDelay();
	virtual ~CNodeDelay();
	
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
		
	DWORD GetDelay(){return m_dwDelay;};
	void SetDelay(DWORD dwDelay){
		m_dwDelay = dwDelay;
	};
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeDelay();
		return pNode;
	};
private:
	DWORD m_dwDelay;
};

#endif // !defined(AFX_NodeDelay_H__0B102651_4AEA_441B_88CB_8D4230586621__INCLUDED_)
