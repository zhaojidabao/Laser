// CmdPaste.h: interface for the CCmdPaste class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDPASTE_H__2C141603_46F6_11D4_B789_E25FABB88F0F__INCLUDED_)
#define AFX_CMDPASTE_H__2C141603_46F6_11D4_B789_E25FABB88F0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdPaste : public CCmdBaseSave  
{
public:
	CCmdPaste(CHLDoc *pDoc);
	virtual ~CCmdPaste();
};

#endif // !defined(AFX_CMDPASTE_H__2C141603_46F6_11D4_B789_E25FABB88F0F__INCLUDED_)
