#pragma once

#include "NodeControl.h"
// CNodePosCali ����Ŀ��

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
	UINT	m_nCaliType;		//���ͣ�0��ֹͣλ��У����1����ʼλ��У��
};


