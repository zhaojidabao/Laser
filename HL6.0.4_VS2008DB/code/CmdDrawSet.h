// CmdDrawSet.h: interface for the CCmdDrawSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDRAWSET_H__83BCA301_B6E9_11D4_A484_00105A615525__INCLUDED_)
#define AFX_CMDDRAWSET_H__83BCA301_B6E9_11D4_A484_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "CmdBaseSave.h"

class CCmdDrawSet : public CCmdBaseSave
{
public:
	CCmdDrawSet(CHLDoc *pDoc);
	virtual ~CCmdDrawSet();

};

#endif // !defined(AFX_CMDDRAWSET_H__83BCA301_B6E9_11D4_A484_00105A615525__INCLUDED_)
