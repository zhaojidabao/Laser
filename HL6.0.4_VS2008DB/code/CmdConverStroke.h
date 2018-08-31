// CmdConvertSelToOneTree.h: interface for the CCmdConverStroke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDCONVERTSELTOONETREE_H__3B2418BD_7C4E_4BF6_9C55_B8091D1C8BF3__INCLUDED_)
#define AFX_CMDCONVERTSELTOONETREE_H__3B2418BD_7C4E_4BF6_9C55_B8091D1C8BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdConverStroke : public CCmdBaseSave  
{
public:
	CCmdConverStroke(CHLDoc *pDoc);
	virtual ~CCmdConverStroke();

};

#endif // !defined(AFX_CMDCONVERTSELTOONETREE_H__3B2418BD_7C4E_4BF6_9C55_B8091D1C8BF3__INCLUDED_)
