// DlgMoveMarkParam.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgMoveMarkParam.h"

#define LASER_FOCUS_LENGTH	(0.5)

// CDlgMoveMarkParam 对话框
IMPLEMENT_DYNAMIC(CDlgMoveMarkParam, CDialog)

CDlgMoveMarkParam::CDlgMoveMarkParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMoveMarkParam::IDD, pParent)
	, m_nMode(0)
	, m_dbWheelDiameter(80)
	, m_dbDivAngle(1)
	, m_dbExcursionX(0)
	, m_dbExcursionY(0)
	, m_dbDivDx(10)
	, m_dbDivDy(10)
	, m_dbPrecisionK(100)
	, m_nEncodePPR(2000)
	, m_nMotorDelay(100)
{

}

CDlgMoveMarkParam::~CDlgMoveMarkParam()
{
}

void CDlgMoveMarkParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_WHEELMARK, m_nMode);
	DDX_Text(pDX, IDC_EDIT_DIVX, m_dbDivDx);
	DDX_Text(pDX, IDC_EDIT_DIVY, m_dbDivDy);
	DDX_Text(pDX, IDC_EDIT_DIAMETER, m_dbWheelDiameter);
	DDX_Text(pDX, IDC_EDIT_DIVANGLE, m_dbDivAngle);
	DDX_Text(pDX, IDC_EDIT_X, m_dbExcursionX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dbExcursionY);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_btnWheel);
	DDX_Control(pDX, IDC_BUTTON_VIEW2, m_btnMove);
	DDV_MinMaxDouble(pDX, m_dbWheelDiameter, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_UNITPRECISION, m_dbPrecisionK);
	DDX_Text(pDX, IDC_EDIT_ENCODEPPR, m_nEncodePPR);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nMotorDelay);
}


BEGIN_MESSAGE_MAP(CDlgMoveMarkParam, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgMoveMarkParam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_WHEELMARK, &CDlgMoveMarkParam::OnBnClickedCheckWheelmark)
	ON_EN_SETFOCUS(IDC_EDIT_DIVANGLE, &CDlgMoveMarkParam::OnEnSetfocusEditDivangle)
END_MESSAGE_MAP()


// CDlgMoveMarkParam 消息处理程序

BOOL CDlgMoveMarkParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nMode				= g_SysCfg.m_sMoveMarkParam.m_nMode;
	m_dbWheelDiameter	= g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter;
	m_dbDivAngle		= g_SysCfg.m_sMoveMarkParam.m_dbDivAngle;
	m_dbExcursionX	= g_SysCfg.m_sMoveMarkParam.m_dbExcursionX;
	m_dbExcursionY	= g_SysCfg.m_sMoveMarkParam.m_dbExcursionY;
	m_dbDivDx			= g_SysCfg.m_sMoveMarkParam.m_dbDivDx;
	m_dbDivDy			= g_SysCfg.m_sMoveMarkParam.m_dbDivDy;
	m_dbPrecisionK	= g_SysCfg.m_sMoveMarkParam.m_dbPrecisionK;
	m_nEncodePPR		= g_SysCfg.m_sMoveMarkParam.m_nEncodePPR;
	m_nMotorDelay			= g_SysCfg.m_sMoveMarkParam.m_nMotorDelay;
	UpdateData(FALSE);

	m_btnWheel.SetIcon(IDI_ICON_WHEELMARK);
	m_btnWheel.DrawBorder(FALSE);
	m_btnMove.SetIcon(IDI_ICON_MOVEMARK);
	m_btnMove.DrawBorder(FALSE);
	UpdateView();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMoveMarkParam::UpdateView()
{
	GetDlgItem(IDC_EDIT_DIVX)->EnableWindow( m_nMode==0 );
	GetDlgItem(IDC_EDIT_DIVY)->EnableWindow( m_nMode==0 );
	GetDlgItem(IDC_EDIT_UNITPRECISION)->EnableWindow((m_nMode==0));

	GetDlgItem(IDC_EDIT_ENCODEPPR)->EnableWindow( m_nMode>0 );
	GetDlgItem(IDC_EDIT_DIAMETER)->EnableWindow((m_nMode>0));
	GetDlgItem(IDC_EDIT_DIVANGLE)->EnableWindow((m_nMode>0));
	GetDlgItem(IDC_EDIT_X)->EnableWindow((m_nMode>0));
	GetDlgItem(IDC_EDIT_DELAY)->EnableWindow((m_nMode>0));

//	GetDlgItem(IDC_EDIT_Y)->EnableWindow((m_nMode>0));
}

void CDlgMoveMarkParam::OnBnClickedOk()
{
	UpdateData(TRUE);
	if ( m_dbDivDx < EPSILON || m_dbDivDy < EPSILON || m_dbDivAngle > 90 || fabs(m_dbPrecisionK) < EPSILON )
	{
		AfxMessageBox(_T("Error parameter!"));
		return;
	}

	g_SysCfg.m_sMoveMarkParam.m_nMode = m_nMode;
	g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter = m_dbWheelDiameter;
	g_SysCfg.m_sMoveMarkParam.m_dbDivAngle = m_dbDivAngle;
	g_SysCfg.m_sMoveMarkParam.m_dbExcursionX = m_dbExcursionX;
	g_SysCfg.m_sMoveMarkParam.m_dbExcursionY = m_dbExcursionY;
	g_SysCfg.m_sMoveMarkParam.m_dbDivDx = m_dbDivDx;
	g_SysCfg.m_sMoveMarkParam.m_dbDivDy = m_dbDivDy;
	g_SysCfg.m_sMoveMarkParam.m_dbPrecisionK = m_dbPrecisionK;
	g_SysCfg.m_sMoveMarkParam.m_nEncodePPR = m_nEncodePPR;
	g_SysCfg.m_sMoveMarkParam.m_nMotorDelay	   = m_nMotorDelay;
	g_sys.WriteSysCfg();

	OnOK();
}

void CDlgMoveMarkParam::OnBnClickedCheckWheelmark()
{
	UpdateData(TRUE);
	UpdateView();
}

// 计算推荐的角度
void CDlgMoveMarkParam::OnEnSetfocusEditDivangle()
{
	UpdateData(TRUE);

	if ( m_dbWheelDiameter > 1 && m_dbWheelDiameter < 5000 )
	{
		double dbAngle = acos((m_dbWheelDiameter-2*LASER_FOCUS_LENGTH)/m_dbWheelDiameter);
		m_dbDivAngle = MyRadToDeg(dbAngle)*2;
		m_dbDivAngle = MyPrecision(m_dbDivAngle,2);
		UpdateData(FALSE);
	}
}
