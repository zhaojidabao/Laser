#if !defined(AFX_PROPERDLG_H__C19A427C_FDF9_47B7_A938_B493044A3FD7__INCLUDED_)
#define AFX_PROPERDLG_H__C19A427C_FDF9_47B7_A938_B493044A3FD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProperDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProperDlg dialog
#include "Propertylist.h"
#include "Lay.h"
#include "SortListCtrl.h"
#include "LayerColoBtn.h"

class CProperDlg : public CDialog
{
// Construction
public:
	CProperDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProperDlg)
	enum { IDD = IDD_DIALOG_PROPRETY };
	CTabCtrl	m_tabMarkPara;
	CComboBox	m_style;
	CPropertyList	m_listPara;
    CSortListCtrl	m_listLayer;
	CLayerColorButton m_layerColorBt0;
	CLayerColorButton m_layerColorBt1;
	CLayerColorButton m_layerColorBt2;
	CLayerColorButton m_layerColorBt3;
	CLayerColorButton m_layerColorBt4;
	CLayerColorButton m_layerColorBt5;
	CLayerColorButton m_layerColorBt6;
	BOOL	m_bSelMark;
	BOOL	m_bDefault;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	CLay m_lay;
// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CProperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickListLayer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModLayerColor();
	afx_msg void OnClickListLayer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonLayer1();
	afx_msg void OnButtonLayer2();
	afx_msg void OnButtonLayer3();
	afx_msg void OnButtonLayer4();
	afx_msg void OnButtonLayer5();
	afx_msg void OnButtonLayer6();
	afx_msg void OnButtonLayer7();
	afx_msg void OnDoubleclickedButtonLayer1();
	afx_msg void OnDoubleclickedButtonLayer2();
	afx_msg void OnDoubleclickedButtonLayer4();
	afx_msg void OnDoubleclickedButtonLayer3();
	afx_msg void OnDoubleclickedButtonLayer5();
	afx_msg void OnDoubleclickedButtonLayer6();
	afx_msg void OnDoubleclickedButtonLayer7();
	afx_msg void OnCheckSelMark();
	afx_msg void OnCheckDefault();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonErase();
	afx_msg void OnButtonDefault();
	afx_msg void OnSelchangeComboStyle();
	afx_msg void OnLockLayer();
    afx_msg void OnUNLockLayer();
	afx_msg void OnSelchangeTabMarkpara(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSelectLayer();
	afx_msg void OnUnselectLayer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL InitLayer();
	BOOL OnNewDocument(CLay &lay);
	void SetButtonColor(int nIndex, COLORREF color);
	BOOL HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem, int &nSubItem);
    void GetPara(PARA &para);
	void SetPara(PARA &para);
	void UpdateView();
	void OnLayerSelChange(NMHDR* pNMHDR, LRESULT* pRes,BOOL  bUnDo = TRUE);
	void OnParaChange(NMHDR* pNMHDR, LRESULT* pRes);
	void OnParaListChange(NMHDR* pNMHDR, LRESULT* pRes);
	BOOL UpdateProperty();
	
	void ClickColorButton(int nNum);
	void OnDoubleclickedColorButton();
	
	void OnLayerColorChange(int nIndex, COLORREF color);
	void SelMarkParaPage(int nIndex);
	void InitMarkPara();
	void SetParaListShow();
	
	void OnDocChange();
	void OnAddLayer(int nStart, int nCount,  CHLDoc *pDoc);

	void SetFillObjMarkSet(CLay &lay);
	void DoUndoLayerPara();

public:
	typedef CArray<CPropertyItem*, CPropertyItem*> PAGE_ITEM;
	CArray<PAGE_ITEM*, PAGE_ITEM*> m_arEmccPara;

private:
	int m_nItem;
	int m_nSubItem;
	
	CString m_strPath;
	CImageList m_ImageList;
public:
	afx_msg void OnLbnSelchangeListPara();
	afx_msg void OnLvnItemchangedListLayer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLaserdot();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERDLG_H__C19A427C_FDF9_47B7_A938_B493044A3FD7__INCLUDED_)
