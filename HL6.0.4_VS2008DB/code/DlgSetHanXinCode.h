#pragma once
#include "afxwin.h"


// CDlgSetHanXinCode �Ի���

class CDlgSetHanXinCode : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetHanXinCode)

public:
	CDlgSetHanXinCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetHanXinCode();
	
// �Ի�������
	enum { IDD = IDD_SET_HANXINCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comVersion;
	CComboBox m_comErrorLevel;
	int m_nVersion;
	int m_nErrorLevel;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
