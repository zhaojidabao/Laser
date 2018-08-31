// CmdDelDot.h: interface for the CCmdDelDot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDELDOT_H__23CA8E55_8604_4D0B_984D_7214878144BF__INCLUDED_)
#define AFX_CMDDELDOT_H__23CA8E55_8604_4D0B_984D_7214878144BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdDelDot : public CCmdBaseSave
{
public:
	CCmdDelDot(CHLDoc *pDoc,CWnd *pWnd);
	virtual ~CCmdDelDot();
};

#endif // !defined(AFX_CMDDELDOT_H__23CA8E55_8604_4D0B_984D_7214878144BF__INCLUDED_)
