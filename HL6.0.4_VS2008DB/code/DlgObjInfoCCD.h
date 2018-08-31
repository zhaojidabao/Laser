#pragma once
#include "NodeCCD.h"
#include "afxwin.h"

// CDlgObjInfoCCD 对话框

class CDlgObjInfoCCD : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjInfoCCD)

public:
	CDlgObjInfoCCD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgObjInfoCCD();

// 对话框数据
	enum { IDD = IDD_DIALOG_OBJINFO_CCD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckSetPos();

public:
	CNodeCCD *m_pNode;
	void Apply();
	void SetInfo(CNode* pNode);
	void UpdateDlgView();
	void InitData();

	UINT m_nModelNo;
	UINT m_nRetryTimes;
	UINT m_nOutDelay;
	BOOL m_bSetPos;
	double m_dbX;
	double m_dbY;
	int m_nExceptPro;
	int m_nBaudRate;	
	UINT m_nJumpDelay;	
	CComboBox m_comExceptPro;
	CComboBox m_comBaudRate;
};
