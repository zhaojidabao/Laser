#if !defined(AFX_DLGRULEREDIT_H__090FFD2F_97CE_4CFA_9292_2DFC3C5EF723__INCLUDED_)
#define AFX_DLGRULEREDIT_H__090FFD2F_97CE_4CFA_9292_2DFC3C5EF723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRulerEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRulerEdit dialog
#include "XZoomWnd.h"
#include "BtnST.h"

typedef struct RulerObjInfo
{
	RulerObjInfo()
	{
		m_pObj = NULL;
		m_nPriority = 1;
		m_nJumpCount = 1;
		m_bViewNumber = FALSE;
		m_nDecimalDigits = 0;
	}
	
	~RulerObjInfo()
	{
		m_pObj = NULL;
		m_nPriority = 1;
		m_nJumpCount = 1;
		m_bViewNumber = FALSE;
		m_nDecimalDigits = 0;
	}
	
	void operator = ( const RulerObjInfo & src )
	{
		if ( &src == this )	
			return;

		m_pObj = src.m_pObj;
		m_nPriority = src.m_nPriority;
		m_nJumpCount = src.m_nJumpCount;
		m_bViewNumber = src.m_bViewNumber;
		m_nDecimalDigits = src.m_nDecimalDigits;
	}

	DWORD   m_pObj;
	int     m_nPriority;      //刻度级别
	int     m_nJumpCount;     //间隔线条数
	BOOL    m_bViewNumber;    //是否显示刻度
	int     m_nDecimalDigits; //小数位数
}RULEROBJINFO;


class CDlgRulerEdit : public CDialog
{
// Construction
public:
	CDlgRulerEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRulerEdit)
	enum { IDD = IDD_DIALOG_RULER };
	CStatic	m_objView;
	CListCtrl	m_listCtrObj;
	int			m_nRulerType;
	double	m_dbDiameter;
	double	m_dbLineDist;
	double	m_dbCircleDis;
	double	m_dbStart;
	int			m_nLineCount;
	double	m_dbAdd;
	BOOL		m_bRemoveLastLine;
	int			m_nAlignType;
	//}}AFX_DATA

public:
	XZoomWnd m_xzoom;
	CArray<RULEROBJINFO, RULEROBJINFO> m_arrayRulerObjInfo;
	CTypedPtrList<CObList,CGroupB *> m_listGroupSource;
	CTypedPtrList<CObList,CGroupB *> m_listGroup;
	int m_nCurObjNu;
	CProperty m_ptyInit;
	CProperty m_ptySel;
	CButtonST	m_btnView;

	void InitZoomView();
	void InitListCtrl();
	void UpdateListObjView();
	void UpdateObjInfo();
	void UpdateZoomList();
	void UpdateRulerTypeInfo();
	BOOL HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem);
	void AddObj(CGroupObj *pGroup);
	void CreateRulerData();
	CGroupObj* CreateRulerLine(int i, RULEROBJINFO &rulerObj);

	void DeleteDataObjList();
	void DeleteSourceObjList();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRulerEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRulerEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListObj(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckRemovelastline();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonSet();
	afx_msg void OnRadioType();
	afx_msg void OnRadio2();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkListObj(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioAligndown();
	afx_msg void OnRadioAligntop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRULEREDIT_H__090FFD2F_97CE_4CFA_9292_2DFC3C5EF723__INCLUDED_)
