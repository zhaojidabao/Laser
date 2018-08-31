// CmdFilterDocPoint.h: interface for the CCmdFilterDocPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDFILTERDOCPOINT_H__1A4134E2_4A08_4328_985A_55D76BCEE0F5__INCLUDED_)
#define AFX_CMDFILTERDOCPOINT_H__1A4134E2_4A08_4328_985A_55D76BCEE0F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CCmdFilterDocPoint  : public CCmdBaseSave  
{
public:
	CCmdFilterDocPoint(CHLDoc *pDoc);
	virtual ~CCmdFilterDocPoint();
};

#endif // !defined(AFX_CMDFILTERDOCPOINT_H__1A4134E2_4A08_4328_985A_55D76BCEE0F5__INCLUDED_)
