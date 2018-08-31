#pragma once
#include "afxwin.h"


// CDlgChangeCard 对话框

class CDlgChangeCard : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeCard)

public:
	CDlgChangeCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChangeCard();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHANGECARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboCard;

public:
	int  m_nCardIndex;
	afx_msg void OnCbnSelchangeComboCardinfo();
	afx_msg void OnBnClickedOk();
};
