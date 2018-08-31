#include "stdafx.h"
#include "iniconfig.h"

CIniConfig::CIniConfig(void)
{
}

CIniConfig::~CIniConfig(void)
{
}

void CIniConfig::ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT& nRet, UINT nDefault /*= 0*/)
{
	nRet = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_strIniFile);
}

void CIniConfig::ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT& nRet, UINT nDefault /*= 0*/)
{
	nRet = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_strIniFile);
}

void CIniConfig::ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, DWORD& nRet, UINT nDefault /*= 0*/)
{
	nRet = GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_strIniFile);
}

void CIniConfig::ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, CString& strRet, LPCTSTR lpDefault)
{
	CString strTmp;

	GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, strTmp.GetBuffer(10240), 10240, m_strIniFile);

	strTmp.ReleaseBuffer();

	strTmp.TrimLeft();

	strRet = strTmp;
}

void CIniConfig::ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, double& lfRet, double lfDefault)
{
	CString str;
	str.Format(_T("%f"),lfDefault);
	CString strRet;
	strRet.Empty();
	GetPrivateProfileString(lpAppName, lpKeyName, str, strRet.GetBuffer(10240), 10240, m_strIniFile);
	strRet.ReleaseBuffer();
	strRet.TrimLeft();
	
	lfRet = (float)_tstof(strRet.GetBuffer(MAX_PATH));
	strRet.ReleaseBuffer();
}

void CIniConfig::SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, CString& strValue)
{
	WritePrivateProfileString(lpAppName, lpKeyName, strValue, m_strIniFile);
}

void CIniConfig::SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT nValue)
{
	CString strTmp;

	strTmp.Format(_T("%d"), nValue);

	WritePrivateProfileString(lpAppName, lpKeyName, strTmp, m_strIniFile);
}

void CIniConfig::SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nValue)
{
	CString strTmp;

	strTmp.Format(_T("%d"), nValue);

	WritePrivateProfileString(lpAppName, lpKeyName, strTmp, m_strIniFile);
}

void CIniConfig::SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, DWORD nValue)
{
	CString strTmp;

	strTmp.Format(_T("%d"), nValue);

	WritePrivateProfileString(lpAppName, lpKeyName, strTmp, m_strIniFile);
}

void CIniConfig::SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, double nValue)
{
	CString strTmp;

	strTmp.Format(_T("%lf"), nValue);

	WritePrivateProfileString(lpAppName, lpKeyName, strTmp, m_strIniFile);
}