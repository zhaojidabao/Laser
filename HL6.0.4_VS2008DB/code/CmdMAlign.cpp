// CmdMAlign.cpp: implementation of the CCmdMAlign class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdMAlign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdMAlign::CCmdMAlign(CHLDoc *pDoc, int nType) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_MALIGN);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->MAlignCmd(nType);
		CQuad quadNew = m_pDoc->GetLimit();
		m_pDoc->SetTrackerQuad(quadNew);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdMAlign::~CCmdMAlign()
{

}

