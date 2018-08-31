#if !defined(AFX_MSGWND_H__1DC7047D_8675_4C80_B335_54F78F3BBD76__INCLUDED_)
#define AFX_MSGWND_H__1DC7047D_8675_4C80_B335_54F78F3BBD76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgWnd window

class CMsgWnd : public CWnd
{
// Construction
public:
	CMsgWnd();

// Attributes
protected:
	BOOL m_bShow;
	CRITICAL_SECTION m_cri;
	int m_nFlashTime;	//ˢ��ʱ��
	int m_nOpenTime;	//����ʱ��
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgWnd)
	//}}AFX_VIRTUAL

// Implementation
public:	
	void SetViewTimer(int nms);
	void Close();
	CString m_strMessage;
	CString m_strCaption;
	virtual ~CMsgWnd();
    void CreateMsgWindow();
	void SetPromptMessage(CString strMsg);
	void SetPromptCaption(CString strCaption);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMsgWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawTime(CDC* pDC,CRect& rt);
	void Lock();
	void UnLock();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGWND_H__1DC7047D_8675_4C80_B335_54F78F3BBD76__INCLUDED_)
