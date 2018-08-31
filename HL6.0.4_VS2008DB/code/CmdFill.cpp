// CmdFill.cpp: implementation of the CCmdFill class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdFill.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdFill::CCmdFill(CHLDoc* pDoc,FILLDATA fillnew,FILLDATA fillold) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_FILL);
	
	if ( m_pDoc != NULL )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->FillCmd(fillnew);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdFill::~CCmdFill()
{
}

