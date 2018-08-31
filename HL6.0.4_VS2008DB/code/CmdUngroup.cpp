// CmdUngroup.cpp: implementation of the CCmdUngroup class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "CmdUngroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdUngroup::CCmdUngroup(CHLDoc *pDoc) 
{
	commandName.LoadString(IDS_COMMAND_UNGROUP);

	m_pDoc = pDoc;
	m_bFirst = TRUE;
	m_pFlag1 = NULL;
	m_pFlag2 = NULL;

	if ( m_pDoc != NULL )
	{
		m_pFlag1 = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag1);

		m_pDoc->UngroupCmd();

		m_pFlag2 = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag2);
	}
}

CCmdUngroup::~CCmdUngroup()
{
	if ( m_pFlag1 )
	{
		delete[] m_pFlag1;
		m_pFlag1 = NULL;
	}
	if ( m_pFlag2 )
	{
		delete[] m_pFlag2;
		m_pFlag2 = NULL;
	}
}

BOOL CCmdUngroup::Do()
{
	if ( m_bFirst )
		m_bFirst = FALSE;
	else if ( m_pDoc && m_pFlag1 )
	{
		m_pDoc->SetSelectFlag(m_pFlag1);
		m_pDoc->UngroupCmd();
	}

	return TRUE;
}

BOOL CCmdUngroup::Undo()
{
	if ( m_pDoc && m_pFlag2 )
	{
		m_pDoc->SetSelectFlag(m_pFlag2);
		m_pDoc->GroupCmd();
	}
	return TRUE;
}
