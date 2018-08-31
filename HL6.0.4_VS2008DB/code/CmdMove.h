// CmdMove.h: interface for the CCmdMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDMOVE_H__C7A85BA1_4604_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDMOVE_H__C7A85BA1_4604_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdMove : public CCommand  
{
public: 
	CCmdMove(CHLDoc *pDoc,double moveX,double moveY,double mvoeZ);
	virtual ~CCmdMove();
	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc*	m_pDoc;
	double	m_dbMoveX;
	double	m_dbMoveY;
	double	m_dbMoveZ;
	BOOL*	m_pFlag;
	BOOL	m_bFirst;
};

#endif // !defined(AFX_CMDMOVE_H__C7A85BA1_4604_11D4_A483_00105A615525__INCLUDED_)
