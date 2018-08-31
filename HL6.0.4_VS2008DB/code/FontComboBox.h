#if !defined(AFX_FONTCOMBOBOX_H__EBE597C1_AAD4_11D7_8AEE_000AE6A39D2F__INCLUDED_)
#define AFX_FONTCOMBOBOX_H__EBE597C1_AAD4_11D7_8AEE_000AE6A39D2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontComboBox.h : header file
#include <afxtempl.h>
#include "FntTipComb.h"

/////////////////////////////////////////////////////////////////////////////
// CFontComboBox window

class AFX_EXT_CLASS CFontComboBox : public CFntTipComb
{
// Construction
	DECLARE_DYNAMIC(CFontComboBox)
public:
	CFontComboBox();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontComboBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void    SetCurSelFont(const LOGFONT &lfData);
	void    SetCurSelFont(const CString &strFontName);
	LOGFONT GetCurSelFont() const;
	CString GetCurSelFontName() const;
	int		AddNewFont(const ENUMLOGFONTEX *, const NEWTEXTMETRICEX *, unsigned long);

	virtual ~CFontComboBox();
	void InitFontComboBox();
	// Generated message map functions
protected:
	LOGFONT m_lfDefault;
	CArray<LOGFONT,LOGFONT> m_fontArray;
	////////////////////////
	
	//{{AFX_MSG(CFontComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTCOMBOBOX_H__EBE597C1_AAD4_11D7_8AEE_000AE6A39D2F__INCLUDED_)
