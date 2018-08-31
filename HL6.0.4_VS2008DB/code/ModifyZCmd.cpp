#include "stdafx.h"
#include "ModifyZCmd.h"
#include "baseMacro.h"


CModifyZCmd::CModifyZCmd(XTracker *pTracker,double dbX, double dbY, double dbZ)
{
	m_pTracker = pTracker;
	m_dbMoveX = dbX;
	m_dbMoveY = dbY;
	m_dbMoveZ = dbZ;
}

CModifyZCmd::~CModifyZCmd()
{

}

BOOL CModifyZCmd::Do( )
{
	if ( m_pTracker )
	{
		m_pTracker->ModifyDot(m_dbMoveX, m_dbMoveY, m_dbMoveZ);
	}

	return TRUE;
}

BOOL CModifyZCmd::Undo()
{
	if ( m_pTracker )
	{
		m_pTracker->ModifyDot(-m_dbMoveX, -m_dbMoveY, -m_dbMoveZ);
	}

	return TRUE;
}
