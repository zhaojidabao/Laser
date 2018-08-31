// CmdDragFrame.cpp: implementation of the CCmdDragFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdDragFrame.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDragFrame::CCmdDragFrame(CHLDoc *pDoc,CQuadRangle &frame,CDot &dot,double dbratioX,double dbratioY,double dbmoveX,double dbmoveY)
{
	commandName.LoadString(IDS_NEWDRAG_CMD);
	m_pDoc = pDoc;

	m_bFirst = TRUE;
	m_frame = frame;
	m_dot = dot;
	m_dbRatioX = dbratioX;
	m_dbRatioY = dbratioY;
	m_dbMoveX = dbmoveX;
	m_dbMoveY = dbmoveY;
	m_pFlag = NULL;

	if ( m_pDoc != NULL )
	{
		pDoc->DragFrameCmd(frame,dot,dbratioX,dbratioY,dbmoveX,dbmoveY);
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
	}
}

CCmdDragFrame::~CCmdDragFrame()
{
	if ( m_pFlag )
	{
		delete []m_pFlag;
		m_pFlag = NULL;
	}
}

BOOL CCmdDragFrame::Do()
{
	if ( m_bFirst )
	{
		m_bFirst = FALSE;
	}
	else if ( m_pDoc )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->DragFrameCmd(m_frame,m_dot,m_dbRatioX,m_dbRatioY,m_dbMoveX,m_dbMoveY);
	}
	return TRUE;
}

BOOL CCmdDragFrame::Undo()
{
	if ( m_pDoc != NULL )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->DragFrameCmd(m_frame,m_dot,1/m_dbRatioX,1/m_dbRatioY,-m_dbMoveX,-m_dbMoveY);
	}

	return TRUE;
}