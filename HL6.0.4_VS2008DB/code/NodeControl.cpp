// NodeControl.cpp: implementation of the CNodeControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeControl::CNodeControl()
{

}

CNodeControl::~CNodeControl()
{

}

CQuad CNodeControl::CalLimit()
{
	CDot dot(0,0);
	CQuad quad(dot,0.000001,0.000001);
	m_quadLimit = quad;
	
	return m_quadLimit;
}

void CNodeControl::Draw(XDC *pDC)
{
	return;
}

BOOL CNodeControl::Click(CPoint &point,CWnd *pWnd)
{
	return FALSE;
}


void CNodeControl::SetSelect(BOOL bSelect)
{
	m_bSelect = bSelect;
}


BOOL CNodeControl::Include(CQuad quad)
{
	return FALSE;
}

BOOL  CNodeControl::CanUnGroup()
{
	return FALSE;
}


BOOL  CNodeControl::CanGroup()
{
	return FALSE;
}
