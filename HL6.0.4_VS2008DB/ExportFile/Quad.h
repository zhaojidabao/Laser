
#pragma once
#include "QuadB.h"


class AFX_EXT_CLASS CQuad:public CQuadB  
{
public:
	CQuad();
	virtual ~CQuad();

public:
	CQuad(RECT rect);
	CQuad(double l,double t,double r,double b);			//�ĸ������
	CQuad(CDot center,double width,double height);	//���ĵ㣬���
	CQuad(CDot dot1,CDot dot2);							//������
	CQuad(int base,CDot dot,double width,double height);//��ĳ�����㣬���ȷ��һ������

public:
	void operator=(const CQuad& src);
	void operator=(const CRect& src);
	void operator*=(double ratio);
	operator CRect();
	
public:
	virtual void Compose(CQuad &quad);
	virtual BOOL Equal(CQuad quad);
	virtual BOOL Union(CQuad quad);
	virtual BOOL Include(CQuad quad);
	virtual BOOL Include(CPoint &point,CWnd *pWnd);
	virtual CDot Center();
	virtual CDot GetBaseDot(int base=BASE_MM);
};



typedef struct tagSelQuad
{
	tagSelQuad()
	{
		m_bSel  = FALSE;
		m_bFill = FALSE;
		m_pGroupObj = NULL;
	}

	~tagSelQuad()
	{
	}

	void operator = ( const tagSelQuad & src )
	{
		if( &src == this )
		{
			return;
		}
		
		m_bSel   = src.m_bSel;
		m_bFill  = src.m_bFill;
        m_pGroupObj  = src.m_pGroupObj;
        m_QD     = src.m_QD;
	}
	
	BOOL    m_bSel;
	BOOL    m_bFill;
	DWORD   m_pGroupObj;
	CQuad   m_QD; 
} SELQUAD;
