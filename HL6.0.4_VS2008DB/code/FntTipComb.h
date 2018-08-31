#if !defined(AFX_MYCOMB_H__4B46D685_0638_4034_A3C7_23AE395E428D__INCLUDED_)
#define AFX_MYCOMB_H__4B46D685_0638_4034_A3C7_23AE395E428D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FntTipComb.h : header file
//
#include <afxwin.h>
#include <afxtempl.h>
#include <Windows.h>

/////////////////////////////////////////////////////
#define TYPE_TTF   0
#define TYPE_SHX   1
#define TYPE_USER  2

class AFX_EXT_CLASS CFntTipComb : public CComboBox
{
// Construction
public:
	CFntTipComb();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmyComb)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetType(int nType,CString strText);
	void ResetString(CString strText);

	//说明:
	//此函数对于每个基类为CFntTipComb的Combox必须被调用一次
	//nType:TYPE_TTF,TYPE_SHX,TYPE_USER
	//strText为显示的内容
	virtual ~CFntTipComb();
	// Generated message map functions
protected:
	//{{AFX_MSG(CmyComb)
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

	RECT	     m_rectClient;
	BOOL	     m_bFirst;
	int          m_nType;
	HMODULE m_hModule;

protected:
	CString GetExePath();
	void	FindAndSet(int nSet,CString strText);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCOMB_H__4B46D685_0638_4034_A3C7_23AE395E428D__INCLUDED_)
