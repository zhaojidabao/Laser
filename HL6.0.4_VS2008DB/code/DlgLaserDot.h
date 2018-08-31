#pragma once
#include "afxcmn.h"
#include "picstatic.h"


// CDlgLaserDot 对话框
class CDlgLaserDot : public CDialog
{
	DECLARE_DYNAMIC(CDlgLaserDot)

public:
	CDlgLaserDot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLaserDot();

// 对话框数据
	enum { IDD = IDD_DIALOG_LASERDOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbRadius;
	double m_dbSpeed;
	double m_dbFreq;
	double m_dbDistance;

	// 最小速度
	double m_dbMinSpeed;
	// 最大速度
	double m_dbMaxSpeed;

	// 最小频率
	double m_dbMinFreq;
	// 最大频率
	double m_dbMaxFreq;

	int m_nType;
	CSliderCtrl m_ctrlSpeed;
	CSliderCtrl m_ctrlFreq;
	CPicStatic m_ctrlPreview;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

	void UdpateView();
	void InitPara();
	afx_msg void OnBnClickedRadioSpeed();
	afx_msg void OnBnClickedRadioFreq();
	afx_msg void OnBnClickedRadioDistance();
	afx_msg void OnNMCustomdrawSliderSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderFreq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditMarkspeed();
	afx_msg void OnEnChangeEditFreq();
	afx_msg void OnEnChangeEditDistance();
	afx_msg void OnEnChangeEditRadius();
};
