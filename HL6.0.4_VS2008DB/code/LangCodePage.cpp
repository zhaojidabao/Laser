#include "stdafx.h"
#include "LangCodePage.h"

CArray<SCodeLang,SCodeLang> g_ArrayCodePage;

BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString)
{
	CPINFOEX info = {0};
	UINT cp = _ttoi(lpCodePageString);

	CString strPageName;
	int pos0 = -1;
	int pos1 = -1;
	SCodeLang sl;
	if (GetCPInfoEx(cp, 0, &info))
	{
		strPageName = info.CodePageName;
		strPageName.Trim();
		pos0 = strPageName.Find(_T('('));
		pos1 = strPageName.Find(_T(')'));
		if( pos0 >= 0 && pos1 > pos0 )
		{
			strPageName = strPageName.Mid(pos0+1,pos1-1-pos0);
		}
		sl.nCodePage = cp;
		sl.CodePageName = strPageName;
		g_ArrayCodePage.Add(sl);
	}
	return TRUE;
}

BOOL EnumCodePages()
{
	g_ArrayCodePage.RemoveAll();
	BOOL bRet =  EnumSystemCodePages(EnumCodePagesProc, CP_INSTALLED);
	if( bRet && g_ArrayCodePage.GetCount() > 0 )
		return TRUE;
	else
		return FALSE;
}
