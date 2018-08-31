// FontFile.h: interface for the CFontFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#if !defined(AFX_FONTFILE_H__71FE5184_5469_11D6_8225_00E04C3F1902__INCLUDED_)
#define AFX_FONTFILE_H__71FE5184_5469_11D6_8225_00E04C3F1902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUM_DOTOFCHAR	 135

struct Point{
	BOOL bOn;
	int x;
	int y;
};

struct PntStruct{
	int ASCNum;
	int ArrayStyle;
	int nPoint;
	Point point[NUM_DOTOFCHAR];
};

struct FontFileHead{
		double dbVersion;
		UINT uAscNum;
		UINT uCode;
		UINT uDotNum;
};

class CFontFile : public CObject  
{
public:
	BOOL IsCharExist(char c);
	BOOL CheckVersion(double dbVer);
	BOOL DelChar(char c);
	FontFileHead m_head;
	BOOL CheckVersion(double dbVer,UINT uAscNum,UINT uCode,UINT uDotNum);
	BOOL ChangFileOpenFlag(int nRead=1);
	BOOL Open(unsigned int flag);
	BOOL InitFontFile(double dbVer);
	BOOL Close();
	BOOL SaveTextFile();
	BOOL ClearDataFile();
	int ReadTextDataFile(CString fileName="");
	BOOL WriteData(PntStruct* pPnt);
	PntStruct* GetDataOfChar(char c);
//	void LookUpAllData(CStringArray* pAr);
	CFontFile();
	virtual ~CFontFile();
private:
	int m_nFileIsReading;
	CString m_strFilePathName;
	CString GetFileName(CString filename=_T(""));
	CArray<PntStruct*,PntStruct*>m_arDotChar;
	long m_lOffPos;
	BOOL WriteFileHead();
	FontFileHead* ReadFileHead();
	CFile* m_pFile;
	CString m_strFilePath;
	CString m_strFileName;
};

#endif // !defined(AFX_FONTFILE_H__71FE5184_5469_11D6_8225_00E04C3F1902__INCLUDED_)
