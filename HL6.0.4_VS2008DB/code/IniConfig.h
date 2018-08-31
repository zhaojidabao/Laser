#pragma once

class CIniConfig
{
protected :

	CIniConfig(void);
	virtual ~CIniConfig(void);

protected :

	CString m_strIniFile;

public :

	void ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, CString& strRet, LPCTSTR lpDefault = NULL);
	void ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT& nRet, UINT nDefault = 0);
	void ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT& nRet, UINT nDefault = 0);
	void ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, DWORD& nRet, UINT nDefault = 0);
	void ReadValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, double& lfRet, double lfDefault = 0.0f);

	void SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, CString& strValue);
	void SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nValue);
	void SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT nValue);
	void SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, DWORD nValue);
	void SaveValue(LPCTSTR lpAppName, LPCTSTR lpKeyName, double nValue);
};
