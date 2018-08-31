// Fnt.h: interface for the CFnt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNT_H__DB4DFD01_FEA4_11D4_A486_00105A615525__INCLUDED_)
#define AFX_FNT_H__DB4DFD01_FEA4_11D4_A486_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"
#include "QuadRangle.h"
#include "Quad.h"	// Added by ClassView

class CFnt : public CNodeV  
{	
public:
	CFnt();
	
	CQuad	m_quadInit;			//初始时边框大小
	CString	m_strText;			//字符串
	CDot	m_dotBase;				
	double	m_dbHeight;			//字高
	double	m_dbCharSpace;		//字间距
	double	m_dbLineSpace;		//行间距
	double	m_dbWidthFactor;	//宽度系数
	
	int		m_nDrawType;
	int		m_nPath;		//路径方式
	int		m_nAlign;		//对齐方式
	double	m_dbRadius;		//弧形方式下的半径
	BOOL	m_bCCW;			//弧形方式下的时针方向
	double	m_dbStart;		//弧形方式下的开始角度
	BOOL	m_bInside;		//弧形方式下的字符朝内
	BOOL	m_bFixSpace;	//固定间距设置
	double	m_dbFixSpace;	//等字符间距
	CString	m_strPrecisK;	//字体精度因子
	BOOL	m_bPrintInverse;	//印章反刻

	// 两个角度、两个半径、加圆心m_dotBase可生成圆弧排布框
	double  m_dbEndAngle;
	double  m_dbRadius2;

public:	
	void		Effect();
	virtual	void	 Draw(XDC *pDC);
	virtual	BOOL  CreateStrokeList();
	virtual	BOOL  MakeStroke();
	virtual	CQuad CalLimit();
	virtual	CQuad GetMarkRange();

	CQuad	CalLimit(StrokeList* pList);

	void	StrokeMove(StrokeList *pList,double moveX,double moveY);
	void	StrokeRotate(StrokeList *pList,CDot dot,double angle);
	CQuad	CalLimit(CDot dot,double dbXScale = 1.0,double dbYScale = 1.0);
	BOOL	GetOffsetDotAngle(	CDot dotCur, double dbRadius, double dbStartAngle, BOOL bCRotate, 
	                            BOOL bInside,CQuad quadTotal,CDot &dotRet,double &dbRotAngle);
	void PrintInverse( StrokeList &listTmp );
};

#endif // !defined(AFX_FNT_H__DB4DFD01_FEA4_11D4_A486_00105A615525__INCLUDED_)
