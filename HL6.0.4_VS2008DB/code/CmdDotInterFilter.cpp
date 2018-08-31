#include "stdafx.h"
#include "CmdDotInterFilter.h"

CCmdDotInterFilter::CCmdDotInterFilter(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName = _T("InterFilter");
	if ( m_pDoc != NULL )
	{
		m_chainOld.StoreChain( pDoc->GetChain() );
		m_pDoc->EditInterfilter();
		m_chainNew.StoreChain( pDoc->GetChain() );
	}
}

CCmdDotInterFilter::~CCmdDotInterFilter(void)
{
}

