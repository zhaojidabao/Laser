#pragma once


// CSock �Ի���

class CSock : public CDialog
{
	DECLARE_DYNAMIC(CSock)

public:
	CSock(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSock();

// �Ի�������
	enum { IDD = IDD_DLG_SOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bServer;
	CString m_strIP;
	UINT m_nAddr;
	BOOL m_bSameIP;
	BOOL m_bUsed;
};
