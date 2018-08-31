// CmdDrawSet.cpp: implementation of the CCmdDrawSet class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "CmdDrawSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDrawSet::CCmdDrawSet(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_DRAWSET);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->DrawsetCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
	
}

CCmdDrawSet::~CCmdDrawSet()
{

}

