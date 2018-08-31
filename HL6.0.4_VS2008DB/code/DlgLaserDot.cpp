// DlgLaserDot.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgLaserDot.h"


// CDlgLaserDot 对话框
IMPLEMENT_DYNAMIC(CDlgLaserDot, CDialog)

CDlgLaserDot::CDlgLaserDot(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLaserDot::IDD, pParent)
	, m_dbRadius(0.05)
	, m_dbSpeed(500)
	, m_dbDistance(0.025)
	, m_nType(1)
	, m_dbFreq(20000)
{

	m_dbMinSpeed = 1;
	m_dbMaxSpeed = 10000;
	m_dbMinFreq = 1;
	m_dbMaxFreq = 200000;
}

CDlgLaserDot::~CDlgLaserDot()
{
}

void CDlgLaserDot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dbRadius);
	DDX_Text(pDX, IDC_EDIT_MARKSPEED, m_dbSpeed);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_ctrlSpeed);
	DDX_Control(pDX, IDC_SLIDER_FREQ, m_ctrlFreq);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_dbDistance);
	DDX_Radio(pDX, IDC_RADIO_SPEED, m_nType);
	DDX_Text(pDX, IDC_EDIT_FREQ, m_dbFreq);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_ctrlPreview);
}


BEGIN_MESSAGE_MAP(CDlgLaserDot, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLaserDot::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgLaserDot::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_SPEED, &CDlgLaserDot::OnBnClickedRadioSpeed)
	ON_BN_CLICKED(IDC_RADIO_FREQ, &CDlgLaserDot::OnBnClickedRadioFreq)
	ON_BN_CLICKED(IDC_RADIO_DISTANCE, &CDlgLaserDot::OnBnClickedRadioDistance)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SPEED, &CDlgLaserDot::OnNMCustomdrawSliderSpeed)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FREQ, &CDlgLaserDot::OnNMCustomdrawSliderFreq)
	ON_EN_CHANGE(IDC_EDIT_MARKSPEED, &CDlgLaserDot::OnEnChangeEditMarkspeed)
	ON_EN_CHANGE(IDC_EDIT_FREQ, &CDlgLaserDot::OnEnChangeEditFreq)
	ON_EN_CHANGE(IDC_EDIT_DISTANCE, &CDlgLaserDot::OnEnChangeEditDistance)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, &CDlgLaserDot::OnEnChangeEditRadius)
END_MESSAGE_MAP()


// CDlgLaserDot 消息处理程序

void CDlgLaserDot::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgLaserDot::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CDlgLaserDot::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlSpeed.SetRange(m_dbMinSpeed,m_dbMaxSpeed);
	m_ctrlSpeed.SetPos(m_dbSpeed);
	m_ctrlFreq.SetRange(m_dbMinFreq,m_dbMaxFreq);
	m_ctrlFreq.SetPos(m_dbFreq);
	UdpateView();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLaserDot::UdpateView()
{
	GetDlgItem(IDC_EDIT_DISTANCE)->EnableWindow(m_nType != 2);
	GetDlgItem(IDC_SLIDER_SPEED)->EnableWindow(m_nType != 0);
	GetDlgItem(IDC_EDIT_MARKSPEED)->EnableWindow(m_nType != 0);
	GetDlgItem(IDC_SLIDER_FREQ)->EnableWindow(m_nType != 1);
	GetDlgItem(IDC_EDIT_FREQ)->EnableWindow(m_nType != 1);
}
void CDlgLaserDot::OnBnClickedRadioSpeed()
{
	UpdateData(TRUE);
	UdpateView();
}

void CDlgLaserDot::OnBnClickedRadioFreq()
{
	UpdateData(TRUE);
	UdpateView();
}

void CDlgLaserDot::OnBnClickedRadioDistance()
{
	UpdateData(TRUE);
	UdpateView();
}

void CDlgLaserDot::InitPara()
{
	m_dbSpeed = 500;
	m_dbDistance = 0.025;
	m_dbFreq = m_dbSpeed/m_dbDistance;
	m_ctrlSpeed.SetPos(500);
	m_ctrlFreq.SetPos((int)m_dbFreq);
	UpdateData(FALSE);
	m_ctrlPreview.m_dbDistance = m_dbDistance;
	m_ctrlPreview.m_dbRadius = m_dbRadius;
	m_ctrlPreview.Invalidate(TRUE);
}

void CDlgLaserDot::OnNMCustomdrawSliderSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	double dbTmp = m_dbSpeed;
	UpdateData(TRUE);
	m_dbSpeed = m_ctrlSpeed.GetPos();

	// 点间距*频率=速度
	if( m_nType == 1)  // 固定频率
	{
		 m_dbDistance = MyPrecision(m_dbSpeed/m_dbFreq);
	}
	if( m_nType == 2 && m_dbDistance > EPSILON ) // 固定间距
	{
		double dbFreq;
		dbFreq = m_dbSpeed/m_dbDistance;
		if( dbFreq < m_dbMinFreq || dbFreq > m_dbMaxFreq )
		{
			//InitPara();
			return;
		}
		m_dbFreq = dbFreq;
		m_ctrlFreq.SetPos((int)m_dbFreq);
	}
	UpdateData(FALSE);
	m_ctrlPreview.m_dbDistance = m_dbDistance;
	m_ctrlPreview.m_dbRadius = m_dbRadius;
	m_ctrlPreview.Invalidate(TRUE);

	*pResult = 0;
}

void CDlgLaserDot::OnNMCustomdrawSliderFreq(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	double dbTmp = m_dbFreq;
	UpdateData(TRUE);
	m_dbFreq = m_ctrlFreq.GetPos();

	// 点间距*频率=速度
	if( m_nType == 0 )    // 固定速度
	{
		m_dbDistance = MyPrecision(m_dbSpeed/m_dbFreq);
	}
	else if( m_nType == 2) // 固定间距
	{
		double dbSpeed;
		dbSpeed = m_dbFreq*m_dbDistance;
		if( dbSpeed < m_dbMinSpeed || dbSpeed > m_dbMaxSpeed )
		{
			//InitPara();
			return;
		}

		m_dbSpeed = dbSpeed;
		m_ctrlSpeed.SetPos((int)m_dbSpeed);
	}
	UpdateData(FALSE);
	m_ctrlPreview.m_dbDistance = m_dbDistance;
	m_ctrlPreview.m_dbRadius = m_dbRadius;
	m_ctrlPreview.Invalidate(TRUE);
	*pResult = 0;
}

void CDlgLaserDot::OnEnChangeEditMarkspeed()
{
	UpdateData(TRUE);
	// 点间距*频率=速度
	if( m_dbDistance <= EPSILON || m_dbSpeed < m_dbMinSpeed || m_dbSpeed > m_dbMaxSpeed )
	{
		//InitPara();
		return;
	}

	// 点间距*频率=速度
	if( m_nType == 1)  // 固定频率
	{
		m_dbDistance = MyPrecision(m_dbSpeed/m_dbFreq);
	}
	if( m_nType == 2 && m_dbDistance > EPSILON ) // 固定间距
	{
		double dbFreq;
		dbFreq = m_dbSpeed/m_dbDistance;
		if( dbFreq < m_dbMinFreq || dbFreq > m_dbMaxFreq )
		{
			//InitPara();
			return;
		}
		m_dbFreq = dbFreq;
		m_ctrlFreq.SetPos((int)m_dbFreq);
	}
	UpdateData(FALSE);
	m_ctrlPreview.m_dbDistance = m_dbDistance;
	m_ctrlPreview.m_dbRadius = m_dbRadius;
	m_ctrlPreview.Invalidate(TRUE);
}

void CDlgLaserDot::OnEnChangeEditFreq()
{
	double dbTmp = m_dbFreq;
	UpdateData(TRUE);
	// 点间距*频率=速度
	if( m_dbFreq < m_dbMinFreq || m_dbFreq > m_dbMaxFreq )
	{
		//InitPara();
		return;
	}

	// 点间距*频率=速度
	if( m_nType == 0 )    // 固定速度
	{
		m_dbDistance = MyPrecision(m_dbSpeed/m_dbFreq);
	}
	else if( m_nType == 2) // 固定间距
	{
		double dbSpeed;
		dbSpeed = m_dbFreq*m_dbDistance;
		if( dbSpeed < m_dbMinSpeed || dbSpeed > m_dbMaxSpeed )
		{
			//InitPara();
			return;
		}

		m_dbSpeed = dbSpeed;
		m_ctrlSpeed.SetPos((int)m_dbSpeed);
	}
	UpdateData(FALSE);
	m_ctrlPreview.m_dbDistance = m_dbDistance;
	m_ctrlPreview.m_dbRadius = m_dbRadius;
	m_ctrlPreview.Invalidate(TRUE);
}

void CDlgLaserDot::OnEnChangeEditDistance()
{
	UpdateData(TRUE);
	if( m_dbDistance < EPSILON )
	{
		//InitPara();
		return;
	}
	else
	{
		if( m_nType == 0 ) // 速度不变
		{
			m_dbFreq = m_dbSpeed/m_dbDistance;
			m_ctrlFreq.SetPos((int)m_dbFreq);
		}
		else if( m_nType == 1 )
		{
			m_dbSpeed = m_dbFreq*m_dbDistance;
			m_ctrlSpeed.SetPos((int)m_dbSpeed);
		}
		UpdateData(FALSE);
	}
	m_ctrlPreview.m_dbDistance = m_dbDistance;
	m_ctrlPreview.m_dbRadius = m_dbRadius;
	m_ctrlPreview.Invalidate(TRUE);
}

void CDlgLaserDot::OnEnChangeEditRadius()
{
	//double dbTmp = m_dbRadius;
	UpdateData(TRUE);
// 	if( m_dbRadius < 0.0001 || m_dbRadius > 10 )
// 	{
// 		AfxMessageBox(_T("Dot radius must between 0.0001 and 10!"));
// 		m_dbRadius = dbTmp;
// 		UpdateData(FALSE);
// 		return;
// 	}
	m_ctrlPreview.m_dbDistance = m_dbDistance;
	m_ctrlPreview.m_dbRadius = m_dbRadius;
	m_ctrlPreview.Invalidate(TRUE);
}
