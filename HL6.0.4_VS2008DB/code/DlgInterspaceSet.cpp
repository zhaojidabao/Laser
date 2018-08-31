// DlgInterspaceSet.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgInterspaceSet.h"


// CDlgInterspaceSet 对话框

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


// CDlgInterspaceSet 消息处理程序

void CDlgInterspaceSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}
