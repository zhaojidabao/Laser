// UserLogin.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "UserLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 
 
/////////////////////////////////////////////////////////////////////////////
// CUserLogin dialog
extern CArray<USERINFO, USERINFO> g_RightInfoList;

CUserLogin::CUserLogin(CWnd* pParent  /*= NULL*/)
	: CDialog(CUserLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLogin)
	m_strPassword = _T("");
	m_bIsOK       = FALSE;
	m_nLoginCount = 0;
	m_dwDispMsg   = 0;
	//}}AFX_DATA_INIT
}


void CUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLogin)
	DDX_Control(pDX, IDC_COMBO_USER, m_comboUser);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLogin, CDialog)
	//{{AFX_MSG_MAP(CUserLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUserLogin message handlers

BOOL CUserLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strmsg("");
	switch( m_dwDispMsg ) 
	{
	case 1:
		strmsg.LoadString(IDS_NODEFAULTUSER);
		break;
	case 2:
		strmsg.LoadString(IDS_USERINFOISNOOK);
		break;
	default:
		break;
	}
	GetDlgItem(IDC_STATIC_DISPMSG)->SetWindowText(strmsg);

	m_comboUser.ResetContent();
	int nCount = g_RightInfoList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		USERINFO userInfo = g_RightInfoList.GetAt(i);
		m_comboUser.AddString(userInfo.szUserName);		
	}
	m_comboUser.SelectString(0,g_CurUserInfo.szUserName);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserLogin::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	int nSel = m_comboUser.GetCurSel();
	if ( nSel <0 ) 	return;

	CString strUser;
	m_comboUser.GetLBText(nSel,strUser);
	BOOL bRet = g_sys.IsAvailUser(strUser, m_strPassword);
	if (!bRet)
	{
		AfxMessageBox(IDS_USERORPASSWORDNOOK);
		m_nLoginCount++;
		if ( m_nLoginCount  >=  3 )
		{
			CDialog::OnOK();
		}
	}
	else
	{
		m_bIsOK = TRUE;
		CDialog::OnOK();
		return;
	}

}
