#if !defined(AFX_DLGPROWAIT_H__57B49274_C720_4958_9C1C_EC5756F32B06__INCLUDED_)
#define AFX_DLGPROWAIT_H__57B49274_C720_4958_9C1C_EC5756F32B06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProWait.h : header file
//
#include "MgrWheelStroke.h"
#define  WM_WHEELPROC_EXIT			( WM_USER+300 )
/////////////////////////////////////////////////////////////////////////////
// CDlgProWait dialog

class CDlgProWait : public CDialog
{
// Construction
public:
	CDlgProWait(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDlgProWait)
	enum { IDD = IDD_DIALOG_PROWAIT };
	//}}AFX_DATA
public:
	int m_nProcessType;
	int m_nProState;
	CMgrWheelStroke m_mgrWheelStroke;

	BOOL Process();
	void SetChain(CChain *pChain, CChain *pChainSource);
	
private:
	CChain *m_pChain;
	CChain *m_pChainSource;
	BOOL    m_bExit;
	void    SetProcessInfo(int nPos);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProWait)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProWait)
	afx_msg void OnButtonExit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnWheelProExit(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROWAIT_H__57B49274_C720_4958_9C1C_EC5756F32B06__INCLUDED_)
