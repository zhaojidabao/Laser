#pragma once
#include "AdoDb.h"
#include "MarkMgr.h"

// CQueryDBDlg 对话框

class CQueryDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CQueryDBDlg)

public:
	CQueryDBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryDBDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTable;
	CAdoDB m_adoDB;
	DBPARA m_IniPara;
	CString m_strRes;
	CAdoRecordset Recordset;
 	CHLDoc * m_pDoc;
 	CChain * m_pChain;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonReplace();
	void GetDBPara();
	int m_nCount;
};
