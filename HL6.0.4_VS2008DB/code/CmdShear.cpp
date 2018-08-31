// CmdShear.cpp: implementation of the CCmdShear class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdShear.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdShear::CCmdShear(CHLDoc *pDoc,CDot dot,double dbAngleX,double dbAngleY )
{
	commandName.LoadString(IDS_COMMAND_ROTATE);
	m_pDoc = pDoc;
	m_dotBase = dot;
	m_dbAngleX = dbAngleX;
	m_dbAngleY = dbAngleY;
	m_pFlag = NULL;
	m_bFirst = TRUE;

	if ( m_pDoc ){
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->ShearCmd(m_dotBase, m_dbAngleX, m_dbAngleY);
	}
}

CCmdShear::~CCmdShear()
{
	if ( m_pFlag )
	{
		delete[] m_pFlag;
		m_pFlag = NULL;
	}
}

BOOL CCmdShear::Do( )
{
	if ( m_bFirst ){
		m_bFirst = FALSE;
	}
	else if ( m_pDoc && m_pFlag )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->ShearCmd(m_dotBase, m_dbAngleX, m_dbAngleY);
	}
	return TRUE;
}

BOOL CCmdShear::Undo()
{
	if ( m_pDoc && m_pFlag )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->ShearCmd(m_dotBase, -m_dbAngleX, -m_dbAngleY);
	}

	return TRUE;
}


