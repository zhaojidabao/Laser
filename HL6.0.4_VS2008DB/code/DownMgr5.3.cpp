// DownMgr.cpp: implementation of the CDownMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "DownMgr5.3.h"
#include "DocPipe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDownMgr* CDownMgr::m_downMgr = NULL;
MAP_DOC CDownMgr::m_mapDoc;
HD_DATA_HEAD CDownMgr::m_data;
HWND CDownMgr::m_hWnd = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownMgr::CDownMgr()
	: CMarkMgr()
{
	m_bMandatory = FALSE;
	if ( m_data.pBlock )
	{
		delete m_data.pBlock;
		m_data.pBlock = NULL;
	}
}

CDownMgr::~CDownMgr()
{
	m_mapDoc.clear();
}

void CDownMgr::Run()
{
	CString strDll;

#ifdef _DEBUG 
	strDll = g_sys.GetInstallPath() + _T("HLDown_D.dll");
#else
	strDll = g_sys.GetInstallPath() + _T("HLDown.dll");
#endif

	BOOL bVerOK = FALSE;
	unsigned int dwSize = GetFileVersionInfoSize((LPTSTR)(LPCTSTR)strDll, 0);
	if ( dwSize )
	{
		char *szData = new char[dwSize];
		memset(szData, 0, dwSize);
		if ( GetFileVersionInfo((LPTSTR)(LPCTSTR)strDll, NULL, dwSize, szData) ) 
		{
			VS_FIXEDFILEINFO *VInfo;
			if ( VerQueryValue(szData, _T("\\"), (LPVOID *)(&VInfo), &dwSize)) 
			{
				if (VInfo->dwFileVersionMS  >=  0x00050003)
				{
					bVerOK = TRUE;
				}
			}
		}
		delete []szData;
	}
	
	HMODULE hMod = LoadLibrary(strDll);
	if ( FALSE == bVerOK || NULL == hMod ) 
	{
		AfxMessageBox(IDS_DAMAGEFILE);
		FreeLibrary(hMod);
		return;
	}
	
	m_downMgr = this;
	
	DOWNMODALDLG dlgDown = (DOWNMODALDLG)GetProcAddress(hMod, "DownModalDlg");
	if ( dlgDown )
		dlgDown((CALLFUNCTION)CallFunction);
	
	m_downMgr = NULL;
}

/*
	说明：此回调函数由于由其他DLL调入，使用主平台的控件资源需指明资源实例
*/
BOOL CDownMgr::CallFunction(UINT eFunc, LPVOID* pPara)
{
	if ( NULL == m_downMgr )  
		return FALSE;

	CHLApp* pApp = (CHLApp*)AfxGetApp();
	HINSTANCE hInstOld = ::AfxGetResourceHandle();
	AfxSetResourceHandle(pApp->m_hLangDLL);

	BOOL bRet = FALSE;
	switch(eFunc) 
	{
		case READY:
			{
				//取得各打开文档的指针
				m_mapDoc.clear();
				CMultiDocTemplate *pDocTem = NULL;
				CHLDoc *pDoc = NULL;
				POSITION posTem = pApp->GetFirstDocTemplatePosition();
				while(posTem  !=  NULL)
				{
					pDocTem = (CMultiDocTemplate *)pApp->GetNextDocTemplate(posTem);
					POSITION posDoc = pDocTem->GetFirstDocPosition();
					while(posDoc  !=  NULL)
					{
						pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
						pDoc->PreprocessMark();

						m_mapDoc[pDoc->GetTitle()] = pDoc;
					}
				}
				
				m_data.nBlock = m_mapDoc.size();
				m_data.pBlock = new HD_DATA_BLOCK[m_data.nBlock];
				MAP_DOC::iterator it = m_mapDoc.begin();
				for (int i = 0; it  !=  m_mapDoc.end(); ++it, ++i ) 
				{
					CString strFile = it->first;
					CStdioFileEx::GetMultiByteFromUnicode(strFile.GetBuffer(strFile.GetLength()),(*(m_data.pBlock+i)).m5_3_4.szFile,260);
					strFile.ReleaseBuffer();
					/////
					pDoc = (CHLDoc *)(it->second);
					(*(m_data.pBlock+i)).m5_3_4.uIOValid = pDoc->m_motor.uIOValid;
					(*(m_data.pBlock+i)).m5_3_4.uIOMdt = pDoc->m_motor.StrIOToWORD();
					(*(m_data.pBlock+i)).m5_3_4.nRet = 0;
				}

				m_hWnd = HWND(*pPara);
				m_downMgr->SetMarkType(0);
				m_downMgr->SetLoopMarkInfo(0,0);
				m_downMgr->RunThread(*pPara);

				*pPara = (LPVOID )&m_data;
			}

			bRet = TRUE;
			break;

		case MARK_START:
			{
				if ( m_downMgr->MarkIsOver() )  
					return FALSE;

				if ( NULL == pPara ) 
					return FALSE;

				HD_DATA_HEAD* pstHead = (HD_DATA_HEAD*)pPara;
				
				CHLDoc* pDoc = m_mapDoc[pstHead->pBlock->m5_3_4.szFile];
				if ( NULL == pDoc ) 
					break;
	
				if ( pDoc->m_chain.GetCount() == 0 )
					break;
				
				if ( pDoc->OutOfLimit() )
					break;

				g_DAT.SetLay(pDoc->GetLay());
				m_downMgr->SetChain(&pDoc->m_chain);
				m_downMgr->m_pChain3d = pDoc->m_b3D?&(pDoc->m_chain3d):NULL;
				m_downMgr->StartMark();
			}
			bRet = TRUE;
			break;

		case MARK_STOP:
			m_downMgr->StopMark();
			break;

		case DOWN_MUL:
			{
				if ( NULL == pPara ) 
					return FALSE;

				HD_DATA_HEAD* pstHead = (HD_DATA_HEAD*)pPara;				
				if ( pstHead->nBlock  <=  0 || NULL == pstHead->pBlock )
					break;

				if ( FALSE == g_DAT.ClearMulDocData() ) 
					break;

				BOOL bStopThd = FALSE;
				HWND hWnd = m_hWnd;
				if ( WAIT_OBJECT_0 == WaitForSingleObject(hThreadRun, 0) )
				{
					m_downMgr->StopThread();
					bStopThd = TRUE;
				}				

				SetEvent(hMarkSts);
				if ( WAIT_OBJECT_0 == WaitForSingleObject(hStopWork, 0) ) 
				{
					if ( TRUE == bStopThd ) 
					{
						m_downMgr->RunThread(hWnd);
					}
					ResetEvent(hMarkSts);
					break;
				}
				
				UINT uSpaceRate = 0;
				CString strFile = _T("");
				CHLDoc* pDoc = NULL;
				MULDOC_INFO stMdtInfo;
				MARKEND_INFO stInfo;
				stMdtInfo.nVer = 1;
				for (int iDoc = 0; iDoc < pstHead->nBlock; ++iDoc) 
				{
					strFile = (*(pstHead->pBlock+iDoc)).m5_3_4.szFile;
					pDoc = m_downMgr->m_mapDoc[strFile];

					if (FALSE == pDoc->HasObjForProcess())
					{
						(*(pstHead->pBlock+iDoc)).m5_3_4.nRet = 4;
						continue;
					}
					
					if (pDoc->OutOfLimit())
					{
						(*(pstHead->pBlock+iDoc)).m5_3_4.nRet = 5;
						continue;
					}

					if ((*(pstHead->pBlock+iDoc)).m5_3_4.uIOValid == 0) {
						(*(pstHead->pBlock+iDoc)).m5_3_4.nRet = 6;
						continue;
					}

					g_DAT.SetLay(pDoc->GetLay());
					m_downMgr->SetChain(&(pDoc->m_chain));

					stMdtInfo.nDocId = iDoc;
					stMdtInfo.nMask = (*(pstHead->pBlock+iDoc)).m5_3_4.uIOValid;
					stMdtInfo.nSign = (*(pstHead->pBlock+iDoc)).m5_3_4.uIOMdt;
					if (HLOK  !=  g_DAT.MarkStart(ENU_DOWNMULDOC, (UINT)&stMdtInfo) )
					{
						(*(pstHead->pBlock+iDoc)).m5_3_4.nRet = 1;
						continue;
					}

					m_dwCurPos = 0;
					if ( pDoc->m_b3D )
						pDoc->m_chain3d.Mark(hWnd, g_DAT.GetMarkFuncPtr(), g_DAT.m_bEnableZOffset ? g_SysCfg.m_dbZOffset : 0 );
					else
						pDoc->m_chain.Mark(hWnd);
					
					
					memset(&stInfo, 0, sizeof(MARKEND_INFO));
					stInfo.nVer = 1;
					g_DAT.MarkEnd(TRUE, (UINT)&stInfo);
					if ( stInfo.nStatus ) 
					{
						(*(pstHead->pBlock+iDoc)).m5_3_4.nRet = 0;
						uSpaceRate = stInfo.nFlashUsed;
					}
					///////
					SendMessage(hWnd,WM_MARK_END,MARK_SEND,0);
				}	
				
				SendMessage(hWnd,WM_MARK_END,MARK_TEND, uSpaceRate);

				if ( TRUE == bStopThd ) {
					m_downMgr->RunThread(hWnd);
				}
				ResetEvent(hMarkSts);
			}
			bRet = TRUE;
			break;

		case QUIT:
			m_mapDoc.clear();
			m_downMgr->m_bMandatory = (BOOL)(*((BOOL**)pPara));
			m_downMgr->StopThread();
			bRet = TRUE;
			delete []m_data.pBlock;
			m_data.pBlock = NULL;
			break;

		case SAVE_DOC:
			{
				if ( NULL == pPara ) 
					return FALSE;

				HD_DATA_HEAD* pstHead = (HD_DATA_HEAD*)pPara;
				CHLDoc* pDoc = m_mapDoc[pstHead->pBlock->m5_3_4.szFile];
				if ( NULL == pDoc ) 
					break;

				pDoc->m_motor.WordToStrIO(pstHead->pBlock->m5_3_4.uIOMdt, g_sys.m_nIOSelCount);
				pDoc->m_motor.uIOValid = pstHead->pBlock->m5_3_4.uIOValid;
			}
			bRet = TRUE;
			break;
		
		case GET_MDTIO:
			{
				UINT uIO(0);
				g_DAT.GetInput(&uIO);
				
				*((WORD*)pPara) = uIO >> 4;
			}
			bRet = TRUE;
			break;
			
		case SAVE_MULDOC:
			{
				static  TCHAR szFilter[] = _T("Muldoc File(*.mul)|*.mul||");
				CFileDialog dlg(FALSE,_T("Mul"),NULL,OFN_HIDEREADONLY,szFilter);
				CString str;

				str.LoadString(IDS_MULDOC_SAVE);
				dlg.m_ofn.lpstrTitle = str;
				if ( dlg.DoModal() == IDOK )
				{
					str = dlg.GetPathName();

					CHLDoc* pDoc = NULL;
					MULDOCFILE stMulDoc;
					MAP_DOC::iterator it = m_mapDoc.begin();
					for (int i = 0; it  !=  m_mapDoc.end(); ++it, ++i )
					{
						pDoc = (CHLDoc *)(it->second);
						CString strPath = pDoc->GetPathName();

#ifdef _UNICODE
						char szPath[MAX_PATH] = {0};
						TCHAR* pChar = strPath.GetBuffer(MAX_PATH);
						WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, pChar, -1, szPath, MAX_PATH, NULL, NULL);
						strPath.ReleaseBuffer();
						stMulDoc.arName.Add(szPath);
#else
						stMulDoc.arName.Add(strPath);
#endif
						stMulDoc.arOption.Add(pDoc->m_motor);
					}
					
					stMulDoc.Serialize(str, FALSE);
				}
			}
			bRet = TRUE;
			break;

		default:
			break;
	}

	AfxSetResourceHandle(hInstOld);

	return bRet;
}
