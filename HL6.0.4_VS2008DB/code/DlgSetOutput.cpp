// DlgSetOutput.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgSetOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetOutput dialog
 
CDlgSetOutput::CDlgSetOutput(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgSetOutput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetOutput)
	m_dwOutputDelay = 0;
	//}}AFX_DATA_INIT
	m_pNode = NULL;
}


void CDlgSetOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetOutput)
	DDX_Text(pDX, IDC_EDIT_DELAY, m_dwOutputDelay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetOutput, CDialog)
	//{{AFX_MSG_MAP(CDlgSetOutput)
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
	ON_BN_CLICKED(IDC_CHECK17, OnCheck17)
	ON_BN_CLICKED(IDC_CHECK18, OnCheck18)
	ON_BN_CLICKED(IDC_CHECK19, OnCheck19)
	ON_BN_CLICKED(IDC_CHECK20, OnCheck20)
	ON_BN_CLICKED(IDC_CHECK21, OnCheck21)
	ON_BN_CLICKED(IDC_CHECK22, OnCheck22)
	ON_BN_CLICKED(IDC_CHECK23, OnCheck23)
	ON_BN_CLICKED(IDC_CHECK24, OnCheck24)
	ON_BN_CLICKED(IDC_CHECK25, OnCheck25)
	ON_BN_CLICKED(IDC_CHECK26, OnCheck26)
	ON_BN_CLICKED(IDC_CHECK27, OnCheck27)
	ON_BN_CLICKED(IDC_CHECK28, OnCheck28)
	ON_BN_CLICKED(IDC_CHECK29, OnCheck29)
	ON_BN_CLICKED(IDC_CHECK30, OnCheck30)
	ON_BN_CLICKED(IDC_CHECK31, OnCheck31)
	ON_BN_CLICKED(IDC_CHECK32, OnCheck32)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetOutput message handlers

UINT CDlgSetOutput::FetchCrlID(UINT nIndex)
{
	UINT nID(0);
	switch(nIndex)
	{
		case 0:		nID = IDC_CHECK1;		break;
		case 1:		nID = IDC_CHECK2;		break;
		case 2:		nID = IDC_CHECK3;		break;
		case 3:		nID = IDC_CHECK4;		break;
		case 4:		nID = IDC_CHECK5;		break;
		case 5:		nID = IDC_CHECK6;		break;
		case 6:		nID = IDC_CHECK7;		break;
		case 7:		nID = IDC_CHECK8;		break;
		case 8:		nID = IDC_CHECK9;		break;
		case 9:		nID = IDC_CHECK10;	break;
		case 10:		nID = IDC_CHECK11;	break;
		case 11:		nID = IDC_CHECK12;	break;
		case 12:		nID = IDC_CHECK13;	break;
		case 13:		nID = IDC_CHECK14;	break;
		case 14:		nID = IDC_CHECK15;	break;
		case 15:		nID = IDC_CHECK16;	break;
		case 16:		nID = IDC_CHECK17;	break;
		case 17:		nID = IDC_CHECK18;	break;
		case 18:		nID = IDC_CHECK19;	break;
		case 19:		nID = IDC_CHECK20;	break;
		case 20:		nID = IDC_CHECK21;	break;
		case 21:		nID = IDC_CHECK22;	break;
		case 22:		nID = IDC_CHECK23;	break;
		case 23:		nID = IDC_CHECK24; break;
		case 24:		nID = IDC_CHECK25; break;
		case 25:		nID = IDC_CHECK26; break;
		case 26:		nID = IDC_CHECK27; break;
		case 27:		nID = IDC_CHECK28; break;
		case 28:		nID = IDC_CHECK29; break;
		case 29:		nID = IDC_CHECK30; break;
		case 30:		nID = IDC_CHECK31; break;
		case 31:		nID = IDC_CHECK32; break;
		case 32:		nID = IDC_CHECK33; break;
		case 33:		nID = IDC_CHECK34; break;
		case 34:		nID = IDC_CHECK35; break;
		case 35:		nID = IDC_CHECK36; break;
		case 36:		nID = IDC_CHECK37; break;
		case 37:		nID = IDC_CHECK38; break;
		case 38:		nID = IDC_CHECK39; break;
		case 39:		nID = IDC_CHECK40; break;
		case 40:		nID = IDC_CHECK41; break;
		case 41:		nID = IDC_CHECK42; break;
		case 42:		nID = IDC_CHECK43; break;
		case 43:		nID = IDC_CHECK44; break;
		case 44:		nID = IDC_CHECK45; break;
		case 45:		nID = IDC_CHECK46; break;
		case 46:		nID = IDC_CHECK47; break;
		case 47:		nID = IDC_CHECK48; break;
		case 48:		nID = IDC_CHECK49; break;
		case 49:		nID = IDC_CHECK50; break;
		case 50:		nID = IDC_CHECK51; break;
		case 51:		nID = IDC_CHECK52; break;
		case 52:		nID = IDC_CHECK53; break;
		case 53:		nID = IDC_CHECK54; break;
		case 54:		nID = IDC_CHECK55; break;
		case 55:		nID = IDC_CHECK56; break;
		case 56:		nID = IDC_CHECK57; break;
		case 57:		nID = IDC_CHECK58; break;
		case 58:		nID = IDC_CHECK59; break;
		case 59:		nID = IDC_CHECK60; break;
		case 60:		nID = IDC_CHECK61; break;
		case 61:		nID = IDC_CHECK62; break;
		case 62:		nID = IDC_CHECK63; break;
		case 63:		nID = IDC_CHECK64; break;
	} 

	return nID;
}

void CDlgSetOutput::CmdRange(UINT nIndex)
{
	BOOL bMask = (((CButton*)GetDlgItem(FetchCrlID(nIndex)))->GetCheck() == 1) ? TRUE : FALSE;
	((CButton*)GetDlgItem(FetchCrlID(nIndex+32)))->EnableWindow(bMask);
	if ( FALSE == bMask )
	{
		((CButton*)GetDlgItem(FetchCrlID(nIndex+32)))->SetCheck(0);
	}
}

void CDlgSetOutput::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect() )
		return;
	
	UpdateData(TRUE);
	//////
	if ( m_dwOutputDelay < 0 ) 
	{
		m_dwOutputDelay = 0;
		UpdateData(FALSE);
	}
	else if (m_dwOutputDelay > 60000) 
	{
		m_dwOutputDelay = 60000;
		UpdateData(FALSE);
	}

	UINT iID(0), nID(0);
	m_dwOutputMask = 0;
	m_dwOutput = 0;

	for (iID = 0; iID < 32; ++iID) 
	{
		nID =  FetchCrlID(iID);
		m_dwOutputMask  |=  (((CButton*)GetDlgItem(nID))->GetCheck() == 1) ? (1<<iID) : 0;
	}

	for (iID = 32; iID < 64; ++iID) 
	{
		nID =  FetchCrlID(iID);
		m_dwOutput  |=  (((CButton*)GetDlgItem(nID))->GetCheck() == 1) ? (1<<(iID-32)) : 0;
	}

	m_pNode->Set(m_dwOutput, m_dwOutputMask, m_dwOutputDelay);
}

void CDlgSetOutput::SetInfo(CNode* pNode)
{
	if ( pNode == NULL || pNode->GetType() != NODE_OUTPUT )
		return;

	m_pNode = (CNodeOutput*)pNode;
	m_pNode->Get(m_dwOutput, m_dwOutputMask, m_dwOutputDelay);

	UINT iID(0), nID(0);
	for ( iID = 0; iID < 32; ++iID ) 
	{
		nID =  FetchCrlID(iID);
		((CButton*)GetDlgItem(nID))->SetCheck(((m_dwOutputMask >> iID) & 0x1) ? TRUE : FALSE);
		nID =  FetchCrlID(iID+32);
		((CButton*)GetDlgItem(nID))->EnableWindow(((m_dwOutputMask >> (iID)) & 0x1) ? TRUE : FALSE);
	}

	for (iID = 32; iID < 64; ++iID) 
	{
		nID	 =  FetchCrlID(iID);
		((CButton*)GetDlgItem(nID))->SetCheck(((m_dwOutput >> (iID-32)) & 0x1) ? TRUE : FALSE);
	}

	UpdateData(FALSE);
}

BOOL CDlgSetOutput::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( (WM_KEYDOWN == pMsg->message) && (VK_ESCAPE == pMsg->wParam) )
		return TRUE;

	if ( (WM_KEYDOWN == pMsg->message) && (VK_RETURN == pMsg->wParam) ) 
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}
