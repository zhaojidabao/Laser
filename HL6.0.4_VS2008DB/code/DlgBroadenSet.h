#pragma once


// CDlgBroadenSet �Ի���

class CDlgBroadenSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgBroadenSet)

public:
	CDlgBroadenSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBroadenSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_BROADEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dbLineWidth;
	double m_dbDistance;
	afx_msg void OnBnClickedOk();
};
