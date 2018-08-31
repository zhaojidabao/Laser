// FontUser.cpp: implementation of the CFontUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FontUser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFontUser::CFontUser()
{
	m_hModule = NULL;
	m_pGenString = NULL;

#ifdef _DEBUG
	m_hModule  = ::LoadLibrary(g_sys.GetInstallPath() + _T("UserFontDll_D.dll"));
#else 
	m_hModule  = ::LoadLibrary(g_sys.GetInstallPath() + _T("UserFontDll.dll"));
#endif
	
	if ( m_hModule )
	{
		m_pGenString  = (GENSTRING)GetProcAddress(m_hModule,"GenString");
	}
	
	m_strFileName = _T("");
}

//
CFontUser::~CFontUser()
{
	if ( m_hModule )
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
		m_pGenString = NULL;
	}
}

//
BOOL CFontUser::MakeStroke()
{	
	if ( NULL == m_hModule || NULL == m_pGenString )
	{
		AfxMessageBox(_T("Load UserFont library failure!"));
		return FALSE;
	}
	
	char szFileName[MAX_PATH] = {0};
	char szText[200] = {0};

	//
#ifdef _UNICODE
	CStdioFileEx::GetMultiByteFromUnicode(m_strFileName.GetBuffer(m_strFileName.GetLength()),szFileName,MAX_PATH);
	m_strFileName.ReleaseBuffer();
	//
	CStdioFileEx::GetMultiByteFromUnicode(m_strText.GetBuffer(m_strText.GetLength()),szText,200);
	m_strText.ReleaseBuffer();
#else
	_tcscpy_s(szFileName,MAX_PATH,m_strFileName);
	_tcscpy_s(szText,200,m_strText);
#endif 
	
	CFileFind fFind;
	if ( !fFind.FindFile(CString(szFileName)) )
	{
		AfxMessageBox(_T("Can not find the font: \n") + CString(szFileName) );
		return FALSE;
	}
	
	CBaseNode *pNode = NULL;
	pNode   =  m_pGenString(	szFileName, 
										szText,
										m_bFixSpace ? TRUE : FALSE,
										m_bFixSpace ? m_dbFixSpace : m_dbCharSpace,
										m_dbHeight,
										1.0,
										m_dbWidthFactor,
										m_dbLineSpace
						  );
	
	if ( NULL == pNode )
		return FALSE;
 	
	CDot dt(0,0);
	BaseDot bt;
	SK *sk = NULL;
	CStroke *pStroke = NULL;
	
	m_quadLimit.SetEmpty();
	POSITION pos = pNode->m_list.GetHeadPosition();
	while( pos )
	{
		sk = pNode->m_list.GetNext(pos);
		int  nDot = sk->m_dotArray.GetSize();
		if ( nDot  <=  0 )
		{
			pStroke = new CStroke(m_property);
			if ( pStroke )
			{
				pStroke->m_bCharEnd = TRUE;
				pStroke->SetParent((LPVOID*)this);
				m_list.AddTail(pStroke);
			}
			continue;
		}
		else
		{
			pStroke = new CStroke(m_property);
			if ( !pStroke )
				continue;

			for( int i = 0; i<nDot; i++ )
			{
				bt = sk->m_dotArray.GetAt(i);
				dt.x = bt.x;
				dt.y = bt.y;
				pStroke->Add(dt);
			}
			pStroke->SetParent((LPVOID*)this);
			m_list.AddTail(pStroke);
			m_quadLimit.Union(pStroke->CalLimit());
		}
	}
	
	delete pNode;
	pNode = NULL;
	
	return TRUE;
}

