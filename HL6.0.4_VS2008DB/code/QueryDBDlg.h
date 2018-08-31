#pragma once
#include "AdoDb.h"
#include "MarkMgr.h"

// CQueryDBDlg �Ի���

class CQueryDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CQueryDBDlg)

public:
	CQueryDBDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryDBDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
