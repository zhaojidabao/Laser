#pragma once


// CDlgBroadenSet 对话框

class CDlgBroadenSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgBroadenSet)

public:
	CDlgBroadenSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBroadenSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_BROADEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbLineWidth;
	double m_dbDistance;
	afx_msg void OnBnClickedOk();
};
