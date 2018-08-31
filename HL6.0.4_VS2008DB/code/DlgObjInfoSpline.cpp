// DlgObjInfoSpline.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgObjInfoSpline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoSpline dialog


CDlgObjInfoSpline::CDlgObjInfoSpline(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjInfoSpline::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgObjInfoSpline)
	m_nCount = 0;
	m_nCurNo = 0;
	m_dbCurX = 0.0;
	m_dbCurY = 0.0;
	//}}AFX_DATA_INIT

	m_pNodeSpline = NULL;
}


void CDlgObjInfoSpline::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjInfoSpline)
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_EDIT_CurNu, m_nCurNo);
	DDX_Text(pDX, IDC_EDIT_CurX, m_dbCurX);
	DDX_Text(pDX, IDC_EDIT_CurY, m_dbCurY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjInfoSpline, CDialog)
	//{{AFX_MSG_MAP(CDlgObjInfoSpline)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoSpline message handlers


void CDlgObjInfoSpline::Apply()
{
	if(m_pNodeSpline == NULL)
		return;
	
}



void CDlgObjInfoSpline::GetObjInfo(CNode *pNode)
{
	if(pNode == NULL)
		return;
	
	if(pNode->GetType() != NODE_SPLINE)
		return;
	
	m_pNodeSpline = (CNodeSpline *)pNode;

	m_nCount = m_pNodeSpline->m_pCtrlDotStroke->GetSize();
	
	m_nCurNo = 1;
	CetPtInfo(m_nCurNo - 1);

	UpdateData(FALSE);
}



void CDlgObjInfoSpline::CetPtInfo(int nIndex)
{
	CDot dot;
	dot = m_pNodeSpline->m_pCtrlDotStroke->GetAt(nIndex);
	m_dbCurX = MyPrecision(dot.x);
	m_dbCurY = MyPrecision(dot.y);
}




//
void CDlgObjInfoSpline::SetPtInfo(int nIndex)
{
	CDot dot;
	dot.x = m_dbCurX;
	dot.y = m_dbCurY;
	
	m_pNodeSpline->m_pCtrlDotStroke->m_dotArray.SetAt(nIndex, dot);
}
