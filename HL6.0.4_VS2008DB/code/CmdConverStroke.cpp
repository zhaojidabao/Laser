// CmdConverStroke.cpp: implementation of the CCmdConverStroke class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "CmdConverStroke.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdConverStroke::CCmdConverStroke(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_COMTOTREE);

	if ( m_pDoc != NULL )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->ConverStrokeCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdConverStroke::~CCmdConverStroke()
{

}

