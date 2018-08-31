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
	case FRC_SUCCESS://���سɹ�
		return TRUE;
	case FRC_SUCESS_WITH_USERID://�����ŷ��سɹ�
		m_bDebugDog = TRUE;
		return TRUE;
	case FRC_SUCCESS_WITH_TIMESHOW://�ɹ�����ʱʹ�ã�������ʹ��ʣ��ʱ��
		//�����Ȩ����������%d�죬��Ҫ��ϵ����ͷ���Ա������������(��ID = %s, У���� = %s)ƥ��������Ȩ�������Ա㵽�ں��ܼ���ʹ��
	case FRC_SUCCESS_WITH_INSPECT://�ɹ���Ѳ������
		//�豸����������һ�꣬��Ҫ��ϵ����ͷ���Ա�����豸����ά�����ޣ���ȷ����ȫ�������У�
		//��������(��ID = %s, У���� = %s)ƥ��������Ȩ����ȷ�ϲ���
		AfxMessageBox(strMsg);
		return TRUE;

	case FRC_FAIL_DOG_LIB_LOAD://���������������̬��ʧ��
	case FRC_FAIL_DOG_LIB_GUID://����ʧ�ܣ�HansLmtH.dllģ������Ͳ�ƥ��
	case FRC_FAIL_DOG_LIB_VERSION://����ʧ�ܣ�HansLmtH.dllģ���汾������V1.1.0
		//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ
	case FRC_FAIL_DOG_EXECUTE://����ʼ��ת�������й�ʧ��
		//����������쳣�����⹷�����Ƿ������������Ƿ�װ��ȷ
	case FRC_FAIL_DOG_TIMEOUT://����ʱ���Ѿ�����
		//��ǰ�����Ȩ�ѵ��ڣ���Ҫ��ϵ����ͷ���Ա������������(��ID = %s, У���� = %s)ƥ��������Ȩ�������Ա����ʹ��
	case FRC_FAIL_DOG_WRONG_TYPE:
		// != 0x00��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵����������������ϵ����ͷ���Ա
		// != 0x01��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵��������������ȷ����������ƿ���������Ƿ���Ҫ����
	case FRC_FAIL_DOG_CMP_CARD://���꿨����Ӧ
	case FRC_FAIL_DOG_FUNC_UNKNOW://�������з���δ֪����
	case FRC_FAIL_DOG_FUNC_NODOG://û�м�⵽�����
	case FRC_FAIL_DOG_FUNC_WRONGDOG://��⵽������������ǺϷ��������
		
	case FRC_FAIL_DOG_FUNC_CHECK_WRONG_BEFORE_EXECUTE://��Execute��������ǰû�гɹ����ù�Check����
	case FRC_FAIL_DOG_FUNC_LIB_L_VERSION://HansLmtL.dllģ���汾����V1.1.0
		//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ
		
	case FRC_FAIL_DOG_FUNC_MOREDOG://��������������1��
	case FRC_FAIL_DOG_FUNC_WRONG_PARAM://�������������֧��
		
	case FRC_FAIL_PARA_UNKNOW_CARDTYPE://δ֪�Ŀ��ƿ�����
	case FRC_FAIL_PARA_UNKONW_DOGTYPE://δ֪�Ĺ�����
	
	case FRC_FAIL_CARD_LIB_LOAD://���ƿ������ʧ��
	case FRC_FAIL_CARD_FUNC_GET://ȡ���ƿ���ӿں���ʧ��
	case FRC_FAIL_CARD_NONE://�޿��ƿ�
	case FRC_FAIL_CARD_WRITE_NEW://д�¿�ʧ��
		//д��ʧ�ܣ�������ƿ������ӻ���
	case FRC_FAIL_CARD_NEW://δ���¿���
	case FRC_FAIL_CARD_NOBIND://δ��⵽�󶨹��Ŀ��ƿ�
	case FRC_FAIL_CARD_FEEFAIL://��⵽�࿨����δ��ͨһ���࿨���ܣ���ε�����Ҫ�Ŀ����������
	case FRC_SURE_BINDREPEAT://ȷ�Ͻ������ظ��󶨵���?
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
			if ( strcmp(szPwd,"Hi,You know!~@$") == 0 ) //����У��,��ֹ�������޸�
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