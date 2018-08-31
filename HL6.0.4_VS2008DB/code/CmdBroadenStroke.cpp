#include "stdafx.h"
#include "CmdBroadenStroke.h"

CCmdBroadenStroke::CCmdBroadenStroke(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName = _T("BroadenStroke");
	if ( m_pDoc != NULL )
	{
		m_chainOld.StoreChain( pDoc->GetChain() );
		m_pDoc->BroadenStrokeList();
		m_chainNew.StoreChain( pDoc->GetChain() );
	}
}

CCmdBroadenStroke::~CCmdBroadenStroke(void)
{
}
