// NodeOutput.h: interface for the CNodeOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEOUTPUT_H__04B13078_05B3_4189_9D8A_F67C25E8A3BA__INCLUDED_)
#define AFX_NODEOUTPUT_H__04B13078_05B3_4189_9D8A_F67C25E8A3BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeControl.h"

class CNodeOutput : public CNodeControl  
{
public:
	CNodeOutput();
	virtual ~CNodeOutput();

	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);

	void Get(DWORD& dwOutput, DWORD& dwOutputMask, DWORD& dwOutputDelay)
		{dwOutput = m_dwOutput;dwOutputMask = m_dwOutputMask;dwOutputDelay = m_dwOutputDelay;};
	void Set(DWORD  dwOutput, DWORD  dwOutputMask, DWORD& dwOutputDelay)
		{m_dwOutput = dwOutput;m_dwOutputMask = dwOutputMask;m_dwOutputDelay = dwOutputDelay;};
	
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeOutput();
		return pNode;
	};

private:
	DWORD m_dwOutput;
	DWORD m_dwOutputMask;
	DWORD m_dwOutputDelay;
};

#endif // !defined(AFX_NODEOUTPUT_H__04B13078_05B3_4189_9D8A_F67C25E8A3BA__INCLUDED_)
