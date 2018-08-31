// GlobalIni.cpp: implementation of the CGlobalIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SINGLETON_IMPLEMENT(CGlobalIni)

CString CGlobalIni::GetAppPath()
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	return strPath;
}

CGlobalIni::CGlobalIni()
{
	//设置默认INI路径，以工作目录作为其默认路径
	SetIniFullPath(GetAppPath() + _T("\\WorkCfg.ini"));
}

CGlobalIni::~CGlobalIni()
{

}

void CGlobalIni::GetDBPara(DBPARA& DBpara)
{
	int nType = 0;
	ReadValue(_T("DATABASE"), _T("TYPE"), nType, driverACCESSXP);
	DBpara.nDriverType = (BYTE)nType;

	CString strResult;
	ReadValue(_T("DATABASE"), _T("NAME"), strResult, _T(""));
	DBpara._Database = (_bstr_t)(LPCTSTR)strResult;
	ReadValue(_T("DATABASE"), _T("PWD"), strResult, _T(""));
	DBpara._Pwd = (_bstr_t)(LPCTSTR)strResult;
	ReadValue(_T("DATABASE"), _T("USER"), strResult, _T(""));
	DBpara._User = (_bstr_t)(LPCTSTR)strResult;
	ReadValue(_T("DATABASE"), _T("SERVER"), strResult, _T(""));
	DBpara._Server = (_bstr_t)(LPCTSTR)strResult;
}

void CGlobalIni::SetDBPara(DBPARA& DBpara)
{
	SaveValue(_T("DATABASE"), _T("TYPE"), (INT)DBpara.nDriverType);
	CString strTmp = (LPCTSTR)DBpara._Database;
	SaveValue(_T("DATABASE"), _T("NAME"), strTmp);
	strTmp = (LPCTSTR)DBpara._Pwd;
	SaveValue(_T("DATABASE"), _T("PWD"), strTmp);
	strTmp = (LPCTSTR)DBpara._User;
	SaveValue(_T("DATABASE"), _T("USER"), strTmp);
	strTmp = (LPCTSTR)DBpara._Server;
	SaveValue(_T("DATABASE"), _T("SERVER"), strTmp);
}
