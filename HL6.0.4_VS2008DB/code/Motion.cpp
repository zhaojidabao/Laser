// Motion.cpp: implementation of the CMotion class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Motion.h"

CMotion g_Motion;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMotion::CMotion()
{
	m_bIsRun = FALSE;
}

CMotion::~CMotion()
{
	
}


//
void CMotion::GoSoftHome(BOOL bAuto)
{
	for( int i = 0; i < 4; i++ )
	{
		if ( TRUE == m_para[i].bHardOrgEnable)
		{
			if ( !bAuto || ( bAuto && m_para[i].bAutoHome) )//清误差回软件原点
				Home(i,TRUE);
		}
		else //只回软件原点
		{
			g_Motion.AMove(i,0,2);
		}
	}
}


//bSoftHome TRUE: 先回机械原点再回软件原点； FALSE: 只回机械原点
BOOL CMotion::Home(int nAxis,BOOL bSoftHome)
{
	if ( !m_para[nAxis].bEnabled )
		return FALSE;
	
	BOOL bResult = FALSE;
	MOTOR_PARA  para;
	
	para.nVer  = 1;       //版本
	para.nAxis = nAxis;   //轴号
	para.nMoveMode = 2;
	
	g_Motion.GetBufferMovePara(2, 0, para);
	para.nMoveMode = m_para[nAxis].nAxisType;

	if ( HLOK == g_DAT.HomeMove(&para, bSoftHome) )
		bResult = TRUE;
	
	return bResult;
}



void CMotion::ResetPara(int nAxis )//重设参数
{
	if (  nAxis < 0 || nAxis > 3 )
		return;
	
	//
	TCHAR szSection[50] = { 0 };
	_stprintf_s(szSection,50,_T("Motion%d"),nAxis);
	//////////////
	m_para[nAxis].nPPR						 =  ::g_sys.GetProfileInt( szSection, _T("PPR"),   2000);
	m_para[nAxis].dbPitch					 =  ::g_sys.GetProfileDouble( szSection, _T("Pitch"),   5);
	m_para[nAxis].nDelay					 =  ::g_sys.GetProfileInt( szSection, _T("Delay"),   100);
	m_para[nAxis].nAxisType				 =  ::g_sys.GetProfileInt( szSection, _T("AxisType"),   1);
	m_para[nAxis].nMotorDire				 =  ::g_sys.GetProfileInt( szSection, _T("MotorDire"),   0);
	
	m_para[nAxis].dbLineEmptySpeed	 =  ::g_sys.GetProfileDouble(szSection, _T("LineEmptySpeed"),   1500);
	m_para[nAxis].dbLineWorkSpeed		 =  ::g_sys.GetProfileDouble( szSection, _T("LineWorkSpeed"),   1000);
	m_para[nAxis].dbHomeHighSpeed	 =  ::g_sys.GetProfileDouble( szSection, _T("HomeHighSpeed"),   2000);
	m_para[nAxis].dbHomeLowSpeed		 =  ::g_sys.GetProfileDouble( szSection, _T("HomeLowSpeed"),   1000);
	
	m_para[nAxis].dbREmptySpeed		 =  ::g_sys.GetProfileDouble( szSection, _T("REmptySpeed"),   720);
	m_para[nAxis].dbRWorkSpeed			 =  ::g_sys.GetProfileDouble( szSection, _T("RWorkSpeed"),   360);
	
	m_para[nAxis].dbEmptyTAcc			 =  ::g_sys.GetProfileDouble( szSection, _T("EmptyTAcc"),   0.1);
	m_para[nAxis].dbWorkTAcc			 =  ::g_sys.GetProfileDouble( szSection, _T("WorkTAcc"),   0.1);
	m_para[nAxis].dbHomeTAcc			 =  ::g_sys.GetProfileDouble( szSection, _T("HomeTAcc"),   0.1);
	//
	m_para[nAxis].bAutoHome				 =  ::g_sys.GetProfileInt( szSection, _T("AutoSoft0"),   0);
	m_para[nAxis].bEnabled					 =  ::g_sys.GetProfileInt( szSection, _T("Enabled") ,	0);
	m_para[nAxis].bHardOrgEnable		 =  ::g_sys.GetProfileInt( szSection, _T("HardOrgEnable") ,0);
	
	ResetBasePara(nAxis);
}



BOOL CMotion::GetMotionPara(int nAxis,MOTIONPARA &para)
{
	if ( nAxis < 0 || nAxis > 3)
		return FALSE;
	
	para = m_para[nAxis];
	
	return TRUE;
}



BOOL CMotion::SetMotionPara(int nAxis,const MOTIONPARA &para)
{
	if ( nAxis < 0 || nAxis > 3)
		return FALSE;
	
	m_para[nAxis] = para;
	ResetBasePara(nAxis);
	
	return TRUE;
}


//传入运动类型、运动量，为缓冲区参数赋值
BOOL CMotion::GetBufferMovePara(int nWorkType,double dbValue,MOTOR_PARA &para)
{	
	int nAxis = para.nAxis;
	if ( nAxis < 0 || nAxis > 3 )
		return FALSE;
	
	//
	double dbPulse = 0;
	ConvertToPulse(nAxis,dbValue,dbPulse);
	para.nMovePos   = (int)(dbPulse);
	para.nDelay     = m_para[nAxis].nDelay;
	
	//
	switch( nWorkType )
	{
	case 0://Work Move
		para.nMoveSpeed = m_basePara[nAxis].nWorkSpeed;
		para.nMoveAcc   = m_basePara[nAxis].nWorkAcc;
		break;
	
	case 1://Empty Move
		para.nMoveSpeed = m_basePara[nAxis].nEmptySpeed;
		para.nMoveAcc   = m_basePara[nAxis].nEmptyAcc;
		break;
	
	case 2://Home Move
		para.nMoveSpeed = m_basePara[nAxis].nHomeHighSpeed;
		para.nMoveAcc   = m_basePara[nAxis].nHomeAcc;
		break;
	}
	
	para.nVer = 1;
	return TRUE;
}

void CMotion::ConvertToPulse(int nAxis,double dbOrg, double& dbPulse)
{
	if ( nAxis < 0 || nAxis > 3)
		return;
	
	/////////////
	switch( m_para[nAxis].nAxisType )
	{
	case lineAxis:	//直线运动
		dbPulse = m_para[nAxis].nPPR/m_para[nAxis].dbPitch*m_para[nAxis].nMotorDire*dbOrg;
		break;
	
	case rotateAxis://圆周运动
		dbPulse = m_para[nAxis].nPPR*m_para[nAxis].nMotorDire/360.0*dbOrg;
		break;
	}
	//dbPulse  +=  0.5*m_para[nAxis].nMotorDire;
}

void CMotion::ResetBasePara(int nAxis)
{
	if ( nAxis < 0 || nAxis > 3 )
		return;
	
	//
	double dbRatio = 0;
	for( int i = 0; i <= 3; i++ )
	{
		if ( !( nAxis<0 || i == nAxis) )
			continue;
		
		switch( m_para[nAxis].nAxisType )
		{
		case lineAxis: //直线运动
			{
				dbRatio = m_para[i].nPPR/m_para[i].dbPitch;
				m_basePara[i].nWorkSpeed   = (int)(dbRatio*m_para[i].dbLineWorkSpeed);
				m_basePara[i].nEmptySpeed  = (int)(dbRatio*m_para[i].dbLineEmptySpeed);
				m_basePara[i].nHomeHighSpeed = (int)(dbRatio*m_para[i].dbHomeHighSpeed);
				m_basePara[i].nHomeLowSpeed  = (int)(dbRatio*m_para[i].dbHomeLowSpeed);
				m_basePara[i].nWorkAcc = (int)(m_basePara[i].nWorkSpeed / m_para[i].dbWorkTAcc);
				m_basePara[i].nEmptyAcc =  (int)(m_basePara[i].nEmptySpeed/ m_para[i].dbEmptyTAcc);
				m_basePara[i].nHomeAcc = (int)(m_basePara[i].nHomeHighSpeed/ m_para[i].dbHomeTAcc);
			}
			break;
			
		case rotateAxis://圆周运动
			{
				dbRatio = m_para[nAxis].nPPR/360.0;
				m_basePara[i].nWorkSpeed  = (int)(dbRatio*m_para[i].dbRWorkSpeed);
				m_basePara[i].nEmptySpeed = (int)(dbRatio*m_para[i].dbREmptySpeed);
				m_basePara[i].nHomeHighSpeed = (int)(dbRatio*m_para[i].dbRWorkSpeed*0.6);
				m_basePara[i].nHomeLowSpeed  = (int)(m_basePara[i].nHomeHighSpeed*0.2);
				m_basePara[i].nWorkAcc = (int)(m_basePara[i].nWorkSpeed / m_para[i].dbWorkTAcc);
				m_basePara[i].nEmptyAcc =  (int)(m_basePara[i].nEmptySpeed/ m_para[i].dbEmptyTAcc);
				m_basePara[i].nHomeAcc = m_basePara[i].nWorkAcc;
			}
			break;
		}
	}
}

//绝对运动，立即指令
BOOL CMotion::AMove(int nAxis,double dbWorkPos,int nType)
{
	if ( nAxis < 0 || nAxis > 3 )
		return FALSE;	
	
	if ( !m_para[nAxis].bEnabled )
		return FALSE;

	MOTOR_PARA para;
	para.nVer  = 1;		//版本
	para.nAxis = nAxis; //轴号
	para.nMoveMode = 0;
	GetBufferMovePara(nType, dbWorkPos, para);
	if ( HLOK == g_DAT.MotorMove(&para) )
		return TRUE;
	else
		return FALSE;
}

//相对运动，立即指令
BOOL CMotion::RMove(int nAxis,double dbRelPos,int nType)
{
	if ( nAxis < 0 || nAxis > 3)
		return FALSE;
	
	if ( !m_para[nAxis].bEnabled )
		return FALSE;
	
	MOTOR_PARA para;
	para.nVer  = 1;		  //版本
	para.nAxis = nAxis;   //轴号
	para.nMoveMode = 1;
	GetBufferMovePara(nType, dbRelPos, para);
	if ( HLOK == g_DAT.MotorMove(&para) )
		return TRUE;
	else
		return FALSE;
}


void CMotion::StopRun()
{
	
}


int CMotion::GetAxisType(int nAxisNo)
{
	return m_para[nAxisNo].nAxisType;
}


int CMotion::GetAxisPulse(int nAxis,double dbOrg)
{
	double dbPos = 0;
	ConvertToPulse(nAxis, dbOrg, dbPos);
	
	return (int)dbPos;
}


//
double CMotion::PosTranslate( int nAxis, LONGLONG nPos)
{
	if ( nAxis < 0 || nAxis > 3 )
		return 0;
	
	//
	double dbPos, dbPulse;
	dbPulse = (double)nPos;
	switch( m_para[nAxis].nAxisType )
	{
	case lineAxis:	// 直线运动
		dbPos = dbPulse/(m_para[nAxis].nPPR/m_para[nAxis].dbPitch*m_para[nAxis].nMotorDire);
		break;
	
	case rotateAxis:// 圆周运动
		dbPos = dbPulse/(m_para[nAxis].nPPR*m_para[nAxis].nMotorDire/360.0);
		break;
	}
	
	return dbPos;
}
