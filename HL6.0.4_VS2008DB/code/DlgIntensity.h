#if !defined(AFX_DLGINTENSITY_H__C6A3D234_0F13_4DAF_BDCE_1F18EE642943__INCLUDED_)
#define AFX_DLGINTENSITY_H__C6A3D234_0F13_4DAF_BDCE_1F18EE642943__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIntensity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity dialog


class CDlgIntensity : public CDialog
{
// Construction
public:
	int		m_iIsDraging;
	CRect	m_MouseRect;
	LONG	m_lHeight;
	LONG	m_lWidth;
	BYTE *	m_lpDIBBits;
	
	//
	LONG	m_lCount[256];
	CDlgIntensity(CWnd* pParent = NULL);
	
	//
	CStringArray  m_arrayStringGray;
	CStringArray  m_arrayStringCount;
	int m_nListType;
	void InitGrayList();
	void UpdateGrayList();


// Dialog Data
	//{{AFX_DATA(CDlgIntensity)
	enum { IDD = IDD_DIALOG_INTENSITY };
	CListCtrl	m_listCtrGray;
	int		m_iLowGray;
	int		m_iUpGray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIntensity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIntensity)
	afx_msg void OnPaint();
	afx_msg void OnKillfocusEDITLowGray();
	afx_msg void OnKillfocusEDITUpGray();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnColumnclickListGraylist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINTENSITY_H__C6A3D234_0F13_4DAF_BDCE_1F18EE642943__INCLUDED_)
