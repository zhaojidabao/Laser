// DlgInterspaceSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgInterspaceSet.h"


// CDlgInterspaceSet �Ի���

IMPLEMENT_DYNAMIC(CDlgInterspaceSet, CDialog)

CDlgInterspaceSet::CDlgInterspaceSet(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgInterspaceSet::IDD, pParent)
	, m_dbInterSpace(0.1)
{

}

CDlgInterspaceSet::~CDlgInterspaceSet()
{
}

void CDlgInterspaceSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dbInterSpace);
}


BEGIN_MESSAGE_MAP(CDlgInterspaceSet, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInterspaceSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInterspaceSet ��Ϣ�������

void CDlgInterspaceSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	OnOK();
}
