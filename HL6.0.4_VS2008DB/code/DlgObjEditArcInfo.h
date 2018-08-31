#if !defined(AFX_DLGOBJEDITARCINFO_H__F17F8D04_BABD_4348_A887_3A9E2A329D9A__INCLUDED_)
#define AFX_DLGOBJEDITARCINFO_H__F17F8D04_BABD_4348_A887_3A9E2A329D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjEditArcInfo.h : header file
//

#include "NodeArc3Pt.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgObjEditArcInfo dialog

class CDlgObjEditArcInfo : public CDialog
{
// Construction
public:
	CDlgObjEditArcInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjEditArcInfo)
	enum { IDD = IDD_DIALOG_OBJINFO_ARC };
	double	m_dbArcR;
	double	m_dbCenterX;
	double	m_dbCenterY;
	double	m_dbEndAngle;
	double	m_dbEndX;
	double	m_dbEndY;
	double	m_dbStartAngle;
	double	m_dbStartX;
	double	m_dbStartY;
	//}}AFX_DATA

public:
	CNodeArc3Pt *m_pNodeArc3Pt;

public:
	void Apply();
	void GetArcInfo(CNode *pNode);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjEditArcInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObjEditArcInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJEDITARCINFO_H__F17F8D04_BABD_4348_A887_3A9E2A329D9A__INCLUDED_)
