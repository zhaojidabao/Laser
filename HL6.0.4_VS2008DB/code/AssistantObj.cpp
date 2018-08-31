#include "stdafx.h"
#include "AssistantObj.h"


CAssistantObj::CAssistantObj()
{
	m_nGridHeight = 210;
	m_nGridWidth = 210;
	m_pGridData = NULL;

	m_nPrecision = 1;
	m_anchorPoint.bEnable = FALSE;

	pen.CreatePen(PS_SOLID,2,RGB(0,0,255));
}

CAssistantObj::~CAssistantObj()
{
	pen.DeleteObject();
	FreeAssistant();
}


BOOL CAssistantObj::InitAssistant()
{
	m_pGridData = new unsigned int*[m_nGridHeight];
	if ( NULL == m_pGridData )
		return FALSE;

	for(unsigned int i = 0; i<m_nGridHeight; i++)
	{
		m_pGridData[i] = new unsigned int[m_nGridWidth];
		if ( NULL == m_pGridData[i] )
			return FALSE;
	}

	for(unsigned int i = 0; i<m_nGridHeight; i++)
	{
		for(unsigned int j = 0; j<m_nGridWidth; j++)
			m_pGridData[i][j] = 0;
	}

	return TRUE;
}

/********************************************************************
作用：扫描线条并添加铺助点

********************************************************************/
BOOL CAssistantObj::LoadAssistantPoints(const CArray<DOT,DOT> &pointArray)
{
	if ( NULL == m_pGridData )
		return FALSE;

	for(unsigned int i = 0; i<m_nGridHeight; i++)
		for(unsigned int j = 0; j<m_nGridWidth; j++)
		{
			if ( m_pGridData[i][j]  !=  0 )
			{
				delete (ASSPoint*)m_pGridData[i][j];
				m_pGridData[i][j] = 0;
			}
		}

		DOT dot;
		for(int i = 0; i<pointArray.GetCount(); i++)
		{
			dot = pointArray.GetAt(i);
			SetPoint(dot.x,dot.y, int(dot.z));
		}
		return TRUE;
}



void CAssistantObj::FreeAssistant()
{
	if ( NULL == m_pGridData )
		return;

	for( UINT i = 0; i<m_nGridHeight; i++)
	{
		for( UINT j = 0; j<m_nGridWidth; j++)
		{
			if ( m_pGridData[i][j]  !=  0 )
			{
				delete (ASSPoint*)m_pGridData[i][j];
				m_pGridData[i][j] = 0;
			}
		}
	}

	for( UINT i = 0; i<m_nGridHeight; i++)
	{
		if ( m_pGridData[i]  !=  NULL )
		{
			delete[] m_pGridData[i];
			m_pGridData[i] = 0;
		}
	}

	delete[] m_pGridData;
	m_pGridData = NULL;
}

void CAssistantObj::DrawAnchor(CDC* pDC)
{
	if ( !pDC )
		return;

	if ( !m_anchorPoint.bEnable )
		return;

	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_NOTXORPEN);
	switch( m_anchorPoint.nType )
	{
	case ASSPOINT_TYPE_ENDPOINT:		// 端点
		pDC->Rectangle(m_anchorPoint.x - 5, m_anchorPoint.y-5, m_anchorPoint.x + 5, m_anchorPoint.y + 5);
		break;
	case ASSPOINT_TYPE_LINECENTER:		// 中心
		{
			pDC->MoveTo(m_anchorPoint.x, m_anchorPoint.y-7);
			pDC->LineTo(m_anchorPoint.x-7, m_anchorPoint.y);
			pDC->LineTo(m_anchorPoint.x, m_anchorPoint.y+7);
			pDC->LineTo(m_anchorPoint.x+7, m_anchorPoint.y);
			pDC->LineTo(m_anchorPoint.x, m_anchorPoint.y-7);
			break;
		}
	case ASSPOINT_TYPE_CIRCLECENTER:	// 圆心
		pDC->Ellipse(m_anchorPoint.x - 6, m_anchorPoint.y-6, m_anchorPoint.x + 6, m_anchorPoint.y + 6);
		break;
	case ASSPOINT_TYPE_CUTPOINT:		// 切线
		{
			pDC->MoveTo(m_anchorPoint.x-4, m_anchorPoint.y+4);
			pDC->LineTo(m_anchorPoint.x+4, m_anchorPoint.y-4);
			pDC->MoveTo(m_anchorPoint.x+4, m_anchorPoint.y+4);
			pDC->LineTo(m_anchorPoint.x-4, m_anchorPoint.y-4);
			break;
		}
	default:
		break;
	}
	pDC->SelectObject(pOldPen);
}


void CAssistantObj::SetPoint(double x, double y, int nType)
{
	int nx = Round(x)+m_nGridWidth/2;
	int ny = Round(y)+m_nGridHeight/2;
	if ( nx < 0 || nx  >=  m_nGridHeight || ny < 0 || ny  >=  m_nGridWidth )
		return;

	if ( m_pGridData[nx][ny]  !=  0 )
	{
		ASSPoint* pPoint = (ASSPoint*)m_pGridData[nx][ny];
		pPoint->x = x;
		pPoint->y = y;
		pPoint->nType = nType;
	}
	else
	{
		ASSPoint* pPoint = new ASSPoint();
		pPoint->x = x;
		pPoint->y = y;
		pPoint->nType = nType;
		m_pGridData[nx][ny] = (unsigned int)pPoint;
	}
}

void   CAssistantObj::SetPointsFromStroke(CStroke* pStroke)
{
	if ( NULL == pStroke )
		return;

	CDot dot1,dot2;
	double xCenter,yCenter;
	dot1 = pStroke->GetAt(0);
	SetPoint(dot1.x, dot1.y, ASSPOINT_TYPE_ENDPOINT);
	for (int i = 1; i<pStroke->GetSize(); i++ )
	{
		dot2 = pStroke->GetAt(i);
		SetPoint(dot2.x, dot2.y, ASSPOINT_TYPE_ENDPOINT);

		xCenter = (dot1.x + dot2.x)/2;
		yCenter = (dot1.y + dot2.y)/2;

		SetPoint(xCenter, yCenter, ASSPOINT_TYPE_LINECENTER);
		dot1 = dot2;
	}
}


//搜索精度范围，看是否存在铺助点
BOOL CAssistantObj::LookUp(double x, double y, double& dbx, double& dby)
{
	m_anchorPoint.bEnable = FALSE;
	int nx = Round(x)+m_nGridWidth/2;
	int ny = Round(y)+m_nGridHeight/2;

	if ( nx <0 || nx  >=  m_nGridWidth || ny < 0  || ny  >=  m_nGridHeight  )
		return FALSE;

	int tmpX, tmpY;
	for(int i = -m_nPrecision; i<m_nPrecision; i++)
		for(int j = -m_nPrecision; j<m_nPrecision; j++)
		{
			if ( fabs((float)i) + fabs((float)j) > m_nPrecision )
				continue;

			tmpX = nx + i;
			tmpY = ny + j;
			if ( tmpX < 0 || tmpX  >=  m_nGridWidth || tmpX < 0  || tmpY  >=  m_nGridHeight  )
				continue;

			if ( m_pGridData[tmpX][tmpY] == 0 )
				continue;

			ASSPoint* pPoint = (ASSPoint*)m_pGridData[tmpX][tmpY];
			if ( pPoint == NULL )
				continue;

			dbx = pPoint->x;
			dby = pPoint->y;

			m_anchorPoint.nType = pPoint->nType;
			m_anchorPoint.bEnable = TRUE;
			return TRUE;
		}

		return FALSE;
}


BOOL CAssistantObj::SetAssistantPara(UINT nGridWidth, UINT nGridHeight, int nPrecision)
{
	FreeAssistant();
	m_nGridWidth = nGridWidth;
	m_nGridHeight = nGridHeight;
	m_nPrecision = nPrecision;
	if ( nPrecision <1 || nPrecision > 5 )
		nPrecision = 1;

	return InitAssistant();
}