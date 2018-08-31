// NodeInput.cpp: implementation of the CNodeInput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeInput::CNodeInput()
{
	m_type = NODE_INPUT;
	m_dwInput = 0;
	m_dwInputMask = 0;
}

CNodeInput::~CNodeInput()
{

}

void CNodeInput::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if ( bFlag )
	{
		fs << _T("NINPUT_B");
		fs << _T("VALUE");
		fs << m_dwInput;
		fs << _T("MARSK");
		fs << m_dwInputMask;
		fs << _T("NINPUT_E");
	}
	else
	{
		fs >> str;
		while ( str  !=  _T("NINPUT_E") )
		{
			if ( str == _T("VALUE") ) 
			{
				fs >> m_dwInput;
			}
			else if (str == _T("MARSK"))
			{
				fs >> m_dwInputMask;
			}

			fs >> str;
		}	
	}
}

UINT CNodeInput::Mark(HWND hWnd)
{
	g_DAT.MarkWaitSignal(m_dwInput, m_dwInputMask);
	return 0;
}


//
BOOL CNodeInput::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_INPUT )
		return FALSE;

	CNodeInput *pNode = (CNodeInput *)pSrc;
	///////
	m_property = pNode->m_property;
	m_bSelect  = pNode->m_bSelect;
	Set(pNode->m_dwInput, pNode->m_dwInputMask);

	return TRUE;
}

