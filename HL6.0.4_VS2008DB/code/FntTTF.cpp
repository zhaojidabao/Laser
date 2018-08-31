// FntTTF.cpp: implementation of the CFntTTF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "FntTTF.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFntTTF,CObject,2000)

CFntTTF::CFntTTF()
{

}

CFntTTF::~CFntTTF()
{

}

BOOL CFntTTF::MakeStroke()
{
	CDC dc;
	if ( !dc.CreateCompatibleDC(NULL) )
		return FALSE;

	int nOldMap = dc.SetMapMode(MM_TEXT);
	if ( dc.GetMapMode() != MM_TEXT )
		return FALSE;
	
	//////////////////
	//以pes个逻辑单位为基准画文本
	int nK  = _ttoi(m_strPrecisK);
	int h    = -MulDiv(100,nK,20);
	
	//还原实际坐标的比例
	double ratioY = -m_dbHeight/(100*abs(h)*0.007418);
	double ratioX = -ratioY*m_dbWidthFactor;
	//
	LOGFONT lf = m_lf;
	lf.lfHeight = h;
	
	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont = dc.SelectObject(&font);
	
	UINT flag = dc.GetTextAlign();
	flag = TA_UPDATECP|TA_TOP;
	flag = dc.SetTextAlign(flag);
	
	int mode  = dc.SetBkMode(TRANSPARENT);
	CSize sz  = dc.GetTextExtent(_T("8"));
	int cs = (int)((m_bFixSpace ? m_dbFixSpace : m_dbCharSpace) / fabs(ratioX));
	int ls  = (int)(m_dbLineSpace/fabs(ratioY));

	int  nNum = m_strText.GetLength();
	LPCTSTR  lpText = m_strText.GetBuffer(nNum);

	CPoint  pt(0,-sz.cy);
	CQuad  quadLimit(0,0,0,0);
	BOOL   bNewLine(TRUE);
	int       nCount = 1;
	
	//由于语言种类太多，对于一些特殊的语言如梵文、阿拉伯语等，字符之间很少需要插入间距，
	//鉴于此，采用比较通用的方法进行判断处理是否能够一次性生成
	BOOL bFullExport = TRUE;
	if ( fabs(m_dbCharSpace) > EPSILON || m_bFixSpace || m_nPath == PATH_ROUND || m_strText.Find(TCHAR(0x000D))  >=  0 )
	{
		bFullExport = FALSE;
	}

	for( int i = 0; i < nNum; i++ )
	{
		dc.BeginPath();
		dc.MoveTo(pt);
		
		if ( bFullExport )
		{
			sz = dc.GetTextExtent( lpText, nNum );
			TextOut( dc.GetSafeHdc(),0,0,lpText,nNum);
			pt.x +=  (sz.cx + cs);
			i = nNum;
		}
		else
		{
			if ( m_strText.GetAt(i) == 0x000D )
			{
				pt = dc.GetCurrentPosition();
				pt.x = 0;
				pt.y += ( sz.cy + ls );
				i++;
				bNewLine = TRUE;
			}
			else
			{
				sz = dc.GetTextExtent(lpText+i, 1 );
				if ( m_bFixSpace && !bNewLine ) 
				{
					pt.x  -=  sz.cx/2;
					dc.MoveTo(pt);
				}
				
				TextOut(dc.GetSafeHdc(),0,0,lpText+i,1);
				pt.x  +=  sz.cx/(m_bFixSpace?2:1)+cs;
				bNewLine = FALSE;
			}
		}
		//
		dc.EndPath();
		
		//检取路径数据
		dc.FlattenPath();
		int nCount =  dc.GetPath(NULL,NULL,0);
		CPoint* points =  new CPoint[nCount];
		if ( points == NULL )
			return FALSE;

		BYTE*  flags  = new BYTE[nCount];
		if ( flags == NULL )
		{
			delete[] points;
			return FALSE;
		}

		dc.GetPath(points,flags,nCount);
		quadLimit = MakeStrokeByPoints(points,flags,nCount);

		delete []points;
		delete []flags;
		
		if ( quadLimit.IsEmpty() )
			continue;
		
		CStroke* pStroke = new CStroke(m_property);
		if ( pStroke )
		{
			pStroke->m_bCharEnd = TRUE;
			pStroke->SetParent((LPVOID*)this);
			m_list.AddTail(pStroke);
		}
	}
	
	m_strText.ReleaseBuffer();
	
	dc.SetMapMode(nOldMap);
	dc.SetBkMode(mode);
	dc.SelectObject(pOldFont);
	font.DeleteObject();
	dc.SetTextAlign(flag);
	dc.DeleteDC();

	m_quadLimit.SetEmpty();
	CStroke* pStroke  = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke )
		{
			pStroke->Scale(CDot(0,0),ratioX,ratioY);
			m_quadLimit.Union(pStroke->CalLimit());
		}
	}
	
	return TRUE;
}

CQuad CFntTTF::MakeStrokeByPoints( CPoint* points, BYTE* flags, int nCount )
{
	CQuad quadLimit(0,0,0,0);
	if ( points == NULL  || flags == NULL || nCount <= 0 )
		return quadLimit;
	
	CStroke *pStroke = NULL;
	CPoint     ptStart;
	BOOL      bFirst = FALSE;
	for( int i = 0; i < nCount; i++ )
	{
 		switch( flags[i] )
		{
		//移动当前点位置
		case PT_MOVETO:
			bFirst = TRUE;
			ptStart = points[i];
			break;
		//画直线
		case PT_LINETO:
			if ( bFirst )
			{
				if ( pStroke )
				{
					quadLimit.Union(pStroke->CalLimit());
				}				
				pStroke = new CStroke(m_property);
				if ( pStroke )
				{
					pStroke->SetParent((LPVOID*)this);
					pStroke->Add(ptStart);
					m_list.AddTail(pStroke);
					bFirst = FALSE;
				}
			}
			pStroke->Add(points[i]);
			break;
		
		case PT_LINETO|PT_CLOSEFIGURE://画直线并封闭图形
			if ( pStroke )
			{
				pStroke->Add(points[i]);
				if ( points[i] != ptStart )
				{
					pStroke->Add(ptStart);
				}
			}
			break;
		}
	}
	
	if ( pStroke )
	{
		quadLimit.Union(pStroke->CalLimit());
	}	
	
	return quadLimit;
}
