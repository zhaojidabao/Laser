#pragma once


// CDlgInterspaceSet �Ի���

class CDlgInterspaceSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgInterspaceSet)

public:
	CDlgInterspaceSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInterspaceSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_INTERSPACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dbInterSpace;
	afx_msg void OnBnClickedOk();
};
