#if !defined(AFX_PROPERTYLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
#define AFX_PROPERTYLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyList.h : header file
//

#define PIT_EDIT	0  //PIT = property item type
#define PIT_COMBO	1
#define PIT_COLOR	2
#define PIT_FONT	3
#define PIT_FILE	4
#define PIT_DISABLE 5	//使无效
#define PIT_ADV		6	//包含曲线设置界面

#define PIT_INT		0	//层参数值类型
#define PIT_DOUBLE	1


#define IDC_PROPCMBBOX   712
#define IDC_PROPEDITBOX  713
#define IDC_PROPBTNCTRL  714
#define IDC_PROPADVBTN	 715

#define PROPERTY_CHANGE	0x1002
#define SEL_CHANGE 0X1003

#define SEL_VALUE_CHANGE 0X1004

#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
//CPropertyList Items
class CPropertyItem
{
// Attributes
public:
	BOOL    m_bShow;
	CString m_propName;
	CString m_curValue;
	int     m_nItemType;
	CString m_cmbItems;
	int     m_FCnt;
	float * pFData;
	int     m_TCnt;
	float * pTData;
	CString m_strAlias;
	int		m_nValueType;
	double	m_dbMax;
	double	m_dbMin;

public:
	CPropertyItem(CString propName, CString curValue,
				  int nItemType, CString cmbItems, BOOL bShow = TRUE, 
				  const CString strAlias = _T(""), int nValueType = PIT_INT, double	dbMin = 0.0, double dbMax = 0.0)
	{
		m_propName  = propName;
		m_curValue  = curValue;
		m_nItemType = nItemType;
		m_cmbItems  = cmbItems;
		m_bShow = bShow;
		pFData  = NULL;
		m_FCnt  = 0;
		pTData  = NULL;
		m_TCnt  = 0;
		m_strAlias = strAlias;
		m_nValueType = nValueType;
		m_dbMin = dbMin;
		m_dbMax = dbMax;
	}

	~CPropertyItem()
	{
		ClearFPKData();
	}
	void ClearFPKData()
	{
		if (pFData  !=  NULL)
		{
			delete[] pFData;
			pFData = NULL;
		}

		if (pTData  !=  NULL)
		{
			delete[] pTData;
			pTData = NULL;
		}

		m_FCnt = 0;
		m_TCnt = 0;
	}
	void GetFPKData(OUT float *pFFPKData, int nFFPKNum, OUT float *pTFPKData, int nTFPKNum)
	{
		if (pFData == NULL) return;
		for (int i =  0; i < m_FCnt; i++)
		{
			pFFPKData[i] = pFData[i];
		}

		if (pTData == NULL) return;
		for (int j = 0; j < m_TCnt; j++)
		{
			pTFPKData[j] = pTData[j];
		}
	}
	void SetFPKData(OUT float *pFFPKData, int nFFPKNum, OUT float *pTFPKData, int nTFPKNum)
	{
		ClearFPKData();
		pFData = new float[nFFPKNum];
		for (int i = 0; i < nFFPKNum; i++)
		{
			pFData[i] = pFFPKData[i];
		}

		pTData = new float[nTFPKNum];
		for (int j = 0;  j < nTFPKNum; j++)
		{
			pTData[j] = pTFPKData[j];
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// CPropertyList window

class CPropertyList : public CListBox
{
// Construction
public:
	CPropertyList();

// Attributes
public:

// Operations
public:
	int AddItem(CString txt);
	int AddPropItem(CPropertyItem* pItem);
	void OnSelectOk();
	void Clear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyList)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DeleteItem( LPDELETEITEMSTRUCT lpDeleteItemStruct );
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void DisplayAdvCtrl(CRect rect);
	void NotifyChange(UINT code = PROPERTY_CHANGE);
	virtual ~CPropertyList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchange();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg void OnKillfocusCmbBox();
	afx_msg void OnSelchangeCmbBox();
	afx_msg void OnKillfocusEditBox();
	afx_msg void OnChangeEditBox();
	
	DECLARE_MESSAGE_MAP()
	
	void InvertLine(CDC* pDC,CPoint ptFrom,CPoint ptTo);
	void DisplayButton(CRect region);
	void OnKillfocusEditNoNotify();
	
	CComboBox m_cmbBox;
	CEdit m_editBox;
	CColorButton m_BtnCtrl;
	CFont m_SSerif8Font;
	CButton m_advBtn;
	
	int m_curSel,m_prevSel;
	int m_nDivider;
	int m_nDivTop;
	int m_nDivBtm;
	int m_nOldDivX;
	int m_nLastBox;
	BOOL m_bTracking;
	BOOL m_bDivIsSet;
	HCURSOR m_hCursorArrow;
	HCURSOR m_hCursorSize;
	
	BOOL m_bComBoxChange;
	BOOL m_bEditChange;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
