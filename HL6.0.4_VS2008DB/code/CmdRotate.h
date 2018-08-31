// CmdRotate.h: interface for the CCmdRotate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDROTATE_H__583122A4_46BF_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDROTATE_H__583122A4_46BF_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdRotate : public CCommand  
{
public:
	CCmdRotate(CHLDoc *pDoc,CDot dot, double angle, int nPlane );
	virtual ~CCmdRotate();
	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc*m_pDoc;
	CDot		m_dotCenter;
	double	m_dbAngle;
	int			m_nPlane;
	BOOL		m_bFirst;
	BOOL	*	m_pFlag;

};

#endif // !defined(AFX_CMDROTATE_H__583122A4_46BF_11D4_A483_00105A615525__INCLUDED_)
