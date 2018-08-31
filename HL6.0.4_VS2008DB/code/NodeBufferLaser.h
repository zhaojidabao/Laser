#pragma once

// CNodeBufferLaser ÃüÁîÄ¿±ê
#include "NodeControl.h"
#include "MarkApiDef.h"

class CNodeBufferLaser : public CNodeControl
{
public:
	CNodeBufferLaser();
	virtual ~CNodeBufferLaser();

public:
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeBufferLaser();
		return pNode;
	};

	//
	void SetBufferLaserPara( LP_LASER_PARA pLaserPara );
	void GetBudderLaserPara( LP_LASER_PARA pLaserPara );
	BOOL IsLaserOn();
	
private:
	LASER_PARA  m_laserPara;
};


