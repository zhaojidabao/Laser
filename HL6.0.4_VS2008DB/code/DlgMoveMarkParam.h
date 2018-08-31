#pragma once
#include "BtnST.h"

// CDlgMoveMarkParam 对话框

class CDlgMoveMarkParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgMoveMarkParam)

public:
	CDlgMoveMarkParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMoveMarkParam();

// 对话框数据
	enum { IDD = IDD_DIALOG_MOVEMARK_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
