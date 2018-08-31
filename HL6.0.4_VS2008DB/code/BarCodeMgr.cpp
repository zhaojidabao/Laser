// BarCodeMgr.cpp: implementation of the CBarCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "BarCodeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
 
CBarCodeMgr g_BarCodeMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarCodeMgr::CBarCodeMgr()
{
	//建立与BARTYPE相对应的字串
	m_BarNameArray.Add(_T("EAN 8"));
	m_BarNameArray.Add(_T("EAN 13"));
	m_BarNameArray.Add(_T("EAN 128"));
	m_BarNameArray.Add(_T("CODE 128A"));
	m_BarNameArray.Add(_T("CODE 128B"));
	m_BarNameArray.Add(_T("CODE 128C"));
	m_BarNameArray.Add(_T("CODE 128AUTO"));
	m_BarNameArray.Add(_T("CODE 39"));
	m_BarNameArray.Add(_T("INT 25"));
	m_BarNameArray.Add(_T("EXT 39"));
	m_BarNameArray.Add(_T("UPCA"));
	m_BarNameArray.Add(_T("UPCE"));
	m_BarNameArray.Add(_T("PDF417"));
	m_BarNameArray.Add(_T("DATA MATRIX"));
	m_BarNameArray.Add(_T(" QR "));
	m_BarNameArray.Add(_T("Micro QR"));
	m_BarNameArray.Add(_T("CODE 25"));
	m_BarNameArray.Add(_T("CODE 93"));
	m_BarNameArray.Add(_T("CODE 93 EX"));
	m_BarNameArray.Add(_T("HANXIN"));
}

CBarCodeMgr::~CBarCodeMgr()
{

}


BOOL CBarCodeMgr::GenBarCode(CNodeCodebar *pNodeCodeBar, CString &strText)
{	
	if ( strText.IsEmpty() )
		return FALSE;

	char  szText[1024] = { 0 };
	BYTE szOutString[1024] = { 0 };
	IMATRIXBAR iMatrix;
	IS_GEN_CODEBAR_1DIM  iCodeBar1;
	
	CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()), szText, 1024);
	strText.ReleaseBuffer();
	
	BOOL bRtn = TRUE;
	switch( pNodeCodeBar->m_BarType ) 
	{
	case EAN8:
		{
			//数字，位数只能为7位＋1个校验位
			int nLen = strText.GetLength();
			if ( nLen > 7 )
			{
				strText = strText.Left(7);
			}
			else
			{
				for( int i = 0; i < 7-nLen; i++)
					strText.Insert( 0, _T('0') );
			}

			CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()),szText,1024);
			strText.ReleaseBuffer();
			
			iCodeBar1._type = eCodebar1Dim_Ean8;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);

			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn  !=  0)
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case EAN13:
		{
			//数字，位数只能为12位＋1个校验位
			int nLen = strText.GetLength();
			if ( nLen>12 )
			{
				strText = strText.Left(12);
			}
			else
			{
				for( int i = 0; i<12-nLen; i++ )
					strText.Insert(0, _T('0'));
			}
			
			CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()),szText,1024);
			strText.ReleaseBuffer();
			
			iCodeBar1._type = eCodebar1Dim_Ean13;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff   = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case EAN128:
		{
			iCodeBar1._type = eCodebar1Dim_Ean128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE128A:
		{
			InsertChar( szText, 1024, char(0xF1) );			
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE128B:
		{
			InsertChar( szText, 1024, char(0xF2) );		
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE128C:
		{			
			InsertChar( szText, 1024, char(0xF3) );		
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff   = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize  = 1024;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol  = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;

			strText = (char*)szOutString;
			break;
		}
	case CODE128AUTO:
		{
			iCodeBar1._type = eCodebar1Dim_Code128Auto;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn != 0)
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE39:
		{
			iCodeBar1._type = eCodebar1Dim_Code39;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw39 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE93:
		{
			iCodeBar1._type = eCodebar1Dim_Code93;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw93 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE93EX:
		{
			iCodeBar1._type = eCodebar1Dim_Code93Ex;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw93 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE25:
		{
			iCodeBar1._type = eCodebar1Dim_Code25;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL,m_BarCode1.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case INT25:
		{
			iCodeBar1._type = eCodebar1Dim_Inter25;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw25 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn  !=  0)
			{
				bRtn = FALSE;
				MessageBoxA( NULL,m_BarCode1.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case EXT39:
		{
			iCodeBar1._type = eCodebar1Dim_Code39Ex;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw39 = pNodeCodeBar->m_bParity;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case UPCA:
		{
			//数字，位数只能为11位＋1个校验位
			int nLen = strText.GetLength();
			if ( nLen > 11 )
			{
				strText = strText.Left(11);
			}
			else
			{
				for ( int i = 0; i<11-nLen; i++ )
					strText.Insert( 0,_T('0') );
			}

			CStdioFileEx::GetMultiByteFromUnicode( strText.GetBuffer(strText.GetLength()), szText, 1024);
			strText.ReleaseBuffer();

			iCodeBar1._type = eCodebar1Dim_UpcA;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn != 0)
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;

			strText = (char*)szOutString;
			break;
		}
	case UPCE:
		{
			//数字，位数只能为6位＋2个校验位
			int nLen = strText.GetLength();
			if ( nLen > 6  )
			{
				strText = strText.Left(6);
			}
			else
			{
				for ( int i = 0; i < 6-nLen; i++)
					strText.Insert(0,_T('0'));
			}

			CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()),szText,1024);
			strText.ReleaseBuffer();

			iCodeBar1._type = eCodebar1Dim_UpcE;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff   = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff   = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize  = 1024;
			
			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol  = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case PDF417:
		{
			iMatrix._type = 3;
			iMatrix._u_value._pdf417._bits._bConver = 0;
			iMatrix._u_value._pdf417._bits._countsConver = 0;
			iMatrix._u_value._pdf417._bits._nCols = 0;		
			iMatrix._u_value._pdf417._bits._nRows = pNodeCodeBar->m_nOutline;		//
			iMatrix._u_value._pdf417._xScale = 1;
			iMatrix._u_value._pdf417._yScale = 1;
			iMatrix._u_value._pdf417._sizeSrcBytes = strlen(szText);
			iMatrix._u_value._pdf417._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._pdf417._lpszRetBuffer = pNodeCodeBar->m_pByte;
			iMatrix._u_value._pdf417._sizeRetBuffer = 256*256;
			iMatrix._u_value._pdf417._bits._eccLevel = pNodeCodeBar->m_nErrLev;
			
			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._pdf417._sizeRetRows;
			pNodeCodeBar->m_nCol = iMatrix._u_value._pdf417._sizeRetCols;
			break;
		}
	case DATAMATRIX:
		{
			iMatrix._type = 2;
			iMatrix._u_value._dataMatrix._bits._bConver = 0;
			iMatrix._u_value._dataMatrix._bits._countsConver = 0;
			iMatrix._u_value._dataMatrix._symbolNo = pNodeCodeBar->m_nSymbolNo;	//
			iMatrix._u_value._dataMatrix._bits._eccType = pNodeCodeBar->m_nErrLev;			//
			iMatrix._u_value._dataMatrix._shapeNo = pNodeCodeBar->m_nShapeNo;			//
			iMatrix._u_value._dataMatrix._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._dataMatrix._sizeSrcBytes = strlen(szText);
			iMatrix._u_value._dataMatrix._lpszRetBuffer = pNodeCodeBar->m_pByte;
			iMatrix._u_value._dataMatrix._sizeRetBuffer = 256*256;
			
			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._dataMatrix._sizeRetRows;
			pNodeCodeBar->m_nCol = iMatrix._u_value._dataMatrix._sizeRetCols;
			break;
		}
	case QR:
		{
			iMatrix._type = 1;
			iMatrix._u_value._qr._bits._bConver		 = 0;
			iMatrix._u_value._qr._bits._countsConver = 0;
			iMatrix._u_value._qr._bits._bMicro = FALSE;
			iMatrix._u_value._qr._bits._levelType		 =  pNodeCodeBar->m_nErrLev;
			iMatrix._u_value._qr._bits._versionType	 =  pNodeCodeBar->m_nHPoint;
			iMatrix._u_value._qr._bits._codetype		 =  pNodeCodeBar->m_nCodeType;
			iMatrix._u_value._qr._lpszRetBuffer			 =  pNodeCodeBar->m_pByte;
			
			iMatrix._u_value._qr._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._qr._sizeRetBuffer = 256*256;
			iMatrix._u_value._qr._sizeSrcBytes = strlen(szText);
			
			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._qr._sizeRetRowsOrCols;
			pNodeCodeBar->m_nCol = iMatrix._u_value._qr._sizeRetRowsOrCols;

			break;
		}
	case MicroQR:
		{
			iMatrix._type = 1;
			iMatrix._u_value._qr._bits._bConver		 = 0;
			iMatrix._u_value._qr._bits._countsConver = 0;
			iMatrix._u_value._qr._bits._bMicro = TRUE; //Micro QR标志
			iMatrix._u_value._qr._bits._levelType		 =  pNodeCodeBar->m_nErrLev;
			iMatrix._u_value._qr._bits._versionType	 =  pNodeCodeBar->m_nHPoint;
			iMatrix._u_value._qr._bits._codetype		 =  pNodeCodeBar->m_nCodeType;
			iMatrix._u_value._qr._lpszRetBuffer			 =  pNodeCodeBar->m_pByte;
			iMatrix._u_value._qr._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._qr._sizeRetBuffer = 256*256;
			iMatrix._u_value._qr._sizeSrcBytes = strlen(szText);
			
			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._qr._sizeRetRowsOrCols;
			pNodeCodeBar->m_nCol = iMatrix._u_value._qr._sizeRetRowsOrCols;
			
			break;
		}
	case HANXIN:
		{
			iMatrix._type = 4;
			iMatrix._u_value._hanxin._bits._levelType = pNodeCodeBar->m_nErrLev;
			iMatrix._u_value._hanxin._bits._versionType = pNodeCodeBar->m_nVersion;
			iMatrix._u_value._hanxin._lpszRetBuffer			 =  pNodeCodeBar->m_pByte;

			iMatrix._u_value._hanxin._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._hanxin._sizeRetBuffer = 256*256;
			iMatrix._u_value._hanxin._sizeSrcBytes = strlen(szText);

			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._hanxin._sizeRetRowsOrCols;
			pNodeCodeBar->m_nCol = iMatrix._u_value._hanxin._sizeRetRowsOrCols;

			break;
		}
	}
	
	return bRtn;
}


BOOL CBarCodeMgr::GenBarCode(CNodeCodebar *pNodeCodeBar, CString &strText,UINT nCodePage)
{	
	if ( strText.IsEmpty() )
		return FALSE;

	char  szText[1024] = { 0 };
	BYTE szOutString[1024] = { 0 };
	IMATRIXBAR iMatrix;
	IS_GEN_CODEBAR_1DIM  iCodeBar1;

	CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()), szText, 1024,nCodePage);
	strText.ReleaseBuffer();

	BOOL bRtn = TRUE;
	switch( pNodeCodeBar->m_BarType ) 
	{
	case EAN8:
		{
			//数字，位数只能为7位＋1个校验位
			int nLen = strText.GetLength();
			if ( nLen > 7 )
			{
				strText = strText.Left(7);
			}
			else
			{
				for( int i = 0; i < 7-nLen; i++)
					strText.Insert( 0, _T('0') );
			}

			CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()),szText,1024,nCodePage);
			strText.ReleaseBuffer();

			iCodeBar1._type = eCodebar1Dim_Ean8;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn  !=  0)
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case EAN13:
		{
			//数字，位数只能为12位＋1个校验位
			int nLen = strText.GetLength();
			if ( nLen>12 )
			{
				strText = strText.Left(12);
			}
			else
			{
				for( int i = 0; i<12-nLen; i++ )
					strText.Insert(0, _T('0'));
			}

			CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()),szText,1024,nCodePage);
			strText.ReleaseBuffer();

			iCodeBar1._type = eCodebar1Dim_Ean13;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff   = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case EAN128:
		{
			iCodeBar1._type = eCodebar1Dim_Ean128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE128A:
		{
			InsertChar( szText, 1024, char(0xF1) );			
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE128B:
		{
			InsertChar( szText, 1024, char(0xF2) );		
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE128C:
		{			
			InsertChar( szText, 1024, char(0xF3) );		
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff   = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize  = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol  = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;

			strText = (char*)szOutString;
			break;
		}
	case CODE128AUTO:
		{
			iCodeBar1._type = eCodebar1Dim_Code128Auto;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn != 0)
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE39:
		{
			iCodeBar1._type = eCodebar1Dim_Code39;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw39 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE93:
		{
			iCodeBar1._type = eCodebar1Dim_Code93;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw93 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE93EX:
		{
			iCodeBar1._type = eCodebar1Dim_Code93Ex;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw93 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA(NULL,m_BarCode1.GetErrInfo(nRtn,g_LangID),"Error",MB_ICONQUESTION);
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case CODE25:
		{
			iCodeBar1._type = eCodebar1Dim_Code25;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL,m_BarCode1.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case INT25:
		{
			iCodeBar1._type = eCodebar1Dim_Inter25;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw25 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn  !=  0)
			{
				bRtn = FALSE;
				MessageBoxA( NULL,m_BarCode1.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case EXT39:
		{
			iCodeBar1._type = eCodebar1Dim_Code39Ex;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;
			iCodeBar1._s_value._uExtraInput._dw39 = pNodeCodeBar->m_bParity;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case UPCA:
		{
			//数字，位数只能为11位＋1个校验位
			int nLen = strText.GetLength();
			if ( nLen > 11 )
			{
				strText = strText.Left(11);
			}
			else
			{
				for ( int i = 0; i<11-nLen; i++ )
					strText.Insert( 0,_T('0') );
			}

			CStdioFileEx::GetMultiByteFromUnicode( strText.GetBuffer(strText.GetLength()), szText, 1024,nCodePage);
			strText.ReleaseBuffer();

			iCodeBar1._type = eCodebar1Dim_UpcA;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol = iCodeBar1._s_value._nRltBinarySize;
			if (nRtn != 0)
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;

			strText = (char*)szOutString;
			break;
		}
	case UPCE:
		{
			//数字，位数只能为6位＋2个校验位
			int nLen = strText.GetLength();
			if ( nLen > 6  )
			{
				strText = strText.Left(6);
			}
			else
			{
				for ( int i = 0; i < 6-nLen; i++)
					strText.Insert(0,_T('0'));
			}

			CStdioFileEx::GetMultiByteFromUnicode(strText.GetBuffer(strText.GetLength()),szText,1024,nCodePage);
			strText.ReleaseBuffer();

			iCodeBar1._type = eCodebar1Dim_UpcE;
			iCodeBar1._s_value._lpcbSrc = (BYTE*)(szText);
			iCodeBar1._s_value._nSrcSize = strlen(szText);
			iCodeBar1._s_value._lpcbRetBinaryBuff   = pNodeCodeBar->m_pByte;
			iCodeBar1._s_value._nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._lpcbRetStrBuff   = szOutString;
			iCodeBar1._s_value._nRetStrBuffSize  = 1024;

			int nRtn = m_BarCode1.GenCodebar(&iCodeBar1);
			pNodeCodeBar->m_nCol  = iCodeBar1._s_value._nRltBinarySize;
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode1.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = 1;
			strText = (char*)szOutString;
			break;
		}
	case PDF417:
		{
			iMatrix._type = 3;
			iMatrix._u_value._pdf417._bits._bConver = 0;
			iMatrix._u_value._pdf417._bits._countsConver = 0;
			iMatrix._u_value._pdf417._bits._nCols = 0;		
			iMatrix._u_value._pdf417._bits._nRows = pNodeCodeBar->m_nOutline;		//
			iMatrix._u_value._pdf417._xScale = 1;
			iMatrix._u_value._pdf417._yScale = 1;
			iMatrix._u_value._pdf417._sizeSrcBytes = strlen(szText);
			iMatrix._u_value._pdf417._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._pdf417._lpszRetBuffer = pNodeCodeBar->m_pByte;
			iMatrix._u_value._pdf417._sizeRetBuffer = 256*256;
			iMatrix._u_value._pdf417._bits._eccLevel = pNodeCodeBar->m_nErrLev;

			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._pdf417._sizeRetRows;
			pNodeCodeBar->m_nCol = iMatrix._u_value._pdf417._sizeRetCols;
			break;
		}
	case DATAMATRIX:
		{
			iMatrix._type = 2;
			iMatrix._u_value._dataMatrix._bits._bConver = 0;
			iMatrix._u_value._dataMatrix._bits._countsConver = 0;
			iMatrix._u_value._dataMatrix._symbolNo = pNodeCodeBar->m_nSymbolNo;	//
			iMatrix._u_value._dataMatrix._bits._eccType = pNodeCodeBar->m_nErrLev;			//
			iMatrix._u_value._dataMatrix._shapeNo = pNodeCodeBar->m_nShapeNo;			//
			iMatrix._u_value._dataMatrix._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._dataMatrix._sizeSrcBytes = strlen(szText);
			iMatrix._u_value._dataMatrix._lpszRetBuffer = pNodeCodeBar->m_pByte;
			iMatrix._u_value._dataMatrix._sizeRetBuffer = 256*256;

			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn,g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._dataMatrix._sizeRetRows;
			pNodeCodeBar->m_nCol = iMatrix._u_value._dataMatrix._sizeRetCols;
			break;
		}
	case QR:
		{
			iMatrix._type = 1;
			iMatrix._u_value._qr._bits._bConver		 = 0;
			iMatrix._u_value._qr._bits._countsConver = 0;
			iMatrix._u_value._qr._bits._bMicro = FALSE;
			iMatrix._u_value._qr._bits._levelType		 =  pNodeCodeBar->m_nErrLev;
			iMatrix._u_value._qr._bits._versionType	 =  pNodeCodeBar->m_nHPoint;
			iMatrix._u_value._qr._bits._codetype		 =  pNodeCodeBar->m_nCodeType;
			iMatrix._u_value._qr._lpszRetBuffer			 =  pNodeCodeBar->m_pByte;

			iMatrix._u_value._qr._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._qr._sizeRetBuffer = 256*256;
			iMatrix._u_value._qr._sizeSrcBytes = strlen(szText);

			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._qr._sizeRetRowsOrCols;
			pNodeCodeBar->m_nCol = iMatrix._u_value._qr._sizeRetRowsOrCols;

			break;
		}
	case MicroQR:
		{
			iMatrix._type = 1;
			iMatrix._u_value._qr._bits._bConver		 = 0;
			iMatrix._u_value._qr._bits._countsConver = 0;
			iMatrix._u_value._qr._bits._bMicro = TRUE; //Micro QR标志
			iMatrix._u_value._qr._bits._levelType		 =  pNodeCodeBar->m_nErrLev;
			iMatrix._u_value._qr._bits._versionType	 =  pNodeCodeBar->m_nHPoint;
			iMatrix._u_value._qr._bits._codetype		 =  pNodeCodeBar->m_nCodeType;
			iMatrix._u_value._qr._lpszRetBuffer			 =  pNodeCodeBar->m_pByte;
			iMatrix._u_value._qr._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._qr._sizeRetBuffer = 256*256;
			iMatrix._u_value._qr._sizeSrcBytes = strlen(szText);

			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._qr._sizeRetRowsOrCols;
			pNodeCodeBar->m_nCol = iMatrix._u_value._qr._sizeRetRowsOrCols;

			break;
		}
	case HANXIN:
		{
			iMatrix._type = 4;
			iMatrix._u_value._hanxin._bits._levelType = pNodeCodeBar->m_nErrLev;
			iMatrix._u_value._hanxin._bits._versionType = pNodeCodeBar->m_nVersion;
			iMatrix._u_value._hanxin._lpszRetBuffer			 =  pNodeCodeBar->m_pByte;

			iMatrix._u_value._hanxin._lpszSrc = (BYTE*)(szText);
			iMatrix._u_value._hanxin._sizeRetBuffer = 256*256;
			iMatrix._u_value._hanxin._sizeSrcBytes = strlen(szText);

			int nRtn = m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if ( nRtn  !=  0 )
			{
				bRtn = FALSE;
				MessageBoxA( NULL, m_BarCode2.GetErrInfo(nRtn, g_LangID), "Error", MB_ICONQUESTION );
			}
			pNodeCodeBar->m_nRow = iMatrix._u_value._hanxin._sizeRetRowsOrCols;
			pNodeCodeBar->m_nCol = iMatrix._u_value._hanxin._sizeRetRowsOrCols;

			break;
		}
	}

	return bRtn;
}



void CBarCodeMgr::InsertChar( char *pData, int nLen, char cValue )
{
	char  *pTemp = new char[nLen];	
	
	memset( pTemp, 0, nLen*sizeof(char) );
	memcpy( pTemp, pData, nLen*sizeof(char) );
	memset( pData, 0, nLen*sizeof(char) );
	pData[0] = cValue;
	
	for( int i = 0; i<nLen; i++ )
	{
		if ( pTemp[i] == 0 )
		{
			break;
		}
		
		pData[i+1] = pTemp[i];
	}
	
	delete []pTemp;
}

