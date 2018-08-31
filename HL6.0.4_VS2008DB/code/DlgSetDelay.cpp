// DlgSetDelay.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSetDelay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetDelay dialog

CDlgSetDelay::CDlgSetDelay(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgSetDelay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetDelay)
	m_dwDelay = 0;
	//}}AFX_DATA_INIT
	m_pNode = NULL;
}


void CDlgSetDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetDelay)
	DDX_Text(pDX, IDC_EDIT_DELAY, m_dwDelay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetDelay, CDialog)
	//{{AFX_MSG_MAP(CDlgSetDelay)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetDelay message handlers

void CDlgSetDelay::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect())
		return;

	UpdateData(TRUE);
	if (m_dwDelay<0) 
	{
		m_dwDelay = 0;
		UpdateData(FALSE);
	}
	else if (m_dwDelay > 600000)
	{	
		m_dwDelay = 600000;
		UpdateData(FALSE);
	}

	m_pNode->SetDelay(m_dwDelay);
}

void CDlgSetDelay::SetInfo(CNode* pNode)
{
	m_pNode = (CNodeDelay*)pNode;
	m_dwDelay = m_pNode->GetDelay();
	UpdateData(FALSE);
}

BOOL CDlgSetDelay::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((WM_KEYDOWN == pMsg->message) && (VK_ESCAPE == pMsg->wParam)) {return TRUE;}
	if ((WM_KEYDOWN == pMsg->message) && (VK_RETURN == pMsg->wParam)) {return TRUE;}
	
	return CDialog::PreTranslateMessage(pMsg);
}
