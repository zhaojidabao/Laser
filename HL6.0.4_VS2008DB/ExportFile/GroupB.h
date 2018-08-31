// GroupObjB.h: interface for the CGroupObjB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPOBJB_H__6D2CAB7D_AAD8_464B_9658_A96E20899941__INCLUDED_)
#define AFX_GROUPOBJB_H__6D2CAB7D_AAD8_464B_9658_A96E20899941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataBase.h"

class AFX_EXT_CLASS CGroupB  : public CDataBase
{
public:
	CGroupB();
	virtual ~CGroupB();

public:
	DWORD    m_dwObjId;
	CString  m_strObjName;

	int m_nFlag;
};

#endif // !defined(AFX_GROUPOBJB_H__6D2CAB7D_AAD8_464B_9658_A96E20899941__INCLUDED_)
