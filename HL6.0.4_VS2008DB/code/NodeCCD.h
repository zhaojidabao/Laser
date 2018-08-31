#pragma once
#include "NodeControl.h"


//CNodeCCD ÃüÁîÄ¿±ê
class CNodeCCD : public CNodeControl
{
public:
	CNodeCCD();
	virtual ~CNodeCCD();
	
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeCCD();
		return pNode;
	};
	
	void GetNodePara( BUF_CAMERA_PARA &cameraPara);
	void SetNodePara( const BUF_CAMERA_PARA &cameraPara);
	
public:
	BUF_CAMERA_PARA m_cameraPara;
};


