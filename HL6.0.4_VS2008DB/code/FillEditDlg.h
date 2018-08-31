#if !defined(AFX_FILLEDITDLG_H__2BF5CC1D_6101_46E9_B20A_B6E8E6793A99__INCLUDED_)
#define AFX_FILLEDITDLG_H__2BF5CC1D_6101_46E9_B20A_B6E8E6793A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FillEditDlg.h : header file
//
#include "Objecteditfill.h"
#include "FillObjmMarkSetDlg.h"
#include "TextEditDlg.h"
#include "DlgBarEdit.h"
#include "DlgSetDelay.h"
#include "DlgSetInput.h"
#include "DlgSetOutput.h"
#include "DlgObjEditArcInfo.h"
#include "DlgObjInfoCircle.h"
#include "DlgObjInfoSpline.h"
#include "DlgObjInfoMotorMove.h"
#include "DlgObjInfoMotorPos.h"
#include "DlgObjInfoBufferLaser.h"
#include "DlgObjInfoCCD.h"
#include "DlgObjPosCali.h"

/////////////////////////////////////////////////////////////////////////////
// CFillEditDlg dialog

class CFillEditDlg : public CDialog
{
// Construction
public:
	CFillEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFillEditDlg)
	enum { IDD = IDD_DIALOG_EDITFILL };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFillEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void GeneralBasePage(BOOL bSelPage);

	// Generated message map functions
	//{{AFX_MSG(CFillEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonApply();
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFastmark();
	afx_msg void OnButtonDownOfflinedata();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	void SetTable(int id);
	void CheckFastmark();
	void DownOfflinedata();
	void GeneralPage(CNode *pNode);
	
public:
	CObjecteditfill				m_ObjFillEdit;
	CFillObjmMarkSetDlg		m_FillObjMarkSet;
	CTextEditDlg				m_TextEditDlg;
	CDlgBarEdit					m_BarEditDlg;
	CDlgSetDelay				m_DelayEditDlg;
	CDlgSetInput				m_InputEditDlg;
	CDlgSetOutput				m_OutputEditDlg;
	CDlgObjEditArcInfo		m_ArcInfo;
	CDlgObjInfoCircle			m_CircleInfo;
	CDlgObjInfoMotorMove	m_MotorMove;
	CDlgObjInfoMotorPos		m_MotorPos;
	CDlgObjInfoBufferLaser  m_BufferLaser;
	CDlgObjInfoCCD			m_CCD;
	CDlgObjPosCali			m_PosCali;
	int		 m_nTabIndex;
	
private:
	int       m_nTabPrev;
	int       m_nObjPrev;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLEDITDLG_H__2BF5CC1D_6101_46E9_B20A_B6E8E6793A99__INCLUDED_)
