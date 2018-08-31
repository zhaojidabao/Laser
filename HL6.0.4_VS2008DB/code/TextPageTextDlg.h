#if !defined(AFX_TEXTPAGETEXTDLG_H__A0E1C923_93D6_4327_94D1_09049744767F__INCLUDED_)
#define AFX_TEXTPAGETEXTDLG_H__A0E1C923_93D6_4327_94D1_09049744767F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TextPageTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextPageTextDlg dialog
class CTextPageTextDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTextPageTextDlg)

	// Construction
public:
	CTextPageTextDlg();
	~CTextPageTextDlg();
	void UpdateDataView();
	
	CString GetSerialString();
	CString GetFileString(int nCur);
	CStringArray * m_pSerial;
	int *m_pCur;

	//�����ı�
	BOOL m_bFill;			//�Ƿ���
	CString m_strPrefix;	//����ǰ���ַ�
	CString m_strLeft;		//����ǰ�ַ���
	CString m_strRight;		//���ֺ��ַ���
	CString m_strFromFile;	//����ȡ�����ļ�֮����
	LONGLONG m_nJumpStart;	//���ֿ�ʼ
	LONGLONG m_nJumpEnd;	//���ֽ���
	int m_nJumpLen;			//���Ų��ֳ���
	int m_nJumpStep;		//����ʱ����
	BOOL m_bJumpLoop;		//ѭ������
	int m_nJumpInterval;	//����ʱ���
	int m_nFromFile;		//�Ƿ�����ȡ�����ļ�
	CString m_strBinText;	//�����ı�����

	// Dialog Data
	//{{AFX_DATA(CTextPageTextDlg)
	enum { IDD = IDD_TEXT_PAGE_TEXT };
	CString	m_strDate;
	CString	m_strFix;
	int		m_nText;
	//}}AFX_DATA

	CString m_strLocal;
	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTextPageRoundDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTextPageTextDlg)
	afx_msg void OnRadioFix();
	afx_msg void OnRadioSerial();
	afx_msg void OnRadioTime();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSerial();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeDateEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPAGETEXTDLG_H__A0E1C923_93D6_4327_94D1_09049744767F__INCLUDED_)
