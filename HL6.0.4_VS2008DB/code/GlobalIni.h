// GlobalIni.h: interface for the CGlobalIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALINI_H__F1047289_229F_4B76_81AF_63BFA1872342__INCLUDED_)
#define AFX_GLOBALINI_H__F1047289_229F_4B76_81AF_63BFA1872342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Singleton.h"
#include "IniConfig.h"
#include "AdoDb.h"

class CGlobalIni : public CIniConfig
{
	SINGLETON_DECLARE(CGlobalIni)

public:
	//ִ��Ŀ¼
	static CString GetAppPath();

public:
	//����/�л�INI��Ĭ�������ļ�·��
	CString GetIniFullPath() const { return m_strIniFile; }
	void SetIniFullPath(CString strIniPath)  { m_strIniFile = strIniPath; }

	void GetDBPara(DBPARA& DBpara);
	void SetDBPara(DBPARA& DBpara);
};

#endif // !defined(AFX_GLOBALINI_H__F1047289_229F_4B76_81AF_63BFA1872342__INCLUDED_)
