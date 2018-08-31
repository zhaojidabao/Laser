#if !defined(AFX_DLGOBJINFOMOTORPOS_H__1F00B4A2_3C66_42AE_A51B_6B0A65012630__INCLUDED_)
#define AFX_DLGOBJINFOMOTORPOS_H__1F00B4A2_3C66_42AE_A51B_6B0A65012630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjInfoMotorPos.h : header file
//
#include "NodeMotorPos.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoMotorPos dialog

class CDlgObjInfoMotorPos : public CDialog
{
// Construction
public:
	CDlgObjInfoMotorPos(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjInfoMotorPos)
	enum { IDD = IDD_DIALOG_OBJINFO_MOTORPOS };
	double	m_dbLineValue0;
	double	m_dbLineValue1;
	double	m_dbLineValue2;
	double	m_dbLineValue3;
	double	m_dbRotateValue0;
	double	m_dbRotateValue1;
	double	m_dbRotateValue2;
	double	m_dbRotateValue3;
	//}}AFX_DATA
 
public:
	CNodeMotorPos *m_pNode;

public:
	void Apply();
	void SetInfo(CNode* pNode);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjInfoMotorPos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjInfoMotorPos)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJINFOMOTORPOS_H__1F00B4A2_3C66_42AE_A51B_6B0A65012630__INCLUDED_)
