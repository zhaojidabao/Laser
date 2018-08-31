// CmdCopy.h: interface for the CCmdCopy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDCOPY_H__583122A6_46BF_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDCOPY_H__583122A6_46BF_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdCopy : public CCmdBaseSave  
{
public:
	CCmdCopy(CHLDoc *pDoc);
	virtual ~CCmdCopy();
};

#endif // !defined(AFX_CMDCOPY_H__583122A6_46BF_11D4_A483_00105A615525__INCLUDED_)
