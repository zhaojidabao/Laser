
// DataBaseToolDlg.h : ͷ�ļ�
//

#pragma once
#include "CheckDBCode.h"
#include "afxcmn.h"
#include "afxwin.h"

UINT FunThread(LPVOID plg);
// CDataBaseToolDlg �Ի���
class CDataBaseToolDlg : public CDialog
{
public:
	void StartFun();
	CRITICAL_SECTION csWriteLog;
public:
	CString m_strPath;	// ini�ļ�·��
	CCheckDBCode m_CheckCode;	//�������ݿ�
	CCheckDBCode* m_pCheckCode;
	DBPARA	m_dbPara;
	void			SetShowDate(CStringArray& arrayName);		// �����ݳ�ʼ��
	int			m_nItem;				// �ܳ���
	CListCtrl	m_listData;
	CStringArray m_ListHandeName;
// ����
public:
	CDataBaseToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DATABASETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
