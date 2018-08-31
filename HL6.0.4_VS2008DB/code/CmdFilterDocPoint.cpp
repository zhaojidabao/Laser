// CmdFilterDocPoint.cpp: implementation of the CCmdFilterDocPoint class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdFilterDocPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdFilterDocPoint::CCmdFilterDocPoint(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_FILTERPOINT_CMD);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->FilterPointCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdFilterDocPoint::~CCmdFilterDocPoint()
{

}

