// DogHandler.cpp: implementation of the CDogHandler class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include ".\Dog\DogHandler.h"
#include ".\Dog\DlgCheckSSNKDog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDogHandler::CDogHandler()
{
	m_bChecked = FALSE;
	m_dwRltOptDog = 0x00;
	m_pILmtCtrl = NULL;
	m_mgrDll.SetLibName(_T("HansLmtH.DLL"));
	////
	BYTE* pcbBuff = new BYTE[sizeof(DLL_VER_INFO)+512];
	DLL_VER_INFO* pDllVerInfo = (DLL_VER_INFO*) pcbBuff;
	pDllVerInfo->_dwByteSize = 0;
	if( vFAILED(m_mgrDll.DllGetVerInfo(*pDllVerInfo)) ){
		::OutputDebugString(_T("false: softdog dll load err\n"));
	}
	else{
		VERIFY( vSUCCEEDED(m_mgrDll.DllGetVerInfo(*pDllVerInfo)) );
		if( 0!=strcmp("{B915D34F-EBBF-4100-A567-51958F732140}", pDllVerInfo->_szLibType) ){
			::OutputDebugString(_T("false: softdog dll type err\n"));
		}
		else{
			VERIFY( vSUCCEEDED(m_mgrDll.DllCreateInterface(1, (void**) &m_pILmtCtrl)) );
		}
	}
	delete[] pcbBuff;
	////
	DoCheck();
}

CDogHandler::~CDogHandler()
{
	m_bChecked = FALSE;
	m_dwRltOptDog = 0x00;
	////
	if( NULL!=m_pILmtCtrl ){
		m_pILmtCtrl->Release();
		m_pILmtCtrl = NULL;
	}
	m_mgrDll.ReleaseLib();
}

void CDogHandler::DoCheck()
{
	if( NULL!=m_pILmtCtrl ){
		if( vSUCCEEDED(m_dwRltOptDog=m_pILmtCtrl->Check(m_dogCheckParas)) ){
			m_bChecked = TRUE;
		}
		else{
			m_bChecked = FALSE;
		}
	}
}

BOOL CDogHandler::IsEnabled()
{
	if( IsChecked() ){
		if( m_dogCheckParas._bEnabled ){
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDogHandler::IsAct()
{
	if( IsChecked() ){
		if( m_dogCheckParas._bEnabled ){
			return TRUE;
		}
		if( 1!=m_dogCheckParas._cbOptRlt ){
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDogHandler::DoExecute(DOG_EXECUTE_PARAS &exeParas)
{
	ASSERT( IsChecked() );
	if( vFAILED(m_pILmtCtrl->Execute(exeParas)) ){
		return FALSE;
	}
	if( !exeParas._bSuccess ){
		return FALSE;
	}
	switch( exeParas._cbExeType ){
	case 1:
		m_dogCheckParas._bEnabled = TRUE;
		m_dogCheckParas._cbOptRlt = 101;
		break;
	case 2:
		m_dogCheckParas._bEnabled = TRUE;
		m_dogCheckParas._cbOptRlt = 102;
		break;
	case 3:
		if( !m_dogCheckParas._bAct )
			m_dogCheckParas._bAct = TRUE;
		else
			m_dogCheckParas._cbActCounts += 1;
		break;
	case 4:
		m_dogCheckParas._cbActCounts = 0;
		break;
	}
	return TRUE;
}
