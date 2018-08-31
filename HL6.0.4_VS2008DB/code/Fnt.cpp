// Fnt.cpp: implementation of the CFnt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "Fnt.h"
#include "XDC.h"
#include "HLView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFnt::CFnt()
{
	//初始化值
	m_dbHeight = 5;
	m_dbLineSpace = 0.3;
	m_dbCharSpace = 0.3;
	m_dbWidthFactor = 1;
	
	m_nPath = PATH_LINE;
	m_dbRadius = 20;
	m_dbStart = 0;
	m_bCCW = FALSE;
	m_dotBase = CDot(0,0);
	
	m_nAlign = ALIGN_MID;
	m_bInside = FALSE;
	m_dbFixSpace = 8.0;  
	m_bFixSpace = FALSE;
	m_bPrintInverse = FALSE;

	m_nDrawType = FONT_TTF;

	m_dbEndAngle = 0;
	m_dbRadius2 = m_dbRadius;
}

BOOL CFnt::CreateStrokeList()
{
	Delete();

	//生成StrokeList
	if ( ! MakeStroke()  )
		return FALSE;

	//得到初始时边框大小
	CStroke *pStroke = NULL;
	m_quadInit.SetEmpty();
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke )
			m_quadInit.Union(pStroke->CalLimit());
	}
	
	//变形成所需的形状
	Effect();
	
	return TRUE;
}


BOOL CFnt::MakeStroke()
{
	return FALSE;
}

void CFnt::Draw(XDC *pDC)
{
	if ( pDC == NULL )
		return;

	HBRUSH hBrsh    = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hBrshOld =  (HBRUSH)pDC->SelectObject(hBrsh);

	CPen pen(PS_DOT,1,BLACK);
	CPen *pOldPen = pDC->SelectObject(&pen);

	if ( m_nPath == PATH_ROUND )
	{
		m_quadInit.Normal();
		
		//圆弧方式
		double d2 = 2*m_dbRadius;
		double d1 = 2*(m_dbRadius+m_quadInit.Height());
		
		CQuad quad1(m_dotBase,d1,d1);
		CQuad quad2(m_dotBase,d2,d2);
		pDC->Ellipse(quad1);
		pDC->Ellipse(quad2);
		
		CDot tmp;
		tmp = quad1.GetBaseDot(BASE_ML);
		pDC->MoveTo(tmp.x,tmp.y);
		tmp = quad1.GetBaseDot(BASE_MR);
		pDC->LineTo(tmp.x,tmp.y);
		tmp = quad1.GetBaseDot(BASE_TM);
		pDC->MoveTo(tmp.x,tmp.y);
		tmp = quad1.GetBaseDot(BASE_BM);
		pDC->LineTo(tmp.x,tmp.y);
	}
	
	pDC->SelectObject(hBrshOld);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	DeleteObject(hBrsh);

	
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos  !=  NULL)
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke)
			pStroke->Draw(pDC, m_nDrawType, 0);
	}
}


void CFnt::Effect()
{
	if ( m_nPath == PATH_ROUND )
	{
		m_quadInit.Normal();
		//以(0,0)为圆心作圆弧方式变形
		double dbRotate = 0;	//字符旋转角
		double dbAngle = m_dbStart*PAI/180.0; //起始角
		CDot   dotCircle,dotDer,dotCur;
		
		// 计算圆圆弧排布终止角，用于生成边框预览的圆框
		double dbTmpAngle;
		if( m_quadInit.Width() < 2*PAI*m_dbRadius )
			dbTmpAngle = m_quadInit.Width()/m_dbRadius;
		else
			dbTmpAngle = 2*PAI+dbAngle;
		m_dbRadius2 = m_dbRadius2 + m_quadInit.Height();


		// 计算文本排布框
		if ( m_bCCW )
			m_dbEndAngle = 180*(m_dbStart+dbTmpAngle)/PAI; //得到当前点的偏移角
		else
			m_dbEndAngle = 180*(m_dbStart-dbTmpAngle)/PAI;

		StrokeList listTmp;
		POSITION posPrev = NULL;
		POSITION pos = m_list.GetHeadPosition();
		CStroke *pStroke = NULL;
		while(pos != NULL)
		{
			posPrev = pos;
			pStroke = (CStroke*)m_list.GetNext(pos);
			if ( pStroke && pStroke->m_bCharEnd )
			{
				m_list.RemoveAt(posPrev);
				delete pStroke;
				pStroke = NULL;
				
				//印章反刻
				if( m_bPrintInverse )
					PrintInverse(listTmp);
				
				//
				CQuad quad = CalLimit(&listTmp);
				dotCur = quad.GetBaseDot(BASE_BM);

				//计算并返回该字符的旋转角及在圆弧上的输出坐标
				GetOffsetDotAngle(dotCur,m_dbRadius,dbAngle,m_bCCW,m_bInside,m_quadInit,dotCircle,dbRotate);
				
				dotDer.x = dotCircle.x - dotCur.x; //获取旋转后点和原点的坐标偏差
				dotDer.y = dotCircle.y - dotCur.y; 
				
				StrokeMove(&listTmp,dotDer.x,dotDer.y);   //首先将该字符所有点移至圆的切线
				if  ( m_bInside )  //以该字符Quad中心点为基准点进行旋转
					StrokeRotate(&listTmp,dotCircle,PAI/2.0+dbRotate);
				else
					StrokeRotate(&listTmp,dotCircle,dbRotate-PAI/2.0);
				
				//变形完毕，将变形后的点以实际圆心所在位置作偏移
				StrokeMove(&listTmp,m_dotBase.x,m_dotBase.y);
				listTmp.RemoveAll();
				continue;
			}
			else
				listTmp.AddTail(pStroke);
		}
	}
	else
	{
		if ( m_bFixSpace )//等间距
		{
			CStroke *pStroke = NULL;
			CQuad  quadCharH,quadCharT;
			quadCharH.SetEmpty();
			quadCharT.SetEmpty();	
			//////////
			POSITION pos = m_list.GetHeadPosition();
			while( pos  !=  NULL )
			{	
				pStroke = (CStroke*)m_list.GetNext(pos);
				if ( !pStroke )
					continue;
				if ( pStroke->m_bCharEnd )
					break;

				quadCharH.Union( pStroke->CalLimit() );
			}

			BOOL  bFirst(TRUE);
			pos = m_list.GetTailPosition();
			while( pos  !=  NULL )
			{	
				pStroke = (CStroke*)m_list.GetPrev(pos);
				if ( !pStroke )
					continue;
				if ( !bFirst && pStroke->m_bCharEnd )
					break;
				bFirst = FALSE;
				quadCharT.Union( pStroke->CalLimit() );
			}
			m_quadInit.left   = (quadCharH.left+quadCharH.right)/2;
			m_quadInit.right = (quadCharT.left+quadCharT.right)/2;
		}

		/* 左下角对齐到(0,0)*/
		double moveX(0);
		
		//对齐方式
		switch( m_nAlign )
		{
		case ALIGN_LEFT:
			moveX = -m_quadInit.left + m_dotBase.x;
			break;
		case ALIGN_RIGHT:
			moveX = -m_quadInit.right + m_dotBase.x;
			break;
		case ALIGN_MID:
			moveX = - (m_quadInit.left+0.5*m_quadInit.Width()) + m_dotBase.x;
			break;
		}
		
		//位置变形
		double moveY = m_dotBase.y;	
		CStroke *pStroke = NULL;
		POSITION posPrev,pos = m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{
			posPrev = pos;
			pStroke = (CStroke*)m_list.GetNext(pos);
			if ( pStroke && pStroke->m_bCharEnd == TRUE )
			{
				m_list.RemoveAt(posPrev);
				delete pStroke;
				continue;
			}
			pStroke->Move(moveX,moveY);
		}
		m_quadLimit.Move(moveX, moveY);
	}
}


BOOL CFnt::GetOffsetDotAngle(CDot dotCur, double dbRadius, double dbStartAngle, BOOL bCRotate, 
		BOOL bInside,CQuad quadTotal,CDot &dotRet,double &dbRotAngle)
{
	double dbNewRadius(0); 
	double dbLength = dotCur.x-quadTotal.left; //弧长
	if ( bCRotate )
		dbRotAngle = dbStartAngle+dbLength/dbRadius; //得到当前点的偏移角
	else
		dbRotAngle = dbStartAngle-dbLength/dbRadius;
	
	//得到当前点的半径
	if ( bInside )	
		dbNewRadius = quadTotal.top-dotCur.y+dbRadius;
	else	
		dbNewRadius = dotCur.y-quadTotal.bottom+dbRadius;
		
	dotRet.x = dbNewRadius*cos(dbRotAngle);
	dotRet.y = dbNewRadius*sin(dbRotAngle);
		
	return TRUE;
}

CQuad CFnt::CalLimit()
{
	m_quadLimit.SetEmpty();

	if ( m_nPath == PATH_ROUND )
	{
		m_quadInit.Normal();
		double d = 2*(m_dbRadius+m_quadInit.Height());
		m_quadLimit = CQuad(m_dotBase,d,d);
	}
	else
	{
		m_quadLimit.SetEmpty();
		POSITION pos = m_list.GetHeadPosition();
		CStroke *pStroke = NULL;
		while(pos != NULL)
		{	
			pStroke = (CStroke*)m_list.GetNext(pos);
			if ( pStroke )
				m_quadLimit.Union(pStroke->CalLimit());
		}
	}

	return m_quadLimit;
}

CQuad CFnt::CalLimit(StrokeList *pList)
{
	CQuad  quad(0,0,0,0);
	CStroke *pStroke = NULL;
	POSITION pos = pList->GetHeadPosition();
	while( pos != NULL )
	{
		pStroke = (CStroke*) pList->GetNext(pos);
		if ( pStroke )
			quad.Union( pStroke->CalLimit() );
	}
	
	return quad;
}

void CFnt::StrokeMove(StrokeList *pList,double moveX,double moveY)
{
	CStroke *pStroke = NULL;
	POSITION pos = pList->GetHeadPosition();
	while( pos != NULL )
	{	
		pStroke = (CStroke*) pList->GetNext(pos);
		if ( pStroke )
			pStroke->Move(moveX,moveY);
	}
	
}

void CFnt::StrokeRotate(StrokeList *pList,CDot dot,double angle)
{
	CStroke *pStroke = NULL;
	POSITION pos = pList->GetHeadPosition();
	while( pos != NULL )
	{
		pStroke = (CStroke*) pList->GetNext(pos);
		if ( pStroke )			
			pStroke->Rotate(dot,angle*180.0/PAI);
	}
}

CQuad CFnt::CalLimit( CDot dot,double dbXScale,double dbYScale )
{
	m_quadLimit.SetEmpty();
	if ( m_nPath == PATH_ROUND )
	{
		m_quadInit.Normal();
		double d = 2*(m_dbRadius+m_quadInit.Height()*dbXScale);
		m_quadLimit = CQuad(m_dotBase,d,d);
	}
	else
	{
		m_quadLimit.SetEmpty();
		
		CStroke *pStroke = NULL;
		POSITION pos = m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pStroke = (CStroke*)m_list.GetNext(pos);
			if ( pStroke )
				m_quadLimit.Union( pStroke->CalLimit( dot, dbXScale, dbYScale) );
		}
	}
	
	return m_quadLimit;
}


//
CQuad CFnt::GetMarkRange()
{
	CQuad quad;
	quad.SetEmpty();
	
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos  !=  NULL )
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke )
			quad.Union(pStroke->CalLimit( CDot(0,0), 1, 1));
	}
	
	return quad;
}


void CFnt::PrintInverse( StrokeList &listTmp )
{
	CQuad quad = CalLimit(&listTmp);
	CDot dot = quad.GetBaseDot(BASE_BM);
	
	//印章反刻
	CStroke *pStroke = NULL;
	POSITION pos = listTmp.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke*)listTmp.GetNext(pos);
		if( !pStroke )
			continue;

		pStroke->Scale( dot, -1, 1 );
	}
}