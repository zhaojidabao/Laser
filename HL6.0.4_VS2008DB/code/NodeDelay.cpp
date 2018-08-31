// NodeDelay.cpp: implementation of the CNodeDelay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeDelay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeDelay::CNodeDelay()
{
	m_type = NODE_DELAY;
	m_dwDelay = 0;
}

CNodeDelay::~CNodeDelay()
{ 

}

void CNodeDelay::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if (bFlag)
	{
		fs << _T("NDELAY_B");
		fs << _T("LEN");
		fs << m_dwDelay;
		fs << _T("NDELAY_E");
	}
	else
	{
		fs >> str;
		while (str  !=  _T("NDELAY_E")) 
		{
			if (str == _T("LEN")) 
			{
				fs >> m_dwDelay;
			}

			fs >> str;
		}	
	}
}

UINT CNodeDelay::Mark(HWND hWnd)
{
	g_DAT.MarkDelay(m_dwDelay);

	return 0;
}

BOOL CNodeDelay::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_DELAY )
		return FALSE;
	
	CNodeDelay *pNode = (CNodeDelay *)pSrc;

	m_property = pNode->m_property;
	m_bSelect  = pNode->m_bSelect;
	SetDelay( pNode->m_dwDelay);

	return TRUE;
}

