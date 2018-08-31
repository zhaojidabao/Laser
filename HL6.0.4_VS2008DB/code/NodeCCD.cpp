// NodeCCD.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "NodeCCD.h"


// CNodeCCD

CNodeCCD::CNodeCCD()
{
	m_type = NODE_CCD;

	m_cameraPara.nVer = 0x0001;
	m_cameraPara.nProgID = 0;
	m_cameraPara.nNGRepNum = 1;
	m_cameraPara.nOverTime = 100;
	m_cameraPara.nErrorMode = 0;
	m_cameraPara.nBaudRate = 5;
	m_cameraPara.bScanJump = FALSE;
	m_cameraPara.dbX = 0;
	m_cameraPara.dbY = 0;
	m_cameraPara.nDelay = 100;
		 
}

CNodeCCD::~CNodeCCD()
{
	
}


// CNodeCCD 成员函数
void CNodeCCD::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if (bFlag)
	{
		fs << _T("NCCD_B");

		fs << _T("001");
		fs << m_cameraPara.nVer;

		fs << _T("002");
		fs << m_cameraPara.nProgID;

		fs << _T("003");
		fs << m_cameraPara.nOverTime;

		fs << _T("004");
		fs << m_cameraPara.nNGRepNum;

		fs << _T("005");
		fs << m_cameraPara.nErrorMode;

		fs << _T("006");
		fs << m_cameraPara.nDelay;
		
		fs << _T("007");
		fs << m_cameraPara.nBaudRate;

		fs << _T("008");
		fs << m_cameraPara.dbY;

		fs << _T("009");
		fs << m_cameraPara.dbX;

		fs << _T("010");
		fs << m_cameraPara.bScanJump;

		fs << _T("NCCD_E");
	}
	else
	{
		fs >> str;
		while ( str != _T("NCCD_E") ) 
		{
			if (str == _T("001")) 
			{
				fs >> m_cameraPara.nVer;
			}
			else if ( str == _T("002") )
			{
				fs >> m_cameraPara.nProgID;
			}
			else if ( str == _T("003") )
			{
				fs >> m_cameraPara.nOverTime;
			}
			else if ( str == _T("004") )
			{
				fs >> m_cameraPara.nNGRepNum;
			}
			else if ( str == _T("005") )
			{
				fs >> m_cameraPara.nErrorMode;
			}
			else if ( str == _T("006") )
			{
				fs >> m_cameraPara.nDelay;
			}
			else if ( str == _T("007") )
			{
				fs >> m_cameraPara.nBaudRate;
			}
			else if ( str == _T("008") )
			{
				fs >> m_cameraPara.dbY;
			}
			else if ( str == _T("009") )
			{
				fs >> m_cameraPara.dbX;
			}
			else if ( str == _T("010") )
			{
				fs >> m_cameraPara.bScanJump;
			}
			
			fs >> str;
		}	
	}
}


UINT CNodeCCD::Mark(HWND hWnd)
{
	g_DAT.MarkBufferCamera( &m_cameraPara );

	return 0;
}



BOOL CNodeCCD::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_CCD )
		return FALSE;
	
	CNodeCCD *pNode = (CNodeCCD*)pSrc;
	m_property = pNode->m_property;
	m_bSelect  = pNode->m_bSelect;
	m_cameraPara = pNode->m_cameraPara;

	return TRUE;
}


void CNodeCCD::GetNodePara( BUF_CAMERA_PARA &cameraPara)
{
	cameraPara = m_cameraPara;
}


void CNodeCCD::SetNodePara( const BUF_CAMERA_PARA &cameraPara)
{
	m_cameraPara = cameraPara;
}