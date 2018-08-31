// FntHzk.cpp: implementation of the CFntHzk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "FntHzk.h"

#include<stdlib.h>
#include<io.h>
#include<fcntl.h>
#include <conio.h>
#include <ctype.h>

#include "Dot.h"
#include "Stroke.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


#define HZK_PES		10		//以pes个逻辑单位为基准画文本
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFntHzk,CObject,2000)

CFntHzk::CFntHzk()
{
	m_pUserFile = new CUserFontFile();

	m_pUserFile->OpenFile(1);
	m_pUserFile->LoadFile();
	m_pUserFile->CloseFile();

	m_DotLeft = m_DotRight = 0;
	m_nDrawType = FONT_MATRIX;

	m_HandleASC = NULL;
	m_Handle = NULL;
}

CFntHzk::~CFntHzk()
{
	if ( m_pUserFile  !=  NULL )
	{
		delete m_pUserFile;
		m_pUserFile = NULL;
	}
}

//
//Open the Hzk 12 or 14 or 16
//
BOOL CFntHzk::OpenHzk()
{
	CString path = g_sys.GetInstallPath();
	char szPath[MAX_PATH] = { 0 };
	char szAsc[MAX_PATH] = { 0 };
	CStdioFileEx::GetMultiByteFromUnicode(path.GetBuffer(path.GetLength()),szPath,MAX_PATH);
	strcpy(szAsc,szPath);

	if (FONT_HZK_12 == m_strHzkType) {
		strcat(szPath,"DFonts\\hzk12");
		m_Handle = _open(szPath, O_RDONLY|O_BINARY);
		m_HZKtype = 12;
	}
	else if (FONT_HZK_14 == m_strHzkType) {
		strcat(szPath,"DFonts\\hzk14");
		m_Handle = _open(szPath, O_RDONLY|O_BINARY);
		m_HZKtype = 14;
	}
	else if (FONT_HZK_16 == m_strHzkType) {
		strcat(szPath,"DFonts\\hzk16");
		m_Handle = _open(szPath, O_RDONLY|O_BINARY);
		m_HZKtype = 16;
	}
	else{
		m_Handle = -1;
	}
	
	if ( m_Handle == -1 )
		return FALSE;
	
	if ( FONT_ASC_8_16 == m_strAscType )
	{
		strcat(szAsc,"DFonts\\ASC16");
		m_HandleASC = _open(szAsc, O_RDONLY|O_BINARY);
		if ( m_HandleASC == -1 )
			return FALSE;
	}
	
	return TRUE;
}

//
//读取点阵字库，获取文本点阵信息
//
int CFntHzk::PutHz(int x, int y, int z, char *p)
{
	unsigned int i,c1,c2,f = 0;
	int    i1,i2,i3,i4,rec;
	long  l;
	char by[32];

	//字间距
	int cs	 =  (int)(HZK_PES*(m_bFixSpace ? m_dbFixSpace : m_dbCharSpace)/m_dbWidthFactor/m_dbHeight);
	int ls	 =  (int)(HZK_PES*m_dbLineSpace/m_dbHeight);

	while( (i = *p++)  !=  0 )
	{
		if ( i > 0xa1 )		//中文
		{
			if (f == 0)
			{
				c1 = (i-0xa1)&0x7f;
				f = 1;
			}
			else
			{
				c2 = (i-0xa1)&0x7f;
				f = 0;
				rec = c1*94+c2;
				l = rec*m_HZKtype*2L;
				_lseek(m_Handle, l, SEEK_SET);
				_read(m_Handle, by, 32);

				////////////////////////////////////////
				//垂直读取
				for(i2 = 0; i2<2; i2++)	//每一行所有的列分为两组, 
				for(i3 = 0; i3<4; i3++)	//每一字节8列分为4个来回
				for(i4 = 0; i4<2; i4++)	//一个来回完成两列
				{
					for(i1 = 0; i1<m_HZKtype; i1++)	//每列有type行
					{
						if ( GetBit(by[i1*2+i2], 7-(i3*2+i4)) )
						{
							SaveInitDotList(x+i2*8+(i3*2+i4), y+i1);
						}
					}
				}
				x = x+z+cs;
				if ( !m_bFixSpace ) 
					x  +=  m_HZKtype;
  			}
		}
		else if (i == 0x0a || i == 0x0d)	//回车换行的处理
		{
			x = 0;
			y = (int)(ls+m_DotBottom);
		}
		else	//英文和数字
		{
			if (FONT_ASC_5_7 == m_strAscType)
			{
				//5*7点阵
				USERFONTFILE * pFont = GetDataOfChar(i);
				if (pFont == NULL)
				{
					x = x+7+z+cs;
					continue;
				}

				//垂直读取
				for(int i = 0; i<5; i++)
				{
					int j = 0;
					StrokeList	m_tempList;
					//垂直向上
					for(j = 6; j >= 0; j--)
					{
						for(i3 = 0; i3<pFont->nPointNumber; i3++)
						{
							if (pFont->point[i3].x == i && pFont->point[i3].y  == j)
							{
								SaveInitDotList(x+pFont->point[i3].x, y+7-pFont->point[i3].y);
							}
						}
					}

				}
				x = x+z+cs;
				if (!m_bFixSpace) {
					x += 7;
				}
			}
			else if (FONT_ASC_8_16 == m_strAscType)
			{
				//8*16点阵,垂直读取
				_lseek(m_HandleASC, i*16, SEEK_SET);
				_read(m_HandleASC, by, 16); // 读ASCII码点阵 
				for(i3 = 0; i3<4; i3++)
				for(i4 = 0; i4<2; i4++)
				{
					for(i1 = 0; i1<16; i1++)
					{
						if (GetBit(by[i1], 7-(i3*2+i4)))
						{
							SaveInitDotList(x+(i3*2+i4), y+i1);
						}
					}
				}
				x = x+z+cs;
				if (!m_bFixSpace) {
					x += 8;
				}
			}
		}

		CStroke *pStroke = new CStroke(m_property);
		pStroke->m_bCharEnd = TRUE;
		m_InitDotList.AddTail(pStroke);
	}
	//整体垂直回转排序
	SortDotList_V();

	return(x);
}


int CFntHzk::GetBit(unsigned char c, int n)
{
	return((c >> n)&1);
}


BOOL CFntHzk::MakeStroke()
{
	m_InitDotList.RemoveAll();

	if ( !OpenHzk() )
	{
		AfxMessageBox(IDS_HZK_OPENERROR);
		return FALSE;
	}

	m_bSaveFirst = TRUE;
	m_curWorkDir = 0;
	m_curFlyDir = 0;

	char szText[100] = {0};
	CStdioFileEx::GetMultiByteFromUnicode(m_strText.GetBuffer(m_strText.GetLength()),szText,100);
	m_strText.ReleaseBuffer();

	PutHz(0, 0, 0, szText);

	_getch();
	if ( m_Handle )
	{
		_close(m_Handle);
		m_Handle = NULL;
	}
	if ( m_HandleASC )
	{
		_close(m_HandleASC);
		m_HandleASC = NULL;
	}

	return TRUE;
}

void CFntHzk::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		//ar << m_lf;
	}
	else
	{
		// TODO: add loading code here
		//ar >> m_lf;
	}
}

//
//原始字库坐标信息的备份
void CFntHzk::SaveInitDotList(double x, double y)
{
	CStroke *pStroke = new CStroke(m_property); //

	pStroke->Add(CDot(x,y));
	m_InitDotList.AddTail(pStroke);

	if ( m_bSaveFirst )
	{
		m_bSaveFirst = FALSE;
		m_DotTop = m_DotBottom = (int)y;
		m_DotRight = m_DotLeft    = (int)x;
	}
	else
	{
		if ( x < m_DotLeft )	    
			m_DotLeft   = (int)x;
		if ( x > m_DotRight )	
			m_DotRight  = (int)x;
		if ( y < m_DotTop )	    
			m_DotTop   =  (int)y;
		if ( y > m_DotBottom )
			m_DotBottom = (int)y;
	}
}

void CFntHzk::SortDotList_V()
{
	m_list.RemoveAll();

	CStroke* pStroke = NULL;
	POSITION pos = m_InitDotList.GetHeadPosition();
	while(pos  !=  NULL)
	{	
		pStroke = (CStroke*)m_InitDotList.GetNext(pos);
		pStroke->SetParent((LPVOID*)this);
		m_list.AddTail(pStroke);
		
		if ( pStroke->IsEmpty() )
			continue;
		
		//座标反转,还原为设定高度,宽度系数
		double ratioY = -m_dbHeight/HZK_PES;
		double ratioX = -ratioY*m_dbWidthFactor;
		pStroke->Scale(CDot(0, 0), ratioX, ratioY);
	}
	
}

void CFntHzk::AddtoDotList(double x, double y, BOOL bRollBack)
{
	CStroke *pStroke = NULL;
	pStroke = new CStroke(m_property);
	
	pStroke->Add(CDot(x, y));
	pStroke->SetParent((LPVOID*)this);
	m_list.AddTail(pStroke);

	//座标反转
	//还原为设定高度,宽度系数
	double ratioY, ratioX;
	
	ratioY = -m_dbHeight/HZK_PES;
	ratioX = -ratioY*m_dbWidthFactor;
	pStroke->Scale(CDot(0, 0), ratioX, ratioY);
}

//
//改变点阵列表的工作台方向
//nDir == 0: Y->X(即将水平排列的点改为垂直排列)
//nDir == 1: X->Y(即将垂直排列的点改为水平排列)
void CFntHzk::ChangeWorkDirect(int nDir)
{
	if ( m_curWorkDir == nDir )
		return;

	if (nDir == 0)
	{
		//Copy m_InitDotList to m_list;
		m_list.RemoveAll();

		CStroke *pStroke = NULL;
		CDot dot;

		POSITION pos = m_InitDotList.GetHeadPosition();
		while(pos != NULL)
		{	
			pStroke = (CStroke*)m_InitDotList.GetNext(pos);
			dot = pStroke->GetAt(0);
			AddtoDotList(dot.x, dot.y);
		}
	}
	else
	{
		//Change the Direct
		m_list.RemoveAll();

		StrokeList	pStrokeList;
		CStroke *pStroke = NULL, *pTmp = NULL;
		POSITION pos, pos1;
		int i;
		CDot	pDot;
		BOOL	bOrder = TRUE, bFind;

		for(i = m_DotTop; i <= m_DotBottom; i++)
		{
			bFind = FALSE;
			pos = m_InitDotList.GetHeadPosition();
			pos1 = pStrokeList.GetTailPosition();
			while(pos != NULL)
			{	
				pStroke = (CStroke*)m_InitDotList.GetNext(pos);
				pDot = pStroke->GetAt(0);

				if (pDot.y == i)
				{
					bFind = TRUE;
					pTmp = new CStroke(m_property);
					pTmp->Add(pDot);

					if (bOrder)
						pStrokeList.AddTail(pTmp);
					else
						pStrokeList.InsertAfter(pos1, pTmp);

				}
			}
			if (bFind)
				bOrder = !bOrder;
		}

		pos = pStrokeList.GetHeadPosition();
		while(pos != NULL)
		{	
			pStroke = (CStroke*)pStrokeList.GetNext(pos);
			pDot = pStroke->GetAt(0);
			AddtoDotList(pDot.x, pDot.y);
		}
	}

	m_curWorkDir = nDir;
}


USERFONTFILE* CFntHzk::GetDataOfChar(WORD c)
{
	USERFONTFILE* pFont = NULL;
	////
	if ( IsDBCSLeadByte(c&0xFF) )
		pFont = m_pUserFile->FindIndex(c,TRUE);
	else
		pFont = m_pUserFile->FindIndex(c,FALSE);
	
	return pFont;
}