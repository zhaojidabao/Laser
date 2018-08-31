#if !defined(AFX_OBJECTLISTDLG_H__615BDF4A_486B_4231_87A0_E4AD8A185093__INCLUDED_)
#define AFX_OBJECTLISTDLG_H__615BDF4A_486B_4231_87A0_E4AD8A185093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectListDlg dialog
#include <map>
#include "Hldoc.h"
#include "VtrlListCtrl.h"

class CObjectListDlg : public CDialog
{
// Construction
public:
	CObjectListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectListDlg)
	enum { IDD = IDD_DIALOG_OBJECT_LIST };
	CVtrlListCtrl	m_DocObjList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectListDlg)
	afx_msg void OnClickListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMovePrev();
	afx_msg void OnMoveNext();
	afx_msg void OnMovetoLast();
	afx_msg void OnMovetoFirst();
	afx_msg void OnObjChangename();
	afx_msg void OnKeydownListDocObject(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnItemDrag(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
public:
	BOOL SetHeadings( const CString& strHeadings );
	BOOL AddClmItem(CGroupObj *pGroupObj);
	BOOL DelAllClmItem();
	void ObjDel();
	void ObjListLoad();
	void ObjListUnLoad();

	int  SetSelByGroupObj(CGroupObj *pGroupObj);
	void SetObjUnSel();
	void SortTreeObject(int nFrom, int nTo);
	//
	CImageList m_ImageList;
	int  m_nListCurItem;
	int  m_nCurItem;
	
	CArray<CGroupObj*, CGroupObj*> m_listInfo;


private:
	BOOL m_bBlockItem;
	//switchType m_eShow;	// 0:不在BOLCK中的对象 1:正常关键字(蓝色) 2:非正常关键字(红色) 3:在BOLCK中的对象
public:
	afx_msg void OnLvnItemchangedListDocObject(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTLISTDLG_H__615BDF4A_486B_4231_87A0_E4AD8A185093__INCLUDED_)
