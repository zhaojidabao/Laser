#if !defined(AFX_BARCODEDLG_H__5FA7ECE9_D0FD_4BA8_A917_D522C621FB7D__INCLUDED_)
#define AFX_BARCODEDLG_H__5FA7ECE9_D0FD_4BA8_A917_D522C621FB7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarCodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBarCodeDlg dialog
#include "TextPageTextDlg.h"
#include "BarPage.h"
#include "DlgBarPageText.h"

class CBarCodeDlg : public CDialog
{
// Construction
public:
	CBarCodeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarCodeDlg)
	enum { IDD = IDD_BARCODE_DLG };
	CTabCtrl		m_tab;
	CComboBox	m_ctrlType;
	//}}AFX_DATA
	CDlgBarPageText m_TextPage;
	CBarPage m_BarPage;

	UINT m_nBarType;
	int     m_nOutline;		//PDF417��������
	int     m_nErrLev;			//����������QR,DATAMATRIX
	int		m_nHPoint;			//��ֱ��������,����QR,DATAMATRIX
	int		m_nWPoint;			//ˮƽ��������,����DATAMATRIX
	int     m_nSymbolNo;		//DATAMATRIX ����
	int     m_nShapeNo;		//DATAMARTIX ��״
	int		m_nVersion;
	int		m_nCodeType;
	BOOL	m_bAutoSet;		//�������볤������;//true �Զ�����;FALSE �û�����
	DWORD   m_nSingleLineMarkCount;
	UINT		 m_nNarrowLineNum;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabText(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeComboBarcodetype();
	afx_msg void OnButBarcodeset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void FlashBarDefaultProperty();

public:
	BOOL CreateCodeBarStatus(long lType);
	CString GetBarString();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODEDLG_H__5FA7ECE9_D0FD_4BA8_A917_D522C621FB7D__INCLUDED_)
