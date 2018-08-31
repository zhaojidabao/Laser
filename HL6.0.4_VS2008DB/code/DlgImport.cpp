// DlgImport.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgImport.h"
#include "NodePlt.h"
#include "Nodedxf_GP.h"
#include "NodeDib.h"
#include "baseMacro.h"
#include "NodeNC.h"
#include "NodeAI.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImport

IMPLEMENT_DYNAMIC(CDlgImport, CFileDialog)

CDlgImport::CDlgImport(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, 0, FALSE)
{
	m_bCenter = TRUE;
	m_bPltBetter = FALSE;
	m_bPreview = FALSE;
	m_pNode = NULL;
	
	m_nAiType = 0;
	m_nAiFillType = 0;
	m_dbAiFillDis = 0.005;
}


BEGIN_MESSAGE_MAP(CDlgImport, CFileDialog)
	//{{AFX_MSG_MAP(CDlgImport)
	ON_BN_CLICKED(IDC_CHECK_PREVIEW, OnCheckPreview)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_AI_TYPE, OnAiType)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADI_FILLTYPE, OnAiFillType)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgImport::OnAiType()
{
	UpdateDlgView();
	RefreshView();
}

void CDlgImport::OnRadio2()
{
	UpdateDlgView();
	RefreshView();
}

void CDlgImport::OnAiFillType()
{
	UpdateDlgView();
	RefreshView();
}

void CDlgImport::OnRadio4()
{
	UpdateDlgView();
	RefreshView();
}

void CDlgImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDibDlg)
	DDX_Check(pDX, IDC_CHECK_Center, m_bCenter);
    DDX_Check(pDX, IDC_CHECK_PLT_BETTER, m_bPltBetter);
	DDX_Check(pDX, IDC_CHECK_PREVIEW, m_bPreview);
	
	DDX_Radio(pDX, IDC_RADIO_AI_TYPE, m_nAiType);
	DDX_Radio(pDX, IDC_RADI_FILLTYPE, m_nAiFillType);
	DDX_Text(pDX, IDC_EDIT_FILL_DIS, m_dbAiFillDis);
	//}}AFX_DATA_MAP
}

void CDlgImport::UpdateDlgView()
{
	UpdateData(TRUE);
	
	BOOL bIsAiFile = FALSE;
	CString strPath, strExt;
	strPath = GetPathName();
	if( FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(strPath) ) //目录
	{
		bIsAiFile = FALSE;
	}
	else
	{
		strExt = GetFileExt();
		strExt.MakeUpper();
		bIsAiFile = ( strExt == _T("AI") ? TRUE : FALSE );
	}
	
	//
	GetDlgItem(IDC_RADIO_AI_TYPE)->EnableWindow(bIsAiFile);
	GetDlgItem(IDC_RADIO2)->EnableWindow(bIsAiFile);
	GetDlgItem(IDC_RADI_FILLTYPE)->EnableWindow(bIsAiFile && (1 == m_nAiType));
	GetDlgItem(IDC_RADIO4)->EnableWindow(bIsAiFile && (1 == m_nAiType));
	GetDlgItem(IDC_EDIT_FILL_DIS)->EnableWindow(bIsAiFile && (1 == m_nAiType));
}


int CDlgImport::DoModal()
{
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_IMPORT);
	m_ofn.Flags  |=  OFN_ENABLETEMPLATE;
	return CFileDialog::DoModal();	
}



BOOL CDlgImport::OnFileNameOK()
{
	UpdateData( TRUE );	
	return CFileDialog::OnFileNameOK();
}



void CDlgImport::OnCheckPreview()
{
	UpdateData();
	RefreshView();
}



BOOL CDlgImport::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateDlgView();

	CRect rect;
	GetDlgItem(IDC_STATIC_PREVIEWZOOM)->GetClientRect(rect);
	//
	double dbWidth = rect.Width();
	double dbHeight = rect.Height();
	m_DC.m_ptCenter.x = (int)dbWidth/2;
	m_DC.m_ptCenter.y = (int)dbHeight/2;
	m_DC.SetViewRect(dbWidth,dbHeight);
	
	GetDlgItem(IDC_STATIC_PREVIEWZOOM)->GetClientRect(m_rect);
	m_rect.DeflateRect(20, 20);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgImport::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if ( m_pNode )
	{
		CRect rect;
		CWnd *pWndDraw = GetDlgItem(IDC_STATIC_PREVIEWZOOM);
		pWndDraw->GetClientRect(&rect);
		CDC* pDC = pWndDraw->GetDC();
		if ( pDC != NULL )
		{
			pDC->FillSolidRect(rect,RGB(255,255,255));
			
			m_DC.m_pDC = pDC;
			m_pNode->Draw(&m_DC);
			
			ReleaseDC(pDC);
		}
	}
	
	// Do not call CFileDialog::OnPaint() for painting messages
}




void CDlgImport::OnFileNameChange()
{
	RefreshView();
	UpdateDlgView();
}



void CDlgImport::RefreshView()
{
	try
	{
		if ( m_pNode )
		{
			delete m_pNode;
			m_pNode = NULL;
		}

		if ( m_bPreview ) 
		{
			CString strPath, strExt;
			strPath = GetPathName();
			if ( FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(strPath) ) 
				return;

			strExt = GetFileExt();
			strExt.MakeUpper();

			CHLDoc *pDoc = GetCurActiveDocument();
			if ( pDoc == NULL )
				return;

			CLay  lay = pDoc->GetLay();
			CLayer ly = lay.GetCurLayer();
			CProperty pty = ly.GetProperty();
			
			AfxGetApp()->BeginWaitCursor();
			if ( 0 == strExt.CompareNoCase(_T("PLT")) )
			{	
				m_pNode = new CNodePlt();
				m_pNode->SetProperty(PTY_INIT,pty);			
				if ( FALSE == ((CNodePlt*)m_pNode)->DeCode(strPath, TRUE, FALSE) )
				{
					delete m_pNode;
					m_pNode = NULL;
				}
			}
			else if ( 0 == strExt.CompareNoCase(_T("DXF")) )
			{
				m_pNode = new CNodeDxf_GP();
				m_pNode->SetProperty(PTY_INIT,pty);			
				if (FALSE == ((CNodeDxf_GP*)m_pNode)->DeCode(strPath, TRUE, FALSE))
				{
					delete m_pNode;
					m_pNode = NULL;
				}			
			}
			else if ( 0 == strExt.CompareNoCase(_T("NC")) )
			{
				m_pNode = new CNodeNC();
				m_pNode->SetProperty(PTY_INIT,pty);			
				if ( FALSE == ((CNodeNC*)m_pNode)->DeCode(strPath, FALSE, FALSE) )
				{
					delete m_pNode;
					m_pNode = NULL;
				}
			}
			else if ( 0 == strExt.CompareNoCase(_T("AI")) )
			{
				m_pNode = new CNodeAI();
				m_pNode->SetProperty(PTY_INIT,pty);
				if( FALSE == ((CNodeAI*)m_pNode)->DeCode(strPath, FALSE, FALSE) )
				{
					delete m_pNode;
					m_pNode = NULL;
				}
			}
			else
			{
				m_pNode = new CNodeDib(strPath);
				m_pNode->SetProperty(PTY_INIT,pty, &m_DC);		
				if (FALSE == m_pNode->Track(NULL,CPoint(0,0)))
				{
					delete m_pNode;
					m_pNode = NULL;
				}
				
				((CNodeDib*)m_pNode)->m_nDibType = 0;
				((CNodeDib*)m_pNode)->DataChange();
			}
			//结束等待光标
			AfxGetApp()->EndWaitCursor();

			if( m_pNode )
			{
				CQuad quad = m_pNode->GetLimit();
				m_pNode->InitQuadRange();
			
				double dbWidth  = quad.Width();
				double dbHeight = quad.Height();

				double dbScaleW = m_rect.Width()/dbWidth;
				double dbScaleH  = m_rect.Height()/dbHeight;
				
				m_DC.SetScale(dbScaleW>dbScaleH ? dbScaleH : dbScaleW);
			}
		}
	}
	catch (...) 
	{
	}

	Invalidate();
}




