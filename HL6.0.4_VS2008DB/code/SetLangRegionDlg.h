#pragma once
#include "afxwin.h"


// CSetLangRegionDlg �Ի���

class CSetLangRegionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetLangRegionDlg)
	
public:
	CSetLangRegionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetLangRegionDlg();

// �Ի�������
	enum { IDD = IDD_SET_LANGREGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	CComboBox m_combLang;

public:
	afx_msg void OnBnClickedOk();

public:
	char  m_szLang[20];
	int    m_nLangId;
};
