#pragma once
#include "afxwin.h"


// CDlgChangeCard �Ի���

class CDlgChangeCard : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeCard)

public:
	CDlgChangeCard(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgChangeCard();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHANGECARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboCard;

public:
	int  m_nCardIndex;
	afx_msg void OnCbnSelchangeComboCardinfo();
	afx_msg void OnBnClickedOk();
};
