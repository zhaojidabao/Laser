#if !defined(AFX_TEXTPAGEFNTDLG_H__CC854D02_1465_4CE4_B83E_A5CE51178C5B__INCLUDED_)
#define AFX_TEXTPAGEFNTDLG_H__CC854D02_1465_4CE4_B83E_A5CE51178C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextPageFntDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextPageFntDlg dialog

class CTextPageFntDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTextPageFntDlg)

// Construction
public:
	void UpdateDataView();
	CTextPageFntDlg();
	~CTextPageFntDlg();

public:
	LOGFONT m_lf;

// Dialog Data
	//{{AFX_DATA(CTextPageFntDlg)
	enum { IDD = IDD_TEXT_PAGE_FNT };
	CComboBox	m_cbxAscType;
	CComboBox	m_cbxHzkType;
	CComboBox	m_comboUser;
	int		m_nFont;
	CComboBox	m_comboNUM;
	CComboBox	m_comboEN;
	CComboBox	m_comboCHS;
	double	m_dbCharSpace;
	double	m_dbWidthFactor;
	double	m_dbHeight;
	double	m_dbLineSpace;
	CString	m_strFontCHS;
	CString	m_strFontEN;
	CString	m_strFontNUM;
	CString	m_strFontUser;
	CString	m_strPrecisK;
	double	m_dbFixSpace;
	BOOL	m_bFixSpace;
	CString	m_strHzkType;
	CString	m_strAscType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTextPageFntDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTextPageFntDlg)
	afx_msg void OnRadioFontShx();
	afx_msg void OnRadioFontTtf();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTtf();
	afx_msg void OnRadioFontUser();
	afx_msg void OnCheckFixspace();
	afx_msg void OnRadioFontMatrix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void Layout();

public:
	afx_msg void OnCbnSelchangeComboPrecisK();
	BOOL m_bWeld;
	BOOL m_bInterFilter;
	double m_dbInterSpace;
	afx_msg void OnBnClickedCheckInterfilter();
	BOOL m_bBroaden;
	double m_dbLineWidth;
	double m_dbBroadenD;
	afx_msg void OnBnClickedCheckBroaden();
	double m_dbMatrixR;
	double m_dbMatrixFillD;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPAGEFNTDLG_H__CC854D02_1465_4CE4_B83E_A5CE51178C5B__INCLUDED_)
