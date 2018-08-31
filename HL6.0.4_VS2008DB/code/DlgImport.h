#if !defined(AFX_DLGIMPORT_H__0011AD2A_DC9F_43D0_8224_3E76F9EA5FC7__INCLUDED_)
#define AFX_DLGIMPORT_H__0011AD2A_DC9F_43D0_8224_3E76F9EA5FC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgImport dialog

class CDlgImport : public CFileDialog
{
	DECLARE_DYNAMIC(CDlgImport)

public:
	CDlgImport(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT| OFN_ENABLETEMPLATE,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

		virtual int DoModal();
		BOOL m_bCenter;
        BOOL m_bPltBetter;
		BOOL m_bPreview;
		
		int m_nAiType;
		int m_nAiFillType;
		double m_dbAiFillDis;
protected:
	//{{AFX_MSG(CDlgImport)
	afx_msg void OnCheckPreview();
	afx_msg void OnPaint();
	afx_msg void OnAiType();
	afx_msg void OnRadio2();
	afx_msg void OnAiFillType();
	afx_msg void OnRadio4();
	
	virtual BOOL OnInitDialog();
	virtual void OnFileNameChange();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnFileNameOK();
	
	//‘§¿¿”√
	XDC m_DC;
	CNode* m_pNode;
	CRect  m_rect;
	void RefreshView();
	void UpdateDlgView();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPORT_H__0011AD2A_DC9F_43D0_8224_3E76F9EA5FC7__INCLUDED_)
