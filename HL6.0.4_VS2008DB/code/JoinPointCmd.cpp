// JoinPointCmd.cpp: implementation of the CJoinPointCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "JoinPointCmd.h"
#include "HLView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CJoinPointCmd::CJoinPointCmd(CHLDoc *pDoc,CWnd *pWnd) : CCmdBaseSave(pDoc)
{
	if ( pDoc && pWnd )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		((CHLView*)pWnd)->JoinPoint();
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CJoinPointCmd::~CJoinPointCmd()
{

}

