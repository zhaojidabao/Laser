#if !defined(AFX_TEXTEDITDLG_H__9DAFB2EC_C74C_4F0B_8D18_97F732FFBF66__INCLUDED_)
#define AFX_TEXTEDITDLG_H__9DAFB2EC_C74C_4F0B_8D18_97F732FFBF66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextEditDlg.h : header file
//
#include "Text.h"
#include "NodeText.h"
#include "FontComboBox.h"
#include "FntTipComb.h"

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg dialog

class CTextEditDlg : public CDialog
{
	typedef struct tagTextInfo
	{
		tagTextInfo()
		{
			nType = 0;
			nTexID = 0;
		};
		
		void operator = ( const tagTextInfo & src )
		{
			if ( &src == this )	
				return;
			
			nType = src.nType;
			nTexID = src.nTexID;
		}
		
		UINT nType;
		UINT nTexID;
	}TEXTINFO;

// Construction
public:
	CTextEditDlg(CWnd* pParent = NULL);   // standard constructor

	void    UpdateDataView();
	CString GetSerialString();

// Dialog Data
	//{{AFX_DATA(CTextEditDlg)
	enum { IDD = IDD_TEXTEDIT_DLG };
	CComboBox	m_cbxAscType;
	CComboBox	m_cbxHzkType;

	CFontComboBox	m_comFontTTF;
	CFntTipComb	m_comUserFont;
	CFntTipComb	m_comShxChs;
	CFntTipComb	m_comShxEn;
	CFntTipComb	m_comShxNum;

	CListBox		m_listData;

	CComboBox	m_comFontType;
	CComboBox	m_comTextType;

	CString		m_strFontCHS;
	CString		m_strFontEN;
	CString		m_strFontNUM;
	CString		m_strFontUser;
	int				m_nTextType;
	int				m_nFontType;
	
	CString		m_strDate;
	CString		m_strFix;
	BOOL			m_bModify;
	double		m_dbFontHeight;
	CString		m_strHzkType;
	CString		m_strAscType;
	CString		m_strModelName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEditDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSerial();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeDateEdit();
	afx_msg void OnButtonTextadd();
	afx_msg void OnButtonTextdel();
	afx_msg void OnButtonTextedit();
	afx_msg void OnSelchangeListEditdata();
	afx_msg void OnDestroy();
	afx_msg void OnCancelMode();
	afx_msg void OnButtonAdvance();
	afx_msg void OnSelchangeCOMBOFontType();
	afx_msg void OnSelchangeCOMBOTextType();
	afx_msg void OnSelchangeCOMBOFontTTF();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetTextInfo(CNode *pNode);
	BOOL FindTextInfo(int nIndex, TEXTINFO &TextInfo);
	void	ListFlash(int nSel = 0);

	void	OnNewDocment();
	BOOL IsCreateDlg();

public:
	CNodeText* 	m_pNodeText;
	CText*			m_pCurSelText;

	CArray<TEXTINFO, TEXTINFO> m_TextList;
	LOGFONT   m_lf;

	void Apply();
	void InitComBox();
	void GetItemMovePos(int res, int obj,int &nX,int &nY);
	void MoveDlgItem(int nItem, int nX,int nY);
	void UpdateDlgTextItemView();
	void UpdateDlgFontItemView();

private:
	void Layout();

public:
	afx_msg void OnEnKillfocusFixEdit();
	afx_msg void OnBnClickedCheckModify();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTEDITDLG_H__9DAFB2EC_C74C_4F0B_8D18_97F732FFBF66__INCLUDED_)
