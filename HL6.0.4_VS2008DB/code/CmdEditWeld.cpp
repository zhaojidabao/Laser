#include "stdafx.h"
#include "CmdEditWeld.h"

CCmdEditWeld::CCmdEditWeld(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName = _T("Edit Weld");

	if ( m_pDoc )
	{
		m_chainOld.StoreChain( pDoc->GetChain() );
		m_pDoc->EditWeldCmd();
		m_chainNew.StoreChain( pDoc->GetChain() );
	}
}


CCmdEditWeld::~CCmdEditWeld(void)
{
}

