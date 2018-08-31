// NodeMotorPos.cpp: implementation of the CNodeMotorPos class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeMotorPos.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeMotorPos::CNodeMotorPos()
{
	m_type = NODE_MOTORPOS;
	memset(&m_arrayAxisPos,0,sizeof(double)*4);
}

CNodeMotorPos::~CNodeMotorPos()
{

}


void CNodeMotorPos::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if (bFlag)
	{
		fs << _T("NMOTORPOS_B");
		
		fs << 100;
		fs << m_arrayAxisPos[0];
		
		fs << 101;
		fs << m_arrayAxisPos[1];
		
		fs << 102;
		fs << m_arrayAxisPos[2];
		
		fs << 103;
		fs << m_arrayAxisPos[3];
		
		fs << _T("NMOTORPOS_E");
	}
	else
	{
		while( fs >> str )
		{	
			if ( str == _T("NMOTORPOS_E") )
				break;
			
			switch(_ttoi(str))
			{
				case 100:
					fs >> m_arrayAxisPos[0];
					break;
				case 101:
					fs >> m_arrayAxisPos[1];
					break;
				case 102:
					fs >> m_arrayAxisPos[2];
					break;
				case 103:
					fs >> m_arrayAxisPos[3];
					break;
			}
		}
	}
}


UINT CNodeMotorPos::Mark(HWND hWnd)
{
	if ( FALSE == g_DAT.m_btExtendInfo.FunData.bMotorMark )
		return 0;

	int nPos = 0;
	for( int i = 0; i < 4; i++)
	{		
		nPos = g_Motion.GetAxisPulse(i, m_arrayAxisPos[i]);
		
		int nRet = g_DAT.MarkSetBufferPos(i, nPos);
		
		if ( nRet  !=  0 )
			return nRet;
	}

	MYTRACE(_T("CNodeMotorPos::Mark()\n"));

	return 0;
}


BOOL CNodeMotorPos::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_MOTORPOS )
		return FALSE;

	CNodeMotorPos *pNode = (CNodeMotorPos *)pSrc;
	m_arrayAxisPos[0] = pNode->m_arrayAxisPos[0];
	m_arrayAxisPos[1] = pNode->m_arrayAxisPos[1];
	m_arrayAxisPos[2] = pNode->m_arrayAxisPos[2];
	m_arrayAxisPos[3] = pNode->m_arrayAxisPos[3];
	
	m_property = pNode->m_property;
	m_bSelect  = pNode->m_bSelect;
	
	return TRUE;
}

