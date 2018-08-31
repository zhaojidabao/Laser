#if !defined(AFX_DLGSETINPUT_H__D433AFD9_AC29_4FDE_AFB0_24358F717B57__INCLUDED_)
#define AFX_DLGSETINPUT_H__D433AFD9_AC29_4FDE_AFB0_24358F717B57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetInput.h : header file
//

#include "NodeInput.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetInput dialog

class CDlgSetInput : public CDialog
{
// Construction
public:
	CDlgSetInput(CWnd* pParent = NULL);   // standard constructor
	DWORD m_dwInput;		
	DWORD m_dwInputMask;
	CNodeInput* m_pNode;

	void Apply();
	void SetInfo(CNode* pNode);

// Dialog Data
	//{{AFX_DATA(CDlgSetInput)
	enum { IDD = IDD_SET_NODEINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetInput)
	afx_msg void OnCheck1(){CmdRange(0);};
	afx_msg void OnCheck2(){CmdRange(1);};
	afx_msg void OnCheck3(){CmdRange(2);};
	afx_msg void OnCheck4(){CmdRange(3);};
	afx_msg void OnCheck5(){CmdRange(4);};
	afx_msg void OnCheck6(){CmdRange(5);};
	afx_msg void OnCheck7(){CmdRange(6);};
	afx_msg void OnCheck8(){CmdRange(7);};
	afx_msg void OnCheck9(){CmdRange(8);};
	afx_msg void OnCheck10(){CmdRange(9);};
	afx_msg void OnCheck11(){CmdRange(10);};
	afx_msg void OnCheck12(){CmdRange(11);};
	afx_msg void OnCheck13(){CmdRange(12);};
	afx_msg void OnCheck14(){CmdRange(13);};
	afx_msg void OnCheck15(){CmdRange(14);};
	afx_msg void OnCheck16(){CmdRange(15);};
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT FetchCrlID(UINT nIndex);
	void CmdRange(UINT nIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETINPUT_H__D433AFD9_AC29_4FDE_AFB0_24358F717B57__INCLUDED_)
