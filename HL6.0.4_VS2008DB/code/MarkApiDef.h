#ifndef __MARK_API_DEF_H__
#define __MARK_API_DEF_H__

typedef UINT (*HS_CARDSTATUS_CALLBACK)(DWORD dwType, __int64 value, DWORD dwCode);
typedef UINT (WINAPI *T_HS_InitCard)(HS_CARDSTATUS_CALLBACK pCardCallback);
typedef UINT (WINAPI *T_HS_GetHdVer)(DWORD *pdwVer);
typedef UINT (WINAPI *T_HS_GetSwVer)(DWORD *pdwVer);
typedef UINT (WINAPI *T_HS_SetCaliParaFile)(char *pFileName);
typedef UINT (WINAPI *T_HS_SetCardParaFile)(char *pFileName);
typedef UINT (WINAPI *T_HS_SetMarkPara)(LPVOID pMarkPara, int nNumPara, int nSize);
typedef UINT (WINAPI *T_HS_EnterMarkEnv)();
typedef UINT (WINAPI *T_HS_LeaveMarkEnv)();
typedef UINT (WINAPI *T_HS_MarkStroke)(UINT nType, LPVOID pInfo, int nNum, UINT nLayerIndex);
typedef UINT (WINAPI *T_HS_MarkStart)(UINT nType, UINT nSts);
typedef UINT (WINAPI *T_HS_MarkEnd)();
typedef UINT (WINAPI *T_HS_GetMarkTime)(UINT64 *pMarkTime);
typedef UINT (WINAPI *T_HS_SetCurrent)(double dbCurrent);
typedef UINT (WINAPI *T_HS_Input)(UINT *pInput);
typedef UINT (WINAPI *T_HS_Output)(UINT nOutput);
typedef UINT (WINAPI *T_HS_SetGrayCurrent)(double *pdbList, int nNum);
typedef UINT (WINAPI *T_HS_GetMarkRange)(double *px, double *py);
typedef UINT (WINAPI *T_HS_MarkPause)();
typedef UINT (WINAPI *T_HS_MarkReMark)();
typedef UINT (WINAPI *T_HS_MarkStop)();
typedef UINT (WINAPI *T_HS_GetErrInfo)(UINT nErrCode, char *pszInfo, int nSize);
typedef UINT (WINAPI *T_HS_ExitCard)();
typedef UINT (WINAPI *T_HS_SetWorkStatus)(int nStatus);
typedef UINT (WINAPI *T_HS_GetWorkStatus)(int *pStatus);
typedef UINT (WINAPI *T_HS_CheckKey)(LPVOID pDogStr, int nSize, int &nNewCount, int &nCardNum);
typedef UINT (WINAPI *T_HS_WriteKey)(LPVOID pDogStr, int nSize);
typedef UINT (WINAPI *T_HS_WriteNewKey)(LPVOID pDogStr, int nSize);
typedef UINT (WINAPI *T_HS_ClearOfflineFlag)();
typedef UINT (WINAPI *T_HS_GetLayerParaType)(int *pParaType);
typedef UINT (WINAPI *T_HS_GetHdVerInfo)(char* pVerInfo, int nSize);

typedef struct OFFLINE_MARKINFO 
{
	WORD nVer;
	UINT nMarkMode;
	DWORD nMarkedOnceTrig;
	WORD nBetweenDelay;
}*LP_OFFLINE_MARKINFO;

typedef struct MULDOC_INFO 
{
	WORD nVer;
	int nDocId;
	WORD nMask;
	WORD nSign;
}*LP_MULDOC_INFO;

typedef struct MARKEND_INFO 
{
	WORD nVer;
	UINT nStatus;
	DWORD dwErrCode;
	UINT nFlashUsed;
}*LPMARKEND_INFO;



typedef UINT (WINAPI *T_HS_ClearMulDocData)();
typedef UINT (WINAPI *T_HS_SetMulMarkStatus)(int nStatus);
typedef UINT (WINAPI *T_HS_MarkEndEx)(UINT nSts);

typedef UINT (WINAPI *T_HS_MarkDelay)(DWORD dwDelay);
typedef UINT (WINAPI *T_HS_MarkWaitSignal)(DWORD dwSignal,DWORD dwMask);
typedef UINT (WINAPI *T_HS_MarkOutSignal)(DWORD dwSignal,DWORD dwMask,DWORD dwDealy);
typedef UINT (WINAPI *T_HS_SetGrayMarkPara)(LPVOID pPara);

typedef UINT (WINAPI *T_HS_MarkBufferMove)(LPVOID pMarkBufferMove);
typedef UINT (WINAPI *T_HS_MotorMove)(LPVOID pMotorMove );

typedef void (*HS_TOUCHSTATUS_CALLBACK)(DWORD dwCode,DWORD dwReserve);
typedef UINT (WINAPI *T_HS_SetTouchMode)(int nType,HS_TOUCHSTATUS_CALLBACK pCallBack);

typedef UINT (WINAPI *T_HS_MarkSetBufferPos)(int nAxis, int nPos);
typedef UINT (WINAPI *T_HS_GetCardKey)(char* pDogStr, int &nBufSize);

typedef UINT (WINAPI *T_HS_MarkCheckSwitch)(int nCheckFlag,DWORD dwSignal,DWORD dwMask);
/*
  nCheckFlag :	1-条件判断开始，dwSignal,dwMask有效。
					0-当前条件判断结束，dwSignal,dwMask无效。
*/

/*
	导出接口号:50
	如果bSoftHome为TRUE,则先回机械原点，再回软件原点。
	如果bSoftHome为FALSE,只回机械原点。
*/
typedef UINT (WINAPI *T_HS_HomeMove)(LPVOID pMotor,BOOL bSoftHome);

/*
	导出接口号:51
	设置任意位置为软件原点，为了保证原点准确性，要求至少回一次机械零点，可反复设置。
*/
typedef UINT (WINAPI *T_HS_SetAsSoftHome)(int nAxis);
typedef UINT (WINAPI *T_HS_GetCardNumber)(DWORD* pNumber);
typedef UINT (WINAPI *T_HS_SetCardIndex)(DWORD nIndex);
typedef UINT (WINAPI *T_HS_EnumCardKey)(char pCardID[][50],int& nNumber);
/*
导出接口号:57
检查Emcc是否支持高级功能，1-升级版运动控制，2-3D打标
*/
typedef UINT (WINAPI *T_HS_CheckSpFuncFlag)(int nFuncNo,BOOL &bFlag);


//获取电机位置
typedef UINT (WINAPI *T_HS_GetCurMotorPos)(int nAxis, LONGLONG& nPos);

//填充线打标
typedef UINT (WINAPI *T_HS_MarkFillStroke)(LPVOID pInfo, int nNum,UINT nLayerIndex,BOOL bAlign,LPVOID pBaseDot);
typedef UINT (WINAPI *T_HS_ImGotoXYZ)(UINT nType, LPVOID pInfo);

//
typedef UINT ( WINAPI *T_HS_SetCardParaFileW)(wchar_t *pFileName);
typedef UINT ( WINAPI *T_HS_SetCaliParaFileW)(wchar_t* pFileName);
typedef UINT ( WINAPI *T_HS_GetErrInfoW )(UINT nErrCode,wchar_t* pwszInfo,int nSize);
typedef UINT ( WINAPI *T_HS_GetHdVerInfoW )(wchar_t* pVerInfo, int nSize);
typedef UINT ( WINAPI *T_HS_GetCardKeyW )(wchar_t* pwDogStr, int &nBufSize);
typedef UINT ( WINAPI *T_HS_EnumCardKeyW )(wchar_t pCardID[][50],int& nNumber);
typedef UINT ( WINAPI *T_HS_CheckKeyW )(wchar_t* pDogStrW, int nSize, int & nNewCount, int & nCardNum);

typedef UINT (WINAPI *T_HS_SendCustomCmd)(__int64 inCmd,__int64 outCmd);
typedef void (*T_HS_USERDEFINE_MSG_CALLBACK)(DWORD dwCode,__int64 value);
typedef UINT (WINAPI *T_HS_SetCustomMsgCall)(T_HS_USERDEFINE_MSG_CALLBACK pCallBack);

typedef struct  BufferLaserPara
{
	int			nVer;
	//////////////
	BOOL		bLaserOn;
	double	dbQFreq;
	double	dbQRls;
	double	dbCurrent;
	double	dbPower;
	int			nLaserMode;//IPG M
}LASER_PARA,*LP_LASER_PARA;

typedef UINT ( WINAPI *T_HS_MarkBufferLaser)(LPVOID pPara);
typedef UINT (WINAPI *T_HS_MarkBufferCamera)(LPVOID lpMarkPara);
typedef UINT (WINAPI *T_HS_MarkBufferDynCali)(int nEnable);

//
// 作用：设置工作有效Box
// 参数说明：工作Box中X和Y的最小和最大值
typedef UINT (WINAPI *T_HS_SetLimitBox)(double dbXMin, double dbXMax, double dbYMin, double dbYMax);

// 作用：设置电机和编码器转换系数，通过该系数转换卡才能准确控制电机运动
// 参数说明：dbInput为编码器转换系数，单位为mm/pulse,dbOutput为电机驱动转换系数，单位为 pulse/mm
typedef UINT (WINAPI *T_HS_SetMotorRatio)(double dbInput/*Unit:mm/pulse*/, double dbOutput/*Unit:pulse/mm*/);
typedef UINT (WINAPI *T_HS_SetCircleCali)(double dbRadius, double dbMin,double dbMax);
typedef UINT (WINAPI *T_HS_SetMotorDelay)(int nDelay/*Unit:us*/);

#endif //__DRV_API_DEF_H__