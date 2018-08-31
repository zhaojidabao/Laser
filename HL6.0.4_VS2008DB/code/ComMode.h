#pragma once
#include "afxcmn.h"
#include "Com.h"
#include "Sock.h"
#include "Include/SockDllDefine.h"
#include "MarkMgr.h"
#include "SerialPort.h"

// CComMode 对话框

class CComMode : public CDialog
{
	DECLARE_DYNAMIC(CComMode)

public:
	CComMode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComMode();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMUNICATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnTcnSelchangeTabcomm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTabcomm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnWmSockets(WPARAM wpara, LPARAM lpara);
	afx_msg LRESULT OnRec(WPARAM wParam,LPARAM lParam);
	void WriteCfgFile();
	void ReadCfgFile();
	void GetComParam();
	void RecvData(CString strRevData);
	void SendData(CString strData);
	void UpdateStatus();
	void NetComStop();
	void OnNetRec();
	void SendSocketMsg(CString strMsg); 

public:
	CSock m_sockDlg;
	CCom m_comDlg;
	CString m_strSendData;
	CString m_strRevData;
	CTabCtrl m_CtrlTab;
	int    m_nComMode;//0 :没有通讯；1为串口通讯；2：为网口通讯；3：串口网口通讯
	BOOL   m_bSockConnect;
	BOOL   m_bComConnect;

	SerialDCB	 m_serialDCB;
	CSerialPort  m_SerialPort;
	IMFCSockDll  m_sock;
	int			 m_nListenID;
	int			 m_nMainID;
	CHLDoc * m_pDoc;
	CChain * m_pChain;
 	CMarkMgr* m_pMarkMgr;	//打标管理类
protected:
	virtual void OnCancel();
};
