// NodeBufferLaser.cpp : 实现文件
//
#include "stdafx.h"
#include "HL.h"
#include "NodeBufferLaser.h"


// CNodeBufferLaser
CNodeBufferLaser::CNodeBufferLaser()
{
	m_type = NODE_BUFFERLASER;
	//////
	m_laserPara.bLaserOn = FALSE;
	m_laserPara.dbCurrent = 5;
	m_laserPara.dbPower = 20;
	m_laserPara.dbQFreq = 10;
	m_laserPara.dbQRls = 5;
	m_laserPara.nLaserMode = 1;
}

CNodeBufferLaser::~CNodeBufferLaser()
{
}


// CNodeBufferLaser 成员函数
void CNodeBufferLaser::Serialize( FILE* fp,BOOL bFlag,BOOL bUnicode )
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if ( bFlag )
	{
		fs << _T("NBUFFERLASER_B");

		fs << _T("001");
		fs << m_laserPara.bLaserOn;

		fs << _T("002");
		fs << m_laserPara.dbCurrent;

		fs << _T("003");
		fs << m_laserPara.dbPower;

		fs << _T("004");
		fs << m_laserPara.dbQFreq;

		fs << _T("005");
		fs << m_laserPara.dbQRls;

		fs << _T("006");
		fs << m_laserPara.nLaserMode;

		fs << _T("007");
		fs << m_laserPara.nVer;

		fs << _T("NBUFFERLASER_E");
	}
	else
	{
		CString strValue = _T("");
		while( fs >> strValue )
		{
			if ( strValue == _T("NBUFFERLASER_B") )
			{
				continue;
			}
			else if ( strValue == _T("NBUFFERLASER_E") )
			{
				break;
			}
			else if ( strValue == _T("001") )
			{
				fs >> m_laserPara.bLaserOn;
			}
			else if ( strValue == _T("002") )
			{
				fs >> m_laserPara.dbCurrent;
			}
			else if ( strValue == _T("003") )
			{
				fs >> m_laserPara.dbPower;
			}
			else if ( strValue == _T("004") )
			{
				fs >> m_laserPara.dbQFreq;
			}
			else if ( strValue == _T("005") )
			{
				fs >> m_laserPara.dbQRls;
			}
			else if ( strValue == _T("006") )
			{
				fs >> m_laserPara.nLaserMode;
			}
			else if ( strValue == _T("007") )
			{
				fs >> m_laserPara.nVer;
			}
		}
	}
}


UINT CNodeBufferLaser::Mark( HWND hWnd )
{
	return  g_DAT.MarkBufferLaser( &m_laserPara );
}


BOOL CNodeBufferLaser::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( pSrc == NULL || pSrc->GetType()  !=  NODE_BUFFERLASER )
		return FALSE;

	SetBufferLaserPara( &(((CNodeBufferLaser*)pSrc)->m_laserPara) );

	return TRUE;
}

void CNodeBufferLaser::SetBufferLaserPara( LP_LASER_PARA pLaserPara )
{
	if ( !pLaserPara  )
		return;

	memcpy( &m_laserPara, pLaserPara, sizeof(BufferLaserPara) );
}

void CNodeBufferLaser::GetBudderLaserPara( LP_LASER_PARA pLaserPara )
{
	if ( !pLaserPara )
		return;

	memcpy( pLaserPara, &m_laserPara, sizeof(BufferLaserPara) );
}


BOOL CNodeBufferLaser::IsLaserOn()
{
	return  m_laserPara.bLaserOn;
}