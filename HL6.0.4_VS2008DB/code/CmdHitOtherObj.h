// CmdHitOtherObj.h: interface for the CCmdHitOtherObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDHITOTHEROBJ_H__97EC4526_DE3C_4287_B8C4_46D964B7A6A8__INCLUDED_)
#define AFX_CMDHITOTHEROBJ_H__97EC4526_DE3C_4287_B8C4_46D964B7A6A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdHitOtherObj :public CCmdBaseSave
{
public:
	CCmdHitOtherObj(CHLDoc *pDoc,CPoint &point,CWnd *pWnd);
	virtual ~CCmdHitOtherObj();
};

#endif // !defined(AFX_CMDHITOTHEROBJ_H__97EC4526_DE3C_4287_B8C4_46D964B7A6A8__INCLUDED_)
