// stdafx.cpp : source file that includes just the standard includes

#include "stdafx.h"
#include "Quad.h"
 
SYSCFG g_SysCfg;
BOOL		g_bIsSort			 =  FALSE;
BOOL		g_bIsRedPreview	 =  FALSE;
BOOL		g_bIsSelMark		 =  FALSE;
BOOL		g_bIsFastMark		 =  FALSE;
BOOL		g_bIsDownLoad   = FALSE;
int			g_nCardType		 =  EMCC;

DWORD64	g_dwLastObjId		 =  0;
LANGID		g_LangID			 =  0x804;
DWORD		g_dwVersion		 =  CUR_DOC_VERSION;

USERINFO	g_CurUserInfo;		// 当前用户信息
CArray<USERINFO, USERINFO> g_RightInfoList; // 权限列表

CString      g_StrCopyTmpFile;

LPLOADDIB	LoadDib = NULL;
LPPROCDIB	ProcDib = NULL;
LPPROCGRAYDIB ProcGrayDib = NULL;
LPDELDIB	DelDib = NULL;

void GetExepath(TCHAR *pPath, int nSize)  //获取执行目录
{
	TCHAR exePath[MAX_PATH];
	::memset(exePath, 0, sizeof(TCHAR)*MAX_PATH);
	::GetModuleFileName(NULL,exePath,MAX_PATH);
	TCHAR* pEnd = _tcsrchr(exePath,_T('\\'));
	*(pEnd+1) = NULL;
	if ( pPath  !=  NULL )
	{
		_tcscpy_s(pPath,nSize,exePath);
	}
}


double MyPrecision(double val,int digit)
{
	double k = pow(10.0,digit);
	if ( val > 0 )
		val = (_int64)(val*k+0.5)/k;
	else
		val = (_int64)(val*k-0.5)/k;

	return val;
}

double MyDegToRad(double val)
{
	return val*PAI/180;
}

double MyRadToDeg(double val)
{
	return val*180/PAI;
}

int ReadIniInt(TCHAR *pszFileName, LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue)
{
	if ( pszFileName == NULL )
		return 0;

	TCHAR szbuff[32] = { 0 };
	
	DWORD dwRet = ::GetPrivateProfileString(lpszSection,lpszKey,lpszDefaultValue,szbuff, 32, pszFileName);
	if ( dwRet  <=  0  )
		return  0;
	
	return  _ttoi(szbuff);
}

void WriteIniInt(TCHAR *pszFileName, LPCTSTR lpszSection, LPCTSTR lpszKey, int nValue)
{
	if ( pszFileName == NULL )
		return;

	TCHAR szbuff[32] = {0};
	_stprintf_s(szbuff,  32, _T("%d"), nValue);
	
	::WritePrivateProfileString(lpszSection, lpszKey, szbuff, pszFileName);
}

CString VerifyTimeFormat(const CString& strOrg)
{
	CString strDate;
	int    nLen  = strOrg.GetLength();
	BOOL bFlag = FALSE;
	for( int i = 0; i<nLen; i++ )
	{
		TCHAR chA = strOrg.GetAt(i);

		if ( chA == _T('%') && FALSE == bFlag )
		{
			bFlag = TRUE;
		}
		else
		{
			if ( TRUE == bFlag )
			{
				if (    chA ==  _T('a') || chA ==  _T('A') || chA ==  _T('b') || chA ==  _T('B')  || chA ==  _T('c')
					|| chA ==  _T('d') || chA ==  _T('H') || chA ==  _T('I')  || chA ==  _T('j')  || chA ==  _T('m')
					|| chA ==  _T('M') || chA ==  _T('p') || chA ==  _T('S') || chA ==  _T('U') || chA ==  _T('w')
					|| chA ==  _T('W') || chA ==  _T('x') || chA ==  _T('y') || chA ==  _T('Y') || chA ==  _T('z')
					|| chA ==  _T('Z') || chA ==  _T('%') )
				{
					strDate  +=  _T('%');
					strDate  +=  chA;
					bFlag = FALSE;
				}
			}
			else
			{
				strDate  +=  chA;
			}
		}		
	}
	
	return strDate;
}



CString GenTimeString( const CString& strDate )
{
	CString strTmp = strDate;
	CTime tt = CTime::GetCurrentTime();
	CString strU = tt.Format(_T("%U"));
	strU.Format(_T("%.2d"),_ttoi((LPCTSTR)strU)+1);
	
	CString strW = tt.Format(_T("%W"));
	strW.Format(_T("%.2d"),_ttoi((LPCTSTR)strW)+1);

	////add by zhq
	CString strw = tt.Format(_T("%w"));
	strw.Format(_T("%d"),_ttoi((LPCTSTR)strw)+1);
	
	strTmp.Replace(_T("##"),_T("_@$^*!~@_"));
	// add by zhq
	strTmp.Replace(_T("#w"),strw); 
	strTmp.Replace(_T("#W"),strW);
	strTmp.Replace(_T("#U"),strU); 
	strTmp.Replace(_T("_@$^*!~@_"),_T("#"));
	
	CString strRet = tt.Format(strTmp);                                                                           
	return strRet;
}