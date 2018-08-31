#pragma once
#include "afxwin.h"
#include "Text.h"
#include "Nodecodebar.h"

// CDlgBarPageText �Ի���

class CDlgBarPageText : public CDialog
{
	DECLARE_DYNAMIC(CDlgBarPageText)

public:
	CDlgBarPageText(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBarPageText();

// �Ի�������
	enum { IDD = IDD_BAR_PAGE_TEXT };
	CListBox		m_listData;
	CTypedPtrList<CObList, CText*> m_list;
	CText*		m_pText;

	//
	CStringArray * m_pSerial;
	int		*m_pCur;

	//�����ı�
	CString	m_strLeft;		//����ǰ�ַ���
	CString	m_strRight;		//���ֺ��ַ���
	CString	m_strFromFile;	//����ȡ�����ļ�֮����
	LONGLONG m_nJumpStart;	//���ֿ�ʼ
	LONGLONG m_nJumpEnd;	//���ֽ���
	int		m_nJumpLen;			//���Ų��ֳ���
	int		m_nJumpStep;		//����ʱ����
	int		m_nJumpInterval;	//����ʱ���
	int		m_nFromFile;		//�Ƿ�����ȡ�����ļ�
	CString	m_strBinText;	//�����ı�����

	CString	m_strDate;
	CString	m_strFix;
	int			m_nText;
	CNodeCodebar *m_pNodeCodeBar;
	int			m_nCurTextSel;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
