// DlgObjInfoCCD.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgObjInfoCCD.h"


// CDlgObjInfoCCD 对话框

IMPLEMENT_DYNAMIC(CDlgObjInfoCCD, CDialog)

CDlgObjInfoCCD::CDlgObjInfoCCD(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjInfoCCD::IDD, pParent)
	, m_nModelNo(0)
	, m_nRetryTimes(0)
	, m_nOutDelay(100)
	, m_bSetPos(FALSE)
	, m_dbX(0)
	, m_dbY(0)
	, m_nExceptPro(0)
	, m_nBaudRate(0)
	, m_nJumpDelay(0)
{
	m_pNode = NULL;
}

CDlgObjInfoCCD::~CDlgObjInfoCCD()
{
}

void CDlgObjInfoCCD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL, m_nModelNo);
	DDX_Text(pDX, IDC_EDIT_TIMES, m_nRetryTimes);
	DDX_Text(pDX, IDC_EDIT_OUT_DELAY, m_nOutDelay);
	DDX_Check(pDX, IDC_CHECK_SET_POS, m_bSetPos);
	DDX_Text(pDX, IDC_EDIT_X, m_dbX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dbY);
	DDX_CBIndex(pDX, IDC_COMBO_EXCEPT, m_nExceptPro);
	DDX_CBIndex(pDX, IDC_COMBO_BAUDRATE, m_nBaudRate);
	DDX_Text(pDX, IDC_EDIT_JUMP_DELAY, m_nJumpDelay);
	DDX_Control(pDX, IDC_COMBO_EXCEPT, m_comExceptPro);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comBaudRate);
	DDV_MinMaxUInt(pDX, m_nModelNo, 0, 99999999);
	DDV_MinMaxUInt(pDX, m_nRetryTimes, 0, 999999999);
	DDV_MinMaxUInt(pDX, m_nOutDelay, 0, 999999999);
	DDV_MinMaxUInt(pDX, m_nJumpDelay, 0, 99999999);
}


BEGIN_MESSAGE_MAP(CDlgObjInfoCCD, CDialog)
	ON_BN_CLICKED(IDC_CHECK_SET_POS, &CDlgObjInfoCCD::OnBnClickedCheckSetPos)
END_MESSAGE_MAP()


// CDlgObjInfoCCD 消息处理程序
BOOL CDlgObjInfoCCD::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitData();
	UpdateDlgView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDlgObjInfoCCD::InitData()
{
	m_comExceptPro.ResetContent();

	CString strValue;

	strValue.LoadString(IDS_STRING_CCD_NG_CONTINUE);
	m_comExceptPro.AddString(strValue);

	strValue.LoadString(IDS_STRING_CCD_NG_EXIT);
	m_comExceptPro.AddString(strValue);
	
	m_comBaudRate.ResetContent();
	m_comBaudRate.AddString(_T("9600"));
	m_comBaudRate.AddString(_T("14400"));
	m_comBaudRate.AddString(_T("19200"));
	m_comBaudRate.AddString(_T("38400"));
	m_comBaudRate.AddString(_T("56000"));
	m_comBaudRate.AddString(_T("57600"));
	m_comBaudRate.AddString(_T("115200"));
}


void CDlgObjInfoCCD::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect())
		return;
	
	UpdateData(TRUE);	
	m_pNode->m_cameraPara.bScanJump = m_bSetPos;
	m_pNode->m_cameraPara.dbX = m_dbX;
	m_pNode->m_cameraPara.dbY = m_dbY;
	m_pNode->m_cameraPara.nDelay = m_nJumpDelay;
	m_pNode->m_cameraPara.nErrorMode = m_nExceptPro;
	m_pNode->m_cameraPara.nNGRepNum = m_nRetryTimes;
	m_pNode->m_cameraPara.nOverTime = m_nOutDelay;
	m_pNode->m_cameraPara.nProgID = m_nModelNo;
	m_pNode->m_cameraPara.nBaudRate = m_nBaudRate;
}


void CDlgObjInfoCCD::SetInfo(CNode* pNode)
{
	if ( !pNode )
		return;
	
	if ( pNode->GetType()  !=  NODE_CCD )
		return;
	
	m_pNode = (CNodeCCD *)pNode;
	
	m_bSetPos = m_pNode->m_cameraPara.bScanJump;
	m_dbX = m_pNode->m_cameraPara.dbX;
	m_dbY = m_pNode->m_cameraPara.dbY;
	m_nJumpDelay = m_pNode->m_cameraPara.nDelay;
	m_nExceptPro = m_pNode->m_cameraPara.nErrorMode;
	m_nRetryTimes = m_pNode->m_cameraPara.nNGRepNum;
	m_nOutDelay = m_pNode->m_cameraPara.nOverTime;
	m_nModelNo = m_pNode->m_cameraPara.nProgID;
	m_nBaudRate = m_pNode->m_cameraPara.nBaudRate;
	
	UpdateData(FALSE);
	UpdateDlgView();
}


void CDlgObjInfoCCD::OnBnClickedCheckSetPos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgView();
}



void CDlgObjInfoCCD::UpdateDlgView()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_X)->EnableWindow(m_bSetPos);
	GetDlgItem(IDC_EDIT_Y)->EnableWindow(m_bSetPos);
	GetDlgItem(IDC_EDIT_JUMP_DELAY)->EnableWindow(m_bSetPos);
}

