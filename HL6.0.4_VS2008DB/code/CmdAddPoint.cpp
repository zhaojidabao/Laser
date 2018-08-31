// CmdAddPoint.cpp: implementation of the CCmdAddPoint class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CmdAddPoint.h"
#include "resource.h"
#include "HLView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdAddPoint::CCmdAddPoint(CHLDoc *pDoc,CPoint &point,CWnd *pWnd) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_ADDPOINT_CMD);
	m_pDoc = pDoc;
	if ( m_pDoc && pWnd )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		((CHLView*)pWnd)->AddPoint(point);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdAddPoint::~CCmdAddPoint()
{

}
