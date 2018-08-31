// DlgSetInput.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSetInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetInput dialog


CDlgSetInput::CDlgSetInput(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgSetInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwInput = 0x0;		
	m_dwInputMask = 0x0;
	m_pNode = NULL;
}


void CDlgSetInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetInput, CDialog)
	//{{AFX_MSG_MAP(CDlgSetInput)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
	ON_BN_CLICKED(IDC_CHECK13, OnCheck13)
	ON_BN_CLICKED(IDC_CHECK14, OnCheck14)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_BN_CLICKED(IDC_CHECK16, OnCheck16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetInput message handlers


UINT CDlgSetInput::FetchCrlID(UINT nIndex)
{
	UINT nID(0);

	switch(nIndex) {
		case 0:		nID = IDC_CHECK1; break;
		case 1:		nID = IDC_CHECK2; break;
		case 2:		nID = IDC_CHECK3; break;
		case 3:		nID = IDC_CHECK4; break;
		case 4:		nID = IDC_CHECK5; break;
		case 5:		nID = IDC_CHECK6; break;
		case 6:		nID = IDC_CHECK7; break;
		case 7:		nID = IDC_CHECK8; break;
		case 8:		nID = IDC_CHECK9; break;
		case 9:		nID = IDC_CHECK10; break;
		case 10:		nID = IDC_CHECK11; break;
		case 11:		nID = IDC_CHECK12; break;
		case 12:		nID = IDC_CHECK13; break;
		case 13:		nID = IDC_CHECK14; break;
		case 14:		nID = IDC_CHECK15; break;
		case 15:		nID = IDC_CHECK16; break;
		case 16:		nID = IDC_CHECK17; break;
		case 17:		nID = IDC_CHECK18; break;
		case 18:		nID = IDC_CHECK19; break;
		case 19:		nID = IDC_CHECK20; break;
		case 20:		nID = IDC_CHECK21; break;
		case 21:		nID = IDC_CHECK22; break;
		case 22:		nID = IDC_CHECK23; break;
		case 23:		nID = IDC_CHECK24; break;
		case 24:		nID = IDC_CHECK25; break;
		case 25:		nID = IDC_CHECK26; break;
		case 26:		nID = IDC_CHECK27; break;
		case 27:		nID = IDC_CHECK28; break;
		case 28:		nID = IDC_CHECK29; break;
		case 29:		nID = IDC_CHECK30; break;
		case 30:		nID = IDC_CHECK31; break;
		case 31:		nID = IDC_CHECK32; break;
	} 

	return nID;
}

void CDlgSetInput::CmdRange(UINT nIndex)
{
	BOOL bMask = (((CButton*)GetDlgItem(FetchCrlID(nIndex)))->GetCheck() == 1) ? TRUE : FALSE;
	((CButton*)GetDlgItem(FetchCrlID(nIndex+16)))->EnableWindow(bMask);
	if (FALSE == bMask)
	{
		((CButton*)GetDlgItem(FetchCrlID(nIndex+16)))->SetCheck(0);
	}
}

void CDlgSetInput::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect())
		return;

	m_dwInputMask = 0;
	m_dwInput = 0;

	UINT iID(0), nID(0);
	for (iID = 0; iID < 16; ++iID) 
	{
		nID = FetchCrlID(iID);
		m_dwInputMask  |=  (((CButton*)GetDlgItem(nID))->GetCheck() == 1) ? (1<<iID) : 0;
	}

	for (iID = 16; iID < 32; ++iID) 
	{
		nID = FetchCrlID(iID);
		m_dwInput  |=  (((CButton*)GetDlgItem(nID))->GetCheck() == 1) ? (1<<(iID-16)) : 0;
	}
	
	m_pNode->Set(m_dwInput, m_dwInputMask);
}

void CDlgSetInput::SetInfo(CNode* pNode)
{
	if ( pNode == NULL || pNode->GetType() != NODE_INPUT )
		return;

	m_pNode = (CNodeInput*)pNode;
	m_pNode->Get(m_dwInput, m_dwInputMask);

	UINT iID(0), nID(0);
	for (iID = 0; iID < 16; ++iID) 
	{
		nID =  FetchCrlID(iID);
		((CButton*)GetDlgItem(nID))->SetCheck(((m_dwInputMask >> iID) & 0x1) ? TRUE : FALSE);
		nID =  FetchCrlID(iID+16);
		((CButton*)GetDlgItem(nID))->EnableWindow(((m_dwInputMask >> (iID)) & 0x1) ? TRUE : FALSE);
	}

	for (iID = 16; iID < 32; ++iID) 
	{
		nID = FetchCrlID(iID);
		((CButton*)GetDlgItem(nID))->SetCheck(((m_dwInput >> (iID-16)) & 0x1) ? TRUE : FALSE);
	}
}

BOOL CDlgSetInput::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( (WM_KEYDOWN == pMsg->message) && (VK_ESCAPE == pMsg->wParam) ) 
		return TRUE;

	if ( (WM_KEYDOWN == pMsg->message) && (VK_RETURN == pMsg->wParam) ) 
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}
