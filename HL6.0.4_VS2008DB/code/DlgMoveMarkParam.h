#pragma once
#include "BtnST.h"

// CDlgMoveMarkParam �Ի���

class CDlgMoveMarkParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgMoveMarkParam)

public:
	CDlgMoveMarkParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMoveMarkParam();

// �Ի�������
	enum { IDD = IDD_DIALOG_MOVEMARK_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int			m_nMode;
	double	m_dbDivDx;
	double	m_dbDivDy;

	double	m_dbWheelDiameter;
	double	m_dbDivAngle;
	double	m_dbExcursionX;
	double	m_dbExcursionY;
	double	m_dbPrecisionK;

	int			m_nEncodePPR;

	virtual	BOOL OnInitDialog();
	void		UpdateView();

	CButtonST m_btnWheel;
	CButtonST m_btnMove;

	afx_msg void OnBnClickedCheckWheelmark();
	afx_msg void OnEnSetfocusEditDivangle();
	afx_msg void OnBnClickedOk();

	int m_nMotorDelay;
};
