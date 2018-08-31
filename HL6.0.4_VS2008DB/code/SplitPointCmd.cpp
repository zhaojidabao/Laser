// SplitPointCmd.cpp: implementation of the SplitPointCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "SplitPointCmd.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SplitPointCmd::SplitPointCmd(CHLDoc *pDoc,CWnd *pWnd, double dbPoint) : CCmdBaseSave(pDoc)
{
	if ( m_pDoc && pWnd )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		((CHLView*)pWnd)->SplitPoint(dbPoint);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

SplitPointCmd::~SplitPointCmd()
{

}
