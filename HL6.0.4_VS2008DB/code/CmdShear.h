// CmdShear.h: interface for the CCmdShear class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSHEAR_H__2CD25BB4_F8CC_4A19_8CE5_9AEE18F9FBC4__INCLUDED_)
#define AFX_CMDSHEAR_H__2CD25BB4_F8CC_4A19_8CE5_9AEE18F9FBC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"

class CCmdShear : public CCommand  
{
public:
	CCmdShear(CHLDoc *pDoc,CDot dot,double dbAngleX,double dbAngleY );
	virtual ~CCmdShear();
	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc * m_pDoc;
	CDot	m_dotBase;
	double	m_dbAngleX;
	double	m_dbAngleY;
	//////
	BOOL	m_bFirst;
	BOOL*	m_pFlag;
};

#endif // !defined(AFX_CMDSHEAR_H__2CD25BB4_F8CC_4A19_8CE5_9AEE18F9FBC4__INCLUDED_)
