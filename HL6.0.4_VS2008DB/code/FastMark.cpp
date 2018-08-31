// FastMark.cpp: implementation of the CFastMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FastMark.h"
#include <process.h>
#include "baseMacro.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFastMark::CFastMark()
{
	m_hThread = NULL;
	m_pDoc = NULL;
	m_bIsDownLoad = FALSE;
	
    m_hEventFastMark  = ::CreateEvent( NULL,TRUE,FALSE,_T("FastMark") );
}

CFastMark::~CFastMark()
{
	CloseHandle(m_hEventFastMark);
	if ( m_hThread  !=  NULL )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

//
UINT CFastMark::StartFastMark(HWND hWnd, BOOL IsDownLoad)
{	
	m_hWnd = hWnd;
	m_bIsDownLoad = IsDownLoad;

	ResetEvent(hStopWork);
	if ( m_hThread  !=  NULL )
	{
		TerminateThread(m_hThread,0);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
	m_pDoc = GetCurActiveDocument();
	if ( NULL == m_pDoc ) 
		return 0;
	
	g_DAT.SetLay(m_pDoc->GetLay());
	g_DAT.SetRedPreview(FALSE);
		
	m_hThread = (HANDLE)::_beginthreadex(  NULL,	0, 
										   (PTHREAD_START)MarkThread, 
										   this, 
									       (unsigned)0, 
										   (unsigned*)NULL
										);
	
	return 0;
}


//停止打标或下载，关闭线程
UINT CFastMark::StopFastMark()
{	
	SetEvent(hStopWork);
	Sleep(50);
	g_DAT.StopMark();
	
	if ( WAIT_TIMEOUT == WaitForSingleObject(m_hThread,1000)  && m_hThread )
	{
		TerminateThread(m_hThread,0);
	}

	if ( m_hThread )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	m_pDoc  = NULL;
	if ( m_hWnd  !=  NULL )
		::PostMessage(m_hWnd, WM_FASTMARK_END, 0, 0);
	
	return 0;
}




UINT CFastMark::MarkThread(LPVOID pParam)
{
	CFastMark* pFastMark  = (CFastMark*)pParam;
	if ( NULL == pFastMark )
		return 0;

	double dbZOffset = g_DAT.m_bEnableZOffset ? g_SysCfg.m_dbZOffset : 0;
	HWND hWnd = pFastMark->m_hWnd;

	int  nResult = HLOK;
	if ( pFastMark->m_bIsDownLoad )
		nResult = g_DAT.MarkStart(ENU_DOWNSINGLE,0,TRUE);	
	else
		nResult = g_DAT.MarkStart(ENU_MARK,0,TRUE);
	
	if ( HLOK  !=  nResult )
	{
		::PostMessage(hWnd, WM_FASTMARK_END, 0, 0);
		return 0;
	}
	
	if ( pFastMark->m_pDoc->m_b3D )
	{
		BOOL bSelMark = pFastMark->m_pDoc->m_chain.GetMarkSel();
		pFastMark->m_pDoc->m_chain3d.SetSelectedObjbeMarked(TRUE);
		pFastMark->m_pDoc->m_chain3d.Mark(hWnd, g_DAT.GetMarkFuncPtr(),dbZOffset);
		pFastMark->m_pDoc->m_chain3d.SetSelectedObjbeMarked(bSelMark);
	}
	else
	{
		if ( !pFastMark->m_pDoc->m_chain.FastMark(hWnd) )
		{
			::PostMessage(hWnd, WM_FASTMARK_END, 0, 0);
			return 0;
		}
	}
	
	UINT uSpaceRate = 0;
	if ( pFastMark->m_bIsDownLoad )
	{
		MARKEND_INFO stInfo;
		stInfo.nVer = 1;
		g_DAT.MarkEnd(TRUE, (UINT)&stInfo);
		if ( stInfo.nStatus )
		{
			uSpaceRate = stInfo.nFlashUsed;
			if ( WAIT_OBJECT_0  !=  WaitForSingleObject(hStopWork, 0) )
			{
				g_DAT.SetWorkStatus(0);
			}
		}
		else
			AfxMessageBox(_T("Download error!"));
	}
	else
	{
		g_DAT.MarkEnd(TRUE);
	}
	
	::PostMessage(hWnd, WM_FASTMARK_END, uSpaceRate, 0);
	return nResult;
}



