// DlgSetHanXinCode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgSetHanXinCode.h"


// CDlgSetHanXinCode �Ի���

IMPLEMENT_DYNAMIC(CDlgSetHanXinCode, CDialog)

CDlgSetHanXinCode::CDlgSetHanXinCode(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgSetHanXinCode::IDD, pParent)
	, m_nVersion(1)
	, m_nErrorLevel(0)
{

}

CDlgSetHanXinCode::~CDlgSetHanXinCode()
{
}

void CDlgSetHanXinCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VERSION, m_comVersion);
	DDX_Control(pDX, IDC_COMBO_ERROR_LEVEL, m_comErrorLevel);
}


BEGIN_MESSAGE_MAP(CDlgSetHanXinCode, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSetHanXinCode::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSetHanXinCode ��Ϣ�������
BOOL CDlgSetHanXinCode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_comVersion.AddString(_T("Auto"));
	for( int i = 0;i<20;i++ )
	{
		CString str;
		str.Format(_T("%d"), i+1);
		m_comVersion.AddString(str);
	}
	m_comVersion.SetCurSel(m_nVersion);

	//
	m_comErrorLevel.AddString(_T("(L1)%8"));
	m_comErrorLevel.AddString(_T("(L2)%15"));
	m_comErrorLevel.AddString(_T("(L3)%23"));
	m_comErrorLevel.AddString(_T("(L4)%30"));
	m_comErrorLevel.SetCurSel(m_nErrorLevel-1);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CDlgSetHanXinCode::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	m_nVersion = m_comVersion.GetCurSel();
	m_nErrorLevel =  m_comErrorLevel.GetCurSel() + 1;

	OnOK();
}