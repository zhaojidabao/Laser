// GrayMgr.cpp: implementation of the CGrayMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "GrayMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CGrayMgr::CNodeMgr::CNodeMgr()
{
}

CGrayMgr::CNodeMgr::~CNodeMgr()
{
	m_arrpData.RemoveAll();
	m_arrDataLen.RemoveAll();
}

void CGrayMgr::CNodeMgr::SetBmpCfg(BMPSPEC* pCfg)
{
	if (NULL == pCfg) {
		return;
	}
	m_bmpSpec = *pCfg;
}

void CGrayMgr::CNodeMgr::AddDataLen(int nDot)
{
	m_arrDataLen.Add(nDot);
}

void CGrayMgr::CNodeMgr::AddDataBlock(DOT6* pData)
{
	m_arrpData.Add(pData);
}

UINT CGrayMgr::CNodeMgr::Mark (HWND hWnd)
{
	if ( 0 == m_arrpData.GetSize() || 
		 0 == m_arrDataLen.GetSize() ||
		 m_arrpData.GetSize()  !=  m_arrDataLen.GetSize())
		return HLERROR;

	if( g_DAT.BmpStart(&m_bmpSpec)  !=  HLOK ) 
	{
		return HLERROR;
	}
	
	for (int iData = 0; iData < m_arrpData.GetSize(); ++iData) 
	{
		if ( g_DAT.Mark_BMP(2, LPVOID(m_arrpData.ElementAt(iData)),m_arrDataLen.ElementAt(iData),0)  !=  HLOK)
		{
			OutputDebugString(_T("Mark Bmp return failed!"));
			m_arrpData.RemoveAll();
			m_arrDataLen.RemoveAll();
			return HLERROR;
		}

	}

	m_arrpData.RemoveAll();
	m_arrDataLen.RemoveAll();
	
	return HLOK;
}


CGrayMgr* gGrayMgr = NULL;
CGrayMgr::CNodeMgr* CGrayMgr::m_pNode;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrayMgr::CGrayMgr()
{
}

CGrayMgr::~CGrayMgr()
{
}

void CGrayMgr::Run()
{
	HMODULE hMod = NULL;

#ifdef DEBUG
	hMod = LoadLibrary(g_sys.GetInstallPath()+"GraySet_D.dll");
#else
	hMod = LoadLibrary(g_sys.GetInstallPath()+"GraySet.dll");
#endif

	if (NULL == hMod) {
		return;
	}

	GRAYSET grayset = (GRAYSET)GetProcAddress(hMod, "GraySet");
	if (grayset) 
	{
		gGrayMgr = this;

		grayset(CallFunction);
	}

	FreeLibrary(hMod);
}

BOOL CGrayMgr::CallFunction(UINT eFunc, LPVOID pPara)
{
	BOOL bRet = TRUE;
	CHLApp* pApp = (CHLApp*)AfxGetApp();
	HINSTANCE hInstOld = ::AfxGetResourceHandle();
	AfxSetResourceHandle(pApp->m_hLangDLL);
	
	switch(eFunc) 
	{
	case READY:
		gGrayMgr->SetMarkType(0);
		gGrayMgr->SetLoopMarkInfo(0,0);
		gGrayMgr->RunThread(pPara);
		
		{
			m_pNode = new CNodeMgr();
			if (NULL == m_pNode) {
				bRet = FALSE;
				break;
			}
			CTree* pTree = new CTree(m_pNode);
			
			CGroupObj *pGroupObj = new CGroupObj();
			pGroupObj->Add(pTree);
			
			CChain* pChain = new CChain();
			pChain->Add(pGroupObj);
			
			gGrayMgr->SetChain(pChain);
		}
		
		break;
	case SET_CFG:
		m_pNode->SetBmpCfg((BMPSPEC*)pPara);
		break;
	case SET_DATALEN:
		m_pNode->AddDataLen((int)pPara);
		break;
	case SET_DATA:
		m_pNode->AddDataBlock((DOT6*)pPara);
		break;
	case MARK_START:
		gGrayMgr->StartMark();
		break;
	case MARK_STOP:	
		gGrayMgr->StopMark();
		break;
	case QUIT:
		if (m_pChain) {
			delete m_pChain;
			m_pChain = NULL;
		}
		gGrayMgr->StopThread();
		break;
	default:
		break;
	}
	
	AfxSetResourceHandle(hInstOld);
	
	return bRet;
}

