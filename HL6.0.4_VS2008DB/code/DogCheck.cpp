// DogCheck.cpp: implementation of the CDogCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DogCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDogCheck::CDogCheck()
{
	m_bInit = FALSE;
	m_hModule = NULL;
	m_bDebugDog = FALSE;

	m_arrDogValid.RemoveAll();

#ifdef _UNICODE 
	#ifdef _DEBUG
		m_hModule = LoadLibrary(_T("FdichUD.dll"));
	#else
		m_hModule = LoadLibrary(_T("FdichU.dll"));
	#endif
#else
	#ifdef _DEBUG
		m_hModule = LoadLibrary(_T("FdichD.dll"));
	#else
		m_hModule = LoadLibrary(_T("Fdich.dll"));
	#endif
#endif

	if ( m_hModule == NULL )
	{
		m_bInit = FALSE;
		return;
	}

	m_pCheckAll3_1			 =  (CheckAll3_1)GetProcAddress(m_hModule, (LPCSTR)14);
	m_pCheckDogIsExist		 =  (CheckDogIsExist)GetProcAddress(m_hModule, (LPCSTR)2);
	m_pShowBindDlg			 =  (ShowBindDlg)GetProcAddress(m_hModule, (LPCSTR)3);
	m_pGetIdAndVerify		 =  (GetDogIdAndLmtVerifyCode)GetProcAddress(m_hModule, (LPCSTR)4);
	m_pSetDogRemoveProc  =  (SetDogRemoveProc)GetProcAddress(m_hModule, (LPCSTR)5);
	m_pGetTypeVerifyCode	 =  (GetTypeVerifyCode)GetProcAddress(m_hModule, (LPCSTR)6);
	m_pGetDogVer				 =  (GetDogVer)GetProcAddress(m_hModule, (LPCSTR)7);
	m_pGetDogTimeInfo		 =  (GetDogTimeInfo)GetProcAddress(m_hModule, (LPCSTR)8);
	m_pTimeCountStart		 =  (TimeCountStart)GetProcAddress(m_hModule, (LPCSTR)9);
	m_pTimeCountSaveCur	 =  (TimeCountSaveCur)GetProcAddress(m_hModule, (LPCSTR)10);
	m_pGetFeeFlag			 =  (GetFeeFlag)GetProcAddress(m_hModule, (LPCSTR)11);
	m_pSwitchDog				 =  (SwitchDog)GetProcAddress(m_hModule, (LPCSTR)13);
	m_pSetAppPath			 =  (SetAppPath)GetProcAddress(m_hModule, (LPCSTR)15);
	m_pGetTime2				 =  (GetDogTimeInfo2)GetProcAddress(m_hModule, (LPCSTR)16);
	
	if ( m_pCheckAll3_1			&& 
		m_pCheckDogIsExist		&& 
		m_pShowBindDlg			&& 
		m_pGetIdAndVerify		&&
		m_pSetDogRemoveProc && 
		m_pGetTypeVerifyCode && 
		m_pGetDogVer				&&
		m_pGetDogTimeInfo		&& 
		m_pTimeCountStart		&& 
		m_pTimeCountSaveCur && 
		m_pGetFeeFlag		   && 
		m_pSwitchDog	)
	{
		m_bInit = TRUE;
	}
	else
	{
		m_bInit = FALSE;
	}
}

CDogCheck::~CDogCheck()
{
	if ( m_hModule  !=  NULL )
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

BOOL CDogCheck::__CheckAll(DWORD dwSoftType)
{
	if ( !m_bInit )
	{
		AfxMessageBox(_T("Not find Fdich.dll or version is incorrect!"));
		return FALSE;
	}
	
	DWORD dwCode = 0;
	CString  strMsg;
	BOOL bCheckBind = TRUE;
	
#ifdef _SOFTMODE_EDITOR_
	bCheckBind = FALSE;
#endif
	
	CString strPath = g_sys.GetInstallPath();
	if ( m_pSetAppPath  !=  NULL )
		m_pSetAppPath(strPath);
	if(dwSoftType == 0x02||dwSoftType == 0x03){
		m_pCheckAll3_1(0x02, strMsg, bCheckBind, dwCode, m_arrDogValid);
		if(dwCode==FRC_FAIL_DOG_WRONG_TYPE){
			m_pCheckAll3_1(0x03, strMsg, bCheckBind, dwCode, m_arrDogValid);
		}
	}
	else{
		m_pCheckAll3_1(dwSoftType, strMsg, bCheckBind, dwCode, m_arrDogValid);
	}
	return  __CheckCode(dwCode, strMsg);
}

BOOL CDogCheck::__CheckDogIsExist(long nStyle)
{
	DWORD dwCode = 0;
	//
	m_pCheckDogIsExist(nStyle, dwCode);

	return ( dwCode == FRC_SUCCESS );
}

BOOL CDogCheck::__ShowBindDlg()
{
	CString strPath = g_sys.GetInstallPath();
	if ( m_pSetAppPath  !=  NULL )
		m_pSetAppPath(strPath);

	if ( m_pShowBindDlg  !=  NULL ) 
		m_pShowBindDlg();

	return TRUE;
}

void CDogCheck::__GetDogIdAndLmtVerifyCode(CString& strDogID, CString& strLmtVerifyCode)
{
	if ( m_pGetIdAndVerify  !=  NULL )
		m_pGetIdAndVerify(strDogID, strLmtVerifyCode);
}

void CDogCheck::__SetDogRemoveProc(pOnDogRemove dogRemoveProc, LPVOID lpContext)
{
	if ( m_pSetDogRemoveProc  !=  NULL )
	{
		m_pSetDogRemoveProc(dogRemoveProc, lpContext);
	}
}

BOOL CDogCheck::__CheckCode(DWORD dwCode, const CString& strMsg)
{
	switch(dwCode)
	{
	case FRC_SUCCESS://返回成功
		return TRUE;
	case FRC_SUCESS_WITH_USERID://带工号返回成功
		m_bDebugDog = TRUE;
		return TRUE;
	case FRC_SUCCESS_WITH_TIMESHOW://成功但限时使用，并提醒使用剩余时间
		//软件授权还允许运行%d天，需要联系大族客服人员，重新申请与(狗ID = %s, 校验码 = %s)匹配的许可授权补丁，以便到期后能继续使用
	case FRC_SUCCESS_WITH_INSPECT://成功但巡检提醒
		//设备已连续运行一年，需要联系大族客服人员，对设备进行维护检修，以确保安全正常运行，
		//并申请与(狗ID = %s, 校验码 = %s)匹配的软件授权检修确认补丁
		AfxMessageBox(strMsg);
		return TRUE;

	case FRC_FAIL_DOG_LIB_LOAD://载入软件狗操作动态库失败
	case FRC_FAIL_DOG_LIB_GUID://操作失败，HansLmtH.dll模块库类型不匹配
	case FRC_FAIL_DOG_LIB_VERSION://操作失败，HansLmtH.dll模块库版本至少是V1.1.0
		//模块库载入失败或模块库版本不正确，请检测软件安装是否正确
	case FRC_FAIL_DOG_EXECUTE://将初始狗转换成运行狗失败
		//软件狗操作异常，请检测狗连接是否正常，驱动是否安装正确
	case FRC_FAIL_DOG_TIMEOUT://试用时间已经用完
		//当前软件授权已到期，需要联系大族客服人员，重新申请与(狗ID = %s, 校验码 = %s)匹配的许可授权补丁，以便继续使用
	case FRC_FAIL_DOG_WRONG_TYPE:
		// != 0x00软件狗类型不匹配，请确认软件狗是否是该设备配套的软件狗，否则请联系大族客服人员
		// != 0x01软件狗类型不匹配，请确认软件狗是否是该设备配套的软件狗，否则，请确认软件，控制卡及软件狗是否需要升级
	case FRC_FAIL_DOG_CMP_CARD://与打标卡不对应
	case FRC_FAIL_DOG_FUNC_UNKNOW://狗操作中发生未知错误
	case FRC_FAIL_DOG_FUNC_NODOG://没有检测到软件狗
	case FRC_FAIL_DOG_FUNC_WRONGDOG://检测到软件狗，但不是合法的软件狗
		
	case FRC_FAIL_DOG_FUNC_CHECK_WRONG_BEFORE_EXECUTE://在Execute函数调用前没有成功调用过Check函数
	case FRC_FAIL_DOG_FUNC_LIB_L_VERSION://HansLmtL.dll模块库版本至少V1.1.0
		//模块库载入失败或模块库版本不正确，请检测软件安装是否正确
		
	case FRC_FAIL_DOG_FUNC_MOREDOG://插入的软件狗多于1个
	case FRC_FAIL_DOG_FUNC_WRONG_PARAM://函数传入参数不支持
		
	case FRC_FAIL_PARA_UNKNOW_CARDTYPE://未知的控制卡类型
	case FRC_FAIL_PARA_UNKONW_DOGTYPE://未知的狗类型
	
	case FRC_FAIL_CARD_LIB_LOAD://控制卡库加载失败
	case FRC_FAIL_CARD_FUNC_GET://取控制卡库接口函数失败
	case FRC_FAIL_CARD_NONE://无控制卡
	case FRC_FAIL_CARD_WRITE_NEW://写新卡失败
		//写卡失败，请检测控制卡的连接或坏损
	case FRC_FAIL_CARD_NEW://未绑定新卡，
	case FRC_FAIL_CARD_NOBIND://未检测到绑定狗的控制卡
	case FRC_FAIL_CARD_FEEFAIL://检测到多卡，但未开通一机多卡功能，请拔掉不需要的卡后重启软件
	case FRC_SURE_BINDREPEAT://确认将狗号重复绑定到卡?
		AfxMessageBox(strMsg);
		return FALSE;

	case FRC_FAIL_CANCEL:
	default:
		return FALSE;
	}
}

BOOL CDogCheck::__Dog()
{
	DWORD dwSoftType = 0x02;
	HMODULE hDChg = LoadLibrary( _T("DynChange.dll") );
	if ( hDChg )
	{
		typedef int (WINAPI *GETSOFTCODE)(char* pSzPwd,int nLen, DWORD* pCode);
		GETSOFTCODE getsoftcode = (GETSOFTCODE)GetProcAddress(hDChg, "GetSoftCode");
		if ( getsoftcode )
		{
			char szPwd[100] = {0};
			DWORD dwCode = 0x02;
			getsoftcode(szPwd,100,&dwCode);
			
			//
			if ( strcmp(szPwd,"Hi,You know!~@$") == 0 ) //密码校验,防止被恶意修改
				dwSoftType = dwCode;
		}
		FreeLibrary(hDChg);
	}

	BOOL bFlag = __CheckAll(dwSoftType);


#ifdef _SOFTMODE_EDITOR_
	if ( !bFlag  )
	{
		bFlag = __CheckAll(0x05);
	}
#endif

	if ( bFlag )
	{
		__RegRemoveProc();
	}
	return bFlag;
}

DWORD WINAPI OnDogRemove(LPVOID lpContext)
{
	CDogCheck *pDogCheck = (CDogCheck*)lpContext;
	do 
	{
		AfxMessageBox(_T("The dongle has been removed, Please insert the dongle!"));
		AfxGetMainWnd()->PostMessage(WM_QUIT, 0, 0);
		break;
	}
	while (!pDogCheck->__CheckDogIsExist(2));

	return 0;
}

void CDogCheck::__RegRemoveProc()
{
	__GetDogIdAndLmtVerifyCode(m_strDogID, m_strLmtVerifyID);
	m_pGetTypeVerifyCode(m_strTypeVerifyID);
	m_pGetDogVer(m_dwDogVerMain, m_dwDogVerSub);
	m_pGetDogTimeInfo(m_bTmSupport, m_strTmEsVerifyID, m_bTmEnable, m_dwTmUsed);
	__SetDogRemoveProc(&OnDogRemove, this);
}

BOOL CDogCheck::__DogTimeCountStart()
{
	if ( m_pTimeCountStart == NULL )
		return FALSE;

	return ( m_pTimeCountStart() == FRC_SUCCESS );
}

BOOL CDogCheck::__DogTimeCountSaveCur()
{
	if ( m_pTimeCountSaveCur == NULL )
		return FALSE;

	return ( m_pTimeCountSaveCur() == FRC_SUCCESS );
}

BOOL CDogCheck::__SwitchDog(const CString& strDogID)
{
	if ( m_pSwitchDog == NULL )
		return  FALSE;

	return ( m_pSwitchDog(strDogID) == FRC_SUCCESS );
}

BOOL  CDogCheck::__GetFeeFlag(BYTE* pFlag,int nSize)
{
	if ( m_pGetFeeFlag == NULL )
		return  FALSE;

	return ( m_pGetFeeFlag(pFlag,nSize) == FRC_SUCCESS );

}

void  CDogCheck::__GetTimeInfo2(CString& strClock,CString &strMax)
{
	if ( m_pGetTime2 == NULL )
		return;

	m_pGetTime2(strClock,strMax);
}