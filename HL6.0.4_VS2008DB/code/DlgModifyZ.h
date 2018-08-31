#pragma once


// CDlgModifyZ 对话框

class CDlgModifyZ : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyZ)

public:
	CDlgModifyZ(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModifyZ();

// 对话框数据
	enum { IDD = IDD_DIALOG_MODIFYZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bEditXY;
	double m_dbX;
	double m_dbY;
	double m_dbZ;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
