#if !defined(AFX_INDEXFILE_H__E7FA7D05_DA0A_11D7_A553_000AE68DB320__INCLUDED_)
#define AFX_INDEXFILE_H__E7FA7D05_DA0A_11D7_A553_000AE68DB320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndexFile.h : header file
//
#include "FontFile.h"
/////////////////////////////////////////////////////////////////////////////
// CUserFontFile window

#define STATE_NEW_CREATE 0X01
#define STATE_MODIFY     0X02
#define STATE_DELETE     0X04
#define VERSION			 1.0

const CString FONTFILENAME  = _T("\\DFonts\\ASC7");

typedef struct _INDEXFILEHEAD
{
	double dbVersion;		//版本号
	DWORD nFontNumber;     //当前字体数目
	UINT  nDoubleIndex;    //汉字起始索引
	DWORD nReserve[20];   //备用
}INDEXFILEHEAD;

typedef struct _USERFONTFILE
{
	WORD nFontCode;		 //字符代码
	BOOL bIsDouble;      //是否汉字          
	UINT nIndex;         //自身索引
	UINT bState;         //状态位      1: 新建 2：修改 4:删除
	UINT nLatticeStyle;  //点阵类型
	int nPointNumber;    //点数
	Point point[NUM_DOTOFCHAR]; //点坐标数组
	BYTE nReserve[36];   //备用
}USERFONTFILE;

class CUserFontFile : public CObject
{
// Construction
public:
	CUserFontFile();       
	
// Attributes
public:
	INDEXFILEHEAD m_nIndexFileHead;          //索引文件头
	CList<USERFONTFILE*,USERFONTFILE*> m_UserFontList;//索引列表
	CFile* m_pUserFontFile;                     //字体文件
	CString m_strFilePath;
	CString m_strFileName;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserFontFile)
	//}}AFX_VIRTUAL

// Implementation
public:
	USERFONTFILE* pSpace;
	BOOL bIsOpen;
	USERFONTFILE* GetDataOfChar(WORD c);
	void SaveAll();
	BOOL DeleteIndex(WORD nCode);
	void CloseFile();
	void OpenFile(UINT nOpenFlag);
	void SaveModify(USERFONTFILE *pIndex);
	void SaveHead();
	void LoadFile();
	void SaveFile();
	USERFONTFILE* FindIndex(WORD nFontCode,BOOL bIsDouble = FALSE);
	BOOL JurdgeVersion(double nVersion);
	CString GetFileName(CString filename = _T(""));
	virtual ~CUserFontFile();

	// Generated message map functions
protected:
	
private:
	void InitSpace();
	void LoadHead();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXFILE_H__E7FA7D05_DA0A_11D7_A553_000AE68DB320__INCLUDED_)
