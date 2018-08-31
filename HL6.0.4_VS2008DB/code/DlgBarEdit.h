#pragma once

#include "Text.h"
#include "Nodecodebar.h"
#include "afxwin.h"

// CDlgBarEdit �Ի���

class CDlgBarEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgBarEdit)

public:
	CDlgBarEdit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBarEdit();

// �Ի�������
	enum { IDD = IDD_DIALOG_BAREDIT };
	CListBox	m_listData;
	CTypedPtrList<CObList, CText*> m_list;
	CText* m_pText;
	
	CComboBox	m_ctrlType;		//��������
	CComboBox	m_comTextType;  //�ı�����

	//
	CStringArray * m_pSerial;
	int *m_pCur;

	CString	m_strDate;
	CString	m_strFix;
	int			m_nText;
	
	CNodeCodebar *m_pNodeBar;
	UINT	m_nBarType;
	int		m_nOutline;		//PDF417��������
	int     m_nErrLev;		//����������QR,DATAMATRIX
	int		m_nHPoint;		//��ֱ��������,����QR,DATAMATRIX
	int		m_nWPoint;		//ˮƽ��������,����DATAMATRIX
	int		m_nCodeType;	
	int     m_nSymbolNo;		//DATAMATRIX ����
	int     m_nShapeNo;		//DATAMARTIX ��״
	int		m_nVersion;   //�汾��HANXIN

	BOOL		m_bAutoSet;	//�������볤������;//true �Զ�����;FALSE �û�����
	DWORD	m_nSingleLineMarkCount;
	UINT		m_nNarrowLineNum;

	BOOL		m_bModel;
	CString	m_strModelName;
	int			m_nCurSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
