// NodeV.cpp: implementation of the CNodeV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeV.h"
#include "baseMacro.h"

CNodeV::CNodeV()
{
	m_quadLimit.Initial();
}

CNodeV::~CNodeV()
{

}

CStrokeB* CNodeV::CreateNewStroke()
{
	CStroke *pStroke = new CStroke(m_property);
	///////
	return pStroke;
}

CNode*	CNodeV::CreateNewNode()
{
	CNode *pNode = new CNodeV();
	/////
	return  pNode;
}

UINT  CNodeV::Mark(HWND hWnd)
{
	if ( m_list.GetCount()  <= 0 )
		return HLOK;

	if ( m_bPathOptimize == TRUE )
		return HLOK;

	CLay *pLay = g_DAT.GetCurLay();
	if ( pLay == NULL )
		return HLERROR;

	CStroke *pStroke = NULL;
	int nCount = pLay->GetRepeat(m_property.m_nLayer);

	// 在指定打标参数集模式下,层参数的打标次数无效。
	if ( ENU_MARKSPECPARASET == m_nMarkMode ) 
		nCount = 1;
	else if ( ENU_MARKSEL == m_nMarkMode ) 
		nCount = 1;

	UINT nStyle = (NODE_DOT == m_type) ? DOT_MARK : LINE_MARK;
	for( int i = 0; i < nCount; i++ )
	{
		if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
			return HLERROR;

		POSITION pos = m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{
			if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
				return HLERROR;

			pStroke = (CStroke*)m_list.GetNext(pos);
			if ( HLERROR == g_DAT.Mark(pStroke,hWnd, nStyle) )
				return HLERROR;
		}
	}

	return HLOK;
}

UINT CNodeV::SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark)
{
	if ( m_list.GetCount()  <=  0 )
		return HLOK;

	UINT nRet = HLOK;
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		nRet   = g_DAT.SelMark(pStroke, nLayer, hWnd, hSelStop, LINE_MARK,  bIsRevMark);
		if ( HLOK  !=  nRet )
			return nRet;
	}
	
	return nRet;
}

//
void CNodeV::InitQuadRange()
{
	CalLimit();
	/////
	CQuadRangle quad( m_quadLimit.GetBaseDot(BASE_BL),m_quadLimit.GetBaseDot(BASE_TL),
							   m_quadLimit.GetBaseDot(BASE_TR),m_quadLimit.GetBaseDot(BASE_BR) );
	
	m_quadRangle = quad;
}


CQuad CNodeV::CalLimit()
{
	m_quadLimit.Initial();
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos != NULL)
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		m_quadLimit.Compose(pStroke->CalLimit());
	}
	return m_quadLimit;
}

void CNodeV::ChangeTo(CQuad quad)
{
	CalLimit();
	
	//计算变形情况
	double ratioX,ratioY;
	if ( m_quadLimit.Width() )
 		ratioX = quad.Width()/m_quadLimit.Width();
	else
 		ratioX = 1;
	
	if ( m_quadLimit.Height() )
 		ratioY = quad.Height()/m_quadLimit.Height();
	else
 		ratioY = 1;
	
	CDot dotOld = m_quadLimit.GetBaseDot();
	CDot dotNew =  quad.GetBaseDot();
	
	double moveX = dotNew.x-dotOld.x;
	double moveY = dotNew.y-dotOld.y;

	Scale(dotOld,ratioX,ratioY);
	Move(moveX,moveY);
}

BOOL CNodeV::HasDot(CQuad &quad)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke->HasDot(quad) )
			return TRUE;
	}
	return FALSE;
}

CTree * CNodeV::UnGroup()
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	if ( pos != NULL )
	{	
		pStroke = (CStroke*)m_list.GetNext(pos);
		CNodeLine *pNode = new CNodeLine(pStroke);
		if ( pNode == NULL )
			return NULL;
		////////
		CTree *pTree = new CTree(pNode);
		if ( pTree  !=  NULL )
		{
			m_list.RemoveHead();
			return pTree;
		}
	}
	return NULL;
}

BOOL CNodeV::GetLockFlg()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return FALSE;
	
	int nLayer     = 0;
    int nFillLayer = 0;
	GetLayColor(nLayer, nFillLayer);
	
	return pDoc->GetLockFlg(nLayer);
}

BOOL CNodeV::IsOnDot(CPoint &point, CWnd *pWnd)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke &&  pStroke->IsOnDot(point,pWnd) )
			return TRUE;
	}
	return FALSE;
}

//
void CNodeV::Draw(XDC *pDC)
{
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos  !=  NULL)
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->Draw(pDC, 0, FALSE);
	}
}
//
void CNodeV::AddRect(CWnd *pWnd)
{
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		CStroke *pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->AddRect(pWnd);
	}
}

BOOL CNodeV::Click(CPoint &point,CWnd *pWnd)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke && pStroke->Click(point,pWnd) )
			return TRUE;
	}
	return FALSE;
}

void CNodeV::DelRect(CWnd *pWnd)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->DelRect(pWnd);
	}
}

BOOL CNodeV::Include(CQuad quad)
{
	return quad.Include(m_quadLimit);
}