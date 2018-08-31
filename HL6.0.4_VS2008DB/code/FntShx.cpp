// FntShx.cpp: implementation of the CFntShx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "FntShx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/*
X、Y 位移和凸度（指定圆弧的曲率）的取值范围为 -127 到 +127。如果位移指定的线段长度为 D，
垂直于该线段中点的距离为 H，则凸度的幅值为 ((2 * H / D) * 127)。
如果圆弧从当前位置到新位置是顺时针走向的，则符号为负。 
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CFntShx,CObject,2000)

CFntShx::CFntShx()
{
	m_strChineseFont = _T("简体宋体");
	m_strEnglishFont  = _T("romans");
	m_strNumberFont = _T("romans");

	m_pFileEnglish  = new CFile();
	m_pFileChinese = new CFile();
	m_pFileNum     = new CFile();

	m_bEnglIshIsUnifont = FALSE;
	m_bNumIsUnifont = FALSE;

	m_ratio = 1;
	m_bDraw = TRUE;
	m_bVert = FALSE;
	m_bFirst = TRUE;
	m_ps = 0;
}

CFntShx::~CFntShx()
{
	if ( m_pFileEnglish )
	{
		delete m_pFileEnglish;
		m_pFileEnglish = NULL;
	}
	if ( m_pFileChinese )
	{
		delete m_pFileChinese;
		m_pFileChinese = NULL;
	}
	if ( m_pFileNum )
	{
		delete m_pFileNum;
		m_pFileNum = NULL;
	}
}

BOOL CFntShx::MakeFixStroke()
{
	char *pszVal = NULL;

#ifdef _UNICODE
	char  szText[300] = {0};
	CStdioFileEx::GetMultiByteFromUnicode(m_strText.GetBuffer(m_strText.GetLength()),szText,300);
	m_strText.ReleaseBuffer();
	pszVal = szText;
#else
	pszVal = (LPCTSTR)m_strText;
#endif

	FONT_INFO fontInfo;
	unsigned __int8 letter;

	m_dotCur.x = 0;
	m_dotCur.y = 0;
	m_dotPrev = m_dotCur;

	m_bEnglIshIsUnifont = m_bNumIsUnifont = FALSE;
	CString path = ::g_sys.GetInstallPath();
	
	if ( m_strChineseFont.IsEmpty())
	{
		AfxMessageBox(IDS_NOFINDFONT);
		return FALSE;
	}

	if ( m_strEnglishFont.IsEmpty() )
	{
		AfxMessageBox(IDS_NOFINDFONT);
		return FALSE;
	}
	
	//打开相应的字体文件
	if ( !m_pFileChinese->Open(path+_T("\\pfonts\\")+m_strChineseFont+_T(".shx"),CFile::modeRead))
	{
		return FALSE;
	}

	if ( !m_pFileEnglish->Open(path+_T("\\efonts\\")+m_strEnglishFont+_T(".shx"),CFile::modeRead))
	{
		m_pFileChinese->Close();
		return FALSE;
	}
	
	if ( m_strEnglishFont == m_strNumberFont )
	{
		m_pFileNumber = m_pFileEnglish;	
	}
	else
	{	
		if ( !m_pFileNum->Open(path+_T("\\efonts\\")+m_strNumberFont+_T(".shx"),CFile::modeRead))
		{
			m_pFileChinese->Close();
			m_pFileEnglish->Close();
			return FALSE;
		}

		m_pFileNumber = m_pFileNum;
	}

	//中文
	char buf[27] = {0};
	UINT n = m_pFileChinese->Read(buf,27);
	if ( strcmp(buf,"AutoCAD-86 bigfont 1.0\r\n\032\x008")  !=  0 )
	{
		m_pFileChinese->Close();
		m_pFileEnglish->Close();
		if ( m_pFileNumber  !=  m_pFileEnglish )
			m_pFileNumber->Close();
		return FALSE;//与标准文件头不符，则退出
	}
	
	BIGSHX_INDEX indexBigshx;
	m_pFileChinese->Read(&(m_BigshxHead),sizeof(BIGSHX_HEAD));			//读文件头
	m_pFileChinese->Seek(m_BigshxHead.number_esc*4,CFile::current);		//跳过换义表
	
	m_posBigshxIndexStart = (DWORD)(m_pFileChinese->GetPosition());
	
	m_pFileChinese->Read(&(indexBigshx),sizeof(BIGSHX_INDEX));			//读第一个散列表（形名区）
	m_pFileChinese->Seek(indexBigshx.pos,CFile::begin);					//跳到形名数据区
	m_pFileChinese->Seek(indexBigshx.len-4,CFile::current);				//跳到文件信息区
	m_pFileChinese->Read(&(fontInfo),sizeof(FONT_INFO));				//读文件信息
	m_dbUnitBigshx = fontInfo.ascent+fontInfo.descent;
	
	//英文
	m_pFileEnglish->SeekToBegin();
	m_pFileEnglish->Read(&(m_EnshxHeadEnglish),sizeof(ENSHX_HEAD));
	ENSHX_HEAD EnshxHeadEnglish1,EnshxHeadEnglish2;
	
	EnshxHeadEnglish2 = EnshxHeadEnglish1 = m_EnshxHeadEnglish;
	EnshxHeadEnglish1.c1[20] = 0;
	EnshxHeadEnglish2.c1[21] = 0;
	if ( strcmp(EnshxHeadEnglish1.c1,"AutoCAD-86 shapes 1.")  !=  0 )
	{
		if ( strcmp(EnshxHeadEnglish2.c1,"AutoCAD-86 unifont 1.")  != 0 )
		{
			m_pFileChinese->Close();
			m_pFileEnglish->Close();
			if ( m_pFileNumber  !=  m_pFileEnglish )
			{
				m_pFileNumber->Close();
			}
			
			return FALSE;
		}
		else
		{
			m_bEnglIshIsUnifont = TRUE;
		}
	}
	else
	{
		m_bEnglIshIsUnifont = FALSE;
	}
	
	if ( m_bEnglIshIsUnifont )
	{
		m_pFileEnglish->SeekToBegin();
		m_pFileEnglish->Read(&(m_UnifontHead),sizeof(UNIFONT_HEAD));
		m_pFileEnglish->Read(&(m_UnifontNum),sizeof(UNIFONT_FONTNUM));
		m_pFileEnglish->Read(&(m_UnifontIndexLen),sizeof(UNIFONT_FONTINDEXLEN));
		m_pFileEnglish->Seek(m_UnifontIndexLen.len_unifont - 6,CFile::current);
		m_pFileEnglish->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitEnglish = fontInfo.ascent+fontInfo.descent;
	}
	else
	{
		m_pIndexEnglish = new ENSHX_INDEX[m_EnshxHeadEnglish.total_ch];
		m_pFileEnglish->Read(m_pIndexEnglish,m_EnshxHeadEnglish.total_ch*4);
		m_pFileEnglish->Seek(m_pIndexEnglish->ch_bytes-4,CFile::current);
		m_pFileEnglish->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitEnglish = fontInfo.ascent+fontInfo.descent;
	}	

	//数字
	m_pFileNumber->SeekToBegin();
	m_pFileNumber->Read(&(m_EnshxHeadNumber),sizeof(ENSHX_HEAD));
	ENSHX_HEAD EnshxHeadNumber1,EnshxHeadNumber2;
	EnshxHeadNumber2 = EnshxHeadNumber1 = m_EnshxHeadNumber;
	EnshxHeadNumber1.c1[20] = 0;
	EnshxHeadNumber2.c1[21] = 0;
	
	if ( strcmp(EnshxHeadNumber1.c1,"AutoCAD-86 shapes 1.")  !=  0 )
	{
		if ( strcmp(EnshxHeadNumber2.c1,"AutoCAD-86 unifont 1.")  !=  0 )	
		{
			m_pFileChinese->Close();
			m_pFileEnglish->Close();
			if ( m_pFileNumber  !=  m_pFileEnglish )
				m_pFileNumber->Close();
			return FALSE;
		}
		else
		{
			m_bNumIsUnifont = TRUE;
		}		
	}
	else
	{
		m_bNumIsUnifont = FALSE;
	}
	
	if (m_bNumIsUnifont)
	{
		m_pFileNumber->SeekToBegin();
		m_pFileNumber->Read(&(m_UnifontHead),sizeof(UNIFONT_HEAD));
		m_pFileNumber->Read(&(m_UnifontNum),sizeof(UNIFONT_FONTNUM));
		m_pFileNumber->Read(&(m_UnifontIndexLen),sizeof(UNIFONT_FONTINDEXLEN));
		m_pFileNumber->Seek(m_UnifontIndexLen.len_unifont - 6,CFile::current);
		m_pFileNumber->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitNumber = fontInfo.ascent+fontInfo.descent;
	}
	else
	{
		m_pIndexNumber = new ENSHX_INDEX[m_EnshxHeadNumber.total_ch];
		m_pFileNumber->Read(m_pIndexNumber,m_EnshxHeadNumber.total_ch*4);
		m_pFileNumber->Seek(m_pIndexNumber->ch_bytes-4,CFile::current);
		m_pFileNumber->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitNumber = fontInfo.ascent+fontInfo.descent;
	}	
	
	BOOL bIsUnitFont = FALSE;
	CDot dotTmp = m_dotCur;
	while((letter = *pszVal)  !=  '\0')
	{	
		if ((unsigned __int8)(*pszVal)>0xa0 && (unsigned __int8)(*(pszVal+1))>0xa0)
		{
			//汉字的处理
			unsigned __int8 codeH = (unsigned __int8)(*pszVal);
			pszVal++;
			unsigned __int8 codeL = (unsigned __int8)(*pszVal);
			unsigned __int16 letter = codeH*256+codeL;
	
			m_ratio = 1/m_dbUnitBigshx;
			m_bDraw = TRUE;
			m_bVert = FALSE;
			m_bFirst = TRUE;
			m_ps = 0;
			process(letter);
		}
		else
		{
			if (letter >= 0x30&&letter <= 0X39)
			{
				//数字的处理
				m_pFile = m_pFileNumber;
				m_EnshxHead = m_EnshxHeadNumber;
				m_pIndexEnshx = m_pIndexNumber;
				m_dbUnitEnshx = m_dbUnitNumber;
				bIsUnitFont = m_bNumIsUnifont;
			}
			else
			{
				//英文的处理
				m_pFile = m_pFileEnglish;
				m_EnshxHead = m_EnshxHeadEnglish;
				m_pIndexEnshx = m_pIndexEnglish;
				m_dbUnitEnshx = m_dbUnitEnglish;
				bIsUnitFont = m_bEnglIshIsUnifont;
			}

			//回车换行的处理
			if ( letter == 0x0A )
			{
				dotTmp.x = m_dotCur.x = m_dotPrev.x = 0;
				m_dotPrev.y -=  m_dbLineSpace/m_dbHeight;
				m_dotCur.y = m_dotPrev.y;
			}
			
			m_ratio = 1/m_dbUnitEnshx;
			m_bDraw = TRUE;
			m_bVert = FALSE;
			m_ps = 0;
			m_bFirst = TRUE;
			if ( bIsUnitFont )
			{
				processUnifont(letter);
			}
			else
			{
				process(letter);
			}
		}
		
		pszVal++;
		//字间距的处理
		if ( letter  !=  0x0a )
		{
			dotTmp.x  +=  m_dbFixSpace/(m_dbHeight*m_dbWidthFactor);
 			m_dotPrev.x = dotTmp.x;
			m_dotCur.x = m_dotPrev.x;
		}

		CStroke *pStroke = new CStroke(m_property);
		if ( pStroke != NULL )
		{
			pStroke->m_bCharEnd = TRUE;
			pStroke->SetParent((LPVOID*)this);
			m_list.AddTail(pStroke);
		}
	}
	
	if ( !m_bEnglIshIsUnifont )
		delete []m_pIndexEnglish;
	
	if ( !m_bNumIsUnifont )
		delete []m_pIndexNumber;
	
	m_pFileChinese->Close();
	m_pFileEnglish->Close();
	
	if ( m_pFileNumber  !=  m_pFileEnglish )
		m_pFileNumber->Close();
	
	return TRUE;
}

BOOL CFntShx::MakeOldStroke()
{
	char *pszVal = NULL;
	//////////////
#ifdef _UNICODE
	char  szText[300] = {0};
	CStdioFileEx::GetMultiByteFromUnicode(m_strText.GetBuffer(m_strText.GetLength()),szText,300);
	m_strText.ReleaseBuffer();
	pszVal = szText;
#else
	pszVal = (LPCTSTR)m_strText;
#endif

	unsigned __int8 letter;
	FONT_INFO fontInfo;

	m_dotCur.x = 0;
	m_dotCur.y = 0;
	m_dotPrev = m_dotCur;

	m_bEnglIshIsUnifont = m_bNumIsUnifont = FALSE;
	CString path = g_sys.GetInstallPath();

	//打开相应的字体文件
	if (!m_pFileChinese->Open(path+_T("pfonts\\")+m_strChineseFont+_T(".shx"),CFile::modeRead))
	{
		return FALSE;
	}
	if ( !m_pFileEnglish->Open(path+_T("efonts\\")+m_strEnglishFont+_T(".shx"),CFile::modeRead))
	{
		m_pFileChinese->Close();
		return FALSE;
	}
	if ( m_strEnglishFont == m_strNumberFont )
	{
		m_pFileNumber = m_pFileEnglish;	
	}
	else
	{	
		if ( !m_pFileNum->Open(path+_T("efonts\\")+m_strNumberFont+_T(".shx"),CFile::modeRead))
			return FALSE;

		m_pFileNumber = m_pFileNum;
	}

	//中文
	char buf[27] = {0};
	m_pFileChinese->Read(buf,27);
	if ( 0  !=  strcmp(buf,"AutoCAD-86 bigfont 1.0\r\n\032\x008") )
	{
		m_pFileChinese->Close();
		m_pFileEnglish->Close();
		if ( m_pFileNumber  !=  m_pFileEnglish )
			m_pFileNumber->Close();
		return FALSE;//与标准文件头不符，则退出
	}
	
	BIGSHX_INDEX indexBigshx;
	m_pFileChinese->Read(&(m_BigshxHead),sizeof(BIGSHX_HEAD));			//读文件头
	m_pFileChinese->Seek(m_BigshxHead.number_esc*4,CFile::current);		//跳过换义表
	
	m_posBigshxIndexStart = (DWORD)(m_pFileChinese->GetPosition());
	
	m_pFileChinese->Read(&(indexBigshx),sizeof(BIGSHX_INDEX));			//读第一个散列表（形名区）
	m_pFileChinese->Seek(indexBigshx.pos,CFile::begin);					//跳到形名数据区
	m_pFileChinese->Seek(indexBigshx.len-4,CFile::current);				//跳到文件信息区
	m_pFileChinese->Read(&(fontInfo),sizeof(FONT_INFO));				//读文件信息
	m_dbUnitBigshx = fontInfo.ascent+fontInfo.descent;
	
	//英文
	m_pFileEnglish->SeekToBegin();
	m_pFileEnglish->Read(&(m_EnshxHeadEnglish),sizeof(ENSHX_HEAD));
	ENSHX_HEAD EnshxHeadEnglish1,EnshxHeadEnglish2;
	
	EnshxHeadEnglish2 = EnshxHeadEnglish1 = m_EnshxHeadEnglish;
	EnshxHeadEnglish1.c1[20] = 0;
	EnshxHeadEnglish2.c1[21] = 0;
	if ( strcmp(EnshxHeadEnglish1.c1,"AutoCAD-86 shapes 1.")  !=  0 )
	{
		if ( 0  !=  strcmp(EnshxHeadEnglish2.c1,"AutoCAD-86 unifont 1.") )
		{
			m_pFileChinese->Close();
			m_pFileEnglish->Close();
			if ( m_pFileNumber  !=  m_pFileEnglish )
			{
				m_pFileNumber->Close();
			}
			
			return FALSE;
		}
		else
		{
			m_bEnglIshIsUnifont = TRUE;
		}
	}
	else
	{
		m_bEnglIshIsUnifont = FALSE;
	}
	
	if (m_bEnglIshIsUnifont)
	{
		m_pFileEnglish->SeekToBegin();
		m_pFileEnglish->Read(&(m_UnifontHead),sizeof(UNIFONT_HEAD));
		m_pFileEnglish->Read(&(m_UnifontNum),sizeof(UNIFONT_FONTNUM));
		m_pFileEnglish->Read(&(m_UnifontIndexLen),sizeof(UNIFONT_FONTINDEXLEN));
		m_pFileEnglish->Seek(m_UnifontIndexLen.len_unifont - 6,CFile::current);
		m_pFileEnglish->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitEnglish = fontInfo.ascent+fontInfo.descent;
	}
	else
	{
		m_pIndexEnglish = new ENSHX_INDEX[m_EnshxHeadEnglish.total_ch];
		m_pFileEnglish->Read(m_pIndexEnglish,m_EnshxHeadEnglish.total_ch*4);
		m_pFileEnglish->Seek(m_pIndexEnglish->ch_bytes-4,CFile::current);
		m_pFileEnglish->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitEnglish = fontInfo.ascent+fontInfo.descent;
	}	

	//数字
	m_pFileNumber->SeekToBegin();
	m_pFileNumber->Read(&(m_EnshxHeadNumber),sizeof(ENSHX_HEAD));
	ENSHX_HEAD EnshxHeadNumber1,EnshxHeadNumber2;
	EnshxHeadNumber2 = EnshxHeadNumber1 = m_EnshxHeadNumber;
	EnshxHeadNumber1.c1[20] = 0;
	EnshxHeadNumber2.c1[21] = 0;
	
	if ( 0  !=  strcmp(EnshxHeadNumber1.c1,"AutoCAD-86 shapes 1.") )
	{
		if ( 0  !=  strcmp(EnshxHeadNumber2.c1,"AutoCAD-86 unifont 1.") )	 
		{
			m_pFileChinese->Close();
			m_pFileEnglish->Close();
			if ( m_pFileNumber  !=  m_pFileEnglish )
				m_pFileNumber->Close();
			return FALSE;
		}
		else
		{
			m_bNumIsUnifont = TRUE;
		}		
	}
	else
	{
		m_bNumIsUnifont = FALSE;
	}
	
	if ( m_bNumIsUnifont )
	{
		m_pFileNumber->SeekToBegin();
		m_pFileNumber->Read(&(m_UnifontHead),sizeof(UNIFONT_HEAD));
		m_pFileNumber->Read(&(m_UnifontNum),sizeof(UNIFONT_FONTNUM));
		m_pFileNumber->Read(&(m_UnifontIndexLen),sizeof(UNIFONT_FONTINDEXLEN));
		m_pFileNumber->Seek(m_UnifontIndexLen.len_unifont - 6,CFile::current);
		m_pFileNumber->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitNumber = fontInfo.ascent+fontInfo.descent;
	}
	else
	{
		m_pIndexNumber = new ENSHX_INDEX[m_EnshxHeadNumber.total_ch];
		m_pFileNumber->Read(m_pIndexNumber,m_EnshxHeadNumber.total_ch*4);
		m_pFileNumber->Seek(m_pIndexNumber->ch_bytes-4,CFile::current);
		m_pFileNumber->Read(&(fontInfo),sizeof(FONT_INFO));
		m_dbUnitNumber = fontInfo.ascent+fontInfo.descent;
	}	
	
	BOOL bIsUnitFont = FALSE;
	while( (letter = *pszVal)  !=  '\0' )
	{	
		//m_letterList.RemoveAll();
		if ((unsigned __int8)(*pszVal)>0xa0 && (unsigned __int8)(*(pszVal+1))>0xa0)
		{
			//汉字的处理
			unsigned __int8 codeH = (unsigned __int8)(*pszVal);
			pszVal++;
			unsigned __int8 codeL = (unsigned __int8)(*pszVal);
			unsigned __int16 letter = codeH*256+codeL;
			m_ratio = 1/m_dbUnitBigshx;
			m_bDraw = TRUE;
			m_bVert = FALSE;
			m_bFirst = TRUE;
			m_ps = 0;
			process(letter);
		}
		else
		{
			if (letter >= 0x30&&letter <= 0X39)
			{
				//数字的处理
				m_pFile = m_pFileNumber;
				m_EnshxHead = m_EnshxHeadNumber;
				m_pIndexEnshx = m_pIndexNumber;
				m_dbUnitEnshx = m_dbUnitNumber;
				bIsUnitFont = m_bNumIsUnifont;
			}
			else
			{
				//英文的处理
				m_pFile = m_pFileEnglish;
				m_EnshxHead = m_EnshxHeadEnglish;
				m_pIndexEnshx = m_pIndexEnglish;
				m_dbUnitEnshx = m_dbUnitEnglish;
				bIsUnitFont = m_bEnglIshIsUnifont;
			}

			//回车换行的处理
			if (letter == 0x0A)
			{	
				m_dotPrev.x  = 0;
				m_dotPrev.y  -= m_dbLineSpace/m_dbHeight;

				m_dotCur.x = m_dotPrev.x;
				m_dotCur.y = m_dotPrev.y;
			}
			
			m_ratio = 1/m_dbUnitEnshx;
			m_bDraw = TRUE;
			m_bVert = FALSE;
			m_bFirst = TRUE;
			m_ps = 0;
			if (bIsUnitFont)
			{
				processUnifont(letter);
			}
			else
			{
				process(letter);
			}		
		}
		
		pszVal++;
		//字间距的处理
		if (letter != 0x0a)
		{
			m_dotPrev.x +=  m_dbCharSpace/m_dbHeight/m_dbWidthFactor;
			m_dotCur.x = m_dotPrev.x;
			//ProcessLetter();
		}

		CStroke *pStroke = new CStroke(m_property);
		pStroke->m_bCharEnd = TRUE;
		pStroke->SetParent((LPVOID*)this);
		m_list.AddTail(pStroke);

	}
	
	if ( !m_bEnglIshIsUnifont )
		delete []m_pIndexEnglish;
	
	if ( !m_bNumIsUnifont )
		delete []m_pIndexNumber;
	
	m_pFileChinese->Close();
	m_pFileEnglish->Close();
	if ( m_pFileNumber  !=  m_pFileEnglish )
		m_pFileNumber->Close();
	
	return TRUE;
}



BOOL CFntShx::MakeStroke()
{
	if ( m_bFixSpace )
		return MakeFixStroke();
	else
		return MakeOldStroke();
}

__int8 CFntShx::decode(unsigned __int8 code)
{
	//取补码的真值
	BOOL neg = code & 0x80;
	if ( neg )		//bit 7 is 1
		return (code-0xFF-1);
	else
		return (code);
}


void CFntShx::CreateLine(CDot dot1,CDot dot2)
{
	CDot dot;
	if ( m_bFirst )
	{
		CStroke *pStroke = new CStroke((m_property));
		if ( pStroke == NULL )
			return;

		m_bFirst = FALSE;
		pStroke->SetParent((LPVOID*)this);
		m_list.AddTail(pStroke);
		m_pStrokeCur =  pStroke;
		//m_letterList.AddTail(pStroke);
	}
	if( m_pStrokeCur )
	{
		if( m_pStrokeCur->m_dotArray.GetCount() <= 0 )
		{
			dot.y = dot1.y*m_dbHeight;
			dot.x = dot1.x*m_dbWidthFactor*m_dbHeight;
			m_pStrokeCur->Add(dot);
		}
		else
		{
			dot.y = dot1.y*m_dbHeight;
			dot.x = dot1.x*m_dbWidthFactor*m_dbHeight;
			CDot dotTmp = m_pStrokeCur->GetAt(m_pStrokeCur->m_dotArray.GetCount()-1);
			if( fabs(dotTmp.x - dot1.x) > 0.0001 || fabs(dotTmp.y - dot1.y) > 0.0001)
			{
				m_pStrokeCur->Add(dot);
			}
		}

		dot.y = dot2.y*m_dbHeight;
		dot.x = dot2.x*m_dbWidthFactor*m_dbHeight;
		CDot dotTmp = m_pStrokeCur->GetAt(m_pStrokeCur->m_dotArray.GetCount()-1);
		if( fabs(dotTmp.x - dot2.x) > 0.0001 || fabs(dotTmp.y - dot2.y) > 0.0001)
		{
			m_pStrokeCur->Add(dot);
		}
	}
}

void CFntShx::CreateLine()
{
	CDot dot;

	if(m_bFirst)
	{
		m_bFirst=FALSE;

		CStroke *pStroke=new CStroke((m_property));
		m_pStrokeCur=pStroke;
		m_list.AddTail(m_pStrokeCur);

		dot.y=m_dotPrev.y*m_dbHeight;
		dot.x=m_dotPrev.x*m_dbWidthFactor*m_dbHeight;
		m_pStrokeCur->Add(dot);
	}

	dot.y=m_dotCur.y*m_dbHeight;
	dot.x=m_dotCur.x*m_dbWidthFactor*m_dbHeight;
	m_pStrokeCur->Add(dot);
}


void CFntShx::CreateArc(CDot dotCenter,double r,double angleStart,double angle)
{
	CDot dot;

	if ( m_bFirst )
	{
		m_bFirst = FALSE;

		CStroke *pStroke = new CStroke((m_property));
		m_pStrokeCur=pStroke;
		m_list.AddTail(m_pStrokeCur);

		dot.y = m_dotPrev.y*m_dbHeight;
		dot.x = m_dotPrev.x*m_dbWidthFactor*m_dbHeight;
		m_pStrokeCur->Add(dot);
	}

	int limit=20;
	double step=angle/limit;
	for( int i=1; i<=limit; i++ )
	{
		angle=i*step+angleStart;
		dot.x=r*cos(angle)+dotCenter.x;
		dot.y=r*sin(angle)+dotCenter.y;
		dot.y=dot.y*m_dbHeight;
		dot.x=dot.x*m_dbWidthFactor*m_dbHeight;
		m_pStrokeCur->Add(dot);
	}
}


void CFntShx::process(unsigned __int8 letter)
{
	int i = 0;
	long  move_bytes = 0;
	UINT read_bytes = 0;
	double lx,ly;
	unsigned __int8 t;
	
	CDot dot1,dot2,dotCenter;

	while(letter>(m_pIndexEnshx+i)->ch_asc)
	{	
		move_bytes += (m_pIndexEnshx+i)->ch_bytes;
		i++;
	}

	read_bytes = (m_pIndexEnshx+i)->ch_bytes;
	m_pFile->Seek(sizeof(ENSHX_HEAD)+m_EnshxHead.total_ch*4+move_bytes,CFile::begin);
	unsigned __int8 *ptr = new unsigned __int8[read_bytes+1];
	m_pFile->Read(ptr,read_bytes);
	unsigned __int8 *p = ptr;

	while(*p) p++;
	p++;         //jump head 0

	while(*p)
	{	
		switch(*p)
		{
		case 0x01:
			{
				if( m_bVert )
				{
					m_bVert = FALSE;
				}
				else
				{
					m_bDraw = TRUE;
					m_bFirst = TRUE;
				}
				p++;
				break;
			}
		case 0x02:
			{
				if( m_bVert )
				{
					m_bVert = FALSE;
				}
				else
				{
					m_bDraw = FALSE;
					m_bFirst = FALSE;
				}
				p++;
				break;
			}
		case 0x03:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					p++;
					break;
				}
				m_ratio /= *p++;
				break;
			}
		case 0x04:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					p++;
					break;
				}
				m_ratio *= *p++;
				break;
			}
		case 0x05:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if( m_ps<=9 )
				{
					m_stack[m_ps][0] = m_dotCur.x;
					m_stack[m_ps][1] = m_dotCur.y;
					m_ps++;
				}
				break;
			}
		case 0x06:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if( m_ps > 0 )
				{
					m_ps--;
					m_dotCur.x = m_stack[m_ps][0];
					m_dotCur.y = m_stack[m_ps][1];
					m_dotPrev=m_dotCur;
				}
				break;
			}
		case 0x07:
			{
				p++;
				t = *p;
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				BOOL bDraw = m_bDraw;
				BOOL bFirst = m_bFirst;
				m_bDraw = TRUE;
				m_bFirst = TRUE;
				process(t);
				m_bDraw = bDraw;
				m_bFirst = bFirst;
				break;
			}
		case 0x08:
			{
				p++;
				lx = decode(*p++);
				ly = decode(*p++);
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if ( m_bDraw )
				{	
					dot1 = m_dotCur;
					m_dotCur.x += m_ratio*lx;
					m_dotCur.y += m_ratio*ly;
					CreateLine();
				}
				else
				{
					m_dotCur.x += m_ratio*lx;
					m_dotCur.y += m_ratio*ly;
				}
				m_dotPrev=m_dotCur;
				break;
			}
		case 0x09:
			{
				/*
				p++;
				while( *p || *(p+1) )
				{
					if (m_bDraw)
					{
						dot1.x = m_dotCur.x;
						dot1.y = m_dotCur.y;
						dot2.x = m_dotCur.x + decode(*p)*m_ratio;
						dot2.y = m_dotCur.y + decode(*(p+1))*m_ratio;
						CreateLine(dot1,dot2);
					}

					if (m_bVert)
					{
						m_bVert = FALSE;
					}
					else
					{
						m_dotCur.x += decode(*p)*m_ratio;
						m_dotCur.y += decode(*(p+1))*m_ratio;
						m_dotPrev=m_dotCur;
					}
					p++;
					p++;
				}
				p++;
				p++;
				break;
				*/
				p++;
				do{
					lx = m_ratio*decode(*(p));
					ly = m_ratio*decode(*(++p));
					if(!m_bVert)
					{
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;

						if(m_bDraw)
							CreateLine();

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));
				if(m_bVert)
					m_bVert = FALSE;
				p++;
				p++;
				break;
			}
		case 0x0a:
			{
				if(m_bVert)
				{
					m_bVert = FALSE;
					p+=2;
				}
				else
				{
					//定义八分圆
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					double r,angleStart,angle;
					int s,c;
					r=*(++p)*m_ratio;
					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;
					angleStart=s*PAI/4;
					dotCenter.x=m_dotCur.x-r*cos(angleStart);
					dotCenter.y=m_dotCur.y-r*sin(angleStart);

					if(s<8)
					{	
						angle=c*PAI/4;
						m_dotCur.x=r*cos((s+c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4)+dotCenter.y;
					}
					else
					{	
						angle=-c*PAI/4;
						m_dotCur.x=r*cos((s-c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4)+dotCenter.y;
					}

					if(m_bDraw)
						CreateArc(dotCenter,r,angleStart,angle);
					m_dotPrev=m_dotCur;
				}
				p++;
				break;
			}
		case 0x0b:
			{
				if(m_bVert)
				{
					m_bVert = FALSE;
					p+=5;
				}
				else
				{	
					//定义小段圆弧,基于8分圆定义的基础上，加上相对起始角和相对终止角
					//S相对起始角
					//E相对终止角
					//H圆弧半径高8位
					//L圆弧半径低8位
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					int S,E,H,L,s,c;
					double r,angleStart,angle;
					double ang1,ang2;

					S=*(++p);
					E=*(++p);
					H=*(++p);
					L=*(++p);
					r=(H*256+L)*m_ratio;

					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;

					ang1=(S*45.0/255)*PAI/180;
					ang2=((255-E)*45.0/255)*PAI/180;
					if(E==0||E==1||E==2||E==255)ang2=0;

					if(s<8)
					{	
						angleStart=s*PAI/4+ang1;
						angle=c*PAI/4-ang2-ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s+c)*PAI/4-ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4-ang2)+dotCenter.y;
					}
					else
					{ 	
						angleStart=s*PAI/4-ang1;
						angle=-c*PAI/4+ang2+ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s-c)*PAI/4+ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4+ang2)+dotCenter.y;
					}

					if(m_bDraw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
				}
				p++;
				break;
			}
		case 0x0c:
			{
				if(m_bVert)
				{
					m_bVert=FALSE;
					p+=3;
				}
				else
				{
					//由位移和凸度定义圆弧
					CDot dotCenter;
					int B;
					double L1,L2,r,angleStart,angle;
					double ang1,ang2;

					lx=m_ratio*decode(*(++p));
					ly=m_ratio*decode(*(++p));

					L1=sqrt(lx*lx+ly*ly)/2;
					B=decode(*(++p));

					L2=L1*abs(B)/127;

					r=(L1*L1+L2*L2)/L2/2;

					ang1=atan2(ly,lx);
					ang2=atan2(r-L2,L1);
					if(B>=0)
					{
						ang1+=ang2;
						angleStart=ang1-PAI;
						angle=2*(PAI/2-ang2);
					}
					else
					{
						ang1-=ang2;
						angleStart=ang1+PAI;
						angle=-2*(PAI/2-ang2);
					}

					dotCenter.x=r*cos(ang1)+m_dotCur.x;
					dotCenter.y=r*sin(ang1)+m_dotCur.y;

					m_dotCur.x+=lx;
					m_dotCur.y+=ly;

					if(m_bDraw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
				}
				p++;
				break;
				
			}
		case 0x0d:
			{
				p++;
				//多个凸度定义圆弧，由（0，0）终止
				CDot dotCenter;
				int B;
				double L1,L2,r,angleStart,angle;
				double ang1,ang2;

				do
				{
					lx=m_ratio*decode(*(p));
					ly=m_ratio*decode(*(++p));
					B=decode(*(++p));

					if(!m_bVert)
					{
						L1=sqrt(lx*lx+ly*ly)/2;
						L2=L1*abs(B)/127;
						r=(L1*L1+L2*L2)/L2/2;

						ang1=atan2(ly,lx);
						ang2=atan2(r-L2,L1);
						if(B>=0)
						{
							ang1+=ang2;
							angleStart=ang1-PAI;
							angle=2*(PAI/2-ang2);
						}
						else
						{
							ang1-=ang2;
							angleStart=ang1+PAI;
							angle=-2*(PAI/2-ang2);
						}

						dotCenter.x=r*cos(ang1)+m_dotCur.x;
						dotCenter.y=r*sin(ang1)+m_dotCur.y;
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;

						if(m_bDraw)
							CreateArc(dotCenter,r,angleStart,angle);

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));

				if(m_bVert)
					m_bVert=FALSE;
				p++;
				p++;
				break;
			}	
		case 0x0E:	
			{
				m_bVert = TRUE;
				p++;
				break;
			}
		default:
			{
				if(*p&&(!m_bVert))
				{       	
					//01 decode
					lx=((*p)&0xF0)*(cad_vec[(*p)&0x0F][0])/32;
					ly=((*p)&0xF0)*(cad_vec[(*p)&0x0F][1])/32;
					m_dotCur.x+=m_ratio*lx;
					m_dotCur.y+=m_ratio*ly;
					if(m_bDraw)
						CreateLine();

					m_dotPrev=m_dotCur;
				}

				if(m_bVert)
					m_bVert=FALSE;
				p++;
				break;
			}
		}	//end of switch
	}	//end of while(*p)

	delete []ptr;
}

// zhq,2014-03-10 for shx双线字体
void CFntShx::process(unsigned __int16 letter)
{
	UINT read_bytes = 0;
	double lx,ly;
	unsigned __int16 t;
	//定义堆栈
	CDot dot1,dot2,dotCenter;

	BIGSHX_INDEX indexBigshx;
	m_pFileChinese->Seek(m_posBigshxIndexStart,CFile::begin);
	for(unsigned __int16 i = 0;i<m_BigshxHead.number_index;i++)
	{
		m_pFileChinese->Read(&indexBigshx,sizeof(BIGSHX_INDEX));
		if (letter == indexBigshx.code)
		{
			read_bytes = indexBigshx.len;
			break;
		}
	}
	
	if (read_bytes == 0)
	{
		return;
	}
	
	m_pFileChinese->Seek(indexBigshx.pos,CFile::begin);
	unsigned __int8 *ptr = new unsigned __int8[read_bytes+1];
	m_pFileChinese->Read(ptr,read_bytes);
	unsigned __int8 *p = ptr;

	// modify by zhq,2014-03-10
	while(*p) p++;
	p++;         //jump head 0
	
	while(*p)
	{	
		switch(*p)
		{
		case 0x01:
			{
				if( m_bVert )
				{
					m_bVert = FALSE;
				}
				else
				{
					m_bDraw = TRUE;
					m_bFirst = TRUE;
				}
				p++;
				break;
			}
		case 0x02:
			{
				if( m_bVert )
				{
					m_bVert = FALSE;
				}
				else
				{
					m_bDraw = FALSE;
					m_bFirst = FALSE;
				}
				p++;
				break;
			}
		case 0x03:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					p++;
					break;
				}
				m_ratio /= *p++;
				break;
			}
		case 0x04:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					p++;
					break;
				}
				m_ratio *= *p++;
				break;
			}
		case 0x05:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if( m_ps<=9 )
				{
					m_stack[m_ps][0] = m_dotCur.x;
					m_stack[m_ps][1] = m_dotCur.y;
					m_ps++;
				}
				break;
			}
		case 0x06:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if( m_ps > 0 )
				{
					m_ps--;
					m_dotCur.x = m_stack[m_ps][0];
					m_dotCur.y = m_stack[m_ps][1];
					m_dotPrev=m_dotCur;
				}
				break;
			}
		case 0x07:
			{
				p++;
				t = (*p)*256+(*(p+1));
				p+=2;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				BOOL bDraw = m_bDraw;
				BOOL bFirst = m_bFirst;
				m_bDraw = TRUE;
				m_bFirst = TRUE;
				process(t);
				m_bDraw = bDraw;
				m_bFirst = bFirst;
				break;
			}
		case 0x08:
			{
				p++;
				lx = decode(*p++);
				ly = decode(*p++);
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if ( m_bDraw )
				{	
					dot1 = m_dotCur;
					m_dotCur.x += m_ratio*lx;
					m_dotCur.y += m_ratio*ly;
					CreateLine();
				}
				else
				{
					m_dotCur.x += m_ratio*lx;
					m_dotCur.y += m_ratio*ly;
				}
				m_dotPrev=m_dotCur;
				break;
			}
		case 0x09:
			{
				/*
				p++;
				while( *p || *(p+1) )
				{
					if (m_bDraw)
					{
						dot1.x = m_dotCur.x;
						dot1.y = m_dotCur.y;
						dot2.x = m_dotCur.x + decode(*p)*m_ratio;
						dot2.y = m_dotCur.y + decode(*(p+1))*m_ratio;
						CreateLine(dot1,dot2);
					}

					if (m_bVert)
					{
						m_bVert = FALSE;
					}
					else
					{
						m_dotCur.x += decode(*p)*m_ratio;
						m_dotCur.y += decode(*(p+1))*m_ratio;
						m_dotPrev=m_dotCur;
					}
					p++;
					p++;
				}
				p++;
				p++;
				break;
				*/
				p++;
				do{
					lx = m_ratio*decode(*(p));
					ly = m_ratio*decode(*(++p));
					if(!m_bVert)
					{
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;

						if(m_bDraw)
							CreateLine();

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));
				if(m_bVert)
					m_bVert = FALSE;
				p++;
				p++;
				break;
			}
		case 0x0a:
			{
				if(m_bVert)
				{
					m_bVert = FALSE;
					p+=2;
				}
				else
				{
					//定义八分圆
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					double r,angleStart,angle;
					int s,c;
					r=*(++p)*m_ratio;
					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;
					angleStart=s*PAI/4;
					dotCenter.x=m_dotCur.x-r*cos(angleStart);
					dotCenter.y=m_dotCur.y-r*sin(angleStart);

					if(s<8)
					{	
						angle=c*PAI/4;
						m_dotCur.x=r*cos((s+c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4)+dotCenter.y;
					}
					else
					{	
						angle=-c*PAI/4;
						m_dotCur.x=r*cos((s-c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4)+dotCenter.y;
					}

					if(m_bDraw)
						CreateArc(dotCenter,r,angleStart,angle);
					m_dotPrev=m_dotCur;
				}
				p++;
				break;
			}
		case 0x0b:
			{
				if(m_bVert)
				{
					m_bVert = FALSE;
					p+=5;
				}
				else
				{	
					//定义小段圆弧,基于8分圆定义的基础上，加上相对起始角和相对终止角
					//S相对起始角
					//E相对终止角
					//H圆弧半径高8位
					//L圆弧半径低8位
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					int S,E,H,L,s,c;
					double r,angleStart,angle;
					double ang1,ang2;

					S=*(++p);
					E=*(++p);
					H=*(++p);
					L=*(++p);
					r=(H*256+L)*m_ratio;

					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;

					ang1=(S*45.0/255)*PAI/180;
					ang2=((255-E)*45.0/255)*PAI/180;
					if(E==0||E==1||E==2||E==255)ang2=0;

					if(s<8)
					{	
						angleStart=s*PAI/4+ang1;
						angle=c*PAI/4-ang2-ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s+c)*PAI/4-ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4-ang2)+dotCenter.y;
					}
					else
					{ 	
						angleStart=s*PAI/4-ang1;
						angle=-c*PAI/4+ang2+ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s-c)*PAI/4+ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4+ang2)+dotCenter.y;
					}

					if(m_bDraw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
				}
				p++;
				break;
			}
		case 0x0c:
			{
				if(m_bVert)
				{
					m_bVert=FALSE;
					p+=3;
				}
				else
				{
					//由位移和凸度定义圆弧
					CDot dotCenter;
					int B;
					double L1,L2,r,angleStart,angle;
					double ang1,ang2;

					lx=m_ratio*decode(*(++p));
					ly=m_ratio*decode(*(++p));

					L1=sqrt(lx*lx+ly*ly)/2;
					B=decode(*(++p));

					L2=L1*abs(B)/127;

					r=(L1*L1+L2*L2)/L2/2;

					ang1=atan2(ly,lx);
					ang2=atan2(r-L2,L1);
					if(B>=0)
					{
						ang1+=ang2;
						angleStart=ang1-PAI;
						angle=2*(PAI/2-ang2);
					}
					else
					{
						ang1-=ang2;
						angleStart=ang1+PAI;
						angle=-2*(PAI/2-ang2);
					}

					dotCenter.x=r*cos(ang1)+m_dotCur.x;
					dotCenter.y=r*sin(ang1)+m_dotCur.y;

					m_dotCur.x+=lx;
					m_dotCur.y+=ly;

					if(m_bDraw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
				}
				p++;
				break;
				
			}
		case 0x0d:
			{
				p++;
				//多个凸度定义圆弧，由（0，0）终止
				CDot dotCenter;
				int B;
				double L1,L2,r,angleStart,angle;
				double ang1,ang2;

				do
				{
					lx=m_ratio*decode(*(p));
					ly=m_ratio*decode(*(++p));
					B=decode(*(++p));

					if(!m_bVert)
					{
						L1=sqrt(lx*lx+ly*ly)/2;
						L2=L1*abs(B)/127;
						r=(L1*L1+L2*L2)/L2/2;

						ang1=atan2(ly,lx);
						ang2=atan2(r-L2,L1);
						if(B>=0)
						{
							ang1+=ang2;
							angleStart=ang1-PAI;
							angle=2*(PAI/2-ang2);
						}
						else
						{
							ang1-=ang2;
							angleStart=ang1+PAI;
							angle=-2*(PAI/2-ang2);
						}

						dotCenter.x=r*cos(ang1)+m_dotCur.x;
						dotCenter.y=r*sin(ang1)+m_dotCur.y;
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;

						if(m_bDraw)
							CreateArc(dotCenter,r,angleStart,angle);

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));

				if(m_bVert)
					m_bVert=FALSE;
				p++;
				p++;
				break;
			}	
		case 0x0E:	
			{
				m_bVert = TRUE;
				p++;
				break;
			}
		default:
			{
				if(*p&&(!m_bVert))
				{       	
					//01 decode
					lx=((*p)&0xF0)*(cad_vec[(*p)&0x0F][0])/32;
					ly=((*p)&0xF0)*(cad_vec[(*p)&0x0F][1])/32;
					m_dotCur.x+=m_ratio*lx;
					m_dotCur.y+=m_ratio*ly;
					if(m_bDraw)
						CreateLine();

					m_dotPrev=m_dotCur;
				}

				if(m_bVert)
					m_bVert=FALSE;
				p++;
				break;
			}
		}	//end of switch
	}	//end of while(*p)

	delete []ptr;
}

void CFntShx::Serialize(CArchive& ar)
{
}


void CFntShx::processUnifont(unsigned __int8 letter)
{	
	int i = 0;
	long move_bytes=0;
	UINT read_bytes=0;
	double lx,ly;

	//
	m_pFile->SeekToBegin();
	m_pFile->Read(&(m_UnifontHead),sizeof(UNIFONT_HEAD));
	m_pFile->Read(&(m_UnifontNum),sizeof(UNIFONT_FONTNUM));
	m_pFile->Read(&(m_UnifontIndexLen),sizeof(UNIFONT_FONTINDEXLEN));
	m_pFile->Seek(m_UnifontIndexLen.len_unifont,CFile::current);

	for(i=0; i<(int)(m_UnifontNum.num_unifont); i++)
	{	
		m_pFile->Read(&(m_UnifontCode),sizeof(UNIFONT_CODE));
		m_pFile->Read(&(m_UnifontCodelen),sizeof(UNIFONT_CODELEN));

		if(letter == m_UnifontCode.code)
		{
			break;
		}
		else
		{
			m_pFile->Seek(m_UnifontCodelen.codelen,CFile::current);
		}
	}

	if(i >= (int)(m_UnifontNum.num_unifont))
	{
		return;
	}

	unsigned __int8 *ptr=new unsigned __int8[m_UnifontCodelen.codelen + 1];
	m_pFile->Read(ptr,m_UnifontCodelen.codelen);
	unsigned __int8 *p=ptr;

	while(*p)
	{
		p++;
	}
	p++;

	while(*p)
	{	
		switch(*p)
		{
		case 0x01:
			if(m_bVert)
				m_bVert = FALSE;
			else
			{
				m_bDraw = TRUE;
				m_bFirst=TRUE;
			}
			p++;
			break;
		case 0x02:
			if( m_bVert )
				m_bVert = FALSE;
			else
			{
				m_bDraw = FALSE;
				m_bFirst=FALSE;
			}
			p++;
			break;
		case 0x03:
			if( m_bVert )
			{
				m_bVert = FALSE;
				p++;
			}
			else
				m_ratio /= *(++p);
			p++;
			break;
		case 0x04:
			if( m_bVert )
			{
				m_bVert = FALSE;
				p++;
			}
			else
				m_ratio*=*(++p);
			p++;
			break;
		case 0x05:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if( m_ps<=9 )
				{
					m_stack[m_ps][0] = m_dotCur.x;
					m_stack[m_ps][1] = m_dotCur.y;
					m_ps++;
				}
				break;
			}
		case 0x06:
			{
				p++;
				if (m_bVert)
				{
					m_bVert = FALSE;
					break;
				}
				if( m_ps > 0 )
				{
					m_ps--;
					m_dotCur.x = m_stack[m_ps][0];
					m_dotCur.y = m_stack[m_ps][1];
					m_dotPrev=m_dotCur;
				}
				break;
			}
		case 0x07:
			if(m_bVert)
			{
				m_bVert = FALSE;
				p++;
			}
			else
			{
				BOOL bDraw = m_bDraw;
				BOOL bFirst = m_bFirst;
				m_bDraw = TRUE;
				m_bFirst = TRUE;
				processUnifont(*(++p));
				m_bDraw = bDraw;
				m_bFirst = bFirst;
			}

			p++;
			break;

		case 0x08:
			lx = m_ratio*decode(*(++p));
			ly = m_ratio*decode(*(++p));

			if( m_bVert )
				m_bVert = FALSE;
			else
			{	
				m_dotCur.x+=lx;
				m_dotCur.y+=ly;

				if(m_bDraw)
					CreateLine();

				m_dotPrev=m_dotCur;
			}
			p++;
			break;
		case 0x09:
			p++;
			do{
				lx = m_ratio*decode(*(p));
				ly = m_ratio*decode(*(++p));
				if(!m_bVert)
				{
					m_dotCur.x+=lx;
					m_dotCur.y+=ly;

					if(m_bDraw)
						CreateLine();

					m_dotPrev=m_dotCur;
				}
				p++;
			}while(*p||*(p+1));
			if(m_bVert)
				m_bVert = FALSE;
			p++;
			p++;
			break;
		case 0x0a:
			if( m_bVert )
			{
				m_bVert = FALSE;
				p+=2;
			}
			else
			{
				//定义八分圆
				//r为圆弧半径
				//s为起始位置 0：0，1：45，2：90;
				//c为8分圆个数，0表示8个
				CDot dotCenter;
				double r,angleStart,angle;
				int s,c;
				r=*(++p)*m_ratio;
				s=(*(++p)&0xf0)>>4;
				c=*p&0x0f;
				if(c==0)c=8;
				angleStart=s*PAI/4;
				dotCenter.x=m_dotCur.x-r*cos(angleStart);
				dotCenter.y=m_dotCur.y-r*sin(angleStart);

				if(s<8)
				{	
					angle=c*PAI/4;
					m_dotCur.x=r*cos((s+c)*PAI/4)+dotCenter.x;
					m_dotCur.y=r*sin((s+c)*PAI/4)+dotCenter.y;
				}
				else
				{	
					angle=-c*PAI/4;
					m_dotCur.x=r*cos((s-c)*PAI/4)+dotCenter.x;
					m_dotCur.y=r*sin((s-c)*PAI/4)+dotCenter.y;
				}

				if(m_bDraw)
					CreateArc(dotCenter,r,angleStart,angle);
				m_dotPrev=m_dotCur;
			}
			p++;
			break;

		case 0x0b:
			if(m_bVert)
			{
				m_bVert=FALSE;
				p+=5;
			}
			else
			{	
				//定义小段圆弧,基于8分圆定义的基础上，加上相对起始角和相对终止角
				//S相对起始角
				//E相对终止角
				//H圆弧半径高8位
				//L圆弧半径低8位
				//r为圆弧半径
				//s为起始位置 0：0，1：45，2：90;
				//c为8分圆个数，0表示8个
				int S,E,H,L,s,c;
				double r,angleStart,angle;
				double ang1,ang2;
				CDot dotCenter;

				S=*(++p);
				E=*(++p);
				H=*(++p);
				L=*(++p);
				r=(H*256+L)*m_ratio;

				s=(*(++p)&0xf0)>>4;
				c=*p&0x0f;
				if(c==0)c=8;

				ang1=(S*45.0/255)*PAI/180;
				ang2=((255-E)*45.0/255)*PAI/180;
				if(E==0||E==1||E==2||E==255)ang2=0;

				if(s<8)
				{
					angleStart=s*PAI/4+ang1;
					angle=c*PAI/4-ang2-ang1;

					dotCenter.x=m_dotCur.x-r*cos(angleStart);
					dotCenter.y=m_dotCur.y-r*sin(angleStart);
					m_dotCur.x=r*cos((s+c)*PAI/4-ang2)+dotCenter.x;
					m_dotCur.y=r*sin((s+c)*PAI/4-ang2)+dotCenter.y;
				}
				else
				{ 	
					angleStart=s*PAI/4-ang1;
					angle=-c*PAI/4+ang2+ang1;

					dotCenter.x=m_dotCur.x-r*cos(angleStart);
					dotCenter.y=m_dotCur.y-r*sin(angleStart);
					m_dotCur.x=r*cos((s-c)*PAI/4+ang2)+dotCenter.x;
					m_dotCur.y=r*sin((s-c)*PAI/4+ang2)+dotCenter.y;
				}

				if(m_bDraw)
					CreateArc(dotCenter,r,angleStart,angle);

				m_dotPrev=m_dotCur;
			}
			p++;
			break;

		case 0x0c:
			if(m_bVert)
			{
				m_bVert=FALSE;
				p+=3;
			}
			else
			{
				//由位移和凸度定义圆弧
				CDot dotCenter;
				int B;
				double L1,L2,r,angleStart,angle;
				double ang1,ang2;

				lx=m_ratio*decode(*(++p));
				ly=m_ratio*decode(*(++p));

				L1=sqrt(lx*lx+ly*ly)/2;
				B=decode(*(++p));

				L2=L1*abs(B)/127;

				r=(L1*L1+L2*L2)/L2/2;

				ang1=atan2(ly,lx);
				ang2=atan2(r-L2,L1);
				if(B>=0)
				{
					ang1+=ang2;
					angleStart=ang1-PAI;
					angle=2*(PAI/2-ang2);
				}
				else
				{
					ang1-=ang2;
					angleStart=ang1+PAI;
					angle=-2*(PAI/2-ang2);
				}

				dotCenter.x=r*cos(ang1)+m_dotCur.x;
				dotCenter.y=r*sin(ang1)+m_dotCur.y;

				m_dotCur.x+=lx;
				m_dotCur.y+=ly;

				if(m_bDraw)
					CreateArc(dotCenter,r,angleStart,angle);

				m_dotPrev=m_dotCur;
			}
			p++;
			break;

		case 0x0d:
			{
				p++;
				//多个凸度定义圆弧，由（0，0）终止
				CDot dotCenter;
				int B;
				double L1,L2,r,angleStart,angle;
				double ang1,ang2;

				do
				{
					lx=m_ratio*decode(*(p));
					ly=m_ratio*decode(*(++p));
					B=decode(*(++p));

					if(!m_bVert)
					{
						L1=sqrt(lx*lx+ly*ly)/2;
						L2=L1*abs(B)/127;
						r=(L1*L1+L2*L2)/L2/2;

						ang1=atan2(ly,lx);
						ang2=atan2(r-L2,L1);
						if(B>=0)
						{
							ang1+=ang2;
							angleStart=ang1-PAI;
							angle=2*(PAI/2-ang2);
						}
						else
						{
							ang1-=ang2;
							angleStart=ang1+PAI;
							angle=-2*(PAI/2-ang2);
						}

						dotCenter.x=r*cos(ang1)+m_dotCur.x;
						dotCenter.y=r*sin(ang1)+m_dotCur.y;
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;

						if(m_bDraw)
							CreateArc(dotCenter,r,angleStart,angle);

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));

				if(m_bVert)
					m_bVert=FALSE;
				p++;
				p++;
				break;
			}	
		case 0x0E:	
			m_bVert=TRUE;
			p++;
			break;
		default	:
			if(*p&&(!m_bVert))
			{       	
				//01 decode
				lx=((*p)&0xF0)*(cad_vec[(*p)&0x0F][0])/32;
				ly=((*p)&0xF0)*(cad_vec[(*p)&0x0F][1])/32;
				m_dotCur.x+=m_ratio*lx;
				m_dotCur.y+=m_ratio*ly;
				if(m_bDraw)
					CreateLine();
				m_dotPrev=m_dotCur;
			}

			if(m_bVert)
				m_bVert=FALSE;
			p++;
			break;
		}	//end of switch
	}	//end of while(*p)

	delete []ptr;
}