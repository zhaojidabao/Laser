// DlgObjInfoBufferLaser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgObjInfoBufferLaser.h"


// CDlgObjInfoBufferLaser �Ի���

IMPLEMENT_DYNAMIC(CDlgObjInfoBufferLaser, CDialog)

CDlgObjInfoBufferLaser::CDlgObjInfoBufferLaser(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjInfoBufferLaser::IDD, pParent)
	, m_bLaserOn(FALSE)
	, m_dbQFreq(0)
	, m_dbQRls(0)
	, m_dbCurrent(0)
	, m_dbPower(0)
	, m_nLaserMode(0)
{
	m_pNode = NULL;
}

CDlgObjInfoBufferLaser::~CDlgObjInfoBufferLaser()
{
}

void CDlgObjInfoBufferLaser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_bLaserOn);
	DDX_Text(pDX, IDC_EDIT_QFREQ, m_dbQFreq);
	DDX_Text(pDX, IDC_EDIT_QRLS, m_dbQRls);
	DDX_Text(pDX, IDC_EDIT_CURRENT, m_dbCurrent);
	DDX_Text(pDX, IDC_EDIT_POWER, m_dbPower);
	DDX_Text(pDX, IDC_EDIT_MODEL, m_nLaserMode);
	DDV_MinMaxDouble(pDX, m_dbPower, 0, 100);
	DDV_MinMaxInt(pDX, m_nLaserMode, 0, 63);
	DDV_MinMaxDouble(pDX, m_dbQFreq, 1, 1000);
	DDV_MinMaxDouble(pDX, m_dbQRls, 1, 1000);
	DDV_MinMaxDouble(pDX, m_dbCurrent, 0.0, 50);
}


BEGIN_MESSAGE_MAP(CDlgObjInfoBufferLaser, CDialog)
	ON_BN_CLICKED(IDC_RADIO_TYPE, &CDlgObjInfoBufferLaser::OnBnClickedRadioType)
	ON_BN_CLICKED(IDC_RADIO02, &CDlgObjInfoBufferLaser::OnBnClickedRadio02)
END_MESSAGE_MAP()


// CDlgObjInfoBufferLaser ��Ϣ�������
void CDlgObjInfoBufferLaser::OnBnClickedRadioType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UpdateView();
}

void CDlgObjInfoBufferLaser::OnBnClickedRadio02()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UpdateView();
}



void CDlgObjInfoBufferLaser::UpdateView()
{
	int nType = 0;
	if ( g_DAT.GetLayerParaType( nType )  !=  HLOK )
	{
		return;
	}
	
	switch( nType )
	{
	case eType_CO2_GENERAL:
	case eType_CO2_CLASSIC:
		GetDlgItem(IDC_EDIT_QFREQ)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_QRLS)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MODEL)->EnableWindow(FALSE);
		break;

	case eType_Q_AP_DQ_CLASSIC:	//��ͨ��Դ������Q ��ͳģʽ
	case eType_Q_AP_AQ_CLASSIC:	//��ͨ��Դ��ģ��Q ��ͳģʽ
	case eType_Q_DP_DQ_CLASSIC:	//���ֵ�Դ������Q ��ͳģʽ
	case eType_Q_DP_AQ_CLASSIC:	//���ֵ�Դ��ģ��Q ��ͳģʽ
	case eType_Q_AP_AQ_GENERAL:	//��ͨ��Դ��ģ��Q ͨ��ģʽ
	case eType_Q_DP_DQ_GENERAL: //���ֵ�Դ������Q ͨ��ģʽ
	case eType_Q_DP_AQ_GENERAL:	//���ֵ�Դ��ģ��Q ͨ��ģʽ
		GetDlgItem(IDC_EDIT_QFREQ)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_QRLS)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_POWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MODEL)->EnableWindow(FALSE);
		break;

	case eType_IPG:
		GetDlgItem(IDC_EDIT_QFREQ)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_QRLS)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POWER)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_MODEL)->EnableWindow(FALSE);
		break;
	
	case eType_LD:
		GetDlgItem(IDC_EDIT_QFREQ)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_QRLS)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_POWER)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_MODEL)->EnableWindow(m_bLaserOn);
		break;
	
	case eType_IPGM:
	case eType_IPGM_GENERAL:
	case eType_YLPP_CLASSIC:
	case eType_YLPP_GENERAL:
	case eType_SPI_GENERAL:
	case eType_SPI_CLASSIC:
	case eType_YLPV2_CLASSIC:
	case eType_YLPV2_GENERAL:
	case eType_HFMA_CLASSIC:
	case eType_HFMA_GENERAL:
		GetDlgItem(IDC_EDIT_QFREQ)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_QRLS)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POWER)->EnableWindow(m_bLaserOn);
		GetDlgItem(IDC_EDIT_MODEL)->EnableWindow(m_bLaserOn);
		break;
	}
}




BOOL CDlgObjInfoBufferLaser::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CDlgObjInfoBufferLaser::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect())
		return;
	
	UpdateData(TRUE);

	BufferLaserPara para;
	para.bLaserOn = m_bLaserOn;
	para.dbCurrent = m_dbCurrent;
	para.dbPower = m_dbPower;
	para.dbQFreq = m_dbQFreq;
	para.dbQRls = m_dbQRls;
	para.nLaserMode = m_nLaserMode;

	m_pNode->SetBufferLaserPara( &para );
}


void CDlgObjInfoBufferLaser::SetInfo(CNode* pNode)
{
	if ( !pNode )
		return;

	m_pNode = (CNodeBufferLaser *)pNode;

	BufferLaserPara para;
	m_pNode->GetBudderLaserPara(&para);
	
	m_bLaserOn = para.bLaserOn;
	m_dbCurrent = para.dbCurrent;
	m_dbPower = para.dbPower;
	m_dbQFreq = para.dbQFreq;
	m_dbQRls = para.dbQRls;
	m_nLaserMode = para.nLaserMode;

	UpdateData(FALSE);
	UpdateView();
}