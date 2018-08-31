#if !defined(AFX_EDITUSERNAMEDLG_H__C18486CE_3491_44A4_86B2_35E936CED973__INCLUDED_)
#define AFX_EDITUSERNAMEDLG_H__C18486CE_3491_44A4_86B2_35E936CED973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditUserNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditUserNameDlg dialog

class CEditUserNameDlg : public CDialog
{
// Construction
public:
	CEditUserNameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditUserNameDlg)
	enum { IDD = IDD_DIALOG_EDITUSERNAME };
	CString	m_strNewUserName;
	CString	m_strOldUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditUserNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditUserNameDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int  m_nCount;
	BOOL m_bIsOk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITUSERNAMEDLG_H__C18486CE_3491_44A4_86B2_35E936CED973__INCLUDED_)
