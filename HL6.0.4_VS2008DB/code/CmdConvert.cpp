// CmdConvert.cpp: implementation of the CCmdConvert class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "CmdConvert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdConvert::CCmdConvert(CHLDoc *pDoc, BOOL bPopMsg) : CCmdBaseSave(pDoc)
{
	commandName = _T("Convert");
	if ( m_pDoc )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		pDoc->ConvertCmd(bPopMsg);
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

CCmdConvert::~CCmdConvert()
{

}

