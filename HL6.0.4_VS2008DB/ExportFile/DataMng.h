// DataMng.h: interface for the CDataMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMNG_H__97BB0553_058E_44F0_B86B_BD12EC8C6CF2__INCLUDED_)
#define AFX_DATAMNG_H__97BB0553_058E_44F0_B86B_BD12EC8C6CF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrokeB.h"
#include "Plane.h"
#include "mat.h"

//
typedef struct SortObj
{
	SortObj()
	{
		m_pObj = NULL;
		m_nFlag = -999;
		m_dbValue = 0;
		m_quad.SetEmpty();
	}
	
	~SortObj()
	{
		m_pObj = NULL;
		m_nFlag = -999;
		m_dbValue = 0;
		m_quad.SetEmpty();
	}
	
	void operator = ( const SortObj & src )
	{
		if( &src == this )
			return;
		
		m_dbValue = src.m_dbValue;
		m_nFlag = src.m_nFlag;
		m_pObj = src.m_pObj;
		m_quad = src.m_quad;
	}
	
	void operator = ( const double dbValue )
	{
		m_dbValue = dbValue;
	}
	
	BOOL operator > ( const SortObj & src )
	{
		if( &src == this )
			return FALSE;
		
		if( m_dbValue > src.m_dbValue && fabs(m_dbValue - src.m_dbValue) > EPSILON )
			return TRUE;
		else
			return FALSE;
	}
	
	BOOL operator == ( const SortObj & src )
	{
		if( &src == this )
			return TRUE;
		
		if( fabs(m_dbValue - src.m_dbValue) < EPSILON )
			return TRUE;
		else
			return FALSE;
	}
	
	BOOL operator < ( const SortObj & src )
	{
		if( &src == this )
			return FALSE;
		
		if( m_dbValue < src.m_dbValue && fabs(m_dbValue - src.m_dbValue) > EPSILON )
			return TRUE;
		else
			return FALSE;
	}
	
public:
	CQuadB m_quad;
	double m_dbValue;
	int m_nFlag;
	DWORD   m_pObj;
} SORTOBJ;






class AFX_EXT_CLASS CDataMng
{
public:
	CDataMng();
	virtual ~CDataMng();
	
	//链表数组排序
	void SortArrayList(CArray<StrokeList*, StrokeList*> &arrayListSource, BOOL &bFlag, BOOL bAxisX = TRUE, BOOL bAscend = TRUE);
	
	//取得链表数组外包框
	CQuadB GetArrayQuad(CArray<StrokeList*, StrokeList*> &arrayList);
	
	//取得链表外包框
	CQuadB GetListQuad(StrokeList &listStrokeSource);
	
	//取得笔画外包框
	CQuadB GetStrokeQuad(CStrokeB *pStroke);
	
	//外包框合并
	CQuadB ComposeQuad(CQuadB quad1, CQuadB quad2);
	
	//求线段(dbX1,dbY1)(dbX2,dbY2)与水平或垂直扫描线的交点，如果有交点返回TRUE，并且dbX,dbY返回交点坐标
	BOOL CalCross(double &dbX1, double &dbY1, double &dbX2, double &dbY2, double &dbX, double &dbY,BOOL bVScan);
	
	//链表内笔画合并
	void ListJoin(StrokeList &listStrokeSour, double dbMinDistance=0.001);
	BOOL ListJoin(CStrokeB *pStrokeNew, StrokeList &listStrokeSour, double dbMinDistance);
	
	//设定链表属性
	void SetStyle( int nStyle, StrokeList &listStrokeSource);

	//设定链表数组属性
	void SetStyle( int nStyle, CArray<StrokeList*, StrokeList*> &arrayListSource);
	
	//设定链表标记
	void SetFlag( BOOL bFlag, StrokeList &listStrokeSource);
	
	//设定链表数组标记
	void SetFlag( BOOL bFlag, CArray<StrokeList*, StrokeList*> &arrayListSource);
	
	//取得链表标记
	BOOL GetListFlag(StrokeList &listStroke);
	
	//删除链表数组
	void DeleteArrayList(CArray<StrokeList*, StrokeList*> &arrayList);

	//删除链表
	void DeleteList( StrokeList &listData);

	//拷贝链表数组
	void ArrayListAddCopy( CArray<StrokeList*, StrokeList*> &m_array, CArray<StrokeList*, StrokeList*> &m_arrayAdd );

	//拷贝链表
	void ListAddCopy( StrokeList &listData, const StrokeList &listDataAdd );

	//将笔画按点顺序(每两点为一个新笔画)切分为链表
	void DivStroke(StrokeList &listStroke, CStrokeB *pStroke);

	//将链表内的所有笔画对象进行"按点顺序(每两点为一个新笔画)切分为链表"的操作
	void CreateDivStrokeList(StrokeList &listStroke, StrokeList &listStrokeSource);
	
	//链表移动
	void ListMove(StrokeList &listStroke, double moveX, double moveY);
	
	//链表数组移动
	void ArraylistMove(CArray<StrokeList*, StrokeList*> &arrayList, double moveX, double moveY);
	
	//链表拉伸
	void ListScale(StrokeList &listStroke, CDot &dot, double ratioX, double ratioY);
	
	//链表数组拉伸
	void ArraylistScale(CArray<StrokeList*, StrokeList*> &arrayList, CDot &dot, double ratioX,  double ratioY);
	
	//判断链表是否在外包框内
	BOOL ListInQuad(StrokeList &listStroke, CQuadB quad);
	
	//判断点是否在外包框内
	BOOL DotInQuad(CDot dot, CQuadB quad);
	
	//将listStrokeAdd中的对象添加到listStroke中
	void ListAppend(StrokeList &listStroke, StrokeList &listStrokeAdd);
	
	//快速排序
	void QuickSort(CArray<SORTOBJ,SORTOBJ> &arraySort, int nLeft, int nRight, BOOL &bFlag, BOOL bAscend = TRUE);	
	
	//将笔画内的点对象进行反序排列
	void InverseStroke(CStrokeB *pStroke);
	
	//将链表内的笔画对象进行反序排列
	void InverseList(StrokeList &ListStroke);

	//判断点是否在多边形内
	BOOL DotInPolygon(CDot dot, CStrokeB *pPolygon);

	//判断点是否在多边形边上
	BOOL DotOnPolygon( CDot dot, CStrokeB *pPolygon );

	//判断stroke是否在多边形内
	BOOL StrokeInPolygon(CStrokeB *pStroke, CStrokeB *pPolygon);
	
	//判断两个多边形相互重叠包含的状况
	int CheckPolygonTouch(CStrokeB *pStrokeA, CStrokeB *pStrokeB);

	//判断两个多边形是否相交
	BOOL StrokeIsInterSection(CStrokeB *pStrokeP, CStrokeB *pStrokeQ);
	BOOL QuadIsInterSection(CQuadB &quadP, CQuadB &quadQ);

	//排列曲线方向
	void CDataMng::ArrangeStroke( CStrokeB *pStroke, BOOL bAsColck=FALSE);
	/*********************************************************/
	//计算点间距
	double GetDistance(CDot &sDot, CDot &eDot);
	
	//两点确定直线方程
	void CreateLine( CDot dot1, CDot dot2, double &a, double &b, double &c);
	
	//点到直线的距离
	double GetDisPtL( CDot dt1, CDot dt2, CDot dt );
	
	//点到直线距离
	double GetDisPtL( double a, double b, double c, CDot dt );
	
	//计算平面空间变换矩阵
	int CalTransMat( CPlane &planeOld, CPlane &planeNew, CMatrix &mat, CMatrix &mat1, CMatrix &mat2, CMatrix &mat3 );
	
	//求平面绕任意向量旋转的变换矩阵
	int CalTransMat( CDot dot1, CDot dot2, double dbAngle, CMatrix &mat );
	
	//通过矩阵变换曲线
	int TransByMat(CArray<StrokeList*, StrokeList*> &arrayList, const CMatrix &mat);
	
	//计算平面的法向量
	CDot CalPlaneNormalLine(CPlane &plane);
	
	//
	double CalVecAngle(CDot vect1, CDot vect2);
	double CalVecAngle(CDot vect1, CDot vect2, CDot vectN);
	
	//绕坐标轴旋转矩阵 0:x, 1:y, 2:z
	void CalRotateAxisMatrix(int nAxis, double dbAngle, CMatrix &matRotate);

	//
	int GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot);
	int IsCross(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 );
};

#endif // !defined(AFX_DATAMNG_H__97BB0553_058E_44F0_B86B_BD12EC8C6CF2__INCLUDED_)
