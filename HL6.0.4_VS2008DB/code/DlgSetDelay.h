#if !defined(AFX_DLGSETDELAY_H__6067F24D_1248_465C_9887_FF40E022A1A6__INCLUDED_)
#define AFX_DLGSETDELAY_H__6067F24D_1248_465C_9887_FF40E022A1A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetDelay.h : header file
//
#include "NodeDelay.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetDelay dialog

class CDlgSetDelay : public CDialog
{
// Construction
public:
	CDlgSetDelay(CWnd* pParent = NULL);   // standard constructor

	void Apply();
	void SetInfo(CNode* pNode);

	CNodeDelay* m_pNode;

// Dialog Data
	//{{AFX_DATA(CDlgSetDelay)
	enum { IDD = IDD_SET_NODEDELAY };
	DWORD	m_dwDelay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetDelay)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetDelay)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETDELAY_H__6067F24D_1248_465C_9887_FF40E022A1A6__INCLUDED_)
