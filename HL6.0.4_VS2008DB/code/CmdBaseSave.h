#pragma once

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdBaseSave :	public CCommand
{
public:
	CCmdBaseSave(CHLDoc *pDoc);
	virtual ~CCmdBaseSave();

	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc *m_pDoc;
	CChain	m_chainOld;
	CChain	m_chainNew;
	BOOL	m_bFirst;
};
