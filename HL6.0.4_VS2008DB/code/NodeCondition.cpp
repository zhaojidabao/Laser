// NodeCondition.cpp: implementation of the CNodeConditionS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeCondition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeConditionS::CNodeConditionS()
	:CNodeInput()
{
	m_type = NODE_CONDITIONS;
}

CNodeConditionS::~CNodeConditionS()
{
	CNode::~CNode();
}

void CNodeConditionS::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);

	if ( bFlag )
	{
		fs << _T("NCONDITIONS_B");
	}
	
	CNodeInput::Serialize(fp, bFlag,bUnicode);
}

UINT CNodeConditionS::Mark(HWND hWnd)
{
	return g_DAT.MarkCheckSwitch(1, m_dwInput, m_dwInputMask);
}

CNodeConditionE::CNodeConditionE()
{
	m_type = NODE_CONDITIONE;
}

CNodeConditionE::~CNodeConditionE()
{
	CNode::~CNode();
}

void CNodeConditionE::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);

	if ( bFlag )
	{
		fs <<  _T("NCONDITIONE_B");
	}
}

UINT CNodeConditionE::Mark(HWND hWnd)
{
	return g_DAT.MarkCheckSwitch(0, 0, 0);
}