#if !defined(AFX_TEXTPAGEDLG_H__40446575_59EA_4E25_AC94_56A6D279233C__INCLUDED_)
#define AFX_TEXTPAGEDLG_H__40446575_59EA_4E25_AC94_56A6D279233C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextPageDlg.h : header file
//
#include "TextPageTextDlg.h"
#include "TextPageRoundDlg.h"
#include "TextPageFntDlg.h"
#include "Nodetext.h"

/////////////////////////////////////////////////////////////////////////////
// CTextPageDlg dialog

class CTextPageDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTextPageDlg)

// Construction
public:
	void ListFlash();
	void Update_Data (BOOL bSaveAndValidate = TRUE);
	CTextPageDlg();
	~CTextPageDlg();

// Dialog Data
	//{{AFX_DATA(CTextPageDlg)
	enum { IDD = IDD_TEXT_PAGE };
	CListBox	m_listData;
	CTabCtrl	m_tab;
	//}}AFX_DATA

	CTypedPtrList<CObList, CText*> m_list;
	CTextPageTextDlg   m_PageText;
	CTextPageRoundDlg m_PageRound;
	CTextPageFntDlg    m_PageFnt;
	
	void Delete();
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTextPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTextPageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButAdd();
	afx_msg void OnButDel();
	afx_msg void OnButModify();
	afx_msg void OnSelchangeListData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPAGEDLG_H__40446575_59EA_4E25_AC94_56A6D279233C__INCLUDED_)
