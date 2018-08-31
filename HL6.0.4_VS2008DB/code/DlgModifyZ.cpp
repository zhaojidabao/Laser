// DlgModifyZ.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgModifyZ.h"


// CDlgModifyZ �Ի���

IMPLEMENT_DYNAMIC(CDlgModifyZ, CDialog)

CDlgModifyZ::CDlgModifyZ(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgModifyZ::IDD, pParent)
	, m_dbZ(0)
	, m_dbX(0)
	, m_dbY(0)
{
	m_bEditXY = FALSE;
}

CDlgModifyZ::~CDlgModifyZ()
{
}

void CDlgModifyZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Z, m_dbZ);
	DDX_Text(pDX, IDC_EDIT_X, m_dbX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dbY);
}


BEGIN_MESSAGE_MAP(CDlgModifyZ, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModifyZ::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModifyZ ��Ϣ�������

BOOL CDlgModifyZ::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_EDIT_X)->EnableWindow(m_bEditXY);
	GetDlgItem(IDC_EDIT_Y)->EnableWindow(m_bEditXY);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgModifyZ::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
