#if !defined(AFX_DLGOBJPRECISIONSET_H__EFF1A03D_2D27_4F81_868F_7C7602485BE6__INCLUDED_)
#define AFX_DLGOBJPRECISIONSET_H__EFF1A03D_2D27_4F81_868F_7C7602485BE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjPrecisionSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgObjPrecisionSet dialog

class CDlgObjPrecisionSet : public CDialog
{
// Construction
public:
	CDlgObjPrecisionSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjPrecisionSet)
	enum { IDD = IDD_DIALOG_OBJ_PRECISIONSET };
	double	m_dbPrecision;
	int		m_nModelNo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjPrecisionSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjPrecisionSet)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJPRECISIONSET_H__EFF1A03D_2D27_4F81_868F_7C7602485BE6__INCLUDED_)
