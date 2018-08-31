#if !defined(AFX_OBJECTEDITFILL_H__DD110CE2_D2D7_4E12_BC67_4327AB325E57__INCLUDED_)
#define AFX_OBJECTEDITFILL_H__DD110CE2_D2D7_4E12_BC67_4327AB325E57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Objecteditfill.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjecteditfill dialog

class CObjecteditfill : public CDialog
{
// Construction
public:
	int  m_nFillStyle;
	NEW_FILL_DATA m_fillData1;
	NEW_FILL_DATA m_fillData2;

public:
	void SetFillData( int nNo, BOOL bSave = FALSE);
	void UpdateDataView();
	void GetNewFillData(FILLDATA &filldata);
	void DisplyFillProp(FILLDATA &filldata);
	CObjecteditfill(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjecteditfill)
	enum { IDD = IDD_DIALOG_OBJEDITFILL };
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
	BOOL	m_bRemoveFill;             //ɾ�������
	BOOL	m_bFillFirst;			   //�ȴ������
	BOOL    m_bAlign;                  //���ʱ����߶���
	//}}AFX_DATA
	int m_nFillNum;
	double m_dbAddDis;
	double m_dbAddAngle; 
	BOOL m_bSaveCountour;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjecteditfill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int  m_nPrevFillNo;	     //���������
	// Generated message map functions
	//{{AFX_MSG(CObjecteditfill)
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
	double m_dbLineReduceEnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTEDITFILL_H__DD110CE2_D2D7_4E12_BC67_4327AB325E57__INCLUDED_)
