#if !defined(AFX_MARKSPECIALDLG_H__AB089EA1_1A12_11D5_97CC_00E04C3AA16B__INCLUDED_)
#define AFX_MARKSPECIALDLG_H__AB089EA1_1A12_11D5_97CC_00E04C3AA16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkSpecialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarkSpecialDlg dialog

class CMarkSpecialDlg : public CDialog
{
// Construction
public:
	void MarkSEnd();
	void MarkEnd();
	void Mark() ;
	void UpdateCount();
	void UpdateTime();
	BOOL GetNext(); //�Ƿ�ɹ�ȡ���´δ���ĵ�
	void UpdateView();
	CMarkSpecialDlg(CWnd* pParent = NULL);   // standard constructor
	CObArray m_arrayDoc;
	CHLDoc * m_pDoc;
	
	//�������ʱ�䣻
	DWORD m_dwSingle;
	//���ĵ����ʱ�䣻
	DWORD m_dwTotalStart;
	DWORD m_dwTotalEnd;
	DWORD m_dwTotal;
	//��������
	int m_nCount;
	BOOL m_bPrevFrame;

	CMarkMgr* m_pMarkMgr;	//��������
// Dialog Data
	//{{AFX_DATA(CMarkSpecialDlg)
	enum { IDD = IDD_MARK_SPECIAL };
	CProgressCtrl	m_progressMark;
	CListBox	m_listDocName;
	BOOL	m_bDebug;
	int		m_nSignalType;
	int		m_nMarkType;
	BOOL	m_bSNO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkSpecialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bMark;
	BOOL m_bMarkStop;
	BOOL m_bMarkExit;
	BOOL m_bFirst;
//	CWinThread* m_pTouch;

	// Generated message map functions
	//{{AFX_MSG(CMarkSpecialDlg)
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDel();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonMark();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonGetio();
	afx_msg void OnCheckDebug();
	//}}AFX_MSG
	afx_msg LRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkPause(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOutPutMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKSPECIALDLG_H__AB089EA1_1A12_11D5_97CC_00E04C3AA16B__INCLUDED_)
