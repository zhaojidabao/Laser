#pragma once

#include "NodeControl.h"
// CNodePosCali 命令目标

class CNodePosCali : public CNodeControl
{
public:
	CNodePosCali();
	virtual ~CNodePosCali();

	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodePosCali();
		return pNode;
	};

public:
	UINT	m_nCaliType;		//类型，0：停止位置校正，1：开始位置校正
};


