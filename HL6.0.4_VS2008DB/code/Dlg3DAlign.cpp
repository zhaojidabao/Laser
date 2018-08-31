// Dlg3DAlign.cpp : 实现文件
//

#include "stdafx.h"
#include "Dlg3DAlign.h"
#include "baseMacro.h"


// CDlg3DAlign 对话框

IMPLEMENT_DYNAMIC(CDlg3DAlign, CDialog)

CDlg3DAlign::CDlg3DAlign(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg3DAlign::IDD, pParent)
	, m_dbBaseX(0)
	, m_dbBaseY(0)
	, m_dbBaseZ(0)
{
	m_pBaseDoc = NULL;
}

CDlg3DAlign::~CDlg3DAlign()
{
}

void CDlg3DAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_dbBaseX);
	DDX_Text(pDX, IDC_EDIT_Y, m_dbBaseY);
	DDX_Text(pDX, IDC_EDIT_Z, m_dbBaseZ);
}


BEGIN_MESSAGE_MAP(CDlg3DAlign, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALIGN, &CDlg3DAlign::OnBnClickedButtonAlign)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlg3DAlign::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlg3DAlign 消息处理程序
void CDlg3DAlign::OnBnClickedButtonAlign()
{
	UpdateData(TRUE);
	if( m_pBaseDoc == NULL )
		return;

	double dbX = 0;
	double dbY = 0;
	double dbZ = 0;
	m_pBaseDoc->m_chain3d.GetCenterPos(dbX,dbY,dbZ);
	TRANSFORM tsf;
	tsf.bRelative = TRUE;

	tsf.dx = m_dbBaseX-dbX;
	tsf.dy = m_dbBaseY-dbY;
	tsf.dz = m_dbBaseZ-dbZ;

	AfxGetApp()->BeginWaitCursor();
	m_pBaseDoc->m_chain3d.Transform(tsf);
	m_pBaseDoc->SetModifiedFlag(TRUE);
	AfxGetApp()->EndWaitCursor();
}

BOOL CDlg3DAlign::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_pBaseDoc = GetCurActiveDocument();
	if( NULL == m_pBaseDoc )
	{
		CDialog::OnCancel();
		return FALSE;
	}

	m_pBaseDoc->m_chain3d.GetCenterPos(m_dbBaseX,m_dbBaseY,m_dbBaseZ);
	int n = int(m_dbBaseX*100);
	m_dbBaseX = n/100.0;
	n = int(m_dbBaseY*100);
	m_dbBaseY = n/100.0;
	n = int(m_dbBaseZ*100);
	m_dbBaseZ = n/100.0;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlg3DAlign::OnClose()
{
	CDialog::OnClose();
}

void CDlg3DAlign::OnBnClickedButtonClose()
{
	CDialog::EndDialog(IDOK);
}
