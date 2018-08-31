#if !defined(AFX_DLGSETOUTPUT_H__093C328F_C724_4F27_8EAD_14C5DD9590FA__INCLUDED_)
#define AFX_DLGSETOUTPUT_H__093C328F_C724_4F27_8EAD_14C5DD9590FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetOutput.h : header file
//

#include "NodeOutput.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetOutput dialog

class CDlgSetOutput : public CDialog
{
// Construction
public:
	CDlgSetOutput(CWnd* pParent = NULL);   // standard constructor
	DWORD m_dwOutput;
	DWORD m_dwOutputMask;
	CNodeOutput* m_pNode;
	
	void Apply();
	void SetInfo(CNode* pNode);
// Dialog Data
	//{{AFX_DATA(CDlgSetOutput)
	enum { IDD = IDD_SET_NODEOUTPUT };
	DWORD	m_dwOutputDelay;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetOutput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetOutput)
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
	afx_msg void OnCheck17(){CmdRange(16);};
	afx_msg void OnCheck18(){CmdRange(17);};
	afx_msg void OnCheck19(){CmdRange(18);};
	afx_msg void OnCheck20(){CmdRange(19);};
	afx_msg void OnCheck21(){CmdRange(20);};
	afx_msg void OnCheck22(){CmdRange(21);};
	afx_msg void OnCheck23(){CmdRange(22);};
	afx_msg void OnCheck24(){CmdRange(23);};
	afx_msg void OnCheck25(){CmdRange(24);};
	afx_msg void OnCheck26(){CmdRange(25);};
	afx_msg void OnCheck27(){CmdRange(26);};
	afx_msg void OnCheck28(){CmdRange(27);};
	afx_msg void OnCheck29(){CmdRange(28);};
	afx_msg void OnCheck30(){CmdRange(29);};
	afx_msg void OnCheck31(){CmdRange(30);};
	afx_msg void OnCheck32(){CmdRange(31);};
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT FetchCrlID(UINT nIndex);
	void CmdRange(UINT nIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETOUTPUT_H__093C328F_C724_4F27_8EAD_14C5DD9590FA__INCLUDED_)
