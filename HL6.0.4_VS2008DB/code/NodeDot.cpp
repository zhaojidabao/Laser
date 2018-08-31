// NodeDot.cpp: implementation of the CNodeDot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeDot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeDot::CNodeDot()
{
	m_type = NODE_DOT;
}

CNodeDot::~CNodeDot()
{

}

BOOL CNodeDot::Create(const CPoint& point)
{
	CStroke *pStroke = new CStroke(m_property);
	if ( pStroke == NULL )
		return FALSE;
	
	Delete();

	CDot  dot(point);	
	pStroke->Add(dot);
	m_list.AddTail(pStroke);
	
	return TRUE;
}



CQuad CNodeDot::CalLimit()
{
	CNodeV::CalLimit();
	
	m_quadLimit.Inflate(0.001,0.001);

	return m_quadLimit;
}

void CNodeDot::Draw(XDC *pDC)
{
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos  !=  NULL)
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->Draw(pDC, FONT_MATRIX, m_bIsCurSortTree);
	}
}


void CNodeDot::Serialize (FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	
	if ( bFlag )
	{
		//写入头信息
		fs << _T("NDOT_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		Serialize_list(fp,bFlag,bUnicode);
		
		//写入尾信息
		fs << _T("NDOT_E");
	}
	else
	{	
		CString str;
		while( fs >> str )
		{
			if ( str == _T("QUAD_RANG_B") )
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("PRO_B") )
				m_property.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("SL_B") )
				Serialize_list(fp,bFlag,bUnicode);
			else if ( str == _T("NDOT_E") )
				break;
		} 
	} 
}


void CNodeDot::AddToPlt(CNode *pNodePlt)
{
}