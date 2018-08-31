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
	
	CQuad	m_quadInit;			//��ʼʱ�߿��С
	CString	m_strText;			//�ַ���
	CDot	m_dotBase;				
	double	m_dbHeight;			//�ָ�
	double	m_dbCharSpace;		//�ּ��
	double	m_dbLineSpace;		//�м��
	double	m_dbWidthFactor;	//���ϵ��
	
	int		m_nDrawType;
	int		m_nPath;		//·����ʽ
	int		m_nAlign;		//���뷽ʽ
	double	m_dbRadius;		//���η�ʽ�µİ뾶
	BOOL	m_bCCW;			//���η�ʽ�µ�ʱ�뷽��
	double	m_dbStart;		//���η�ʽ�µĿ�ʼ�Ƕ�
	BOOL	m_bInside;		//���η�ʽ�µ��ַ�����
	BOOL	m_bFixSpace;	//�̶��������
	double	m_dbFixSpace;	//���ַ����
	CString	m_strPrecisK;	//���徫������
	BOOL	m_bPrintInverse;	//ӡ�·���

	// �����Ƕȡ������뾶����Բ��m_dotBase������Բ���Ų���
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
