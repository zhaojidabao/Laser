#pragma once


// CDlgRunWarn �Ի���

class CDlgRunWarn : public CDialog
{
	DECLARE_DYNAMIC(CDlgRunWarn)

public:
	CDlgRunWarn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRunWarn();

// �Ի�������
	enum { IDD = IDD_DIALOG_NOTADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
};
