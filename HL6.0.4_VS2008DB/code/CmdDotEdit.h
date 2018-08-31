// CmdDotEdit.h: interface for the CCmdDotEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDOTEDIT_H__9DABA7DD_493D_4984_A25C_843B4E83D579__INCLUDED_)
#define AFX_CMDDOTEDIT_H__9DABA7DD_493D_4984_A25C_843B4E83D579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"
#include "XTracker.h"

class CCmdDotEdit : public CCmdBaseSave
{
public:
	CCmdDotEdit(CHLDoc *pDoc,CPoint &point,CWnd *pWnd);
	virtual ~CCmdDotEdit();
};

#endif // !defined(AFX_CMDDOTEDIT_H__9DABA7DD_493D_4984_A25C_843B4E83D579__INCLUDED_)
