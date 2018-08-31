#pragma once


// CSock 对话框

class CSock : public CDialog
{
	DECLARE_DYNAMIC(CSock)

public:
	CSock(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSock();

// 对话框数据
	enum { IDD = IDD_DLG_SOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bServer;
	CString m_strIP;
	UINT m_nAddr;
	BOOL m_bSameIP;
	BOOL m_bUsed;
};
