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
	int		m_nFillNo;                 //填充序号
	BOOL	m_bEquaDist;               //是否等间距充
	BOOL	m_bKeepAngle;              //填充线随轮廓旋转
	double	m_dbSpace;                 //填充间距
	BOOL	m_bEnable;                 //填充使能
	BOOL	m_bLoop;                   //缩扩使能
	double	m_dbAngle;                 //填充角度
	double	m_dbBeamComp;              //光栅大小
	double	m_dbLineReduce;            //线缩进量
	int		m_nLoopNum;                //缩扩圈数
	double	m_dbStartOffset;           //起始偏移
	double	m_dbEndOffset;             //末尾偏移
	double	m_dbMinJump;               //最小跳转间距
	BOOL	m_bRemoveOrg;              //缩扩时删除原始轮廓
	double	m_dbLoopDist;              //缩扩间距
	BOOL	m_bRemoveFill;             //删除填充线
	BOOL	m_bFillFirst;			   //先打填充线
	BOOL    m_bAlign;                  //打标时填充线对齐
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
	int  m_nPrevFillNo;	     //填充线类型
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
