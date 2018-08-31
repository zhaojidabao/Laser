// CmdExtendObj.cpp: implementation of the CCmdExtendObj class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdExtendObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdExtendObj::CCmdExtendObj(CHLDoc *pDoc, double dbSpace, BOOL bIsKillOldObj) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_EXTENDOBJ_CMD);
	/////////
	if ( m_pDoc != NULL )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->ExtendObjCmd(dbSpace, bIsKillOldObj);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdExtendObj::~CCmdExtendObj()
{

}


