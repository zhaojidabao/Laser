#pragma once
#include "afxwin.h"
#include "Text.h"
#include "Nodecodebar.h"

// CDlgBarPageText 对话框

class CDlgBarPageText : public CDialog
{
	DECLARE_DYNAMIC(CDlgBarPageText)

public:
	CDlgBarPageText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBarPageText();

// 对话框数据
	enum { IDD = IDD_BAR_PAGE_TEXT };
	CListBox		m_listData;
	CTypedPtrList<CObList, CText*> m_list;
	CText*		m_pText;

	//
	CStringArray * m_pSerial;
	int		*m_pCur;

	//跳号文本
	CString	m_strLeft;		//数字前字符串
	CString	m_strRight;		//数字后字符串
	CString	m_strFromFile;	//跳号取自于文件之名字
	LONGLONG m_nJumpStart;	//数字开始
	LONGLONG m_nJumpEnd;	//数字结束
	int		m_nJumpLen;			//跳号部分长度
	int		m_nJumpStep;		//跳号时步长
	int		m_nJumpInterval;	//跳号时间隔
	int		m_nFromFile;		//是否跳号取自于文件
	CString	m_strBinText;	//进制文本内容

	CString	m_strDate;
	CString	m_strFix;
	int			m_nText;
	CNodeCodebar *m_pNodeCodeBar;
	int			m_nCurTextSel;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnButAdd();
	afx_msg void OnButDel();
	afx_msg void OnButModify();
	afx_msg void OnSelchangeListData();
	afx_msg void OnRadioFix();
	afx_msg void OnRadioSerial();
	afx_msg void OnRadioTime();
	afx_msg void OnButtonSerial();
	afx_msg void OnChangeDateEdit();
	afx_msg void OnTimer(UINT nIDEvent);

	void UpdateDataView();
	void SetTextInfo(CNode *pNode);
	void Apply();
	void Update_Data (BOOL bSaveAndValidate);
	void UpdateDlgView();
	void ListFlash();

	CString GetSerialString();
	CString GetFileString(int nCur);
};
