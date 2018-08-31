#include "stdafx.h"
#include "CmdBaseSave.h"

CCmdBaseSave::CCmdBaseSave(CHLDoc *pDoc )
{
	m_bFirst = TRUE;
	m_pDoc = pDoc;
}

CCmdBaseSave::~CCmdBaseSave(void)
{
}

BOOL CCmdBaseSave::Do()
{
	if ( m_bFirst )
		m_bFirst = FALSE;
	else if ( m_pDoc )
		m_pDoc->SetChain(m_chainNew);

	return TRUE;
}

BOOL CCmdBaseSave::Undo()
{
	 if ( m_pDoc )
		m_pDoc->SetChain(m_chainOld);

	return TRUE;
}