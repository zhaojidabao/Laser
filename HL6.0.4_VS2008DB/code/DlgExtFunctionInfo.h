#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgExtFunctionInfo 对话框

class CDlgExtFunctionInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgExtFunctionInfo)
	
public:
	CDlgExtFunctionInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgExtFunctionInfo();
	
// 对话框数据
	enum { IDD = IDD_DIALOG_EXTENDFUNCTION_INFO };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	
	void InitListInfo();
	void GetCardName();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_listctrlInfo;
	CString m_strCardID;
	CString m_strCardName; //别名alias
	BOOL m_bModify;        //修改标记
	afx_msg void OnBnClickedButtonRename();
};
