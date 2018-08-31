// CmdDelDot.cpp: implementation of the CCmdDelDot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdDelDot.h"
#include "HLView.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDelDot::CCmdDelDot(CHLDoc *pDoc,CWnd *pWnd) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_DELDOT_CMD);

	if ( m_pDoc != NULL && pWnd != NULL )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		((CHLView*)pWnd)->DelDot();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdDelDot::~CCmdDelDot()
{
}

