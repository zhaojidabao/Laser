#if !defined(AFX_TEXTBASEDLG_H__8D3014D9_4D82_453C_9482_3E7E8C00FAA6__INCLUDED_)
#define AFX_TEXTBASEDLG_H__8D3014D9_4D82_453C_9482_3E7E8C00FAA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextBaseDlg.h : header file
//
#include "TextPageDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTextBaseDlg dialog

class CTextBaseDlg : public CDialog
{
// Construction
public:
	CTextBaseDlg(CWnd* pParent = NULL);   // standard constructor
	void Update_Data(BOOL bSaveAndValidate = TRUE);

// Dialog Data
	//{{AFX_DATA(CTextBaseDlg)
	enum { IDD = IDD_TEXT_BASEDLG };
	CTabCtrl	m_ctrlTabBase;
	//}}AFX_DATA

	CTextPageDlg m_TextPage;
	CNodeText* m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextBaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextBaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabBase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabBase(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTBASEDLG_H__8D3014D9_4D82_453C_9482_3E7E8C00FAA6__INCLUDED_)
