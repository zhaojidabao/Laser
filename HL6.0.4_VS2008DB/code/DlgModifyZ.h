#pragma once


// CDlgModifyZ �Ի���

class CDlgModifyZ : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyZ)

public:
	CDlgModifyZ(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModifyZ();

// �Ի�������
	enum { IDD = IDD_DIALOG_MODIFYZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bEditXY;
	double m_dbX;
	double m_dbY;
	double m_dbZ;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
