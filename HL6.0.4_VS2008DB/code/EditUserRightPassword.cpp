// EditUserRightPassword.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "EditUserRightPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditUserRightPassword dialog


CEditUserRightPassword::CEditUserRightPassword(CWnd* pParent  /*= NULL*/)
	: CDialog(CEditUserRightPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditUserRightPassword)
	m_strOldPassword = _T("");
	m_strNewPassword1 = _T("");
	m_strNewPassword2 = _T("");
	m_strUserName = _T("");
	//}}AFX_DATA_INIT
}


void CEditUserRightPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditUserRightPassword)
	DDX_Text(pDX, IDC_EDIT_OLDPASSWORD, m_strOldPassword);
	DDX_Text(pDX, IDC_EDIT_NEWPASSWORD1, m_strNewPassword1);
	DDX_Text(pDX, IDC_EDIT_NEWPASSWORD2, m_strNewPassword2);
	DDX_Text(pDX, IDC_EDIT_ADDUSERNAME, m_strUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditUserRightPassword, CDialog)
	//{{AFX_MSG_MAP(CEditUserRightPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditUserRightPassword message handlers

BOOL CEditUserRightPassword::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditUserRightPassword::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	///////////////
	USERINFO stUserInfo;
	BOOL bRet = g_sys.GetUserInfoByName(m_strUserName, stUserInfo);
	if (!bRet)
	{
		AfxMessageBox(IDS_USERNAMEISNOEXIST);
		return;
	}
	
	if ( FALSE == m_RightMng.InfoCheck(m_strOldPassword, stUserInfo) )
	{
		AfxMessageBox(IDS_OLDPASSWORDNOOK);
		return;
	}
	
	m_nCount++;
	if ( m_nCount > 3 )
	{
	    CDialog::OnOK();
	}

	if ( 0  !=  m_strNewPassword1.CompareNoCase(m_strNewPassword2) )
	{
		AfxMessageBox(IDS_PASSWORDISNOOK);
		return;
	}
	else
	{
		stUserInfo.nFlag  &=  ~(0x0001);
		_tcsncpy(stUserInfo.szPassWord, m_strNewPassword1, 64);
		g_sys.EditUserInfo(stUserInfo);
		
		CDialog::OnOK();
		return;
	}
}

void CEditUserRightPassword::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}
