// NodeOutput.cpp: implementation of the CNodeOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeOutput.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeOutput::CNodeOutput()
{
	m_type = NODE_OUTPUT;
	m_dwOutput = 0;
	m_dwOutputMask = 0;
	m_dwOutputDelay = 0;
}

CNodeOutput::~CNodeOutput()
{

}

void CNodeOutput::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if (bFlag)
	{
		fs << _T("NOUTPUT_B");
		fs << _T("VALUE");
		fs << m_dwOutput;
		fs << _T("MARSK");
		fs << m_dwOutputMask;
		fs << _T("DELAY");
		fs << m_dwOutputDelay;
		fs << _T("NOUTPUT_E");
	}
	else
	{
		fs >> str;
		while (str  !=  _T("NOUTPUT_E")) 
		{
			if (str == _T("VALUE")) {
				fs >> m_dwOutput;
			}
			else if (str == _T("MARSK")) {
				fs >> m_dwOutputMask;
			}
			else if (str == _T("DELAY")) {
				fs >> m_dwOutputDelay;
			}

			fs >> str;
		}	
	}
}

UINT CNodeOutput::Mark(HWND hWnd)
{
	g_DAT.MarkOutSignal(m_dwOutput, m_dwOutputMask, m_dwOutputDelay);
	return 0;
}

BOOL CNodeOutput::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_OUTPUT )
		return FALSE;

	CNodeOutput *pNode = (CNodeOutput *)pSrc;
	m_property = pNode->m_property;
	m_bSelect  = pNode->m_bSelect;
	Set(pNode->m_dwOutput, pNode->m_dwOutputMask, pNode->m_dwOutputDelay);

	return TRUE;
}


