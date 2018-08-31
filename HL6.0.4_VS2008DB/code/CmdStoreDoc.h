// CmdStoreDoc.h: interface for the CCmdStoreDoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSTOREDOC_H__0A84213F_45B9_4058_8FD0_3B373761A5FE__INCLUDED_)
#define AFX_CMDSTOREDOC_H__0A84213F_45B9_4058_8FD0_3B373761A5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdStoreDoc : public CCmdBaseSave  
{
public:
	CCmdStoreDoc(CHLDoc *pDoc);
	virtual ~CCmdStoreDoc();
};

#endif // !defined(AFX_CMDSTOREDOC_H__0A84213F_45B9_4058_8FD0_3B373761A5FE__INCLUDED_)
