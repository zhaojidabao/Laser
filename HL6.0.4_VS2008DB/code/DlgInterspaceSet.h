#pragma once


// CDlgInterspaceSet 对话框

class CDlgInterspaceSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgInterspaceSet)

public:
	CDlgInterspaceSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInterspaceSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_INTERSPACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbInterSpace;
	afx_msg void OnBnClickedOk();
};
