#if !defined(AFX_ELLIPSETDLG_H__539DBB1D_4C90_4E00_9B10_A314C26A6B5D__INCLUDED_)
#define AFX_ELLIPSETDLG_H__539DBB1D_4C90_4E00_9B10_A314C26A6B5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EllipSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEllipSetDlg dialog

class CEllipSetDlg : public CDialog
{
// Construction
public:
	CEllipSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEllipSetDlg)
	enum { IDD = IDD_ELLIPSET };
	double	m_dbRuler;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEllipSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEllipSetDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELLIPSETDLG_H__539DBB1D_4C90_4E00_9B10_A314C26A6B5D__INCLUDED_)
