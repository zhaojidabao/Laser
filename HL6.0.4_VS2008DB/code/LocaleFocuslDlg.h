#pragma once

#include "MarkMgr.h"

// CLocaleFocusDlg �Ի���

class CLocaleFocusDlg : public CDialog
{
	DECLARE_DYNAMIC(CLocaleFocusDlg)

public:
	CLocaleFocusDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLocaleFocusDlg();

	CHLDoc *   m_pDoc;
	CMarkMgr* m_pMarkMgr;
	BOOL m_bMarking;

	void UpdateView();

// �Ի�������
	enum { IDD = IDD_DIALOG_TESTFOCUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg LRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStop(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnOutPutMsg(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonMark();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedOk();


	double  m_dbZDerStep;
	int     m_nSelect;
	afx_msg void OnBnClickedCancel();

};
