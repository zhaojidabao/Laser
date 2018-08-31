#if !defined(AFX_CMDUNUNION_H__78EBAF49_DDDD_4703_9C77_D8469DE25246__INCLUDED_)
#define AFX_CMDUNUNION_H__78EBAF49_DDDD_4703_9C77_D8469DE25246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdUnunion : public CCommand  
{
public:
	CCmdUnunion(CHLDoc *pDoc);
	virtual ~CCmdUnunion();

	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc*	m_pDoc;
	BOOL*	m_pFlag1;
	BOOL*	m_pFlag2;
	BOOL	m_bFirst;
};

#endif // !defined(AFX_CMDUNUNION_H__78EBAF49_DDDD_4703_9C77_D8469DE25246__INCLUDED_)