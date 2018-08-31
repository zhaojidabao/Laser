// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__957408EF_B3F8_4F02_A462_3A36AE2BC7C6__INCLUDED_)
#define AFX_SERIALPORT_H__957408EF_B3F8_4F02_A462_3A36AE2BC7C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//设置input and output缓冲区
#define READLOCK    5120    //5K
#define MAXMBLOCK   5120	

#define WM_SERIAL   WM_USER+150
#define WP_RECEIVE   1
#define WP_ERROR     2

typedef struct SerialDCB
{
	HWND hMsgWnd;   //接收消息窗口句柄
	BYTE  nPort;    //串口号
    DWORD BaudRate; //波特率 
    BYTE ByteSize;  //数据位:   4-8 
    BYTE Parity;    //奇偶校验: 0-4=no,odd,even,mark,space 
    BYTE StopBits;  //停止位:   0,1,2 = 1, 1.5, 2 
	/////////////////////////////////
	SerialDCB::SerialDCB()
	{
		::ZeroMemory(this,sizeof(SerialDcb));
		nPort=1;
		BaudRate=9600;
		ByteSize=8;
		Parity=0;
		StopBits=0;
	};
}SerialDcb; 
 
class AFX_EXT_CLASS CSerialPort  
{
public:
	HANDLE m_hEventExit;

public:
	CSerialPort();
	virtual ~CSerialPort();

public:
	void ClearReceBuffer();
	BOOL OpenCom(const SerialDcb& dcb);
	BOOL CloseCom();
	BOOL SetComConfig(SerialDcb& dcb);
	BOOL SendData(void* szBuff,DWORD dwSize);
	DWORD  ReadData(void* pszBuff,DWORD dwSize);
	HANDLE GetComHandle(){ return m_hCom; };
	BOOL IsOpenSerial(){return (m_hCom!=NULL);};
	BOOL NotifyParent(UINT msg,WPARAM wParam,LPARAM lParam);

protected:
	BOOL AddDataToBuffer(void* pData,DWORD dwLength);
	BOOL OpenIOThread();
	BOOL ConfigSerial(const SerialDcb &dcb);
	BOOL CompareRetValue(void* pszCompare,DWORD dwLenght);

private:
	unsigned char m_szReceive[MAXMBLOCK];
	int   m_nCurIndex;
	HANDLE  m_hReadProc;
	HANDLE  m_hCom;			//串行口
	HWND  m_hMsgWnd;
};

#endif // !defined(AFX_SERIALPORT_H__957408EF_B3F8_4F02_A462_3A36AE2BC7C6__INCLUDED_)
