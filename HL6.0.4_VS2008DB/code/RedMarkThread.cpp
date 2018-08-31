
// RedMarkThread.cpp: implementation of the RedMarkThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <process.h>
#include "baseMacro.h"
#include "RedMarkThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRITICAL_SECTION g_LockReadMark;

CRedMarkThread::CRedMarkThread()
{
	m_QuadList.RemoveAll();
	//初始化事件
	m_hEventThreadExit		 = 	::CreateEvent(NULL,TRUE,TRUE, _T("HF_RedExitMark"));
	m_hEventRedPreview	 = 	::CreateEvent(NULL,TRUE,FALSE,_T("HF_RedPreview"));
	m_hEventUpdateArray	 = 	::CreateEvent(NULL,TRUE,FALSE,_T("HF_DataUpdate"));

	m_bIsRed   = TRUE;
	m_bIsArray = FALSE;

	InitializeCriticalSection(&g_LockReadMark);
}

CRedMarkThread::~CRedMarkThread()
{
	//关闭事件
	CloseHandle(m_hEventRedPreview);
	CloseHandle(m_hEventThreadExit);
	CloseHandle(m_hEventUpdateArray);
	
	if ( m_hRedThread != NULL )
	{
		CloseHandle(m_hRedThread);
		m_hRedThread = NULL;
	}

	DeleteCriticalSection(&g_LockReadMark);
}



//
UINT CRedMarkThread::StartPreview(BOOL bIsRed, BOOL bIsArray)	//启动边框预览
{	
	m_bIsRed = bIsRed;
	m_bIsArray = bIsArray;
	
	ResetEvent(hStopWork);
	SetEvent(m_hEventRedPreview);
	ResetEvent(m_hEventThreadExit);
	if (m_hRedThread != NULL)
	{
		CloseHandle(m_hRedThread);
		m_hRedThread = NULL;
	}
	
	CHLDoc* pDoc = GetCurActiveDocument();
	if ( NULL == pDoc ) 
		return 0;
	
	g_DAT.SetLay(pDoc->GetLay());
	g_DAT.EnterMarkEnv();
	if ( m_bIsArray )
	{
		m_hRedThread = (HANDLE)::_beginthreadex( NULL, 	0, 
												 (PTHREAD_START)ArrayMarkThread, 
												 this, 
												 (unsigned)0, 
												 (unsigned*)NULL
												);
	}
	else
	{
		SetEvent(m_hEventUpdateArray);
		m_hRedThread = (HANDLE)::_beginthreadex(   NULL, 0, 
												   (PTHREAD_START)PreviewThread, 
												   this, 
												   (unsigned)0, 
												   (unsigned*)NULL
												);
	}
	
	if ( m_hRedThread == NULL )
	{
		ResetEvent(m_hEventRedPreview);
		SetEvent(m_hEventThreadExit);
	}
	
	return 0;
}




//停止边框预览 //关闭线程
UINT CRedMarkThread::StopPreview()
{
	ResetEvent(m_hEventRedPreview); //通知停止边框预览
	g_DAT.StopMark();
	g_DAT.LeaveMarkEnv();
	int i = 0;
	while (WAIT_OBJECT_0 != WaitForSingleObject(m_hEventThreadExit,0)) 
	{
		//等待预览线程退出
		i++;
		if (i>1000)
		{
			OutputDebugString(_T("Can't stop thread\n"));
			if (m_hRedThread != NULL)
			{
				TerminateThread(m_hRedThread,0);
				OutputDebugString(_T("Force terminate Red Thread\n"));
			}
			break;
		}
		Sleep(10);
	}
	if (m_hRedThread != NULL)
	{
		CloseHandle(m_hRedThread);
		m_hRedThread = NULL;
	}

	m_PosArray.RemoveAll();
	m_MarkArray.RemoveAll();
	
	m_bIsRed   = TRUE;
	m_bIsArray = FALSE;
	
	return 0;
}
		
void CRedMarkThread::SetQuadArray(CArray<SELQUAD ,SELQUAD> &QuadList) //在此加上同步
{
	RmLock lock;
	m_QuadList.RemoveAll();
	m_QuadList.Copy(QuadList);
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEventRedPreview,0))
 	{
 		g_DAT.StopMark();	
 	}

	SetEvent(m_hEventUpdateArray);
}



//预览打标线程
UINT CRedMarkThread::PreviewThread(LPVOID pParam)
{
	CRedMarkThread* pThread = (CRedMarkThread*)pParam;
	if ( !pThread )		return 0;
	
	SELQUAD quad;
	CStroke stroke;
	CDot	dot(0,0);
				
	while( TRUE )
	{
		if ( WAIT_TIMEOUT == WaitForSingleObject(pThread->m_hEventRedPreview,0) )
			break;

		pThread->GetMarkData();
		int  nCount = pThread->m_QuadMarkArray.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			quad = pThread->m_QuadMarkArray.GetAt(i);
			stroke.RemoveAllPoint();
			dot.x = quad.m_QD.left;
			dot.y = quad.m_QD.bottom;
			stroke.Add(dot);
			dot.x = quad.m_QD.right;
			dot.y = quad.m_QD.bottom;
			stroke.Add(dot);
			dot.x = quad.m_QD.right;
			dot.y = quad.m_QD.top;
			stroke.Add(dot);
			dot.x = quad.m_QD.left;
			dot.y = quad.m_QD.top;
			stroke.Add(dot);
			dot.x = quad.m_QD.left;
			dot.y = quad.m_QD.bottom;
			stroke.Add(dot);
			
			g_DAT.MarkStroke(&stroke,NULL,LINE_MARK);
			
			pThread->GetMarkData();
			nCount = pThread->m_QuadMarkArray.GetSize();
		}
		Sleep(1);
	}
	g_DAT.StopMark();
	SetEvent(pThread->m_hEventThreadExit);//通知线程退出
	
	return 0;
}


DWORD CRedMarkThread::GetMarkData(BOOL bFirstTouch) //在此加上同步
{
	if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hEventUpdateArray,0) )
	{
		g_DAT.StopMark();
		
		RmLock lock;
		m_QuadMarkArray.RemoveAll();
		m_QuadMarkArray.Copy(m_QuadList);
		
		g_DAT.SetRedPreview(m_bIsRed);	
		g_DAT.MarkStart(m_bIsRed ? ENU_RED : ENU_MARK,0,TRUE);
		
		ResetEvent(m_hEventUpdateArray);
	}
	
	return 0;
}

void CRedMarkThread::SetMarkStrk(CArray<ARRAYMARK, ARRAYMARK> &ArrayMark) //在此加上同步
{
	RmLock lock;
	m_PosArray.RemoveAll();
	m_PosArray.Append(ArrayMark);
	
	if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hEventRedPreview,0) )
  		g_DAT.StopMark();	
	
	SetEvent(m_hEventUpdateArray);
}

//
DWORD CRedMarkThread::GetArrayMarkData() //在此加上同步
{	
	if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hEventUpdateArray,0) )
	{	
		g_DAT.StopMark();
		RmLock lock;

		m_MarkArray.RemoveAll();
		m_MarkArray.Append(m_PosArray);

		g_DAT.SetRedPreview(m_bIsRed);	
		g_DAT.MarkStart(m_bIsRed ? ENU_RED : ENU_MARK ,0, TRUE);
		
		ResetEvent(m_hEventUpdateArray);
	}
	
	return 0;
}

//预览打标线程
UINT CRedMarkThread::ArrayMarkThread(LPVOID pParam)
{	
	CDot  dot(0,0);
	CStroke  stroke;
	CRedMarkThread* pThread = (CRedMarkThread*)pParam;
	////////////////////////////
	while (WAIT_OBJECT_0 == WaitForSingleObject(pThread->m_hEventRedPreview,0))
	{
		pThread->GetArrayMarkData();

		int nCount = pThread->m_MarkArray.GetSize();
		for ( int i = 0; i < nCount; i++ )
		{
			ARRAYMARK AryMark = pThread->m_MarkArray.GetAt(i);
		
			stroke.RemoveAllPoint();
			dot.x = AryMark.qd.left;
			dot.y = AryMark.qd.bottom;
			stroke.Add(dot);
			dot.x = AryMark.qd.right;
			dot.y = AryMark.qd.bottom;
			stroke.Add(dot);
			dot.x = AryMark.qd.right;
			dot.y = AryMark.qd.top;
			stroke.Add(dot);
			dot.x = AryMark.qd.left;
			dot.y = AryMark.qd.top;
			stroke.Add(dot);
			dot.x = AryMark.qd.left;
			dot.y = AryMark.qd.bottom;
			stroke.Add(dot);

			stroke.Rotate(AryMark.qd.Center(), AryMark.rota);			
			g_DAT.MarkStroke(&stroke,NULL,LINE_MARK);

			pThread->GetArrayMarkData();
			nCount = pThread->m_MarkArray.GetSize();
		}
		Sleep(1);
	}
	
	g_DAT.StopMark();
	SetEvent(pThread->m_hEventThreadExit);//通知线程退出
	
	return 0;
}	




