#if !defined(AFX_MARKWHEELDLG_H__0A73C614_5B27_4B4B_A4C6_D7296E6B1978__INCLUDED_)
#define AFX_MARKWHEELDLG_H__0A73C614_5B27_4B4B_A4C6_D7296E6B1978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkWheelDlg.h : header file
//
#include "MarkMgr.h"
#include "XColorStatic.h"
#include "BtnST.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CMarkWheelDlg dialog

class CMarkWheelDlg : public CDialog
{
// Construction
public:
	CToolTipCtrl m_tooltip;
	CHLDoc* m_pDoc;
	CChain* m_pChain;
	CChain* m_pChainWheel;
	
	void UpdateView();
	CMarkWheelDlg(CWnd* pParent = NULL);   // standard constructor
	int  m_nTotal;		//�������(�������ѭ������)��
	void UpdateTime(BOOL flag = FALSE);
	void Mark() ;
	
// Dialog Data
	//{{AFX_DATA(CMarkWheelDlg)
	enum { IDD = IDD_MARK_WHEEL };
	CListBox	m_InfoList;
	CXColorStatic	m_MarkInfo;
	CProgressCtrl	m_progressMark;
	BOOL	m_bUpdate;
	int		m_nMarkType;
	BOOL	m_bLoopMark;
	int		m_nIntervalTime;
	int		m_nProcessType;
	BOOL	m_bHome;
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkWheelDlg)
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
	
	int   m_nPrevType;
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
	CButtonST	m_btnView;
	CButtonST	m_chkRedEnable;
	
	//
	BOOL m_bCreateWheelStroke;   //�Ƿ��������ת�������ת��

	// Generated message map functions
	//{{AFX_MSG(CMarkWheelDlg)
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
	afx_msg void OnCheckHome();
	afx_msg void OnRadioProcess();
	afx_msg void OnRadio2();
	afx_msg void OnButtonView();
	afx_msg void OnRadio7();
	afx_msg void OnButtonMotorhome();
	//}}AFX_MSG
	afx_msg LRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStop(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOutPutMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void OnMarkEndL(WPARAM wParam,LPARAM lParam);
	void ShowInfoItem();
	BOOL CreateWheelStroke();

public:
	BOOL m_bShowInfoItem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKWHEELDLG_H__0A73C614_5B27_4B4B_A4C6_D7296E6B1978__INCLUDED_)
