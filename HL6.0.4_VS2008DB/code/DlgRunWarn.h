#pragma once


// CDlgRunWarn 对话框

class CDlgRunWarn : public CDialog
{
	DECLARE_DYNAMIC(CDlgRunWarn)

public:
	CDlgRunWarn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRunWarn();

// 对话框数据
	enum { IDD = IDD_DIALOG_NOTADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
};
