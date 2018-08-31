// CmdArray.h: interface for the CCmdArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDARRAY_H__8DBD87A5_DCCA_11D4_A485_00105A615525__INCLUDED_)
#define AFX_CMDARRAY_H__8DBD87A5_DCCA_11D4_A485_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdArray : public CCmdBaseSave
{
public:
	CCmdArray(CHLDoc *pDoc, CArray<OBJPOS, OBJPOS> &dtArray, BOOL flag);
	virtual ~CCmdArray();
};

#endif // !defined(AFX_CMDARRAY_H__8DBD87A5_DCCA_11D4_A485_00105A615525__INCLUDED_)
