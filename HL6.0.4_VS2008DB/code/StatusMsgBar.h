#if !defined(AFX_STATUSMSGBAR_H__1B920E10_F132_4B6C_B763_D5F09E9AD6A9__INCLUDED_)
#define AFX_STATUSMSGBAR_H__1B920E10_F132_4B6C_B763_D5F09E9AD6A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusMsgBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusMsgBar dialog

class CStatusMsgBar : public CDialog
{
// Construction
public:
	CStatusMsgBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatusMsgBar)
	enum { IDD = IDD_DIALOG_STATUS_BAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusMsgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatusMsgBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSMSGBAR_H__1B920E10_F132_4B6C_B763_D5F09E9AD6A9__INCLUDED_)
