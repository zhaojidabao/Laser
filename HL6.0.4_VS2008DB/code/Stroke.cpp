// Stroke.cpp: implementation of the CStroke class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "Stroke.h"
#include "HLDoc.h"
#include "HLView.h"
#include "Node.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CStroke,CObject,1)
CStroke::CStroke(CProperty pty)
{
	m_property  = pty;
	m_bTag      = FALSE;
	m_pParent   = NULL;
	m_bLockFlag = pty.m_bLockFlag;

	m_bCharEnd = FALSE;
	m_bFillFirst = FALSE;
}

CStroke::CStroke()
{
	m_bTag   = FALSE;
	m_pParent =  NULL;
	m_bLockFlag = FALSE;

	m_bCharEnd = FALSE;

}

CStroke::~CStroke()
{
}


void CStroke::AddRect(CWnd *pWnd)//给对象的结点加上红框
{
	if ( !pWnd || IsFillStyle() ) 
		return;

	int nCount = m_dotArray.GetSize();
	if ( nCount<1 )
		return;

	CZoomView *pView = (CZoomView*)pWnd;
	CDC  *pDC = pView->GetDC();
	ASSERT_VALID( pDC );

	CPen   pen(PS_SOLID,1,RGB(255,0,0));
	CPen  *pOldPen = pDC->SelectObject(&pen);
	
	double dbData = 3;
	pView->ActualLen(dbData);
	CQuad  quad;
	for( int i = 0; i < nCount; i++ )
	{
		CDot  dot = m_dotArray.GetAt(i);
		quad.left  = dot.x-dbData;
		quad.top  = dot.y+dbData;
		quad.right = dot.x+dbData;
		quad.bottom = dot.y-dbData;

		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);

		CRect  rect = (CRect)quad;

		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);

		if ( 0 == i )
		{
			pDC->FillSolidRect(&rect, RED);
		}

		if (1 == i )
		{
			pDC->FillSolidRect(&rect, BLUE);
		}
	}

	pDC->SelectObject(pOldPen);
	pView->ReleaseDC(pDC);
}

BOOL CStroke::Click(CPoint &point,CWnd *pWnd)//判断鼠标是否击中对象
{
	int nCount = m_dotArray.GetSize();
	if ( !pWnd || nCount <  1 )
		return FALSE;

	m_dbR =  g_SysCfg.m_dbClick;

	double x1,y1,x2,y2;
	double x,y;
	CDot mid,dot;
	CQuad quad;
	dot.x = point.x;
	dot.y = point.y;
	CZoomView *pView = (CZoomView*)pWnd;
	pView->LogicToPhysic(dot.x,dot.y);
	pView->ActualLen(m_dbR);
	for( int i = 0; i < nCount-1; i++ )
	{	
		x = dot.x;
		y = dot.y;
		x1 = m_dotArray[i].x;
		x2 = m_dotArray[i+1].x;
		y1 = m_dotArray[i].y;
		y2 = m_dotArray[i+1].y;

		// a----p----b
		double dbap,dbpb,dbab,dx,dy;
		dx = fabs(x1-dot.x);
		dy = fabs(y1-dot.y);
		dbap = sqrt(dx*dx+dy*dy);
		dx = fabs(x2-dot.x);
		dy = fabs(y2-dot.y);
		dbpb = sqrt(dx*dx+dy*dy);
		dx = fabs(x1-x2);
		dy = fabs(y1-y2);
		dbab = sqrt(dx*dx+dy*dy);

		if ( (dbap+dbpb) < (dbab+1.5*m_dbR) )
			return TRUE;
	}

	return FALSE;
}

void CStroke::DelRect(CWnd *pWnd)//去掉结点的红框
{	
	int nCount = m_dotArray.GetSize();
	if ( !pWnd || nCount < 1 )
		return;

	CZoomView *pView = (CZoomView*)pWnd;
	CDC     *pDC  = pView->GetDC();
	ASSERT_VALID( pDC );

	CQuad   quad;
	
	CPen    pen(PS_SOLID,1,RGB(255,0,0));
	CPen   *pOldPen  = pDC->SelectObject(&pen);
	
	double  dbData  = 3;
	pView->ActualLen(dbData);
	int mod = pDC->SetROP2(R2_NOTXORPEN);
	for(int i = 0; i < nCount; i++)
	{	
		CDot dot = m_dotArray.GetAt(i);
		quad.left = dot.x-dbData;
		quad.top = dot.y+dbData;
		quad.right = dot.x+dbData;
		quad.bottom = dot.y-dbData;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		CRect rect = (CRect)quad;
		//pDC->Ellipse(&rect);
		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);
	}
	pDC->SetROP2(mod);
	pDC->SelectObject(pOldPen);
	pView->ReleaseDC(pDC);
}

//
BOOL CStroke::IsOnDot(CPoint &point, CWnd *pWnd)//鼠标是否击中结点的红框
{
	int nCount = m_dotArray.GetSize();
	if ( nCount < 1 || pWnd == NULL )
		return FALSE;

	CQuad quad;
	CZoomView *pView = (CZoomView*)pWnd;
	
	double dbData = 3;
	pView->ActualLen(dbData);
	for( int i = 0; i < nCount; i++ )
	{
		CDot dot   = m_dotArray.GetAt(i);
		quad.left   = dot.x-dbData;
		quad.top   = dot.y+dbData;
		quad.right = dot.x+dbData;
		quad.bottom = dot.y-dbData;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);

		CRect rect = (CRect)quad;
		if ( rect.left <= point.x && point.x <= rect.right && rect.top <= point.y && point.y <= rect.bottom )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CStroke::AddPoint(CPoint &point,CWnd *pWnd)//添加结点
{
	int nCount = m_dotArray.GetSize();
	if ( nCount < 1 || pWnd == NULL )
		return FALSE;

	m_dbR = g_SysCfg.m_dbClick;

	double x1,y1,x2,y2;
	double x,y;

	CQuad quad;
	CDot   dot,mid;
	dot.x = point.x;
	dot.y = point.y;

	CZoomView *pView = (CZoomView*)pWnd;
	pView->LogicToPhysic(dot.x,dot.y);
	pView->ActualLen(m_dbR);

	for ( int i = 0; i < nCount-1; i++ )
	{
		x  = dot.x;
		y  = dot.y;
		x1 = m_dotArray[i].x;
		x2 = m_dotArray[i+1].x;
		y1 = m_dotArray[i].y;
		y2 = m_dotArray[i+1].y;

		double dbap,dbpb,dbab,dx,dy;

		dx = fabs(x1-dot.x);
		dy = fabs(y1-dot.y);
		dbap = sqrt(dx*dx+dy*dy);
		dx = fabs(x2-dot.x);
		dy = fabs(y2-dot.y);
		dbpb = sqrt(dx*dx+dy*dy);
		dx = fabs(x1-x2);
		dy = fabs(y1-y2);
		dbab = sqrt(dx*dx+dy*dy);

		BOOL bLenX = ( dx > dy );//长边是否为X边

		if ( (dbap+dbpb)<(dbab+1.5*m_dbR)){
			CDot AddDot;
			dx = x2-x1;
			dy = y2-y1;
			if (bLenX){
				AddDot.x = dot.x;
				AddDot.y = (y2-y1)*(dot.x-x1)/(x2-x1)+y1;
			}
			else{
				AddDot.y = dot.y;
				AddDot.x = (x2-x1)*(dot.y-y1)/(y2-y1)+x1;
			}
			m_dotArray.InsertAt(i+1,AddDot);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CStroke::DotRight(CDot &dot,CWnd *pWnd)
{
	if ( !pWnd )
		return FALSE;

	CQuad quad;
	CZoomView *pView = (CZoomView*)pWnd;

	double dbData  = 3;
	pView->ActualLen(dbData);
	for( int i = 0;i < m_dotArray.GetSize(); i++ )
	{
		CDot dotIndex = m_dotArray.GetAt(i);
		quad.left   = dotIndex.x-dbData;
		quad.right  = dotIndex.x+dbData;
		quad.top    = dotIndex.y+dbData;
		quad.bottom = dotIndex.y-dbData;

		if ( quad.DotIn(dot) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CStroke::SplitDot(int nBegin,int nEnd,CDot &dt1,CDot &dt2)//只增不删
{
	CStroke *pStroke = new CStroke();
	if ( NULL == pStroke )
		return;

	pStroke->SetProperty(PTY_INIT, m_property);
	pStroke->m_bLockFlag = m_bLockFlag;
	pStroke->m_bTag      =  m_bTag;
	pStroke->SetStyle(GetStyle());

	CNode   *pNode   = (CNode *)GetParent();
	if ( NULL == pNode )
		return;

	if ( dt1 == dt2 )
	{
		pStroke->m_dotArray.Add(dt1);
		for( int i = nBegin; i <= nEnd; i++ )
		{
			pStroke->m_dotArray.Add(m_dotArray[i]);
		}
	}
	else
	{
		pStroke->m_dotArray.Add(dt1);
		for( int i = nBegin; i <= nEnd; i++ )
		{
			pStroke->m_dotArray.Add(m_dotArray[i]);
		}
		pStroke->m_dotArray.Add(dt2);
	}

	pNode->GetList().AddTail(pStroke);
}



// 首尾相连
void CStroke::AddStroke(CStroke *pStroke)
{
	int nCount = pStroke->m_dotArray.GetSize();
	if ( nCount <= 0 )
		return;

	for ( int i = 0; i < nCount; i++ )
	{
		m_dotArray.Add(pStroke->m_dotArray[i]);
	}

	CNode *pNode = (CNode*)m_pParent;
	POSITION pos = pNode->GetList().GetHeadPosition();
	POSITION prev = NULL;
	CStroke *pSearch = NULL;
	while( pos )
	{
		prev = pos;
		pSearch = (CStroke *)pNode->GetList().GetNext(pos);
		if ( pSearch == pStroke )
		{
			pNode->GetList().RemoveAt(prev);
			delete pStroke;
			break;
		}
	}
}

// 两个首节点相连
void CStroke::ConnectHeadToHead(CStroke *pStroke)
{
	if ( NULL == pStroke || m_pParent == NULL )
		return;

	Reverse();
	int nCount = pStroke->m_dotArray.GetSize();
	for( int i = 0; i < nCount; i++ )
	{
		m_dotArray.Add(pStroke->m_dotArray[i]);
	}

	CNode *pNode = (CNode*)m_pParent;
	POSITION pos = pNode->GetList().GetHeadPosition();
	POSITION prev = NULL;
	CStroke *pSearch = NULL;
	while( pos )
	{
		prev = pos;
		pSearch = (CStroke *)pNode->GetList().GetNext(pos);
		if ( pSearch == pStroke )
		{
			pNode->GetList().RemoveAt(prev);
			delete pStroke;
			break;
		}
	}
}

// 两个尾节点相连
void CStroke::ConnectTailToTail(CStroke *pStroke)
{
	if ( NULL == pStroke )
		return;

	pStroke->Reverse();
	int nCount = pStroke->m_dotArray.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		m_dotArray.Add(pStroke->m_dotArray[i]);
	}

	CNode *pNode = (CNode*)m_pParent;
	POSITION pos = pNode->GetList().GetHeadPosition();
	POSITION prev = NULL;
	CStroke *pSearch = NULL;
	while( pos )
	{
		prev = pos;
		pSearch = (CStroke *)pNode->GetList().GetNext(pos);
		if (pSearch == pStroke)
		{
			pNode->GetList().RemoveAt(prev);
			delete pStroke;
			pStroke = NULL;
			break;
		}
	}
}


void CStroke::Draw( XDC *pDC, int nFont, BOOL bSort )
{
	if ( m_dotArray.GetSize() < 1 )
		return;

	if ( nFont == FONT_MATRIX )
	{
		double x, y, r(3);
		int x1,y1,x2,y2;

		CBrush brush;
		brush.CreateSolidBrush(m_property.m_color);
		CBrush* pOldBrush = (CBrush*) pDC->m_pDC->SelectObject(&brush);

		for( int i = 0; i<m_dotArray.GetSize(); i++ )
		{
			x = m_dotArray[i].x;
			y = m_dotArray[i].y;
			pDC->PhysicToLogic(x, y);

			x1 = (int)( x - r*0.5 );
			y1 = (int)( y - r*0.5 );
			x2 = (int)( x + r*0.5 );
			y2 = (int)( y + r*0.5 );
			pDC->m_pDC->Ellipse(x1,y1,x2,y2);
		}

		pDC->m_pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
		return;
	}

	COLORREF drawColor = 0;
	if ( g_bIsSort ) 
	{
		if ( bSort )
		{
			drawColor = m_property.m_color;
		}
		else
		{
			drawColor = RGB(200, 200, 200);
		}
	}
	else
	{
		drawColor = m_property.m_color;
	}

	CPen pen(PS_SOLID, 1, drawColor);
	CPen *pOldPen = pDC->SelectObject(&pen);
	CDot dot = m_dotArray[0];

	double x = dot.x;
	double y = dot.y;

	pDC->MoveTo(x,y);
	for( int i = 1;i < m_dotArray.GetSize(); i++ )	
	{
		dot = m_dotArray[i];
		x   = dot.x;
		y   = dot.y;
		pDC->LineTo(x,y);
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}



int CStroke::GetMarkUnit()
{
	PARA para = g_DAT.GetPara(m_property);
	//
	return para.emcc.nCount;
}


//
void CStroke::MarkPointPreview(CDC *pDC, const int &nPreviewSpeed)
{
	CDot dotTmp;
	for( int i = 0; i < m_dotArray.GetSize(); i++ )
	{
		dotTmp = m_dotArray[i];
		dotTmp.TrueToLog();

		pDC->SetPixelV( (int)dotTmp.x, (int)dotTmp.y, m_property.m_color );
		Sleep(nPreviewSpeed);

		if ( g_sys.m_bStopPreview )
		{
			AfxEndThread(0);
		}
	}
}


void CStroke::FilterExcursion( StrokeList& listData )
{
	//需要对点进行导圆或直角处理，且把反向点删除
	DotExList dotExList;
	for( int i = 0; i < m_dotArray.GetSize(); i++ )
	{
		CDot src = m_dotArray.GetAt(i);
		DOTEX dt;
		DotToDotEx(src,dt);
		dotExList.Add(dt);
	}

	int nCount = 0;
	FilterUnAvlityDot(dotExList,listData,nCount);	
}


//过滤多余的杂点信息，求交点，有交点跌代，无交点退出,
//为了防止出现死机增加跌代次数，最多上限不过100次
void CStroke::FilterUnAvlityDot( DotExList& dotList,StrokeList& listdata,int& nCount )
{
	if ( ++nCount == 100 )	//防死处理
	{
		//将多的点信息转成listdata后返回
		CStroke* pStroke = new CStroke(m_property);
		if ( pStroke == NULL )
			return;

		for(int m = 0;m < dotList.GetSize();m++)
		{
			DOTEX tmp0;
			CDot  dot;

			tmp0 = dotList.GetAt(m);
			DotExToDot(tmp0,dot);
			pStroke->Add(dot);
		}

		listdata.AddTail(pStroke);
		return;
	}

	//计算相交点
	CDot dot1,dot2;		//交点1与交点2，一般情况下只处理交点1即可
	CDot adot,bdot,cdot,ddot;
	int i,j = 0;
	BOOL bFind = FALSE;	//有无交点
	int nSize = dotList.GetSize();

	if ( nSize < 3 )
		return;

	for( i = 0; i < nSize-1; i++ )
	{
		DOTEX tmp = dotList.GetAt(i);
		DotExToDot(tmp,adot);
		tmp = dotList.GetAt(i+1);
		DotExToDot(tmp,bdot);

		for(j = i+2;j<nSize-1;j++)
		{
			DOTEX tmp0;
			UINT rtn;
			UINT ndir1,ndir2;

			tmp0 = dotList.GetAt(j);
			DotExToDot(tmp0,cdot);
			tmp0 = dotList.GetAt(j+1);
			DotExToDot(tmp0,ddot);
			rtn = CalCrossEx(adot,bdot,cdot,ddot,dot1,dot2,ndir1,ndir2);

			if ((ndir1 == 1)&&(ndir2 == 1)&&(rtn == 1)&&(!(adot == dot1)))
			{
				bFind = TRUE;
				break;
			}
		}

		if ( bFind )
			break;
	}

	if ( bFind )
	{
		//分解成两个DotExList进行下一步跌代
		DotExList dotList1,dotList2;
		DOTEX   tmp,dtmp;

		DotToDotEx(dot1,tmp);
		//组建DOTLIST1
		int m = 0;
		for( m = 0;m < i+1; m++)
		{
			dtmp = dotList.GetAt(m);
			dotList1.Add(dtmp);
		}
		dotList1.Add(tmp);
		for( m = j+1; m < nSize; m++ )
		{
			dtmp = dotList.GetAt(m);
			dotList1.Add(dtmp);
		}

		//组建DOTLIST2
		dotList2.Add(tmp);
		for( m = i+1; m <j +1; m++ )
		{
			dtmp = dotList.GetAt(m);
			dotList2.Add(dtmp);
		}
		dotList2.Add(tmp);

		//修正dotList进入下一步跌代
		FilterUnAvlityDot(dotList1,listdata,nCount);
		FilterUnAvlityDot(dotList2,listdata,nCount);
	}
	else{
		//无交点，全为有效数据时才进行转换处理
		//有效数据才加点到listdata中
		CStroke* pStroke =  new CStroke(m_property);
		if ( pStroke == NULL )
			return;

		nSize = dotList.GetSize();
		if ( nSize > 1 )
		{
			for( int m = 0; m < nSize; m++ )
			{
				CDot dot;
				DOTEX tmp0 = dotList.GetAt(m);
				DotExToDot(tmp0,dot);
				pStroke->Add(dot);
			}
			listdata.AddTail(pStroke);
		}
		else
		{
			delete pStroke;
		}
	}
}

CStroke& CStroke::operator  = (const CStroke &stroke)
{
	CopyStroke(&stroke);

	return *this;
}


CStroke& CStroke::operator  = (const CStrokeB &stroke)
{
	CopyStrokeB(&stroke);

	return *this;
}

//Stroke内找一线条的中点做一水平线做扫描线，找扫pListData来判断是否为外轮廓
BOOL CStroke::ISOutLineStroke(StrokeList *plistData)
{
	if ( !IsRegion() )//找水平线的中点
	{
		OutputDebugString(_T("\nCstroke_ISOutLineStroke Err:Stroke not closed "));
		return FALSE;
	}
	
	BOOL bFind = FALSE;
	CDot aDot,bDot;//线段点
	CDot cDot; //线段中间

	int i = 0;
	for ( ; i < m_dotArray.GetSize()-1 ;i++ )
	{
		aDot = m_dotArray.GetAt(i);
		bDot = m_dotArray.GetAt(i+1);
		double angle = CalLineAngle(aDot,bDot);
		if ( !(SAMEVALUE(angle,PAI) || SAMEVALUE(angle,0)) )
		{
			bFind = TRUE;
			break;
		}
	}
	if ( !bFind )
	{
		OutputDebugString(_T("\nCStroke_ISOutLineStroke Err:Can't Find  horizonal line"));
		return FALSE;
	}
	cDot.x = (aDot.x+bDot.x)/2; //
	cDot.y = (aDot.y+bDot.y)/2;

	CArray<HotDot,HotDot> arrayCross;
	HotDot tmpHotDot;

	CStroke* pStroke = NULL;	
	POSITION pos = plistData->GetHeadPosition();

	CDot   dDot,eDot; //计算交点线段
	CDot   dHotDot;   //怀疑点(边界上的点)
	double dbHotDotDir;//(边界点前一点方向)
	int      nDotSize = 0;
	BOOL   bEndPointCross = FALSE; //是否交点在线条的未点
	i = -1;
	while (pos != NULL)
	{
		pStroke = (CStroke *)plistData->GetNext(pos);
		i++;
		nDotSize = pStroke->GetSize();
		bEndPointCross = FALSE;
		int j = 0;
		for (j = 0;j<nDotSize-1;j++) //循环求交点
		{
			dDot = pStroke->GetAt(j);
			eDot = pStroke->GetAt(j+1);
			while (dDot == eDot)
			{
				j++;
				if ( j >= nDotSize-1 )
				{
					break;
				}
				eDot = pStroke->GetAt(j+1);
			}
			if ( j >= nDotSize-1 )
			{
				break;
			}
			//计算与水平线y = cDot.y的交点
			if ( ( (dDot.y-cDot.y)*(eDot.y-cDot.y) <= 0 ) || bEndPointCross || SAMEVALUE(cDot.y,eDot.y) )
			{
				if ( SAMEVALUE(cDot.y,eDot.y) )
				{
					if ( !bEndPointCross )
					{
						dHotDot = eDot;//记下边界点
						dbHotDotDir = dDot.y;//记下边界点前一点的X值
						bEndPointCross = TRUE;
					}					
				}
				else //未点未在信息点处
				{
					if (bEndPointCross) //上一次在未点上
					{
						//判断方向是否同侧
						if ((dbHotDotDir-dHotDot.y)*(eDot.y-dHotDot.y)<0)
						{
							//不在同侧为有效点
							tmpHotDot.m_CrossDot = dHotDot;
							tmpHotDot.m_iStrokeIndex = i;
							arrayCross.Add(tmpHotDot);
						}
					}
					else //上一次未在信息点处
					{
						dHotDot.x = eDot.x-(eDot.x-dDot.x)*(eDot.y-cDot.y)/(eDot.y-dDot.y);
						dHotDot.y = cDot.y;
						tmpHotDot.m_CrossDot = dHotDot;
						tmpHotDot.m_iStrokeIndex = i;
						arrayCross.Add(tmpHotDot);
					}
					bEndPointCross = FALSE;
				}
			}
		}
		if ( bEndPointCross && arrayCross.GetSize()%2 == 1 )
		{
			dDot = pStroke->GetAt( (j >= nDotSize-1) ? j-(nDotSize-1) : j);
			eDot = pStroke->GetAt( (j+1 >= nDotSize-1) ? (j+1)-(nDotSize-1):(j+1));
			while (dDot == eDot)
			{
				j++;
				eDot = pStroke->GetAt((j+1 >= nDotSize-1)?(j+1)-(nDotSize-1):(j+1));
			}
			//判断方向是否同侧
			if ((dbHotDotDir-dHotDot.y)*(eDot.y-dHotDot.y) <0)
			{
				//不在同侧为有效点
				tmpHotDot.m_CrossDot = dHotDot;
				tmpHotDot.m_iStrokeIndex = i;
				arrayCross.Add(tmpHotDot);
			}
		}
	}
	//arrayCross:线条与y = cDot.y的所有交点
	//cDot:本曲线交点;
	//判断是否为外轮廓
	//a.先对arrayCross从小到大排序
	int size = arrayCross.GetSize();
	CDot fDot;
	int jindex;
	HotDot tmpHotDot2;
	for ( i = 0; i < size-1; i++ )
	{
		tmpHotDot = arrayCross.GetAt(i);
		jindex = i;
		for (int j = i+1;j<size;j++)
		{
			tmpHotDot2 = arrayCross.GetAt(j);
			if (tmpHotDot2.m_CrossDot.x<tmpHotDot.m_CrossDot.x)
			{	
				//switch
				tmpHotDot = tmpHotDot2;
				jindex = j;
			}			
		}
		if ( i != jindex )
		{
			arrayCross.SetAt(jindex,arrayCross.GetAt(i));
			arrayCross.SetAt(i,tmpHotDot);
		}
	}
	//b.找出cDot在arrayCross中的排序
	int nOutLine = -1;
	jindex = -1;
	for ( i = 0; i < size; i++ )
	{
		dDot = arrayCross.GetAt(i).m_CrossDot;
		{
			if ( dDot == cDot )
			{
				nOutLine = arrayCross.GetAt(i).m_iStrokeIndex;
				jindex = i;
				break;
			}
		}
	}
	if ( jindex == -1 )
	{
		return FALSE; //基本不可能，仅为容错
	}

	//c.判断(注意索引基于0)
	tmpHotDot = arrayCross.GetAt(0);
	tmpHotDot.m_bOutLine = TRUE;
	arrayCross.SetAt(0,tmpHotDot);

	HotDot ehdot;
	HotDot tmp;
	tmp.m_bOutLine = TRUE;
	CArray<HotDot, HotDot> arrayStack;
	for ( i = 0; i < size; i++ )
	{
		ehdot = arrayCross.GetAt(i);
		int size = arrayStack.GetSize();
		if ( size == 0 )
		{
			arrayStack.Add(ehdot);
		}
		else
		{
			HotDot tmpHDot = arrayStack.GetAt(size-1);
			if (ehdot.m_iStrokeIndex  !=  tmpHDot.m_iStrokeIndex)
			{
				arrayStack.Add(ehdot);
			}
			else
			{
				arrayStack.RemoveAt(size-1);
				tmpHDot.m_bOutLine = ( arrayStack.GetSize()%2 == 0 ) ? TRUE:FALSE;
				if ( tmpHDot.m_iStrokeIndex == nOutLine ) 
				{
					tmp.m_bOutLine = tmpHDot.m_bOutLine;
					break;
				}
			}
		}
	}

	return tmp.m_bOutLine;
} 


// 设置指定点位该曲线的起点
void CStroke::SetDotToStart(CDot &dt)
{
	int nCount = m_dotArray.GetSize();
	if ( nCount < 2 )
		return;

	CDot sDot = m_dotArray[0];
	CDot eDot = m_dotArray[nCount - 1];
	if ( sDot == eDot )
	{
		CArray<CDot,CDot> dotArrayTemp;
		dotArrayTemp.RemoveAll();

		for (int i = 0; i < nCount; i++)
		{
			CDot dtTemp = m_dotArray[i];
			if ( !(dt == dtTemp) )// 找指定点
			{
				dotArrayTemp.Add(dtTemp);
			}
			else
			{
				dotArrayTemp.Add(dt);
				CArray<CDot,CDot> dotArray;
				dotArray.RemoveAll();
				for (int j = i; j < (nCount-1); j++)
				{
					CDot Dot = m_dotArray[j];
					dotArray.Add(Dot);
				}
				m_dotArray.RemoveAll();
				m_dotArray.Append(dotArray);
				m_dotArray.Append(dotArrayTemp);
				dotArray.RemoveAll();
				dotArrayTemp.RemoveAll();
				return;
			}
		}// 
		dotArrayTemp.RemoveAll();
	}// end if
	else
	{
		AfxMessageBox(IDS_THELINEISNOTCIRCL);
	}
}


CStrokeB * CStroke::Copy()
{
	CStroke *pTmp = new CStroke(m_property);
	if ( NULL == pTmp )
		return NULL;

	pTmp->SetParent((LPVOID*)GetParent());
	pTmp->m_bCharEnd = m_bCharEnd;
	pTmp->m_dotArray.Append(m_dotArray);

	return pTmp;
}


CQuad CStroke::CalLimit()
{
	int nCount = m_dotArray.GetSize();
	if ( nCount  <=  0 )
	{
		m_quadLimit.Initial();
	}
	else
	{
		m_quadLimit.SetEmpty();
		CDot dot = m_dotArray[0];
		m_quadLimit.left = m_quadLimit.right = dot.x;
		m_quadLimit.top  = m_quadLimit.bottom = dot.y;
		m_quadLimit.zmin = m_quadLimit.zmax = dot.z;
		for( int  i = 1; i < nCount; i++ )
		{
			dot = m_dotArray[i];
			m_quadLimit.left  = min(m_quadLimit.left,dot.x);
			m_quadLimit.right = max(m_quadLimit.right,dot.x);
			m_quadLimit.top   = max(m_quadLimit.top,dot.y);
			m_quadLimit.bottom = min(m_quadLimit.bottom,dot.y);
			m_quadLimit.zmin = min(m_quadLimit.zmin, dot.z);
			m_quadLimit.zmax = max(m_quadLimit.zmax, dot.z);
		}
	}

	return m_quadLimit;
}


CQuad CStroke::CalLimit(CDot dot,double dbXScale,double dbYScale)
{
	if ( m_dotArray.GetSize() <= 0 )
	{
		m_quadLimit.SetEmpty();
	}
	else
	{
		CDot dotNew = m_dotArray[0];
		dotNew.Scale(dot,dbXScale,dbYScale);
		m_quadLimit = CQuad(dotNew.x,dotNew.y,dotNew.x,dotNew.y);
		for( int i = 1; i < m_dotArray.GetSize(); i++ )
		{
			dotNew = m_dotArray[i];
			dotNew.Scale(dot,dbXScale,dbYScale);
			m_quadLimit.left	 = min(m_quadLimit.left	, dotNew.x);
			m_quadLimit.right	 = max(m_quadLimit.right	, dotNew.x);
			m_quadLimit.top		 = max(m_quadLimit.top	, dotNew.y);
			m_quadLimit.bottom	 = min(m_quadLimit.bottom	, dotNew.y);
			m_quadLimit.zmin = min(m_quadLimit.zmin, dot.z);
			m_quadLimit.zmax = max(m_quadLimit.zmax, dot.z);
		}
	}
	return m_quadLimit;
}

void CStroke::CopyStroke(const CStroke *pStroke)
{
	if ( this == pStroke )
		return;

	m_dotArray.RemoveAll();
	m_dotArray.Append(pStroke->m_dotArray);

	m_property = pStroke->m_property;
	m_bCharEnd = pStroke->m_bCharEnd;
	CalLimit();
}

//
void CStroke::CopyStrokeB( const CStrokeB *pStroke )
{
	if ( this == pStroke )
		return;

	m_dotArray.RemoveAll();
	m_dotArray.Append(pStroke->m_dotArray);

	m_property = pStroke->m_property;
	m_bCharEnd = FALSE;
	CalLimit();
}




