// CmdRotate.cpp: implementation of the CCmdRotate class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdRotate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CCmdRotate::CCmdRotate(CHLDoc *pDoc,CDot dot, double angle, int nPlane )
{
	commandName.LoadString(IDS_COMMAND_ROTATE);
	m_pDoc = pDoc;
	m_dotCenter = dot;
	m_dbAngle = angle;
	m_nPlane = nPlane;
	m_pFlag = NULL;
	m_bFirst = TRUE;

	if ( m_pDoc )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->RotateCmd(m_dotCenter,m_dbAngle,m_nPlane);
	}
}

CCmdRotate::~CCmdRotate()
{
	if ( m_pFlag )
	{
		delete[] m_pFlag;
		m_pFlag = NULL;
	}
}

BOOL CCmdRotate::Do( )
{
	if ( m_bFirst )
		m_bFirst = FALSE;
	else	if ( m_pDoc && m_pFlag )
	{
		m_pDoc->SetSelectFlag(m_pFlag);
		m_pDoc->RotateCmd(m_dotCenter,m_dbAngle,m_nPlane);
	}

	return TRUE;
}

BOOL CCmdRotate::Undo()
{
	if ( m_pDoc && m_pFlag )
	{
		m_pDoc->SetSelectFlag(m_pFlag);
		m_pDoc->RotateCmd(m_dotCenter,-m_dbAngle,m_nPlane);
	}

	return TRUE;
}

