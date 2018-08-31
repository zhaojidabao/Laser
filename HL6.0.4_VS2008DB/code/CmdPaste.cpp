// CmdPaste.cpp: implementation of the CCmdPaste class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdPaste.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CCmdPaste::CCmdPaste(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_PASTE);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->PasteCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdPaste::~CCmdPaste()
{
}
