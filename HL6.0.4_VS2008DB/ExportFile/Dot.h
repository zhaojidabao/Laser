// DotB.h: interface for the CDot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTB_H__CC5347BE_2640_407E_B6C0_DB18F575E434__INCLUDED_)
#define AFX_DOTB_H__CC5347BE_2640_407E_B6C0_DB18F575E434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	double x;
 	double y;
	double z;
}DOT;

class AFX_EXT_CLASS CDot:public DOT
{
public:
	CDot();
	CDot(CPoint &pt);
	CDot(POINT pt);	
	CDot(double xValue,double yValue,double zValue=0);
	virtual ~CDot();
	
public:
	void XChangeY();
	void TrueToLog();
	void LogToTrue();
	
	void Rotate(CDot center,double angle);
	void Rotate(CDot center, int nPlane, double angle);
	void Move(double moveX,double moveY);
	void Move(double moveX,double moveY,double moveZ);
	void SetOffset(double xf,double yf);
	void Scale(CDot center,double ratioX,double ratioY);
	BOOL IsDotInLine(CDot& p1 , CDot& p2 );
	void Useless(){return;};
	void Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	
	double GetVectorAngle(CDot dot);
	double GetVectorAngleX(CDot dot);
	int GetQuadrantNo(CDot dot);

	void Unit();
	double GetM();
	
public:
	CDot operator*=(double mul);
	CDot operator=(const CDot& src);
	CDot operator=(const DOT& src);
	operator CPoint();
	BOOL operator==(const CDot& src);
	
	//	
	CDot operator *(double d);
	CDot operator *(CDot dot);
	CDot operator -(CDot dot);
	CDot operator +(CDot dot);
	CDot operator /(double d);
	BOOL operator !=(CDot dot);

	
};
#endif // !defined(AFX_DOTB_H__CC5347BE_2640_407E_B6C0_DB18F575E434__INCLUDED_)
