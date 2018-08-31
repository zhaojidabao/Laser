// CmdDelObj.cpp: implementation of the CCmdDelObj class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdDelObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CCmdDelObj::CCmdDelObj(CHLDoc *pDoc, DWORD dwPara) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_DELETE);

	if ( m_pDoc != NULL )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->DelObjCmd(dwPara);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdDelObj::~CCmdDelObj()
{

}

