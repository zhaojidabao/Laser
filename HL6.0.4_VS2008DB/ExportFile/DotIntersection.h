// DotIntersection.h: interface for the PointOfIntersection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTINTERSECTION_H__64F7C7D8_D865_4BCB_8C48_8822BC6A2F2E__INCLUDED_)
#define AFX_DOTINTERSECTION_H__64F7C7D8_D865_4BCB_8C48_8822BC6A2F2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct Inter_Dot
{
	double x;
	double y; 
	BOOL bFilterExtreme;//该点和端点是否重合
	BOOL bLineEnd;		//是否线段终点
	Inter_Dot()
	{
		::ZeroMemory(this,sizeof(Inter_Dot));
	};
}InterDot;

typedef struct 
{
	InterDot pt1;
	InterDot pt2;
}LINE;

class CDotIntersection   
{
public:
	CDotIntersection();
	virtual ~CDotIntersection();

public:
	BOOL   GetInterPoint(LINE,LINE,double,InterDot& );
	double GetLineAngle(LINE lineTmp);
	void    SetInterSpace( double dbSpace ){
		if ( dbSpace>=0.01 )
			m_dbInterSpace = dbSpace;
	};
	double GetInterSpace(){return m_dbInterSpace;};

protected:	
	double m_dbInterSpace;

protected:
	double GetDVal(LINE,LINE);
	BOOL   IsDotInLine(LINE,LINE,InterDot);

};

#endif // !defined(AFX_DOTINTERSECTION_H__64F7C7D8_D865_4BCB_8C48_8822BC6A2F2E__INCLUDED_)




