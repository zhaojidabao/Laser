#pragma once
#include "afxwin.h"


// CSetLangRegionDlg 对话框

class CSetLangRegionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetLangRegionDlg)
	
public:
	CSetLangRegionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetLangRegionDlg();

// 对话框数据
	enum { IDD = IDD_SET_LANGREGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	CComboBox m_combLang;

public:
	afx_msg void OnBnClickedOk();

public:
	char  m_szLang[20];
	int    m_nLangId;
};
