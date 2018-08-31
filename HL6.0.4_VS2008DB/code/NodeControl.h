// NodeControl.h: interface for the CNodeControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECONTROL_H__97F59847_4645_4EE1_A304_8A07E15D7E5A__INCLUDED_)
#define AFX_NODECONTROL_H__97F59847_4645_4EE1_A304_8A07E15D7E5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeV.h"

class CNodeControl : public CNodeV  
{
public:
	CNodeControl();
	virtual ~CNodeControl();

	virtual CQuad CalLimit();
	virtual void    Draw(XDC *pDC);
	virtual BOOL  Click(CPoint &point,CWnd *pWnd);
	virtual void    SetSelect(BOOL bSelect);
	virtual BOOL  Include(CQuad quad);
	virtual BOOL  CanUnGroup();
	virtual BOOL  CanGroup();
};

#endif // !defined(AFX_NODECONTROL_H__97F59847_4645_4EE1_A304_8A07E15D7E5A__INCLUDED_)
