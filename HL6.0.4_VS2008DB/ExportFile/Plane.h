#pragma once


#include "Dot.h"
#include "QuadB.h"

// CPlane ÃüÁîÄ¿±ê
class AFX_EXT_CLASS CPlane : public CObject
{
public:
	CPlane();
	CPlane(const CPlane& src);
	CPlane(CDot dot1, CDot dot2, CDot dot3);
	virtual ~CPlane();
	
	BOOL operator ==( CPlane &plane );
	virtual void operator =( const CPlane &plane );
	
	void SetDotArray(CDot dot1, CDot dot2, CDot dot3);
	void GetDotArray(CDot &dot1, CDot &dot2, CDot &dot3);
	CDot GetElement(int nPos);
	void SetElement(int nPos, CDot dot);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	
	//
	void Scale(CDot dot,double ratioX,double ratioY);
	void Rotate(CDot dot,double angle);
	void Rotate(CDot dot, int nPlane,double angle);
	void Move(double moveX,double moveY);
	void Move(double moveX,double moveY, double moveZ);
	void Effect(double r,double start,BOOL bCCW,BOOL bInside,CQuadB quad);
	void Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);

	//
	CArray<CDot, CDot> m_dotArray;
};


