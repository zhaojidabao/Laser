// CmdSetLay.cpp: implementation of the CCmdSetLay class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "CmdSetLay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdSetLay::CCmdSetLay(CHLDoc *pDoc,int mode,CLayer &ly)
{
	m_bFirst = TRUE;
	m_bFlag = FALSE;
	m_pFlag = NULL;
	m_pLayer = m_pFillLayer = m_pFillScalLayer = NULL;
 	if ( mode == PTY_OUTLINE || mode == PTY_FILL || mode == PTY_DEL|| mode == PTY_LAYER  )
 		m_bFlag = TRUE;

	m_pDoc = pDoc;
	commandName.LoadString(IDS_COMMAND_SETLAY);
	if ( m_pDoc == NULL )
		return;

	m_layerOld = pDoc->GetLayer(ly.m_nLayer);
	if ( m_bFlag )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pLayer = new int[m_pDoc->m_chain.GetCount()];
		m_pFillLayer = new int[m_pDoc->m_chain.GetCount()];
		m_pFillScalLayer = new int[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->m_chain.GetAllGroupLayColor(m_pLayer,m_pFillLayer,m_pFillScalLayer);
	}
	m_pDoc->SetLayCmd(mode,ly);
	m_layerNew = pDoc->GetLayer(ly.m_nLayer);
	m_nMode = mode;
}

CCmdSetLay::~CCmdSetLay()
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


BOOL CCmdSetLay::Do( )
{
	if ( m_bFirst )
	{
		m_bFirst = FALSE;
	}
	else
	{
		if ( m_bFlag )    
		{
			m_pDoc->m_chain.SetSelectFlag(m_pFlag);
			m_pDoc->SetLayCmd(m_nMode,m_layerNew);
		}
		else
		{
			m_pDoc->SetLayer(m_layerNew);
		}
	}

	return TRUE;
}

BOOL CCmdSetLay::Undo()
{
	if ( m_bFlag )    
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
		m_pDoc->m_chain.SetAllGroupLayColor(m_pLayer,m_pFillLayer,m_pFillScalLayer);
	}
	else
	{
		m_pDoc->SetLayer(m_layerOld);
	}

	return TRUE;
}
