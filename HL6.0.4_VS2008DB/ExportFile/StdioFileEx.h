#pragma once

#define nUNICODE_BOM      0xFEFF						// Unicode "byte order mark" which goes at start of file
#define sNEWLINE			_T("\r\n")					// New line characters
#define DEFAULT_UNICODE_FILLER_CHAR		"#"	// Filler char used when no conversion from Unicode to local code page is possible



class AFX_EXT_CLASS CStdioFileEx: public CStdioFile
{
public:
	CStdioFileEx();
	CStdioFileEx( FILE* pOpenStream,BOOL bUnicode );
	CStdioFileEx( LPCTSTR lpszFileName, UINT nOpenFlags );

	virtual BOOL Open( LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL );
	virtual BOOL ReadString(CString& rString);
	virtual void  WriteString( LPCTSTR lpsz );
	BOOL  IsFileUnicodeText() { return m_bIsUnicodeText; } 
	unsigned long GetCharCount();

	// Additional flag to allow Unicode text writing
	static const UINT modeWriteUnicode;

	// static utility functions
	static BOOL  GetUnicodeFromMultiByte(char * szMultiByteString,wchar_t* szUnicodeString,
																		short nUnicodeBufferSize,UINT nCodePage=-1);

	static BOOL  GetMultiByteFromUnicode(wchar_t * szUnicodeString,char* szMultiByteString,
																		short nMultiByteBufferSize,UINT nCodePage=-1);

	static BOOL   IsFileUnicode(const CString& sFilePath);

protected:
	UINT  ProcessFlags(const CString& sFilePath, UINT& nOpenFlags);
	BOOL  m_bIsUnicodeText;
	UINT	 m_nFlags;
};

