// DownMgr.h: interface for the CDownMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNMGR_H__167AECED_B697_438B_B52F_AAB34144753E__INCLUDED_)
#define AFX_DOWNMGR_H__167AECED_B697_438B_B52F_AAB34144753E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MarkMgr.h"
#include "..\DllInterface\DownApiDefine.h"
#include "HLDoc.h"

#pragma warning(disable:4786)
#include <map>
using namespace std;

typedef map<CString, CHLDoc*> MAP_DOC;

class CDownMgr : public CMarkMgr  
{
public:
	CDownMgr();
	virtual ~CDownMgr();
	map<CString, CString> mapex;

public:
	void Run();
	static CDownMgr*		m_downMgr;
	static MAP_DOC			m_mapDoc;
	static HD_DATA_HEAD	m_data;
	static HWND				m_hWnd;
	static BOOL  CallFunction(UINT eFunc, LPVOID* pPara); 

	BOOL  m_bMandatory;	
};

#endif // !defined(AFX_DOWNMGR_H__167AECED_B697_438B_B52F_AAB34144753E__INCLUDED_)
