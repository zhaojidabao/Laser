 // DogCheck.h: interface for the CDogCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOGCHECK_H__B9F5BC4D_500F_4466_B367_DFE5DA8D725F__INCLUDED_)
#define AFX_DOGCHECK_H__B9F5BC4D_500F_4466_B367_DFE5DA8D725F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum FDICH_RETURN_CODE
{
	FRC_SUCCESS,//���سɹ�
	FRC_SUCESS_WITH_USERID,//�����ŷ��سɹ�
	FRC_SUCCESS_WITH_TIMESHOW,//�ɹ�����ʱʹ�ã�������ʹ��ʣ��ʱ��
	//�����Ȩ����������%d�죬��Ҫ��ϵ����ͷ���Ա������������(��ID = %s, У���� = %s)ƥ��������Ȩ�������Ա㵽�ں��ܼ���ʹ��
	FRC_SUCCESS_WITH_INSPECT,//�ɹ���Ѳ������
	//�豸����������һ�꣬��Ҫ��ϵ����ͷ���Ա�����豸����ά�����ޣ���ȷ����ȫ�������У�
	//��������(��ID = %s, У���� = %s)ƥ��������Ȩ����ȷ�ϲ���

	FRC_FAIL_DOG_LIB_LOAD,//���������������̬��ʧ��
	FRC_FAIL_DOG_LIB_GUID,//����ʧ�ܣ�HansLmtH.dllģ������Ͳ�ƥ��
	FRC_FAIL_DOG_LIB_VERSION,//����ʧ�ܣ�HansLmtH.dllģ���汾������V1.1.0
	//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ

	FRC_FAIL_DOG_EXECUTE,//����ʼ��ת�������й�ʧ��
	//����������쳣�����⹷�����Ƿ������������Ƿ�װ��ȷ

	FRC_FAIL_DOG_TIMEOUT,//����ʱ���Ѿ�����
	//��ǰ�����Ȩ�ѵ��ڣ���Ҫ��ϵ����ͷ���Ա������������(��ID = %s, У���� = %s)ƥ��������Ȩ�������Ա����ʹ��

	FRC_FAIL_DOG_WRONG_TYPE,
	// != 0x00��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵����������������ϵ����ͷ���Ա
	// != 0x01��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵��������������ȷ����������ƿ���������Ƿ���Ҫ����

	FRC_FAIL_DOG_CMP_CARD,//���꿨����Ӧ

	FRC_FAIL_DOG_FUNC_UNKNOW,//�������з���δ֪����
	FRC_FAIL_DOG_FUNC_NODOG,//û�м�⵽�����
	FRC_FAIL_DOG_FUNC_WRONGDOG,//��⵽������������ǺϷ��������

	FRC_FAIL_DOG_FUNC_CHECK_WRONG_BEFORE_EXECUTE,//��Execute��������ǰû�гɹ����ù�Check����
	FRC_FAIL_DOG_FUNC_LIB_L_VERSION,//HansLmtL.dllģ���汾����V1.1.0
	//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ

	FRC_FAIL_DOG_FUNC_MOREDOG,//��������������1��
	FRC_FAIL_DOG_FUNC_WRONG_PARAM,//�������������֧��

	FRC_FAIL_PARA_UNKNOW_CARDTYPE,//δ֪�Ŀ��ƿ�����
	FRC_FAIL_PARA_UNKONW_DOGTYPE,//δ֪�Ĺ�����

	FRC_FAIL_CARD_LIB_LOAD,//���ƿ������ʧ��
	FRC_FAIL_CARD_FUNC_GET,//ȡ���ƿ���ӿں���ʧ��
	FRC_FAIL_CARD_NONE,//�޿��ƿ�
	FRC_FAIL_CARD_WRITE_NEW,//д�¿�ʧ��
	//д��ʧ�ܣ�������ƿ������ӻ���

	FRC_FAIL_CARD_NEW,//δ���¿���
	FRC_FAIL_CARD_NOBIND,//δ��⵽�󶨹��Ŀ��ƿ�
	FRC_FAIL_CARD_FEEFAIL,//��⵽�࿨����δ��ͨһ���࿨���ܣ���ε�����Ҫ�Ŀ����������
	FRC_SURE_BINDREPEAT,//ȷ�Ͻ������ظ��󶨵���?
	FRC_FAIL_CANCEL,
};

typedef void (WINAPI *CheckAll)(DWORD dwDogType, CString& pStrReturn, DWORD dwCardType, DWORD &dwReturnCode);
//GetProcAddress(hModule, (LPCSTR)0x0001)
//dwDogType ��׼������0x02,CO2������0x01
//pStrReturn ���ش����ַ���
//dwCardType �����ͣ����bit��ʾ�Ƿ����ð󶨼��
//dwReturnCode ����������ȷ�����״̬��

typedef void (WINAPI *CheckAll3_1)(DWORD dwDogType, CString& pStrReturn, BOOL bCheckBind, DWORD &dwReturnCode, CStringArray& arrDogValid);
//GetProcAddress(hModule, (LPCSTR)14)
//dwDogType ��׼������0x02,CO2������0x01
//pStrReturn ���ش����ַ���
//bNeedBind �Ƿ���а󶨼��
//dwReturnCode ����������ȷ�����״̬��
//arrDogValid ���õĹ�

typedef void (WINAPI *CheckDogIsExist)(long nStyle, DWORD &dwReturnCode);
//GetProcAddress(m_hModule, (LPCSTR)0x0002)
//nStyle��
//		0��ʾ������⹷�Ƿ����
//		1��ʾ��⹷�Ĵ����Լ����Ƿ���ϴ���淶
//		2��ʾ��⹷�Ƿ�����Լ����Ƿ���ϴ���淶���Լ���⹷ID�Ƿ���ͬ 
//dwReturnCode ����������ȷ�����״̬��

typedef void  (WINAPI *ShowBindDlg)();
//�󶨶Ի���

typedef DWORD (WINAPI *GetDogIdAndLmtVerifyCode)(CString& strDogID, CString& strLmtVerifyCode);
												 //GetProcAddress(hModule, (LPCSTR)0x0004)
												 //strDogID �����ַ����������õ�����ID
												 //strLmtVerifyCode �����ַ��������õ��������Ƽ�����
												 
typedef DWORD (WINAPI *pOnDogRemove)(LPVOID lpContext);
typedef void (WINAPI *SetDogRemoveProc)(pOnDogRemove dogRemoveProc, LPVOID lpContext);

//���ûص��������������ε�ʱ����
//lpContext ����ص���������

typedef DWORD (WINAPI *GetTypeVerifyCode)(CString& strTypeVerifyID);
typedef DWORD (WINAPI *GetDogVer)(DWORD &dwMainVer, DWORD &dwSubVer);
typedef DWORD (WINAPI *GetDogTimeInfo)(	BYTE &bTmSupport, CString& strTmEsVerifyID,
										BYTE &bTmEable, DWORD &dwTmUsed);
typedef DWORD (WINAPI *TimeCountStart)();
typedef DWORD (WINAPI *TimeCountSaveCur)();
typedef DWORD (WINAPI *GetFeeFlag)(BYTE* pFlag,int nSize);

typedef DWORD (WINAPI *SwitchDog)(const CString& strDogID);//strDogID���л����Ĺ���
typedef void     (WINAPI *SetAppPath)(const CString & strPath);
typedef DWORD (WINAPI *GetDogTimeInfo2)(CString &strClockTime,CString &strMaxDate);

//���к�������ֵ�������壬��ʹ���������dwReturnCode������ֵ
//ע�⣬��������к�������Ҫ��ִ�е�һ��CheckAll�ɹ������ʹ��

DWORD WINAPI OnDogRemove(LPVOID lpContext);

#define WM_DOGREMOVE WM_USER+1

class CDogCheck  
{
private:
	HMODULE		m_hModule;

protected:
	CheckAll3_1				m_pCheckAll3_1;
	CheckDogIsExist		m_pCheckDogIsExist;
	ShowBindDlg			m_pShowBindDlg;
	SetDogRemoveProc	m_pSetDogRemoveProc;
	GetTypeVerifyCode	m_pGetTypeVerifyCode;
	GetDogVer				m_pGetDogVer;
	GetDogTimeInfo		m_pGetDogTimeInfo;
	TimeCountStart		m_pTimeCountStart;
	TimeCountSaveCur	m_pTimeCountSaveCur;
	GetFeeFlag				m_pGetFeeFlag;	
	SwitchDog				m_pSwitchDog;
	SetAppPath				m_pSetAppPath;
	GetDogTimeInfo2		m_pGetTime2;
	GetDogIdAndLmtVerifyCode m_pGetIdAndVerify;

public:
	CDogCheck();
	virtual ~CDogCheck();
	//////
	BOOL	__CheckAll(DWORD dwSoftType);
	BOOL	__CheckDogIsExist(long nStyle);
	BOOL	__ShowBindDlg();
	void	__GetDogIdAndLmtVerifyCode(CString& strDogID, CString& strLmtVerifyCode);
	void	__SetDogRemoveProc(pOnDogRemove dogRemoveProc, LPVOID lpContext);
	BOOL	__DogTimeCountSaveCur();
	BOOL	__DogTimeCountStart();
	void	__RegRemoveProc();
	BOOL	__Dog();
	BOOL	__CheckCode(DWORD dwCode, const CString& strMsg);
	BOOL    __SwitchDog(const CString& strDogID);
	BOOL	__GetFeeFlag(BYTE* pFlag,int nSize); 
	void   __GetTimeInfo2(CString& strClock,CString &strMax);

public:
	BOOL	m_bInit;
	CString	m_strDogID;
	CString	m_strLmtVerifyID;
	CString	m_strTypeVerifyID;
	DWORD	m_dwDogVerMain;
	DWORD	m_dwDogVerSub;
	BYTE	m_bTmSupport;
	CString	m_strTmEsVerifyID;
	BYTE	m_bTmEnable;
	DWORD	m_dwTmUsed;
	CStringArray m_arrDogValid;
	BOOL	m_bDebugDog;

};

#endif // !defined(AFX_DOGCHECK_H__B9F5BC4D_500F_4466_B367_DFE5DA8D725F__INCLUDED_)
