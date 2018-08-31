// Card.cpp: implementation of the CCard class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "Card.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CCard  g_DAT;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UINT CardStatusCallBack(DWORD dwType, __int64 value, DWORD dwCode)
{
	UINT nValue = (UINT)value;
	switch ( dwType )
	{
	case 0:
		{
			g_DAT.OutputMsg(dwCode);
			break;
		}
	case 1:
		{
			UINT nCBValue = (UINT)(((dwType & 0xFFFF) << 16) | (nValue & 0xFFFF));
			g_DAT.OutputMsg(dwCode, nCBValue);
			break;
		}
	case 2:
		{
			g_DAT.OutputMsg(dwCode);
			g_DAT.DispCardInfo(dwCode, (UINT)value);
			break;
		}
	default:
		break;
	}
	return HLOK;
}


void CardTouchMark(DWORD dwCode, DWORD dwReserve)
{
	if ( WaitForSingleObject(hMarkSts, 0)  !=  WAIT_OBJECT_0 && g_DAT.m_hMarkWnd  !=  NULL )
	{
		PostMessage( g_DAT.m_hMarkWnd, WM_MARK_START,0,0);
	}
}


CCard::CCard()
{
	m_bTouchFrist		=  TRUE;
	m_nTouchCount	=  0;
	m_nMarkLayNum	= -1;
	m_bIsResMark		=  FALSE;
	m_bRedPreview	=  FALSE;
	m_nOutputState	=  0;
	m_bRedStatus		=  FALSE;
	
	m_pInitCard					=  NULL;
	m_pGetHdVer				=  NULL;
	m_pGetSwVer				=  NULL;
	m_pSetCaliParaFile		=  NULL;
	m_pSetCardParaFile		=  NULL;
	m_pSetMarkPara			=  NULL;
	m_pEnterMarkEnv			=  NULL;
	m_pLeaveMarkEnv		=  NULL;
	m_pMarkStroke			=  NULL;
	m_pMarkStart				=  NULL;
	m_pMarkEnd				=  NULL;
	m_pGetMarkTime			=  NULL;
	m_pSetCurrent				=  NULL;
	m_pInput					=  NULL;
	m_pOutput					=  NULL;
	m_pSetGrayCurrent		=  NULL;
	m_pGetMarkRange		=  NULL;
	m_pMarkPause				=  NULL;
	m_pMarkReMark			=  NULL;
	m_pMarkStop				=  NULL;
	m_pGetErrInfo				=  NULL;
	m_pExitCard				=  NULL;
	m_pClearOfflineFlag		=  NULL;
	
	m_pCheckKey				=  NULL;
	m_pWriteKey				=  NULL;
	m_pWriteNewKey			=  NULL;
	m_pGetLayerParaType	=  NULL;
	m_pClearMulDocData		=  NULL;
	m_pSetMulMarkStatus	=  NULL;
	m_pMarkEndEx				=  NULL;
	m_pMarkDelay				=  NULL;
	m_pMarkWaitSignal		=  NULL;
	m_pMarkOutSignal		=  NULL;
	m_pSetGrayMarkPara	=  NULL;
	m_pMarkBufferMove		=  NULL;
	m_pMotorMove			=  NULL;
	m_pSetTouchMode		=  NULL;
	m_pMarkSetBufferPos	=  NULL;
	m_pGetCardKey			=  NULL;
	
	m_pMarkCheckSwitch	=  NULL;
	m_pSetAsSoftHome		=  NULL;
	m_pGetCardNumber		=  NULL;
	m_pSetCardIndex			=  NULL;
	m_pEnumCardKey			=  NULL;
	m_pCheckSpFuncFlag	=  NULL;
	m_pGetCurMotorPos		=  NULL;
	m_pMarkFillStroke			=  NULL;
	m_pImGotoXYZ			=  NULL;

	m_pSetCardParaFileW	=  NULL;
	m_pSetCaliParaFileW		=  NULL;
	m_pGetErrInfoW			=  NULL;
	m_pGetHdVerInfoW		=  NULL;
	m_pGetCardKeyW			=  NULL;
	m_pEnumCardKeyW		=  NULL;
	m_pCheckKeyW			=  NULL;
	m_pMarkBufferCamera	=  NULL;
	m_pMarkBufferDynCali	=  NULL;


//	m_pSetLimitBox			=	NULL;
//	m_pSetMotorRatio		=	NULL;
//	m_pSetCircleCali			=	NULL;

	::InitializeCriticalSection(&m_LockMark);
	
	m_hMarkWnd	 =  NULL;
	m_bInitOK		 =  FALSE;
	m_dwCardSet	 = 0;
	m_dwCardIndex = 0;
	m_nInitStatus = 0;
	m_bEnableZOffset = TRUE;

	
#ifdef _DEBUG
	memset(m_btExtendInfo.bFunc, 1, 200*sizeof(BYTE));
#else
	memset(m_btExtendInfo.bFunc, 0, 200*sizeof(BYTE));
#endif
}


CCard::~CCard()
{
	::DeleteCriticalSection(&m_LockMark);
	///////
	Destory();
}


BOOL CCard::IsInit()
{ 
	return m_bInitOK; 
}

BOOL CCard::Reset()
{
#ifdef _SOFTMODE_EDITOR_
	return TRUE;
#else
	return FlashCardParaEx();
#endif 
}

BOOL CCard::Destory()
{
#ifdef _SOFTMODE_EDITOR_
	return TRUE;
#else
	if ( m_hMarkLib )
	{
		if ( m_pExitCard )
			m_pExitCard();
		
		FreeLibrary(m_hMarkLib);
		m_hMarkLib = NULL;
	}
	return TRUE;
#endif
}

BOOL CCard::LoadCardLib()
{
	TCHAR szPath[256] =  { 0 };
	TCHAR szFileName[256] = { 0 };
	///
	GetExepath(szPath);
	if ( !g_sys.m_bDeviceDebug  )
	{
		_stprintf_s(szFileName, 255,_T("%s%s\\%s"), szPath, g_SysCfg.m_szCardName, _T("Device.dll"));
	}
	else
	{
		_stprintf_s(szFileName,255, _T("%s%s\\%s"), szPath, g_SysCfg.m_szCardName, _T("DeviceTrace.dll"));
	}
	
	BOOL bLoadOK = FALSE;
	m_hMarkLib = LoadLibrary( szFileName );
	if ( !m_hMarkLib )
	{
		OutputDebugString( _T(" CCard::LoadCardLib() Error...\n") );
		return FALSE;
	}
	
	m_pInitCard					=  (T_HS_InitCard)GetProcAddress(m_hMarkLib,			LPCSTR(0x01));
	m_pGetHdVer				=  (T_HS_GetHdVer)GetProcAddress(m_hMarkLib,			LPCSTR(0x02));
	m_pGetSwVer				=  (T_HS_GetSwVer)GetProcAddress(m_hMarkLib,			LPCSTR(0x03));
	m_pSetCaliParaFile		=  (T_HS_SetCaliParaFile)GetProcAddress(m_hMarkLib,	LPCSTR(0x04));
	m_pSetCardParaFile		=  (T_HS_SetCardParaFile)GetProcAddress(m_hMarkLib,	LPCSTR(0x05));
	
	m_pSetMarkPara			=  (T_HS_SetMarkPara)GetProcAddress(m_hMarkLib,		LPCSTR(0x06));
	m_pEnterMarkEnv			=  (T_HS_EnterMarkEnv)GetProcAddress(m_hMarkLib,	LPCSTR(0x07));
	m_pLeaveMarkEnv		=  (T_HS_LeaveMarkEnv)GetProcAddress(m_hMarkLib,	LPCSTR(0x08));
	m_pMarkStroke			=  (T_HS_MarkStroke)GetProcAddress(m_hMarkLib,		LPCSTR(0x09));
	m_pMarkStart				=  (T_HS_MarkStart)GetProcAddress(m_hMarkLib,			LPCSTR(0x0A));
	m_pMarkEnd				=  (T_HS_MarkEnd)GetProcAddress(m_hMarkLib,			LPCSTR(0x0B));
	m_pGetMarkTime			=  (T_HS_GetMarkTime)GetProcAddress(m_hMarkLib,		LPCSTR(0x0C));
	m_pSetCurrent				=  (T_HS_SetCurrent)GetProcAddress(m_hMarkLib,		LPCSTR(0x0D));
	m_pInput					=  (T_HS_Input)GetProcAddress(m_hMarkLib,				LPCSTR(0x0E));
	m_pOutput					=  (T_HS_Output)GetProcAddress(m_hMarkLib,				LPCSTR(0x0F));
	
	m_pSetGrayCurrent		=  (T_HS_SetGrayCurrent)GetProcAddress(m_hMarkLib,	LPCSTR(0x10));
	m_pGetMarkRange		=  (T_HS_GetMarkRange)GetProcAddress(m_hMarkLib,	LPCSTR(0x11));
	m_pMarkPause				=  (T_HS_MarkPause)GetProcAddress(m_hMarkLib,		LPCSTR(0x13));
	m_pMarkReMark			=  (T_HS_MarkReMark)GetProcAddress(m_hMarkLib,		LPCSTR(0x14));
	m_pMarkStop				=  (T_HS_MarkStop)GetProcAddress(m_hMarkLib,			LPCSTR(0x15));
	m_pGetErrInfo				=  (T_HS_GetErrInfo)GetProcAddress(m_hMarkLib,			LPCSTR(0x16));
	m_pExitCard				=  (T_HS_ExitCard)GetProcAddress(m_hMarkLib,			LPCSTR(0x17));
	m_pSetWorkStatus		=  (T_HS_SetWorkStatus)GetProcAddress(m_hMarkLib,	LPCSTR(0x18));
	m_pGetWorkStatus		=  (T_HS_GetWorkStatus)GetProcAddress(m_hMarkLib,	LPCSTR(0x19));
	m_pClearOfflineFlag		=  (T_HS_ClearOfflineFlag)GetProcAddress(m_hMarkLib,	LPCSTR(0x1A));
	
	m_pGetLayerParaType	=  (T_HS_GetLayerParaType)GetProcAddress(m_hMarkLib,	LPCSTR(0x1C));
	m_pGetHdVerInfo			=  (T_HS_GetHdVerInfo)GetProcAddress(m_hMarkLib,			LPCSTR(0x1D));
	m_pClearMulDocData		=  (T_HS_ClearMulDocData)GetProcAddress(m_hMarkLib,	LPCSTR(30));
	m_pSetMulMarkStatus	=  (T_HS_SetMulMarkStatus)GetProcAddress(m_hMarkLib,	LPCSTR(31));
	m_pMarkEndEx				=  (T_HS_MarkEndEx)GetProcAddress(m_hMarkLib,			LPCSTR(32));
	m_pMarkDelay				=  (T_HS_MarkDelay)GetProcAddress(m_hMarkLib,				LPCSTR(33));
	m_pMarkWaitSignal		=  (T_HS_MarkWaitSignal)GetProcAddress(m_hMarkLib,		LPCSTR(34));
	m_pMarkOutSignal		=  (T_HS_MarkOutSignal)GetProcAddress(m_hMarkLib,		LPCSTR(35));
	m_pImGotoXYZ			=  (T_HS_ImGotoXYZ)GetProcAddress(m_hMarkLib,			LPCSTR(37));
	m_pSetGrayMarkPara	=  (T_HS_SetGrayMarkPara)GetProcAddress(m_hMarkLib,	LPCSTR(38));	
	m_pSetTouchMode		=  (T_HS_SetTouchMode)GetProcAddress(m_hMarkLib,		LPCSTR(41));
	m_pMarkBufferMove		=  (T_HS_MarkBufferMove)GetProcAddress(m_hMarkLib,	    LPCSTR(44));
	m_pMarkSetBufferPos	=  (T_HS_MarkSetBufferPos)GetProcAddress(m_hMarkLib,	LPCSTR(45));
	m_pMotorMove			=  (T_HS_MotorMove)GetProcAddress(m_hMarkLib,			LPCSTR(46));
	m_pMarkCheckSwitch	=  (T_HS_MarkCheckSwitch)GetProcAddress(m_hMarkLib,	LPCSTR(47));
	m_pHomeMove				=  (T_HS_HomeMove)GetProcAddress(m_hMarkLib,			LPCSTR(50));
	m_pSetAsSoftHome		=  (T_HS_SetAsSoftHome)GetProcAddress(m_hMarkLib,		LPCSTR(51));
	m_pGetCardNumber		=  (T_HS_GetCardNumber)GetProcAddress(m_hMarkLib,		LPCSTR(52));
	m_pSetCardIndex			=  (T_HS_SetCardIndex)GetProcAddress(m_hMarkLib,			LPCSTR(53));
	m_pEnumCardKey			=  (T_HS_EnumCardKey)GetProcAddress(m_hMarkLib,			LPCSTR(54));
	m_pGetCardKey			=  (T_HS_GetCardKey)GetProcAddress(m_hMarkLib,			LPCSTR(83));
	m_pCheckSpFuncFlag	=  (T_HS_CheckSpFuncFlag)GetProcAddress(m_hMarkLib,   LPCSTR(57));
	m_pGetCurMotorPos		=  (T_HS_GetCurMotorPos)GetProcAddress(m_hMarkLib,	    LPCSTR(55));
	m_pMarkFillStroke			=  (T_HS_MarkFillStroke)GetProcAddress(m_hMarkLib,		LPCSTR(58));
	
	m_pSetCardParaFileW	=  (T_HS_SetCardParaFileW)GetProcAddress(m_hMarkLib, LPCSTR(59));
	m_pSetCaliParaFileW		=  (T_HS_SetCaliParaFileW)GetProcAddress(m_hMarkLib, LPCSTR(60));
	m_pGetErrInfoW			=  (T_HS_GetErrInfoW)GetProcAddress(m_hMarkLib, LPCSTR(61));
	m_pGetHdVerInfoW		=  (T_HS_GetHdVerInfoW)GetProcAddress(m_hMarkLib, LPCSTR(62));
	m_pMarkBufferLaser		=  (T_HS_MarkBufferLaser)GetProcAddress(m_hMarkLib, LPCSTR(64));
	m_pMarkBufferCamera	=  (T_HS_MarkBufferCamera)GetProcAddress(m_hMarkLib, LPCSTR(67));
	m_pMarkBufferDynCali	=  (T_HS_MarkBufferDynCali)GetProcAddress(m_hMarkLib, LPCSTR(68));
	m_pGetCardKeyW			=  (T_HS_GetCardKeyW)GetProcAddress(m_hMarkLib, LPCSTR(84));
	m_pEnumCardKeyW		=  (T_HS_EnumCardKeyW)GetProcAddress(m_hMarkLib, LPCSTR(85));
	m_pCheckKeyW			=  (T_HS_CheckKeyW)GetProcAddress(m_hMarkLib, LPCSTR(86));

	//m_pSetLimitBox			=	(T_HS_SetLimitBox)GetProcAddress(m_hMarkLib, LPCSTR(72));
	//m_pSetMotorRatio		=	(T_HS_SetMotorRatio)GetProcAddress(m_hMarkLib, LPCSTR(73));
	//m_pSetCircleCali			=   (T_HS_SetCircleCali)GetProcAddress(m_hMarkLib, LPCSTR(74));
	
	if (     m_pInitCard
		&& m_pGetHdVer
		&& m_pGetSwVer
		&& m_pSetCaliParaFile
		&& m_pSetCardParaFile
		&& m_pSetMarkPara
		&& m_pEnterMarkEnv
		&& m_pLeaveMarkEnv
		&& m_pMarkStroke
		&& m_pMarkStart
		&& m_pMarkEnd
		&& m_pGetMarkTime
		&& m_pSetCurrent
		&& m_pInput
		&& m_pOutput
		&& m_pSetGrayCurrent
		&& m_pGetMarkRange
		&& m_pMarkPause
		&& m_pMarkReMark
		&& m_pMarkStop
		&& m_pGetErrInfo
		&& m_pExitCard
		&& m_pSetWorkStatus
		&& m_pGetWorkStatus
		&& m_pClearOfflineFlag
		&& m_pGetLayerParaType
		&& m_pGetHdVerInfo
		&& m_pClearMulDocData
		&& m_pSetMulMarkStatus
		&& m_pMarkEndEx
		&& m_pMarkDelay
		&& m_pMarkWaitSignal
		&& m_pMarkOutSignal
		&& m_pSetGrayMarkPara
		&& m_pMarkBufferMove		
		&& m_pSetTouchMode
		&& m_pMarkSetBufferPos
		&& m_pMotorMove
		&& m_pGetCardKey
		&& m_pMarkCheckSwitch
		&& m_pHomeMove
		&& m_pSetAsSoftHome
		&& m_pGetCardNumber
		&& m_pSetCardIndex
		&& m_pEnumCardKey
		&& m_pCheckSpFuncFlag
		&& m_pGetCurMotorPos
		&& m_pMarkFillStroke
		&& m_pImGotoXYZ
		&& m_pSetCardParaFileW
		&& m_pSetCaliParaFileW
		&& m_pGetErrInfoW
		&& m_pGetHdVerInfoW
		&& m_pGetCardKeyW
		&& m_pEnumCardKeyW
		&& m_pCheckKeyW
		&& m_pMarkBufferLaser
		&& m_pMarkBufferCamera
		&& m_pMarkBufferDynCali
		//&& m_pSetLimitBox
		//&& m_pSetMotorRatio
		//&& m_pSetCircleCali
	  )
	{
		bLoadOK = TRUE;
	}

	return bLoadOK;
}


BOOL CCard::InitCard()
{
#ifdef _SOFTMODE_EDITOR_
	return TRUE;
#endif

	//初始化脚踏信号
	InitTouch();
	//
	if ( !m_pInitCard  )
		return FALSE;

	m_bInitOK = FALSE;
	m_nInitStatus = 0;
	UINT nRet = m_pInitCard(CardStatusCallBack);
	if ( HLOK  !=  nRet )
	{
		TCHAR	szMsg[256]={0};
		_stprintf_s(szMsg, 255,_T("m_pInitCard() Error ret = %d...\n"),nRet);
		OutputDebugString(szMsg);
		return FALSE;
	}

	m_nInitStatus = 1;
	m_dwCardIndex = m_dwCardSet;
	if ( !FlashCardParaEx() )
		return FALSE;

	m_bInitOK = TRUE;
	
	//判断下位机程序版本
#ifndef _DEBUG //正式版
	BOOL  bSupport3D = FALSE;
	if ( HLOK  !=  m_pCheckSpFuncFlag(2,bSupport3D) )
		bSupport3D = FALSE;
	
	if ( !bSupport3D )
	{
		AfxMessageBox(IDS_DSP_NOTSUPPORT3D);
		return FALSE;
	}
#endif

	if ( SetCardTouchMode(::g_sys.m_nCardTouchModel)  !=  HLOK)
		return FALSE;

	return TRUE;
}


void CCard::InitLayerCfg()
{
	::memset(&m_stEMCCParaDft, 0, sizeof(EMCCPARA));
	m_arrParaCfg.RemoveAll();
	
	GetParaGroup( m_arrParaCfg, g_sys.GetInstallPath() );
	for( int iPage = 0; iPage < m_arrParaCfg.GetSize(); ++iPage ) 
	{
		PARA_PAGE& page = m_arrParaCfg.ElementAt(iPage);
		for( int iPara = 0; iPara < page.m_arrCfg.GetSize(); ++iPara ) 
		{
			PARA_CFG& paraCfg = page.m_arrCfg[iPara];
			if (_T("CNT") == paraCfg.m_strAlias)    {
				m_stEMCCParaDft.nCount = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("ASV") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbMarkV = _tstof(paraCfg.m_curValue);					
			}
			else if (_T("ESV") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbJumpV = _tstof(paraCfg.m_curValue);
			}
			else if (_T("QSF") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbQFreq = _tstof(paraCfg.m_curValue);
			}
			else if (_T("QRT") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbQRls = _tstof(paraCfg.m_curValue);
			}
			else if (_T("WAT") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbCurrent = _tstof(paraCfg.m_curValue);
			}
			else if (_T("FPT") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nFpkTime = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("FIP") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbFPKInitRate = _tstof(paraCfg.m_curValue);
			}
			else if (_T("FAP") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbFPKLenRate = _tstof(paraCfg.m_curValue);
			}
			else if (_T("FMP") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbFPKMaxPRate = _tstof(paraCfg.m_curValue);
			}
			else if (_T("FCR") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbFPKCurRate = _tstof(paraCfg.m_curValue);
			}
			else if (_T("MQR") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nQuality = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("LDT") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nLayerDelay = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("ODL") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nLaserOnDelay = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("FDL") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nLaserOffDelay = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("MLT") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nMoveLineDelay = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("JDL") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nJumpDelay = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("MDL") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nRoundDelay = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("CUR") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbCurrentR = _tstof(paraCfg.m_curValue);
			}
			else if (_T("SMC") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbSimmerCur = _tstof(paraCfg.m_curValue);
			}
			else if (_T("WAF") == paraCfg.m_strAlias || _T("PLS") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.nWaveForm = _ttoi(paraCfg.m_curValue);
			}
			else if (_T("POW") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbPowerR = _tstof(paraCfg.m_curValue);
			}
			else if (_T("SMP") == paraCfg.m_strAlias) {
				m_stEMCCParaDft.dbSimmerCur = _tstof(paraCfg.m_curValue);
			}
		}
	}
	
	//
	for( int i = 0; i < 4; i++ )
	{
		::g_Motion.ResetPara(i);
	}
}


BOOL CCard::GetLayerParaCfg(PARA_CFG* pCfg)
{
	if ( NULL == pCfg )
		return FALSE;
	
	for( int iPage = 0; iPage < m_arrParaCfg.GetSize(); ++iPage ) 
	{
		PARA_PAGE& page = m_arrParaCfg.ElementAt(iPage);
		for( int iPara = 0; iPara < page.m_arrCfg.GetSize(); ++iPara ) 
		{
			PARA_CFG& paraCfg = page.m_arrCfg[iPara];
			if (paraCfg.m_strAlias == pCfg->m_strAlias)
			{
				*pCfg = paraCfg;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}




BOOL CCard::IsTouch( BOOL bTouchSts )
{
	int result = bTouchSts;
	if ( g_sys.m_bMarkOnce )//一个对象上只触发一次
	{
		if ( m_bTouchFrist )
		{
			if ( result == TRUE )
			{
				m_nTouchCount++;
				m_bTouchFrist = FALSE;
				if ( m_nTouchCount  >=  g_sys.m_nTouchTimes )
				{
					m_nTouchCount = 0;
					return TRUE;
				}
				else
					return FALSE;
			}
			else
				return FALSE;
		}
		else
		{
			if ( result == FALSE )
				m_bTouchFrist = TRUE;

			return FALSE;
		}
	}
	else
		return result;
}



//重复打标时，中间状态处理
void CCard::Between(HANDLE hStop)
{
	DWORD dwStart = timeGetTime();
	for (int i = 0; i < (int)(g_sys.m_nDelayBetween); i++)
	{
		if ( ::WaitForSingleObject(hStop,0) == WAIT_OBJECT_0 )
			break;
		
		DWORD dwEnd = timeGetTime();
		if ((int)(dwEnd - dwStart)  >=  (int)(g_sys.m_nDelayBetween))
		{
			break;
		}
		Sleep(1);
	}
}

void CCard::SetLay(const CLay &lay)
{
	m_lay = lay;
	//
	SetLayEx();
}

PARA CCard::GetPara(CProperty pty)
{
	CLayer ly = m_lay.GetLayer(pty);
	return ly.m_para;
}

PARA CCard::GetPara(int nIndex)
{
	CLayer ly = m_lay.GetLayer(nIndex);
	return ly.m_para;
}

int CCard::GetLayerNum(CProperty pty)
{	
	return m_lay.GetLayerNum(pty);
}

int CCard::GetRepeat(int nLay)
{
	return m_lay.GetRepeat(nLay);
}

void CCard::InitTouch()
{
	//读取脚踏开关状态参数
	m_bTouchFrist = TRUE;
	m_nTouchCount = 0;
}


CLay* CCard::GetCurLay()
{
	return &m_lay;
}

void CCard::SetRedPreview(BOOL bFlag)
{
	m_bRedPreview = bFlag;
}

BOOL CCard::GetRedPreview()
{
	return m_bRedPreview;
}


//说明：返回 I11, I4-I7的信号字符串
CString CCard::GetInputStr(BOOL bFromCard)
{
	if ( 1 == g_sys.m_nCardTouchModel || bFromCard )
	{
		UINT v = 0;
		GetInput(&v);
		
		switch ( ::g_sys.m_nIOSelCount )
		{
		case 4:
			m_strMulSel.Format(_T("%d%d%d%d"),(v&I_MULSEL_3)?1:0, (v&I_MULSEL_2)?1:0,(v&I_MULSEL_1)?1:0,(v&I_MULSEL_0)?1:0);
			break;

		case 5:
			m_strMulSel.Format(_T("%d%d%d%d%d"),(v&I_MULSEL_3)?1:0, (v&I_MULSEL_2)?1:0, (v&I_MULSEL_1)?1:0, (v&I_MULSEL_0)?1:0, (v&I_MULSEL_4)?1:0 );
			break;

		default:
			m_strMulSel.Format(_T("%d%d%d%d"),(v&I_MULSEL_3)?1:0, (v&I_MULSEL_2)?1:0,(v&I_MULSEL_1)?1:0,(v&I_MULSEL_0)?1:0);
			break;
		}
	}
	
	return m_strMulSel;
}


void CCard::SetLayEx()
{
#ifdef _SOFTMODE_EDITOR_
	return;
#endif

	if ( NULL == m_pSetMarkPara )
		return;
	
	//在每次换文档时进行层参数设置处理
	int   nSize = m_lay.GetSize();
	if ( nSize < 0 ) 
	{
		OutputDebugString(_T("CCard::SetLayEx() m_lay.GetSize()  <=  0...\n"));
		return;
	}
	
	PARA* pPara = new PARA[nSize];
	if ( NULL == pPara )
		return;
	
	memset(pPara,0,sizeof(PARA));
	double   dbMaxMQR = 0.0;
	PARA_CFG paraCfg;
	paraCfg.m_strAlias = _T("MQR");
	
	if ( GetLayerParaCfg(&paraCfg) )
		dbMaxMQR = paraCfg.m_dbMax;
	
	for(int i = 0; i<nSize; i++ )
	{
		CLayer ly = m_lay.GetLayer(i);
		PARA para = ly.m_para;
		para.emcc.nQuality = (int)dbMaxMQR - para.emcc.nQuality;
		if ( m_strEMCCLaser == _T("M_IPG") )	
		{
			para.emcc.nWaveForm--;
		}
		*(pPara+i) = para;
	}
	
	UINT nRet = m_pSetMarkPara(pPara, nSize, sizeof(PARA));
	if ( nRet  !=  HLOK )
	{
		OutputMsg(nRet);
	}
	
	//
	if ( pPara )
	{
		delete[] pPara;
		pPara = NULL;
	}
}

BOOL CCard::CloseCard()
{
#ifdef _SOFTMODE_EDITOR_
	return TRUE;
#endif
	
	if ( m_bInitOK )
	{
		m_pExitCard();
		m_bInitOK = FALSE;
	}
	return TRUE;
}

CString CCard::GetCaliBoxName()
{
	if ( m_nInitStatus < 2  )
		return _T("");
	
	TCHAR  szRet[50] = {0};
	TCHAR  szSetFile[MAX_PATH] = {0},szKey[50] = {0};
	_stprintf_s(szSetFile,255, _T("%s\\%s"), g_sys.m_strCaliPath, _T("SystemSet.ini"));
	_stprintf_s(szKey,50,_T("%sCurrentStyle"),m_strCardKey);
	 GetPrivateProfileString( _T("CALICONFIG"), szKey,_T("Default"),szRet,50,szSetFile);

	 return szRet;
}


BOOL CCard::FlashCardParaEx()
{
#ifdef _SOFTMODE_EDITOR_
	return TRUE;
#endif

	//设定卡参数
	TCHAR szFile[MAX_PATH] = {0};
	_stprintf_s(szFile,255,_T("%s"), g_sys.m_strCardFile);
	if ( HLOK  !=  SetCardParaFile(szFile) )
		return FALSE;
	
	int   nSiZe = 50;
	TCHAR  szCardKey[50] = {0};
	if (  HLOK  !=  GetCardKey(szCardKey, nSiZe) )
		return FALSE;
	
	m_strCardKey = szCardKey;
	
	//设定校正参数
	CString strCaliFile = _T("");
	strCaliFile.Format(_T("\\%s\\%s\\%s"), _T("CaliSet"), m_strCardKey, _T("calidat.box"));
	strCaliFile = g_sys.m_strCaliPath + strCaliFile;
	
	CFileStatus sts = {0};
	if ( !CFile::GetStatus(strCaliFile,sts) )
		return FALSE;
	
	_stprintf_s(szFile,255,_T("%s"),strCaliFile);
	if ( HLOK  !=  SetCaliParaFile(szFile) )
		return FALSE;
	
	m_nInitStatus = 2;

	return TRUE;
}

//
BOOL CCard::Touch()
{
#ifdef _SOFTMODE_EDITOR_
	return FALSE;
#endif

	if ( !m_pInput )
		return FALSE;
	
	UINT vInput = 0;
	if ( HLOK  !=  m_pInput(&vInput) )
		return FALSE;
	
	if ( g_sys.m_nIOSelCount == 4 )
	{
		m_strMulSel.Format(_T("%d%d%d%d") ,	( vInput & I_MULSEL_3 ) ? 1 : 0,
			( vInput & I_MULSEL_2 ) ? 1 : 0,
			( vInput & I_MULSEL_1 ) ? 1 : 0,
			( vInput & I_MULSEL_0 ) ? 1 : 0);
	}
	else if ( g_sys.m_nIOSelCount == 5 )
	{
		m_strMulSel.Format(_T("%d%d%d%d%d") ,	( vInput & I_MULSEL_3 ) ? 1 : 0,
			( vInput & I_MULSEL_2 ) ? 1 : 0,
			( vInput & I_MULSEL_1 ) ? 1 : 0,
			( vInput & I_MULSEL_0 ) ? 1 : 0,
			( vInput & I_MULSEL_4 ) ? 1 : 0);
	}
	
	return IsTouch( !( vInput & 0x01 ) );
}



//获取当前输入
BOOL CCard::GetInput(UINT* pIn) 
{
#ifdef _SOFTMODE_EDITOR_
	return TRUE;
#endif

	if ( !m_pInput ||  !m_bInitOK )
		return FALSE;
	
 	if ( HLOK == m_pInput(pIn) )
 		return TRUE;
 	
	return FALSE;
}


//获取当前输出
BOOL CCard::GetOutput(UINT* pOut)
{
	*pOut = m_nOutputState;

	return TRUE;
}

//
int CCard::MarkStart(int nMarkType, UINT para, BOOL bFastMark)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif
	
	//获取红光状态
	UINT uOut =  0;
	GetOutput(&uOut);
	m_bRedStatus = ( uOut & O_RED ? TRUE : FALSE );
	//
	m_bTimePaused = FALSE;
	m_dwStart  = timeGetTime();
	UINT nRet  = HLOK;
	
	switch ( nMarkType )
	{
	case ENU_MARK:
		{
			if ( !bFastMark )
				::Sleep( g_sys.m_nDelayBefore );
			
			if ( g_SysCfg.m_bCloseRedIfMark )
				OutStatus( O_RED, 0 );
			
			nRet = m_pMarkStart(ENU_MARK, 0);
		}
		break;
	case ENU_RED:
		{
			OutStatus( O_RED, O_RED );	//红光
			nRet = m_pMarkStart(ENU_RED, 0);
		}
		break;
	case ENU_FRAME:
		{
			OutStatus(O_RED,O_RED);	//红光
			nRet = m_pMarkStart(ENU_FRAME, 0);
		}
		break;
	case ENU_DOWNSINGLE:
		{
			OFFLINE_MARKINFO stInfo = { 0 };
			stInfo.nVer = 1;
			stInfo.nMarkMode		    = g_sys.m_bLoopMdt;
			stInfo.nMarkedOnceTrig	 =  g_sys.m_nMarkRepeat;
			stInfo.nBetweenDelay	 =  g_sys.m_nDelayBetween;
			nRet = m_pMarkStart( ENU_DOWNSINGLE, (UINT)&stInfo );
		}
		break;
	case ENU_DOWNMULDOC:
		{
			nRet = m_pMarkStart(ENU_DOWNMULDOC, para);
		}
		break;
	}
	
	if ( HLOK  !=  LOWORD(nRet) )
	{
		OutputMsg(nRet);
	}
	
	return nRet;
}

//
UINT CCard::MarkEnd( BOOL bFastMark, UINT para )
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif
	
	if ( m_pMarkEnd == NULL )
		return HLERROR;
	
	UINT nRet = m_pMarkEndEx(para);
	
	//重置红光状态
	if ( m_bRedStatus )
	{
		OutStatus(O_RED,O_RED);
	}
	
	if ( LOWORD(nRet)  !=  HLOK )
	{
		OutputMsg(nRet);
		m_dwEnd = timeGetTime();
		return nRet;
	}
	
	if ( !bFastMark && g_sys.m_nDelayAfter > 0 )
		::Sleep(g_sys.m_nDelayAfter);
	
	m_dwEnd  = timeGetTime();
	
	return nRet;
}


//nMsk:设置输出点掩码信息
//nSts:0设置该点为低电平 1设置该点为高电平
UINT CCard::OutStatus( UINT nMsk,UINT nSts )
{
#ifdef _SOFTMODE_EDITOR_
	return HLOK;
#endif
	
	if ( !m_pOutput || !m_bInitOK )
		return HLERROR;
	
	m_nOutputState   &=  ~nMsk;
	m_nOutputState   |=  (nMsk & nSts);
	//
	UINT nRet = m_pOutput(m_nOutputState);
	if ( LOWORD(nRet)  !=  HLOK )
		OutputMsg(nRet);

	return nRet;
}


UINT CCard::Mark_BMP(UINT nType, LPVOID pInfo,int nNum, UINT nLayerIndex)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pMarkStroke == NULL )
		return HLERROR;
	
	if ( WAIT_OBJECT_0 == WaitForSingleObject(hStopWork,0) )
		return HLERROR;
	
	UINT nRet = m_pMarkStroke(nType,pInfo,nNum,nLayerIndex);
	if ( LOWORD(nRet) > 0 )
		OutputMsg(nRet);
	
	if (  LOWORD(nRet) == HLOK || LOWORD(nRet) > 10 )
		return HLOK;
	else
		return HLERROR;
	
	return HLOK;
}


//
UINT CCard::Mark(CStroke* pStroke, HWND hWnd,UINT nStyle)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif
	if ( WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
		return HLERROR;
	
	if ( !pStroke || pStroke->m_dotArray.GetSize() <= 0 )
		return HLOK;
	
	return  MarkStroke(pStroke,hWnd,nStyle);
}


//////////////////////////////////////////////////////////////////////////
//以下函数是流程控制
//////////////////////////////////////////////////////////////////////////
UINT CCard::MarkStroke(CStroke* pStroke,HWND hWnd,int nStyle)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif
	
	if ( !pStroke || !m_pMarkStroke || WAIT_OBJECT_0 == ::WaitForSingleObject(hStopWork,0) )
		return HLERROR;
	
	UINT nRet = 0;
	int nLayer = GetLayerNum(pStroke->m_property);	//当前笔画层号
	int nSize = pStroke->m_dotArray.GetSize();
	if ( nSize  <=  0 )
		return HLOK;
	
	DOT6* pDot = new DOT6[nSize];
	if ( NULL == pDot )
	{
		OutputDebugString(_T("CCard::MarkStroke() Allocate memory fail!\n"));
		return HLERROR;
	}
	
	memset(pDot, 0, nSize*sizeof(DOT6));
	CDot tmp;
	
	if ( !m_bIsResMark )
	{
		for( int i = 0; i < nSize; i++ )
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[i].x = tmp.x;
			pDot[i].y = tmp.y;
			pDot[i].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
		}
	}
	else
	{
		int nI = 0;
		for( int i = (nSize-1); i  >=  0; i-- )
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[nI].x = tmp.x;
			pDot[nI].y = tmp.y;
			pDot[nI].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
			nI++;
		}
	}
	
	//红光预览层号选择
	if ( TRUE == m_bRedPreview )
	{
		m_nMarkLayNum = g_SysCfg.m_nRedPreviewLayerNo >= 0 ? g_SysCfg.m_nRedPreviewLayerNo : 0;
	}
	//////
	if ( m_nMarkLayNum  >=  0 )
	{
		if ( g_SysCfg.m_nLayerCnt < m_nMarkLayNum )
		{
			m_nMarkLayNum = g_SysCfg.m_nLayerCnt;
		}
		
		nRet = m_pMarkStroke( nStyle, LPVOID(pDot), nSize, m_nMarkLayNum );
	}
	else
	{
		if ( g_SysCfg.m_nLayerCnt < nLayer )
		{
			nLayer = g_SysCfg.m_nLayerCnt;
		}
		nRet = m_pMarkStroke(nStyle,LPVOID(pDot),nSize,nLayer);
	}

	delete[] pDot;
	pDot = NULL;
	
	if ( LOWORD(nRet) > 10 )
		OutputMsg(nRet);
	
	if ( 0 == LOWORD(nRet) || LOWORD(nRet) > 10 )// 继续打标
		return HLOK;
	else
		return HLERROR;
	
	return HLERROR;
}


UINT CCard::SelMark(CStroke* pStroke, int nLayer, HWND hWnd, HANDLE hSelStop, UINT nStyle, BOOL bIsRevMark)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( !m_pMarkStroke || !pStroke )
		return HLERROR;
	
	UINT nSize = pStroke->m_dotArray.GetSize();
	if ( nSize  <=  0 )	
		return HLOK;
	
	if ( WAIT_OBJECT_0 == WaitForSingleObject( hSelStop, 0) )
		return 	HLERROR;
	
	DOT6* pDot = new DOT6[nSize];
	if ( NULL == pDot )
	{
		OutputDebugString(_T("CCard::SelMark() Allocate memory error!\n"));
		return HLERROR;
	}
	
	memset(pDot, 0, nSize*sizeof(DOT6));
	CDot tmp(0,0,0);
	if ( !bIsRevMark )
	{
		for( UINT i = 0; i < nSize; i++ )
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[i].x = tmp.x;
			pDot[i].y = tmp.y;
			pDot[i].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
		}
	}
	else
	{
		int nI = 0;
		for( int i = (nSize - 1); i  >=  0; i-- )
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[nI].x = tmp.x;
			pDot[nI].y = tmp.y;
			pDot[i].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
			nI++;
		}
	}
	
	if ( g_SysCfg.m_nLayerCnt < nLayer )
		nLayer = g_SysCfg.m_nLayerCnt;
	
	UINT  nRet = m_pMarkStroke( 0, LPVOID(pDot), nSize, nLayer );
    
	if (  LOWORD(nRet) > 10 )
		OutputMsg(nRet);

	delete[] pDot;
	pDot = NULL;
	
	if ( LOWORD(nRet) > 10 || LOWORD(nRet) == 0 )
	{
		return HLOK;//继续打标
	}
	else
	{
		return HLERROR;
	}
}


UINT CCard::SelMarkStroke(CStroke* pStroke, int nLayer, HWND hWnd, BOOL bIsRevMark)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( !pStroke || !m_pMarkStroke  )
		return HLERROR;
	
	UINT nSize = pStroke->m_dotArray.GetSize();
	if ( nSize  <=  0 )
		return HLOK;
	
	DOT6* pDot =  new DOT6[nSize];
	if ( NULL == pDot ) 
	{
	    OutputDebugString(_T("CCard::SelMarkStroke() Allocate memory failure..\n"));
		return HLERROR;
	}

	CDot tmp(0,0,0);
	memset(pDot, 0, nSize*sizeof(DOT6));

	if ( !bIsRevMark ) 
	{
		for(UINT i = 0; i<nSize; i++)
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[i].x = tmp.x;
			pDot[i].y = tmp.y;
			pDot[i].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
		}
	}
	else
	{
		int nI = 0;
		for(int i = (nSize - 1); i  >=  0; i--)
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[nI].x = tmp.x;
			pDot[nI].y = tmp.y;
			pDot[i].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
			nI++;
		}
	}

	if ( g_SysCfg.m_nLayerCnt < nLayer ) 
		nLayer = g_SysCfg.m_nLayerCnt;

	UINT nRet = m_pMarkStroke(0,LPVOID(pDot),nSize, nLayer);
    
	delete[] pDot;
	pDot = NULL;
	
	if ( LOWORD(nRet) > 10 || LOWORD(nRet) == 0 )
	{
		if ( LOWORD(nRet) > 10 )
			OutputMsg(nRet);
		
		return HLOK;//继续打标
	}
	else
	{
		return HLERROR;
	}

	return HLERROR;
}


void CCard::StartMark()
{
}


UINT CCard::PauseMark()
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pMarkPause == NULL )
		return HLERROR;

	Lock  tmp(&m_LockMark);
	UINT nRet = m_pMarkPause();
	if ( LOWORD(nRet) == HLOK )
	{
		m_bTimePaused =  TRUE;
		return HLOK;
	}
	return HLERROR;
#endif
}


UINT CCard::ReMark()
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pMarkReMark == NULL )
		return HLERROR;
	
	Lock  tmp(&m_LockMark);
	UINT nRet = m_pMarkReMark();
	if ( LOWORD(nRet)  !=  HLOK )
	{
		OutputMsg(nRet);
		return HLERROR;
	}
	m_bTimePaused = FALSE;
	return HLOK;
#endif
}


void CCard::StopMark()
{
#ifdef _SOFTMODE_EDITOR_
	return;
#else
	if ( m_pMarkStop == NULL )
		return;

	Lock tmp(&m_LockMark);
	UINT nRet = m_pMarkStop();
	if ( LOWORD(nRet)  !=  HLOK )
	{
		OutputMsg(nRet);
		return;
	}
	m_bTimePaused = TRUE;
#endif
}


BOOL CCard::EnterMarkEnv()
{
#ifdef _SOFTMODE_EDITOR_
	return FALSE;
#else
	if ( m_pEnterMarkEnv == NULL )
		return HLERROR;
	
	if ( m_lay.GetSize()  <=  0 )
		return HLERROR;

	CLayer ly = m_lay.GetLayer(0);
	SetCurrent(ly.m_para.emcc.dbCurrent);

	UINT nRet = m_pEnterMarkEnv();
	if ( LOWORD(nRet)  !=  HLOK )
	{
		TCHAR szMsg[256] = { 0 };
		if ( HLOK  !=  g_DAT.GetErrInfo(nRet, szMsg, 256) ) 
			return FALSE;
		///////
		AfxMessageBox(szMsg);
	}
		
	return TRUE;
#endif
}


BOOL CCard::LeaveMarkEnv()
{
#ifdef _SOFTMODE_EDITOR_
	return FALSE;
#else
	UINT nRet = m_pLeaveMarkEnv();
	if ( LOWORD(nRet)  !=  HLOK )
		OutputMsg(nRet);

	OutStatus(O_ENT_MARK_ENV,0);
	return TRUE;
#endif
}


DWORD CCard::GetMarkTime()
{
	if ( m_pGetMarkTime == NULL )
		return 0;
	
	if ( m_bTimePaused )
	{
		return m_dwEnd-m_dwStart;
	}
	else
	{
		UINT64 nTime = 0;
		UINT nRet = m_pGetMarkTime(&nTime);
		if ( LOWORD(nRet)  !=  HLOK )
		{
			OutputMsg(nRet);
		}
		return (DWORD)(nTime/1000);
	}
	
	return 0;
}


void CCard::SetCurrent( double dbCurrent )
{
	if ( !m_pSetCurrent )
		return;
	
	UINT nRet = m_pSetCurrent(dbCurrent);		
	if ( LOWORD(nRet)  !=  HLOK )
		OutputMsg(nRet);
}


void CCard::GetMarkRect(double& x,double& y)
{
#ifdef _SOFTMODE_EDITOR_
	x = 160;
	y = 160;
#else
	if ( m_pGetMarkRange != NULL )
		m_pGetMarkRange(&x,&y);
#endif
}

CString CCard::GetCardVer()
{
#ifdef _SOFTMODE_EDITOR_
	return _T("Demo version");
#else
	
	CString strRtn(_T("Hardware Ver: "));
	CString strVer(_T(""));
	
	if ( m_pGetHdVerInfoW == NULL || m_pGetHdVerInfo == NULL )
		return strRtn;
	
	TCHAR szHdVer[200] = {0};
	UINT nRet = HLERROR;
	
#ifdef _UNICODE
	nRet = m_pGetHdVerInfoW(szHdVer, 200);
#else
	nRet = m_pGetHdVerInfo(szHdVer, 200);
#endif
	
	if ( LOWORD(nRet)  !=  HLOK )
	{
		OutputMsg(nRet);
		return strRtn;
	}
	
	strVer = szHdVer;
	strRtn  +=  strVer;
	
	DWORD dwVer = 0;
	nRet = m_pGetSwVer(&dwVer);
	if ( LOWORD(nRet)  !=  HLOK )
	{
		OutputMsg(nRet);
	}
	strVer.Format( _T("\nDll Ver: %d.%d.%d.%d"),	(BYTE)((dwVer&0xFF000000) >> 24),
																	(BYTE)((dwVer&0x00FF0000) >> 16),
																	(BYTE)((dwVer&0x0000FF00) >> 8),
																	(BYTE)(dwVer&0x000000FF) );

	strRtn  +=  strVer;

	return strRtn;

#endif
}

UINT CCard::GetErrInfo(int nErr,TCHAR* pInfo,int nLen)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if (m_pGetErrInfoW == NULL || m_pGetErrInfo == NULL)
		return HLERROR;

#ifdef _UNICODE
	return m_pGetErrInfoW(nErr, pInfo, nLen);
#else
	return m_pGetErrInfo(nErr, pInfo, nLen);
#endif
}


void CCard::SetMarkLayNum(int nLayNum, BOOL bIsRevMark)
{
	m_nMarkLayNum = nLayNum;
	m_bIsResMark  = bIsRevMark;
}


void CCard::OutputMsg( UINT nErrCode, UINT nValue )
{
	CWnd *pWnd = AfxGetApp()->m_pMainWnd;
	if ( pWnd )
	{
		::PostMessage(pWnd->m_hWnd, WM_DISPLAY_CARD_STATUS, nErrCode, nValue);
	}
	
	if ( m_hMarkWnd )
	{
		::PostMessage( m_hMarkWnd, WM_OUTPUT_MARK_MSG, nErrCode, nValue );
	}
}



void CCard::DispCardInfo(UINT nErrCode, UINT nValue)
{
	CWnd *pWnd = AfxGetApp()->m_pMainWnd;
	
	if ( NULL == pWnd )
		return;
	
	::PostMessage(pWnd->m_hWnd, WM_DISPLAY_INFOR, nErrCode, nValue);
}



UINT CCard::BmpStart( BMPSPEC* pBmpSpec )
{
	if ( m_pSetGrayMarkPara == NULL )
		return HLERROR;
	
	return m_pSetGrayMarkPara(pBmpSpec);
}


UINT CCard::SetWorkStatus(int nStatus)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pSetWorkStatus == NULL )
		return HLERROR;

	UINT nRet = m_pSetWorkStatus(nStatus);
	if ( LOWORD(nRet)  !=  HLOK )
		OutputMsg( nRet );
	
	return nRet;
}


//
UINT CCard::GetWorkStatus(int *pStatus)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif
	
	if ( m_pGetWorkStatus == NULL )
		return HLERROR;
	
	UINT nRet = m_pGetWorkStatus(pStatus);
	if ( LOWORD(nRet)  !=  HLOK )
	{
		TCHAR szMsg[255] = {0};
		nRet = g_DAT.GetErrInfo(nRet, szMsg, 255);
		if ( nRet == HLERROR )
		{
			return FALSE;
		}
		AfxMessageBox(szMsg);
	}
	
	return nRet;
}

DWORD CCard::GetCardNumber()
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pGetCardNumber == NULL )
		return HLERROR;

	DWORD dwCount = 0;
	m_pGetCardNumber(&dwCount);

	return dwCount;
}

void CCard::SetCardIndex(DWORD dwIndex)
{
#ifdef _SOFTMODE_EDITOR_
	return;
#endif
	
	if ( m_pSetCardIndex == NULL )
		return;

	m_pSetCardIndex(dwIndex);
	m_dwCardSet = dwIndex;
}

UINT CCard::ClearOfflineFlag()
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pClearOfflineFlag == NULL )
		return HLERROR;

	return m_pClearOfflineFlag();
}

BOOL CCard::Init_SetLayerPara()
{
	if ( !LoadCardLib() )
		return FALSE;
	
	E_LASER_TYPE eEMCCLaser;
	if ( HLOK != m_pGetLayerParaType((int*)&eEMCCLaser) )
		return FALSE;

	switch(eEMCCLaser)
	{	
		case eType_CO2_GENERAL:
			m_strEMCCLaser = _T("CO2_GENERAL");
			break;
		case eType_CO2_CLASSIC:
			m_strEMCCLaser = _T("CO2_CLASSIC");
			break;
		
		case eType_Q_AP_DQ_CLASSIC:	//普通电源＋数字Q 传统模式
			m_strEMCCLaser = _T("Q_AP_DQ_CLASSIC");
			break;
		
		case eType_Q_AP_AQ_CLASSIC:	//普通电源＋模拟Q 传统模式
			m_strEMCCLaser = _T("Q_AP_AQ_CLASSIC");
			break;
		
		case eType_Q_DP_DQ_CLASSIC:	//数字电源＋数字Q 传统模式
			m_strEMCCLaser = _T("Q_DP_DQ_CLASSIC");
			break;
		
		case eType_Q_DP_AQ_CLASSIC:	//数字电源＋模拟Q 传统模式
			m_strEMCCLaser = _T("Q_DP_AQ_CLASSIC");
			break;
		
		case eType_Q_AP_AQ_GENERAL:	//普通电源＋模拟Q 通用模式
			m_strEMCCLaser = _T("Q_AP_AQ_GENERAL");
			break;
			
		case eType_Q_DP_DQ_GENERAL: //数字电源＋数字Q 通用模式
			m_strEMCCLaser = _T("Q_DP_DQ_GENERAL");
			break;
		
		case eType_Q_DP_AQ_GENERAL:	//数字电源＋模拟Q 通用模式
			m_strEMCCLaser = _T("Q_DP_AQ_GENERAL");
			break;
		
		case eType_IPG:
			m_strEMCCLaser = _T("IPG");
			break;

		case eType_SPI_GENERAL:
			m_strEMCCLaser = _T("SPI_GENERAL");
			break;

		case eType_SPI_CLASSIC:
			m_strEMCCLaser = _T("SPI_CLASSIC");
			break;

		case eType_LD:
			m_strEMCCLaser = _T("LD");
			break;

		case eType_IPGM:
			m_strEMCCLaser = _T("M_IPG");
			break;

		case eType_IPGM_GENERAL:
			m_strEMCCLaser = _T("M_IPG_GENERAL");
			break;
	}
	
	if ( EMCC == g_nCardType )
	{
		InitLayerCfg();
	}	

	return TRUE;
}


BOOL CCard::ClearMulDocData()
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pClearMulDocData == NULL )
		return HLERROR;

	return (HLOK == m_pClearMulDocData());
}


BOOL CCard::SetMulMarkStatus(int nStatus)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pSetMulMarkStatus == NULL )
		return HLERROR;

	return (HLOK == m_pSetMulMarkStatus(nStatus));
}


UINT CCard::MarkDelay(DWORD dwDelay)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif
	if ( m_pMarkDelay == NULL )
		return HLERROR;
	
	return m_pMarkDelay(dwDelay);
}


UINT CCard::MarkWaitSignal(DWORD dwSignal,DWORD dwMask)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pMarkWaitSignal == NULL )
		return HLERROR;

	return m_pMarkWaitSignal(dwSignal, dwMask);
}


UINT CCard::MarkOutSignal(DWORD dwSignal,DWORD dwMask,DWORD dwDealy)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#endif

	if ( m_pMarkOutSignal == NULL )
		return HLERROR;

	return m_pMarkOutSignal(dwSignal, dwMask, dwDealy);
}

//
BOOL CCard::IsCheckStatusOK()
{
#ifdef _SOFTMODE_EDITOR_
	return TRUE;
#else
	int nStatus = -1;
	if ( HLOK  !=  GetWorkStatus(&nStatus) )
		return FALSE;
	
	if ( 0 == nStatus )
	{
		if ( IDNO == AfxMessageBox(IDS_OFFLINETOONLINE, MB_YESNO) )
			return FALSE;
		
		if ( HLOK  !=  SetWorkStatus(1) )
			return FALSE;
	}
	return TRUE;

#endif
}


//缓冲区运动指令
UINT CCard::MarkBufferMove(LP_MOTOR_PARA lpMovePara)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pMarkBufferMove == NULL )
		return HLERROR;

	return m_pMarkBufferMove((void*)lpMovePara);
#endif
}


//立即运动指令
UINT CCard::MotorMove(LP_MOTOR_PARA lpMovePara)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pMotorMove == NULL )
		return HLERROR;

	return m_pMotorMove((void*)lpMovePara);
#endif
}


UINT CCard::SetCardTouchMode(int nType)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pSetTouchMode == NULL )
		return HLERROR;

	if ( nType == 0 ) //普通触发
	{
		return m_pSetTouchMode(0,CardTouchMark);
	}
	else //下位机触发
	{
		return m_pSetTouchMode(2,CardTouchMark);
	}
#endif
}


UINT CCard::MarkSetBufferPos(int nAxis, int nPos)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pMarkSetBufferPos == NULL )
		return HLERROR;

	return m_pMarkSetBufferPos(nAxis, nPos);
#endif
}


UINT CCard::GetCardKey(TCHAR* pDogStr, int &nBufSize)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pGetCardKey == NULL || m_pGetCardKeyW == NULL)
		return HLERROR;

	#ifdef _UNICODE
		return m_pGetCardKeyW(pDogStr, nBufSize);
	#else
		return m_pGetCardKey(pDogStr, nBufSize);
	#endif

#endif
}


UINT CCard::MarkCheckSwitch(int nCheckFlag,DWORD dwSignal,DWORD dwMask)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pMarkCheckSwitch == NULL )
		return HLERROR;

	return m_pMarkCheckSwitch(nCheckFlag,dwSignal,dwMask);
#endif
}

UINT CCard::HomeMove(LP_MOTOR_PARA pMotor,BOOL bSoftHome)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pHomeMove == NULL )
		return HLERROR;

	return m_pHomeMove(pMotor, bSoftHome);
#endif
}


UINT CCard::SetAsSoftHome(int nAxis)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pSetAsSoftHome == NULL )
		return HLERROR;

	return m_pSetAsSoftHome(nAxis);

#endif
}


UINT CCard::EnumCardKey(TCHAR pCardID[][50],int& nNumber)
{
#ifdef _SOFTMODE_EDITOR_
	return 0;
#else
	if ( m_pEnumCardKey == NULL || m_pEnumCardKeyW == NULL )
		return HLERROR;
	
#ifdef  _UNICODE
	return m_pEnumCardKeyW(pCardID,nNumber);
#else
	return m_pEnumCardKey(pCardID,nNumber);
#endif

#endif
}


UINT CCard::GetCurMotorPos(int nAxis, LONGLONG& nPos)
{
	if ( m_pGetCurMotorPos == NULL )
		return HLERROR;
	
	return m_pGetCurMotorPos( nAxis, nPos );
}

//
UINT CCard::MarkFillStroke(CStroke* pStroke,HWND hWnd,int nStyle, BOOL bAlign, CDot dotBase )
{
	if ( WAIT_OBJECT_0 == ::WaitForSingleObject(hStopWork,0) || !m_pMarkFillStroke || !pStroke )
		return HLERROR;
	
	UINT nRet = 0;
	int nLayer  = GetLayerNum(pStroke->m_property);	//当前笔画层号
	int nSize = pStroke->m_dotArray.GetSize();
	if ( nSize  <=  0 )
		return HLOK;
	
	DOT6* pDot = new DOT6[nSize];
	if ( NULL == pDot ) 
	{
		OutputDebugString(_T("CCard::MarkStroke() Allocate memory fail!\n"));
		return HLERROR;
	}
	
	memset(pDot, 0, nSize*sizeof(DOT6));
	CDot tmp;
	
	if ( !m_bIsResMark ) 
	{	
		for( int i = 0; i < nSize; i++ )
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[i].x = tmp.x;
			pDot[i].y = tmp.y;
			pDot[i].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
		}
	}
	else
	{
		int nI = 0;
		for( int i = (nSize-1); i  >=  0; i-- )
		{
			tmp = (pStroke->m_dotArray.GetAt(i));
			pDot[nI].x = tmp.x;
			pDot[nI].y = tmp.y;
			pDot[nI].z = (float)(m_bEnableZOffset ? g_SysCfg.m_dbZOffset+tmp.z : tmp.z);
			nI++;
		}
	}
	
	//红光预览层号选择
	if ( TRUE == m_bRedPreview )
	{
		m_nMarkLayNum = g_SysCfg.m_nRedPreviewLayerNo  >= 0 ? g_SysCfg.m_nRedPreviewLayerNo : 0;
	}
	
	//
	DOT6 *pDotBase = new DOT6();
	if ( pDotBase == NULL )
	{
		delete pDot;
		pDot = NULL;
		OutputDebugString(_T("CCard::MarkStroke() Allocate memory fail!\n"));
		return HLERROR;
	}

	pDotBase->x = dotBase.x;
	pDotBase->y = dotBase.y;
	pDotBase->z = (float)(dotBase.z);

	if ( m_nMarkLayNum  >=  0 )
	{	
		if ( g_SysCfg.m_nLayerCnt < m_nMarkLayNum )
		{
			m_nMarkLayNum = g_SysCfg.m_nLayerCnt;
		}
		
		nRet = m_pMarkFillStroke(LPVOID(pDot), nSize, m_nMarkLayNum, bAlign, LPVOID(pDotBase));
	}
	else
	{
		if ( g_SysCfg.m_nLayerCnt < nLayer )
		{	
			nLayer = g_SysCfg.m_nLayerCnt;
		}
		nRet = m_pMarkFillStroke(LPVOID(pDot), nSize, nLayer, bAlign, LPVOID(pDotBase));
	}

	delete   pDotBase;
	delete[] pDot;
	
	if ( LOWORD(nRet) > 10 )
		OutputMsg(nRet);
	
	if ( 0 == LOWORD(nRet) || LOWORD(nRet) > 10 )// 继续打标
		return HLOK;
	else
		return HLERROR;
	
	return HLERROR;
}


UINT CCard::ImGotoXYZ(int nType, double dbX, double dbY, double dbZ)
{
	if ( m_pImGotoXYZ == NULL )
		return HLERROR;

	DOT6  para;
	memset( &para,0,sizeof(DOT6) );
	/////////
	para.x = (float)dbX;
	para.y = (float)dbY;
	para.z = (float)dbZ;
	
	return m_pImGotoXYZ(nType, (void*)&para);
}


UINT CCard::SetCardParaFile(TCHAR *pFileName)
{
	if ( m_pSetCardParaFile == NULL || m_pSetCardParaFileW == NULL )
		return HLERROR;
	
#ifdef  _UNICODE
	return m_pSetCardParaFileW(pFileName);
#else
	return m_pSetCardParaFile(pFileName);
#endif
}


UINT CCard::SetCaliParaFile(TCHAR* pFileName)
{
	if ( m_pSetCaliParaFile == NULL ||m_pSetCaliParaFileW == NULL )
		return HLERROR;

#ifdef  _UNICODE
	return m_pSetCaliParaFileW(pFileName);
#else
	return m_pSetCaliParaFile(pFileName);
#endif
}


UINT CCard::MarkBufferLaser( LP_LASER_PARA pLaserPara )
{
	if ( !m_pMarkBufferLaser )
		return HLERROR;

	return m_pMarkBufferLaser( (LPVOID) pLaserPara );
}


UINT CCard::GetLayerParaType( int &nType )
{
	if ( !m_pGetLayerParaType )
		return HLERROR;
	
	return m_pGetLayerParaType( &nType );
}


UINT CCard::MarkBufferCamera( LP_CAMERA_PARA pCameraPara )
{
	if ( !m_pMarkBufferCamera )
		return HLERROR;

	return m_pMarkBufferCamera( (LPVOID)pCameraPara );
}

UINT CCard::MarkBufferDynCali( BOOL bEnable )
{
	if ( !m_pMarkBufferDynCali )
		return HLERROR;
	
	return m_pMarkBufferDynCali( bEnable );
}

/*
UINT CCard::SetLimitBox(double dbXMin, double dbXMax, double dbYMin, double dbYMax)
{
	if( !m_pSetLimitBox )
		return HLERROR;

	return m_pSetLimitBox(dbXMin,dbXMax,dbYMin,dbYMax);
}

UINT CCard::SetMotorRatio(double dbInput, double dbOutput)
{
	if( !m_pSetMotorRatio )
		return HLERROR;

	return m_pSetMotorRatio(dbInput,dbOutput);
}

UINT CCard::SetCircleCali(double dbRadius, double dbMin,double dbMax)
{
	if( !m_pSetCircleCali )
		return HLERROR;

	return m_pSetCircleCali(dbRadius,dbMin,dbMax);
}
*/

BOOL CCard::CheckSoftSpFunc()
{
#ifdef _DEBUG
	g_DogCheck.m_bDebugDog = TRUE;
#endif
	//取得当前版本的扩展功能信息
	if ( !g_DogCheck.m_bDebugDog )//运行狗
	{
		if ( !g_DogCheck.__GetFeeFlag(m_btExtendInfo.bFunc, 200)  )
		{
			AfxMessageBox(_T("Get function flag failure..\n"));
			return FALSE;
		}
	}
	else
	{
		memset(m_btExtendInfo.bFunc,1,200);
	}

	return TRUE;
}