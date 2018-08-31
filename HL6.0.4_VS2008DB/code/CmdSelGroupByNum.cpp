// CmdSelGroupByNum.cpp: implementation of the CCmdSelGroupByNum class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdSelGroupByNum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdSelGroupByNum::CCmdSelGroupByNum(CHLDoc *pDoc,int nNum, CWnd *pWnd): CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_SELGROUPBYNUM_CMD);

	 if ( m_pDoc != NULL  && !pWnd )
	 {
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->SelGroupByNumCmd(nNum, pWnd);
		m_chainNew.StoreChain(pDoc->GetChain());
	 }
}

CCmdSelGroupByNum::~CCmdSelGroupByNum()
{

}

