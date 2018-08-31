// Card.h: interface for the CCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARD_H__7F908A3E_6C3C_4DCC_BD93_2F083082BA29__INCLUDED_)
#define AFX_CARD_H__7F908A3E_6C3C_4DCC_BD93_2F083082BA29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dot.h"
#include "Stroke.h"
#include "Lay.h"
#include "MarkMgr.h"
#include <mmsystem.h>
#include "Layer.h"	// Added by ClassView
#include "MarkApiDef.h"
#include "Motion.h"
#include "LayerMgn.h"

//扩展功能
typedef union  FUNFLAG
{
	BYTE  bFunc[200];
	struct  FunData
	{
		BYTE  bGrayMark;			//灰度位图
		BYTE  bMotorMark;			//旋转打标
		BYTE  bAdvCali;				//高精度校正
		BYTE  bMulCard;				//多卡功能
		BYTE  bNormal3d;			//标准3d
		BYTE  bQualityTracker;		//监管码
		BYTE  bIndustrial3d;		//行业3d
	}FunData;
}FUNFLAG;

enum E_LASER_TYPE
{
	eType_CO2_GENERAL,	//CO2 通用模式
	eType_CO2_CLASSIC,	//CO2 传统模式
	eType_Q_AP_DQ_CLASSIC,	//普通电源＋数字Q 传统模式
	eType_Q_AP_AQ_CLASSIC,	//普通电源＋模拟Q 传统模式
	eType_Q_DP_DQ_CLASSIC,	//数字电源＋数字Q 传统模式
	eType_Q_DP_AQ_CLASSIC,	//数字电源＋模拟Q 传统模式
	eType_Q_AP_AQ_GENERAL,	//普通电源＋模拟Q 通用模式
	eType_Q_DP_DQ_GENERAL,	//数字电源＋数字Q 通用模式
	eType_Q_DP_AQ_GENERAL,	//数字电源＋模拟Q 通用模式
	eType_IPG,			//IPG
	eType_SPI_GENERAL,	//SPI 通用模式
	eType_SPI_CLASSIC,	//SPI 传统模式
	eType_LD,			//LD
	eType_IPGM,			//M_IPG
	eType_IPGM_GENERAL,	//M_IPG 通用模式
	eType_YLPV2_CLASSIC,            
	eType_YLPV2_GENERAL,
	eType_YLPP_CLASSIC,			//M_IPG
	eType_YLPP_GENERAL,	//M_IPG 通用模式
	eType_HFMA_CLASSIC,
	eType_HFMA_GENERAL
};

#define LINE_MARK	0
#define DOT_MARK	1
#define BMP_MARK	2

//////////////////////////////////////////////////////////////////////////
class Lock
{
public:
	Lock(CRITICAL_SECTION *pCS)
	{
		if ( pCS  !=  NULL )
		{
			m_pObj = pCS;
			::EnterCriticalSection(m_pObj);
		}
		else
			m_pObj = NULL;
	};
	~Lock()
	{
		if ( m_pObj  !=  NULL )
		{
			::LeaveCriticalSection(m_pObj);
			m_pObj = NULL;
		}
	};

private:
	CRITICAL_SECTION* m_pObj;
};

class CCard  
{
public:
	CCard();
	virtual ~CCard();
	
public:
	BOOL Reset();
	BOOL Destory();
	BOOL InitCard();
	BOOL LoadCardLib();
	
	BOOL  IsTouch(BOOL bTouchSts);
	void    Between(HANDLE hStop);      // 两次打标中间处理
	void    InitTouch();                // 用来设置脚踏设置
	int		 GetRepeat(int nLay);
	PARA  GetPara(CProperty pty);
	void	 SetLay(const CLay &lay);
	PARA  GetPara(int nIndex);        // 取层参数
	int		 GetLayerNum(CProperty pty); // 取层号
	CLay* GetCurLay();
	
     //红光
	BOOL		GetRedPreview();
	void		SetRedPreview(BOOL bFlag);
	CString	GetInputStr(BOOL bFromCard = FALSE);
	
public:
	BOOL IsInit();
	void	OutputMsg(UINT nErrCode, UINT nValue = 0);
	void	DispCardInfo(UINT nErrCode, UINT nValue);
	BOOL CloseCard();
	BOOL Touch();		 // 脚踏开关检测
	int    MarkStart(int nMarkType, UINT para = 0, BOOL bFastMark = FALSE);
	UINT MarkEnd(BOOL bFastRedPreview = FALSE, UINT para = 0);

	//nMsk:设置输出点掩码信息
	//nSts:0设置该点为低电平 1设置该点为高电平
	UINT	OutStatus(UINT nMsk, UINT nSts);
	BOOL GetInput(UINT* pIn);   // 获取当前输入
	BOOL GetOutput(UINT* pOut); // 获取当前输出

	//nStyle风格设置：
	//BIT0和BIT1	标识是否为点阵字体 0为线阵打标 1为点阵打标 2位图打标
	UINT Mark(CStroke* pStroke, HWND hWnd,UINT nStyle = LINE_MARK);
	UINT SelMark(CStroke* pStroke, int nLayer, HWND hWnd, HANDLE hSelStop, UINT nStyle = LINE_MARK, BOOL bIsRevMark = FALSE);
	UINT BmpStart(BMPSPEC* pBmpSpec);
	UINT Mark_BMP(UINT nType, LPVOID pInfo,int nNum, UINT nLayerIndex);
	UINT MarkDelay(DWORD dwDelay);	
	UINT MarkWaitSignal(DWORD dwSignal,DWORD dwMask);	
	UINT MarkOutSignal(DWORD dwSignal,DWORD dwMask,DWORD dwDealy = 0);
	
	//打标标识处理
	void  StopMark();
	UINT ReMark();
	UINT PauseMark();
	void  StartMark();
	DWORD GetMarkTime();
	BOOL EnterMarkEnv();		// 进入打标环境
	BOOL LeaveMarkEnv();		// 离开打标环境
	BOOL FlashCardParaEx();

	void SetCurrent(double dbCurrent);
	BOOL GetMarkRect(double& x,double& y);

	CString GetCardVer();
	CString GetCaliBoxName();	
	
	void  SetCardIndex(DWORD dwIndex);
	void  SetMarkLayNum(int nLayNum, BOOL bIsRevMark = FALSE);
	DWORD GetCardNumber();
	DWORD GetCurCardIndex(){ return m_dwCardIndex; };

	UINT MarkStroke(CStroke* pStroke,HWND hWnd,int nStyle);
	UINT GetErrInfo(int nErr,TCHAR* pInfo,int nLen);
	UINT SelMarkStroke(CStroke* pStroke, int nLayer, HWND hWnd, BOOL bIsRevMark = FALSE);
	UINT SetWorkStatus(int nStatus);
	UINT GetWorkStatus(int *pStatus);
	UINT ClearOfflineFlag();
	
	BOOL ClearMulDocData();
	BOOL SetMulMarkStatus(int nStatus);

	UINT MarkBufferMove(LP_MOTOR_PARA lpMovePara);
	UINT MotorMove(LP_MOTOR_PARA lpMovePara);
	
	UINT SetCardTouchMode(int nType);
	UINT MarkSetBufferPos(int nAxis, int nPos);
	UINT GetCardKey(TCHAR* pDogStr, int &nBufSize);
	UINT MarkCheckSwitch(int nCheckFlag,DWORD dwSignal,DWORD dwMask);

	UINT HomeMove(LP_MOTOR_PARA pMotor,BOOL bSoftHome);
	UINT SetAsSoftHome(int nAxis);
	UINT EnumCardKey(TCHAR pCardID[][50],int& nNumber);
	UINT GetCurMotorPos(int nAxis, LONGLONG& nPos);	
	UINT MarkFillStroke(CStroke* pStroke,HWND hWnd,int nStyle, BOOL bAlign, CDot dotBase);
	UINT ImGotoXYZ(int nType, double dbX, double dbY, double dbZ);
	UINT SetCardParaFile(TCHAR *pFileName);
	UINT SetCaliParaFile(TCHAR* pFileName);
	UINT MarkBufferLaser( LP_LASER_PARA pLaserPara );
	UINT GetLayerParaType( int &nType );
	UINT MarkBufferCamera( LP_CAMERA_PARA pCameraPara );
	UINT MarkBufferDynCali( BOOL bEnable );
	BOOL IsCheckStatusOK();
	BOOL CheckSoftSpFunc();

	BOOL Init_SetLayerPara();
	BOOL GetLayerParaCfg(PARA_CFG* pCfg);

	UINT SetLimitBox(double dbXMin, double dbXMax, double dbYMin, double dbYMax);
	UINT SetMotorRatio(double dbInput/*Unit:mm/pulse*/, double dbOutput/*Unit:pulse/mm*/);
	UINT SetCircleCali(double dbRadius, double dbMin,double dbMax);
	UINT SetMotorDelay(int nDelay);

	T_HS_MarkStroke GetMarkFuncPtr(){ return m_pMarkStroke;};
	
private:
	BOOL			m_bInitOK;
	DWORD		m_dwCardIndex;
	DWORD		m_dwCardSet;
	//////////////
	BOOL			m_bRedPreview;		//是否处理红光预览

	CRITICAL_SECTION  m_LockMark;
	void    InitLayerCfg();
	void    SetLayEx();	

protected:
	T_HS_InitCard				m_pInitCard;
	T_HS_GetHdVer			m_pGetHdVer;
	T_HS_GetSwVer			m_pGetSwVer;

	T_HS_SetCaliParaFile	m_pSetCaliParaFile;
	T_HS_SetCardParaFile	m_pSetCardParaFile;
	T_HS_SetMarkPara		m_pSetMarkPara;
	T_HS_EnterMarkEnv		m_pEnterMarkEnv;
	T_HS_LeaveMarkEnv		m_pLeaveMarkEnv;
	T_HS_MarkStart		    m_pMarkStart;
	T_HS_MarkEnd		 		m_pMarkEnd;
	T_HS_GetMarkTime		m_pGetMarkTime;
	T_HS_SetCurrent		    m_pSetCurrent;
	T_HS_Input					m_pInput;
	T_HS_Output			    m_pOutput;
	T_HS_SetGrayCurrent	m_pSetGrayCurrent;
	T_HS_GetMarkRange		m_pGetMarkRange;
	T_HS_MarkPause		    m_pMarkPause;
	T_HS_MarkReMark		m_pMarkReMark;
	T_HS_MarkStop			m_pMarkStop;
	T_HS_GetErrInfo		    m_pGetErrInfo;
	T_HS_ExitCard				m_pExitCard;
    T_HS_SetWorkStatus	m_pSetWorkStatus;
    T_HS_GetWorkStatus	m_pGetWorkStatus;
	T_HS_CheckKey           m_pCheckKey; 
	T_HS_WriteKey			m_pWriteKey;
	T_HS_WriteNewKey		m_pWriteNewKey;
	T_HS_ClearOfflineFlag   m_pClearOfflineFlag;
	T_HS_GetLayerParaType	m_pGetLayerParaType;
	T_HS_GetHdVerInfo		m_pGetHdVerInfo;
	T_HS_ClearMulDocData	m_pClearMulDocData;
	T_HS_SetMulMarkStatus	m_pSetMulMarkStatus;
	T_HS_MarkEndEx			m_pMarkEndEx;
	T_HS_MarkDelay			m_pMarkDelay;
	T_HS_MarkWaitSignal		m_pMarkWaitSignal;
	T_HS_MarkOutSignal		m_pMarkOutSignal;
	T_HS_SetGrayMarkPara	m_pSetGrayMarkPara;
	T_HS_MarkBufferMove		m_pMarkBufferMove;
	T_HS_MotorMove			m_pMotorMove;
	T_HS_SetTouchMode		m_pSetTouchMode;
	T_HS_MarkSetBufferPos	m_pMarkSetBufferPos;
	T_HS_GetCardKey			m_pGetCardKey;
	T_HS_MarkCheckSwitch	m_pMarkCheckSwitch;
	T_HS_HomeMove			m_pHomeMove;
	T_HS_SetAsSoftHome		m_pSetAsSoftHome;
	T_HS_GetCardNumber		m_pGetCardNumber;
	T_HS_SetCardIndex		m_pSetCardIndex;
	T_HS_EnumCardKey		m_pEnumCardKey;
	T_HS_CheckSpFuncFlag	m_pCheckSpFuncFlag;
	T_HS_GetCurMotorPos		m_pGetCurMotorPos;
	T_HS_MarkFillStroke		m_pMarkFillStroke;	
	T_HS_ImGotoXYZ			m_pImGotoXYZ;
	
	T_HS_SetCardParaFileW	m_pSetCardParaFileW;
	T_HS_SetCaliParaFileW	m_pSetCaliParaFileW;
	T_HS_GetErrInfoW		m_pGetErrInfoW;
	T_HS_GetHdVerInfoW		m_pGetHdVerInfoW;
	T_HS_GetCardKeyW		m_pGetCardKeyW;
	T_HS_EnumCardKeyW		m_pEnumCardKeyW;
	T_HS_CheckKeyW			m_pCheckKeyW;
	T_HS_MarkBufferLaser	m_pMarkBufferLaser;
	T_HS_MarkBufferCamera	m_pMarkBufferCamera;
	T_HS_MarkBufferDynCali	m_pMarkBufferDynCali;
	T_HS_MarkStroke			m_pMarkStroke;

	T_HS_SetLimitBox		m_pSetLimitBox;
	T_HS_SetMotorRatio		m_pSetMotorRatio;
	T_HS_SetCircleCali		m_pSetCircleCali;
	T_HS_SetMotorDelay		m_pSetMotorDelay;

public:
	HWND		m_hMarkWnd;			//打标窗口句柄
	int				m_nInitStatus;
	FUNFLAG		m_btExtendInfo;		//版本的扩展功能信息
	BOOL			m_bEnableZOffset;
	CString		m_strEMCCLaser;		//EMCC控制卡采用的激光器类型
	EMCCPARA  m_stEMCCParaDft;	//EMCC默认层参数
	CString		m_strCardKey;			//控制卡号
	PARA_CFG_GROUP m_arrParaCfg;

private:
	HMODULE	m_hMarkLib;
	CLay			m_lay;					// 层参数体
	CString		m_strMulSel;			// I/O选择多文档时IO口状态
	
	// 本参数只与Tree对象相关
	int				m_nMarkLayNum;     
	BOOL			m_bIsResMark;
	UINT			m_nOutputState;
	BOOL			m_bRedStatus;		//红光状态 
	BOOL			m_bTimePaused;	
	
	//脚踏处理
	UINT			m_nTouchCount;		// 用于触发计数，每几个触发信号打标一次
	BOOL			m_bTouchFrist;		// 指示是否为第一次脚踏触发
	
	//打标计时
	DWORD		m_dwStart;				// 打标开始计时
	DWORD		m_dwEnd;
};

extern CCard  g_DAT;

#endif // !defined(AFX_CARD_H__7F908A3E_6C3C_4DCC_BD93_2F083082BA29__INCLUDED_)
