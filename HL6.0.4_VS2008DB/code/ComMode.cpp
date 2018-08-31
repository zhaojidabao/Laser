// ComMode.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "ComMode.h"


// CComMode 对话框

IMPLEMENT_DYNAMIC(CComMode, CDialog)

CComMode::CComMode(CWnd* pParent /*=NULL*/)
	: CDialog(CComMode::IDD, pParent)
	, m_strSendData(_T(""))
	, m_strRevData(_T(""))
{
	m_nComMode		=0;
	m_bSockConnect	=FALSE;
	m_bComConnect	=FALSE;
}

CComMode::~CComMode()
{
}

void CComMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCOMM, m_CtrlTab);
	DDX_Text(pDX, IDC_EDIT1, m_strSendData);
	DDX_Text(pDX, IDC_EDIT2, m_strRevData);
}


BEGIN_MESSAGE_MAP(CComMode, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CComMode::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CComMode::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CComMode::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CComMode::OnBnClickedButtonSend)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCOMM, &CComMode::OnTcnSelchangeTabcomm)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TABCOMM, &CComMode::OnTcnSelchangingTabcomm)
	//ON_MESSAGE(WM_MARK_START,&CMarkNormalDlg::OnMarkStart)
	ON_MESSAGE(WM_SOCKETS,&CComMode::OnWmSockets)
	ON_MESSAGE(WM_SERIAL,OnRec)
END_MESSAGE_MAP()


// CComMode 消息处理程序

void CComMode::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_CtrlTab.GetCurSel();
	if (0 == nIndex)
	{
		GetComParam( );
		if (m_SerialPort.OpenCom(m_serialDCB))
			m_bComConnect=TRUE;
		else
			m_bComConnect=FALSE;
	} 
	else
	{
		m_sockDlg.UpdateData(TRUE);
		char szIP[20]={0};
		WideCharToMultiByte(CP_ACP,0,m_sockDlg.m_strIP,-1,szIP,20,NULL,NULL);
		BOOL nInit = m_sock.InitSock(GetSafeHwnd(),m_sockDlg.m_bServer,m_nMainID,szIP,m_sockDlg.m_nAddr,m_sockDlg.m_bSameIP);
		if (nInit)
		{
			m_bSockConnect = TRUE; 
		} 
		else
		{
			m_bSockConnect = FALSE;
		}
	}
	UpdateStatus();
}

void CComMode::OnBnClickedButtonDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_CtrlTab.GetCurSel();
	if (0 == nIndex)
	{
		if (m_SerialPort.CloseCom())
			m_bComConnect=FALSE;
	} 
	else
	{
		if (m_sock.CloseSock())
			m_bSockConnect=FALSE;
	}
	UpdateStatus();
}

void CComMode::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nIndex = m_CtrlTab.GetCurSel();
	char szData[1024]={0};
	WideCharToMultiByte(CP_ACP,0,m_strSendData,-1,szData,1024,NULL,NULL);
	switch (nIndex)
	{
	case 0:
		if (m_bComConnect)
			m_SerialPort.SendData(szData,strlen(szData));
		break;
	case 1:
		if (m_bSockConnect)
		{
			if (m_sockDlg.m_bServer)
				m_sock.SendData(szData,strlen(szData),m_nMainID,NULL,m_nListenID);
			else
				m_sock.SendData(szData,strlen(szData),m_nMainID);
		}
		break;
	}
}

void CComMode::OnBnClickedButtonSave()
{
	UpdateData(TRUE);
	m_comDlg.UpdateData(TRUE);
	m_sockDlg.UpdateData(TRUE);
	WriteCfgFile( );
}

void CComMode::OnTcnSelchangeTabcomm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	int nIndex = m_CtrlTab.GetCurSel();

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	m_CtrlTab.GetItem(nIndex, &tci);

	ASSERT(tci.lParam);
	CWnd *pWnd = (CWnd *)tci.lParam;

	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(TRUE);
	UpdateStatus( );
	*pResult = 0;
}

void CComMode::OnTcnSelchangingTabcomm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	int nIndex = m_CtrlTab.GetCurSel();

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	m_CtrlTab.GetItem(nIndex, &tci);

	ASSERT(tci.lParam);
	CWnd *pWnd = (CWnd *)tci.lParam;

	pWnd->ShowWindow(SW_HIDE);
	pWnd->EnableWindow(FALSE);
	UpdateStatus( );
	*pResult = 0;
}

BOOL CComMode::OnInitDialog()
{
	m_pMarkMgr = new CMarkMgr();
	CDialog::OnInitDialog();
	TC_ITEM tm;
	tm.mask=TCIF_PARAM|TCIF_TEXT;
	tm.lParam=(LPARAM)&m_comDlg;
	tm.pszText=new TCHAR[255];	
	VERIFY(m_comDlg.Create(CCom::IDD,&m_CtrlTab));
	_tcscpy_s(tm.pszText,10,_T("串口通讯"));
	m_CtrlTab.InsertItem(0,&tm);
	m_CtrlTab.SetWindowPos(&m_comDlg,10,10,0,0,TRUE);
	m_comDlg.SetWindowPos(NULL, 3, 23, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	m_comDlg.ShowWindow(SW_SHOW);
	m_comDlg.EnableWindow(TRUE);

	tm.lParam=(LPARAM)&m_sockDlg;
	_tcscpy_s(tm.pszText,10,_T("网口通讯"));
	VERIFY(m_sockDlg.Create(CSock::IDD,&m_CtrlTab));
	m_CtrlTab.InsertItem(1,&tm);
	m_CtrlTab.SetWindowPos(&m_sockDlg,10,10,0,0,TRUE);
	m_sockDlg.SetWindowPos(NULL, 3, 23, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	m_sockDlg.ShowWindow(SW_HIDE);
	m_sockDlg.EnableWindow(FALSE);

	delete tm.pszText;

	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(m_bComConnect);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(!m_bComConnect);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(!m_bComConnect);

	ReadCfgFile( );
	m_sockDlg.UpdateData(FALSE);
	m_comDlg.UpdateData(FALSE);

	UpdateStatus( );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CComMode::WriteCfgFile()
{
	CString strCom;
	CString strBandRate;
	CString strDataBit;
	CString strStopBit;
	CString strCheckBit;
	m_comDlg.m_ccom.GetLBText(m_comDlg.m_ccom.GetCurSel(),strCom);
	m_comDlg.m_cBandRate.GetLBText(m_comDlg.m_cBandRate.GetCurSel(),strBandRate);
	m_comDlg.m_cDataBit.GetLBText(m_comDlg.m_cDataBit.GetCurSel(),strDataBit);
	m_comDlg.m_cStopBit.GetLBText(m_comDlg.m_cStopBit.GetCurSel(),strStopBit);
	m_comDlg.m_cCheckBit.GetLBText(m_comDlg.m_cCheckBit.GetCurSel(),strCheckBit);

	m_nComMode=m_comDlg.m_bUsed+(m_sockDlg.m_bUsed?2:0);
	g_sys.WriteProfileInt(_T("ComInfo"),_T("Commode"),m_nComMode);
	g_sys.WriteIniString(_T("ComInfo"),_T("COM"),strCom.GetBuffer());
	g_sys.WriteIniString(_T("ComInfo"),_T("BandRate"),strBandRate.GetBuffer());
	g_sys.WriteIniString(_T("ComInfo"),_T("DataBit"),strDataBit.GetBuffer());
	g_sys.WriteIniString(_T("ComInfo"),_T("StopBit"),strStopBit.GetBuffer());
	g_sys.WriteIniString(_T("ComInfo"),_T("CheckBit"),strCheckBit.GetBuffer());

	g_sys.WriteIniString(_T("ComInfo"),_T("IP"),m_sockDlg.m_strIP.GetBuffer());
	g_sys.WriteProfileInt(_T("ComInfo"),_T("Address"),m_sockDlg.m_nAddr);
	g_sys.WriteProfileInt(_T("ComInfo"),_T("Server"),m_sockDlg.m_bServer);
	g_sys.WriteProfileInt(_T("ComInfo"),_T("SameIP"),m_sockDlg.m_bSameIP);
}

void CComMode::ReadCfgFile()
{
	CString strCom;
	CString strBandRate;
	CString strDataBit;
	CString strStopBit;
	CString strCheckBit;

	m_nComMode			=g_sys.GetProfileInt(_T("ComInfo"),_T("Commode"),0);
	strCom				=g_sys.GetIniString(_T("ComInfo"),_T("COM"),_T("COM1"));
	strBandRate			=g_sys.GetIniString(_T("ComInfo"),_T("BandRate"),_T("9600"));
	strDataBit			=g_sys.GetIniString(_T("ComInfo"),_T("DataBit"),_T("8"));
	strStopBit			=g_sys.GetIniString(_T("ComInfo"),_T("StopBit"),_T("1"));
	strCheckBit			=g_sys.GetIniString(_T("ComInfo"),_T("CheckBit"),_T("NONE"));

	m_sockDlg.m_strIP		=g_sys.GetIniString(_T("ComInfo"),_T("IP"),_T("127.0.0.1"));
	m_sockDlg.m_nAddr		=g_sys.GetProfileInt(_T("ComInfo"),_T("Address"),9001);
	m_sockDlg.m_bServer  	=g_sys.GetProfileInt(_T("ComInfo"),_T("Server"),0);
	m_sockDlg.m_bSameIP	    =g_sys.GetProfileInt(_T("ComInfo"),_T("SameIP"),0);

	m_comDlg.m_ccom.SetCurSel(m_comDlg.m_ccom.FindString(-1,strCom));
	m_comDlg.m_cBandRate.SetCurSel(m_comDlg.m_cBandRate.FindString(-1,strBandRate));
	m_comDlg.m_cDataBit.SetCurSel(m_comDlg.m_cDataBit.FindString(-1,strDataBit));
	m_comDlg.m_cStopBit.SetCurSel(m_comDlg.m_cStopBit.FindString(-1,strStopBit));
	m_comDlg.m_cCheckBit.SetCurSel(m_comDlg.m_cCheckBit.FindString(-1,strCheckBit));

	m_comDlg.m_bUsed=BOOL(m_nComMode&0x01);
	m_sockDlg.m_bUsed=BOOL(m_nComMode&0x02);
}

void CComMode::GetComParam( )
{
	CString strCom;
	CString strBandRate;
	CString strDataBit;
	CString strStopBit;
	CString strCheckBit;

	m_comDlg.m_ccom.GetLBText(m_comDlg.m_ccom.GetCurSel(),strCom);
	m_comDlg.m_cBandRate.GetLBText(m_comDlg.m_cBandRate.GetCurSel(),strBandRate);
	m_comDlg.m_cDataBit.GetLBText(m_comDlg.m_cDataBit.GetCurSel(),strDataBit);
	m_comDlg.m_cStopBit.GetLBText(m_comDlg.m_cStopBit.GetCurSel(),strStopBit);
	m_comDlg.m_cCheckBit.GetLBText(m_comDlg.m_cCheckBit.GetCurSel(),strCheckBit);

	m_serialDCB.hMsgWnd	=GetSafeHwnd();
	m_serialDCB.nPort	=m_comDlg.m_ccom.GetCurSel()+1;
	m_serialDCB.BaudRate=_tstoi(strBandRate);
	m_serialDCB.ByteSize=_tstoi(strDataBit);
	m_serialDCB.StopBits=(_tstoi(strStopBit)==1?0:2);
	m_serialDCB.Parity	=_tstoi(strCheckBit);
}

void CComMode::UpdateStatus()
{
	int nIndex = m_CtrlTab.GetCurSel();
	if (nIndex==0)
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(!m_bComConnect);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(!m_bComConnect);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(m_bComConnect);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(m_bComConnect);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(!m_bSockConnect);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(!m_bSockConnect);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(m_bSockConnect);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(m_bSockConnect);
	}
}

LRESULT CComMode::OnWmSockets(WPARAM wpara,LPARAM lpara)
{
	CString StrRec;
	CString strIp;
	if(HIWORD(wpara)==m_nMainID)
	{
		switch( LOWORD(wpara) )
		{
		case IDWM_SOCKET_ACCEPT:
			m_nListenID=LOWORD(lpara);
			DWORD dwMainIP;
			DWORD dwListenIP;
			m_sock.GetAssociatsSocketIP(m_nMainID,m_nListenID,&dwMainIP,&dwListenIP);

			m_bSockConnect = TRUE;
			break;
		case IDWM_SOCKET_RECEIVE:
			OnNetRec( );
			break;
		case IDWM_SOCKET_CLOSE:
			/*if(!m_sockDlg.m_bServer)*/ m_sock.CloseSock(m_nMainID,m_nListenID);
			m_bSockConnect = FALSE;
			UpdateStatus();
			break;
		}
	}
	return 0;
}

LRESULT CComMode::OnRec(WPARAM wParam,LPARAM lParam)
{
	char szBuff[255];
	BOOL bReturn;
	if(wParam==WP_ERROR)
	{
		AfxMessageBox(_T("数据丢失！"));
		return FALSE;
	}
	else
	{
		::Sleep(50);
		bReturn=m_SerialPort.ReadData(szBuff,255);
		if(!bReturn)
		{
			AfxMessageBox(_T("数据接收出错！"));
			return FALSE;
		}
		//RecvData(szBuff);
		m_strRevData=szBuff;
		UpdateData(FALSE);
	}
	UpdateData(FALSE);
	return TRUE;
}

void CComMode::NetComStop(void)
{
	if (m_bComConnect)
	{
		m_bComConnect=FALSE;
		m_SerialPort.CloseCom();
	}
	if (m_bSockConnect)
	{
		m_bSockConnect=FALSE;
		m_sock.CloseSock();
	}
}

void CComMode::OnNetRec( )
{
	char buff[8193];
	::ZeroMemory((void*) buff,sizeof(char)*8192);	
	if(m_sockDlg.m_bServer)
		m_sock.ReadData(buff,8192,m_nMainID,0,m_nListenID);
	else
		m_sock.ReadData(buff,8192,m_nMainID);
	m_strRevData=buff;
	UpdateData(FALSE);

	CString strRev=buff;
	
	////-----检测数据包是否符合规则----////
	CString strFlagB,strFlagE;
	strFlagB.Format(_T("%c"),0x02);
	strFlagE.Format(_T("%c"),0x03);
	int nFlagB= strRev.Find(strFlagB);
	int nFlagE= strRev.Find(strFlagE);
	int nLen= strRev.GetLength();
	if (nFlagB>=0 && nFlagE>0)
	{
		OutputDebugString(_T("数据格式符合规则！"));
		strRev= strRev.Mid(nFlagB+1, nFlagE-nFlagB-1);
		nLen  = strRev.GetLength();
	} 
	else
	{
		OutputDebugString(_T("数据格式不正确！\n"));
		::AfxMessageBox(_T("数据格式不正确"));
		return;
	}
	///////////////////////////////////////////////
	CString strErr;
	CString strFeedback;
	int nIdx = strRev.Find(_T("$"),0);
	if (nIdx<0)
	{
		strErr = _T("数据格式不正确！");
		::AfxMessageBox(strErr);
		strFeedback = _T("Receive_Error");
		//SendSocketMsg(strFeedback);
		return;
	}
	int nPos  = strRev.Find('_');
	if (nPos<0)  
	{
		strErr = _T("数据格式不正确！");
		::AfxMessageBox(strErr);
		strFeedback = _T("Receive_Error");
		//SendSocketMsg(strFeedback);
		return;
	}
	//

	CString strCmd = strRev.Mid(nIdx+1, nPos-nIdx-1);
	CString strPara = strRev.Mid(nPos+1);
	if (0 == strCmd.CompareNoCase(_T("Initialize")))   //初始化指令
	{
		CString strHsFile = _T("D:\\HSFILE\\") + strPara + _T(".hs");
		if (!PathFileExists(strHsFile))
		{
			strErr = _T("模板不存在！");
			::AfxMessageBox(strErr,MB_TOPMOST|MB_OKCANCEL);
			strFeedback = _T("Initialize_FALSE");	   //模板不存在
			//SendSocketMsg(strFeedback);
			return;
		}

		CHLApp* pHLApp = (CHLApp*) AfxGetApp();
		CHLDoc* pDoc;
		pDoc = (CHLDoc*) pHLApp->OpenDocumentFile(strHsFile);
		if (NULL == pDoc)
		{
			CString strErr = _T("打开模板失败！");
			AfxMessageBox(strErr, MB_TOPMOST|MB_OKCANCEL);
			strFeedback = _T("Initialize_FALSE");	   //打开模板失败
			//SendSocketMsg(strFeedback);
			return;
		} 
		else
		{
			strFeedback = _T("Initialize_OK");
			SendSocketMsg(strFeedback);
			m_pDoc = pDoc;
			m_pChain = &(m_pDoc->m_chain);
// 			m_pMarkMgr->SetLay(m_pDoc->GetLay());
// 			m_pMarkMgr->SetChain(m_pChain);
		}
	}
	else if(0 == strCmd.CompareNoCase(_T("Data")))  //数据指令
	{
		CString strTmp;
		int i=0;
		while (AfxExtractSubString(strTmp,(LPCTSTR)strPara,i,_T(',')))
		{
			CString strModelName;
			strModelName.Format(_T("%d"), i);
			if (!m_pChain->TextReplace(strTmp, strModelName))
			{
				strFeedback =  _T("Receive_ERROR");
				SendSocketMsg(strFeedback);
				strErr.Format(_T("替换变量[%s]失败"),strModelName);
				::AfxMessageBox(strErr);
				return;
			}
			i++;
		}
		strFeedback = _T("Receive_OK");
		SendSocketMsg(strFeedback);
		m_pDoc->UpdateAllViews(NULL);
	}
	else if (0 == strCmd.CompareNoCase(_T("MarkStart"))) //打标指令
	{
		//::SendMessage(GetSafeHwnd(),WM_MARK_START,0,0);
		//m_pMarkMgr->StartMark();
	}
	else
	{
		strFeedback = _T("Receive_Error");
		SendSocketMsg(strFeedback);
	}
	return;
}

void CComMode::SendSocketMsg(CString strMsg) 
{
// 	DWORD dwSended(0);
// 	int nListenSocketID(0);
// 
// #ifdef UNICODE
// 	USES_CONVERSION;
// 	CStringA strSend;
// 	strSend.Format(("%c%s%c"),0x02,W2A(strMsg),0x03);
// #else
// 	CString strSend;
// 	strSend.Format(_T("%c%s%c"),0x02,strMsg,0x03);
// #endif
// 
// 	return m_sock->SendData(strSend,strSend.GetLength(),m_nMainSocketID,&dwSended,nListenSocketID);

	int nIndex = m_CtrlTab.GetCurSel();
	char szData[1024]={0};
	WideCharToMultiByte(CP_ACP,0,strMsg,-1,szData,1024,NULL,NULL);
	switch (nIndex)
	{
	case 0:
		//m_SerialPort.SendData(szData,strlen(szData));
		break;
	case 1:
		m_sock.SendData(szData,strlen(szData),m_nMainID,NULL,m_nListenID);
		break;
	}
}

void CComMode::OnCancel()
{
	NetComStop();
	CDialog::OnCancel();
}