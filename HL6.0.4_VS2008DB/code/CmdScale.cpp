// CmdScale.cpp: implementation of the CCmdScale class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdScale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdScale::CCmdScale(CHLDoc *pDoc,CDot dot,double ratioX,double ratioY)
{
	commandName.LoadString(IDS_COMMAND_SCALE);
	m_pDoc = pDoc;
	m_dotCenter = dot;
	m_dbRatioX = ratioX;
	m_dbRatioY = ratioY;
	m_bFirst = TRUE;
	m_pFlag = NULL;

	if ( m_pDoc )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->ScaleCmd(m_dotCenter,m_dbRatioX,m_dbRatioY);
	}
}

CCmdScale::~CCmdScale()
{
	if ( m_pFlag )
	{
		delete[] m_pFlag;
		m_pFlag = NULL;
	}
}

BOOL CCmdScale::Do( )
{
	if ( m_bFirst )
		m_bFirst = FALSE;
	else	if ( m_pDoc && m_pFlag )
	{
		m_pDoc->SetSelectFlag(m_pFlag);
		m_pDoc->ScaleCmd(m_dotCenter,m_dbRatioX,m_dbRatioY);
	}

	return TRUE;
}

BOOL CCmdScale::Undo()
{
	if ( m_pDoc  && m_pFlag )
	{
		m_pDoc->SetSelectFlag(m_pFlag);
		m_pDoc->ScaleCmd(m_dotCenter,1/m_dbRatioX,1/m_dbRatioY);
	}

	return TRUE;
}
