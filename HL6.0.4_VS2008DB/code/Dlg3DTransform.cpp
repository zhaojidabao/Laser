// Dlg3DTransform.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "Dlg3DTransform.h"


// CDlg3DTransform 对话框

IMPLEMENT_DYNAMIC(CDlg3DTransform, CDialog)

CDlg3DTransform::CDlg3DTransform(CHLDoc* pDoc, CWnd* pParent  /*= NULL*/)
	: CDialog(CDlg3DTransform::IDD, pParent)
	, m_dbOffsetX(0)
	, m_dbOffsetY(0)
	, m_dbOffsetZ(0)
	, m_dbRotateZ(0)
	, m_iRefer(0)
	, m_iDirect(0)
	, m_dbToBaseZ(0)
	, m_nTransType(FALSE)
{
	m_pDoc = pDoc;
	m_bRotateXY = FALSE;
	
	//
	m_dbOffsetXOld = 0;
	m_dbOffsetYOld = 0;
	m_dbOffsetZOld = 0;
	m_dbRotateZOld = 0;
}

CDlg3DTransform::~CDlg3DTransform()
{
}

void CDlg3DTransform::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_3DOFFSETX, m_dbOffsetX);
	DDX_Text(pDX, IDC_EDIT_3DOFFSETY, m_dbOffsetY);
	DDX_Text(pDX, IDC_EDIT_3DOFFSETZ, m_dbOffsetZ);
	DDX_Text(pDX, IDC_EDIT_3DROTATEZ, m_dbRotateZ);
	DDX_Radio(pDX, IDC_RADIO1, m_iRefer);
	DDX_Radio(pDX, IDC_RADIO3, m_iDirect);
	DDX_Text(pDX, IDC_EDIT_TOBASEZ, m_dbToBaseZ);
	DDX_Radio(pDX, IDC_RADIO_TransType, m_nTransType);
}


BEGIN_MESSAGE_MAP(CDlg3DTransform, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CDlg3DTransform::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlg3DTransform::OnBnClickedRadio2)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlg3DTransform::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SETTOBASEZ, &CDlg3DTransform::OnBnClickedButtonSettobasez)
END_MESSAGE_MAP()


// CDlg3DTransform 消息处理程序

BOOL CDlg3DTransform::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SwitchRadio();
			
	GetDlgItem(IDC_RADIO4)->EnableWindow(m_bRotateXY);
	GetDlgItem(IDC_RADIO5)->EnableWindow(m_bRotateXY);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg3DTransform::OnBnClickedRadio1()
{
	UpdateData();
	SwitchRadio();
}

void CDlg3DTransform::OnBnClickedRadio2()
{
	UpdateData();
	SwitchRadio();
}

void CDlg3DTransform::SwitchRadio()
{
	m_pDoc->m_chain3d.GetViewPos(m_dbOffsetX, m_dbOffsetY, m_dbOffsetZ, m_dbRotateZ, m_dbToBaseZ, m_bRotateXY);
	if (0 == m_iRefer)
	{
		;
	}
	else
	{
		m_dbOffsetX = 0;
		m_dbOffsetY = 0;
		m_dbOffsetZ = 0;
		m_dbRotateZ = 0;
	}

	m_dbOffsetXOld = m_dbOffsetX;
	m_dbOffsetYOld = m_dbOffsetY;
	m_dbOffsetZOld = m_dbOffsetZ;
	m_dbRotateZOld = m_dbRotateZ;
	
	UpdateData(FALSE);
}


void CDlg3DTransform::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	TransPreprocess();
	Transforom();
	SwitchRadio();
}




void CDlg3DTransform::Transforom()
{
	TRANSFORM tsf;

	if ( 0 == m_iRefer )
	{
		tsf.bRelative = FALSE;
	}
	else
	{
		tsf.bRelative = TRUE;		
	}
	tsf.dx = m_dbOffsetX;
	tsf.dy = m_dbOffsetY;
	tsf.dz = m_dbOffsetZ;
	tsf.dbAngle = m_dbRotateZ;
	tsf.nDirect = m_iDirect;
		
	AfxGetApp()->BeginWaitCursor();
	m_pDoc->m_chain3d.Transform(tsf);
	m_pDoc->SetModifiedFlag(TRUE);
	AfxGetApp()->EndWaitCursor();
}

void CDlg3DTransform::OnBnClickedButtonSettobasez()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pDoc->m_chain3d.ClearToBaseZ();
	SwitchRadio();
}





//m_iRefer  0:绝对 1:相对
//m_iDirect 0:z, 1:x, 2:y
void CDlg3DTransform::TransPreprocess()
{
	UpdateData(TRUE);

	TRANSFORM tsf;
	tsf.nDirect = m_iDirect;
	tsf.dbAngle = m_dbRotateZ;

	if ( 0 == m_iRefer )
	{
		tsf.bRelative = FALSE;

		tsf.dx = m_dbOffsetX - m_dbOffsetXOld;
		tsf.dy = m_dbOffsetY - m_dbOffsetYOld;
		tsf.dz = m_dbOffsetZ - m_dbOffsetZOld;
	}
	else
	{
		tsf.bRelative = TRUE;
		tsf.dx = m_dbOffsetX;
		tsf.dy = m_dbOffsetY;
		tsf.dz = m_dbOffsetZ;
	}
	
	if ( 0 == m_nTransType )
	{
		m_pDoc->m_b3dTransForce = FALSE;
	}
	else if ( 1 == m_nTransType )
	{
		m_pDoc->m_b3dTransForce = TRUE;
		m_pDoc->TransPreprocess(tsf, 1);
	}
	else if ( 2 == m_nTransType )
	{
		m_pDoc->m_b3dTransForce = FALSE;
		m_pDoc->TransPreprocess(tsf, 2);
	}
}

