#include "stdafx.h"
#include <math.h>
#include "AIEntity.h"


int CCurve::GetType()
{
	return m_nCurveType;
}

void CCurve::GetArrayDot(CArray<DBPT,DBPT>& dtArray)
{
	dtArray.RemoveAll();
	dtArray.Copy(m_dtArray);
}

CCurve_Line::CCurve_Line()
{ 
	m_nCurveType = AI_LINE; 
	m_pt0.x = m_pt0.y = m_pt1.x = m_pt1.y = 0;
}

CCurve_Line::CCurve_Line(DBPT& pt0,DBPT& pt1)
{
	m_nCurveType = AI_LINE; 
	m_pt0 = pt0;
	m_pt1 = pt1;
}

CCurve_Line::CCurve_Line(double x0,double y0, double x1, double y1)
{
	m_nCurveType = AI_IMAGE;
	m_pt0.x = x0; 
	m_pt0.y = y0;
	m_pt1.x = x1;
	m_pt1.y = y1;
}

void  CCurve_Line::CreateCurve(double dbPrecision)
{
	m_dtArray.RemoveAll();
	m_dtArray.Add(m_pt0);
	m_dtArray.Add(m_pt1);
}

void CCurve_Line::operator=(const CCurve_Line& src)
{
	m_pt0 = src.m_pt0;
	m_pt1 = src.m_pt1;
	CreateCurve();
}


CCurve_Bezier::CCurve_Bezier()
{
	m_nCurveType = AI_BEZIER;
}

// 四个点的bezier曲线
CCurve_Bezier::CCurve_Bezier(DBPT* pt)
{
	m_nCurveType = AI_BEZIER;
	m_vecCtrlPt[0] = pt[0];
	m_vecCtrlPt[1] = pt[1];
	m_vecCtrlPt[2] = pt[2];
	m_vecCtrlPt[3] = pt[3];
}

CCurve_Bezier::~CCurve_Bezier()
{
}

void CCurve_Bezier::operator=(const CCurve_Bezier& src)
{
	for(int i = 0; i<4; i++)
		m_vecCtrlPt[i] = src.m_vecCtrlPt[i];

	CreateCurve();
}

void CCurve_Bezier::SetCtrlPoint(DBPT& pt,int index)
{
	if(index <0 || index > 3)
		return;

	m_vecCtrlPt[index] = pt;
}

void CCurve_Bezier::CreateCurve(double dbPrecision)
{
	// t的增量， 可以通过setp大小确定需要保存的曲线上点的个数
	m_dtArray.RemoveAll();
	m_dtArray.Add(m_vecCtrlPt[0]);
	double dX = (m_vecCtrlPt[3].x-m_vecCtrlPt[0].x);
	double dY = (m_vecCtrlPt[3].y-m_vecCtrlPt[0].y);
	double dbLen = sqrt( dX*dX + dY*dY );
	int n = int(dbLen/dbPrecision+0.5);
	if( n > 1 )
	{
		float step = 1.0/n;
		for (float t = step; t < 1.0; t += step)
		{
			DBPT stPt;
			CalCurvePoint(t, stPt);
			m_dtArray.Add(stPt);
		}
	}
	m_dtArray.Add(m_vecCtrlPt[3]);
}

void CCurve_Bezier::ClearCtrlPoint()
{
	m_dtArray.RemoveAll();
}

void CCurve_Bezier::CalCurvePoint(float t, DBPT& stPt)
{
	float x = (float)m_vecCtrlPt[0].x * pow(1 - t, 3)  + 
		(float)m_vecCtrlPt[1].x * t * pow(1 - t,2) * 3 + 
		(float)m_vecCtrlPt[2].x * pow(t, 2) * (1-t) * 3 +
		(float)m_vecCtrlPt[3].x * pow(t,3);
	float y = (float)m_vecCtrlPt[0].y * pow(1 - t, 3)  + 
		(float)m_vecCtrlPt[1].y * t * pow(1 - t,2) * 3 + 
		(float)m_vecCtrlPt[2].y * pow(t, 2) * (1-t) * 3 +
		(float)m_vecCtrlPt[3].y * pow(t,3);

	stPt.x =x;
	stPt.y= y;
}


CAIPath::~CAIPath()
{
	Delete();
}

int CAIPath::GetCount()
{
	return m_curveList.GetCount();
}


void CAIPath::Add(CCurve* pCurve)
{
	if( NULL == pCurve )
		return;

	CCurve* p = NULL;
	POSITION pos = m_curveList.GetHeadPosition();
	while( NULL != pos )
	{
		p = (CCurve*)m_curveList.GetNext(pos);
		if( p == pCurve)
			return;
	}
	m_curveList.AddTail(pCurve);
}

void CAIPath::GetArrayDot(CArray<DBPT,DBPT>& dtArray)
{
	dtArray.RemoveAll();
	CArray<DBPT,DBPT> dtTmpArray;
	CCurve* p = NULL;
	BOOL bFirst = TRUE;
	POSITION pos = m_curveList.GetHeadPosition();
	while( NULL != pos )
	{
		p = (CCurve*)m_curveList.GetNext(pos);
		if( p )
		{
			dtTmpArray.RemoveAll();
			p->GetArrayDot(dtTmpArray);

			if( bFirst )
			{
				dtArray.Copy(dtTmpArray);
				bFirst = FALSE;
			}
			else
			{
				for(int j =1; j<dtTmpArray.GetCount(); j++)
					dtArray.Add(dtTmpArray[j]);
			}
		}
	}

}

void CAIPath::Delete()
{
	CCurve* p = NULL;
	POSITION pos = m_curveList.GetHeadPosition();
	while( NULL != pos )
	{
		p = (CCurve*)m_curveList.GetNext(pos);
		if( p )
		{	
			delete p;
			p = NULL;
		}
	}

	m_curveList.RemoveAll();
}




CAILayer::~CAILayer()
{
	Delete();
}

int CAILayer::GetCount()
{
	return m_pathList.GetCount();
}

AIPathList &CAILayer::GetAIPathList()
{
	return m_pathList;
}

void CAILayer::Add(CAIPath* pPath)
{
	if( NULL == pPath || pPath->GetCount() <= 0)
		return;

	CAIPath* p = NULL;
	POSITION pos = m_pathList.GetHeadPosition();
	while( pos != NULL )
	{
		p = (CAIPath*)m_pathList.GetNext(pos);
		if( p == pPath )
			return;
	}
	m_pathList.AddTail(pPath);
}


void CAILayer::Delete()
{
	CAIPath* p = NULL;
	POSITION pos = m_pathList.GetHeadPosition();
	while( pos != NULL )
	{
		p = (CAIPath*)m_pathList.GetNext(pos);
		if( p )
		{
			delete p;
			p = NULL;
		}
	}
	m_pathList.RemoveAll();
}


CAIFile::~CAIFile()
{
	Delete();
}

int CAIFile::GetCount()
{
	return m_layerLists.GetCount();
}

void CAIFile::Add(CAILayer* pLayer)
{
	if( NULL == pLayer /*|| pLayer->GetCount() <= 0 */)
		return;

	CAILayer* p = NULL;
	POSITION pos = NULL;
	while( NULL != pos )
	{
		p = (CAILayer*)m_layerLists.GetNext(pos);
		if( p == pLayer )
			return;
	}
	m_layerLists.AddTail(pLayer);
}

AILayerList &CAIFile::GetAILayerList()
{
	return m_layerLists;
}

void CAIFile::Delete()
{
	CAILayer* p = NULL;
	POSITION pos = NULL;
	while( NULL != pos )
	{
		p = (CAILayer*)m_layerLists.GetNext(pos);
		if( p  )
		{
			delete p;
			p = NULL;
		}
	}

	m_layerLists.RemoveAll();
}
