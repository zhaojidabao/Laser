// PicStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "PicStatic.h"


// CPicStatic

IMPLEMENT_DYNAMIC(CPicStatic, CStatic)

CPicStatic::CPicStatic()
{
	m_dbRadius = 0.05;
	m_dbDistance = 0.025;

}

CPicStatic::~CPicStatic()
{
}


BEGIN_MESSAGE_MAP(CPicStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPicStatic 消息处理程序
void CPicStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect,RGB(255,255,255));
	dc.SetMapMode(MM_ISOTROPIC);
	dc.SetWindowExt(rect.Width()*20,rect.Height()*20);
	dc.SetViewportExt(rect.Width(),-rect.Height());
	dc.SetViewportOrg(rect.Width()/2,rect.Height()/2);

	CBrush*brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//类名可以调用静态成员函数，而不可以调用非静态成员函数。但是对象都可以调用。
	CBrush *pOldbrush = dc.SelectObject(brush);
 
	CPen   pen(PS_DOT, 1, RGB(255,0,0));
	CPen  *pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(-10*rect.Width(),0);
	dc.LineTo(10*rect.Width(),0);
	dc.MoveTo(0,-10*rect.Height());
	dc.LineTo(0,10*rect.Height());
	dc.SelectObject(pOldPen);

// 	double r = m_dbRadius*10000;
// 	double d = m_dbDistance*10000;
	m_dbRadius /=2;
	if( m_dbDistance > 0.0001 && m_dbRadius > 0)
	{
		double r = 600;
		double max = 10*rect.Width()-r;
		double d = 600*m_dbDistance/m_dbRadius;
		for(double dbValue = 0; dbValue<max;dbValue += d)
		{
			dc.Ellipse(dbValue-r,-r,dbValue+r,r);
			dc.Ellipse(-dbValue+r,-r,-dbValue-r,r);
		}
	}
	dc.SelectObject(pOldbrush);
}
