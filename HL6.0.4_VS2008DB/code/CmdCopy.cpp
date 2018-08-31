// CmdCopy.cpp: implementation of the CCmdCopy class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdCopy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdCopy::CCmdCopy(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_COPY);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->CopyCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdCopy::~CCmdCopy()
{

}
