 // HL.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "HL.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "HLDoc.h"
#include "HLView.h"
#include "SetTrackerDlg.h"
#include "MarkMulDocDlg.h"
#include "MarkdataStatDlg.h"
#include "DlgGrayCompensate.h"
#include "OpenIoDefine.h"
#include "UserLogin.h"
#include "RedMarkThread.h"

#include "DownMgr5.3.h"
#include "GrayMgr.h"
#include "baseMacro.h"
#include "DlgChangeCard.h"
#include "DlgExtFunctionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDataMng g_dataMng;
typedef HRESULT (__stdcall* PDllRegisterServer)();
/////////////////////////////////////////////////////////////////////////////
// CHLApp

BEGIN_MESSAGE_MAP(CHLApp, CWinApp)
	//{{AFX_MSG_MAP(CHLApp)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SET_HARDWARE, OnSetHardware)
	ON_COMMAND(ID_OPTION, OnOption)
	ON_COMMAND(ID_FILE_MULDOC, OnFileMuldoc)
	ON_COMMAND(ID_MARK_STAT, OnMarkStat)
	ON_COMMAND(ID_CALIBRATE_NET, OnCalibrate)
	ON_COMMAND(IDM_GRAYCURRENT,OnGrayCurrent)
	ON_COMMAND(IDM_GRAYCOMPENSATE, OnGrayCompensate)
	ON_COMMAND(ID_SET_TESTIO, OnSetTestio)
	ON_COMMAND(ID_DOWN_MUL, OnDownMul)
	ON_COMMAND(ID_MARK_MUL, OnMarkMul) //
	ON_UPDATE_COMMAND_UI(ID_DOWN_MUL, OnUpdateDownMul)
	ON_UPDATE_COMMAND_UI(IDM_GRAYCURRENT, OnUpdateGraycurrent)
	ON_UPDATE_COMMAND_UI(IDM_GRAYCOMPENSATE, OnUpdateGraycompensate)
	ON_UPDATE_COMMAND_UI(ID_SET_HARDWARE, OnUpdateSetHardware)
	ON_COMMAND(ID_CALIBRATE, OnCalibrate)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)

	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_CHANGE_CARD, &CHLApp::OnChangeCard)
	ON_COMMAND(ID_EXTFUNCTION_INFO, &CHLApp::OnExtfunctionInfo)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CARD, &CHLApp::OnUpdateSelCard)
	ON_UPDATE_COMMAND_UI(ID_OPTION, &CHLApp::OnUpdateOption)
	ON_UPDATE_COMMAND_UI(ID_SET_TESTIO, &CHLApp::OnUpdateSetTestio)
	ON_COMMAND(ID_EYE_DISPLAY, &CHLApp::OnEyeDisplay)
	ON_COMMAND(IDM_HELP_CHS, &CHLApp::OnHelpChs)
	ON_COMMAND(IDM_HELP_ENGLISH, &CHLApp::OnHelpEnglish)
	
	ON_UPDATE_COMMAND_UI(ID_MARK_MUL, &CHLApp::OnUpdateMarkMul)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLApp construction

CHLApp::CHLApp()
{
	m_hLib       = NULL;
	m_hLangDLL   = NULL;
	m_pdlgSplash = NULL;

	m_pDlgWarning = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHLApp object
extern CArray<USERINFO, USERINFO> g_RightInfoList;

BOOL g_bAdminPrivilege;
CStringArray g_arryDisk;
CDogCheck   g_DogCheck;
CHLApp	    theApp;
/////////////////////////////////////////////////////////////////////////////
// CHLApp initialization
CONST char G_HL5VER[] = "HAN*S_LASER_A91ECCE4_AD85_4550_B8E8_8E9E5149B22D";


BOOL CHLApp::InitInstance()
{
	//ȷ�����԰汾
 	SetRegistryKey(_T("5.3ForXP"));
	GetValidLogicalDrive(g_arryDisk);

	//
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szFileName[MAX_PATH] = { 0 };
	
	GetExepath(szPath,MAX_PATH);
	_stprintf_s(szFileName,MAX_PATH, _T("%s%s"), szPath, _T("SysCfg.ini"));
		
	int nLng = ReadIniInt(szFileName, _T("LANG"), _T("LANG"),   _T("0"));
	switch( nLng )
	{
	case 0:
		g_LangID = 0x804;
		break;
	case 1:
		g_LangID = 0x404;
		break;
	case 2:
		g_LangID = 0x409;
		break;
	default:
		g_LangID = 0x804;
		break;
	}
	
	CString str;
#ifdef _DEBUG
	str.Format(_T("HL%x_D.DLL"),g_LangID);
#else
	str.Format(_T("HL%x.DLL"),g_LangID);
#endif
	
 	m_hLangDLL = AfxLoadLibrary(str);
 	if ( m_hLangDLL )
		AfxSetResourceHandle(m_hLangDLL); //�趨��Դ�ļ�
	else
		m_hLangDLL = AfxGetResourceHandle();

	g_bAdminPrivilege = IsProcessRunAsAdmin();
	BOOL bFirstRun = ReadIniInt(szFileName,_T("SysCfg"), _T("AUTOGUIDE"), _T("1") );
	if ( !g_bAdminPrivilege ) //�ǹ���ԱȨ��
	{
		BOOL bCheckPath = ReadIniInt(szFileName,_T("SysCfg"), _T("CheckPath"), _T("1") );
		if( bCheckPath )
		{
			BOOL bOK = FALSE;
			CString strLogic;
			for ( int i = 0; i < g_arryDisk.GetCount(); i++ )
			{
				CString str1 = g_arryDisk.GetAt(i).Left(1);
				CString str2 = szPath[0];
				if ( str1.CompareNoCase(str2) == 0  )
				{
					bOK = TRUE;
					break;
				}
				else
				{
					if ( i == 0 )
						strLogic += g_arryDisk.GetAt(i);
					else
						strLogic = strLogic + _T(" ,  ") + g_arryDisk.GetAt(i);
				}
			}
			if ( !bOK )
			{
				m_pDlgWarning = new CDlgRunWarn(NULL);
				m_pDlgWarning->Create(IDD_DIALOG_NOTADMIN,NULL);
				m_pDlgWarning->ShowWindow(SW_SHOW);
				m_pDlgWarning->UpdateWindow();
			}
		}
	}

#ifndef _SOFTMODE_EDITOR_
	HANDLE hMutexNew = ::OpenMutexA(MUTEX_ALL_ACCESS, TRUE, G_HL5VER);
	if ( NULL == hMutexNew )
	{
		hMutexNew = ::CreateMutexA(NULL, TRUE, G_HL5VER);
	}
	else
	{
		AfxMessageBox(IDS_ONLY_ONE_COPY);
		return FALSE;
	}

	m_pdlgSplash = new CDlgSplash(NULL);
 	m_pdlgSplash->Create(IDD_SPLASH, NULL);
 	m_pdlgSplash->ShowWindow(SW_SHOW);
 	m_pdlgSplash->UpdateWindow();
#endif	

	if( !UserLoginCheck() ) //�û���¼
	{ 
#ifndef _SOFTMODE_EDITOR_
		m_pdlgSplash->SendMessage(WM_CLOSE);
#endif
		return FALSE;
	}

	if( !g_sys.ReadSysCfg() )// ȡϵͳ������Ϣ
	{
#ifndef _SOFTMODE_EDITOR_
		m_pdlgSplash->SendMessage(WM_CLOSE);
#endif
		return FALSE;
	}

	if ( !::g_DAT.Init_SetLayerPara() )
	{
		AfxMessageBox(_T("Load Device.dll failure or dll version is incorrect!"));
#ifndef _SOFTMODE_EDITOR_
		m_pdlgSplash->SendMessage(WM_CLOSE);
#endif
		return FALSE;
	}

	//Initialize OLE libraries
	if ( !AfxOleInit() )
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
#ifndef _SOFTMODE_EDITOR_
		m_pdlgSplash->SendMessage(WM_CLOSE);
#endif
		return FALSE;
	}
	
	AfxEnableControlContainer();
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
#ifdef _SOFTMODE_EDITOR_
		IDR_HETYPE,
#else
		IDR_HLTYPE,
#endif
		RUNTIME_CLASS(CHLDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CHLView));
	AddDocTemplate(pDocTemplate);
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame();
#ifdef _SOFTMODE_EDITOR_
	if ( !pMainFrame->LoadFrame(IDR_EDITFRAME) )
		return FALSE;
#else
	if ( !pMainFrame->LoadFrame(IDR_MAINFRAME) )
	{
		m_pdlgSplash->SendMessage(WM_CLOSE);
		return FALSE;
	}
#endif

	m_pMainWnd = pMainFrame;	
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	
	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	RegistryFileType();


	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Parse command line for standard shell commands, DDE, file open
	// Dispatch commands specified on the command line
	if ( !ProcessShellCommand(cmdInfo) )
	{
#ifndef _SOFTMODE_EDITOR_
		m_pdlgSplash->SendMessage(WM_CLOSE);
#endif
		return FALSE;
	}

	::g_sys.ReadSysMarkPara();  // ȡϵͳ���������Ϣ
	/////////
	if ( -1 == LoadImageProcLib() )
	{
		AfxMessageBox(_T("Load ImageProc.dll failure,System will exit!"));
		goto EXIT_INIT;
	}

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_HIDE);

	if ( bFirstRun )
	{
		TCHAR szDest[MAX_PATH] = {0};
		TCHAR szSource[MAX_PATH]={0};
		_stprintf_s(szSource,MAX_PATH,_T("%s%s"),szPath,_T("MSCOMM32.OCX"));
		::GetSystemDirectory(szDest,MAX_PATH);
		if ( !::CopyFile(szSource,szDest,FALSE) )
		{
			CString strDebug;
			strDebug.Format(_T("Copy File failure, Source=%s, Destination=%s.\n"),szSource,szDest);
			OutputDebugString(strDebug);
		}

		HMODULE hMscommMod = ::LoadLibrary(TEXT("MSCOMM32.OCX"));
		if(  hMscommMod )
		{
			PDllRegisterServer RegServ = (PDllRegisterServer)GetProcAddress(hMscommMod,"DllRegisterServer");
			if( RegServ )
			{
				HRESULT hr = RegServ();
				if( FAILED(hr) ) // ʧ�����ֶ�ע��
				{
					::OutputDebugString(_T("DllRegisterServer failed,ShellExecute..."));
					::ShellExecute(NULL,_T("runas"),_T("RegMscomm.bat"),NULL,NULL,SW_HIDE);
				}
				::OutputDebugString(_T("DllRegisterServer success!"));
			}
			::FreeLibrary(hMscommMod);
		}
		else
		{
			::OutputDebugString(_T("Load mscomm32.ocx failed,ShellExecute..."));
			::ShellExecute(NULL,_T("runas"),_T("RegMscomm.bat"),NULL,NULL,SW_HIDE);
		}
		ShowNewFlag();
	}
	//
	::AfxBeginThread(CHLApp::ThreadCheckInit,this,THREAD_PRIORITY_NORMAL);
	return TRUE;

EXIT_INIT:
#ifndef _SOFTMODE_EDITOR_
	m_pdlgSplash->SendMessage(WM_CLOSE);
#endif
	PostMessage(pMainFrame->GetSafeHwnd(),WM_CLOSE,0,0);
	return TRUE;
}

BOOL CHLApp::GetValidLogicalDrive(CStringArray &arryDisk)
{
	TCHAR* pDriveName = NULL;
	TCHAR  szSysDir[MAX_PATH] = {0};
	GetSystemDirectory(szSysDir,MAX_PATH);

	BOOL bRet = FALSE;
	int nDriveNum = GetLogicalDriveStrings(0,NULL);
	if ( nDriveNum > 0 )
	{
		pDriveName = new TCHAR[nDriveNum+1];
		memset(pDriveName,0,(nDriveNum+1)*sizeof(TCHAR));
		if ( pDriveName != NULL )
		{
			GetLogicalDriveStrings(nDriveNum,pDriveName);

			for ( int i = 0; i < nDriveNum/4; i++ )
			{
				if (  DRIVE_FIXED == GetDriveType(&pDriveName[i*4]) )
				{
					if ( pDriveName[i*4] == _T('C') || pDriveName[i*4] == _T('c') || pDriveName[i*4] == szSysDir[0] )
						continue;
					arryDisk.Add(&pDriveName[i*4]);
				}
			}
			bRet = TRUE;

			delete[] pDriveName;
		}
	}
	return bRet;
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	BOOL m_bView;
	CString	m_strInfo;
	
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strVer;
	CString	m_strSpecial;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strVer = _T("");
	m_strSpecial = _T("");
	//}}AFX_DATA_INIT
	m_bView = FALSE;
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_VER, m_strVer);
	DDX_Text(pDX, IDC_STATIC_SPECIAL, m_strSpecial);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// App command to run the dialog
void CHLApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CHLApp::RegistryFileType()
{
	CString strBuf;
	HKEY hKey0, hKey1;
	DWORD dwSize(1024), dwType(0);
	TCHAR  szHDKey[] =  { _T("HD.Document") };
	//
	RegCreateKey(HKEY_CLASSES_ROOT, _T(".hd"), &hKey0);
	RegCreateKey(HKEY_CLASSES_ROOT,szHDKey, &hKey1);
	SHCopyKey(HKEY_CLASSES_ROOT, _T(".hs"), hKey0, NULL);
	SHCopyKey(HKEY_CLASSES_ROOT, _T("HS.Document"), hKey1, NULL);
	SHSetValue(hKey0, NULL, NULL, REG_SZ, szHDKey, _tcslen(szHDKey)*sizeof(TCHAR));
	SHSetValue(hKey1, NULL, NULL, REG_SZ, szHDKey, _tcslen(szHDKey)*sizeof(TCHAR));
	SHGetValue(HKEY_CLASSES_ROOT, _T("HD.Document\\DefaultIcon"), _T(""), &dwType, strBuf.GetBuffer(1024), &dwSize);
	strBuf.ReleaseBuffer();
	strBuf = strBuf.Left(strBuf.Find(_T(','))); 
	strBuf  +=  _T(",2");
	SHSetValue(HKEY_CLASSES_ROOT,_T("HD.Document\\DefaultIcon"), _T(""), REG_SZ, strBuf.GetBuffer(), strBuf.GetLength()*sizeof(TCHAR));
	strBuf.ReleaseBuffer();
	RegCloseKey(hKey0);
	RegCloseKey(hKey1);
}

void CHLApp::ShowNewFlag()
{
	CString strCmdLine = _T("");
	GetModuleFileName(NULL, strCmdLine.GetBuffer(MAX_PATH), MAX_PATH);
	strCmdLine.ReleaseBuffer();
	strCmdLine = strCmdLine.Left(strCmdLine.ReverseFind('\\'));
	CString strWorkFolder = strCmdLine+_T("\\");
	strCmdLine  +=  _T("\\HLGuide.exe");
	
	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize	 =   sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask		 =   SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd		 =   NULL;  
	ShExecInfo.lpVerb	 =   _T("open");
	ShExecInfo.lpFile		 =   strCmdLine; 

	ShExecInfo.lpParameters	 =  _T(""); 
	ShExecInfo.lpDirectory		 =   strWorkFolder;   
	ShExecInfo.nShow			 =   SW_NORMAL;  
	ShExecInfo.hInstApp			 =   NULL;              
	
	if ( ShellExecuteEx(&ShExecInfo) )
	{
		DWORD dwExitCode;
		MSG msg;
		while( WaitForSingleObject(ShExecInfo.hProcess,0)  !=  WAIT_OBJECT_0 )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if ( ( WM_PAINT == msg.message ) || (WM_PRINTCLIENT == msg.message) )
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				WaitMessage();
			}
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}
}


void CHLApp::OnFileOpen()
{
	CString strFileType;

#ifdef _SOFTMODE_EDITOR_
	strFileType = _T("HS Files(*.hs)|*.hs|HD Files(*.hd)|*.hd||");
#else
	if ( g_DAT.m_btExtendInfo.FunData.bNormal3d )
		strFileType = _T("HS Files(*.hs)|*.hs|HD Files(*.hd)|*.hd||");
	else
		strFileType = _T("HS Files(*.hs)|*.hs|");
#endif

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT/*|OFN_EXPLORER*/, strFileType);
	if ( dlg.DoModal()  !=  IDOK )
		return;

	CString strFile = dlg.GetPathName();
	CString strExt = dlg.GetFileExt().MakeLower();

	if ( strExt  !=  _T("hs") && strExt  !=  _T("hd") )
		return;	

	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = GetNextDocTemplate(pos);

	CDocument* pDoc = NULL;
	POSITION p1 = pTemplate->GetFirstDocPosition();
	while( p1 )
	{
		pDoc = pTemplate->GetNextDoc(p1);
		if (strFile == pDoc->GetPathName())
		{
			POSITION posOpenDoc = pDoc->GetFirstViewPosition();
			if (posOpenDoc  !=  NULL)
			{
				CView* pView = pDoc->GetNextView(posOpenDoc); // get first one
				ASSERT_VALID(pView);
				CFrameWnd* pFrame = pView->GetParentFrame();
				
				if ( pFrame == NULL )
					TRACE(traceAppMsg, 0, _T("Error: Can not find a frame for document to activate.\n") );
				else
				{
					pFrame->ActivateFrame();
					//////////
					if ( pFrame->GetParent()  !=  NULL )
					{
						CFrameWnd* pAppFrame;
						if ( pFrame  !=  (pAppFrame  = (CFrameWnd*)m_pMainWnd) )
						{
							ASSERT_KINDOF(CFrameWnd, pAppFrame);
							pAppFrame->ActivateFrame();
						}
					}
				}
			}
			return;
		}
	}
	
	pTemplate->OpenDocumentFile(strFile);
}

/////////////////////////////////////////////////////////////////////////////
// CHLApp message handlers

void CHLApp::OnSetHardware() 
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}
	if ( g_bIsSelMark ) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	TCHAR szPath[MAX_PATH] = { 0 };
	GetExepath(szPath,MAX_PATH);
	
	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize  =  sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask   =  SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd    =  NULL;  
	ShExecInfo.lpVerb  =  _T("open");
	CString str("");
#ifdef _DEBUG
	str.Format(_T("%sEmcc\\%s"),szPath,_T("SystemSet_D.exe"));
#else
	str.Format(_T("%sEmcc\\%s"),szPath,_T("SystemSet.exe"));
#endif
	
	ShExecInfo.lpFile  =  str;
	ShExecInfo.lpParameters  = _T("\\c systemset.cfg \\i SystemSet.ini"); 
	CString str1(_T(""));
	str1.Format(_T("%sEmcc\\"),szPath);
	ShExecInfo.lpDirectory		 =   str1;
	ShExecInfo.nShow			 =   SW_NORMAL;
	ShExecInfo.hInstApp			 =   NULL;
	
	if ( ShellExecuteEx(&ShExecInfo) )
	{
		DWORD dwExitCode;
		MSG msg;
		while( WaitForSingleObject(ShExecInfo.hProcess,0)  !=  WAIT_OBJECT_0 )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if (( WM_PAINT == msg.message ) || (WM_PRINTCLIENT == msg.message))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				WaitMessage();
			}
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}
	
	//���²���
	::g_sys.FlashCardPara(); 
}


//
void CHLApp::OnOption()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	SHELLEXECUTEINFO  ShExecInfo  =  {0};
	ShExecInfo.cbSize  =  sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask   =  SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd    =  NULL;
	ShExecInfo.lpVerb  =  _T("open");
	
	CString str;
#ifdef _DEBUG
	str.Format(_T("%s%s"),g_sys.GetInstallPath(),_T("SystemSet_D.exe"));
#else
	str.Format(_T("%s%s"),g_sys.GetInstallPath(),_T("SystemSet.exe"));
#endif
	
	ShExecInfo.lpFile  =  str; 
	ShExecInfo.lpParameters  = _T("\\c SysCfg.cfg \\i SysCfg.ini"); 
	CString str1 = g_sys.GetInstallPath();
	ShExecInfo.lpDirectory  =  str1;
	ShExecInfo.nShow		 =   SW_NORMAL;
	ShExecInfo.hInstApp		 =   NULL;
	
	BOOL bRunOK = ShellExecuteEx(&ShExecInfo); 
	if ( bRunOK )
	{
		DWORD dwExitCode;
		MSG msg;
		while( WaitForSingleObject(ShExecInfo.hProcess,0)  !=  WAIT_OBJECT_0 )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if ( ( WM_PAINT == msg.message ) || (WM_PRINTCLIENT == msg.message) )
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				WaitMessage();
			}
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}
	
	::g_sys.ReadSysCfg();
	CMDIChildWnd *pChildWnd = ((CMDIFrameWnd *)m_pMainWnd)->MDIGetActive();
	if ( pChildWnd )
	{
		CHLDoc *pDoc = (CHLDoc *)pChildWnd->GetActiveDocument();
		if ( pDoc )
		{
			pDoc->UpdatePerUnit();
			pDoc->UpdateAllViews(NULL);
		}
	}
}

//
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	TCHAR filename[_MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule,filename,_MAX_PATH);

	DWORD zero,size;
	size = GetFileVersionInfoSize(filename,&zero);
	if ( size > 0 )
	{
		LPVOID versionData = new TCHAR[size];
		GetFileVersionInfo(filename,zero,size,versionData);
		
		LPVOID textData;
		UINT  textSize = 0;
		VerQueryValue(versionData,_T("\\StringFileInfo\\080404b0\\ProductVersion"),&textData,&textSize);
		m_strVer = _T("Ver: ");
		m_strVer  +=  (TCHAR *)textData;
		
		VerQueryValue(versionData,_T("\\StringFileInfo\\080404b0\\SpecialBuild"),&textData,&textSize);
		m_strSpecial = (TCHAR *)textData;
		
		delete[] versionData;
		UpdateData(FALSE);
	}
	
#ifdef _SOFTMODE_EDITOR_
	SetDlgItemText( IDC_INFO, _T("Software 6.0 for Edit Model") );
#else
	CString strVer = g_DAT.GetCardVer();
	SetDlgItemText(IDC_INFO,strVer);
#endif
	
	//
	SetDlgItemText(IDC_STATIC_NUM,   _T("Serial Number: 150819000001109123"));//��׼����Ϣ
	SetDlgItemText(IDC_STATIC_CUSTOM,_T("Custom Serial: ------------------"));//���������Ϣ
	
	SetDlgItemText(IDC_STATIC_SOFTID,g_DogCheck.m_strDogID);
	SetDlgItemText(IDC_STATIC_LIMITCODE,g_DogCheck.m_strLmtVerifyID);
	SetDlgItemText(IDC_STATIC_TYPECODE,g_DogCheck.m_strTypeVerifyID);
	
	TCHAR szVersion[64] = {0};
	_stprintf_s(szVersion,64, _T("%d.%d"),g_DogCheck.m_dwDogVerMain, g_DogCheck.m_dwDogVerSub);
	SetDlgItemText(IDC_STATIC_DOG_VERSION,szVersion);

	CString strTime1,strTime2;
	g_DogCheck.__GetTimeInfo2(strTime2,strTime1);
	
	CString strYear = strTime1.Left(4);
	if ( _ttoi(strYear)  >=  2999 )
	{
		strTime1 = _T("No time limit");
	}
	SetDlgItemText(IDC_STATIC_DOG_TIME, strTime1);
	
	return TRUE;
}

//
int CHLApp::ExitInstance()
{
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}
	
	if ( g_bIsSelMark ) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if ( g_bIsFastMark )
	{
		theApp.m_FastMark.StopFastMark();
	}
	
	CloseHandle(hMark);
	CloseHandle(hStopWork);
	CloseHandle(hMarkSts);
	CloseHandle(hThreadRun);

	if ( m_hLib )
	{
		FreeLibrary(m_hLib);
		m_hLib = NULL;
	}
	
 	if ( m_hLangDLL )
 	{
 		AfxFreeLibrary(m_hLangDLL);
 	}
	
	return CWinApp::ExitInstance();
}



void CHLApp::OnMarkMul()
{
	if ( !g_DAT.IsCheckStatusOK() )
		return;

	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if ( g_bIsSelMark ) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if ( g_bIsFastMark ) 
	{
		theApp.m_FastMark.StopFastMark();
	}
	
	CMarkMulDocDlg dlg;
	g_DAT.m_hMarkWnd = NULL;	
	g_DAT.EnterMarkEnv();
	dlg.DoModal();
	g_DAT.LeaveMarkEnv();
}

void CHLApp::OnFileMuldoc() 
{
	// TODO: Add your command handler code here
	//�򿪶��ĵ�����
	CString str;
	CString strVer;
	static TCHAR szFilter[] = _T("Muldoc File(*.mul)|*.mul||");
	CFileDialog dlg(TRUE,_T("mul"),NULL,OFN_HIDEREADONLY,szFilter);
	
	str.LoadString(IDS_MULDOC_LOAD);
	dlg.m_ofn.lpstrTitle = str;
	if ( dlg.DoModal() == IDOK )
	{
		str = dlg.GetPathName();

		CHLDoc *pDoc = NULL;
		MULDOCFILE stMulDoc;
		if (stMulDoc.Serialize(str, TRUE)) 
		{
			for (int i = 0; i<stMulDoc.arName.GetSize(); ++i) 
			{
				CHLDoc *pDoc = (CHLDoc *)OpenDocumentFile(CString(stMulDoc.arName[i]));
				if ( pDoc ) 
				{
					pDoc->m_motor = stMulDoc.arOption[i];
				}
			}
		}
		else
		{
			AfxMessageBox(IDS_ERROR_OPENFILE);
		}
	}
}


//
void CHLApp::OnMarkStat() 
{
	CMarkdataStatDlg dlg;
	dlg.DoModal();
}

//
void CHLApp::OnCalibrate() 
{
	StartCalibrate();
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	dc.SetBkMode(TRANSPARENT );

	if ( m_bView )
		SetWindowText( _T("HAN'S") );
	else
		SetWindowText( _T("About") );
}


void CHLApp::StartCalibrate()
{
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}

	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize  =  sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask  =  SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd  =  NULL;  
	ShExecInfo.lpVerb  =  _T("open");

	TCHAR szPath[MAX_PATH] = { 0 };
	GetExepath(szPath,MAX_PATH);
	TCHAR szFileName[MAX_PATH] = { 0 };

#ifdef _DEBUG
	_stprintf_s(szFileName, 255,_T("%s%s\\%s"), szPath, g_SysCfg.m_szCardName, _T("Cali_D.exe"));
#else
	_stprintf_s(szFileName, 255,_T("%s%s\\%s"), szPath, g_SysCfg.m_szCardName, _T("Cali.exe"));
#endif

	ShExecInfo.lpFile		 =   szFileName;                          
	ShExecInfo.lpParameters =  NULL;              
	ShExecInfo.lpDirectory  =  szFileName;      
	ShExecInfo.nShow		 =   SW_NORMAL;  
	ShExecInfo.hInstApp		 =   NULL;              
		
	if ( ShellExecuteEx(&ShExecInfo) )
	{
		MSG msg;
		DWORD dwExitCode;
		while( WaitForSingleObject(ShExecInfo.hProcess,0)  !=  WAIT_OBJECT_0 )
		{
			//��δ�˳�
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if ( msg.message == WM_PAINT){
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
				WaitMessage();
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}

	g_DAT.Reset();
	g_sys.FlashCardPara();
}

void CHLApp::OnGrayCompensate() 
{
	CDlgGrayCompensate dlg;
	dlg.m_nDest0	 = 	::g_sys.GetProfileInt(_T("GrayCom"),_T("Dest0"),0);
	dlg.m_nDest1	 = 	::g_sys.GetProfileInt(_T("GrayCom"),_T("Dest1"),51);
	dlg.m_nDest2	 = 	::g_sys.GetProfileInt(_T("GrayCom"),_T("Dest2"),102);
	dlg.m_nDest3	 = 	::g_sys.GetProfileInt(_T("GrayCom"),_T("Dest3"),153);
	dlg.m_nDest4	 = 	::g_sys.GetProfileInt(_T("GrayCom"),_T("Dest4"),204);
	dlg.m_nDest5	 = 	::g_sys.GetProfileInt(_T("GrayCom"),_T("Dest5"),255);
	if (	  IDOK == dlg.DoModal() )
	{
		::g_sys.WriteProfileInt(_T("GrayCom"),_T("Dest0"),dlg.m_nDest0);
		::g_sys.WriteProfileInt(_T("GrayCom"),_T("Dest1"),dlg.m_nDest1);
		::g_sys.WriteProfileInt(_T("GrayCom"),_T("Dest2"),dlg.m_nDest2);
		::g_sys.WriteProfileInt(_T("GrayCom"),_T("Dest3"),dlg.m_nDest3);
		::g_sys.WriteProfileInt(_T("GrayCom"),_T("Dest4"),dlg.m_nDest4);
		::g_sys.WriteProfileInt(_T("GrayCom"),_T("Dest5"),dlg.m_nDest5);
		::g_sys.UpdateGrayCompensate();
	}
}

BOOL CHLApp::PreTranslateMessage(MSG* pMsg) 
{
	return CWinApp::PreTranslateMessage(pMsg);
}

int CHLApp::Run() 
{
	return CWinApp::Run();
}

void CHLApp::OnGrayCurrent()
{
	if ( !g_DAT.IsCheckStatusOK() )
		return;

	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}
	
	if ( g_bIsSelMark ) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if ( g_bIsFastMark ) 
	{
		theApp.m_FastMark.StopFastMark();
	}
	
	CHLDoc* pDoc = (CHLDoc* )GetCurActiveDocument();
	
	g_DAT.SetLay(pDoc->GetLay());
	g_DAT.EnterMarkEnv();
	CGrayMgr clsGray;
	clsGray.Run();

	g_DAT.LeaveMarkEnv();
}

UINT CHLApp::LoadImageProcLib()
{
	TCHAR szExePath[MAX_PATH] = {0};
	GetExepath(szExePath,MAX_PATH);

	CString strDllName(szExePath);
	strDllName  +=  _T("ImageProc.DLL");

	m_hLib = LoadLibrary(strDllName);
	if ( NULL == m_hLib )
	{
		return -1;
	}

	LoadDib		 =  (LPLOADDIB)GetProcAddress(m_hLib,"LoadDib");
	ProcDib		 =  (LPPROCDIB)GetProcAddress(m_hLib,"ProcDib");
	ProcGrayDib =  (LPPROCGRAYDIB)GetProcAddress(m_hLib,"ProcDib_Gray");
	DelDib		 =  (LPDELDIB)GetProcAddress(m_hLib,"DelDib");

	if ( LoadDib && ProcDib && ProcGrayDib && DelDib )
	{
		return 0; //�ɹ�
	}
	else
	{
		return -1; 
	}
}

void CHLApp::OnSetTestio() 
{
	// TODO: Add your command handler code here
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	CString strDll;
#ifdef _DEBUG 
	strDll = g_sys.GetInstallPath() + _T("HLIOCheck_D.dll");
#else
	strDll = g_sys.GetInstallPath() + _T("HLIOCheck.dll");
#endif

	BOOL bVerOK = FALSE;
	unsigned int dwSize = GetFileVersionInfoSize((LPTSTR)(LPCTSTR)strDll, 0);
	if ( dwSize ) 
	{
		TCHAR *szData = new TCHAR[dwSize];
		memset(szData, 0, MAX_PATH*sizeof(TCHAR));
		if ( GetFileVersionInfo(strDll, NULL, dwSize, szData) ) 
		{
			VS_FIXEDFILEINFO *VInfo = NULL;
			if (VerQueryValue(szData, _T("\\"), (LPVOID *)(&VInfo), &dwSize)) 
			{
				if ((VInfo->dwFileVersionMS  >=  0x00050003) &&
					(VInfo->dwFileVersionLS  >=  0x00010000)) 
				{
					bVerOK = TRUE;
				}	
			}
		}	
		delete []szData;
	}
	
	HMODULE hMod = LoadLibrary(strDll);
	if (FALSE == bVerOK || NULL == hMod) 
	{
		AfxMessageBox(IDS_DAMAGEFILE);
		FreeLibrary(hMod);
		return;
	}
	
	typedef  void (*T_ShowIODlg)();

	T_ShowIODlg dlgIO = (T_ShowIODlg)GetProcAddress(hMod, "ShowIODlg");
	if ( dlgIO  !=  NULL )
		dlgIO();
}

void CHLApp::OnDownMul() 
{
	// TODO: Add your command handler code here
	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_MULMARK))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if ( g_bIsSelMark ) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if ( g_bIsFastMark ) 
	{
		theApp.m_FastMark.StopFastMark();
	}
	
	CHLDoc* pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;

	g_DAT.SetLay(pDoc->GetLay());
	g_DAT.EnterMarkEnv();

	CDownMgr downMgr;
	downMgr.Run();
	if ( downMgr.m_bMandatory )
		g_DAT.SetWorkStatus(2);

	g_DAT.LeaveMarkEnv();
}

void CHLApp::OnUpdateDownMul(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	BOOL bEnabled = FALSE;
	CHLDoc *pDoc = NULL;
	CMultiDocTemplate *pDocTem = NULL;
	POSITION posTem = GetFirstDocTemplatePosition();
	while(posTem  !=  NULL)
	{
		pDocTem = (CMultiDocTemplate *)GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc  !=  NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			if (pDoc->HasObjForProcess())
			{
				bEnabled = TRUE;
				break;
			}
		}
	}
	pCmdUI->Enable(bEnabled);
#endif
}

void CHLApp::OnUpdateGraycurrent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) && TRUE == g_DAT.m_btExtendInfo.FunData.bGrayMark);
#endif
}

void CHLApp::OnUpdateGraycompensate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) && TRUE == g_DAT.m_btExtendInfo.FunData.bGrayMark);
#endif
}

void CHLApp::OnUpdateSetHardware(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET);	
#endif
}


void CHLApp::OnChangeCard()
{
	// TODO: �ڴ���������������
	if ( g_DogCheck.m_arrDogValid.GetCount()  <=  1)
		return;
	if ( g_DAT.GetCardNumber()  <=  1 )
		return;

	int  nIndex = g_DAT.GetCurCardIndex();

	CDlgChangeCard  dlg;	
	dlg.m_nCardIndex = nIndex;
	if ( IDCANCEL == dlg.DoModal() )
		return;

	//�����������ı�
	nIndex = dlg.m_nCardIndex;
	g_DAT.CloseCard();
	g_DAT.SetCardIndex(nIndex);

	if ( !g_DAT.InitCard() )
		AfxMessageBox(IDS_CHANGECARD_FAIL);
	else
		AfxMessageBox(IDS_CHANGECARD_SUCC);
}


void CHLApp::OnExtfunctionInfo()
{
	CDlgExtFunctionInfo dlg;
	dlg.DoModal();
}

void CHLApp::OnUpdateSelCard(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable((g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) && TRUE == g_DAT.m_btExtendInfo.FunData.bMulCard);
#endif
}

void CHLApp::OnUpdateOption(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET);
}

void CHLApp::OnUpdateSetTestio(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET);
#endif
}

void CHLApp::OnEyeDisplay()
{
	// TODO: �ڴ���������������
	ShowNewFlag();
}


BOOL CHLApp::IsProcessRunAsAdmin()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;

	BOOL  bOK = AllocateAndInitializeSid(
						&NtAuthority,
						2,
						SECURITY_BUILTIN_DOMAIN_RID,
						DOMAIN_ALIAS_RID_ADMINS,
						0, 0, 0, 0, 0, 0,
						&AdministratorsGroup);

	if ( bOK )
	{
		CheckTokenMembership(NULL, AdministratorsGroup, &bOK);
		FreeSid(AdministratorsGroup);
	}
	
	return  bOK;
}

void CHLApp::OnHelpChs()
{
	// TODO: �ڴ���������������
	CString strCmdLine = _T("");
	GetModuleFileName(NULL, strCmdLine.GetBuffer(MAX_PATH), MAX_PATH);
	strCmdLine.ReleaseBuffer();
	strCmdLine = strCmdLine.Left(strCmdLine.ReverseFind('\\'));
	CString strWorkFolder = strCmdLine+_T("\\");
	strCmdLine  +=  _T("\\UserManual(CN).pdf");

	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize	 =   sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask		 =   SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd		 =   NULL;  
	ShExecInfo.lpVerb	 =   _T("open");
	ShExecInfo.lpFile		 =   strCmdLine; 

	ShExecInfo.lpParameters	 =  _T(""); 
	ShExecInfo.lpDirectory		 =   strWorkFolder;   
	ShExecInfo.nShow			 =   SW_NORMAL;  
	ShExecInfo.hInstApp			 =   NULL;              

	if ( ShellExecuteEx(&ShExecInfo) )
	{
		DWORD dwExitCode;
		MSG msg;
		while( WaitForSingleObject(ShExecInfo.hProcess,0)  !=  WAIT_OBJECT_0 )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if ( ( WM_PAINT == msg.message ) || (WM_PRINTCLIENT == msg.message) )
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				WaitMessage();
			}
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}
}



void CHLApp::OnHelpEnglish()
{
	// TODO: �ڴ���������������
	CString strCmdLine = _T("");
	GetModuleFileName(NULL, strCmdLine.GetBuffer(MAX_PATH), MAX_PATH);
	strCmdLine.ReleaseBuffer();
	strCmdLine = strCmdLine.Left(strCmdLine.ReverseFind('\\'));
	CString strWorkFolder = strCmdLine+_T("\\");
	strCmdLine  +=  _T("\\UserManual(EN).pdf");
	
	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize	 =   sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask	 =   SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd		 =   NULL;  
	ShExecInfo.lpVerb	 =   _T("open");
	ShExecInfo.lpFile	 =   strCmdLine; 
	
	ShExecInfo.lpParameters	 =  _T(""); 
	ShExecInfo.lpDirectory		 =   strWorkFolder;   
	ShExecInfo.nShow			 =   SW_NORMAL;  
	ShExecInfo.hInstApp			 =   NULL;              
	
	if ( ShellExecuteEx(&ShExecInfo) )
	{
		DWORD dwExitCode;
		MSG msg;
		while( WaitForSingleObject(ShExecInfo.hProcess,0)  !=  WAIT_OBJECT_0 )
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if ( ( WM_PAINT == msg.message ) || (WM_PRINTCLIENT == msg.message) )
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				WaitMessage();
			}
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}
}

void CHLApp::OnUpdateMarkMul(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	BOOL bEnable = g_CurUserInfo.dwRight1 & USER_RIGHT_MULMARK;

	if ( !GetCurActiveDocument() )
	{
		bEnable = FALSE;
	}
	
#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif

	pCmdUI->Enable(bEnable);
}

BOOL CHLApp::UserLoginCheck()
{
	//��ȡ�û���Ϣ
	CString  strFileName(_T(""));
	g_sys.ReadRightInfo(strFileName);//ȡĬ���û���Ϣ
	
	int nCount = g_RightInfoList.GetSize();
	if ( 0 == nCount )
	{
		if ( IDNO  !=  AfxMessageBox(IDS_NOUSERINFO, MB_YESNO) ) 
			return FALSE;
	}
	
	USERINFO stUserInfo;
	BOOL bRet = g_sys.GetDefaultUserInfo(stUserInfo);
	if ( bRet )
	{
		::g_CurUserInfo = stUserInfo;
	}
	else
	{
		CUserLogin dlgLogin;
		if ( 0 == nCount )
			dlgLogin.m_dwDispMsg = 2;
		else
			dlgLogin.m_dwDispMsg = 1;
		
		if ( IDOK == dlgLogin.DoModal() )
		{
			if ( !dlgLogin.m_bIsOK )
				return FALSE;
		}
		else
		{
			return FALSE;
		}
	}

	return bRet;
}

BOOL CHLApp::InitCheckDog()
{
#ifdef _DEBUG	//
	g_DAT.CheckSoftSpFunc();
	return TRUE;
#endif 

	if ( !::g_DogCheck.__Dog() )
		return FALSE;
	
	g_DAT.CheckSoftSpFunc();
	
	#ifndef _SOFTMODE_EDITOR_	
		if ( ::g_DogCheck.m_bDebugDog )
		{
			g_DAT.SetCardIndex(0);
			CString strDog = g_DogCheck.m_arrDogValid.GetAt(0);
			g_DogCheck.__SwitchDog(strDog);
		}
		else
		{
			int nCardCount = g_DAT.GetCardNumber();
			int nDogCount  = g_DogCheck.m_arrDogValid.GetCount();
			if ( nDogCount == 0 ) //�޿��õĿ�
			{
				return FALSE;
			}
			else if ( nDogCount == 1 && nCardCount == 1 )
			{
				g_DAT.SetCardIndex(0);
				CString strDog = g_DogCheck.m_arrDogValid.GetAt(0);
				g_DogCheck.__SwitchDog(strDog);
			}
			else
			{
				CString strDog1,strDog2;
				for( int i = 0; i < nDogCount-1; i++ )
				{
					strDog1 = g_DogCheck.m_arrDogValid.GetAt(i);
					for( int j = i+1; j < nDogCount; j++ )
					{
						strDog2 = g_DogCheck.m_arrDogValid.GetAt(j);
						if ( strDog1.CompareNoCase(strDog2) == 0 )
						{
							AfxMessageBox(IDS_BINDMUCHCARD);
							return FALSE;
						}
					}
				}
				CDlgChangeCard dlg;
				dlg.m_nCardIndex = 0;
				if ( IDCANCEL == dlg.DoModal() )
					return FALSE;
				
				g_DAT.SetCardIndex(dlg.m_nCardIndex);
			}
		}
	#endif

	return TRUE;
}

int CHLApp::InitMachine()
{
	if ( !InitCheckDog() )
	{
		::OutputDebugString(_T("InitCheckDog failed ..."));
		return 1;
	}

	if ( !::g_DAT.InitCard() )////// Init card
	{
		if ( g_DAT.m_nInitStatus >= 1  )
		{
			return 2;
		}
		else //Init failure
		{
#ifndef _DEBUG
			return 3;
#endif
		}
	}

#ifndef _SOFTMODE_EDITOR_
	if ( TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark )
		::g_Motion.GoSoftHome(TRUE);
#endif

	return 0;
}

unsigned int CHLApp::ThreadCheckInit(LPVOID lpPara)
{
	//
	CHLApp* pApp = (CHLApp*)AfxGetApp();
	BOOL bRet = pApp->InitMachine();
	CWnd* pWnd = (CWnd*)(pApp->GetMainWnd());
	if( NULL != pWnd )
		pWnd->PostMessage(WM_START_MSG,bRet,0);
	return 0;
}
