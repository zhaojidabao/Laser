// System.h: interface for the CSystemPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_)
#define AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Card.h"
#include "RightMng.h"

class CSystem
{	
public:
	CSystem();
	virtual ~CSystem();

public:
	void		FlashCardPara();
	static		CCard* GetCardHandle();
	CString	GetInstallPath();
	int			GetProfileInt(TCHAR* pSec,TCHAR* pKey,int nDefault);
	double	GetProfileDouble(TCHAR* pSec,TCHAR* pKey,double dbDefault);
	void		WriteProfileDouble(TCHAR* pSec,TCHAR* pKey,double dbValue);
	void		WriteProfileInt(TCHAR* pSec,TCHAR* pKey,int dbValue);
	CString GetIniString(TCHAR* pSec,TCHAR* pKey,TCHAR *pDefault);
	void      WriteIniString(TCHAR* pSec,TCHAR* pKey,TCHAR *pVal);
	BOOL		ReadSysCfg();       // ����ϵͳ������Ϣ
	void		WriteSysCfg();      // дϵͳ������Ϣ
	
	void		WriteRightInfo(CString &strFileName);
	void		ReadRightInfo(CString &strFileName);
	BOOL		AddUser(USERINFO &stUserInfo);
	BOOL		DelUser(CString &strUserName);
	void		EditUserInfo(USERINFO &stUserInfo);
	BOOL		GetUserInfoByName(CString strUserName, USERINFO &stUserInfo);
	void		EditUserInfoByUserName(USERINFO &stUserInfo, CString &strUserName);
	int			GetIndexByName(CString strUserName);
	BOOL    OnSetDefautUser(CString &strUserName,  BOOL bSel);
	BOOL    GetDefaultUserInfo(USERINFO &stUserInfo);
	BOOL    IsAvailUser(CString &strUserName, CString &strPassWord);
	void		WriteSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFileName);
	void		ReadSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFileName);
	UINT		ReadSysMarkPara();
	//
	BOOL		m_bStopPreview;			//�Ƿ�ֹͣԤ��
	int			m_nDibSampleMode;	//ͼ�������ʽ
	//�Ҷȷ����Բ�������
	BYTE		m_GrayCompensate[256];
	void		UpdateGrayCompensate();
	
	//�����ź�����
	UINT	m_nTouchTimes;			//ÿ�����źŴ���һ��
	BOOL	m_bMarkOnce;				//����ͬһ�����ϴ���
	UINT	m_nDelayBefore;			//��������ʱ
	UINT	m_nDelayAfter;			//�����ɺ���ʱ
	UINT	m_nDelayBetween;		//ÿ�δ���м���
	UINT	m_nDelayEnd;				//�����ź�
	UINT	m_nMarkRepeat;			//�����ظ�������
	UINT	m_bLoopMdt;				//�Ƿ�����ѭ���ѻ�
	int		m_nIOSelCount;			// IOѡ�ĵ��ź���
	int     m_nCardTouchModel;    //��̤�źŴ�����ʽ
	BOOL	m_bDeviceDebug;         //Device ������Ϣ���
	
	double m_dbMinCurrent;
	double m_dbMaxCurrent;		//
	
	CString m_strCaliPath;			//У���ļ���·��
	CString m_strCardFile;			//�������ļ���
	
	BOOL		m_bEnableUpdate;
	BOOL		m_bDrawCrossLine;
	BOOL		m_bFastDrawEnable;			//���ÿ��ٻ�ͼ
	double	m_dbFastDrawDistance;	//���ٳɻ�ͼ��С����

private:
	TCHAR	m_szFileName[MAX_PATH];	//ini�ļ�����
	TCHAR	m_szExePath[MAX_PATH];		//EXE�ļ�����Ŀ¼
	
	CRightMng m_RightMng;
};

extern CSystem g_sys;

#endif // !defined(AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_)





















