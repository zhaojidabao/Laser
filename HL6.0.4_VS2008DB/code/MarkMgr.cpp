// MarkMgr.cpp: implementation of the CMarkMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "MarkMgr.h"
#include "Error.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CChain*		CMarkMgr::m_pChain = NULL;
CChain3D*	CMarkMgr::m_pChain3d = NULL;
DWORD		CMarkMgr::m_dwSingle = 0;
DWORD		CMarkMgr::m_dwCurPos = 0;
DWORD		CMarkMgr::m_dwMarkCount = 0;
DWORD		CMarkMgr::m_nMarkType = ENU_MARK;
BOOL			CMarkMgr::m_bLoopMark = FALSE;
int				CMarkMgr::m_nIntervalTime = 0;
BOOL			CMarkMgr::m_bPreProc = TRUE;

CRITICAL_SECTION  g_Lock;
HANDLE hMark			 =  ::CreateEvent(NULL,TRUE,FALSE,_T("7204E733-2C0D-44A6-A388-CAB3D9B701EA")); //��ʼ��꣬�·�����
HANDLE hStopWork	 =  ::CreateEvent(NULL,TRUE,FALSE,_T("BBAFEACD-28FF-4C00-AB3A-CD1F72F5156F"));
HANDLE hMarkSts		 =  ::CreateEvent(NULL,TRUE,FALSE,_T("2FFBC117-25B9-4493-9436-7403A21DA927")); //���ڴ�������Signal������No Signal
HANDLE hThreadRun	 =  ::CreateEvent(NULL,TRUE,FALSE,_T("23E0E4C5-42A8-4BD7-8C84-DB32B6B77109"));
HANDLE hStopAfterTouch = ::CreateEvent(NULL,TRUE,FALSE,_T("72CA2488-4732-4C44-A66E-9D0EB64483A9"));//��̤������������ֹ ZY

UINT Mark_Process(LPVOID lpPara)
{
	HWND hWnd = (HWND)lpPara;
	::SetEvent(hThreadRun);
	
	while( TRUE )
	{
		if( WaitForSingleObject(hThreadRun,0)  !=  WAIT_OBJECT_0 )	//����߳��źű��ⲿ�ͷ����˳�
			break;
		
		if( ( g_sys.m_nCardTouchModel == 0 ) && ( CMarkMgr::Touch(lpPara) == 0 ) ) //��λ�����IO�ź�
		{
			::ResetEvent(hMark);
			::ResetEvent(hStopAfterTouch);
			
			//�����̷߳���Ϣ
			::PostMessage(hWnd,WM_MARK_START,0,0);
		}
		else if( WaitForSingleObject(hMark,1) == WAIT_OBJECT_0 )//�˻������Ľ����괥��
		{
			::ResetEvent(hStopAfterTouch);
		}
		else//�����̤�ź�ĩ��������CONTINUE
		{
			continue;
		}
		
		HANDLE hEvents[2] = {0};
		hEvents[0] = hStopAfterTouch;
		hEvents[1] = hMark;
		
		//ʵ�ʴ������
		if( WAIT_OBJECT_0 == WaitForMultipleObjects(2, hEvents, FALSE, INFINITE) )//��̤������������ֹ ��ȫ�� �ĵ��źŲ�ƥ�� ��...
			continue;
		
		if( WaitForSingleObject(hThreadRun,0)  !=  WAIT_OBJECT_0 )
			break;
		
		if( 1 )//����ͬ����ʹ�÷�Χ
		{
			Lock tmp(&g_Lock);
			//
			SetEvent(hMarkSts);
			CMarkMgr::Mark(lpPara);
			CMarkMgr::m_dwCurPos = 0;
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMarkMgr::CMarkMgr()
{
	::InitializeCriticalSection(&g_Lock);
}

CMarkMgr::~CMarkMgr()
{
	::DeleteCriticalSection(&g_Lock);
}

void CMarkMgr::RunThread(LPVOID lPara)
{
	ResetEvent(hThreadRun);
	ResetEvent(hMark);
	ResetEvent(hMarkSts);
	ResetEvent(hStopWork);
	
	m_pChain = NULL;
	m_pChain3d = NULL;
	m_dwSingle = 0;
	
	AfxBeginThread(Mark_Process,lPara,THREAD_PRIORITY_HIGHEST);
}


void CMarkMgr::StopThread()
{
	while( TRUE )
	{
		if ( WaitForSingleObject(hMarkSts,0)  !=  WAIT_OBJECT_0 )
			break;
		
		::Sleep(20);
	}
	
	::ResetEvent(hThreadRun);
	::SetEvent(hMark);
	::Sleep(200);
	m_pChain = NULL;
	m_pChain3d = NULL;
}


BOOL CMarkMgr::SetChain(CChain *pChain)
{
	if ( pChain == NULL )
		return FALSE;

	Lock tmp(&g_Lock);
	////
	m_pChain = pChain;
	m_dwMarkCount = 1;
	m_dwMarkCount = m_pChain->GetCount();
	return TRUE;
}


/*
��̤��������ֵ���壺
	0-������ʾ��ʼ���
	9-����״̬
*/
UINT CMarkMgr::Touch(LPVOID lPara)
{
	UINT nRtn = 9;
	if ( g_DAT.Touch() )
		nRtn = 0;
	
	return nRtn;
}



UINT CMarkMgr::PreProcMark(LPVOID lPara)
{
	if ( m_pChain )
	{
		//ֻ������Ԥ����
		m_pChain->PreprocessMark(0x00);
	}
	
	return 0;
}


// ��꺯������ֵ���壺
// 0-�����������˳����˺�Ҫ����PreprocessMark����
// 1-�����ֹ�˳����˺�ֻ������ź��Ƿ��д���
UINT CMarkMgr::Mark(LPVOID lPara)
{
	UINT    nRtn = 0;
	BOOL   bPreview = FALSE;
	m_dwSingle = 0;
	double dbZOffset = g_DAT.m_bEnableZOffset ? g_SysCfg.m_dbZOffset : 0;
	if ( m_pChain )
	{
		m_bPreProc = m_pChain->PreprocessMark(0x05);
		///////
		switch( m_nMarkType )
		{
		case ENU_RED:
		case ENU_FRAME:
			bPreview = TRUE;
			g_DAT.SetRedPreview(TRUE);
			break;

		default:
			g_DAT.SetRedPreview(FALSE);
			break;
		}
		
		HWND hWnd = (HWND)lPara;
		if ( m_nMarkType  !=  ENU_FRAME )
		{
			if ( m_bLoopMark && m_bPreProc ) 
			{
				while( m_bLoopMark && m_bPreProc )
				{
//					while(::WaitForSingleObject(hMark,0)  !=  WAIT_OBJECT_0)
//					{
//						if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
//							break;
//						
//						Sleep(1);
//					}
					
					nRtn = g_DAT.MarkStart( m_nMarkType, 0, bPreview );
					if ( HLOK  !=  nRtn )
						break;
					
					CMarkMgr::m_dwCurPos = 0;
					if ( m_pChain3d )//3D���
					{
						m_pChain3d->Mark( hWnd, g_DAT.GetMarkFuncPtr(), dbZOffset );
					}
					else//��ͨƽ����
					{
						m_pChain->Mark(hWnd);
					}

					nRtn = g_DAT.MarkEnd();//�ȴ����δ�����
					m_dwSingle = g_DAT.GetMarkTime();
					if ( nRtn  !=  HLOK || ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
					{
						break;
					}
					
					if ( ENU_MARK == m_nMarkType )
					{
						//���û�е�ֹͣʱ���������һ�����ݵĴ���
						if ( WAIT_OBJECT_0  !=  ::WaitForSingleObject(hStopWork,0) )
						{
							m_bPreProc = m_pChain->PreprocessMark(0x01);
						}
					}
					
					DWORD dwStart = timeGetTime();
					for( int i = 0; i < m_nIntervalTime; i++ )
					{
						if ( !m_bLoopMark )
							break;
						
						DWORD dwEnd = timeGetTime();
						if ( (int)(dwEnd - dwStart)  >=  m_nIntervalTime )
							break;
						
						Sleep(1);
					}
					SendMessage(hWnd,WM_MARK_END,MARK_SEND,0);
				}
			}
			else if ( m_bPreProc )
			{
				for( int i = 0; i < (int)g_sys.m_nMarkRepeat; i++ )
				{
//					while(::WaitForSingleObject(hMark,0)  !=  WAIT_OBJECT_0)
//					{
//						if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
//							break;
//						
//						Sleep(1);
//					}
					
					nRtn = g_DAT.MarkStart( m_nMarkType,0,bPreview );
					if ( HLOK  !=  nRtn )
						break;
					
					CMarkMgr::m_dwCurPos = 0;
					if ( m_pChain3d )
					{
						m_pChain3d->Mark(hWnd, g_DAT.GetMarkFuncPtr(),dbZOffset);
					}
					else
					{
						m_pChain->Mark(hWnd);
					}
					
					nRtn = g_DAT.MarkEnd();//�ȴ����δ�����
					
					m_dwSingle = g_DAT.GetMarkTime();
					if ( nRtn !=  HLOK || ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
						break;
					
					if ( i < (int)(g_sys.m_nMarkRepeat-1) )
					{
						SendMessage(hWnd,WM_MARK_END,MARK_SEND,0);
						g_DAT.Between(hStopWork);
					}
				}
				
				if ( ENU_MARK == m_nMarkType )
				{
					//���û�е�ֹͣʱ���������һ�����ݵĴ���
					if ( WAIT_OBJECT_0  !=  ::WaitForSingleObject(hStopWork,0) && HLOK == nRtn )
					{
						m_bPreProc = m_pChain->PreprocessMark(0x01);
					}
				}
			}
		}
		else
		{
			if ( !m_pChain3d )//��ͨƽ��߿�Ԥ��
			{
				g_DAT.SetRedPreview(TRUE);
				int nRet = g_DAT.MarkStart(ENU_FRAME,0,bPreview);
				if ( HLOK == nRet )
				{
					m_pChain->PreTrack();
					g_DAT.MarkEnd(TRUE);
				}
			}
		}
		
		//���������
		if ( WAIT_OBJECT_0 == ::WaitForSingleObject(hStopWork,0) )
			nRtn = 1;
		
		SendMessage( hWnd,WM_MARK_END,MARK_TEND,  m_bPreProc ? 0 : 1 );
	}
	
	ResetEvent(hMark);
	ResetEvent(hStopWork);
	ResetEvent(hMarkSts);
	
	return nRtn;
}

//
DWORD CMarkMgr::GetSingleMarkTime()
{
	return m_dwSingle;
}

//
void CMarkMgr::StartMark()
{
	g_DAT.StartMark();
	::SetEvent(hMark);
	
	//�ȴ����ڴ��״̬ʱ�˳�
	while( TRUE )
	{
		if ( WAIT_OBJECT_0 == WaitForSingleObject(hMarkSts,5) )
			break;
	}
}


void CMarkMgr::PauseMark()
{
	::ResetEvent(hMark);
	//////
	g_DAT.PauseMark();
}



UINT CMarkMgr::ReMark()
{
	::SetEvent(hMark);
	/////
	return g_DAT.ReMark();
}

void CMarkMgr::StopMark()
{
	::SetEvent(hStopWork);
	::Sleep(60);
	
	g_DAT.StopMark();
}



BOOL CMarkMgr::GetRedPreview()
{
	return g_DAT.GetRedPreview();
}

void CMarkMgr::SetLay(const CLay &lay)
{
	g_DAT.SetLay(lay);
}

void CMarkMgr::SetMarkType(DWORD nType)
{
	m_nMarkType = nType;
}

BOOL CMarkMgr::MarkIsOver()
{
	//���ڴ������з���TRUE�����򷵻�FALSE
	return ( WaitForSingleObject(hMarkSts,0) == WAIT_OBJECT_0 );
}


void CMarkMgr::SetLoopMarkInfo(BOOL bLoopMark, int nIntervalTime)
{
	m_bLoopMark     = bLoopMark;
	m_nIntervalTime = nIntervalTime;
}


