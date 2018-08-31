// System.cpp: implementation of the CSystemPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "System.h"
#include "LangCodePage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
 
CSystem g_sys;

extern SYSCFG g_SysCfg;
extern CArray<USERINFO, USERINFO> g_RightInfoList; // 用户权限列表

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystem::CSystem()
{
	m_nDibSampleMode = 1;
	m_nIOSelCount = 4;
	m_bEnableUpdate = TRUE;
}



CSystem::~CSystem()
{

}


BOOL CSystem::ReadSysCfg()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szFileName[MAX_PATH] = { 0 };
	//
	GetExepath(szPath,MAX_PATH);
	g_StrCopyTmpFile = szPath;
	g_StrCopyTmpFile += _T("TMPDATA.DAT");
	_stprintf_s(szFileName,  MAX_PATH,_T("%s%s"), szPath, _T("SysCfg.ini"));
	_tcscpy_s(m_szFileName,256, szFileName);
	
	TCHAR szCardName[20] = {0};
	DWORD dwRet = ::GetPrivateProfileString(_T("Card"), _T("CardName"), _T("EMCC"), szCardName, 20, szFileName);
	if ( dwRet  <=  0 )
	    return	FALSE;
	
	CString strCardName(szCardName);
	if ( 0 == strCardName.CompareNoCase(_T("EMCC")) )
	{
		g_nCardType = EMCC;
	}
	else
	{
		g_nCardType = -1;
		return FALSE;
	}
	
	_tcsncpy(g_SysCfg.m_szCardName, szCardName, 20);
	//
	g_SysCfg.m_dbDotFilterPrecis		 =  GetProfileDouble( _T("EditCfg"), _T("PointFilterPrecis"),  0.1);
    g_SysCfg.m_dbDotFilterAngle		 =  GetProfileDouble( _T("EditCfg"), _T("PointFilterAngle"),   45);
	g_SysCfg.m_dbDotMoveStepLen	 =  GetProfileDouble( _T("EditCfg"), _T("PointPrecisMoveStep"),1);
	g_SysCfg.m_dbMicro					 =  GetProfileDouble( _T("EditCfg"), _T("MICRO"),				2);
	g_SysCfg.m_dbClick					 =  GetProfileDouble( _T("EditCfg"), _T("CLICK"),				5);
	g_SysCfg.m_nUndoCount			 =  GetProfileInt(      _T("EditCfg"), _T("UNDO"),				10);	
	g_SysCfg.m_dbOneToTwo			 =  GetProfileDouble( _T("EditCfg"), _T("ONETOTWO"),		0.1);
	g_SysCfg.m_dbExtendSpace		 =  GetProfileDouble( _T("EditCfg"), _T("ExtendSpace"),		-0.1);
    g_SysCfg.m_dbKillAngleSpace		 =  GetProfileDouble( _T("EditCfg"), _T("KillAngleSpace"),	0.01);
	g_SysCfg.m_dbJoinDistance		 =  GetProfileDouble( _T("EditCfg"), _T("JoinDistance"),0.01);	//曲线合并时的距离限制
	g_SysCfg.m_nLayerCnt				 =  GetProfileInt( _T("EditCfg"), _T("LayerCnt"), 10);	
	if ( g_SysCfg.m_nLayerCnt > LAYERCOUNT )
		g_SysCfg.m_nLayerCnt = LAYERCOUNT;

	g_SysCfg.m_dbPreViewZoomScale  = GetProfileDouble( _T("EditCfg"), _T("PreViewZoomScale"),1.2);//预览鼠标缩放比例
	g_SysCfg.m_bFillOptimize = GetProfileInt(_T("EditCfg"), _T("FillOptimize"), 0); //填充优化
	
	//绘图参数
	g_SysCfg.m_dbGraphImportPrics	 =  GetProfileDouble( _T("SysCfg"), _T("GraphImportPrics"),	0.01);
    g_SysCfg.m_dbDrawEllipSet	 =  GetProfileDouble( _T("SysCfg"), _T("DrawEllipSet"), 0.25);
	g_SysCfg.m_nGridCol				 =  GetProfileInt( _T("SysCfg"), _T("DrawGridCOL"),3);
	g_SysCfg.m_nGridRow			 =  GetProfileInt( _T("SysCfg"), _T("DrawGridRow"), 3);
	
	g_SysCfg.m_nPolygon			 =  GetProfileInt( _T("SysCfg"), _T("POLYGON"), 0);
	g_SysCfg.m_nPolyNum			 =  GetProfileInt( _T("SysCfg"), _T("POLYNUM"), 5);
	g_SysCfg.m_nPolyCross			 =  GetProfileInt( _T("SysCfg"), _T("POLYCROSS"), 1);
	g_SysCfg.m_nPolyStar			 =  GetProfileInt( _T("SysCfg"), _T("POLYSTAR"), 50);
	g_SysCfg.m_dbZOffset			 =  GetProfileDouble( _T("SysCfg"), _T("MarkZOFFSET"), 0);
	g_SysCfg.m_dbPrecision		 =  GetProfileDouble( _T("SysCfg"), _T("CIRCLEPRECIS"), 0.1);
	g_SysCfg.m_dbDotmatrixCirPrec		 =  GetProfileDouble( _T("SysCfg"), _T("DotmatrixCirPrec"), 0.01);
	g_SysCfg.m_nSampleCount	 =  GetProfileInt( _T("SysCfg"), _T("SAMPLECOUNT"), 100);

	m_bDrawCrossLine		=  ReadIniInt(szFileName, _T("SysCfg"), _T("DrawCrossLine"), _T("1"));
	m_bFastDrawEnable		= ReadIniInt(szFileName, _T("SysCfg"), _T("FastDrawEnable"), _T("0"));
	m_dbFastDrawDistance	= GetProfileDouble( _T("SysCfg"), _T("FastDrawDistance"), 0.05);
	
	// add by zhq
	g_SysCfg.m_dbInterSpace		= GetProfileDouble( _T("SysCfg"), _T("INTERSPACE"), 0.1);				// 交叉滤点长度
	g_SysCfg.m_nCatchPrecision	= GetProfileInt( _T("SysCfg"), _T("CATCHPRECISION"), 3);			// 捕获点精度,取值范围只能是1-5mm,精度越小捕获的越精确

	//
	g_SysCfg.m_dbLineWidth		= GetProfileDouble( _T("SysCfg"), _T("BroadenR"), 0.1); // 加粗的半径
	g_SysCfg.m_dbBroadenD		= GetProfileDouble( _T("SysCfg"), _T("BroadenD"), 0.05); // 加粗的间距

	//
	g_SysCfg.m_dbDiameter		=	GetProfileDouble(_T("WheelSet"),_T("WheelDiameter"),80.0);
	g_SysCfg.m_dbDivAngle		=	GetProfileDouble(_T("WheelSet"),_T("DivAngle"),   1.0);
	g_SysCfg.m_nFillNum			=	GetProfileInt( _T("WheelSet"),  _T("DivFillNum"), 1);

	//
	g_SysCfg.m_sMoveMarkParam.m_nMode			=	GetProfileInt(_T("MoveMarkParam"),_T("Mode"),0);			
	g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter	=	GetProfileDouble(_T("MoveMarkParam"),_T("WheelDiameter"),80);			
	g_SysCfg.m_sMoveMarkParam.m_dbDivAngle		=	GetProfileDouble(_T("MoveMarkParam"),_T("DivAngle"),1);		
	g_SysCfg.m_sMoveMarkParam.m_dbExcursionX	=	GetProfileDouble(_T("MoveMarkParam"),_T("ExcursionX"),0);				
	g_SysCfg.m_sMoveMarkParam.m_dbExcursionY	=	GetProfileDouble(_T("MoveMarkParam"),_T("ExcursionY"),0);
	g_SysCfg.m_sMoveMarkParam.m_dbDivDx			=	GetProfileDouble(_T("MoveMarkParam"),_T("DivDx"),10);				
	g_SysCfg.m_sMoveMarkParam.m_dbDivDy			=	GetProfileDouble(_T("MoveMarkParam"),_T("DivDy"),10);
	g_SysCfg.m_sMoveMarkParam.m_dbPrecisionK	=	GetProfileDouble(_T("MoveMarkParam"),_T("Precision_K"),100);
	g_SysCfg.m_sMoveMarkParam.m_nEncodePPR		=	GetProfileInt(_T("MoveMarkParam"),_T("EncodePPR"),2000);
	g_SysCfg.m_sMoveMarkParam.m_nMotorDelay			=	GetProfileInt(_T("MoveMarkParam"),_T("MotorDelay"),100);


	//
	g_SysCfg.m_nRedPreviewLayerNo = GetProfileInt( _T("MarkCfg"), _T("RedPreviewLayerNo"),0);   //红光预览时对应的层号
	g_SysCfg.m_bCloseRedIfMark = GetProfileInt( _T("MarkCfg"), _T("CloseRedIfMark"),1);				//打标时是否关红光
	g_SysCfg.m_nSafeDoorRemark =  GetProfileInt( _T("MarkCfg"), _T("SafeDoorRemark"), 0);

	//
	g_SysCfg.m_db3dModelPrecision = GetProfileDouble( _T("3DCfg"),  _T("3dModelPrecision"), 0.5);
	g_SysCfg.m_db3dTrancePrecision = GetProfileDouble( _T("3DCfg"),  _T("3dTrancePrecision"), 0.1);
	g_SysCfg.m_n3DOptimize = GetProfileInt( _T("3DCfg"),  _T("STLOptimize"), 0);
	
	TCHAR szSysSetFile[MAX_PATH];
	_stprintf_s(szSysSetFile,  MAX_PATH,_T("%s%s\\%s"), szPath, szCardName, _T("SystemSet.ini"));
	m_strCardFile.Format(_T("%s"), szSysSetFile); //卡参数文件
	m_strCaliPath.Format(_T("%s%s"),szPath,szCardName);	 //设定校正文件路径
	
	//接口信号配置
	m_nTouchTimes	 =  ReadIniInt(szSysSetFile, _T("TOUCH"), _T("CYCLE"),_T("1"));
	m_bMarkOnce		  = ReadIniInt(szSysSetFile, _T("TOUCH"), _T("B_MARK_ONCE"),_T("1"));
	m_nDelayBefore	 =  ReadIniInt(szSysSetFile, _T("TOUCH"), _T("DELAY"),_T("0"));
	m_nMarkRepeat	 =  ReadIniInt(szSysSetFile, _T("TOUCH"), _T("MARK_CNT"),_T("1"));
	m_nDelayAfter		 =  ReadIniInt(szSysSetFile, _T("TOUCH"), _T("MARK_END_DELAY"),_T("0"));
	m_nDelayBetween =  ReadIniInt(szSysSetFile, _T("TOUCH"), _T("DLY_BETWEEN_MARK"),_T("0"));
	m_nDelayEnd	     = ReadIniInt(szSysSetFile, _T("TOUCH"), _T("MARK_END_PULSE_W"),_T("0"));
	m_bLoopMdt		 =  ReadIniInt(szSysSetFile, _T("TOUCH"), _T("ENABLE_LOOP_MDT"),_T("0"));
	m_nIOSelCount	 =  ReadIniInt(szSysSetFile, _T("TOUCH"), _T("IOSELCOUNT"), _T("4"));
	m_nCardTouchModel = ReadIniInt(szSysSetFile, _T("TOUCH"), _T("ENABLE_CARDTOUCH"),_T("0"));
	//
	m_bDeviceDebug = ReadIniInt(szSysSetFile, _T("Machine"), _T("DEVICE_DEBUG"),_T("0"));
	//

	// 获取系统支持的代码页(即支持的语言包)
	EnumCodePages();

	return TRUE;
}

//
void CSystem::WriteSysCfg()
{
	// 绘图参数
    WriteProfileDouble( _T("SysCfg"), _T("DrawEllipSet"),g_SysCfg.m_dbDrawEllipSet);
    WriteProfileInt( _T("SysCfg"), _T("DrawGridCOL"),g_SysCfg.m_nGridCol);
    WriteProfileInt( _T("SysCfg"), _T("DrawGridRow"),g_SysCfg.m_nGridRow);
	
	WriteProfileInt( _T("SysCfg"), _T("POLYGON"),  g_SysCfg.m_nPolygon);
    WriteProfileInt( _T("SysCfg"), _T("POLYNUM"),  g_SysCfg.m_nPolyNum);
	WriteProfileInt( _T("SysCfg"), _T("POLYCROSS"),g_SysCfg.m_nPolyCross);
    WriteProfileInt( _T("SysCfg"), _T("POLYSTAR"), g_SysCfg.m_nPolyStar);
	
	WriteProfileDouble( _T("SysCfg"), _T("CIRCLEPRECIS"), g_SysCfg.m_dbPrecision);
	WriteProfileInt( _T("SysCfg"), _T("SAMPLECOUNT"), g_SysCfg.m_nSampleCount);

	WriteProfileDouble(_T("WheelSet"),_T("WheelDiameter"),g_SysCfg.m_dbDiameter);
	WriteProfileDouble(_T("WheelSet"),_T("DivAngle"),     g_SysCfg.m_dbDivAngle);
	WriteProfileInt( _T("WheelSet"),  _T("DivFillNum"),   g_SysCfg.m_nFillNum);

	
	WriteProfileInt( _T("MoveMarkParam"),  _T("Mode"),   g_SysCfg.m_sMoveMarkParam.m_nMode);
	WriteProfileDouble(_T("MoveMarkParam"),_T("WheelDiameter"),g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter);
	WriteProfileDouble(_T("MoveMarkParam"),_T("DivAngle"),g_SysCfg.m_sMoveMarkParam.m_dbDivAngle);
	WriteProfileDouble(_T("MoveMarkParam"),_T("ExcursionX"),g_SysCfg.m_sMoveMarkParam.m_dbExcursionX);
	WriteProfileDouble(_T("MoveMarkParam"),_T("ExcursionY"),g_SysCfg.m_sMoveMarkParam.m_dbExcursionY);

	WriteProfileDouble(_T("MoveMarkParam"),_T("DivDx") , g_SysCfg.m_sMoveMarkParam.m_dbDivDx);
	WriteProfileDouble(_T("MoveMarkParam"),_T("DivDy"), g_SysCfg.m_sMoveMarkParam.m_dbDivDy);
	WriteProfileDouble(_T("MoveMarkParam"),_T("Precision_K"), g_SysCfg.m_sMoveMarkParam.m_dbPrecisionK);
	WriteProfileInt( _T("MoveMarkParam"),  _T("EncodePPR"),   g_SysCfg.m_sMoveMarkParam.m_nEncodePPR);
	WriteProfileInt( _T("MoveMarkParam"),  _T("MotorDelay"),   g_SysCfg.m_sMoveMarkParam.m_nMotorDelay);
	
}

void CSystem::FlashCardPara()
{
	ReadSysCfg();
	//
	g_DAT.Reset();
	g_DAT.SetCardTouchMode(m_nCardTouchModel);
}


int CSystem::GetProfileInt(TCHAR *pSec, TCHAR *pKey, int nDefault)
{
	TCHAR szDefault[32] = {0};
	TCHAR szVal[50] = {0};

	::_stprintf_s(szDefault,32, _T("%d"),nDefault);
	::GetPrivateProfileString(pSec,pKey,szDefault,szVal, 50, m_szFileName);

	return _ttoi(szVal);
}



CString CSystem::GetIniString(TCHAR* pSec,TCHAR* pKey,TCHAR *pDefault)
{
	TCHAR szBuff[MAX_PATH] = { 0 };
	//
	::GetPrivateProfileString(pSec,pKey,pDefault,szBuff, MAX_PATH, m_szFileName);

	return CString(szBuff);
}

void CSystem::WriteIniString(TCHAR* pSec,TCHAR* pKey,TCHAR *pString)
{
	::WritePrivateProfileString(pSec,pKey,pString, m_szFileName);
}

double CSystem::GetProfileDouble(TCHAR* pSec,TCHAR* pKey,double dbDefault)
{
	TCHAR szDefault[32] = {0},szVal[100] = {0};
	_stprintf_s(szDefault,32,_T("%f"),dbDefault);
	
	DWORD dwRet = ::GetPrivateProfileString(pSec,pKey,szDefault,szVal, 100, m_szFileName);
	if (  dwRet  <=  0  )
		return dbDefault;
	
	return _tstof(szVal);
}


void  CSystem::WriteProfileDouble(TCHAR* pSec,TCHAR* pKey,double dbValue)
{
	TCHAR szbuff[50] = { 0 };
	_stprintf_s(szbuff,50, _T("%f"), dbValue);
	
	::WritePrivateProfileString(pSec, pKey, szbuff, m_szFileName);
}

void  CSystem::WriteProfileInt(TCHAR* pSec,TCHAR* pKey,int nValue)
{
	TCHAR szBuff[32] = { 0 };
	_stprintf_s(szBuff,32, _T("%d"), nValue);
	
	::WritePrivateProfileString(pSec, pKey, szBuff, m_szFileName);
}

CString CSystem::GetInstallPath()
{
	GetExepath(m_szExePath,MAX_PATH);
	//
	CString strPath(m_szExePath);

	return  strPath;
}

void CSystem::UpdateGrayCompensate()
{
	UINT nDest0 = GetProfileInt( _T("GrayCom"),_T("Dest0"),0);
	UINT nDest1 = GetProfileInt( _T("GrayCom"),_T("Dest1"),51);
	UINT nDest2 = GetProfileInt( _T("GrayCom"),_T("Dest2"),102);
	UINT nDest3 = GetProfileInt( _T("GrayCom"),_T("Dest3"),153);
	UINT nDest4 = GetProfileInt(_T("GrayCom"),_T("Dest4"),204);
	UINT nDest5 = GetProfileInt(_T("GrayCom"),_T("Dest5"),255);
	
	//线性插值
	double dbTmp = 0.0;
	int i = 0;
	for( i = 0; i < 51; i++ )
	{
		dbTmp =  double(i)/51.0*(double(nDest1)-nDest0)+nDest0;
		m_GrayCompensate[i] = BYTE (dbTmp+0.5);
	}

	for( i = 51; i<102; i++ )
	{	
		m_GrayCompensate[i] = BYTE (double(i-51)/(102-51)*(double(nDest2)-nDest1)+nDest1+0.5);
	}
	for( i = 102; i<153; i++ )
	{
		m_GrayCompensate[i] = BYTE (double(i-102)/(153-102)*(double(nDest3)-nDest2)+nDest2+0.5);
	}
	for ( i = 153; i<204; i++ )
	{
		m_GrayCompensate[i] = BYTE (double(i-153)/(204-153)*(double(nDest4)-nDest3)+nDest3+0.5);
	}
	for ( i = 204; i<256; i++ )
	{
		m_GrayCompensate[i] = BYTE (double(i-204)/(255-204)*(double(nDest5)-nDest4)+nDest4+0.5);
	}
}

//
UINT CSystem::ReadSysMarkPara()
{
	UpdateGrayCompensate();

	m_nDibSampleMode	 =  GetProfileInt(_T("Picture"),_T("Sample_Mode"),1);
	m_dbMinCurrent       = GetProfileInt(_T("Machine"),_T("POWER_MINCURRENT"),0);
	m_dbMaxCurrent      = GetProfileInt(_T("Machine"),_T("POWER_MAXCURRENT"),35);
	
	if ( m_nDibSampleMode  >=  3 || m_nDibSampleMode < 0 )
		m_nDibSampleMode = 0;
	
	return 0;
}


//
void CSystem::WriteRightInfo( CString &strFileName )
{
	m_RightMng.WriteRightInfo(g_RightInfoList, strFileName);
}


//
void CSystem::WriteSpMarkInfo( OBJMARKSET &objMarkSet, CString &strFileName )
{
	m_RightMng.WriteSpMarkInfo(objMarkSet, strFileName);
}


//
void CSystem::ReadRightInfo(CString &strFileName)
{
	m_RightMng.ReadRightInfo(g_RightInfoList, strFileName);
}


//
void CSystem::ReadSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFileName)
{
	m_RightMng.ReadSpMarkInfo(objMarkSet, strFileName);
}

//
BOOL CSystem::IsAvailUser(CString &strUserName, CString &strPassWord)
{
	int nCount = g_RightInfoList.GetSize();
	for( int i = 0; i < nCount; i++ )
	{
		USERINFO stUserInfo = g_RightInfoList.GetAt(i);
		if ( 0 == strUserName.CompareNoCase(stUserInfo.szUserName) && m_RightMng.InfoCheck(strPassWord, stUserInfo) )
		{
			g_CurUserInfo = stUserInfo;
			return TRUE;
		}
	}
	
	return FALSE;
}



BOOL CSystem::AddUser(USERINFO &stUserInfo)
{
	BOOL bFindUser = FALSE;
	int nCount = g_RightInfoList.GetSize();
	for( int i = 0; i < nCount; i++ )
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == _tcsncmp(stUserInfo.szUserName, stUInfo.szUserName, 64))
		{
			bFindUser = TRUE;
			break;
		}
	}
	
	if ( bFindUser )
	{
		AfxMessageBox(IDS_USERISINLIST);
		return FALSE;
	}
	else
	{
		g_RightInfoList.Add(stUserInfo);
		return TRUE;
	}
	
	return FALSE;
}



BOOL CSystem::DelUser(CString &strUserName)
{
	int nCount = g_RightInfoList.GetSize();
	for ( int i = 0; i < nCount; i++ )
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if ( 0 == strUserName.CompareNoCase(stUInfo.szUserName) )
		{
			g_RightInfoList.RemoveAt(i);
			break;
		}
	}
	
	return TRUE;
}




void CSystem::EditUserInfo(USERINFO &stUserInfo)
{
	int  nCount = g_RightInfoList.GetSize();
	for ( int i = 0; i < nCount; i++ )
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if ( 0 == _tcsncmp(stUserInfo.szUserName, stUInfo.szUserName, 64) )
		{
			_tcsncpy(g_RightInfoList[i].szUserName, stUserInfo.szUserName, 64);
			_tcsncpy(g_RightInfoList[i].szPassWord, stUserInfo.szPassWord, 64);
			g_RightInfoList[i].bIsDefaultUser = stUserInfo.bIsDefaultUser;
			g_RightInfoList[i].nFlag = stUserInfo.nFlag;
			g_RightInfoList[i].dwRight1 = stUserInfo.dwRight1;
			g_RightInfoList[i].dwRight2 = stUserInfo.dwRight2;
			break;
		}
	}
}




void CSystem::EditUserInfoByUserName(USERINFO &stUserInfo, CString &strUserName)
{
	BOOL bFindUser = FALSE;
	int nCount = g_RightInfoList.GetSize();
	for( int i = 0; i < nCount; i++ )
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == _tcsncmp(stUInfo.szUserName, strUserName, 64))
		{
			_tcsncpy(g_RightInfoList[i].szUserName, stUserInfo.szUserName, 64);
			_tcsncpy(g_RightInfoList[i].szPassWord, stUserInfo.szPassWord, 64);
			g_RightInfoList[i].bIsDefaultUser = stUserInfo.bIsDefaultUser;
			g_RightInfoList[i].dwRight1 = stUserInfo.dwRight1;
			g_RightInfoList[i].dwRight2 = stUserInfo.dwRight2;
			break;
		}
	}
}




BOOL CSystem::OnSetDefautUser(CString &strUserName, BOOL bSel)
{
	int nCount = g_RightInfoList.GetSize();
	for ( int i = 0; i < nCount; i++ )
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if ( 0 == strUserName.CompareNoCase(stUInfo.szUserName) ) 
		{
			g_RightInfoList[i].bIsDefaultUser = bSel;
		}
		else
		{
			g_RightInfoList[i].bIsDefaultUser = FALSE;
		}
	}

	if (0 == strUserName.CompareNoCase(g_CurUserInfo.szUserName)) 
	{
		g_CurUserInfo.bIsDefaultUser = bSel;
	}
	else
	{
		g_CurUserInfo.bIsDefaultUser = FALSE;
	}
	
	return FALSE;
}

BOOL CSystem::GetUserInfoByName(CString strUserName, USERINFO &stUserInfo)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strUserName.CompareNoCase(stUInfo.szUserName))
		{
			stUserInfo = stUInfo;
			return TRUE;
		}
	}
	
	return FALSE;
}

int CSystem::GetIndexByName(CString strUserName)
{
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if (0 == strUserName.CompareNoCase(stUInfo.szUserName))
		{
			return i;
		}
	}
	
	return 0;
}




BOOL CSystem::GetDefaultUserInfo(USERINFO &stUserInfo)
{
	int nCount = g_RightInfoList.GetSize();
	for ( int i = 0; i < nCount; i++ )
	{
		USERINFO stUInfo = g_RightInfoList.GetAt(i);
		if ( stUInfo.bIsDefaultUser )
		{
			stUserInfo = stUInfo;
			return TRUE;
		}
	}
	
	return FALSE;
}

