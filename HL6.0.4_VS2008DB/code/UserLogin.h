#if !defined(AFX_USERLOGIN_H__11A66D8C_902A_4AC7_9F16_D5956332E7A2__INCLUDED_)
#define AFX_USERLOGIN_H__11A66D8C_902A_4AC7_9F16_D5956332E7A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserLogin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserLogin dialog

class CUserLogin : public CDialog
{
// Construction
public:
	CUserLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserLogin)
	enum { IDD = IDD_DIALOG_USER_LOGIN };
	CComboBox	m_comboUser;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserLogin)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL  m_bIsOK;
	int   m_nLoginCount;
	DWORD m_dwDispMsg; // ��Ϣ����ʾ��Ϣ; 0������ʾ��1��û��Ĭ���û���2���û���Ϣ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLOGIN_H__11A66D8C_902A_4AC7_9F16_D5956332E7A2__INCLUDED_)
