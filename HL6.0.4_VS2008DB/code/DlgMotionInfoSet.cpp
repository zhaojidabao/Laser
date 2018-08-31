// DlgMotionInfoSet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgMotionInfoSet.h"
#include "DlgMotorMoveWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMotionInfoSet dialog


CDlgMotionInfoSet::CDlgMotionInfoSet(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgMotionInfoSet::IDD, pParent)
	, m_bHardOrgEnable(FALSE)
{
	//{{AFX_DATA_INIT(CDlgMotionInfoSet)
	m_nDelay = 0;
	m_dbEmptyTAcc = 0.5;
	m_dbHomeHighSpeed = 500.0;
	m_dbHomeLowSpeed = 100.0;
	m_dbHomeTAcc = 0.5;
	m_dbLineEmptySpeed = 0.0;
	m_dbLineWorkSpeed = 0.0;
	m_dbPitch = 5.0;
	m_nPPR = 2000;
	m_dbREmptySpeed = 0.0;
	m_dbRWorkSpeed = 0.0;
	m_dbWorkTAcc = 0.0;
	m_nMotorDire = 0;
	m_nAxisNo = 0;
	m_nAxisType = 1;
	m_dbMoveValue = 0.0;
	m_nMoveTestModel = 0;
	m_nMoveTestType = 0;
	m_bAutoSoft0 = FALSE;
	m_bEnabled = FALSE;
	//}}AFX_DATA_INIT

	m_nOldAxisNo = 0;
}


void CDlgMotionInfoSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMotionInfoSet)
	DDX_Control(pDX, IDC_COMBO_AxisType, m_comAxisType);
	DDX_Control(pDX, IDC_COMBO_AxisNo, m_comAxisNo);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nDelay);
	DDV_MinMaxInt(pDX, m_nDelay, 0, 99999);
	DDX_Text(pDX, IDC_EDIT_EmptyTAcc, m_dbEmptyTAcc);
	DDV_MinMaxDouble(pDX, m_dbEmptyTAcc, 1.e-002, 999999.);
	DDX_Text(pDX, IDC_EDIT_HomeHighSpeed, m_dbHomeHighSpeed);
	DDV_MinMaxDouble(pDX, m_dbHomeHighSpeed, 0.1, 9999999.);
	DDX_Text(pDX, IDC_EDIT_HomeLowSpeed, m_dbHomeLowSpeed);
	DDV_MinMaxDouble(pDX, m_dbHomeLowSpeed, 0.1, 99999.);
	DDX_Text(pDX, IDC_EDIT_HomeTAcc, m_dbHomeTAcc);
	DDV_MinMaxDouble(pDX, m_dbHomeTAcc, 1.e-002, 99999.);
	DDX_Text(pDX, IDC_EDIT_LineEmptySpeed, m_dbLineEmptySpeed);
	DDV_MinMaxDouble(pDX, m_dbLineEmptySpeed, 0.1, 99999.);
	DDX_Text(pDX, IDC_EDIT_LineWorkSpeed, m_dbLineWorkSpeed);
	DDV_MinMaxDouble(pDX, m_dbLineWorkSpeed, 0.1, 999999.);
	DDX_Text(pDX, IDC_EDIT_Pitch, m_dbPitch);
	DDV_MinMaxDouble(pDX, m_dbPitch, 0.5, 999999.);
	DDX_Text(pDX, IDC_EDIT_PPR, m_nPPR);
	DDV_MinMaxInt(pDX, m_nPPR, 10, 9999999);
	DDX_Text(pDX, IDC_EDIT_REmptySpeed, m_dbREmptySpeed);
	DDV_MinMaxDouble(pDX, m_dbREmptySpeed, 1., 999999.);
	DDX_Text(pDX, IDC_EDIT_RWorkSpeed, m_dbRWorkSpeed);
	DDV_MinMaxDouble(pDX, m_dbRWorkSpeed, 1., 999999.);
	DDX_Text(pDX, IDC_EDIT_WorkTAcc, m_dbWorkTAcc);
	DDV_MinMaxDouble(pDX, m_dbWorkTAcc, 1.e-002, 999999.);
	DDX_Radio(pDX, IDC_RADIO_MotorDire, m_nMotorDire);
	DDX_CBIndex(pDX, IDC_COMBO_AxisNo, m_nAxisNo);
	DDX_CBIndex(pDX, IDC_COMBO_AxisType, m_nAxisType);
	DDX_Text(pDX, IDC_EDIT_MOVEVALUE, m_dbMoveValue);
	DDX_Radio(pDX, IDC_RADIO_MOVEMODEL, m_nMoveTestModel);
	DDX_Radio(pDX, IDC_RADIO_MOVETYPE, m_nMoveTestType);
	DDX_Check(pDX, IDC_CHECK_AUTOSOFT0, m_bAutoSoft0);
	DDX_Check(pDX, IDC_CHECK_ENABLED, m_bEnabled);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_HARDORG, m_bHardOrgEnable);
}


BEGIN_MESSAGE_MAP(CDlgMotionInfoSet, CDialog)
	//{{AFX_MSG_MAP(CDlgMotionInfoSet)
	ON_CBN_SELCHANGE(IDC_COMBO_AxisNo, OnSelchangeCOMBOAxisNo)
	ON_CBN_SELCHANGE(IDC_COMBO_AxisType, OnSelchangeCOMBOAxisType)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_RADIO_MOVEMODEL, OnRadioMovemodel)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO_MOVETYPE, OnRadioMovetype)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_BUTTON_SETSOFT0, OnButtonSetsoft0)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_CHECK_ENABLED, OnCheckEnabled)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_HARDORG, &CDlgMotionInfoSet::OnBnClickedCheckHardorg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMotionInfoSet message handlers

BOOL CDlgMotionInfoSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	m_comAxisType.ResetContent();
	str.LoadString(IDS_MOTION_ROTATEAXIS);
	m_comAxisType.AddString(str);
	str.LoadString(IDS_MOTION_LINEAXIS);
	m_comAxisType.AddString(str);
	m_comAxisType.SetCurSel(1);

	m_nAxisNo = m_nOldAxisNo = 0;
	memset(m_bHomed,0,sizeof(BOOL)*4);
	UpdateDlgView(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMotionInfoSet::UpdateDlgView(BOOL bSave) 
{
	if ( bSave == TRUE )
	{
		UpdateData(TRUE);

		m_para[m_nOldAxisNo].nDelay				 =  m_nDelay;
		m_para[m_nOldAxisNo].dbEmptyTAcc		 =  m_dbEmptyTAcc;
		m_para[m_nOldAxisNo].dbHomeHighSpeed	 =  m_dbHomeHighSpeed;
		m_para[m_nOldAxisNo].dbHomeLowSpeed		 =  m_dbHomeLowSpeed;
		m_para[m_nOldAxisNo].dbHomeTAcc			 =  m_dbHomeTAcc;
		m_para[m_nOldAxisNo].dbLineEmptySpeed	 =  m_dbLineEmptySpeed;
		m_para[m_nOldAxisNo].dbLineWorkSpeed	 =  m_dbLineWorkSpeed;
		m_para[m_nOldAxisNo].dbPitch			 =  m_dbPitch;
		m_para[m_nOldAxisNo].nPPR				 =  m_nPPR;
		m_para[m_nOldAxisNo].dbREmptySpeed		 =  m_dbREmptySpeed;
		m_para[m_nOldAxisNo].dbRWorkSpeed		 =  m_dbRWorkSpeed;
		m_para[m_nOldAxisNo].dbWorkTAcc			 =  m_dbWorkTAcc;
		m_para[m_nOldAxisNo].nAxisType			 =  m_nAxisType;
		m_para[m_nOldAxisNo].bAutoHome			 =  m_bAutoSoft0;
		m_para[m_nOldAxisNo].bEnabled			 =  m_bEnabled;
		m_para[m_nOldAxisNo].bHardOrgEnable		 =  m_bHardOrgEnable;

		if ( m_nMotorDire == 0 )
			m_para[m_nOldAxisNo].nMotorDire		 =  1;
		else
			m_para[m_nOldAxisNo].nMotorDire		 =  -1;
	}
	else
	{
		m_nDelay			 =  m_para[m_nOldAxisNo].nDelay;
		m_dbEmptyTAcc		 =  m_para[m_nOldAxisNo].dbEmptyTAcc;
		m_dbHomeHighSpeed	 =  m_para[m_nOldAxisNo].dbHomeHighSpeed;
		m_dbHomeLowSpeed	 =  m_para[m_nOldAxisNo].dbHomeLowSpeed;
		m_dbHomeTAcc		 =  m_para[m_nOldAxisNo].dbHomeTAcc;
		m_dbLineEmptySpeed	 =  m_para[m_nOldAxisNo].dbLineEmptySpeed;
		m_dbLineWorkSpeed	 =  m_para[m_nOldAxisNo].dbLineWorkSpeed;
		m_dbPitch			 =  m_para[m_nOldAxisNo].dbPitch;
		m_nPPR				 =  m_para[m_nOldAxisNo].nPPR;
		m_dbREmptySpeed		 =  m_para[m_nOldAxisNo].dbREmptySpeed;
		m_dbRWorkSpeed		 =  m_para[m_nOldAxisNo].dbRWorkSpeed;
		m_dbWorkTAcc		 =  m_para[m_nOldAxisNo].dbWorkTAcc;
		m_nAxisType			 =  m_para[m_nOldAxisNo].nAxisType;
		m_bAutoSoft0		 =  m_para[m_nOldAxisNo].bAutoHome;
		m_bEnabled			 =  m_para[m_nOldAxisNo].bEnabled;
		m_bHardOrgEnable	 =  m_para[m_nOldAxisNo].bHardOrgEnable;

		if ( m_para[m_nOldAxisNo].nMotorDire == 1 )
			m_nMotorDire = 0;
		else
			m_nMotorDire = 1;
		
		UpdateData(FALSE);

		GetDlgItem(IDC_BUTTON_SETSOFT0)->EnableWindow(m_bHomed[m_nOldAxisNo]);
		UpdateView();

	}
}

void CDlgMotionInfoSet::OnSelchangeCOMBOAxisNo() 
{
	// TODO: Add your control notification handler code here
	UpdateDlgView(TRUE);
	
	m_nOldAxisNo = m_nAxisNo;
	UpdateDlgView(FALSE);
}

void CDlgMotionInfoSet::OnOK() 
{
	// TODO: Add extra validation here
	SaveMotionPara();
	
	CDialog::OnOK();
}

void CDlgMotionInfoSet::OnSelchangeCOMBOAxisType() 
{
	// TODO: Add your control notification handler code here
	UpdateView();
}

void CDlgMotionInfoSet::UpdateView()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_CHECK_HARDORG)->EnableWindow(m_bEnabled );
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow( m_bEnabled );

	GetDlgItem(IDC_EDIT_Pitch)->EnableWindow(m_bEnabled && !(m_nAxisType));
	GetDlgItem(IDC_EDIT_LineWorkSpeed)->EnableWindow(m_bEnabled && !(m_nAxisType));
	GetDlgItem(IDC_EDIT_LineEmptySpeed)->EnableWindow(m_bEnabled && !(m_nAxisType));
	GetDlgItem(IDC_EDIT_HomeHighSpeed)->EnableWindow(m_bEnabled && !(m_nAxisType));
	GetDlgItem(IDC_EDIT_HomeLowSpeed)->EnableWindow(m_bEnabled && !(m_nAxisType));
	GetDlgItem(IDC_EDIT_RWorkSpeed)->EnableWindow(m_bEnabled && m_nAxisType);
	GetDlgItem(IDC_EDIT_REmptySpeed)->EnableWindow(m_bEnabled && m_nAxisType);

	GetDlgItem(IDC_EDIT_PPR)->EnableWindow( m_bEnabled );
	GetDlgItem(IDC_EDIT_DELAY)->EnableWindow( m_bEnabled );
	GetDlgItem(IDC_EDIT_WorkTAcc)->EnableWindow( m_bEnabled );
	GetDlgItem(IDC_EDIT_EmptyTAcc)->EnableWindow( m_bEnabled );
	GetDlgItem(IDC_EDIT_HomeTAcc)->EnableWindow( m_bEnabled );
	GetDlgItem(IDC_RADIO_MotorDire)->EnableWindow( m_bEnabled );
	GetDlgItem(IDC_RADIO2)->EnableWindow( m_bEnabled );
	GetDlgItem(IDC_RADIO6)->EnableWindow( m_bEnabled && m_bHardOrgEnable );
	GetDlgItem(IDC_CHECK_AUTOSOFT0)->EnableWindow( m_bEnabled && m_bHardOrgEnable  );
}



void CDlgMotionInfoSet::OnButtonTest()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//
	CDlgMotorMoveWait dlg;
	dlg.SetData(m_nMoveTestModel, m_nAxisNo, m_dbMoveValue, m_nMoveTestType );
	dlg.DoModal();
	
	if ( dlg.m_bHome && 2 == m_nMoveTestModel )
	{
		m_bHomed[m_nAxisNo] = TRUE;
		GetDlgItem(IDC_BUTTON_SETSOFT0)->EnableWindow(TRUE);
	}
	
// 	AfxGetApp()->BeginWaitCursor();
// 	switch ( m_nMoveTestModel )
// 	{
// 	case 0://绝对
// 		g_Motion.AMove(m_nAxisNo,m_dbMoveValue,m_nMoveTestType);
// 		break;
// 
// 	case 1://相对
// 		g_Motion.RMove(m_nAxisNo,m_dbMoveValue,m_nMoveTestType);
// 		break;
// 
// 	case 2://回零
// 		if ( g_Motion.Home(m_nAxisNo,FALSE) )
// 		{
// 			m_bHomed[m_nAxisNo] = TRUE;
// 			GetDlgItem(IDC_BUTTON_SETSOFT0)->EnableWindow(TRUE);
// 		}
// 		break;
// 	}
// 	AfxGetApp()->EndWaitCursor();
}



void CDlgMotionInfoSet::OnRadioMovemodel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}



void CDlgMotionInfoSet::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


void CDlgMotionInfoSet::OnRadioMovetype() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CDlgMotionInfoSet::OnRadio5()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


void CDlgMotionInfoSet::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CDlgMotionInfoSet::OnButtonSetsoft0() 
{
	// TODO: Add your control notification handler code here
	g_DAT.SetAsSoftHome(m_nAxisNo);
}





void CDlgMotionInfoSet::SaveMotionPara()
{
	UpdateDlgView(TRUE);
	
	TCHAR szSection[50] = { 0 };
	for ( int i = 0;  i < 4; i++ )
	{
		_stprintf_s( szSection, 50,_T("Motion%d"), i );
		
		::g_sys.WriteProfileInt( szSection, _T("PPR"),		m_para[i].nPPR);
		::g_sys.WriteProfileDouble( szSection, _T("Pitch"),		m_para[i].dbPitch);
		::g_sys.WriteProfileInt( szSection, _T("Delay"),			m_para[i].nDelay);
		::g_sys.WriteProfileInt( szSection, _T("AxisType"),		m_para[i].nAxisType);
		::g_sys.WriteProfileInt( szSection, _T("MotorDire"),		m_para[i].nMotorDire);
		
		::g_sys.WriteProfileDouble( szSection, _T("LineEmptySpeed"),   m_para[i].dbLineEmptySpeed);
		::g_sys.WriteProfileDouble( szSection, _T("LineWorkSpeed"),   m_para[i].dbLineWorkSpeed);
		::g_sys.WriteProfileDouble( szSection, _T("HomeHighSpeed"),   m_para[i].dbHomeHighSpeed);
		::g_sys.WriteProfileDouble( szSection, _T("HomeLowSpeed"),   m_para[i].dbHomeLowSpeed);
		
		::g_sys.WriteProfileDouble( szSection, _T("REmptySpeed"),   m_para[i].dbREmptySpeed);
		::g_sys.WriteProfileDouble( szSection, _T("RWorkSpeed"),   m_para[i].dbRWorkSpeed);
		
		::g_sys.WriteProfileDouble( szSection, _T("EmptyTAcc"),   m_para[i].dbEmptyTAcc);
		::g_sys.WriteProfileDouble( szSection, _T("WorkTAcc"),   m_para[i].dbWorkTAcc);
		::g_sys.WriteProfileDouble( szSection, _T("HomeTAcc"),   m_para[i].dbHomeTAcc);
		
		::g_sys.WriteProfileInt( szSection, _T("AutoSoft0"),		m_para[i].bAutoHome);
		::g_sys.WriteProfileInt( szSection, _T("Enabled"),		m_para[i].bEnabled);
		::g_sys.WriteProfileInt( szSection, _T("HardOrgEnable"),		m_para[i].bHardOrgEnable);
		
		g_Motion.SetMotionPara(i, m_para[i]);
	}
}


void CDlgMotionInfoSet::OnButtonApply()
{
	// TODO: Add your control notification handler code here
	SaveMotionPara();
}



void CDlgMotionInfoSet::OnCheckEnabled()
{
	// TODO: Add your control notification handler code here
	UpdateView();
}


void CDlgMotionInfoSet::OnBnClickedCheckHardorg()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (FALSE == m_bHardOrgEnable && 2 == m_nMoveTestModel)
	{
		m_nMoveTestModel = 0;
		UpdateData(FALSE);
	}
	
	UpdateView();
}



