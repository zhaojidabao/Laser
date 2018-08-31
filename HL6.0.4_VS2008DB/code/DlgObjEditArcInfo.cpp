// DlgObjEditArcInfo.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgObjEditArcInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObjEditArcInfo dialog


CDlgObjEditArcInfo::CDlgObjEditArcInfo(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjEditArcInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgObjEditArcInfo)
	m_dbArcR = 0.0;
	m_dbCenterX = 0.0;
	m_dbCenterY = 0.0;
	m_dbEndAngle = 0.0;
	m_dbEndX = 0.0;
	m_dbEndY = 0.0;
	m_dbStartAngle = 0.0;
	m_dbStartX = 0.0;
	m_dbStartY = 0.0;
	//}}AFX_DATA_INIT
	
	m_pNodeArc3Pt = NULL;
}


void CDlgObjEditArcInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjEditArcInfo)
	DDX_Text(pDX, IDC_EDIT_ARCR, m_dbArcR);
	DDV_MinMaxDouble(pDX, m_dbArcR, 1.e-003, 99999.);
	DDX_Text(pDX, IDC_EDIT_CENTERX, m_dbCenterX);
	DDX_Text(pDX, IDC_EDIT_CENTERY, m_dbCenterY);
	DDX_Text(pDX, IDC_EDIT_ENDANGLE, m_dbEndAngle);
	DDX_Text(pDX, IDC_EDIT_ENDX, m_dbEndX);
	DDX_Text(pDX, IDC_EDIT_ENDY, m_dbEndY);
	DDX_Text(pDX, IDC_EDIT_STARTANGLE, m_dbStartAngle);
	DDX_Text(pDX, IDC_EDIT_STARTX, m_dbStartX);
	DDX_Text(pDX, IDC_EDIT_STARTY, m_dbStartY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjEditArcInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgObjEditArcInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjEditArcInfo message handlers

BOOL CDlgObjEditArcInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
void CDlgObjEditArcInfo::Apply()
{	
	if ( m_pNodeArc3Pt == NULL || !m_pNodeArc3Pt->IsSelect() )
		return;
	
	if ( m_pNodeArc3Pt->GetLockFlg() == TRUE )
		return;

	UpdateData(TRUE);
	
	CDot dot1, dot2, dot3;
	if ( m_pNodeArc3Pt->m_nArcType == 1 )
	{	
		double dbMiddleAngle;
		if ( m_dbStartAngle < m_dbEndAngle )
			dbMiddleAngle = (m_dbEndAngle + m_dbStartAngle)*0.5;
		else
		{
			dbMiddleAngle = m_dbStartAngle +(360 - m_dbStartAngle + m_dbEndAngle)*0.5;
		}
		
		if ( dbMiddleAngle > 360 )
			dbMiddleAngle = dbMiddleAngle - 360;
		
		dot1.x = m_dbArcR * cos(m_dbStartAngle*PAI/180) + m_dbCenterX; 		
		dot1.y = m_dbArcR * sin(m_dbStartAngle*PAI/180) + m_dbCenterY;
		
		dot2.x = m_dbArcR * cos(dbMiddleAngle*PAI/180) + m_dbCenterX;	
		dot2.y = m_dbArcR * sin(dbMiddleAngle*PAI/180) + m_dbCenterY;

		dot3.x = m_dbArcR * cos(m_dbEndAngle*PAI/180) + m_dbCenterX;		
		dot3.y = m_dbArcR * sin(m_dbEndAngle*PAI/180) + m_dbCenterY;
				
		m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.SetAt(0, dot1);
		m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.SetAt(1, dot2);
		m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.SetAt(2, dot3);
		
		//
		m_dbStartX = MyPrecision(dot1.x);
		m_dbStartY = MyPrecision(dot1.y);
		m_dbEndX = MyPrecision(dot3.x);
		m_dbEndY = MyPrecision(dot3.y);
		
		UpdateData(FALSE);
	}
	else if ( m_pNodeArc3Pt->m_nArcType == 2 )
	{
		dot1.x = m_dbCenterX;
		dot1.y = m_dbCenterY;
		
		dot2.x = m_dbArcR * cos(m_dbStartAngle*PAI/180) + dot1.x; 		
		dot2.y = m_dbArcR * sin(m_dbStartAngle*PAI/180) + dot1.y;
		
		dot3.x = m_dbArcR * cos(m_dbEndAngle*PAI/180) + dot1.x;
		dot3.y = m_dbArcR * sin(m_dbEndAngle*PAI/180) + dot1.y;
		
		m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.SetAt(0, dot1);
		m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.SetAt(1, dot2);
		m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.SetAt(2, dot3);
		
		//
		m_dbStartX = MyPrecision(dot2.x);
		m_dbStartY = MyPrecision(dot2.y);
		m_dbEndX = MyPrecision(dot3.x);
		m_dbEndY = MyPrecision(dot3.y);
		
		UpdateData(FALSE);
	}
	
	m_pNodeArc3Pt->CreateStroke();
}


//
void CDlgObjEditArcInfo::GetArcInfo(CNode *pNode)
{	
	if ( pNode == NULL )
		return;
	
	if ( pNode->GetType()  !=  NODE_ARC )
		return;
	
	//
	m_pNodeArc3Pt = (CNodeArc3Pt *)pNode;
	
	CDot dot1, dot2, dot3;
	dot1 = m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.GetAt(0);
	dot2 = m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.GetAt(1);
	dot3 = m_pNodeArc3Pt->m_stkCtrlDot.m_dotArray.GetAt(2);
	
	if ( m_pNodeArc3Pt->m_nArcType == 1 )
	{	
		CDot dotCenter;
		m_pNodeArc3Pt->m_GArc.GetCenter(dot1, dot2, dot3, dotCenter);
		
		double dbCenterAngle = m_pNodeArc3Pt->m_GArc.GetCenterAngle(dot1, dot2, dot3, dotCenter, 1);
		double dbAngle1, dbAngle2;
		
		dbAngle1 = m_pNodeArc3Pt->m_GArc.GetVectorAngleX(CDot(dot1.x - dotCenter.x, dot1.y - dotCenter.y));
		dbAngle2 = m_pNodeArc3Pt->m_GArc.GetVectorAngleX(CDot(dot3.x - dotCenter.x, dot3.y - dotCenter.y));
		
		if ( fabs(dbAngle1 - dbAngle2 + dbCenterAngle) < 0.01 )
		{	
			if ( dbAngle1 < dbAngle2 )
			{	
				m_dbStartX = MyPrecision(dot1.x);
				m_dbStartY = MyPrecision(dot1.y);
				
				m_dbEndX = MyPrecision(dot3.x);
				m_dbEndY = MyPrecision(dot3.y);
				
				m_dbStartAngle = MyPrecision(dbAngle1*180/PAI);
				m_dbEndAngle = MyPrecision(dbAngle2*180/PAI);
			}
			else
			{
				m_dbStartX = MyPrecision(dot3.x);
				m_dbStartY = MyPrecision(dot3.y);
				
				m_dbEndX = MyPrecision(dot1.x);
				m_dbEndY = MyPrecision(dot1.y);
				
				m_dbStartAngle = MyPrecision(dbAngle2*180/PAI);
				m_dbEndAngle = MyPrecision(dbAngle1*180/PAI);
			}
		}
		else
		{
			if ( dbAngle1 > dbAngle2 )
			{
				m_dbStartX = MyPrecision(dot1.x);
				m_dbStartY = MyPrecision(dot1.y);

				m_dbEndX = MyPrecision(dot3.x);
				m_dbEndY = MyPrecision(dot3.y);

				m_dbStartAngle = MyPrecision(dbAngle1*180/PAI);
				m_dbEndAngle = MyPrecision(dbAngle2*180/PAI);
			}
			else
			{
				m_dbStartX = MyPrecision(dot3.x);
				m_dbStartY = MyPrecision(dot3.y);

				m_dbEndX = MyPrecision(dot1.x);
				m_dbEndY = MyPrecision(dot1.y);

				m_dbStartAngle = MyPrecision(dbAngle2*180/PAI);
				m_dbEndAngle = MyPrecision(dbAngle1*180/PAI);
			}
		}

		m_dbCenterX = MyPrecision(dotCenter.x);
		m_dbCenterY = MyPrecision(dotCenter.y);
		
		m_dbArcR = sqrt((m_dbCenterX - m_dbStartX)*(m_dbCenterX - m_dbStartX) + (m_dbCenterY - m_dbStartY)*(m_dbCenterY - m_dbStartY));
		m_dbArcR = MyPrecision(m_dbArcR);
	}
	else if ( m_pNodeArc3Pt->m_nArcType == 2 )
	{	
		m_dbCenterX = MyPrecision(dot1.x);
		m_dbCenterY = MyPrecision(dot1.y);
		
		m_dbStartX = MyPrecision(dot2.x);
		m_dbStartY = MyPrecision(dot2.y);
		
		m_dbEndX = MyPrecision(dot3.x);
		m_dbEndY = MyPrecision(dot3.y);
		
		//
		m_dbArcR = sqrt((m_dbCenterX - m_dbStartX)*(m_dbCenterX - m_dbStartX) + (m_dbCenterY - m_dbStartY)*(m_dbCenterY - m_dbStartY));
		m_dbArcR = MyPrecision(m_dbArcR);
		
		m_dbStartAngle = m_pNodeArc3Pt->m_GArc.GetVectorAngleX(CDot(m_dbStartX - m_dbCenterX, m_dbStartY - m_dbCenterY));
		m_dbEndAngle = m_pNodeArc3Pt->m_GArc.GetVectorAngleX(CDot(m_dbEndX - m_dbCenterX, m_dbEndY - m_dbCenterY));
		
		m_dbStartAngle = MyPrecision(m_dbStartAngle*180/PAI);
		m_dbEndAngle = MyPrecision(m_dbEndAngle*180/PAI);
	}
	//
	UpdateData(FALSE);
}

