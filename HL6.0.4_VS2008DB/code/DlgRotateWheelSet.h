#if !defined(AFX_DLGROTATEWHEELSET_H__39EDFA1C_D3B7_443C_BC7D_F3BF6C607C8A__INCLUDED_)
#define AFX_DLGROTATEWHEELSET_H__39EDFA1C_D3B7_443C_BC7D_F3BF6C607C8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRotateWheelSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRotateWheelSet dialog

class CDlgRotateWheelSet : public CDialog
{
// Construction
public:
	CDlgRotateWheelSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRotateWheelSet)
	enum { IDD = IDD_DIALOG_ROTATEWHEEL_SET };
	double	m_dbDiameter;
	double	m_dbDivAngle;
	int		m_nFillNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRotateWheelSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRotateWheelSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditFillno();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROTATEWHEELSET_H__39EDFA1C_D3B7_443C_BC7D_F3BF6C607C8A__INCLUDED_)
