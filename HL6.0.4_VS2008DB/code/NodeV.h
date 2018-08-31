// NodeV.h: interface for the CNodeV class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Quad.h"
#include "Stroke.h"
#include "Node.h"

class CNodeV : public CNode  
{
	friend class CTree;

public:
	CNodeV();
	virtual ~CNodeV();

public:
	virtual CStrokeB* CreateNewStroke(); 
	virtual CNode*	 CreateNewNode();
	virtual UINT Mark(HWND hWnd);
    virtual UINT SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark);
	virtual void	 InitQuadRange();
	virtual CQuad CalLimit();
	virtual BOOL Include(CQuad quad);
	////
public:
	void    ChangeTo(CQuad quad);
	BOOL   HasDot(CQuad &quad);
	////
	virtual BOOL  IsOnDot(CPoint &point,CWnd *pWnd);
	virtual  CTree * UnGroup();
	virtual  CQuad	 GetLimit(BOOL bRef = FALSE){return m_quadLimit;};
	virtual  BOOL    GetLockFlg();
	virtual void   Draw(XDC *pDC);
	virtual void   AddRect(CWnd *pWnd);
	virtual BOOL Click(CPoint &point,CWnd *pWnd);
	virtual void   DelRect(CWnd *pWnd);

protected:
	CQuad		m_quadLimit;
};

