#pragma once

// CFillMaker 命令目标
#include "StrokeB.h"
#include "Dot.h"
#include "Property.h"
#include "DataMng.h"
#include "ExternStroke.h"


class AFX_EXT_CLASS CFillMaker
{
public:
	CFillMaker();
	virtual ~CFillMaker();
	
	//
	CDataMng m_dataMng;
	BaseHatch m_baseHatch;
	
	BOOL m_bClose;  //曲线是否闭合

	//
	BOOL MakeFill( const StrokeList &list_In, StrokeList &list_out, BaseHatch baseHatch, BOOL bFirstContour , BOOL bOpt=FALSE);
	
	//
	int ScaleProc( const StrokeList &listData, StrokeList &listContour, StrokeList &listActiveContour, double dbScale, BOOL bOpt, BOOL bBeamComp );
	BOOL ISOutLineStroke( CStrokeB *pStrokeData,  const StrokeList &listData );
	void FilterExcursion(CStrokeB *pStroke, StrokeList& listData);
	void FilterUnAvlityDot(CStrokeB *pStrokeData, DotExList& dotlist, StrokeList& listdata, int& nCount);
	
	int FillContour( StrokeList &listData, StrokeList &listFill, double dbSpace, double dbAngle );
};


