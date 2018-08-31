// CmdDelete.h: interface for the CCmdDelete class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDDELETE_H__583122A3_46BF_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDDELETE_H__583122A3_46BF_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdDelete: public CCmdBaseSave  
{
public:
	CCmdDelete(CHLDoc *pDoc);
	virtual ~CCmdDelete();

};

#endif // !defined(AFX_CMDDELETE_H__583122A3_46BF_11D4_A483_00105A615525__INCLUDED_)
