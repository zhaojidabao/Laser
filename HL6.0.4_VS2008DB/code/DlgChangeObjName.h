#if !defined(AFX_DLGCHANGEOBJNAME_H__0A368083_B7E3_4FF5_AD07_3324468A16A0__INCLUDED_)
#define AFX_DLGCHANGEOBJNAME_H__0A368083_B7E3_4FF5_AD07_3324468A16A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChangeObjName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChangeObjName dialog

class CDlgChangeObjName : public CDialog
{
// Construction
public:
	CDlgChangeObjName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChangeObjName)
	enum { IDD = IDD_DIALOG_CHANGEOBJNAME };
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChangeObjName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChangeObjName)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHANGEOBJNAME_H__0A368083_B7E3_4FF5_AD07_3324468A16A0__INCLUDED_)
