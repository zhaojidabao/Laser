#pragma once
#include "afxwin.h"


// CCom �Ի���

class CCom : public CDialog
{
	DECLARE_DYNAMIC(CCom)

public:
	CCom(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCom();

// �Ի�������
	enum { IDD = IDD_DLG_COM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bUsed;
	CComboBox m_ccom;
	CComboBox m_cBandRate;
	CComboBox m_cDataBit;
	CComboBox m_cCheckBit;
	CComboBox m_cStopBit;
};
