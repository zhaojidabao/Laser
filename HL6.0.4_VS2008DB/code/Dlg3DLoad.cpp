// Dlg3DLoad.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "Dlg3DLoad.h"

#define BOOLTOFLAG(b) ((b)?SW_SHOW:SW_HIDE) 

// CDlg3DLoad 对话框

IMPLEMENT_DYNAMIC(CDlg3DLoad, CDialog)

CDlg3DLoad::CDlg3DLoad(HL_3DPARA* pPara, CWnd* pParent  /*= NULL*/)
	: CDialog(CDlg3DLoad::IDD, pParent)
	, m_iModule(0)
	, m_strSTLFile(_T(""))
	, m_dbX0(0)
	, m_dbY0(0)
	, m_dbX1(0)
	, m_dbY1(0)
	, m_nShape(0)
	, m_dbZ0(0)
	, m_dbZ1(0)
	, m_dbRadius(0)
	, m_dbRadius2(0)
	, m_dbLong(0)
	, m_bInside(FALSE)
	, m_dbWidth(0)
	, m_iILDirect(0)
	, m_bNew(TRUE)
	, m_dbSR(0)
	, m_dbER(0)
	, m_bVertical(FALSE)
{
	ASSERT(pPara);

	m_pPara = pPara;
}

CDlg3DLoad::~CDlg3DLoad()
{
}

void CDlg3DLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADI_3DSHAPE, m_iModule);
	DDX_Control(pDX, IDC_COMBO_3DBASE, m_cbxShape);
	DDX_Text(pDX, IDC_EDIT_3DFILE, m_strSTLFile);
	DDX_Text(pDX, IDC_EDIT_X0, m_dbX0);
	DDX_Text(pDX, IDC_EDIT_Y0, m_dbY0);
	DDX_Text(pDX, IDC_EDIT_X1, m_dbX1);
	DDX_Text(pDX, IDC_EDIT_Y1, m_dbY1);
	DDX_CBIndex(pDX, IDC_COMBO_3DBASE, m_nShape);
	DDX_Text(pDX, IDC_EDIT_Z0, m_dbZ0);
	DDX_Text(pDX, IDC_EDIT_Z1, m_dbZ1);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dbRadius);
	DDX_Text(pDX, IDC_EDIT_RADIUS2, m_dbRadius2);
	DDX_Text(pDX, IDC_EDIT_LONG, m_dbLong);
	DDX_Check(pDX, IDC_CHECK_MARKINSIDE, m_bInside);
	DDX_Control(pDX, IDC_LIST_STEP, m_listStep);
	DDX_Text(pDX, IDC_EDIT_STEPWIDTH, m_dbWidth);
	DDX_Radio(pDX, IDC_RADIO_ILDRIRECT, m_iILDirect);
	DDX_Check(pDX, IDC_CHECK_NEWOBJECT, m_bNew);
	DDX_Text(pDX, IDC_EDIT_STARTR, m_dbSR);
	DDX_Text(pDX, IDC_EDIT_ENDR, m_dbER);
	DDX_Check(pDX, IDC_CHECK_VERTICAL, m_bVertical);
}


BEGIN_MESSAGE_MAP(CDlg3DLoad, CDialog)
	ON_BN_CLICKED(IDC_RADI_3DSHAPE, &CDlg3DLoad::OnBnClickedRadi3dshape)
	ON_BN_CLICKED(IDC_RADIO2, &CDlg3DLoad::OnBnClickedRadi3dshape)
	ON_BN_CLICKED(IDC_RADIO3, &CDlg3DLoad::OnBnClickedRadi3dshape)
	ON_BN_CLICKED(IDC_BUTTON_3DFILESEL, &CDlg3DLoad::OnBnClickedButton3dfilesel)
	ON_CBN_SELCHANGE(IDC_COMBO_3DBASE, &CDlg3DLoad::OnCbnSelchangeCombo3dbase)
	ON_BN_CLICKED(IDC_BUTTON_ADDSTEP, &CDlg3DLoad::OnBnClickedButtonAddstep)
	ON_BN_CLICKED(IDC_BUTTON_DELSTEP, &CDlg3DLoad::OnBnClickedButtonDelstep)
	ON_BN_CLICKED(IDC_BUTTON_MODSTEP, &CDlg3DLoad::OnBnClickedButtonModstep)
	ON_BN_CLICKED(IDOK, &CDlg3DLoad::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STEP, &CDlg3DLoad::OnLvnItemchangedListStep)
END_MESSAGE_MAP()



BOOL CDlg3DLoad::OnInitDialog()
{
	CDialog::OnInitDialog();

	Layout();

	CString str;
	str.LoadString(IDS_MODELSTEP);
	m_cbxShape.AddString(str);
	str.LoadString(IDS_MODELINCLINE);
	m_cbxShape.AddString(str);
	str.LoadString(IDS_MODELCOLUMN);
	m_cbxShape.AddString(str);
	str.LoadString(IDS_MODELSPHERE);
	m_cbxShape.AddString(str);
	str.LoadString(IDS_MODELTAPER);
	m_cbxShape.AddString(str);
	m_cbxShape.SetCurSel(m_nShape);

	CRect rt;
	m_listStep.GetClientRect(rt); //rt.DeflateRect(10,0);

	str.LoadString(IDS_STEPPOSITION);
	m_listStep.InsertColumn(0, str, LVCFMT_CENTER, rt.Width()/5*3);
	str.LoadString(IDS_STEPLONGTH);
	m_listStep.InsertColumn(1, str, LVCFMT_CENTER, rt.Width()/5);
	str.LoadString(IDS_STEPWIDTH);
	m_listStep.InsertColumn(2, str, LVCFMT_CENTER, rt.Width()/5);
	m_listStep.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	Update_Data(FALSE);

	OnBnClickedRadi3dshape();
	OnCbnSelchangeCombo3dbase();



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CDlg3DLoad 消息处理程序

void CDlg3DLoad::OnBnClickedRadi3dshape()
{
	// TODO: 在此添加控件通知处理程序代码
	int iModule = m_iModule;
	UpdateData();

	if ( iModule  !=  m_iModule )
	{
		m_strSTLFile = "";
		UpdateData(FALSE);
	}

	m_cbxShape.EnableWindow(0 == m_iModule);

	GetDlgItem(IDC_STATIC_STEPWIDTH)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_STEPWIDTH)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_BUTTON_ADDSTEP)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_BUTTON_DELSTEP)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_BUTTON_MODSTEP)->EnableWindow(0 == m_iModule);
	m_listStep.EnableWindow(0 == m_iModule);

	GetDlgItem(IDC_STATIC_P0)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_P0X)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_X0)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_P0Y)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_Y0)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_P0Z)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_Z0)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_P1)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_P1X)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_X1)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_P1Y)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_Y1)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_P1Z)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_Z1)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_RADIO_ILDRIRECT)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_RADIO3)->EnableWindow(0 == m_iModule);

	GetDlgItem(IDC_STATIC_RADIUS)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_LONG)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_LONG)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_RADIUS2)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_RADIUS2)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_CHECK_MARKINSIDE)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_TIPTAPER)->EnableWindow(0 == m_iModule);

	GetDlgItem(IDC_STATIC_STARTR)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_STARTR)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_ENDR)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_EDIT_ENDR)->EnableWindow(0 == m_iModule);
	GetDlgItem(IDC_STATIC_TIPCOLUMN)->EnableWindow(0 == m_iModule);	

	GetDlgItem(IDC_EDIT_3DFILE)->EnableWindow(0 != m_iModule);
	GetDlgItem(IDC_BUTTON_3DFILESEL)->EnableWindow(0 != m_iModule);
}

void CDlg3DLoad::OnBnClickedButton3dfilesel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CFileDialog dlg(TRUE, (1 == m_iModule) ? _T("STL or DXF or STP or IGES") : _T("MAP"), NULL,OFN_HIDEREADONLY,
						( 1 == m_iModule) ? _T("STL Files(*.stl)|*.stl|DXF Files(*.dxf)|*.dxf|STEP Files(*.step)|*.step|STP Files(*.stp)|*.stp|IGES Files(*.iges)|*.iges|IGS Files(*.igs)|*.igs||") : _T("MAP Files(*.map)|*.map||")  );
	if ( dlg.DoModal()  !=  IDOK )
		return;

	m_strSTLFile = dlg.GetPathName();
	UpdateData(FALSE);
}


void CDlg3DLoad::OnCbnSelchangeCombo3dbase()
{
	UpdateData();

	m_listStep.ShowWindow(BOOLTOFLAG(0 == m_nShape));
	GetDlgItem(IDC_STATIC_STEPWIDTH)->ShowWindow(BOOLTOFLAG(0 == m_nShape));
	GetDlgItem(IDC_EDIT_STEPWIDTH)->ShowWindow(BOOLTOFLAG(0 == m_nShape));
	GetDlgItem(IDC_BUTTON_ADDSTEP)->ShowWindow(BOOLTOFLAG(0 == m_nShape));
	GetDlgItem(IDC_BUTTON_DELSTEP)->ShowWindow(BOOLTOFLAG(0 == m_nShape));
	GetDlgItem(IDC_BUTTON_MODSTEP)->ShowWindow(BOOLTOFLAG(0 == m_nShape));	

	GetDlgItem(IDC_STATIC_P0)->ShowWindow(BOOLTOFLAG(0 == m_nShape||1 == m_nShape));
	GetDlgItem(IDC_STATIC_P0X)->ShowWindow(BOOLTOFLAG(0 == m_nShape||1 == m_nShape));
	GetDlgItem(IDC_EDIT_X0)->ShowWindow(BOOLTOFLAG(0 == m_nShape||1 == m_nShape));
	GetDlgItem(IDC_STATIC_P0Y)->ShowWindow(BOOLTOFLAG(0 == m_nShape||1 == m_nShape));
	GetDlgItem(IDC_EDIT_Y0)->ShowWindow(BOOLTOFLAG(0 == m_nShape||1 == m_nShape));
	GetDlgItem(IDC_STATIC_P0Z)->ShowWindow(BOOLTOFLAG(0 == m_nShape||1 == m_nShape));
	GetDlgItem(IDC_EDIT_Z0)->ShowWindow(BOOLTOFLAG(0 == m_nShape||1 == m_nShape));
	GetDlgItem(IDC_STATIC_P1)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_STATIC_P1X)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_EDIT_X1)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_STATIC_P1Y)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_EDIT_Y1)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_STATIC_P1Z)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_EDIT_Z1)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_RADIO_ILDRIRECT)->ShowWindow(BOOLTOFLAG(1 == m_nShape));
	GetDlgItem(IDC_RADIO3)->ShowWindow(BOOLTOFLAG(1 == m_nShape));

	GetDlgItem(IDC_STATIC_TIPCOLUMN)->ShowWindow(BOOLTOFLAG(2 == m_nShape));
	GetDlgItem(IDC_STATIC_STARTR)->ShowWindow(BOOLTOFLAG(2 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_EDIT_STARTR)->ShowWindow(BOOLTOFLAG(2 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_STATIC_ENDR)->ShowWindow(BOOLTOFLAG(2 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_EDIT_ENDR)->ShowWindow(BOOLTOFLAG(2 == m_nShape||4 == m_nShape));
		
	GetDlgItem(IDC_STATIC_RADIUS)->ShowWindow(BOOLTOFLAG(2 == m_nShape||3 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_EDIT_RADIUS)->ShowWindow(BOOLTOFLAG(2 == m_nShape||3 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_STATIC_LONG)->ShowWindow(BOOLTOFLAG(0 == m_nShape||2 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_EDIT_LONG)->ShowWindow(BOOLTOFLAG(0 == m_nShape||2 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_CHECK_MARKINSIDE)->ShowWindow(BOOLTOFLAG(3 == m_nShape||4 == m_nShape));
	GetDlgItem(IDC_STATIC_RADIUS2)->ShowWindow(BOOLTOFLAG(4 == m_nShape));
	GetDlgItem(IDC_EDIT_RADIUS2)->ShowWindow(BOOLTOFLAG(4 == m_nShape));
	GetDlgItem(IDC_CHECK_VERTICAL)->ShowWindow(BOOLTOFLAG(4 == m_nShape));
	GetDlgItem(IDC_STATIC_TIPTAPER)->ShowWindow(BOOLTOFLAG(4 == m_nShape));
}

void CDlg3DLoad::Layout()
{
	CRect rtPara;
	GetDlgItem(IDC_STATIC_PARA)->GetWindowRect(rtPara);
	ScreenToClient(rtPara);

	CRect rt = rtPara;
	rt.SetRect(0,0, rt.BottomRight().x+15, rt.BottomRight().y+75);
	MoveWindow(rt);
	CenterWindow();

	int nMx(0), nMy(0);
	//
	GetDlgItem(IDC_STATIC_STEP)->GetWindowRect(&rt); 
	ScreenToClient(rt);
	nMx = rtPara.left-rt.left;
	nMy = rtPara.top-rt.top;

	m_listStep.GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	m_listStep.MoveWindow(rt);

	GetDlgItem(IDC_STATIC_STEPWIDTH)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_STEPWIDTH)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_STEPWIDTH)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_STEPWIDTH)->MoveWindow(rt);

	GetDlgItem(IDC_BUTTON_ADDSTEP)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_BUTTON_ADDSTEP)->MoveWindow(rt);

	GetDlgItem(IDC_BUTTON_DELSTEP)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_BUTTON_DELSTEP)->MoveWindow(rt);

	GetDlgItem(IDC_BUTTON_MODSTEP)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_BUTTON_MODSTEP)->MoveWindow(rt);

	//
	GetDlgItem(IDC_STATIC_INCLINE)->GetWindowRect(&rt); 
	ScreenToClient(rt);
	nMx = rtPara.left-rt.left;
	nMy = rtPara.top-rt.top;

	GetDlgItem(IDC_STATIC_P0)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P0)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_P0X)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P0X)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_X0)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_X0)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_P0Y)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P0Y)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_Y0)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_Y0)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_P0Z)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P0Z)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_Z0)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_Z0)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_P1)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P1)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_P1X)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P1X)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_X1)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_X1)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_P1Y)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P1Y)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_Y1)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_Y1)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_P1Z)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_P1Z)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_Z1)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_Z1)->MoveWindow(rt);

	GetDlgItem(IDC_RADIO_ILDRIRECT)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_RADIO_ILDRIRECT)->MoveWindow(rt);

	GetDlgItem(IDC_RADIO3)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_RADIO3)->MoveWindow(rt);

	//
	GetDlgItem(IDC_STATIC_COLUMN)->GetWindowRect(&rt); 
	ScreenToClient(rt);
	nMx = rtPara.left-rt.left;
	nMy = rtPara.top-rt.top;

	GetDlgItem(IDC_STATIC_STARTR)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_STARTR)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_STARTR)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_STARTR)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_ENDR)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_ENDR)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_ENDR)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_ENDR)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_TIPCOLUMN)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_TIPCOLUMN)->MoveWindow(rt);

	//
	GetDlgItem(IDC_STATIC_TAPER)->GetWindowRect(&rt); 
	ScreenToClient(rt);
	nMx = rtPara.left-rt.left;
	nMy = rtPara.top-rt.top;

	GetDlgItem(IDC_STATIC_RADIUS)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_RADIUS)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_RADIUS)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_RADIUS)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_LONG)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_LONG)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_LONG)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_LONG)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_RADIUS2)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_RADIUS2)->MoveWindow(rt);

	GetDlgItem(IDC_EDIT_RADIUS2)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_EDIT_RADIUS2)->MoveWindow(rt);

	GetDlgItem(IDC_CHECK_MARKINSIDE)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_CHECK_MARKINSIDE)->MoveWindow(rt);

	GetDlgItem(IDC_CHECK_VERTICAL)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_CHECK_VERTICAL)->MoveWindow(rt);

	GetDlgItem(IDC_STATIC_TIPTAPER)->GetWindowRect(rt);
	ScreenToClient(rt);
	rt.OffsetRect(nMx, nMy);
	GetDlgItem(IDC_STATIC_TIPTAPER)->MoveWindow(rt);
}

void CDlg3DLoad::Update_Data(BOOL bSave)
{
	if (bSave)
	{
		UpdateData();

		m_pPara->iModule = m_iModule;
		m_pPara->strFile = (m_iModule == 0) ? "" : m_strSTLFile;
		m_pPara->nShape = (m_iModule == 0) ? m_nShape : 0;
		m_pPara->step.pos.RemoveAll();
		m_pPara->step.length.RemoveAll();
		m_pPara->step.width.RemoveAll();
		memset(&m_pPara->incline, 0, sizeof(INCLINE));
		memset(&m_pPara->column, 0, sizeof(COLUMN));
		memset(&m_pPara->sphere, 0, sizeof(SPHERE));
		memset(&m_pPara->taper, 0, sizeof(TAPER));

		if (0 == m_iModule)
		{
			switch (m_nShape)
			{
			case 0:	
				{
					int iPos = 0;
					POS_DOT dot;
					CString strValue;
					int n = m_listStep.GetItemCount();
					for	(int i  = 0; i < n; ++i)
					{
						strValue = m_listStep.GetItemText(i, 0);
						iPos = strValue.Find(_T(','));
						dot.x = _tstof(strValue.Mid(1, iPos-1));
						strValue.Delete(0, iPos+1);
						iPos = strValue.Find(_T(','));
						dot.y = _tstof(strValue.Mid(0, iPos));
						strValue.Delete(0, iPos+1);
						dot.z = _tstof(strValue);
						m_pPara->step.pos.Add(dot);

						strValue = m_listStep.GetItemText(i, 1);
						m_pPara->step.length.Add(_tstof(strValue));

						strValue = m_listStep.GetItemText(i, 2);
						m_pPara->step.width.Add(_tstof(strValue));
					}
				}
				break;
			case 1:
				{
					m_pPara->incline.p0.x = m_dbX0;
					m_pPara->incline.p0.y = m_dbY0;
					m_pPara->incline.p0.z = m_dbZ0;
					m_pPara->incline.p1.x = m_dbX1;
					m_pPara->incline.p1.y = m_dbY1;
					m_pPara->incline.p1.z = m_dbZ1;
					m_pPara->incline.nDirect = m_iILDirect;
				}
				break;
			case 2:
				{
					m_pPara->column.dbRadius = m_dbRadius;
					m_pPara->column.dbLong = m_dbLong;
					m_pPara->column.dbSR = m_dbSR;
					m_pPara->column.dbER = m_dbER;
				}
				break;
			case 3:
				{
					m_pPara->sphere.dbRadius = m_dbRadius;
					m_pPara->sphere.bInside = m_bInside;		
				}
				break;
			case 4:	
				{
					m_pPara->taper.dbLong = m_dbLong;
					m_pPara->taper.dbRadius0 = m_dbRadius;
					m_pPara->taper.dbRadius1 = m_dbRadius2;
					m_pPara->taper.dbSR = m_dbSR;
					m_pPara->taper.dbER = m_dbER;
					m_pPara->taper.bInside = m_bInside;
					m_pPara->taper.nDirect = m_bVertical;					
				}
				break;
			}
		}
	}
	else
	{
		m_iModule = m_pPara->iModule;
		m_strSTLFile = (m_iModule == 0) ? _T("") : m_pPara->strFile;
		m_nShape = (m_iModule == 0) ? m_pPara->nShape : 0;

		if ( 0 == m_iModule )
		{
			switch(m_nShape)
			{
			case 0:	
				{
					CString strValue;
					m_listStep.DeleteAllItems();
					int n = m_pPara->step.pos.GetCount();
					for( int i  = 0; i < n; ++i )
					{
						strValue.Format(_T("%d"), i);
						m_listStep.InsertItem( i, strValue );
						
						strValue.Format(_T("(%.3f,%.3f,%.3f)"), m_pPara->step.pos[i].x, m_pPara->step.pos[i].y, m_pPara->step.pos[i].z);
						m_listStep.SetItemText( i, 0, strValue );
						
						strValue.Format(_T("%.3f"), m_pPara->step.length[i]);
						m_listStep.SetItemText( i, 1, strValue );
						
						strValue.Format(_T("%.3f"), m_pPara->step.width[i]);
						m_listStep.SetItemText( i, 2, strValue );
					}
				}
				break;
			case 1:	
				{
					m_dbX0 = m_pPara->incline.p0.x;
					m_dbY0 = m_pPara->incline.p0.y;
					m_dbZ0 = m_pPara->incline.p0.z;
					m_dbX1 = m_pPara->incline.p1.x;
					m_dbY1 = m_pPara->incline.p1.y;
					m_dbZ1 = m_pPara->incline.p1.z;
					m_iILDirect = m_pPara->incline.nDirect;
				}
				break;
			case 2:
				{
					m_dbRadius = m_pPara->column.dbRadius;
					m_dbLong = m_pPara->column.dbLong;
					m_dbSR = m_pPara->column.dbSR;
					m_dbER = m_pPara->column.dbER;
				}
				break;
			case 3:
				{
					m_dbRadius = m_pPara->sphere.dbRadius;
					m_bInside = m_pPara->sphere.bInside;
				}
				break;
			case 4:	
				{
					m_dbLong = m_pPara->taper.dbLong;
					m_dbRadius = m_pPara->taper.dbRadius0;
					m_dbRadius2 = m_pPara->taper.dbRadius1;
					m_dbSR = m_pPara->taper.dbSR;
					m_dbER = m_pPara->taper.dbER;
					m_bInside = m_pPara->taper.bInside;
					m_bVertical = m_pPara->taper.nDirect;
				}
				break;
			}
		}

		UpdateData(FALSE);
	}
}


void CDlg3DLoad::OnBnClickedButtonAddstep()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( FALSE == UpdateData() )
		return;
	
	CString strValue;
	int n = m_listStep.GetItemCount();
	strValue.Format(_T("%d"), n);
	m_listStep.InsertItem(n, strValue);

	strValue.Format(_T("(%.3f,%.3f,%.3f)"), m_dbX0, m_dbY0, m_dbZ0);
	m_listStep.SetItemText(n, 0, strValue);

	strValue.Format(_T("%.3f"), m_dbLong);
	m_listStep.SetItemText(n, 1, strValue);

	strValue.Format(_T("%.3f"), m_dbWidth);
	m_listStep.SetItemText(n, 2, strValue);
}

void CDlg3DLoad::OnBnClickedButtonDelstep()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listStep.GetFirstSelectedItemPosition();
	if ( pos )
	{
		int iSel = m_listStep.GetNextSelectedItem(pos);
		m_listStep.DeleteItem(iSel);
	}	
}

void CDlg3DLoad::OnBnClickedButtonModstep()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	POSITION pos = m_listStep.GetFirstSelectedItemPosition();
	if (pos)
	{
		CString strValue;
		int iSel = m_listStep.GetNextSelectedItem(pos);

		strValue.Format(_T("(%.3f,%.3f,%.3f)"), m_dbX0, m_dbY0, m_dbZ0);
		m_listStep.SetItemText(iSel, 0, strValue);

		strValue.Format(_T("%.3f"), m_dbLong);
		m_listStep.SetItemText(iSel, 1, strValue);

		strValue.Format(_T("%.3f"), m_dbWidth);
		m_listStep.SetItemText(iSel, 2, strValue);
	}
}

void CDlg3DLoad::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	Update_Data();
	OnOK();
}

void CDlg3DLoad::OnLvnItemchangedListStep(NMHDR *pNMHDR, LRESULT *pResult)
{
	static int iLastItem = -1;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if (LVIF_STATE == pNMLV->uChanged && (pNMLV->uNewState & LVIS_SELECTED))  
	{  
		int nItem = pNMLV->iItem;  
		if (-1  !=  nItem)  
		{  
			UpdateData(TRUE);

			m_listStep.SetCheck(iLastItem, 0);
			m_listStep.SetCheck(nItem);
			iLastItem = nItem;

			int iPos(0);
			CString strValue;
			strValue = m_listStep.GetItemText(nItem, 0);
			iPos = strValue.Find(_T(','));
			m_dbX0 = _tstof(strValue.Mid(1, iPos-1));
			strValue.Delete(0, iPos+1);
			iPos = strValue.Find(_T(','));
			m_dbY0 = _tstof(strValue.Mid(0, iPos));
			strValue.Delete(0, iPos+1);
			m_dbZ0 = _tstof(strValue);

			strValue = m_listStep.GetItemText(nItem, 1);
			m_dbLong = _tstof(strValue);

			strValue = m_listStep.GetItemText(nItem, 2);
			m_dbWidth = _tstof(strValue);

			UpdateData(FALSE);
		}  
	}  
	
	*pResult = 0;
}
