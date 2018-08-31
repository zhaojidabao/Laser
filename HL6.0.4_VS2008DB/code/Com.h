#pragma once
#include "afxwin.h"


// CCom 对话框

class CCom : public CDialog
{
	DECLARE_DYNAMIC(CCom)

public:
	CCom(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCom();

// 对话框数据
	enum { IDD = IDD_DLG_COM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bUsed;
	CComboBox m_ccom;
	CComboBox m_cBandRate;
	CComboBox m_cDataBit;
	CComboBox m_cCheckBit;
	CComboBox m_cStopBit;
};
