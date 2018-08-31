#if !defined(AFX_MARKNORMALDLG_H__B8087380_FE40_11D3_A07E_00105A615525__INCLUDED_)
#define AFX_MARKNORMALDLG_H__B8087380_FE40_11D3_A07E_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkNormalDlg.h : header file
//
#include "MarkMgr.h"
#include "BtnST.h"
#include "XColorStatic.h"
#include "afxwin.h"
#include "Include/SockDllDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CMarkNormalDlg dialog

class CMarkNormalDlg : public CDialog
{
// Construction
public:
	CToolTipCtrl m_tooltip;
	CHLDoc * m_pDoc;
	CChain * m_pChain;
	CChain3D* m_pChain3d;
	void UpdateView();
	CMarkNormalDlg(CWnd* pParent = NULL);   // standard constructor
	int  m_nTotal;			//�������(�������ѭ������)��
	void UpdateTime(BOOL flag = FALSE);
	void Mark() ;
	
// Dialog Data
	//{{AFX_DATA(CMarkNormalDlg)
	enum { IDD = IDD_MARK_NORMAL };
	CListBox	m_InfoList;
	CXColorStatic	m_MarkInfo;
	CProgressCtrl	m_progressMark;
	BOOL	m_bUpdate;
	int		m_nMarkType;
	BOOL	m_bLoopMark;
	int		m_nIntervalTime;
	BOOL	m_bRedMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkNormalDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void MarkSEnd(int nSts);
	void MarkEnd(int nSts);
	int   m_nCount;			//��������
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
		
	// Generated message map functions
	//{{AFX_MSG(CMarkNormalDlg)
	afx_msg void OnButtonMark();
	afx_msg void OnButtonStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	afx_msg void OnCheckUpdate();
	afx_msg void OnRadioLaser();
	afx_msg void OnRadioRed();
	afx_msg void OnRadioFrame();
	afx_msg void OnCheckLoopmark();
	afx_msg void OnCheckRed();
	afx_msg void OnButtonMotorhome();
	//}}AFX_MSG
	afx_msg LRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStop(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOutPutMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnWmSockets(WPARAM wpara, LPARAM lpara);
	DECLARE_MESSAGE_MAP()

public:
	void OnMarkEndL(WPARAM wParam,LPARAM lParam);
	void ShowInfoItem();

public:
	BOOL m_bShowInfoItem;
	afx_msg void OnBnClickedCancel();

public:
	void OnNetRec();
	void SendSocketMsg(CString strMsg);
	void StartNet();
public:
	IMFCSockDll  m_sock;
	int	m_nListenID;
	int	m_nMainID;
	CString m_strIP;
	int m_nAddr;
	bool m_bServer;
	bool m_bSameIP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKNORMALDLG_H__B8087380_FE40_11D3_A07E_00105A615525__INCLUDED_)
