#pragma once


// CDlgChangeOther 对话框

class CDlgChangeOther : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeOther)

public:
	CDlgChangeOther(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChangeOther();

// 对话框数据
	enum { IDD = ID_CHANG_DLG_OTHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbShearX;
	double m_dbShearY;
	afx_msg void OnBnClickedCenter();
	afx_msg void OnBnClickedButtonExtendobj();
	afx_msg void OnEnChangeShearx();
	afx_msg void OnEnChangeSheary();

	BOOL m_bShellEdit;
};
