// CmdFill.h: interface for the CCmdFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdFill : public CCmdBaseSave
{
public:
	CCmdFill(CHLDoc *pDoc,FILLDATA fillnew,FILLDATA fillold);
	virtual ~CCmdFill();
};

#endif // !defined(AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_)
