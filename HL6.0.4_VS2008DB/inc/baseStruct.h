#ifndef __HL_BASEQUAD_DEFINE_H_2010_0520_012345_
#define __HL_BASEQUAD_DEFINE_H_2010_0520_012345_

#include "..\ExportFile\Dot.h"
#include "InnerMsg.h"

typedef struct tagQuadSG
{
	tagQuadSG()
	{
		memset(this, 0, sizeof(tagQuadSG));
	};
	
	double Width()
	{
		return right - left;
	}
	
	double Height()
	{
		return top - bottom;
	}
	
	CDot Center()
	{
		CDot dot;
		dot.x = (left + right)  / 2;
		dot.y = (top  + bottom) / 2;
		return dot;
	}
	
	void operator = ( const tagQuadSG & src )
	{
		if( &src == this )	
			return;
		
		left = src.left;
		right = src.right;
		top = src.top;
		bottom = src.bottom;
	}
	
public:
	double left;
	double right;
	double top;
	double bottom;
}QUADSG;

typedef struct tagGNode
{
	tagGNode()
	{
		m_nNum  = 0;
		m_pGroupObj = 0;
		m_dbDistanc = -1;
		m_bIsSort   = FALSE;
	}
	
	~tagGNode()
	{
	}
	
	void operator = ( const tagGNode & src )
	{
		if( &src == this )	
			return;
		
		m_nNum   = src.m_nNum;
		m_pGroupObj  = src.m_pGroupObj;
        m_rt     = src.m_rt;
		m_bIsSort= src.m_bIsSort;
		m_dbDistanc = src.m_dbDistanc;
	}
	
	int     m_nNum;
	BOOL    m_bIsSort;
	DWORD   m_pGroupObj;
	double  m_dbDistanc; // 与线段起点距离
	QUADSG  m_rt;        // 标示范围起点
} GNODE;

typedef struct tagSortNode
{
	tagSortNode()
	{
		this->m_pGroupObj = NULL;
		this->m_nSort = 0;
		this->m_MNum = 0;
		this->m_Color = 0;
		this->m_nNewSort = 0;
	}
	
	~tagSortNode()
	{
		this->m_pGroupObj = NULL;
		this->m_nSort = 0;
		this->m_MNum = 0;
		this->m_Color = 0;
		this->m_nNewSort = 0;
	}
	
	void operator = ( const tagSortNode & src )
	{
		if( &src == this )	
			return;
		
		m_SPt.x = src.m_SPt.x;
		m_SPt.y = src.m_SPt.y;
		m_pGroupObj = src.m_pGroupObj;
		m_nSort = src.m_nSort;
		m_MNum  = src.m_MNum;
		m_Color = src.m_Color;
		m_nNewSort = src.m_nNewSort;
	}
	
public:
	CDot    m_SPt;       // 标示范围起点
	DWORD   m_pGroupObj;     // 排序对象指针
	int     m_nSort;     // 当前序号
	int     m_MNum;      // 标注点个数, 有重点时，当前点的序号
	COLORREF m_Color;    // 标注框颜色
	int     m_nNewSort;  // 新序
} SORTNODE;

typedef struct tagASNode
{
	tagASNode()
	{
		this->m_pGroupObj = NULL;
		this->m_nSort = 0;
	}
	
	~tagASNode()
	{
		this->m_pGroupObj = NULL;
		this->m_nSort = 0;
	}
	
	void operator = ( const tagASNode & src )
	{
		if( &src == this )	
			return;
		
		m_SDt   = src.m_SDt;
		m_EDt   = src.m_EDt;
		m_pGroupObj = src.m_pGroupObj;
		m_nSort = src.m_nSort;
	}
	
public:
	CDot    m_SDt;       // 标示范围起点
	CDot    m_EDt;       // 标示范围终点
	DWORD   m_pGroupObj;     // 排序对象指针
	int     m_nSort;     // 当前序号
} ASNODE;

typedef struct tagDOUBLEOUTPARA
{	
	tagDOUBLEOUTPARA()
	{
		memset(this,0,sizeof(tagDOUBLEOUTPARA));
		dbLineWidth=0.1;
	};
	
	void operator = ( const tagDOUBLEOUTPARA & src )
	{
		if( &src == this )	
			return;
		
		dbLeftOutAliX   = src.dbLeftOutAliX;
		dbLeftOutAliY   = src.dbLeftOutAliY;
        dbRightOutAliX  = src.dbRightOutAliX;
		dbRightOutAliY  = src.dbRightOutAliY;
		bDWFlag         = src.bDWFlag;
		dbLineWidth     = src.dbLineWidth;
		pos             = src.pos;
	}
	
public:	
	double	dbLeftOutAliX;
	double	dbLeftOutAliY;
	double	dbRightOutAliX;
	double	dbRightOutAliY;
	BOOL    bDWFlag;
	double dbLineWidth;
	CDot pos;
}DOUBLEOUTPARA;

#endif //__HL_BASEQUAD_DEFINE_H_2010_0520_012345_