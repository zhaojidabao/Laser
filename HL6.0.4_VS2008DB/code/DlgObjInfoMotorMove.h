#if !defined(AFX_DLGOBJINFOMOTORMOVE_H__A2545B97_1EE1_47E3_980B_0341AA44C26D__INCLUDED_)
#define AFX_DLGOBJINFOMOTORMOVE_H__A2545B97_1EE1_47E3_980B_0341AA44C26D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjInfoMotorMove.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoMotorMove dialog

class CDlgObjInfoMotorMove : public CDialog
{
// Construction
public:
	CDlgObjInfoMotorMove(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDlgObjInfoMotorMove)
	enum { IDD = IDD_DIALOG_OBJINFO_MOTORMOVE };
	int m_nAxisNo;
	double	m_dbLineValue;
	double	m_dbRotateValue;
	int		m_nMoveModel;
	int		m_nWorkType;
	//}}AFX_DATA

public:
	CNodeMotorMove *m_pNode;

public:
	void Apply();
	void SetInfo(CNode* pNode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjInfoMotorMove)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgObjInfoMotorMove)
	afx_msg void OnSelchangeCOMBOAxisNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGOBJINFOMOTORMOVE_H__A2545B97_1EE1_47E3_980B_0341AA44C26D__INCLUDED_)
