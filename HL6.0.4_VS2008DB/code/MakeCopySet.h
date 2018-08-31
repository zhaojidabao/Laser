#if !defined(AFX_MAKECOPYSET_H__19A0B90F_1530_44F0_90B8_3DD799B7964A__INCLUDED_)
#define AFX_MAKECOPYSET_H__19A0B90F_1530_44F0_90B8_3DD799B7964A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeCopySet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeCopySet dialog

class CMakeCopySet : public CDialog
{
// Construction
public:
	CMakeCopySet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMakeCopySet)
	enum { IDD = IDD_MAKECP_SET };
	int		m_nMakeCpCount;
	int		m_nScanCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeCopySet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMakeCopySet)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKECOPYSET_H__19A0B90F_1530_44F0_90B8_3DD799B7964A__INCLUDED_)
