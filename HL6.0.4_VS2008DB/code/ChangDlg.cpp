// CChangDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ChangDlg.h"
#include "SelectCoordinary.h"
#include "HLView.h"
#include "MakeCopySet.h"
#include "ExternViewDlg.h"
#include "baseMacro.h"
#include "DlgChangeOther.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangDlg dialog


CChangDlg::CChangDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CChangDlg::IDD, pParent)
	, m_dbZ(0)
	, m_nRotatePlane(0)
{	
	//{{AFX_DATA_INIT(CChangDlg)
	m_dbX = 0.0;
	m_dbY = 0.0;
	m_dbRotate = 0.0;
	m_bProrata = TRUE;
	m_strSizeX = _T("");
	m_strSizeY = _T("");
	m_dbPropX = 100.0;
	m_dbPropY = 100.0;
	m_nBase = 4;
	m_dbSizeX  = 0.0;
	m_dbSizeY  = 0.0;
	//}}AFX_DATA_INIT
	
	m_dbShearX = 0.0;
	m_dbShearY = 0.0;
}


void CChangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangDlg)
	DDX_Text(pDX, IDC_POSX, m_dbX);
	DDX_Text(pDX, IDC_POSY, m_dbY);
	DDX_Text(pDX, IDC_ROTATE, m_dbRotate);
	DDX_Check(pDX, IDC_CHECK_SCALE, m_bProrata);
	DDX_Text(pDX, IDC_SIZEX, m_strSizeX);
	DDX_Text(pDX, IDC_SIZEY, m_strSizeY);
	DDX_Text(pDX, IDC_PROPEX, m_strPropX);
	DDX_Text(pDX, IDC_PROPEY, m_strPropY);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_POSZ, m_dbZ);
	DDX_Radio(pDX, IDC_RADIO1, m_nRotatePlane);
}


BEGIN_MESSAGE_MAP(CChangDlg, CDialog)
	//{{AFX_MSG_MAP(CChangDlg)
	ON_BN_CLICKED(IDOK, OnApply)
	ON_EN_CHANGE(IDC_PROPEX, OnChangePropX)
	ON_EN_CHANGE(IDC_PROPEY, OnChangePropY)
	ON_EN_CHANGE(IDC_ROTATE, OnChangeRotate)
	ON_BN_CLICKED(IDC_BUTT_SELPOS, OnButtSelpos)
	ON_BN_CLICKED(IDC_MAKECOPY, OnMakecopy)
	ON_EN_CHANGE(IDC_SIZEX, OnChangeSizeX)
	ON_EN_CHANGE(IDC_SIZEY, OnChangeSizeY)
	ON_BN_CLICKED(IDC_CHECK_SCALE, OnCheckScale)
    ON_BN_CLICKED(IDC_CHECK_CENTERPOS, OnCheckPOS)
	ON_BN_CLICKED(IDC_MAKECOPY_SET, OnMakecopySet)
	ON_EN_CHANGE(IDC_POSX, OnChangePosx)
	ON_EN_CHANGE(IDC_POSY, OnChangePosy)
	ON_EN_SETFOCUS(IDC_POSX, OnSetfocusPosx)
	ON_EN_SETFOCUS(IDC_POSY, OnSetfocusPosy)
	ON_EN_SETFOCUS(IDC_ROTATE, OnSetfocusRotate)
	ON_EN_SETFOCUS(IDC_SIZEX, OnSetfocusSizex)
	ON_EN_SETFOCUS(IDC_SIZEY, OnSetfocusSizey)
	ON_EN_SETFOCUS(IDC_PROPEX, OnSetfocusPropex)
	ON_EN_SETFOCUS(IDC_PROPEY, OnSetfocusPropey)
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_POSZ, &CChangDlg::OnEnChangePosz)
	ON_BN_CLICKED(IDC_MAKE_OTHER, &CChangDlg::OnBnClickedMakeOther)
	ON_BN_CLICKED(IDC_RADIO1, &CChangDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CChangDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO10, &CChangDlg::OnBnClickedRadio10)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CChangDlg::OnNewDocument()
{
	m_dbX      = 0.0;
	m_dbY      = 0.0;
	m_dbZ	   = 0.0;
	m_nRotatePlane = 0;
	m_dbRotate = 0.0;
	m_dbPropX  = 100.0;
	m_dbPropY  = 100.0;
	m_dbSizeX  = 0.0;
	m_dbSizeY  = 0.0;
	m_strSizeX = _T("0.0");
	m_strSizeY = _T("0.0");
	m_bProrata = TRUE;
	
	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetCheck(TRUE);
	HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON19);
	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);
	
	UpdateUIData(FALSE);
	
	m_strSizeX = _T("0.0");
	m_strSizeY = _T("0.0");
    m_strPropX = _T("100.0");
	m_strPropY = _T("100.0");
	
	UpdateData(FALSE);
	
	return TRUE;
}





void CChangDlg::OnApply()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;
	
	CHLView *pView = GetCurActiveView();
	if ( pView == NULL )
		return;
	CHLDoc *pDoc = pView->GetDocument();
	if ( NULL == pDoc )
		return;
	
	CWnd *pWnd = (CWnd *)GetDlgItem(IDOK);
	CButton *pButt = (CButton*)pWnd;
	
	CQuad oldQuad;
    oldQuad.SetEmpty();
	oldQuad = pDoc->GetTrackerQuad();
	
	CDot dot = oldQuad.GetBaseDot(m_nBase);
	double dbWith   = oldQuad.Width();
	double dbHeight = oldQuad.Height();
	
	UpdateUIData(TRUE);
	
	m_dbSizeX = _tstof(m_strSizeX);
	m_dbSizeY = _tstof(m_strSizeY);
	
	m_dbPropX = _tstof(m_strPropX);
	m_dbPropY = _tstof(m_strPropY);
	
	if ( m_dbSizeX < 0 )
		m_dbSizeX = dbWith;
	else if ( fabs(m_dbSizeX) < 0.0001 )
		m_dbSizeX  = 0.0001;
	
	if ( m_dbSizeY < 0 )
		m_dbSizeY = dbHeight; 
	else if (fabs(m_dbSizeY) < 0.0001 ) 
		m_dbSizeY = 0.0001;
	
	if ( m_dbPropX < 0 )  
		m_dbPropX = 100; 
	else if ( fabs(m_dbPropX)  <=  0.0001 ) 
		m_dbPropX = 0.0001;
	
	if (m_dbPropY < 0)
		m_dbPropY = 100;
	else if (fabs(m_dbPropY) < 0.0001)
		m_dbPropY = 0.0001;
	
	switch( m_dwOperate )
	{	
	case ENUM_OBJECT_SIZE:
		{
			if ( (fabs(dbWith) > EPSILON) && (fabs(dbHeight) > EPSILON) )
			{	
				pDoc->Drag(dot, m_dbSizeX/dbWith, m_dbSizeY/dbHeight, 0, 0);
			}
			else if ( (fabs(dbWith)  <=  EPSILON) && (fabs(dbHeight) > EPSILON) )
			{
				pDoc->Drag(dot, 1, m_dbSizeY/dbHeight, 0, 0);
			}
			else if ( (fabs(dbWith) > EPSILON) && (fabs(dbHeight)  <=  EPSILON) )
			{
				pDoc->Drag(dot, m_dbSizeX/dbWith, 1, 0, 0);
			}
			m_dwOperate = ENUM_OBJECT_END;
			break;
		}
	case ENUM_OBJECT_SCALE:
		{
			pDoc->Drag(dot, m_dbPropX/100, m_dbPropY/100, 0, 0);
			break;
		}
	case ENUM_OBJECT_ROTATE:
		{
			if ( fabs(m_dbRotate) > EPSILON )
			{
				pDoc->Rotate(dot, m_dbRotate, m_nRotatePlane);
			}
			break;
		}
	case ENUM_OBJECT_POS:
		{
			CQuad quad = pDoc->GetLimit();
			CDot dotOld,dotNew;
			dotOld = quad.GetBaseDot(m_nBase);
			
			BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_CENTERPOS))->GetCheck();
			if (bRet)
			{
				dot = quad.Center();
				dotNew = CDot( dotOld.x + m_dbX, dotOld.y + m_dbY, dotOld.z + m_dbZ );
			}
			else
			{
				dotNew = CDot( m_dbX, m_dbY, m_dbZ );
			}
			
			double moveX = dotNew.x-dotOld.x;
			double moveY = dotNew.y-dotOld.y;
			double moveZ = dotNew.z - dotOld.z;
			
//			pDoc->Drag( dotOld, 1, 1, moveX, moveY, moveZ );
			pDoc->Move( moveX, moveY, moveZ );
			
			break;
		}
	case ENUM_OBJECT_SHEAR:
		{
			pDoc->Shear(dot, -1*m_dbShearX, m_dbShearY);
		}
		break;
	
	default:
		break;
	}
	
	UpdateUIData(FALSE);
	
	pView->SetFocus();
	pView->DispSelObjProp();
}

//
void CChangDlg::OnMakecopy()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;
	
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;
	
	pDoc->Copy();
	OnApply();
}

void CChangDlg::UpdateUIData(BOOL bSaveAndValidate)
{
	if ( !bSaveAndValidate ) 
	{
		m_dbX = MyPrecision(m_dbX);
		m_dbY = MyPrecision(m_dbY);
		m_dbZ = MyPrecision(m_dbZ);
		m_dbRotate = MyPrecision(m_dbRotate);
		
		m_strPropX.Format(_T("%0.3f"), m_dbPropX);
		m_strPropY.Format(_T("%0.3f"), m_dbPropY);
		
		m_strSizeX.Format(_T("%0.3f"), m_dbSizeX);
		m_strSizeY.Format(_T("%0.3f"), m_dbSizeY);
	}

	UpdateData(bSaveAndValidate);

	//
	if ( !bSaveAndValidate )
	{
		UpdateDlgView();
	}
}

void CChangDlg::SetPos(double dbX,double dbY, double dbZ)
{
	BOOL bCheck = ((CButton*)(GetDlgItem(IDC_CHECK_CENTERPOS)))->GetCheck();
	
	if (!bCheck) 
	{
		m_dbX = MyPrecision(dbX);
		m_dbY = MyPrecision(dbY);
		m_dbZ = MyPrecision(dbZ);
	}
	
	UpdateUIData(FALSE);
}

void CChangDlg::SetSize(double dbSizeX,double dbSizeY)
{
	m_strSizeX.Format(_T("%0.3f"), dbSizeX);
	m_strSizeY.Format(_T("%0.3f"), dbSizeY);

	m_dbSizeX = dbSizeX;
	m_dbSizeY = dbSizeY;
	UpdateUIData(FALSE);
}

BOOL CChangDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON14);
	((CButton*)GetDlgItem(IDC_BUTT_SELPOS))->SetIcon(hIcon);
	
	hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON22);
	((CButton*)GetDlgItem(IDC_MAKECOPY_SET))->SetIcon(hIcon);
	
	m_bProrata = TRUE;
	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetCheck(TRUE);
	hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON19);
	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);
	
	UpdateData(FALSE);
	return TRUE;
}

void CChangDlg::OnChangeRotate() 
{
	m_dwOperate = ENUM_OBJECT_ROTATE;
}

void CChangDlg::CloseDoor()
{
	(CButton*)GetDlgItem(IDC_POSX)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_POSY)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_PROPEX)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_PROPEY)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_SIZEY)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_SIZEX)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_ROTATE)->EnableWindow(FALSE);
}

void CChangDlg::OpenDoor()
{
	(CButton*)GetDlgItem(IDC_POSX)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_POSY)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_PROPEX)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_PROPEY)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_SIZEY)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_SIZEX)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_ROTATE)->EnableWindow(TRUE);
}

void CChangDlg::OnButtSelpos() 
{
	CHLView *pView = GetCurActiveView();
	if ( !pView )  return;
	
	CButton *pButt = (CButton*)GetDlgItem(IDC_BUTT_SELPOS);
	SelectCoordinary dlg(pView,&m_dbX,&m_dbY,&m_dbSizeX,&m_dbSizeY,pButt,&m_nBase);
	if ( dlg.DoModal() == IDOK )
	{
		SetPosIcon();
		UpdateUIData(FALSE);
	}
}




void CChangDlg::OnChangeSizeX()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	if (!pDoc->IsSelectedObj())
	{
		m_dbSizeX = 0;
		UpdateUIData(FALSE);
		return;
	}
	
	m_dwOperate = ENUM_OBJECT_SIZE;

	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad = pDoc->GetTrackerQuad();

	CDot dot  = oldQuad.GetBaseDot(m_nBase);
	double dbWith   = oldQuad.Width();
	double dbHeight = oldQuad.Height();
	
    UpdateUIData(TRUE);
	
	m_dbSizeX = _tstof(m_strSizeX);
	m_dbSizeY = _tstof(m_strSizeY);
	
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		if (fabs(dbWith) < EPSILON) 
		{
			m_dbPropX = 100;
		}
		else
		{
			m_dbSizeY = m_dbSizeX  * dbHeight  / dbWith;
		    m_dbPropX = (m_dbSizeX / dbWith)   * 100;
		}

		if (fabs(dbHeight) < EPSILON) 
		{
			m_dbPropY = 100;
		}
		else
		{
			m_dbPropY = (m_dbSizeY / dbHeight) * 100;
		}
	}
	else
	{
		if (fabs(dbWith) < EPSILON) 
		{
			m_dbPropX = 100;
		}
		else
		{
			m_dbPropX = (m_dbSizeX / dbWith)   * 100;
		}
	}

	m_strSizeY.Format(_T("%0.3f"), m_dbSizeY);

	m_strPropX.Format(_T("%0.3f"), m_dbPropX);
	m_strPropY.Format(_T("%0.3f"), m_dbPropY);
	
	UpdateData(FALSE);
}

void CChangDlg::OnChangeSizeY()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	if (!pDoc->IsSelectedObj())
	{
		m_dbSizeY = 0;
		UpdateUIData(FALSE);
		return;
	}

	m_dwOperate = ENUM_OBJECT_SIZE;
	
	UpdateUIData(TRUE);
	
	m_dbSizeX = _tstof(m_strSizeX);
	m_dbSizeY = _tstof(m_strSizeY);

	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad = pDoc->GetTrackerQuad();
	
	double dbWith   = oldQuad.Width();
	double dbHeight = oldQuad.Height();
	
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		if (fabs(dbHeight) < EPSILON) 
		{
		    m_dbPropY = 100;
		}
		else
		{
			m_dbSizeX = m_dbSizeY  * dbWith / dbHeight;
		    m_dbPropY = (m_dbSizeY / dbHeight) * 100;
		}
		
		if (fabs(dbWith) < EPSILON)
		{
			m_dbPropX = 100;
		}
		else
		{
			m_dbPropX = (m_dbSizeX / dbWith)   * 100;
		}
	}
	else
	{
		if (fabs(dbHeight) < EPSILON) 
		{
			m_dbPropY = 100;
		}
		else
		{
			m_dbPropY = (m_dbSizeY / dbHeight) * 100;
		}
		
	}

	m_strSizeX.Format(_T("%0.3f"), m_dbSizeX);
	m_strPropX.Format(_T("%0.3f"), m_dbPropX);
	m_strPropY.Format(_T("%0.3f"), m_dbPropY);

	UpdateData(FALSE);
}

void CChangDlg::OnChangePropX() 
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	if (!pDoc->IsSelectedObj())
	{   
		m_dbPropX = 100;
		UpdateUIData(FALSE);
		return;
	}
	
	m_dwOperate = ENUM_OBJECT_SCALE;

	UpdateUIData(TRUE);

	m_dbSizeX = _tstof(m_strSizeX);
	m_dbSizeY = _tstof(m_strSizeY);

	m_dbPropX = _tstof(m_strPropX);
	m_dbPropY = _tstof(m_strPropY);

	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad = pDoc->GetTrackerQuad();

	double dbWidth  = oldQuad.Width();
	double dbHeight = oldQuad.Height();

	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		m_dbPropY = m_dbPropX;
		m_dbSizeX = dbWidth  * (m_dbPropX / 100);
		m_dbSizeY = dbHeight * (m_dbPropY / 100);
	}
	else
	{
		m_dbSizeX = dbWidth  * (m_dbPropX / 100);
	}

	m_strSizeX.Format(_T("%0.3f"), m_dbSizeX);
	m_strSizeY.Format(_T("%0.3f"), m_dbSizeY);

	m_strPropY.Format(_T("%0.3f"), m_dbPropY);

	UpdateData(FALSE);
}

void CChangDlg::OnChangePropY() 
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	if (!pDoc->IsSelectedObj())
	{   
		m_dbPropY = 100;
		UpdateUIData(FALSE);
		return;
	}

	m_dwOperate = ENUM_OBJECT_SCALE;

	UpdateUIData(TRUE);

	m_dbSizeX = _tstof(m_strSizeX);
	m_dbSizeY = _tstof(m_strSizeY);

	m_dbPropX = _tstof(m_strPropX);
	m_dbPropY = _tstof(m_strPropY);
	
	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad = pDoc->GetTrackerQuad();

	double dbHeight = oldQuad.Height();
	double dbWidth  = oldQuad.Width();

	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		m_dbSizeY = dbHeight * (m_dbPropY / 100);
		m_dbPropX = m_dbPropY;
		m_dbSizeX = dbWidth  * (m_dbPropX / 100);

	}
	else
	{
		m_dbSizeY = dbHeight * (m_dbPropY / 100);
	}

	m_strSizeX.Format(_T("%0.3f"), m_dbSizeX);
	m_strSizeY.Format(_T("%0.3f"), m_dbSizeY);

	m_strPropX.Format(_T("%0.3f"), m_dbPropX);
	
	UpdateData(FALSE);
}

void CChangDlg::OnCheckScale() 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (!bRet)
	{
		HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON20);
	    ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);
		m_bProrata = FALSE;
	}
	else
	{
		HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON19);
	    ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);
		m_bProrata = TRUE;
	}

	return;
}

void CChangDlg::OnCheckPOS() 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_CENTERPOS))->GetCheck();
	CString str;

	if (!bRet)
	{
		str.LoadString(IDS_POS_ABSOLUTE);
		GetDlgItem(IDC_STATIC_OBJPOS)->SetWindowText(str);
	}
	else
	{
		str.LoadString(IDS_POS_RELATIVE);
		GetDlgItem(IDC_STATIC_OBJPOS)->SetWindowText(str);
		GetDlgItem(IDC_POSX)->SetWindowText(_T("0"));
		GetDlgItem(IDC_POSY)->SetWindowText(_T("0"));
		m_dbX = 0.0;
		m_dbY = 0.0;
		m_dbZ = 0.0;
	}
}

void CChangDlg::OnMakecopySet() 
{
	CHLView* pView = GetCurActiveView();
	if ( NULL == pView )
		return;

	CHLDoc *pDoc = pView->GetDocument();
	if ( NULL == pDoc )
		return;

	CMakeCopySet MakeCopySet;
	if (IDOK == MakeCopySet.DoModal())
	{
		CQuad oldQuad;
		oldQuad.SetEmpty();
		oldQuad = pDoc->GetTrackerQuad();

		CDot dot;
		dot = oldQuad.GetBaseDot(m_nBase);

		if (m_dbPropX < 0)  m_dbPropX = 100;
		if (m_dbPropY < 0)  m_dbPropY = 100;

		double dbPropX = m_dbPropX / 100;
		double dbPropY = m_dbPropY / 100;

		for (int i = 1; i < MakeCopySet.m_nScanCount; i++)
		{
			for (int j = 0; j < (i + 1); j++)
			{
				pDoc->Copy();
				
				if ( (dbPropX > 0 || dbPropY > 0) & (0 == j) )
				{
					pDoc->Drag(dot, dbPropX, dbPropY, 0, 0);
					pView->Invalidate();
				}
			}
		}
		
		UpdateUIData(FALSE);
	}

	pView->SetFocus();
}



void CChangDlg::OnChangePosx() 
{
	m_dwOperate = ENUM_OBJECT_POS;
}




void CChangDlg::OnChangePosy() 
{
	m_dwOperate = ENUM_OBJECT_POS;
}




void CChangDlg::OnSetfocusPosx() 
{
	m_dwOperate = ENUM_OBJECT_POS;
}



void CChangDlg::OnSetfocusPosy() 
{
	m_dwOperate = ENUM_OBJECT_POS;	
}



void CChangDlg::OnSetfocusRotate() 
{
	m_dwOperate = ENUM_OBJECT_ROTATE;	
}



void CChangDlg::OnSetfocusSizex() 
{
	m_dwOperate = ENUM_OBJECT_SIZE;	
}



void CChangDlg::OnSetfocusSizey() 
{
	m_dwOperate = ENUM_OBJECT_SIZE;
}

void CChangDlg::OnSetfocusPropex() 
{
	m_dwOperate = ENUM_OBJECT_SCALE;
}



void CChangDlg::OnSetfocusPropey() 
{
	m_dwOperate = ENUM_OBJECT_SCALE;
}




BOOL CChangDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F10) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL , NULL);
		return TRUE;
	}

	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F7) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
		return TRUE;
	}
	
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F11) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_DWONLOAD_MARKDATA , NULL);
		return TRUE;
	}

	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_RETURN) & 0x8000))
	{
		return TRUE;
	}
	
	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_ESCAPE) & 0x8000))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CChangDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
}


void CChangDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


void CChangDlg::UpdateProp(CQuad &quad) 
{
	BOOL bCheck = ((CButton*)(GetDlgItem(IDC_CHECK_CENTERPOS)))->GetCheck();
	
	if (!bCheck) 
	{
		CDot dotCur;
		dotCur = quad.GetBaseDot(m_nBase);
		
		m_dbX = dotCur.x;
		m_dbY = dotCur.y;
		m_dbZ = dotCur.z;
		
		m_dbX = MyPrecision(m_dbX);
		m_dbY = MyPrecision(m_dbY);
		m_dbZ = MyPrecision(m_dbZ);
	}
	
	m_dbSizeX = quad.Width();
	m_dbSizeY = quad.Height();
	m_dbPropX = 100;
	m_dbPropY = 100;
	
	m_dbPropX  = MyPrecision(m_dbPropX);
	m_dbPropY  = MyPrecision(m_dbPropY);
	
	m_strSizeX.Format(_T("%0.3f"), m_dbSizeX);
	m_strSizeY.Format(_T("%0.3f"), m_dbSizeY);
	
    UpdateData(FALSE);
	UpdateDlgView();
}






//
void CChangDlg::SetPosIcon()
{
	CButton *pButt = (CButton*)GetDlgItem(IDC_BUTT_SELPOS);
	switch(m_nBase)
	{
	case BASE_TL:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON10);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_TM:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON11);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_TR:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON12);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_ML:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON13);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_MM:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON14);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_MR:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON15);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_BL:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON16);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_BM:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON17);
			pButt->SetIcon(hIcon);
			break;
		}
	case BASE_BR:
		{
			HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON18);
			pButt->SetIcon(hIcon);
			break;
		}
	}
}


void CChangDlg::UpdateDlgView()
{
	CHLView *pView = GetCurActiveView();
	if ( pView == NULL )
		return;

	CHLDoc *pDoc = pView->GetDocument();
	if ( NULL == pDoc )
		return;
	
	BOOL bFlag = pDoc->IsEnable3DEdit();
	
	//
	GetDlgItem(IDC_POSZ)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO1)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO4)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO10)->EnableWindow(bFlag);
	
	if ( !bFlag )
	{
		m_dbZ = 0;
		m_nRotatePlane = 0;
		UpdateData(FALSE);
	}
}



void CChangDlg::OnEnChangePosz()
{
	// TODO:  在此添加控件通知处理程序代码
	m_dwOperate = ENUM_OBJECT_POS;
}


//
void CChangDlg::OnBnClickedMakeOther()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgChangeOther dlg;	
	if ( dlg.DoModal() == IDOK )
	{
		if ( dlg.m_bShellEdit )
		{
			m_dwOperate = ENUM_OBJECT_SHEAR;
			m_dbShearX = dlg.m_dbShearX;
			m_dbShearY = dlg.m_dbShearY;
		}
	}
}


void CChangDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_dwOperate = ENUM_OBJECT_ROTATE;
}

void CChangDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_dwOperate = ENUM_OBJECT_ROTATE;
}

void CChangDlg::OnBnClickedRadio10()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_dwOperate = ENUM_OBJECT_ROTATE;
}
