#if !defined(AFX_DLGPATHOPTIMIZESET_H__50A08EDC_6BAC_4C7B_9931_74132FA679DA__INCLUDED_)
#define AFX_DLGPATHOPTIMIZESET_H__50A08EDC_6BAC_4C7B_9931_74132FA679DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPathOptimizeSet.h : header file
//
#include "Label.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPathOptimizeSet dialog

class CDlgPathOptimizeSet : public CDialog
{
// Construction
public:
	CDlgPathOptimizeSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPathOptimizeSet)
	enum { IDD = IDD_DIALOG_PATHOPTIMIZE_SET };
	CLabel	m_staticWarn;
	int		m_nOptimizeType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPathOptimizeSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPathOptimizeSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPATHOPTIMIZESET_H__50A08EDC_6BAC_4C7B_9931_74132FA679DA__INCLUDED_)
