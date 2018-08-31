// Objecteditfill.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "Objecteditfill.h"
#include "DlgAddFillSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjecteditfill dialog


CObjecteditfill::CObjecteditfill(CWnd* pParent  /*= NULL*/)
	: CDialog(CObjecteditfill::IDD, pParent)
	, m_bAlign(FALSE)
{
	//{{AFX_DATA_INIT(CObjecteditfill)
	m_nFillNo = 0;
	m_bEquaDist = FALSE;
	m_bKeepAngle = FALSE;
	m_dbSpace = 0.01;
	m_bEnable = FALSE;
	m_bLoop = FALSE;
	m_dbAngle = 0.0;
	m_dbBeamComp = 0.0;
	m_dbLineReduce = 0.0;
	m_dbLineReduceEnd = 0.0;
	m_nLoopNum = 0;
	m_dbStartOffset = 0.0;
	m_dbEndOffset = 0.0;
	m_dbMinJump = 0.0;
	m_bRemoveOrg = FALSE;
	m_dbLoopDist = 0.0;
	m_bRemoveFill = FALSE;
	m_nFillStyle = 0;
	m_bFillFirst = FALSE;
	//}}AFX_DATA_INIT
	m_nPrevFillNo = 0;

	m_nFillNum = 1;
	m_dbAddDis = 0;
	m_dbAddAngle = 0;
	m_bSaveCountour = FALSE;
}


void CObjecteditfill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjecteditfill)
	DDX_Radio(pDX, IDC_RADIO_FILL1, m_nFillNo);
	DDX_Check(pDX, IDC_CHECK_EQUADIST, m_bEquaDist);
	DDX_Check(pDX, IDC_CHECK_KEEPANGLE, m_bKeepAngle);
	DDX_Text(pDX, IDC_EDIT_SPACE, m_dbSpace);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnable);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bLoop);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_dbAngle);
	DDX_Text(pDX, IDC_EDIT_BEAMCOMP, m_dbBeamComp);
	DDX_Text(pDX, IDC_EDIT_LINEREDU, m_dbLineReduce);
	DDX_Text(pDX, IDC_EDIT_LINEREDU_END, m_dbLineReduceEnd);
	DDX_Text(pDX, IDC_EDIT_LOOPNUM, m_nLoopNum);
	DDX_Text(pDX, IDC_EDIT_STARTOFFSET, m_dbStartOffset);
	DDX_Text(pDX, IDC_EDIT_ENDOFFSET, m_dbEndOffset);
	DDX_Text(pDX, IDC_EDIT_MINJUMP, m_dbMinJump);
	DDX_Check(pDX, IDC_CHECK_REMOVEORG, m_bRemoveOrg);
	DDX_Text(pDX, IDC_EDIT_LOOPDIST, m_dbLoopDist);
	DDX_Check(pDX, IDC_CHECK_REMOVEFILL, m_bRemoveFill);
	DDX_Check(pDX, IDC_CHECK_FILLFIRST, m_bFillFirst);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_ALIGN, m_bAlign);
}


BEGIN_MESSAGE_MAP(CObjecteditfill, CDialog)
	//{{AFX_MSG_MAP(CObjecteditfill)
	ON_BN_CLICKED(IDC_BUTTON_STYLE, OnButtonStyle)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, OnCheckEnable)
	ON_BN_CLICKED(IDC_CHECK_LOOP, OnCheckLoop)
	ON_BN_CLICKED(IDC_RADIO_FILL1, OnRadioFill1)
	ON_BN_CLICKED(IDC_RADIO_FILL2, OnRadioFill2)
	ON_BN_CLICKED(IDC_CHECK_FILLFIRST, OnCheckFillfirst)
	ON_BN_CLICKED(IDC_BUTTON_ADDFILLSET, OnButtonAddfillset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjecteditfill message handlers

BOOL CObjecteditfill::OnInitDialog()
{	
	CDialog::OnInitDialog();
	
	HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON_LOOP);
	((CButton*)GetDlgItem(IDC_CHECK_LOOP))->SetIcon(hIcon);
	
	///
	SetFillData(0);
	m_nPrevFillNo = m_nFillNo = 0;
	
	UpdateDataView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
void CObjecteditfill::UpdateDataView()
{	
	GetDlgItem(IDC_EDIT_LOOPNUM)->EnableWindow(m_bLoop && m_bEnable);
	GetDlgItem(IDC_EDIT_LOOPDIST)->EnableWindow(m_bLoop && m_bEnable);
	GetDlgItem(IDC_CHECK_REMOVEORG)->EnableWindow(m_bLoop && m_bEnable);
	GetDlgItem(IDC_CHECK_REMOVEFILL)->EnableWindow(m_bLoop && m_bEnable);
	
	//
	GetDlgItem(IDC_CHECK_EQUADIST)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_EDIT_SPACE)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_EDIT_ANGLE)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_EDIT_MINJUMP)->EnableWindow( m_bEnable && (m_nFillStyle == 1 || m_nFillStyle == 3 ));
	GetDlgItem(IDC_EDIT_STARTOFFSET)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_EDIT_ENDOFFSET)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_EDIT_LINEREDU)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_EDIT_LINEREDU_END)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_EDIT_BEAMCOMP)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_CHECK_LOOP)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_BUTTON_STYLE)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_BUTTON_ADDFILLSET)->EnableWindow( m_bEnable);
	GetDlgItem(IDC_CHECK_ALIGN)->EnableWindow( m_nFillStyle == 0 || m_nFillStyle == 2 );
		
	HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON_FILLDIRE1);
	switch ( m_nFillStyle )
	{
	case 1:
		hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON_FILLDIRE2);
		break;
		
	case 2:
		hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON_FILLREVE1);
		break;
		
	case 3:
		hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON_FILLREVE2);
		break;
		
	case 4:
		hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON_FILLLOOP);
		break;
	}
	((CButton*)GetDlgItem(IDC_BUTTON_STYLE))->SetIcon(hIcon);
}

//
void CObjecteditfill::OnButtonStyle() 
{
	// TODO: Add your control notification handler code here
	m_nFillStyle ++;
//	if ( m_nFillStyle  >=  5)
	if ( m_nFillStyle  >=  4)
		m_nFillStyle = 0;
	
	UpdateDataView();
}


//
void CObjecteditfill::OnCheckEnable()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	/////
	UpdateDataView();
}


//
void CObjecteditfill::OnCheckLoop() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	/////
	UpdateDataView();
}


//
void CObjecteditfill::OnRadioFill1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	/////
	if ( m_nPrevFillNo == m_nFillNo )
	{
		SetFillData(m_nFillNo,TRUE);
		return;
	}
	
	SetFillData(m_nPrevFillNo,TRUE);
	SetFillData(m_nFillNo);
	m_nPrevFillNo = m_nFillNo;
	UpdateDataView();
}


//
void CObjecteditfill::OnRadioFill2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	/////
	if ( m_nPrevFillNo == m_nFillNo )
	{
		SetFillData(m_nFillNo,TRUE);
		return;
	}
	
	SetFillData(m_nPrevFillNo,TRUE);
	SetFillData(m_nFillNo);
	m_nPrevFillNo = m_nFillNo;
	UpdateDataView();
}


//
void CObjecteditfill::SetFillData(int nNo, BOOL bSave)
{
	NEW_FILL_DATA data;
	//
	if ( bSave )
	{
		data.bEnable	 =  m_bEnable;
		data.nFillStyle = m_nFillStyle;
		data.bEquaDist	 =  m_bEquaDist;
		data.bKeepAngle	 =  m_bKeepAngle;
		data.dbSpace	 =  m_dbSpace;
		data.dbAngle	 =  m_dbAngle;
		data.dbBeamComp	 =  m_dbBeamComp;
		data.dbLineReduce = m_dbLineReduce;
		data.dbLineReduceEnd = m_dbLineReduceEnd;
		data.dbStartOffset = m_dbStartOffset;
		data.dbEndOffset   = m_dbEndOffset;
		data.dbMinJump	 =  m_dbMinJump;
		
		data.bLoop		 =  m_bLoop;
		data.nLoopNum	 =  m_nLoopNum;
		data.dbLoopDist = m_dbLoopDist;
		data.bRemoveOrg	 =  m_bRemoveOrg;
		data.bRemoveLine = m_bRemoveFill;

		data.nFillNum = m_nFillNum;
		data.AddDis   = m_dbAddDis;
		data.ProAngle = m_dbAddAngle;
		data.bSaveCountour = m_bSaveCountour;
		
		if ( nNo == 0 )
			m_fillData1 = data;
		else
			m_fillData2 = data;
	}
	else
	{	
		if ( nNo == 0 )
			data = m_fillData1;
		else
			data = m_fillData2;
		
		m_bEnable	 		 =  data.bEnable;
		m_nFillStyle		 =  data.nFillStyle;
		m_bEquaDist			 =  data.bEquaDist;
		m_bKeepAngle		 =  data.bKeepAngle;
		m_dbSpace			 =  data.dbSpace;
		m_dbAngle			 =  data.dbAngle;
		m_dbBeamComp		 =  data.dbBeamComp;
		m_dbLineReduce		 =  data.dbLineReduce;
		m_dbLineReduceEnd	 =  data.dbLineReduceEnd;
		m_dbStartOffset		 =  data.dbStartOffset;
		m_dbEndOffset		 =  data.dbEndOffset;
		m_dbMinJump			 =  data.dbMinJump;

		//
		m_bLoop				 =  data.bLoop;
		m_nLoopNum			 =  data.nLoopNum;
		m_dbLoopDist		 =  data.dbLoopDist;
		m_bRemoveOrg		 =  data.bRemoveOrg;
		m_bRemoveFill       = data.bRemoveLine;

		m_nFillNum          = data.nFillNum;
		m_dbAddDis          = data.AddDis;
		m_dbAddAngle        = data.ProAngle;
		m_bSaveCountour     = data.bSaveCountour;
		UpdateData(FALSE);
	}
}

//
void CObjecteditfill::GetNewFillData(FILLDATA &filldata)
{
	UpdateData(TRUE);
	if ( m_dbSpace < FILLMINDIS )
	{
		m_dbSpace = 0;
	}	
	
	SetFillData(m_nFillNo, TRUE);
	filldata.newFileData1 = m_fillData1;
	filldata.newFileData2 = m_fillData2;
	filldata.bFillFirst = m_bFillFirst;
	filldata.bFillAlign = m_bAlign;
}

//
void CObjecteditfill::DisplyFillProp(FILLDATA &filldata)
{
	m_fillData1 = filldata.newFileData1;
	m_fillData2 = filldata.newFileData2;	
	m_bFillFirst = filldata.bFillFirst;
	m_bAlign     = filldata.bFillAlign;
	
	SetFillData(m_nPrevFillNo);
	UpdateDataView();
}

void CObjecteditfill::OnCheckFillfirst() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CObjecteditfill::OnButtonAddfillset() 
{
	// TODO: Add your control notification handler code here
	g_sys.m_bEnableUpdate = FALSE;
	CDlgAddFillSet dlg;
		
	dlg.m_nNum       = m_nFillNum;
	dlg.m_dbAddDis   = m_dbAddDis;
	dlg.m_dbAddAngle = m_dbAddAngle;
	dlg.m_bSaveCountour = m_bSaveCountour;
	
	if (dlg.DoModal() == IDOK)
	{
		m_nFillNum = dlg.m_nNum;
		m_dbAddDis   = dlg.m_dbAddDis;
		m_dbAddAngle = dlg.m_dbAddAngle;
		m_bSaveCountour = dlg.m_bSaveCountour;
	}

	SetFillData(m_nPrevFillNo,TRUE);
}
