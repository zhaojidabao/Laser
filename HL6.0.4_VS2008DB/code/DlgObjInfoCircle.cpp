// DlgObjInfoCircle.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgObjInfoCircle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoCircle dialog


CDlgObjInfoCircle::CDlgObjInfoCircle(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjInfoCircle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgObjInfoCircle)
	m_dbCenterX = 0.0;
	m_dbCenterY = 0.0;
	m_dbR = 0.0;
	//}}AFX_DATA_INIT

	m_pNodeCircle = NULL;
}


void CDlgObjInfoCircle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjInfoCircle)
	DDX_Text(pDX, IDC_EDIT_CENTERX, m_dbCenterX);
	DDX_Text(pDX, IDC_EDIT_CENTERY, m_dbCenterY);
	DDX_Text(pDX, IDC_EDIT_R, m_dbR);
	DDV_MinMaxDouble(pDX, m_dbR, 1.e-003, 999999.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjInfoCircle, CDialog)
	//{{AFX_MSG_MAP(CDlgObjInfoCircle)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoCircle message handlers

//
void CDlgObjInfoCircle::Apply()
{	
	if ( m_pNodeCircle == NULL || !m_pNodeCircle->IsSelect() )
		return;
	
	if ( m_pNodeCircle->GetLockFlg() == TRUE )
		return;
	
	//
	UpdateData(TRUE);
	
	CDot dot1, dot2;
	dot1.x = m_dbCenterX;
	dot1.y = m_dbCenterY;
	
	dot2.x = m_dbR + dot1.x;
	dot2.y = dot1.y;
	
	m_pNodeCircle->m_stkCtrlDot.m_dotArray.SetAt(0, dot1);
	m_pNodeCircle->m_stkCtrlDot.m_dotArray.SetAt(1, dot2);
	
	m_pNodeCircle->CreateStroke();

	CTree *pTree = (CTree*)m_pNodeCircle->GetParent();
	if ( pTree  !=  NULL )
		pTree->Fill();
}

//
void CDlgObjInfoCircle::GetObjInfo(CNode *pNode)
{	
	if ( pNode == NULL )
		return;
	
	if ( pNode->GetType()  !=  NODE_CIRCLE )
		return;
	//
	m_pNodeCircle = (CNodeCircle *)pNode;
	
	CDot dot1, dot2;
	dot1 = m_pNodeCircle->m_stkCtrlDot.m_dotArray.GetAt(0);
	dot2 = m_pNodeCircle->m_stkCtrlDot.m_dotArray.GetAt(1);
	
	//
	m_dbCenterX = MyPrecision(dot1.x);
	m_dbCenterY = MyPrecision(dot1.y);
	
	m_dbR = sqrt((dot1.x - dot2.x)*(dot1.x - dot2.x) + (dot1.y - dot2.y)*(dot1.y - dot2.y));
	m_dbR = MyPrecision(m_dbR);

	//
	UpdateData(FALSE);
}
