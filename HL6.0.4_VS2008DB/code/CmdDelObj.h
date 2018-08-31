// CmdDelObj.h: interface for the CCmdDelObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDELOBJ_H__AD1E1253_4EE5_47CC_A05C_761E7B848466__INCLUDED_)
#define AFX_CMDDELOBJ_H__AD1E1253_4EE5_47CC_A05C_761E7B848466__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdDelObj : public CCmdBaseSave  
{
public:
	CCmdDelObj(CHLDoc *pDoc, DWORD dwTree);
	virtual ~CCmdDelObj();

};

#endif // !defined(AFX_CMDDELOBJ_H__AD1E1253_4EE5_47CC_A05C_761E7B848466__INCLUDED_)
