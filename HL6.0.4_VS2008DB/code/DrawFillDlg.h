#if !defined(AFX_DRAWFILLDLG_H__6C15CA01_4D40_11D4_B789_C4B0DE1A197E__INCLUDED_)
#define AFX_DRAWFILLDLG_H__6C15CA01_4D40_11D4_B789_C4B0DE1A197E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawFillDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawFillDlg dialog

class CDrawFillDlg : public CDialog
{
// Construction
public:
	int  m_nFillStyle;
	NEW_FILL_DATA m_fillData1;
	NEW_FILL_DATA m_fillData2;
	
public:
	void SetFillData( int nNo, BOOL bSave = FALSE);
	void UpdateDataView();
	CDrawFillDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDrawFillDlg)
	enum { IDD = IDD_DRAW_FILL };
	int		m_nFillNo;                 //������
	BOOL	m_bEquaDist;               //�Ƿ�ȼ���
	BOOL	m_bKeepAngle;              //�������������ת
	double	m_dbSpace;                 //�����
	BOOL	m_bEnable;                 //���ʹ��
	BOOL	m_bLoop;                   //����ʹ��
	double	m_dbAngle;                 //���Ƕ�
	double	m_dbBeamComp;              //��դ��С
	double	m_dbLineReduce;            //��������
	int		m_nLoopNum;                //����Ȧ��
	double	m_dbStartOffset;           //��ʼƫ��
	double	m_dbEndOffset;             //ĩβƫ��
	double	m_dbMinJump;               //��С��ת���
	BOOL	m_bRemoveOrg;              //����ʱɾ��ԭʼ����
	double	m_dbLoopDist;              //�������
	BOOL	m_bRemoveFill;
	BOOL	m_bFillFirst;
	//}}AFX_DATA

	int m_nFillNum;
	double m_dbAddDis;
	double m_dbAddAngle;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawFillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int  m_nPrevFillNo;	     //���������
	// Generated message map functions
	//{{AFX_MSG(CDrawFillDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStyle();
	afx_msg void OnCheckEnable();
	afx_msg void OnCheckLoop();
	afx_msg void OnRadioFill1();
	afx_msg void OnRadioFill2();
	afx_msg void OnCheckFillfirst();
	afx_msg void OnButtonAddfillset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL m_bFillAlign;
	double m_dbLineReduceEnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DrawFillDlg_H__6C15CA01_4D40_11D4_B789_C4B0DE1A197E__INCLUDED_)
