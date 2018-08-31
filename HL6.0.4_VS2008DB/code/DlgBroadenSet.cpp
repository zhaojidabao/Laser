// DlgBroadenSet.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgBroadenSet.h"


// CDlgBroadenSet 对话框

IMPLEMENT_DYNAMIC(CDlgBroadenSet, CDialog)

CDlgBroadenSet::CDlgBroadenSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBroadenSet::IDD, pParent)
	, m_dbLineWidth(0.1)
	, m_dbDistance(0.05)
{

}

CDlgBroadenSet::~CDlgBroadenSet()
{
}

void CDlgBroadenSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dbLineWidth);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_dbDistance);
	DDV_MinMaxDouble(pDX, m_dbLineWidth, 0.001, 1000);
	DDV_MinMaxDouble(pDX, m_dbDistance, 0.001, 1000);
}


BEGIN_MESSAGE_MAP(CDlgBroadenSet, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBroadenSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBroadenSet 消息处理程序

void CDlgBroadenSet::OnBnClickedOk()
{
	UpdateData(TRUE);
	OnOK();
}
