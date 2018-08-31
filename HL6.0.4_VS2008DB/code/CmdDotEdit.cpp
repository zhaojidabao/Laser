// CmdDotEdit.cpp: implementation of the CCmdDotEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdDotEdit.h"
#include "resource.h"
#include "HLView.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdDotEdit::CCmdDotEdit(CHLDoc *pDoc,CPoint &point,CWnd *pWnd) : CCmdBaseSave(pDoc)
{ 
	commandName.LoadString(IDS_DOTEDIT_CMD);

	 if ( m_pDoc && pWnd ) 
	 {
		m_chainOld.StoreChain(pDoc->GetChain());
		CHLView *pView = (CHLView*)pWnd;
		pView->m_track.SingleModify(point,pWnd);
		pView->ReplaceDot();
		m_chainNew.StoreChain(pDoc->GetChain());
	 }
	
}

//
CCmdDotEdit::~CCmdDotEdit()
{

}
