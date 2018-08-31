
// Pdf417Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "Pdf417Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdf417Dlg dialog


CPdf417Dlg::CPdf417Dlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CPdf417Dlg::IDD, pParent)
	, m_nErrorLevel(0)
{
	//{{AFX_DATA_INIT(CPdf417Dlg)
	m_nLineNum = 0;
	//}}AFX_DATA_INIT
}


void CPdf417Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPdf417Dlg)
	DDX_Text(pDX, IDC_EDIT_LINENUM, m_nLineNum);
	DDV_MinMaxUInt(pDX, m_nLineNum, 0, 90);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_ERROR, m_comError);
	DDX_CBIndex(pDX, IDC_COMBO_ERROR, m_nErrorLevel);
}

BEGIN_MESSAGE_MAP(CPdf417Dlg, CDialog)
	//{{AFX_MSG_MAP(CPdf417Dlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPdf417Dlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdf417Dlg message handlers

void CPdf417Dlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CPdf417Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for( int i = 0; i<9; i++ )
	{
		CString str;
		str.Format(_T("L%d"), i);
		m_comError.AddString(str);
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPdf417Dlg::OnBnClickedOk()
{
	//TODO: 在此添加控件通知处理程序代码
	OnOK();
}
