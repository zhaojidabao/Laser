#pragma once

#include "resource.h"
#include "DataMng.h"

// CDlg3DTransform 对话框

class CHLDoc;
class CDlg3DTransform : public CDialog
{
	DECLARE_DYNAMIC(CDlg3DTransform)

public:
	CDlg3DTransform(CHLDoc* pDoc, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg3DTransform();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRANSFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
	
public:
	double	m_dbOffsetX;
	double	m_dbOffsetY;
	double	m_dbOffsetZ;
	double	m_dbRotateZ;
	int		m_iRefer;
	int		m_iDirect;
	double	m_dbToBaseZ;
	CHLDoc* m_pDoc;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnEnChangeEdit3doffsety();
	afx_msg void OnBnClickedButtonSettobasez();
	
	void SwitchRadio();
	void Transforom();
	
	BOOL m_bRotateXY;
	BOOL m_nTransType;
	
	//
	double	m_dbOffsetXOld;
	double	m_dbOffsetYOld;
	double	m_dbOffsetZOld;
	double	m_dbRotateZOld;

	void TransPreprocess();
};
