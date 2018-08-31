

#include "stdafx.h"
#include "hl.h"
#include "Fill.h"
#include "FillMaker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFill::CFill(double space,double dbBorderSpace,double dbAngle,
			 UINT style,CProperty prty,double dbPtp,double dbCircle,double dbHeadKillLen,double dbTailKillLen)
{
	m_dbSpace = space;     //线间距
	m_dbBorderSpace = dbBorderSpace;	//边距
	m_nStyle = style;          //类型
	m_dbAngle = dbAngle;    //角度
	m_property = prty;		//层参数
	m_dbPtp = dbPtp;		//光栅点间距
    m_dbCircle = dbCircle;  //绕线间距
	m_dbHeadKillLen = -dbHeadKillLen;   //首点缩进
	m_dbTailKillLen = -dbTailKillLen;		//尾点缩进
}

//
CFill::CFill(NEW_FILL_DATA newFillData, CProperty prty)
{
	m_newFillData = newFillData;
	m_property = prty;
}


CFill::~CFill()
{
}

CQuad CFill::CalLimit()
{
	m_quadLimit.SetEmpty();

	CStroke *pStroke = NULL;
	POSITION pos = m_plist->GetHeadPosition();
	while(pos  !=  NULL)
	{
		pStroke = (CStroke*)m_plist->GetNext(pos);
		if ( !pStroke )
			continue;

		CQuad quad = pStroke->CalLimit();
		m_quadLimit.Union(quad);
	}
	m_quadLimit.Normal();
	
	return m_quadLimit;
}

//plistData:线条数据，不产生实体只是从TREE中扫出所有的线条指针
//listFillData:填充后的数据用于输出，在此函数中只会往里加数据
void CFill::Proc(StrokeList *plistData,StrokeList &listFillData)
{
	StrokeList listDataIn;
	listDataIn.AddTail(plistData);
	
	CFillMaker fillMaker;
	fillMaker.MakeFill(listDataIn, listFillData, m_newFillData, FALSE, g_SysCfg.m_bFillOptimize);
}


//
void CFill::Proc(StrokeList *plistData,StrokeList &listFillData, double dbFillAngle, BOOL bFillFirst)
{	
	if ( !plistData )
		return;

	StrokeList listDataIn;
	listDataIn.AddTail(plistData);
	
	double angle = 0;
	if (m_newFillData.bKeepAngle)
	{
		angle = m_newFillData.dbAngle + dbFillAngle;
	}
	else
	{
		angle = m_newFillData.dbAngle;
	}
	if ( fabs(angle) > 0.001 )
		Rotate(&listDataIn,CDot(0,0),-angle);
	
	CFillMaker fillMaker;
	fillMaker.MakeFill(listDataIn, listFillData, m_newFillData, !bFillFirst, g_SysCfg.m_bFillOptimize);
	//
	if ( fabs(angle) > 0.001 )
	{
		Rotate(&listDataIn,CDot(0,0),angle);
		Rotate(&listFillData,CDot(0,0),angle);
	}
}

//
void CFill::CirclProc(StrokeList *pList, StrokeList *pRetList, double dbLen)
{
	if ( !pList || !pRetList || dbLen <= 0  )
		return;

	CStrokeB *pStrokeTmp = NULL;
	CDot dotS,dotE;
	
	BOOL bFirst = TRUE;
	StrokeList strList;
	
	POSITION pos = pList->GetHeadPosition();
	while( pos  !=  NULL )
	{
		CStroke *pStroke = (CStroke *)pList->GetNext(pos);
		if ( bFirst )
		{
			pStrokeTmp  =  pStroke->Copy();
			strList.AddTail(pStrokeTmp);
			bFirst = FALSE;
			int nCount = pStrokeTmp->GetSize();
			if ( nCount > 0 ) 
			{
				dotE = pStrokeTmp->GetAt(nCount-1);
			}
			continue;
		}
		
		int nCount = pStroke->GetSize();
		if ( nCount > 0 )
		{
			dotS = pStroke->GetAt(0);
			double dbDist = Distance(dotE, dotS);
			if ( fabs(dbDist)  <=  dbLen ) 
			{
				for( int i = 0; i < nCount; i ++ )
				{
					pStrokeTmp->Add(pStroke->m_dotArray.GetAt(i));
				}
				dotE = pStroke->m_dotArray.GetAt(nCount - 1);
			}
			else
			{
				pStrokeTmp  =  pStroke->Copy();
				strList.AddTail(pStrokeTmp);
				int nCount = pStrokeTmp->GetSize();
				if ( nCount > 0 )
				{
					dotE = pStrokeTmp->GetAt(nCount-1);
				}
			}
		} // if (nCount > 0)
	}
	
	DeletStrokeList(pList);
	
	POSITION posl = strList.GetHeadPosition();
	while( posl  !=  NULL )
	{
		CStrokeB *pStroke = (CStrokeB *)strList.GetNext(posl);
		if ( pStroke )
			pRetList->AddTail(pStroke);
	}
	
	strList.RemoveAll();
}

//
void CFill::CirclLinePro( CArray<StrokeList*, StrokeList*> &RetList, StrokeList& listFillData )
{
	if ( m_nStyle & FILL_CIRCLE )
	{
		int nlistCount = RetList.GetSize();
		for( int i = 0; i < nlistCount; i++ )
		{
			StrokeList* pStrokeList = RetList.GetAt(i);
			if ( NULL == pStrokeList ) 
			{
				continue;
			}
			
			CirclProc(pStrokeList, &listFillData, m_dbCircle);
			DeletStrokeList(pStrokeList);
			pStrokeList->RemoveAll();
			delete pStrokeList;
			pStrokeList = NULL;
		}
	}
	else
	{
		int nlistCount = RetList.GetSize();
		for( int i = 0; i < nlistCount; i++ )
		{
			StrokeList* pStrokeList = RetList.GetAt(i);
			if ( NULL == pStrokeList )
			{
				continue;
			}
			
			POSITION posl = pStrokeList->GetHeadPosition();
			while( posl  !=  NULL )
			{
				CStrokeB *pStroke = (CStrokeB*)pStrokeList->GetNext(posl);
				if ( pStroke )
					listFillData.AddTail(pStroke);
			}
			
			pStrokeList->RemoveAll();
			delete pStrokeList;
			pStrokeList = NULL;
		}
	}
}
