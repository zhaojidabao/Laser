#if !defined(AFX_DLGMOTIONINFOSET_H__1906D236_E177_4A8D_9F95_7E23C29817DF__INCLUDED_)
#define AFX_DLGMOTIONINFOSET_H__1906D236_E177_4A8D_9F95_7E23C29817DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMotionInfoSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMotionInfoSet dialog

class CDlgMotionInfoSet : public CDialog
{
// Construction
public:
	CDlgMotionInfoSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMotionInfoSet)
	enum { IDD = IDD_DIALOG_MOTIONINFO_SET };
	CComboBox	m_comAxisType;
	CComboBox	m_comAxisNo;
	int		m_nDelay;
	double	m_dbEmptyTAcc;
	double	m_dbHomeHighSpeed;
	double	m_dbHomeLowSpeed;
	double	m_dbHomeTAcc;
	double	m_dbLineEmptySpeed;
	double	m_dbLineWorkSpeed;
	double	m_dbPitch;
	int		m_nPPR;
	double	m_dbREmptySpeed;
	double	m_dbRWorkSpeed;
	double	m_dbWorkTAcc;
	int		m_nMotorDire;
	int		m_nAxisNo;
	int		m_nAxisType;
	double	m_dbMoveValue;
	int		m_nMoveTestModel;
	int		m_nMoveTestType;
	BOOL	m_bAutoSoft0;
	BOOL	m_bEnabled;
	//}}AFX_DATA

public:
	void  UpdateView();
	int			m_nOldAxisNo;
	BOOL		m_bHomed[4];
	MOTIONPARA	m_para[4];
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMotionInfoSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateDlgView(BOOL bSave);
	void SaveMotionPara();

	// Generated message map functions
	//{{AFX_MSG(CDlgMotionInfoSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCOMBOAxisNo();
	virtual void OnOK();
	afx_msg void OnSelchangeCOMBOAxisType();
	afx_msg void OnButtonTest();
	afx_msg void OnRadioMovemodel();
	afx_msg void OnRadio3();
	afx_msg void OnRadioMovetype();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnButtonSetsoft0();
	afx_msg void OnButtonApply();
	afx_msg void OnCheckEnabled();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckHardorg();
	BOOL m_bHardOrgEnable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOTIONINFOSET_H__1906D236_E177_4A8D_9F95_7E23C29817DF__INCLUDED_)
