#include "afxwin.h"
#if !defined(AFX_BARPAGE_H__CE29F172_7846_41EA_9DDD_34000F923CAF__INCLUDED_)
#define AFX_BARPAGE_H__CE29F172_7846_41EA_9DDD_34000F923CAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBarPage dialog

class CBarPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBarPage)
	
// Construction
public:
	CBarPage();
	~CBarPage();

// Dialog Data
	//{{AFX_DATA(CBarPage)
	enum { IDD = IDD_BAR_PAGE };
	BOOL	m_bRev;
	BOOL    m_bGoBack;
	BOOL    m_bConnect;
	double	m_dbHeight;
	double	m_dbNarrow;
	double	m_dbShrink;
	double	m_dbWidth;
	double	m_dbSpace;
	double	m_dbAngle;
	DWORD   m_nSingleLineMarkCount;
	UINT	m_nNarrowLineNum;
	BOOL	m_bMatrix;
	double	m_dbHeightRev;
	BOOL	m_bAutoFill;
	BOOL	m_bParity;
	//}}AFX_DATA
	double m_dbDotRadius;

	// ïÎ¿Õ²ÎÊý
	BOOL m_bHollow;
	double m_dbHollowWidth;
	double m_dbHollowHeight;

	UINT m_nCodePage;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBarPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBarPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckRev();
	afx_msg void OnCheckHollow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbCodePage;
	afx_msg void OnCbnSelchangeComboCodepage();
	afx_msg void OnBnClickedButtonDeflng();
	BOOL m_bLangEnable;
	afx_msg void OnBnClickedCheckLang();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARPAGE_H__CE29F172_7846_41EA_9DDD_34000F923CAF__INCLUDED_)
