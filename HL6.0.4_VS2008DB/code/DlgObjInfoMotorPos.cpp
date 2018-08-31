// DlgObjInfoMotorPos.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgObjInfoMotorPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoMotorPos dialog


CDlgObjInfoMotorPos::CDlgObjInfoMotorPos(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjInfoMotorPos::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgObjInfoMotorPos)
	m_dbLineValue0 = 0.0;
	m_dbLineValue1 = 0.0;
	m_dbLineValue2 = 0.0;
	m_dbLineValue3 = 0.0;
	m_dbRotateValue0 = 0.0;
	m_dbRotateValue1 = 0.0;
	m_dbRotateValue2 = 0.0;
	m_dbRotateValue3 = 0.0;
	//}}AFX_DATA_INIT

	m_pNode = NULL;
}


void CDlgObjInfoMotorPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjInfoMotorPos)
	DDX_Text(pDX, IDC_EDIT_LINE_VALUE0, m_dbLineValue0);
	DDX_Text(pDX, IDC_EDIT_LINE_VALUE1, m_dbLineValue1);
	DDX_Text(pDX, IDC_EDIT_LINE_VALUE2, m_dbLineValue2);
	DDX_Text(pDX, IDC_EDIT_LINE_VALUE3, m_dbLineValue3);
	DDX_Text(pDX, IDC_EDIT_ROTATE_VALUE0, m_dbRotateValue0);
	DDX_Text(pDX, IDC_EDIT_ROTATE_VALUE1, m_dbRotateValue1);
	DDX_Text(pDX, IDC_EDIT_ROTATE_VALUE2, m_dbRotateValue2);
	DDX_Text(pDX, IDC_EDIT_ROTATE_VALUE3, m_dbRotateValue3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjInfoMotorPos, CDialog)
	//{{AFX_MSG_MAP(CDlgObjInfoMotorPos)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjInfoMotorPos message handlers


void CDlgObjInfoMotorPos::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect())
		return;
	
	UpdateData(TRUE);
	
	//
	if (g_Motion.GetAxisType(0) == 0)
	{
		m_pNode->m_arrayAxisPos[0] = m_dbLineValue0;
	}
	else
	{
		m_pNode->m_arrayAxisPos[0] = m_dbRotateValue0;
	}

	if (g_Motion.GetAxisType(1) == 0)
	{
		 m_pNode->m_arrayAxisPos[1] = m_dbLineValue1;
	}
	else
	{
		 m_pNode->m_arrayAxisPos[1] = m_dbRotateValue1;
	}

	if (g_Motion.GetAxisType(2) == 0)
	{
		 m_pNode->m_arrayAxisPos[2] = m_dbLineValue2;
	}
	else
	{
		 m_pNode->m_arrayAxisPos[2] = m_dbRotateValue2;
	}
	
	if (g_Motion.GetAxisType(3) == 0)
	{
		 m_pNode->m_arrayAxisPos[3] = m_dbLineValue3;
	}
	else
	{
		 m_pNode->m_arrayAxisPos[3] = m_dbRotateValue3;
	}
}





void CDlgObjInfoMotorPos::SetInfo(CNode* pNode)
{
	m_pNode = (CNodeMotorPos *)pNode;
	
	//
	if (g_Motion.GetAxisType(0) == 0)
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE0)->EnableWindow(FALSE);
		
		m_dbLineValue0 = m_pNode->m_arrayAxisPos[0];
	}
	else
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE0)->EnableWindow(TRUE);

		m_dbRotateValue0 = m_pNode->m_arrayAxisPos[0];
	}

	if (g_Motion.GetAxisType(1) == 0)
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE1)->EnableWindow(FALSE);

		m_dbLineValue1 = m_pNode->m_arrayAxisPos[1];
	}
	else
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE1)->EnableWindow(TRUE);

		m_dbRotateValue1 = m_pNode->m_arrayAxisPos[1];
	}

	if (g_Motion.GetAxisType(2) == 0)
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE2)->EnableWindow(FALSE);

		m_dbLineValue2 = m_pNode->m_arrayAxisPos[2];
	}
	else
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE2)->EnableWindow(TRUE);

		m_dbRotateValue2 = m_pNode->m_arrayAxisPos[2];
	}
	
	if (g_Motion.GetAxisType(3) == 0)
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE3)->EnableWindow(FALSE);

		m_dbLineValue3 = m_pNode->m_arrayAxisPos[3];
	}
	else
	{
		GetDlgItem(IDC_EDIT_LINE_VALUE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROTATE_VALUE3)->EnableWindow(TRUE);

		m_dbRotateValue3 = m_pNode->m_arrayAxisPos[3];
	}
	
	UpdateData(FALSE);
}

