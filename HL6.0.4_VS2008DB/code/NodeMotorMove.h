// NodeMotorMove.h: interface for the CNodeMotorMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEMOTORMOVE_H__F3FB4EBB_24A3_4405_B2AD_AE3999C726F9__INCLUDED_)
#define AFX_NODEMOTORMOVE_H__F3FB4EBB_24A3_4405_B2AD_AE3999C726F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeControl.h"
#include "Motion.h"

class CNodeMotorMove : public CNodeControl  
{
public:
	CNodeMotorMove();
	virtual ~CNodeMotorMove();
	
public:
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual UINT Mark(HWND hWnd);
	virtual  BOOL CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual  CNode*	CreateNewNode(){
		CNode *pNode = new CNodeMotorMove();
		return pNode;
	};	

public:
	double m_dbMoveAngle;
	int      m_nWorkType;     //运动类型 0-工作,1-空程，2-归零
	/////
	MOTOR_PARA m_para;
};

#endif // !defined(AFX_NODEMOTORMOVE_H__F3FB4EBB_24A3_4405_B2AD_AE3999C726F9__INCLUDED_)
