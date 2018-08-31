// MoveAccuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MoveAccuDlg.h"
#include "HLView.h"
#include "MainFrm.h"
#include "baseMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern  CHLApp	theApp;
/////////////////////////////////////////////////////////////////////////////
// CMoveAccuDlg dialog
 
CMoveAccuDlg::CMoveAccuDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CMoveAccuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoveAccuDlg)
	m_dbMoveX = 0.0;
	m_dbMoveY = 0.0;
	m_dbMoveStep = 0.0;
	//}}AFX_DATA_INIT
}


void CMoveAccuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveAccuDlg)
	DDX_Text(pDX, IDC_MOVEX, m_dbMoveX);
	DDX_Text(pDX, IDC_MOVEY, m_dbMoveY);
	DDX_Text(pDX, IDC_MOVE_STEP_VALUE, m_dbMoveStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveAccuDlg, CDialog)
	//{{AFX_MSG_MAP(CMoveAccuDlg)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, OnButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DWON, OnButtonMoveDwon)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_LEFT, OnButtonMoveLeft)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_RIGTH, OnButtonMoveRigth)
    ON_BN_CLICKED(IDC_BUTTON_SELMARK, OnButtonSelMark)
	ON_WM_CANCELMODE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SELMARK_END,OnSelMarkEnd)
	ON_BN_CLICKED(IDOK, &CMoveAccuDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveAccuDlg message handlers

BOOL CMoveAccuDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dbMoveStep = g_SysCfg.m_dbDotMoveStepLen;
	UpdateData(FALSE);

#ifdef _SOFTMODE_EDITOR_
	GetDlgItem(IDC_BUTTON_SELMARK)->EnableWindow(FALSE);
#endif
	return TRUE;
}


void CMoveAccuDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	StopMark();
	g_SysCfg.m_dbDotMoveStepLen = m_dbMoveStep;

	CDialog::OnOK();
}


void CMoveAccuDlg::OnButtonMoveUp()
{
	UpdateData(TRUE);
	
	CHLView *pView = (CHLView*)GetCurActiveView();
	if ( pView == NULL )
		return;
	
	pView->m_track.InverDrawDot(this);
	m_dbMoveY  +=  m_dbMoveStep;
	double dbMoveY = MyPrecision(m_dbMoveStep);

	CHLDoc *pDoc = pView->GetDocument();
	pDoc->MoveAccurate(0, dbMoveY);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);
	pView->Invalidate();
}




void CMoveAccuDlg::OnButtonMoveDwon() 
{
	UpdateData(TRUE);
	
	CHLView *pView = (CHLView*)GetCurActiveView();
	if ( pView == NULL )
		return;
	
	pView->m_track.InverDrawDot(this);
	m_dbMoveY  -=  m_dbMoveStep;

	double dbMoveY = MyPrecision(-m_dbMoveStep);
	CHLDoc *pDoc = (CHLDoc*)pView->GetDocument();
	pDoc->MoveAccurate(0, dbMoveY);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);

	pView->Invalidate();
}

void CMoveAccuDlg::OnButtonMoveLeft() 
{
	UpdateData(TRUE);
	
	CHLView *pView = (CHLView*)GetCurActiveView();
	if ( pView == NULL )
		return;

	pView->m_track.InverDrawDot(this);
	m_dbMoveX  -=  m_dbMoveStep;
	double dbMoveX = MyPrecision(-m_dbMoveStep);
	
	CHLDoc *pDoc = (CHLDoc*)pView->GetDocument();
	pDoc->MoveAccurate(dbMoveX, 0);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);

	pView->Invalidate();
}

void CMoveAccuDlg::OnButtonMoveRigth() 
{
	UpdateData(TRUE);

	CHLView *pView = (CHLView*)GetCurActiveView();
	if ( pView == NULL )
		return;

	pView->m_track.InverDrawDot(this);
	m_dbMoveX  +=  m_dbMoveStep;
	double dbMoveX = MyPrecision(m_dbMoveStep);

	CHLDoc *pDoc = (CHLDoc*)pView->GetDocument();

	pDoc->MoveAccurate(dbMoveX, 0);
	pDoc->SetTrackerQuad(pDoc->GetLimit());

	UpdateData(FALSE);

	pView->Invalidate();
}

void CMoveAccuDlg::OnButtonSelMark() 
{
	if (!g_bIsSelMark)
		StartMark();
	else
		StopMark();
}

void CMoveAccuDlg::OnDestroy()
{
	StopMark();
	
	CDialog::OnDestroy();
}

LRESULT CMoveAccuDlg::OnSelMarkEnd(WPARAM wParam,LPARAM lParam)
{
	if (g_bIsSelMark)
	{
		g_DAT.LeaveMarkEnv();
		g_bIsSelMark = FALSE;
	}
	
	double dbSingleTime = g_DAT.GetMarkTime();
	CMainFrame *pMFrame = (CMainFrame*)AfxGetMainWnd();
	if ( pMFrame  !=  NULL )
		pMFrame->SetMarkTime(dbSingleTime);
	
	return 0;
}



void CMoveAccuDlg::StopMark()
{
	if ( g_bIsSelMark )
	{
		theApp.m_SelMark.StopSelMark();
		g_bIsSelMark = FALSE;
	}

	if ( g_bIsRedPreview )
	{
		theApp.m_RedPreView.StopPreview();
		g_bIsRedPreview = FALSE;
	}

	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
		g_bIsFastMark = FALSE;
	}
}



void CMoveAccuDlg::StartMark()
{
	if ( g_bIsSelMark )
	{
		theApp.m_SelMark.StopSelMark();
	}

	if ( g_bIsRedPreview  ) 
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if ( !g_DAT.IsCheckStatusOK() )
		return;

	CMainFrame *pMainWnd = (CMainFrame*)AfxGetMainWnd();
	if ( NULL == pMainWnd )
		return;

	int nCount   = pMainWnd->m_PropDlg.m_listLayer.GetItemCount();
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	int nCurSel = pMainWnd->m_PropDlg.m_listLayer.GetCurSel();
	if ( ( nCurSel < 0 ) || ( nCurSel  >=  nCount ) )
		return;

	CLayer ly = pDoc->GetLayer(nCurSel);
	CLay lay;
	lay.Add(ly);
	g_DAT.SetLay(lay);

	SUBOBJMARKSET stSub;
	stSub.bIsFillFirst = TRUE;
	stSub.bIsMarkFill  = TRUE;
	stSub.bIsMarkFram  = TRUE;
	stSub.nLayer       = 0;
	stSub.nSubCnt      = 1;

	theApp.m_SelMark.StartSelMark(this->m_hWnd, stSub, ENU_MARKSPECPARASET);

	g_bIsSelMark = TRUE;
}
void CMoveAccuDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
