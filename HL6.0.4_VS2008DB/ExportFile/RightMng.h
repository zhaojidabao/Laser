// RightMng.h: interface for the CRightMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGHTMNG_H__371D9435_7920_4E0B_98D2_187B8D953BCB__INCLUDED_)
#define AFX_RIGHTMNG_H__371D9435_7920_4E0B_98D2_187B8D953BCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CRightMng  
{
public:
	CRightMng();
	virtual ~CRightMng();
	
public:
	void WriteRightInfo(CArray<USERINFO, USERINFO> &RightInfo, CString &strRightFile);
	void ReadRightInfo(CArray<USERINFO, USERINFO> &RightInfo, CString &strRightFile);
	
	void WriteSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFile);
	void ReadSpMarkInfo(OBJMARKSET &objMarkSet, CString &strFile);
	BOOL InfoCheck(CString strPassWord, USERINFO &userInfo);	
	
private:
	BOOL ReadDataFile(CString &FileName,  TCHAR *pszBuffer, DWORD &dwBufferLen);
	BOOL WriteDataFile(CString &FileName, TCHAR *pszBuffer, DWORD dwBufferLen);
	
	BOOL OpenDataFile(CString &FileName);
	BOOL CloseDataFile();
	void InfoEncrypt(TCHAR *pszPassWord, int nBufferLen);
	
private:
	CFile m_fDataFile;
};

#endif // !defined(AFX_RIGHTMNG_H__371D9435_7920_4E0B_98D2_187B8D953BCB__INCLUDED_)
