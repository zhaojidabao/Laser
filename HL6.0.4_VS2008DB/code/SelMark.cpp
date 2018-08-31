
// SelMark.cpp: implementation of the CSelMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "SelMark.h"
#include "baseMacro.h"
#include <Process.h>
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelMark::CSelMark()
{
	m_hThread   = NULL;
	m_pDoc      = NULL;
	m_bIsStop   = TRUE;
	m_nMarkMode = ENU_MARKSEL;
    m_hStopSelMark  = ::CreateEvent(NULL,TRUE,FALSE,_T("8ADF8CA3-4824-44DE-B5F7-282208E541B0"));
}



CSelMark::~CSelMark()
{
	CloseHandle(m_hStopSelMark);
	m_hStopSelMark = NULL;
	
	if ( m_hThread  )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}




//
UINT CSelMark::StartSelMark(HWND hWnd , SUBOBJMARKSET &stSub, int nMarkMode)
{
	m_bIsStop = FALSE;

	m_hWnd   = hWnd;
	m_stSub  = stSub;
	m_nMarkMode = nMarkMode;

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
	g_DAT.EnterMarkEnv();
	
	ResetEvent(m_hStopSelMark);
	m_hThread = (HANDLE)::_beginthreadex(	NULL,	0, 
											(PTHREAD_START)MarkThread, 
											this, 
											(unsigned)0, 
											(unsigned*)NULL
										);
	if ( m_hThread == NULL )
	{
		SetEvent(m_hStopSelMark);
	}

	return 0;
}




//Í£Ö¹±ß¿òÔ¤ÀÀ 
//¹Ø±ÕÏß³Ì
UINT CSelMark::StopSelMark()
{	
	SetEvent(m_hStopSelMark);
	g_DAT.StopMark();
	int i = 0;
	while ( !m_bIsStop ) 
	{
		if ( i > 1000 )
		{
			if (m_hThread  !=  NULL)
			{
				TerminateThread(m_hThread,1);
			}
			break;
		}
		Sleep(10);
		i++;
	}
	if (m_hThread  !=  NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	m_bIsStop = TRUE;
	m_pDoc    = NULL;

	if (m_hWnd  !=  NULL) 
	{
		::SendMessage(m_hWnd, WM_SELMARK_END, 0, 0);
	}
	
	return 0;
}



//
UINT CSelMark::MarkThread( LPVOID pParam )
{
	CSelMark* pSelMark = (CSelMark*)pParam;
	if ( !pSelMark )
		return 0;
	
	//
	g_DAT.MarkStart( ENU_MARK,0,TRUE );
	HWND hWnd = pSelMark->m_hWnd;
	pSelMark->m_pDoc->m_chain.SelMark( hWnd, pSelMark->m_hStopSelMark, pSelMark->m_stSub );
	
	g_DAT.MarkEnd();
	
	SetEvent(pSelMark->m_hStopSelMark);
	pSelMark->m_bIsStop = TRUE;
	
	::PostMessage(hWnd, WM_SELMARK_END, 0, 0);
	return 0;
}


