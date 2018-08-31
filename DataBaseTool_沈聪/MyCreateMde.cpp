#include "StdAfx.h"
#include "MyCreateMde.h"
//#ifndef _AFX_DISABLE_DEPRECATED
#define _AFX_DISABLE_DEPRECATED
#include <afxdao.h>
CMyCreateMde::CMyCreateMde(void)
{
}

CMyCreateMde::~CMyCreateMde(void)
{
}

CString CMyCreateMde::CreateMdbFile()
{
	CDaoDatabase  mdb;

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	SYSTEMTIME  st;
	GetLocalTime(&st);
	CString strName;
	strName.Format(_T("%s\\%04d%02d%02d_%02d%02d%02d.mdb"), 
		strPath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	//strName = _T("123.mdb");
	mdb.Create(strName);
	mdb.Close();

	return strName;
}
