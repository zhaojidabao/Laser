#pragma once


// CDlgChangeOther �Ի���

class CDlgChangeOther : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeOther)

public:
	CDlgChangeOther(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgChangeOther();

// �Ի�������
	enum { IDD = ID_CHANG_DLG_OTHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dbShearX;
	double m_dbShearY;
	afx_msg void OnBnClickedCenter();
	afx_msg void OnBnClickedButtonExtendobj();
	afx_msg void OnEnChangeShearx();
	afx_msg void OnEnChangeSheary();

	BOOL m_bShellEdit;
};
