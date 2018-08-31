// CmdSelGroupByNum.h: interface for the CCmdSelGroupByNum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSELGROUPBYNUM_H__C4DD9849_00AD_4049_A4F3_7DB85A77C6D9__INCLUDED_)
#define AFX_CMDSELGROUPBYNUM_H__C4DD9849_00AD_4049_A4F3_7DB85A77C6D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdSelGroupByNum : public CCmdBaseSave  
{
public:
	CCmdSelGroupByNum(CHLDoc *pDoc,int nNum,CWnd *pWnd);
	virtual ~CCmdSelGroupByNum();
};

#endif // !defined(AFX_CMDSELGROUPBYNUM_H__C4DD9849_00AD_4049_A4F3_7DB85A77C6D9__INCLUDED_)
