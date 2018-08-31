// CmdNewDrag.cpp: implementation of the CCmdNewDrag class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CmdNewDrag.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdNewDrag::CCmdNewDrag(CHLDoc *pDoc,CDot &dot,double dbRatioX,double dbRatioY,double dbMoveX,double dbMoveY, double dbMoveZ)
{	
	m_pDoc = pDoc;
	commandName.LoadString(IDS_NEWDRAG_CMD);
	
	 m_bFirst = TRUE;
	 m_dbMoveX = dbMoveX;
	 m_dbMoveY = dbMoveY;
	 m_dbMoveZ = dbMoveZ;
	 m_dotCenter = dot;
	 m_pFlag  = NULL;
	 m_dbRatioX = dbRatioX;
	 m_dbRatioY = dbRatioY;
	 if ( fabs( m_dbRatioX ) < 0.0001 )
		 m_dbRatioX = 0.001;
	 if ( fabs( m_dbRatioY ) < 0.0001 )
		 m_dbRatioX = 0.0001;

	 if ( m_pDoc )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->DragCmd(m_dotCenter,m_dbRatioX,m_dbRatioY,m_dbMoveX,m_dbMoveY,m_dbMoveZ);
	}

} 

CCmdNewDrag::~CCmdNewDrag()
{
	if ( m_pFlag )
	{
		delete[] m_pFlag;
		m_pFlag = NULL;
	}
}

BOOL CCmdNewDrag::Do( )
{
	if ( m_bFirst )
	{
		m_bFirst = FALSE;
	}
	else if ( m_pDoc && m_pFlag )
	{
		m_pDoc->SetSelectFlag(m_pFlag);
		m_pDoc->DragCmd(m_dotCenter,m_dbRatioX,m_dbRatioY,m_dbMoveX,m_dbMoveY,m_dbMoveZ);
	}

	return TRUE;
}

BOOL CCmdNewDrag::Undo()
{
	if ( m_pDoc && m_pFlag )
	{
		m_pDoc->SetSelectFlag(m_pFlag);
		m_pDoc->DragCmd(m_dotCenter,1/m_dbRatioX,1/m_dbRatioY,-m_dbMoveX,-m_dbMoveY,-m_dbMoveZ);
	}

	return TRUE;
}

