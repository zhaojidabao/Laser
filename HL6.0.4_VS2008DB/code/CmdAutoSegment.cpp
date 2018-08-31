// CmdAutoSegment.cpp: implementation of the CCmdAutoSegment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdAutoSegment.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdAutoSegment::CCmdAutoSegment(CHLDoc *pDoc) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_AUTOSEGMENT);

	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		m_pDoc->EditWheelCmd();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdAutoSegment::~CCmdAutoSegment()
{

}

