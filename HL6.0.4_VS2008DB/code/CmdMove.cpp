// CmdMove.cpp: implementation of the CCmdMove class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdMove.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdMove::CCmdMove(CHLDoc *pDoc,double moveX,double moveY, double moveZ)
{
	commandName.LoadString(IDS_COMMAND_MOVE);
	m_pDoc = pDoc;
	m_dbMoveX = moveX;
	m_dbMoveY = moveY;
	m_dbMoveZ = moveZ;
	m_pFlag = NULL;
	m_bFirst = TRUE;

	if ( m_pDoc )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->MoveCmd(m_dbMoveX,m_dbMoveY,m_dbMoveZ);
	}
}


CCmdMove::~CCmdMove()
{
	if ( m_pFlag )
	{
		delete []m_pFlag;
		m_pFlag = NULL;
	}
}

BOOL CCmdMove::Do()
{
	if ( m_bFirst )
	{
		m_bFirst = FALSE;
	}
	else if ( m_pDoc )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->MoveCmd(m_dbMoveX,m_dbMoveY,m_dbMoveZ);
	}

	return TRUE;
}

BOOL CCmdMove::Undo()
{
	if ( m_pDoc )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->MoveCmd(-m_dbMoveX,-m_dbMoveY,-m_dbMoveZ);
	}

	return TRUE;
}

