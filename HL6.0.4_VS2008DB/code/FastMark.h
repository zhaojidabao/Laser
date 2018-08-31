// FastMark.h: interface for the CFastMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASTMARK_H__798A1B5E_2FB9_47CC_88C3_907933190EC8__INCLUDED_)
#define AFX_FASTMARK_H__798A1B5E_2FB9_47CC_88C3_907933190EC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HLView.h"

class CFastMark  
{
public:
	CFastMark();
	virtual ~CFastMark();

public:
	static UINT MarkThread(LPVOID pParam);
	
	UINT StartFastMark(HWND hWnd, BOOL IsDownLoad = FALSE);
	UINT StopFastMark();
	
public:
	HANDLE m_hThread;
	HANDLE m_hEventFastMark;
	BOOL m_bIsDownLoad;
	HWND m_hWnd;
	
private:
	CHLDoc *m_pDoc;

};

#endif // !defined(AFX_FASTMARK_H__798A1B5E_2FB9_47CC_88C3_907933190EC8__INCLUDED_)
