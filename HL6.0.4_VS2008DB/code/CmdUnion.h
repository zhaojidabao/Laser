// CmdUnion.h: interface for the CCmdUnion class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CMDUNION_H__6BF69CBB_AAE4_433b_80DA_C6816510A5BA__INCLUDED_)
#define AFX_CMDUNION_H__6BF69CBB_AAE4_433b_80DA_C6816510A5BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdUnion : public CCommand  
{
public:
	CCmdUnion(CHLDoc *pDoc);
	virtual ~CCmdUnion();
	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc* m_pDoc;
	BOOL	m_bFirst;
	BOOL*   m_pFlag;
	BOOL*	m_pFlag2;
};

#endif // !defined(AFX_CMDUNION_H__6BF69CBB_AAE4_433b_80DA_C6816510A5BA__INCLUDED_)
