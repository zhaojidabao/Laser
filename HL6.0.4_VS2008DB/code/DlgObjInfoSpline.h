#if !defined(AFX_DLGOBJINFOSPLINE_H__0164FE79_A617_49CE_951A_79FE1B5AC39A__INCLUDED_)
#define AFX_DLGOBJINFOSPLINE_H__0164FE79_A617_49CE_951A_79FE1B5AC39A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjInfoSpline.h : header file
//

#include "NodeSpline.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoSpline dialog

class CDlgObjInfoSpline : public CDialog
{
// Construction
public:
	CDlgObjInfoSpline(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjInfoSpline)
	enum { IDD = IDD_DIALOG_OBJINFO_SPLINE };
	int		m_nCount;
	int		m_nCurNo;
	double	m_dbCurX;
	double	m_dbCurY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjInfoSpline)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CNodeSpline *m_pNodeSpline;

public:
	void Apply();
	void GetObjInfo(CNode *pNode);

	void CetPtInfo(int nIndex);
	void SetPtInfo(int nIndex);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjInfoSpline)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJINFOSPLINE_H__0164FE79_A617_49CE_951A_79FE1B5AC39A__INCLUDED_)
