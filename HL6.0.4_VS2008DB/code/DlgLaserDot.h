#pragma once
#include "afxcmn.h"
#include "picstatic.h"


// CDlgLaserDot �Ի���
class CDlgLaserDot : public CDialog
{
	DECLARE_DYNAMIC(CDlgLaserDot)

public:
	CDlgLaserDot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLaserDot();

// �Ի�������
	enum { IDD = IDD_DIALOG_LASERDOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dbRadius;
	double m_dbSpeed;
	double m_dbFreq;
	double m_dbDistance;

	// ��С�ٶ�
	double m_dbMinSpeed;
	// ����ٶ�
	double m_dbMaxSpeed;

	// ��СƵ��
	double m_dbMinFreq;
	// ���Ƶ��
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
