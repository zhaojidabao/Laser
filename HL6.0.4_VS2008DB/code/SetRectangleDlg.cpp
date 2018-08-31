
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
