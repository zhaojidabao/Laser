// NodePosCali.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "NodePosCali.h"


// CNodePosCali

CNodePosCali::CNodePosCali()
{
	m_type = NODE_POSCALI;
	m_nCaliType = 0;
}

CNodePosCali::~CNodePosCali()
{
}


// CNodePosCali 成员函数
void CNodePosCali::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if (bFlag)
	{
		fs << _T("NPOSCALI_B");

		fs << _T("001");
		fs << m_nCaliType;

		fs << _T("NPOSCALI_E");
	}
	else
	{
		fs >> str;
		while (str  !=  _T("NPOSCALI_E")) 
		{
			if (str == _T("001")) 
			{
				fs >> m_nCaliType;
			}
			fs >> str;
		}	
	}
}


UINT CNodePosCali::Mark(HWND hWnd)
{
	g_DAT.MarkBufferDynCali(m_nCaliType);
	return 0;
}



BOOL CNodePosCali::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_POSCALI  )
		return FALSE;

	CNodePosCali *pSrcIn = (CNodePosCali*)pSrc;
	m_property = pSrcIn->m_property;
	m_bSelect  = pSrcIn->m_bSelect;
	m_nCaliType = pSrcIn->m_nCaliType;

	return TRUE;
}


