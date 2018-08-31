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
	BOOL		ReadSysCfg();       // 加载系统配置信息
	void		WriteSysCfg();      // 写系统配置信息
	
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
	BOOL		m_bStopPreview;			//是否停止预览
	int			m_nDibSampleMode;	//图像采样方式
	//灰度非线性补偿数组
	BYTE		m_GrayCompensate[256];
	void		UpdateGrayCompensate();
	
	//触发信号设置
	UINT	m_nTouchTimes;			//每多少信号触发一次
	BOOL	m_bMarkOnce;				//不在同一物体上打多次
	UINT	m_nDelayBefore;			//触发后延时
	UINT	m_nDelayAfter;			//打标完成后延时
	UINT	m_nDelayBetween;		//每次打标中间间隔
	UINT	m_nDelayEnd;				//结束信号
	UINT	m_nMarkRepeat;			//整体重复打标次数
	UINT	m_bLoopMdt;				//是否启用循环脱机
	int		m_nIOSelCount;			// IO选文档信号数
	int     m_nCardTouchModel;    //脚踏信号触发方式
	BOOL	m_bDeviceDebug;         //Device 调试信息输出
	
	double m_dbMinCurrent;
	double m_dbMaxCurrent;		//
	
	CString m_strCaliPath;			//校正文件夹路径
	CString m_strCardFile;			//卡设置文件名
	
	BOOL		m_bEnableUpdate;
	BOOL		m_bDrawCrossLine;
	BOOL		m_bFastDrawEnable;			//启用快速绘图
	double	m_dbFastDrawDistance;	//快速成绘图最小距离

private:
	TCHAR	m_szFileName[MAX_PATH];	//ini文件名称
	TCHAR	m_szExePath[MAX_PATH];		//EXE文件运行目录
	
	CRightMng m_RightMng;
};

extern CSystem g_sys;

#endif // !defined(AFX_SYSTEMPARA_H__D552C92C_31A0_4511_A6A0_F2E1DABE4517__INCLUDED_)





















