// CmdArray.cpp: implementation of the CCmdArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "CmdArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD g_dwJumpCnt  = -1;
CCmdArray::CCmdArray(CHLDoc *pDoc, CArray<OBJPOS, OBJPOS> &dtArray,BOOL flag) : CCmdBaseSave(pDoc)
{
	commandName.LoadString(IDS_COMMAND_ARRAY);

	if ( pDoc != NULL )
	{
		m_chainOld.StoreChain(pDoc->GetChain());
		g_dwJumpCnt = flag ? 0 : -1;
		m_pDoc->ArrayCmd(dtArray);
		g_dwJumpCnt  = -1;
		m_chainNew.StoreChain(pDoc->GetChain());
	}
}

//
CCmdArray::~CCmdArray()
{

}

