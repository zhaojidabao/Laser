#if !defined(AFX_DLGADDFILLSET_H__BEBC34F1_EC50_4F15_8D1D_124C5A1EC6FA__INCLUDED_)
#define AFX_DLGADDFILLSET_H__BEBC34F1_EC50_4F15_8D1D_124C5A1EC6FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddFillSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddFillSet dialog

class CDlgAddFillSet : public CDialog
{
// Construction
public:
	CDlgAddFillSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddFillSet)
	enum { IDD = IDD_DIALOG_ADDFILLSET };
	int	m_nNum;
	double	m_dbAddDis;
	double	m_dbAddAngle;
	BOOL	m_bSaveCountour;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddFillSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddFillSet)
	afx_msg void OnCheckSavecontour();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDFILLSET_H__BEBC34F1_EC50_4F15_8D1D_124C5A1EC6FA__INCLUDED_)
