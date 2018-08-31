#ifndef __SOCKDLLDEFINE2_H__
#define __SOCKDLLDEFINE2_H__
/////////////////////////////////////
#include "vMacro.h"
/////////////////////////////////////
//������Ϣ����
#define WM_SOCKETS				(WM_USER+57)
#define WM_VBSENDFILE           (WM_USER+50)	

#define IDWM_SOCKET_ACCEPT			0		//һ���µĿͻ���������������
#define IDWM_SOCKET_RECEIVE			1		//��������Ϣ��Ҫ��ȡ
#define IDWM_SOCKET_CLOSE			2		//�ر�ָ����SOCKET ��ر���������
//��չ��
#define IDWM_SOCKET_SEND_COMPLETED	3		//����֧�ִ����������ݴ���

//���շ��͵Ļ�������С
#define SOCKET_BUFSIZE		0x4000

////////////////////////////////
//��������Ӧ��Ϣ	WM_SOCKETS
/*
//LOWORD(LPARAM)==nID(��������ListenSocket's_ID), HIWORD(LPARAM)--Ԥ����չ
//LOWORD(WPARAM)==�����¼���Ϣ, HIWORD(WPARAM)==nID(��SOCKET's_ID)
switch( LOWORD(wParam) )
{
	case IDWM_SOCKET_ACCEPT:
		����Ϣ�Է�������Ч������֪ͨһ���µĿͻ�����

	case IDWM_SOCKET_RECEIVE:
		����Ϣ��Ҫ��ȡ������ReadData();

	case IDWM_SOCKET_CLOSE:
		SOCKET�ر�
	case IDWM_SOCKET_SEND_COMPLETED:
		֪ͨǰһ��SendData()�������������Ѵ������
}
*/

typedef BOOL (__stdcall* pfuncInitSock)(HWND,BOOL,int&,LPCSTR,UINT,BOOL);// 
typedef BOOL (__stdcall* pfuncReadSend)(LPCSTR,DWORD,int,DWORD*,int);
typedef BOOL (__stdcall* pfuncCloseSock)(int,int);
typedef BOOL (__stdcall* pfuncGetIP)(int,int,DWORD*,DWORD*);

class IMFCSockDll
{
public:
	IMFCSockDll(){ Clear(); }
	~IMFCSockDll(){ ReleaseLib(); }
	BOOL InitSock(HWND hWnd,BOOL bServer,int& nMainSocketID,LPCSTR lpsIP=NULL,UINT nPort=5000,BOOL bEnableMultiSameClients=FALSE)
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncInit(hWnd,bServer,nMainSocketID,lpsIP,nPort,bEnableMultiSameClients);
		//	return m_pfuncInit(hWnd,FALSE,nMainSocketID,"127.0.0.1",8888,bEnableMultiSameClients);

	}
	BOOL ReadData(LPCSTR lpsData,DWORD dwBuffLen,int nMainSocketID,DWORD* pdwReaded=NULL,int nListenSocketID=0)
	{//��������(�����ȡdwBuffLen-1���ַ�)
		if( !LoadLib() ) return FALSE;
		return m_pfuncRead(lpsData,dwBuffLen,nMainSocketID,pdwReaded,nListenSocketID);
	}
	BOOL SendData(LPCSTR lpsData,DWORD dwToSend,int nMainSocketID,DWORD* pdwSended=NULL,int nListenSocketID=0)//LPCTSTR
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncSend(lpsData,dwToSend,nMainSocketID,pdwSended,nListenSocketID);
	}
	BOOL CloseSock(int nMainSocketID=0,int nListenSocketID=0)
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncClose(nMainSocketID,nListenSocketID);
	}
	BOOL GetAssociatsSocketIP(int nMainSocketID,int nListenSocketID,DWORD* pdwMainSocketIP=NULL,DWORD* pdwListenSocketIP=NULL)
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncGetIP(nMainSocketID,nListenSocketID,pdwMainSocketIP,pdwListenSocketIP);
	}
private:
	void Clear()
	{
		m_hLib=NULL; m_pfuncInit=NULL; m_pfuncRead=NULL;
		m_pfuncSend=NULL; m_pfuncClose=NULL; m_pfuncGetIP=NULL;
	}
	void ReleaseLib()
	{
		if( m_hLib ) ::FreeLibrary(m_hLib);
		Clear();
	}
	BOOL LoadLib()
	{
		if( !m_hLib ) m_hLib=::LoadLibrary(TEXT("SockDll.dll"));
		if( !m_hLib )
		{
			Trace(_T("SockDll.dll�����ڻ�·������ȷ!"));
			return FALSE;
		}
	
		if( !m_pfuncInit )
			m_pfuncInit=GetPfunc(pfuncInitSock,m_hLib,"InitSockEx");
		if( !m_pfuncRead )
			m_pfuncRead=GetPfunc(pfuncReadSend,m_hLib,"ReadDataEx");
		if( !m_pfuncSend )
			m_pfuncSend=GetPfunc(pfuncReadSend,m_hLib,"SendDataEx");
		if( !m_pfuncClose )
			m_pfuncClose=GetPfunc(pfuncCloseSock,m_hLib,"CloseSockEx");
		if( !m_pfuncGetIP )
			m_pfuncGetIP=GetPfunc(pfuncGetIP,m_hLib,"GetAssociatsSocketIP");
		if( !m_pfuncInit || !m_pfuncRead || !m_pfuncSend ||
			!m_pfuncClose || !m_pfuncGetIP )
		{
			Trace(_T("SockDll.dll�汾����ȷ,��ѡ����ȷ�汾��SockDll.dll!"));
			ReleaseLib();
			return FALSE;
		}
		return TRUE;
	}
	//////////////
	pfuncInitSock m_pfuncInit;
	pfuncReadSend m_pfuncRead;
	pfuncReadSend m_pfuncSend;
	pfuncCloseSock m_pfuncClose;
	pfuncGetIP m_pfuncGetIP;
	HINSTANCE m_hLib;
};
/////////////////////////////////////
#endif
