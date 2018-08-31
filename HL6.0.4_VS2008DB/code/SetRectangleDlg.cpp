
// SetRectangleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetRectangleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRectangleDlg dialog


CSetRectangleDlg::CSetRectangleDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CSetRectangleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRectangleDlg)
	m_dbValue = 0.5;
	m_nType = 0;
	//}}AFX_DATA_INIT
}


void CSetRectangleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRectangleDlg)
	DDX_Text(pDX, IDC_VALUE, m_dbValue);
	DDV_MinMaxDouble(pDX, m_dbValue, 0.005, 99999.);
	DDX_Radio(pDX, IDC_RADIO_NORMAL, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRectangleDlg, CDialog)
	//{{AFX_MSG_MAP(CSetRectangleDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_VALUE, &CSetRectangleDlg::OnEnChangeValue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRectangleDlg message handlers

void CSetRectangleDlg::OnEnChangeValue()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
