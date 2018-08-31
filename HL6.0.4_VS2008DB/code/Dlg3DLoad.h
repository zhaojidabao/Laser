#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "HLDoc.h"

// CDlg3DLoad 对话框

class CDlg3DLoad : public CDialog
{
	DECLARE_DYNAMIC(CDlg3DLoad)

public:
	CDlg3DLoad(HL_3DPARA* pPara, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg3DLoad();

// 对话框数据
	enum { IDD = IDD_DIALOG_3DLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void Layout();
	void Update_Data(BOOL bSave = TRUE);
	HL_3DPARA* m_pPara;

	DECLARE_MESSAGE_MAP()
public:
	int m_iModule;
	CComboBox m_cbxShape;
	CString m_strSTLFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadi3dshape();
	afx_msg void OnBnClickedButton3dfilesel();
	double m_dbX0;
	double m_dbY0;
	double m_dbZ0;
	double m_dbX1;
	double m_dbY1;
	double m_dbZ1;
	int m_nShape;
	afx_msg void OnCbnSelchangeCombo3dbase();
	double m_dbRadius;
	double m_dbRadius2;
	double m_dbLong;
	BOOL m_bInside;
	CListCtrl m_listStep;
	double m_dbWidth;
	afx_msg void OnBnClickedButtonAddstep();
	afx_msg void OnBnClickedButtonDelstep();
	afx_msg void OnBnClickedButtonModstep();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListStep(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iILDirect;
	BOOL m_bNew;
	double m_dbSR;
	double m_dbER;
	BOOL m_bVertical;
};
