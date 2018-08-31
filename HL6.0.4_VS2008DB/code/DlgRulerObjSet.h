#if !defined(AFX_DLGRULEROBJSET_H__34736C74_0C71_4CC7_871E_1EBA7664085A__INCLUDED_)
#define AFX_DLGRULEROBJSET_H__34736C74_0C71_4CC7_871E_1EBA7664085A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRulerObjSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRulerObjSet dialog

class CDlgRulerObjSet : public CDialog
{
// Construction
public:
	CDlgRulerObjSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRulerObjSet)
	enum { IDD = IDD_DIALOG_RULEROBJ_SET };
	BOOL	m_bViewNumber;
	int		m_nJumpCount;
	int		m_nDecimalDigits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRulerObjSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRulerObjSet)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRULEROBJSET_H__34736C74_0C71_4CC7_871E_1EBA7664085A__INCLUDED_)
