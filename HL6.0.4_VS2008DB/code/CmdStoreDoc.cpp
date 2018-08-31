// CmdStoreDoc.cpp: implementation of the CCmdStoreDoc class.
//
#include "stdafx.h"
#include "hl.h"
#include "CmdStoreDoc.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdStoreDoc::CCmdStoreDoc(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_STOREDOC);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->m_chain.SetSelect();
		pDoc->SetTrackerQuad(pDoc->GetAllLimit());
		m_chainNew.StoreChain(pDoc->GetChain());
	}
	
}

CCmdStoreDoc::~CCmdStoreDoc()
{

}

