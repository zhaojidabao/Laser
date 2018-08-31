#pragma once
#include "AdoDb.h"

// CDataSourceDlg �Ի���

class CDataSourceDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataSourceDlg)

public:
	CDataSourceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataSourceDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DBPARA m_Para;
	CString m_strMdb;
	CString m_strTable;
	CString m_strServer;
	CString m_strUserName;
	CString m_strPwd;
	int m_nType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonMdb();
	void SetDBPara();
	void GetDBPara();
protected:
	virtual void OnOK();
};
