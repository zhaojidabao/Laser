// CmdGroup.h: interface for the CCmdGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDGROUP_H__2C141601_46F6_11D4_B789_E25FABB88F0F__INCLUDED_)
#define AFX_CMDGROUP_H__2C141601_46F6_11D4_B789_E25FABB88F0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
#include "CmdBaseSave.h"

class CCmdGroup : public CCmdBaseSave 
{
public:
	CCmdGroup(CHLDoc *pDoc);
	virtual ~CCmdGroup();
};

#endif // !defined(AFX_CMDGROUP_H__2C141601_46F6_11D4_B789_E25FABB88F0F__INCLUDED_)
