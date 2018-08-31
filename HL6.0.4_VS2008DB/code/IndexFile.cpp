// IndexFile.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "IndexFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserFontFile

CUserFontFile::CUserFontFile()
{
	m_pUserFontFile = new CFile();
	bIsOpen = FALSE;
	pSpace = new USERFONTFILE;
	InitSpace();
}

CUserFontFile::~CUserFontFile()
{
	while(!m_UserFontList.IsEmpty())
		delete m_UserFontList.RemoveTail();
	if (pSpace)
		delete pSpace;
	if (m_pUserFontFile)
	{
		if (bIsOpen)
			CloseFile();
		delete m_pUserFontFile;
	}
}




/////////////////////////////////////////////////////////////////////////////
// CUserFontFile message handlers

BOOL CUserFontFile::JurdgeVersion(double nVersion)
{
	if (m_nIndexFileHead.dbVersion > 1.4)
		return FALSE;

	return TRUE;
}

USERFONTFILE *CUserFontFile::FindIndex(WORD nFontCode, BOOL bIsDouble)
{
	USERFONTFILE *pFont;
	POSITION pos;
	//if (bIsDouble)
	//	pos = m_UserFontList.FindIndex(m_nIndexFileHead.nDoubleIndex);
	//else
	pos = m_UserFontList.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pFont = m_UserFontList.GetNext(pos);
		if (pFont->nFontCode == nFontCode)
			return pFont;
		//if (!bIsDouble)
		//	if (pFont->nIndex > m_nIndexFileHead.nDoubleIndex)
		//		break;
	}
	return NULL;
}

void CUserFontFile::SaveFile()
{
	m_strFileName = GetFileName(FONTFILENAME);
	if (m_pUserFontFile->Open(m_strFileName,CFile::modeReadWrite | CFile::modeCreate) == 0)
	{
		AfxMessageBox(IDS_FONT_FILE_ERR,MB_ICONWARNING);
		return;
	}

	SaveHead();
	POSITION pos;
	USERFONTFILE* pIndex = NULL;
	pos = m_UserFontList.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pIndex = m_UserFontList.GetNext(pos);
		if (pIndex == NULL)
			return;
		m_pUserFontFile->Write(pIndex,sizeof(USERFONTFILE));	

	}
}

void CUserFontFile::LoadFile()
{
	ASSERT(m_pUserFontFile  !=  NULL);

	if (!bIsOpen)
		return;

	LoadHead();
	while(!m_UserFontList.IsEmpty())
		delete m_UserFontList.RemoveTail();
	m_pUserFontFile->Seek(sizeof(INDEXFILEHEAD),CFile::begin);
	USERFONTFILE *pFont = NULL;
	for(UINT i  = 0;i<m_nIndexFileHead.nFontNumber;i++)
	{
		pFont = new USERFONTFILE;
		memset(pFont,0,sizeof(USERFONTFILE));
		m_pUserFontFile->Read(pFont,sizeof(USERFONTFILE));
		m_UserFontList.AddTail(pFont);
	}
}


CString CUserFontFile::GetFileName(CString filename)
{
	m_strFilePath = ::g_sys.GetInstallPath();

	if ( filename == _T("") )
	{
		filename = m_strFilePath + filename;
		return filename;
	}

	filename = m_strFilePath+FONTFILENAME;
	return filename;
}

void CUserFontFile::SaveHead()
{
	ASSERT(m_pUserFontFile  !=  NULL);
	m_pUserFontFile->SeekToBegin( );	
	m_pUserFontFile->Write(&m_nIndexFileHead,sizeof(INDEXFILEHEAD));
}

void CUserFontFile::SaveModify(USERFONTFILE *pIndex)
{
	ASSERT(pIndex  !=  NULL);
	ASSERT(m_pUserFontFile  !=  NULL);
	UINT nFontNumber = sizeof(PntStruct);       
	UINT nHeadNumber = sizeof(FontFileHead);	
}

void CUserFontFile::LoadHead()
{
	ASSERT(m_pUserFontFile  !=  NULL);
	memset(&m_nIndexFileHead,0,sizeof(INDEXFILEHEAD));
	m_pUserFontFile->SeekToBegin( );	
	m_pUserFontFile->Read(&m_nIndexFileHead,sizeof(INDEXFILEHEAD));
}

void CUserFontFile::OpenFile(UINT nOpenFlag)
{
	ASSERT(m_pUserFontFile);
	if (bIsOpen)
		return;
	m_strFileName = GetFileName(FONTFILENAME);
	switch(nOpenFlag)
	{
	case 1:    //read
		{
			if (m_pUserFontFile->Open(m_strFileName,CFile::modeRead) == 0)
			{
				AfxMessageBox(IDS_FONT_FILE_ERR,MB_ICONERROR);
				bIsOpen = FALSE;
				return;
			}
		}
		break;
	case 2:    //read and write          
		{
			if (m_pUserFontFile->Open(m_strFileName,CFile::modeReadWrite ) == 0)
			{
				if (m_pUserFontFile->Open(m_strFileName,CFile::modeReadWrite | CFile::modeCreate) == 0)
				{
					AfxMessageBox(IDS_FONT_FILE_ERR,MB_ICONWARNING);
					bIsOpen = FALSE;
					return;
				}
			}
		}
		break;
	case 3:    //create . read and write
		{
			if (m_pUserFontFile->Open(m_strFileName,CFile::modeReadWrite | CFile::modeCreate) == 0)
			{
				AfxMessageBox(IDS_FONT_FILE_ERR,MB_ICONWARNING);
				return;
			}
		}
		break;
	default:
		break;
	}
	bIsOpen = TRUE;
	//LoadHead();
}

void CUserFontFile::CloseFile()
{
	ASSERT(m_pUserFontFile  !=  NULL);
	if (bIsOpen)
		m_pUserFontFile->Close();
	bIsOpen = FALSE;
}

BOOL CUserFontFile::DeleteIndex(WORD nCode)
{
	BOOL bFlag = FALSE;
	POSITION pos,prev;
	USERFONTFILE *pFont = NULL;
	pos = m_UserFontList.GetHeadPosition();
	while(pos  !=  NULL)
	{
		prev = pos;
		pFont = m_UserFontList.GetNext(pos);
		if (pFont->nFontCode == nCode)
		{
			delete m_UserFontList.GetAt(prev);
			m_UserFontList.RemoveAt(prev);
			bFlag = TRUE;
			break;
		}
	}
	return bFlag;
}

void CUserFontFile::SaveAll()
{
	POSITION pos;
    USERFONTFILE *pFont = NULL;
	m_pUserFontFile->SeekToBegin();
	m_nIndexFileHead.dbVersion = VERSION;
	m_nIndexFileHead.nFontNumber = m_UserFontList.GetCount();
	m_pUserFontFile->Write(&m_nIndexFileHead,sizeof(INDEXFILEHEAD));
	//m_pUserFontFile->Seek(sizeof(INDEXFILEHEAD),CFile::begin);
	pos = m_UserFontList.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pFont = m_UserFontList.GetNext(pos);
		m_pUserFontFile->Write(pFont,sizeof(USERFONTFILE));
	}
}

USERFONTFILE* CUserFontFile::GetDataOfChar(WORD c)
{

	OpenFile(1);
	LoadFile();
	USERFONTFILE*pTem = FindIndex(c);
	UINT nHeight = 0;
	if (pTem == NULL)
	{
		if (c == 0x20)//´¦Àí¿Õ¸ñ
			return pSpace;
		else
			return NULL;
	}
	switch (pTem->nLatticeStyle)
	{
	case 0:
		nHeight = 7;
		break;
	case 1:
		nHeight = 9;
		break;
	case 2:
		nHeight = 12;
		break;
	case 3:
		nHeight = 16;
		break;
	}
	for(int t  = 0;t<pTem->nPointNumber;t++)
	{
		pTem->point[t].y = pTem->point[t].y - nHeight;
	}
	CloseFile();
	return pTem;
}

void CUserFontFile::InitSpace()
{
	memset(pSpace,0,sizeof(USERFONTFILE));
	pSpace->nFontCode = 0x20;
}

