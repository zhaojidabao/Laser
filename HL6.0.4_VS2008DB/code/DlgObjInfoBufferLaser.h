#pragma once

#include "NodeBufferLaser.h"
// CDlgObjInfoBufferLaser 对话框

class CDlgObjInfoBufferLaser : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjInfoBufferLaser)

public:
	CDlgObjInfoBufferLaser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgObjInfoBufferLaser();

// 对话框数据
	enum { IDD = IDD_DIALOG_OBJINFO_BUFFER_LASER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bLaserOn;
	double m_dbQFreq;
	double m_dbQRls;
	double m_dbCurrent;
	double m_dbPower;
	int m_nLaserMode;
	afx_msg void OnBnClickedRadioType();
	afx_msg void OnBnClickedRadio02();


	void UpdateView();
	void Apply();
	void SetInfo(CNode* pNode);

	//
	CNodeBufferLaser *m_pNode;
	
};
