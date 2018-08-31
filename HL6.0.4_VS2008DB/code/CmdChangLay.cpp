// CmdChangLay.cpp: implementation of the CCmdChangLay class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "CmdChangLay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdChangLay::CCmdChangLay(CHLDoc *pDoc,int nLayer,int nFillLayer,int nFillScalLayer)
{
	commandName.LoadString(IDS_COMMAND_CHANGE_LAYER);
	m_pDoc = pDoc;
	m_nLayer = nLayer;
	m_nFillLayer = nFillLayer;
	m_nFillScalLayer = nFillScalLayer;
	m_pFlag = NULL;
	m_bFirst = TRUE;
	if ( m_pDoc )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pLayer = new int[m_pDoc->m_chain.GetCount()];
		m_pFillLayer = new int[m_pDoc->m_chain.GetCount()];
		m_pFillScalLayer = new int[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->m_chain.GetAllGroupLayColor(m_pLayer,m_pFillLayer,m_pFillScalLayer);
		m_pDoc->SetLayerCmd(m_nLayer,m_nFillLayer,m_nFillScalLayer);
	}
}

CCmdChangLay::~CCmdChangLay()
{
	if( m_pFlag )
	{
		delete[] m_pFlag;
		m_pFlag = NULL;
	}
	if( m_pLayer )
	{
		delete[] m_pLayer;
		m_pLayer = NULL;
	}
	if( m_pFillLayer )
	{
		delete[] m_pFillLayer;
		m_pFillLayer = NULL;
	}
	if( m_pFillScalLayer )
	{
		delete[] m_pFillScalLayer;
		m_pFillScalLayer = NULL;
	}
}
BOOL CCmdChangLay::Do()
{
	if (m_bFirst)
		m_bFirst = FALSE;
	else
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->SetLayerCmd(m_nLayer,m_nFillLayer,m_nFillScalLayer);
	}
	return TRUE;
}

BOOL CCmdChangLay::Undo()
{
	m_pDoc->m_chain.SetSelectFlag(m_pFlag);
	m_pDoc->m_chain.SetAllGroupLayColor(m_pLayer,m_pFillLayer,m_pFillScalLayer);
	return TRUE;
}
