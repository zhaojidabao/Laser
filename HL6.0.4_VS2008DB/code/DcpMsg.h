#if !defined(AFX_DCPMSG_H__8D16DE0C_A099_4C1D_9D96_741761DDC60C__INCLUDED_)
#define AFX_DCPMSG_H__8D16DE0C_A099_4C1D_9D96_741761DDC60C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DcpMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDcpMsg dialog

class CDcpMsg : public CDialog
{
// Construction
public:
	CDcpMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDcpMsg)
	enum { IDD = IDD_DIALOG_DCPMSG };
	CString	m_strDcpMsg;
	BOOL	m_bRedOpen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDcpMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDcpMsg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLaserstatus();
	afx_msg void OnCheckRed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetLaserstatus(BOOL bDcpStatus);
	void SetDispMsg(CString &strDcpMsg);

public:
	BOOL m_bLaserSatus;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCPMSG_H__8D16DE0C_A099_4C1D_9D96_741761DDC60C__INCLUDED_)
