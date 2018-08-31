// NodeInput.h: interface for the CNodeInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEINPUT_H__C2282C07_AF76_4D22_893B_960C85FD6B2A__INCLUDED_)
#define AFX_NODEINPUT_H__C2282C07_AF76_4D22_893B_960C85FD6B2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeControl.h"

class CNodeInput : public CNodeControl  
{
public:
	CNodeInput();
	virtual ~CNodeInput();

	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);

	void Get(DWORD& dwInput, DWORD& dwInputMask){dwInput = m_dwInput;dwInputMask = m_dwInputMask;};
	void Set(DWORD  dwInput, DWORD  dwInputMask){m_dwInput = dwInput;m_dwInputMask = dwInputMask;};

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeInput();
		return pNode;
	};

protected:
	DWORD m_dwInput;
	DWORD m_dwInputMask;
};

#endif // !defined(AFX_NODEINPUT_H__C2282C07_AF76_4D22_893B_960C85FD6B2A__INCLUDED_)
