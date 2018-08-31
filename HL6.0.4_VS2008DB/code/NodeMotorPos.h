// NodeMotorPos.h: interface for the CNodeMotorPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEMOTORPOS_H__AD8D228E_A89C_4793_B8FC_0B4278FE3749__INCLUDED_)
#define AFX_NODEMOTORPOS_H__AD8D228E_A89C_4793_B8FC_0B4278FE3749__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeControl.h"

class CNodeMotorPos : public CNodeControl
{
public:
	CNodeMotorPos();
	virtual ~CNodeMotorPos();

public:
	double m_arrayAxisPos[4];

public:
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeMotorPos();
		return pNode;
	};
};

#endif // !defined(AFX_NODEMOTORPOS_H__AD8D228E_A89C_4793_B8FC_0B4278FE3749__INCLUDED_)
