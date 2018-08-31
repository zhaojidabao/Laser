#if !defined(AFX_MARKWHEELDLG_H__71C90C5C_7451_40f4_AFED_D04B557E9567__INCLUDED_)
#define AFX_MARKWHEELDLG_H__71C90C5C_7451_40f4_AFED_D04B557E9567__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkWheelDlg2.h : header file
//
#include "MarkMgr.h"
#include "XColorStatic.h"
#include "BtnST.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CMarkWheelDlg2 dialog

class CMarkWheelDlg2 : public CDialog
{
	// Construction
public:
	CToolTipCtrl m_tooltip;
	CHLDoc* m_pDoc;
	CChain* m_pChain;

	void UpdateView();
	CMarkWheelDlg2(CWnd* pParent = NULL);   // standard constructor
	int  m_nTotal;		//�������(�������ѭ������)��
	void UpdateTime(BOOL flag = FALSE);
	void Mark() ;

	// Dialog Data
	//{{AFX_DATA(CMarkWheelDlg)
	enum { IDD = IDD_MARK_WHEEL2 };
	CListBox	m_InfoList;
	CXColorStatic	m_MarkInfo;
	CProgressCtrl	m_progressMark;
	BOOL	m_bUpdate;
	int		m_nMarkType;
	BOOL	m_bLoopMark;
	int		m_nIntervalTime;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkWheelDlg2)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
public:
	void MarkSEnd();
	void MarkEnd();
	int m_nCount;			//��������
	CMarkMgr* m_pMarkMgr;	//��������

protected:
	BOOL m_bMark;       //TRUE���ڱ�ʾ��ǰ�Ƿ��ڴ��׼���ã�����δ���д���״̬�����紦�ڴ����ͣ��ֹͣ���״̬
	BOOL m_bMarkStop;   //TRUE���ڱ�ʾ��ǰ��ֹͣ��ť���ڼ���״̬
	BOOL m_bMarkExit;   //��ʾ�Ƿ���ĳ�εĴ������У�TRUE��ʾ�����ڣ�FALSE-��ʾ����

	BOOL  m_bFirst;
	CTime m_timeStart;
	CTime m_timeEnd;
	CTimeSpan m_tsTotal;	//��ʱ��
	CTimeSpan m_tsWork;		//����ʱ��
	CTimeSpan m_tsIdle;		//����ʱ��

	DWORD m_dwSingle;		//�������ʱ��
	DWORD m_dwWork;			//��깤��ʱ��

	DWORD m_dwStart;		//�������ʱ�俪ʼ
	DWORD m_dwEnd;			//�������ʱ�����

	//
	CButtonST	m_btnStartMark;
	CButtonST	m_btnStopMark;
	CButtonST	m_btnExit;
	CButtonST	m_chkRedEnable;

	//
	// Generated message map functions
	//{{AFX_MSG(CMarkWheelDlg2)
	afx_msg void OnButtonMark();
	afx_msg void OnButtonStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	afx_msg void OnCheckUpdate();
	afx_msg void OnRadioLaser();
	afx_msg void OnRadioRed();
	afx_msg void OnCheckLoopmark();
	afx_msg void OnCheckRed();
	afx_msg void OnButtonView();
	//}}AFX_MSG
	afx_msg LRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStop(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOutPutMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void OnMarkEndL(WPARAM wParam,LPARAM lParam);
	void ShowInfoItem();

public:
	BOOL m_bShowInfoItem;
	BOOL m_bHome;
	afx_msg void OnBnClickedCheckHome();
	afx_msg void OnBnClickedButtonMotorhome();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKWHEELDLG_H__71C90C5C_7451_40f4_AFED_D04B557E9567__INCLUDED_)
