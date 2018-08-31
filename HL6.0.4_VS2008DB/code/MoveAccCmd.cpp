// MoveAccCmd.cpp: implementation of the CMoveAccCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "MoveAccCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CMoveAccCmd::CMoveAccCmd(XTracker *pTracker, double dbMoveX,double dbMoveY)
{
	m_dbMoveX = dbMoveX;
	m_dbMoveY = dbMoveY;
	m_pTracker = pTracker;
}

CMoveAccCmd::~CMoveAccCmd()
{

}

BOOL CMoveAccCmd::Do( )
{
	if ( m_pTracker )
	{
		m_pTracker->MoveDot(m_dbMoveX,m_dbMoveY);
	}

	return TRUE;
}

BOOL CMoveAccCmd::Undo()
{
	if ( m_pTracker )
	{
		m_pTracker->MoveDot(-m_dbMoveX,-m_dbMoveY);
	}

	return TRUE;
}

