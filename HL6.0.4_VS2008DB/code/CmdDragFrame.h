// CmdDragFrame.h: interface for the CCmdDragFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDRAGFRAME_H__50A42521_C6B6_4738_AF9F_FF6534EDA780__INCLUDED_)
#define AFX_CMDDRAGFRAME_H__50A42521_C6B6_4738_AF9F_FF6534EDA780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
#include "QuadRangle.h"

class CCmdDragFrame : public CCommand  
{
public:
	CCmdDragFrame(CHLDoc *pDoc,CQuadRangle&frame,CDot &dot,double dbratioX,double dbratioY,double dbmoveX,double dbmoveY);
	virtual ~CCmdDragFrame();
	virtual BOOL Do();
	virtual BOOL Undo();

private:
	CHLDoc * m_pDoc;
	CQuadRangle m_frame;
	CDot		m_dot;
	double	m_dbRatioX;
	double	m_dbRatioY;
	double	m_dbMoveX;
	double	m_dbMoveY;
	BOOL	m_bFirst;
	BOOL*	m_pFlag;
};

#endif // !defined(AFX_CMDDRAGFRAME_H__50A42521_C6B6_4738_AF9F_FF6534EDA780__INCLUDED_)
