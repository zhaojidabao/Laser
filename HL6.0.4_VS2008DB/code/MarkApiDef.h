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
  nCheckFlag :	1-�����жϿ�ʼ��dwSignal,dwMask��Ч��
					0-��ǰ�����жϽ�����dwSignal,dwMask��Ч��
*/

/*
	�����ӿں�:50
	���bSoftHomeΪTRUE,���Ȼػ�еԭ�㣬�ٻ����ԭ�㡣
	���bSoftHomeΪFALSE,ֻ�ػ�еԭ�㡣
*/
typedef UINT (WINAPI *T_HS_HomeMove)(LPVOID pMotor,BOOL bSoftHome);

/*
	�����ӿں�:51
	��������λ��Ϊ���ԭ�㣬Ϊ�˱�֤ԭ��׼ȷ�ԣ�Ҫ�����ٻ�һ�λ�е��㣬�ɷ������á�
*/
typedef UINT (WINAPI *T_HS_SetAsSoftHome)(int nAxis);
typedef UINT (WINAPI *T_HS_GetCardNumber)(DWORD* pNumber);
typedef UINT (WINAPI *T_HS_SetCardIndex)(DWORD nIndex);
typedef UINT (WINAPI *T_HS_EnumCardKey)(char pCardID[][50],int& nNumber);
/*
�����ӿں�:57
���Emcc�Ƿ�֧�ָ߼����ܣ�1-�������˶����ƣ�2-3D���
*/
typedef UINT (WINAPI *T_HS_CheckSpFuncFlag)(int nFuncNo,BOOL &bFlag);


//��ȡ���λ��
typedef UINT (WINAPI *T_HS_GetCurMotorPos)(int nAxis, LONGLONG& nPos);

//����ߴ��
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
// ���ã����ù�����ЧBox
// ����˵��������Box��X��Y����С�����ֵ
typedef UINT (WINAPI *T_HS_SetLimitBox)(double dbXMin, double dbXMax, double dbYMin, double dbYMax);

// ���ã����õ���ͱ�����ת��ϵ����ͨ����ϵ��ת��������׼ȷ���Ƶ���˶�
// ����˵����dbInputΪ������ת��ϵ������λΪmm/pulse,dbOutputΪ�������ת��ϵ������λΪ pulse/mm
typedef UINT (WINAPI *T_HS_SetMotorRatio)(double dbInput/*Unit:mm/pulse*/, double dbOutput/*Unit:pulse/mm*/);
typedef UINT (WINAPI *T_HS_SetCircleCali)(double dbRadius, double dbMin,double dbMax);
typedef UINT (WINAPI *T_HS_SetMotorDelay)(int nDelay/*Unit:us*/);

#endif //__DRV_API_DEF_H__