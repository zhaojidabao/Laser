#pragma once

#include "Text.h"
#include "Nodecodebar.h"
#include "afxwin.h"

// CDlgBarEdit 对话框

class CDlgBarEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgBarEdit)

public:
	CDlgBarEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBarEdit();

// 对话框数据
	enum { IDD = IDD_DIALOG_BAREDIT };
	CListBox	m_listData;
	CTypedPtrList<CObList, CText*> m_list;
	CText* m_pText;
	
	CComboBox	m_ctrlType;		//条码类型
	CComboBox	m_comTextType;  //文本类型

	//
	CStringArray * m_pSerial;
	int *m_pCur;

	CString	m_strDate;
	CString	m_strFix;
	int			m_nText;
	
	CNodeCodebar *m_pNodeBar;
	UINT	m_nBarType;
	int		m_nOutline;		//PDF417条码行数
	int     m_nErrLev;		//纠错级别，用于QR,DATAMATRIX
	int		m_nHPoint;		//垂直方向点个数,用于QR,DATAMATRIX
	int		m_nWPoint;		//水平方向点个数,用于DATAMATRIX
	int		m_nCodeType;	
	int     m_nSymbolNo;		//DATAMATRIX 编码
	int     m_nShapeNo;		//DATAMARTIX 形状
	int		m_nVersion;   //版本，HANXIN

	BOOL		m_bAutoSet;	//设置条码长宽属性;//true 自动设置;FALSE 用户设置
	DWORD	m_nSingleLineMarkCount;
	UINT		m_nNarrowLineNum;

	BOOL		m_bModel;
	CString	m_strModelName;
	int			m_nCurSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()

private:
	void Layout();
	void UpdateDlgView();

public:
	afx_msg void OnBnClickedButBarcodeset();
	afx_msg void OnBnClickedButtonTextadd();
	afx_msg void OnBnClickedButtonTextdel();
	afx_msg void OnBnClickedButtonTextedit();
	afx_msg void OnBnClickedButtonAdvance();
	afx_msg void OnLbnSelchangeListData();
	afx_msg void OnBnClickedButtonSerial();
	afx_msg void OnEnChangeDateEdit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckModify();
	afx_msg void OnCbnSelchangeComboBarcodetype();
	afx_msg void OnCbnSelchangeComboTexttype();
	afx_msg void OnDestroy();
	
	void SetTextInfo(CNode *pNode);
	void Apply();
	void ListFlash();
	BOOL Update_Data (BOOL bSaveAndValidate);
	CString GetSerialString();
	void FlashBarDefaultProperty();
	void GetItemMovePos(int res, int obj,int &nX,int &nY);
	void MoveDlgItem(int nItem, int nX,int nY);
	void ResetData();

	BOOL CheckBarText();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
