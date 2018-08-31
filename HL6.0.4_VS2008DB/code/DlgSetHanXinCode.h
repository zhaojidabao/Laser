#pragma once
#include "afxwin.h"


// CDlgSetHanXinCode 对话框

class CDlgSetHanXinCode : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetHanXinCode)

public:
	CDlgSetHanXinCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetHanXinCode();
	
// 对话框数据
	enum { IDD = IDD_SET_HANXINCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comVersion;
	CComboBox m_comErrorLevel;
	int m_nVersion;
	int m_nErrorLevel;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
