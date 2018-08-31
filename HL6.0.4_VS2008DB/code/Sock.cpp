// Sock.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "Sock.h"


// CSock 对话框

IMPLEMENT_DYNAMIC(CSock, CDialog)

CSock::CSock(CWnd* pParent /*=NULL*/)
	: CDialog(CSock::IDD, pParent)
	, m_bServer(FALSE)
	, m_strIP(_T(""))
	, m_nAddr(0)
	, m_bSameIP(FALSE)
	, m_bUsed(FALSE)
{

}

CSock::~CSock()
{
}

void CSock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_IS_SERVER, m_bServer);
	DDX_Text(pDX, IDC_EDIT1, m_strIP);
	DDX_Text(pDX, IDC_EDIT2, m_nAddr);
	DDX_Check(pDX, IDC_CHECK65, m_bSameIP);
	DDX_Check(pDX, IDC_CHECK_USE, m_bUsed);
}


BEGIN_MESSAGE_MAP(CSock, CDialog)
END_MESSAGE_MAP()


// CSock 消息处理程序
