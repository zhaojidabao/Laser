#if !defined(AFX_DLGOBJINFOCIRCLE_H__AE1BD90B_9337_4D4B_9923_B213B485EEFF__INCLUDED_)
#define AFX_DLGOBJINFOCIRCLE_H__AE1BD90B_9337_4D4B_9923_B213B485EEFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjInfoCircle.h : header file
//

#include "NodeCircle.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoCircle dialog

class CDlgObjInfoCircle : public CDialog
{
// Construction
public:
	CDlgObjInfoCircle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjInfoCircle)
	enum { IDD = IDD_DIALOG_OBJINFO_CIRCLE };
	double	m_dbCenterX;
	double	m_dbCenterY;
	double	m_dbR;
	//}}AFX_DATA
public:
	CNodeCircle *m_pNodeCircle;

public:
	void Apply();
	void GetObjInfo(CNode *pNode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjInfoCircle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjInfoCircle)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJINFOCIRCLE_H__AE1BD90B_9337_4D4B_9923_B213B485EEFF__INCLUDED_)
