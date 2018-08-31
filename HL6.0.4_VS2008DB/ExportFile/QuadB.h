// QuadB.h: interface for the CQuadB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADB_H__7EE63447_7009_4866_A6D0_F700DD7C994D__INCLUDED_)
#define AFX_QUADB_H__7EE63447_7009_4866_A6D0_F700DD7C994D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dot.h"
#include "Stdafx.h"

//�����ķ���ṹ
typedef struct
{
	double left;
	double right;
	double top;
	double bottom;
	double zmin;
	double zmax;
}QUAD;

class AFX_EXT_CLASS CQuadB:public QUAD
{
public:
	CQuadB();
	virtual ~CQuadB();

	CQuadB(RECT rect);
	CQuadB(double l, double t, double r, double b, double dmin=0, double dmax=0);			//���������
	CQuadB(CDot center,double width, double height, double zlen=0);			//���ĵ㣬�� �� Z����
	CQuadB(CDot dot1,CDot dot2);							//������
	CQuadB(int base,CDot dot,double width,double height);//��ĳ�����㣬���ȷ��һ������
	
public:
	virtual void LogToTrue();
	virtual void TrueToLog();
	virtual void Move(double dbMoveX, double dbMoveY, double dbMoveZ=0.0);
	virtual void SetOffset(double x,double y);
	virtual void Scale(CDot &dot,double &ratioX,double &ratioY);
	virtual double GetWid();
	virtual double GetLen();
	virtual void Initial();

	virtual BOOL Click(CDot dot, double dbR=15.0);	//ʰȡ
	virtual BOOL DotIn(CDot dot);
	virtual void Inflate(double x,double y);

	CDot Center();
	CDot GetBaseDot(int base=BASE_MM);
	
	virtual BOOL Include(CDot &dt);
	virtual BOOL Include(CQuadB quad);
	virtual double Height();
	virtual double Width();
	virtual void Normal();
	virtual BOOL IsEmpty();
	virtual BOOL IsNull();
	virtual BOOL Empty(){ return(left==right && top==bottom); }

	void operator=(const CQuadB& src);
	void operator=(const CRect& src);
	void operator*=(double ratio);
	operator CRect();
	void SetEmpty();
};

#endif // !defined(AFX_QUADB_H__7EE63447_7009_4866_A6D0_F700DD7C994D__INCLUDED_)
