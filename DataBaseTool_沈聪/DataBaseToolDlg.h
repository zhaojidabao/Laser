
// DataBaseToolDlg.h : 头文件
//

#pragma once
#include "CheckDBCode.h"
#include "afxcmn.h"
#include "afxwin.h"

UINT FunThread(LPVOID plg);
// CDataBaseToolDlg 对话框
class CDataBaseToolDlg : public CDialog
{
public:
	void StartFun();
	CRITICAL_SECTION csWriteLog;
public:
	CString m_strPath;	// ini文件路径
	CCheckDBCode m_CheckCode;	//连接数据库
	CCheckDBCode* m_pCheckCode;
	DBPARA	m_dbPara;
	void			SetShowDate(CStringArray& arrayName);		// 表数据初始化
	int			m_nItem;				// 总长度
	CListCtrl	m_listData;
	CStringArray m_ListHandeName;
// 构造
public:
	CDataBaseToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DATABASETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSet();
	CString m_strExtName;
	afx_msg void OnEnKillfocusEditName();
	afx_msg void OnBnClickedButtonInquiry();
	afx_msg void OnBnClickedButtonRun();
public:
	void	GetExcelName();
	CComboBox m_ComName;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
