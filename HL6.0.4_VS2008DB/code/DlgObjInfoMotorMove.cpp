// DlgObjInfoMotorMove.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgObjInfoMotorMove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoMotorMove dialog


CDlgObjInfoMotorMove::CDlgObjInfoMotorMove(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjInfoMotorMove::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgObjInfoMotorMove)
	m_nAxisNo = 0;
	m_dbLineValue = 0.0;
	m_dbRotateValue = 0.0;
	m_nMoveModel = 0;
	m_nWorkType = 0;
	//}}AFX_DATA_INIT
	m_pNode = NULL;
}


void CDlgObjInfoMotorMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjInfoMotorMove)
	DDX_CBIndex(pDX, IDC_COMBO_AxisNo, m_nAxisNo);
	DDX_Text(pDX, IDC_EDIT_VALUE_LINE, m_dbLineValue);
	DDX_Text(pDX, IDC_EDIT_VALUE_ROTATE, m_dbRotateValue);
	DDX_Radio(pDX, IDC_RADIO_MOVEMODEL, m_nMoveModel);
	DDX_Radio(pDX, IDC_RADIO_WORKTYPE, m_nWorkType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjInfoMotorMove, CDialog)
	//{{AFX_MSG_MAP(CDlgObjInfoMotorMove)
	ON_CBN_SELCHANGE(IDC_COMBO_AxisNo, OnSelchangeCOMBOAxisNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoMotorMove message handlers


void CDlgObjInfoMotorMove::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect())
		return;
	
	UpdateData(TRUE);
	
	m_pNode->m_para.nAxis = m_nAxisNo;
	m_pNode->m_nWorkType = m_nWorkType;	
	m_pNode->m_para.nMoveMode = m_nMoveModel;
	
	if (g_Motion.GetAxisType(m_nAxisNo) == 0)
	{
		 m_pNode->m_dbMoveAngle = m_dbLineValue;
	}
	else
	{
		 m_pNode->m_dbMoveAngle = m_dbRotateValue;
	}
}





void CDlgObjInfoMotorMove::SetInfo( CNode* pNode )
{
	if ( !pNode )
		return;
	
	if ( pNode->GetType()  !=  NODE_MOTORMOVE )
		return;

	m_pNode = (CNodeMotorMove *)pNode;
	
	m_nWorkType = m_pNode->m_nWorkType;	
	m_nAxisNo = m_pNode->m_para.nAxis;
	m_nMoveModel = m_pNode->m_para.nMoveMode;
	
	if ( g_Motion.GetAxisType(m_nAxisNo) == 0 )
	{
		m_dbLineValue = m_pNode->m_dbMoveAngle;
		GetDlgItem(IDC_EDIT_VALUE_ROTATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VALUE_LINE)->EnableWindow(TRUE);
	}
	else
	{
		m_dbRotateValue = m_pNode->m_dbMoveAngle;
		GetDlgItem(IDC_EDIT_VALUE_ROTATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VALUE_LINE)->EnableWindow(FALSE);
	}
	
	UpdateData(FALSE);
}




void CDlgObjInfoMotorMove::OnSelchangeCOMBOAxisNo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if (g_Motion.GetAxisType(m_nAxisNo) == 0)
	{
		GetDlgItem(IDC_EDIT_VALUE_ROTATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VALUE_LINE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_VALUE_ROTATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VALUE_LINE)->EnableWindow(FALSE);
	}
}
