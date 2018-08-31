// CmdExtendObj.h: interface for the CCmdExtendObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDEXTENDOBJ_H__4417E5B3_7C8F_474A_8C53_6BC4EDAB789E__INCLUDED_)
#define AFX_CMDEXTENDOBJ_H__4417E5B3_7C8F_474A_8C53_6BC4EDAB789E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdExtendObj  : public CCmdBaseSave
{
public:
	CCmdExtendObj(CHLDoc *pDoc, double dbSpace,BOOL bIsKillOldObj);
	virtual ~CCmdExtendObj();
};

#endif // !defined(AFX_CMDEXTENDOBJ_H__4417E5B3_7C8F_474A_8C53_6BC4EDAB789E__INCLUDED_)
