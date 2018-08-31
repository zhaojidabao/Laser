#pragma once
#include "resource.h"
#include "AdoDb.h"
// CDataSourceDlg 对话框

class CDataSourceDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataSourceDlg)

public:
	CDataSourceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataSourceDlg();

// 对话框数据
	enum { IDD = IDD_SET_DATASOURCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString	m_strPath;
	CString m_strDsn;
	CString m_strMdb;
	CString m_strUserName;
	CString m_strPwd;
	CString m_strServer;
	int m_nType;
	virtual BOOL OnInitDialog();
	void UpdateFrame(void);
	afx_msg void OnCbnSelchangeComboData();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnCbnEditchangeComboData();
	afx_msg void OnBnClickedButtonMdb();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
