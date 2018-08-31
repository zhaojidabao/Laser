// CmdHitOtherObj.cpp: implementation of the CCmdHitOtherObj class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "CmdHitOtherObj.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdHitOtherObj::CCmdHitOtherObj(CHLDoc *pDoc,CPoint &point,CWnd *pWnd) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_HITOTHEROBJ_CMD);

	if ( m_pDoc != NULL )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->ChangeQuadCmd(point,pWnd);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdHitOtherObj::~CCmdHitOtherObj()
{

}

