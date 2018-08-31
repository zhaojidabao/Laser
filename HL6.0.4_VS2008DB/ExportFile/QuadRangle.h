// QuadRangle.h: interface for the CQuadRangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADRANGLE_H__3D62C918_1A4C_4C87_8510_5C47DCDE68C9__INCLUDED_)
#define AFX_QUADRANGLE_H__3D62C918_1A4C_4C87_8510_5C47DCDE68C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dot.h"

class AFX_EXT_CLASS CQuadRangle //四边形类
{
public:
	CQuadRangle();
	CQuadRangle(CDot dotLB, CDot dotLT, CDot dotRT, CDot dotRB);
	virtual ~CQuadRangle();
	
	void operator=(const CQuadRangle& src);
	virtual double GetHeight();
	virtual double GetWidth();
	virtual BOOL   IsMirror();
	virtual double GetVerHeight();//校正后的高度
	virtual double GetVerWidth(); //校正后的宽度
	virtual double CalShearAngle();
	virtual double CalRotateAngle();
	virtual void  Move(double moveX, double moveY);
	virtual void  Scale(CDot dot, double ratioX, double ratioY);
	virtual void  Rotate(CDot dot, double angle);
	virtual void  Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual CDot  GetBaseDot(int nType);
	virtual void  Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual void  Inflate(double dbOffset);
	CDot  GetShearBaseDot(int nType);
	
protected:
	CDot	m_dotLB;
	CDot	m_dotLT;
	CDot	m_dotRT;
	CDot	m_dotRB;
	
protected:
	double GetLeftLineLen();
	double GetRightLineLen();
	double GetTopLineLen();
	double GetBottomLineLen();
	CDot UnitVec(CDot dotStart, CDot dotEnd);
	double GetVectorAngleX(CDot dot);
	double GetVectorAngle(CDot dot1, CDot dot2);
	BOOL GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot);
	int GetQuadrantNo(CDot dot);
};

#endif // !defined(AFX_QUADRANGLE_H__3D62C918_1A4C_4C87_8510_5C47DCDE68C9__INCLUDED_)
