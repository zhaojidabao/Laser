// NodeT.h: interface for the CNodeT class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Quad.h"
#include "Stroke.h"
#include "Text.h"
#include "NodeT_B.h"

class CNodeT : public CNodeT_B  
{
	friend class CTree;

public:
	CNodeT();
	virtual ~CNodeT();

public:
	virtual CStrokeB* CreateNewStroke(); 
	virtual UINT   Mark(HWND hWnd);
    virtual UINT   SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark);
	virtual void    InitQuadRange();
	virtual CQuad CalLimit();
	virtual BOOL  Include(CQuad quad);
	virtual BOOL  IsOnDot(CPoint &point,CWnd *pWnd);
	virtual void   Draw(XDC *pDC);
	virtual void   AddRect(CWnd *pWnd);
	virtual BOOL Click(CPoint &point,CWnd *pWnd);
	virtual void   DelRect(CWnd *pWnd);
	//
public:
	void    ChangeTo(CQuad quad);
	BOOL   HasDot(CQuad &quad);
	////
	virtual  CTree * UnGroup();

	//bRefΪTRUE��ֻȡ������ݷ�Χ(��������Բ���Ų��е�Բ��),FALSEȡ����Բ���ķ�Χ
	virtual  CQuad	 GetLimit(BOOL bRef = FALSE){return m_quadLimit;};
	virtual  BOOL  GetLockFlg();
	virtual  void  RefreshBarText(CWnd* pWnd, int nTextID){};

protected:
	CQuad		m_quadLimit;
};

