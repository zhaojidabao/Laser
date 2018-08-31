// DlgRunWarn.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgRunWarn.h"


// CDlgRunWarn 对话框

IMPLEMENT_DYNAMIC(CDlgRunWarn, CDialog)

CDlgRunWarn::CDlgRunWarn(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRunWarn::IDD, pParent)
{

}

CDlgRunWarn::~CDlgRunWarn()
{
}

void CDlgRunWarn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRunWarn, CDialog)
END_MESSAGE_MAP()


// CDlgRunWarn 消息处理程序

BOOL CDlgRunWarn::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strMsg;
	strMsg.LoadString(IDS_INSTALLATION_FAULT);
	SetDlgItemText(IDC_STATIC_TIP,strMsg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgRunWarn::PostNcDestroy()
{

	CDialog::PostNcDestroy();
	delete this;
}
