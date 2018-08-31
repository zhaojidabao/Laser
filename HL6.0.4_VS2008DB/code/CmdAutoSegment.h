// CmdAutoSegment.h: interface for the CCmdAutoSegment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDAUTOSEGMENT_H__96429CD0_E798_4A34_85FB_6E4F5BA56A22__INCLUDED_)
#define AFX_CMDAUTOSEGMENT_H__96429CD0_E798_4A34_85FB_6E4F5BA56A22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdAutoSegment : public CCmdBaseSave  
{
public:
	CCmdAutoSegment(CHLDoc *pDoc);
	virtual ~CCmdAutoSegment();
};

#endif // !defined(AFX_CMDAUTOSEGMENT_H__96429CD0_E798_4A34_85FB_6E4F5BA56A22__INCLUDED_)
