#if !defined(AFX_LANGCODEPAGE_H__17E1A9BD_F5BC_4d61_9333_73D93A431F39__INCLUDED_)
#define AFX_LANGCODEPAGE_H__17E1A9BD_F5BC_4d61_9333_73D93A431F39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SCodeLang
{
	UINT nCodePage;
	CString CodePageName;

	SCodeLang()
	{
		nCodePage = -1;
		CodePageName = _T("");
	};

	void operator = ( const SCodeLang & src )
	{
		if ( &src == this )	
			return;

		nCodePage = src.nCodePage;
		CodePageName = src.CodePageName;
	}
};

extern CArray<SCodeLang,SCodeLang> g_ArrayCodePage;
BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString);

BOOL EnumCodePages();


#endif