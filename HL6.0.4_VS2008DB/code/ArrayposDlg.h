#if !defined(AFX_ARRAYPOSDLG_H__BA909242_DC9D_11D4_A485_00105A615525__INCLUDED_)
#define AFX_ARRAYPOSDLG_H__BA909242_DC9D_11D4_A485_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArrayposDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArrayPosDlg dialog

class CArrayPosDlg : public CDialog
{
// Construction
public:
	CArrayPosDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArrayPosDlg)
	enum { IDD = IDD_ARRAYPOS };
	double	m_dbX;
	double	m_dbY;
	double	m_dbR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArrayPosDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArrayPosDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARRAYPOSDLG_H__BA909242_DC9D_11D4_A485_00105A615525__INCLUDED_)
