#include "StdAfx.h"
#include "hl.h"
#include "CmdUnion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdUnion::CCmdUnion(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_COMMAND_UNION);
	m_pDoc = pDoc;

	if ( m_pDoc )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->UnionCmd();
		m_pFlag2 = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag2);
	}
	m_bFirst = TRUE;
}

CCmdUnion::~CCmdUnion()
{
	if ( m_pFlag )
	{
		delete[] m_pFlag;
		m_pFlag = NULL;
	}
	////////
	if ( m_pFlag2 )
	{
		delete[] m_pFlag2;
		m_pFlag2 = NULL;
	}
}

BOOL CCmdUnion::Do( )
{
	if ( m_bFirst )
	{
		m_bFirst = FALSE;
	}
	else if ( m_pDoc && m_pFlag )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->UnionCmd();
	}

	return TRUE;
}

BOOL CCmdUnion::Undo()
{
	if ( m_pDoc && m_pFlag2 )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag2);
		m_pDoc->UnunionCmd();
	}
	return TRUE;
}
