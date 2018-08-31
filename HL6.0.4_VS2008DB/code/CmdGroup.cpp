// CmdGroup.cpp: implementation of the CCmdGroup class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "CmdGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdGroup::CCmdGroup(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_GROUP);
	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->GroupCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdGroup::~CCmdGroup()
{
}
