#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgExtFunctionInfo �Ի���

class CDlgExtFunctionInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgExtFunctionInfo)
	
public:
	CDlgExtFunctionInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgExtFunctionInfo();
	
// �Ի�������
	enum { IDD = IDD_DIALOG_EXTENDFUNCTION_INFO };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	
	void InitListInfo();
	void GetCardName();
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_listctrlInfo;
	CString m_strCardID;
	CString m_strCardName; //����alias
	BOOL m_bModify;        //�޸ı��
	afx_msg void OnBnClickedButtonRename();
};
