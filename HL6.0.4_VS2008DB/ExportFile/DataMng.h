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
	
	//������������
	void SortArrayList(CArray<StrokeList*, StrokeList*> &arrayListSource, BOOL &bFlag, BOOL bAxisX = TRUE, BOOL bAscend = TRUE);
	
	//ȡ���������������
	CQuadB GetArrayQuad(CArray<StrokeList*, StrokeList*> &arrayList);
	
	//ȡ�����������
	CQuadB GetListQuad(StrokeList &listStrokeSource);
	
	//ȡ�ñʻ������
	CQuadB GetStrokeQuad(CStrokeB *pStroke);
	
	//�����ϲ�
	CQuadB ComposeQuad(CQuadB quad1, CQuadB quad2);
	
	//���߶�(dbX1,dbY1)(dbX2,dbY2)��ˮƽ��ֱɨ���ߵĽ��㣬����н��㷵��TRUE������dbX,dbY���ؽ�������
	BOOL CalCross(double &dbX1, double &dbY1, double &dbX2, double &dbY2, double &dbX, double &dbY,BOOL bVScan);
	
	//�����ڱʻ��ϲ�
	void ListJoin(StrokeList &listStrokeSour, double dbMinDistance=0.001);
	BOOL ListJoin(CStrokeB *pStrokeNew, StrokeList &listStrokeSour, double dbMinDistance);
	
	//�趨��������
	void SetStyle( int nStyle, StrokeList &listStrokeSource);

	//�趨������������
	void SetStyle( int nStyle, CArray<StrokeList*, StrokeList*> &arrayListSource);
	
	//�趨������
	void SetFlag( BOOL bFlag, StrokeList &listStrokeSource);
	
	//�趨����������
	void SetFlag( BOOL bFlag, CArray<StrokeList*, StrokeList*> &arrayListSource);
	
	//ȡ��������
	BOOL GetListFlag(StrokeList &listStroke);
	
	//ɾ����������
	void DeleteArrayList(CArray<StrokeList*, StrokeList*> &arrayList);

	//ɾ������
	void DeleteList( StrokeList &listData);

	//������������
	void ArrayListAddCopy( CArray<StrokeList*, StrokeList*> &m_array, CArray<StrokeList*, StrokeList*> &m_arrayAdd );

	//��������
	void ListAddCopy( StrokeList &listData, const StrokeList &listDataAdd );

	//���ʻ�����˳��(ÿ����Ϊһ���±ʻ�)�з�Ϊ����
	void DivStroke(StrokeList &listStroke, CStrokeB *pStroke);

	//�������ڵ����бʻ��������"����˳��(ÿ����Ϊһ���±ʻ�)�з�Ϊ����"�Ĳ���
	void CreateDivStrokeList(StrokeList &listStroke, StrokeList &listStrokeSource);
	
	//�����ƶ�
	void ListMove(StrokeList &listStroke, double moveX, double moveY);
	
	//���������ƶ�
	void ArraylistMove(CArray<StrokeList*, StrokeList*> &arrayList, double moveX, double moveY);
	
	//��������
	void ListScale(StrokeList &listStroke, CDot &dot, double ratioX, double ratioY);
	
	//������������
	void ArraylistScale(CArray<StrokeList*, StrokeList*> &arrayList, CDot &dot, double ratioX,  double ratioY);
	
	//�ж������Ƿ����������
	BOOL ListInQuad(StrokeList &listStroke, CQuadB quad);
	
	//�жϵ��Ƿ����������
	BOOL DotInQuad(CDot dot, CQuadB quad);
	
	//��listStrokeAdd�еĶ�����ӵ�listStroke��
	void ListAppend(StrokeList &listStroke, StrokeList &listStrokeAdd);
	
	//��������
	void QuickSort(CArray<SORTOBJ,SORTOBJ> &arraySort, int nLeft, int nRight, BOOL &bFlag, BOOL bAscend = TRUE);	
	
	//���ʻ��ڵĵ������з�������
	void InverseStroke(CStrokeB *pStroke);
	
	//�������ڵıʻ�������з�������
	void InverseList(StrokeList &ListStroke);

	//�жϵ��Ƿ��ڶ������
	BOOL DotInPolygon(CDot dot, CStrokeB *pPolygon);

	//�жϵ��Ƿ��ڶ���α���
	BOOL DotOnPolygon( CDot dot, CStrokeB *pPolygon );

	//�ж�stroke�Ƿ��ڶ������
	BOOL StrokeInPolygon(CStrokeB *pStroke, CStrokeB *pPolygon);
	
	//�ж�����������໥�ص�������״��
	int CheckPolygonTouch(CStrokeB *pStrokeA, CStrokeB *pStrokeB);

	//�ж�����������Ƿ��ཻ
	BOOL StrokeIsInterSection(CStrokeB *pStrokeP, CStrokeB *pStrokeQ);
	BOOL QuadIsInterSection(CQuadB &quadP, CQuadB &quadQ);

	//�������߷���
	void CDataMng::ArrangeStroke( CStrokeB *pStroke, BOOL bAsColck=FALSE);
	/*********************************************************/
	//�������
	double GetDistance(CDot &sDot, CDot &eDot);
	
	//����ȷ��ֱ�߷���
	void CreateLine( CDot dot1, CDot dot2, double &a, double &b, double &c);
	
	//�㵽ֱ�ߵľ���
	double GetDisPtL( CDot dt1, CDot dt2, CDot dt );
	
	//�㵽ֱ�߾���
	double GetDisPtL( double a, double b, double c, CDot dt );
	
	//����ƽ��ռ�任����
	int CalTransMat( CPlane &planeOld, CPlane &planeNew, CMatrix &mat, CMatrix &mat1, CMatrix &mat2, CMatrix &mat3 );
	
	//��ƽ��������������ת�ı任����
	int CalTransMat( CDot dot1, CDot dot2, double dbAngle, CMatrix &mat );
	
	//ͨ������任����
	int TransByMat(CArray<StrokeList*, StrokeList*> &arrayList, const CMatrix &mat);
	
	//����ƽ��ķ�����
	CDot CalPlaneNormalLine(CPlane &plane);
	
	//
	double CalVecAngle(CDot vect1, CDot vect2);
	double CalVecAngle(CDot vect1, CDot vect2, CDot vectN);
	
	//����������ת���� 0:x, 1:y, 2:z
	void CalRotateAxisMatrix(int nAxis, double dbAngle, CMatrix &matRotate);

	//
	int GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot);
	int IsCross(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 );
};

#endif // !defined(AFX_DATAMNG_H__97BB0553_058E_44F0_B86B_BD12EC8C6CF2__INCLUDED_)
