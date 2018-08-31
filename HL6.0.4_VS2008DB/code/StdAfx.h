// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B8BC1849_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_STDAFX_H__B8BC1849_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <AFXPRIV.H>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <shlwapi.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#define _AFX_ALL_WARNINGS
#define  _SPECIALPROP_EDIT

#pragma warning(disable:4786)
//#pragma comment(linker,"/manifestdependency:\"type = 'win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = 'x86' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

//以下是本软件所加的库
#include <math.h>
#include <conio.h>
#include "CommDefine.h"

//自处理的序列化处理类
#include "MySerialize.h"
#include "Layer.h"

#define  CUR_DOC_VERSION  60001;

void		GetExepath(TCHAR *pPath,int nSize);
double	MyPrecision(double val,int digit = 3);
double	MyDegToRad(double val);
double	MyRadToDeg(double val);

typedef  unsigned  ( __stdcall *PTHREAD_START )( void * );
typedef int (APIENTRY *LPLOADDIB)(char*,int&,int&,int&,int&, BOOL,BOOL);
typedef int (APIENTRY *LPPROCDIB)(BYTE*,int,int,int,int,int,long,long);
typedef int (APIENTRY *LPPROCGRAYDIB)(BYTE*,int,int,int,int,int,long,long);
typedef int (APIENTRY *LPDELDIB)();

extern LPLOADDIB			LoadDib;
extern LPPROCDIB			ProcDib;
extern LPPROCGRAYDIB	ProcGrayDib;
extern LPDELDIB			DelDib;

int			ReadIniInt(TCHAR *pszFileName, LPCTSTR lpszSection,LPCTSTR lpszKey, LPCTSTR lpszDefaultValue);
void		WriteIniInt(TCHAR *pszFileName, LPCTSTR lpszSection,LPCTSTR lpszKey, int nValue);
CString	VerifyTimeFormat(const CString& strOrg);
CString GenTimeString( const CString& strDate );

#define MARKPREVIEW_SPEEDMAX	80		// 打标预览速度的最大值
#define MAXDELAY	65535					// 最大延时
extern  DWORD64	    g_dwLastObjId;		
extern  LANGID	    g_LangID;				// 系统语言ID
extern  DWORD	    g_dwVersion;			// 版本号
extern  SYSCFG	    g_SysCfg;				// 系统配置信息
extern  int				g_nCardType;
extern  CUIntArray	g_arryIDMap;

//全局外部变量
extern DWORD			g_dwJumpCnt;		// 陈列时跳号自动增加
extern BOOL			g_bIsSort;			// 系统当前是否在排序状态
extern BOOL			g_bIsRedPreview; // 红光预览
extern BOOL			g_bIsSelMark;		// 选择打标
extern BOOL			g_bIsFastMark;	// 快速打标
extern BOOL			g_bIsDownLoad;  // 
extern USERINFO		g_CurUserInfo;

extern CString      g_StrCopyTmpFile;

//运动功能显隐标记
#define  VIEW_MOTORFUN_ENABLE


//IO选择多文档信号口
#define I_MULSEL_0		0x10
#define I_MULSEL_1		0x20
#define I_MULSEL_2		0x40
#define I_MULSEL_3		0x80
#define I_MULSEL_4		0x800

//输出口对应标记
#define O_POWER				0x40		// 开关机
#define O_RED						0x8000	// 启用红光
#define O_ENT_MARK_ENV		0x10		// 光闸信息，进入打标环境时On,退出打标环境时Off

#include "system.h"
#include "BarCodeMgr.h"
#include <afx.h>

#define NEEDOUTPUTDBG

#ifdef  NEEDOUTPUTDBG
#define MYTRACE(sz)  \
	OutputDebugString(sz);

#define MYTRACE1(sz, p1) { \
	TCHAR szTemp[MAX_PATH,];\
	_stprintf_s(szTemp, MAX_PATH, sz, p1);\
	OutputDebugString(szTemp);}

#define MYTRACE2(sz, p1, p2){\
	TCHAR szTemp[MAX_PATH];\
	_stprintf_s(szTemp, sz, p1, p2);\
	OutputDebugString(szTemp);}

#define MYTRACE3(sz, p1, p2, p3){\
	TCHAR szTemp[MAX_PATH];\
	_stprintf_s(szTemp, MAX_PATH,sz, p1, p2, p3);\
	OutputDebugString(szTemp);}

#define MYTRACE4(sz, p1, p2, p3, p4){\
	TCHAR szTemp[MAX_PATH];\
	_stprintf_s(szTemp, MAX_PATH, sz, p1, p2, p3, p4);\
	OutputDebugString(szTemp);}

#define MYTRACE8(sz, p1, p2, p3, p4, p5, p6, p7, p8){\
	TCHAR szTemp[MAX_PATH];\
	_stprintf_s(szTemp, MAX_PATH, sz, p1, p2, p3, p4, p5, p6, p7, p8);\
	OutputDebugString(szTemp);}

#define MYTRACE10(sz, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10){\
	TCHAR szTemp[MAX_PATH];\
	_stprintf_s(szTemp, MAX_PATH, sz, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);\
	OutputDebugString(szTemp);}

#else
#define MYTRACE(sz)
#define MYTRACE1(sz,p1)
#define MYTRACE2(sz,p1,p2)
#define MYTRACE3(sz,p1,p2,p3)
#define MYTRACE4(sz,p1,p2,p3,p4)
#define MYTRACE8(sz, p1, p2, p3, p4, p5, p6, p7, p8)
#define MYTRACE10(sz,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)

#endif //End NEEDOUTPUTDBG

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type = 'win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = 'x86' publicKeyToken = '6595b64144ccf1df' language = '*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type = 'win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = 'ia64' publicKeyToken = '6595b64144ccf1df' language = '*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type = 'win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = 'amd64' publicKeyToken = '6595b64144ccf1df' language = '*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type = 'win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")
#endif
#endif


#endif // !defined(AFX_STDAFX_H__B8BC1849_DE24_11D3_845D_444553540001__INCLUDED_)

