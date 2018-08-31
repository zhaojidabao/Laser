// CmdKillAngle.cpp: implementation of the CCmdKillAngle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdKillAngle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdKillAngle::CCmdKillAngle(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_HITOTHEROBJ_CMD);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->KillAngleCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdKillAngle::~CCmdKillAngle()
{

}

