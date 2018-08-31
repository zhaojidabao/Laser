// CmdPathOptimize.cpp: implementation of the CCmdPathOptimize class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdPathOptimize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdPathOptimize::CCmdPathOptimize(CHLDoc *pDoc)
{
	commandName.LoadString(IDS_COMMAND_PATHOPTIMIZE);
	m_pDoc = pDoc;
}

CCmdPathOptimize::~CCmdPathOptimize()
{

}


BOOL CCmdPathOptimize::Do( )
{
	if ( m_pDoc )
		m_pDoc->PathOptimizeCmd();

	return TRUE;
}

BOOL CCmdPathOptimize::Undo()
{
	if ( m_pDoc )
		m_pDoc->PathOptimizeCmd();

	return TRUE;
}