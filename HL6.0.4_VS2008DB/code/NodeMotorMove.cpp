// NodeMotorMove.cpp: implementation of the CNodeMotorMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeMotorMove.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeMotorMove::CNodeMotorMove()
{
	m_type = NODE_MOTORMOVE;
	m_dbMoveAngle = 0;
	m_nWorkType = 0; //运动类型
	
	memset(&m_para,0,sizeof(MOTOR_PARA));
	m_para.nVer = 1;
	m_para.nMoveMode = 0;
	m_para.nAxis = 0;
}


CNodeMotorMove::~CNodeMotorMove()
{

}


void CNodeMotorMove::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if ( bFlag )
	{
		fs << _T("NMOTORMOVE_B");
		
		fs << 100;
		fs << m_dbMoveAngle;
		
		fs << 101;
		fs << m_nWorkType;
		
		fs << 102;
		fs << m_para.nVer;

		fs << 103;
		fs << m_para.nMoveMode;

		fs << 104;
		fs << m_para.nAxis;
		
		fs << _T("NMOTORMOVE_E");
	}
	else
	{
		while( fs >> str )
		{
			if ( str == _T("NMOTORMOVE_E") )
				break;
			
			switch( _ttoi(str))
			{
				case 100:
					fs >> m_dbMoveAngle;
					break;

				case 101:
					fs >> m_nWorkType;
					break;

				case 102:
					fs >> m_para.nVer;
					break;

				case 103:
					fs >> m_para.nMoveMode;
					break;

				case 104:
					fs >> m_para.nAxis;
					break;
			}
		}
	}
}



UINT CNodeMotorMove::Mark(HWND hWnd)
{
	if ( FALSE == g_DAT.m_btExtendInfo.FunData.bMotorMark )
		return 0;

	double dbMoveAngle = m_dbMoveAngle;	
	g_Motion.GetBufferMovePara(m_nWorkType, dbMoveAngle, m_para);
	
// 	CString strDebug;
// 	strDebug.Format("\nCNodeMotorMove::Mark Angle:%.3f, nMovePos:%d, nMoveSpeed:%d,nMoveAcc:%d,nDelay:%d\n", 
// 					   dbMoveAngle, m_para.nMovePos, m_para.nMoveSpeed, m_para.nMoveAcc, m_para.nDelay);
// 	::OutputDebugString(strDebug);
	
	g_DAT.MarkBufferMove(&m_para);
	
	return 0;
}

BOOL CNodeMotorMove::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_MOTORMOVE )
		return FALSE;

	CNodeMotorMove* pNode = (CNodeMotorMove*)pSrc;
	////////
	m_dbMoveAngle	 =  pNode->m_dbMoveAngle;
	m_nWorkType		 =  pNode->m_nWorkType;
	m_para.nVer		 =  pNode->m_para.nVer;
	m_para.nMoveMode = pNode->m_para.nMoveMode;
	m_para.nAxis		 =  pNode->m_para.nAxis;
	m_property			 =  pNode->m_property;
	m_bSelect			 =  pNode->m_bSelect;
	
	return TRUE;
}

