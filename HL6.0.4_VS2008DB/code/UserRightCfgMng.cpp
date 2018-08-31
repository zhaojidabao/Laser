// UserRightCfgMng.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "UserRightCfgMng.h"
#include "AddUserDlg.h"
#include "EditUserNameDlg.h"
#include "EditUserRightPassword.h"
#include "MainFrm.h"
#include "LocNukeForHL.h"
  
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserRightCfgMng dialog
extern CArray<USERINFO, USERINFO> g_RightInfoList;

CUserRightCfgMng::CUserRightCfgMng(CWnd* pParent  /*= NULL*/)
	: CDialog(CUserRightCfgMng::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserRightCfgMng)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nItem    = 0; 
    m_nSubItem = 0;
}


void CUserRightCfgMng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserRightCfgMng)
	DDX_Control(pDX, IDC_LIST_USERNAMELIST, m_UserNameList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserRightCfgMng, CDialog)
	//{{AFX_MSG_MAP(CUserRightCfgMng)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USERNAMELIST, OnClickListUsernamelist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_USERNAMELIST, OnRclickListUsernamelist)
	ON_COMMAND(ID_ADDUSER, OnAddUser)
	ON_COMMAND(ID_DELUSER, OnDelUser)
    ON_COMMAND(ID_EDITUSERNAME, OnEidtUserName)
    ON_COMMAND(ID_EDITUSERPASSWORD, OnEidtUserPassword)
	ON_BN_CLICKED(IDC_CHECK_MARKNORMAL, OnCheckMarknormal)
	ON_BN_CLICKED(IDC_CHECK_MARKMULDOC, OnCheckMarkmuldoc)
	ON_BN_CLICKED(IDC_CHECK_CREATEOBJ, OnCheckCreateobj)
	ON_BN_CLICKED(IDC_CHECK_OBJEDIT, OnCheckObjedit)
	ON_BN_CLICKED(IDC_CHECK_SYSTEMSET, OnCheckSystemset)
	ON_BN_CLICKED(IDC_CHECK_CALIBSET, OnCheckCalibset)
	ON_BN_CLICKED(IDC_CHECK_FILESAVE, OnCheckFilesave)
	ON_BN_CLICKED(IDC_CHECK_OPENFILE, OnCheckOpenfile)
	ON_BN_CLICKED(IDC_CHECK_LAYMARKPARA, OnCheckLaymarkpara)
    ON_BN_CLICKED(IDC_CHECK_ADV_PARA, OnCheckADVpara)
	ON_BN_CLICKED(IDC_CHECK_MNG, OnCheckMng)
	ON_BN_CLICKED(IDC_CHECK_SET_DEFAULTUSER, OnCheckSetDefaultuser)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserRightCfgMng message handlers

void CUserRightCfgMng::OnOK() 
{
	// TODO: Add extra validation here
	CString strFileName("");
	g_sys.WriteRightInfo(strFileName);
	CDialog::OnOK();
}

//
BOOL CUserRightCfgMng::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_UserNameList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString str;
	str.LoadString(IDS_USER_USER);
	m_UserNameList.InsertColumn(0,str,LVCFMT_LEFT,70);
	str.LoadString(IDS_USER_TYPE);
	m_UserNameList.InsertColumn(1,str,  LVCFMT_LEFT,50);
	
	int nCount = g_RightInfoList.GetSize();
	for (int i  = 0; i < nCount; i++)
	{
		USERINFO userInfo = g_RightInfoList.GetAt(i);
		CString strUserName("");
		if (userInfo.bIsDefaultUser) 
		{
			strUserName.Format(_T("%s *"), userInfo.szUserName);
		}
		else
		{
			strUserName.Format(_T("%s"), userInfo.szUserName);
		}
		m_UserNameList.InsertItem(i, strUserName, 0);
		
		CString strMsg(_T(""));
		if (userInfo.dwRight1 & 1)
		{
			strMsg.LoadString(IDS_MANAGER);
		}
		else
		{
			strMsg.LoadString(IDS_USER);
		}
		m_UserNameList.SetItemText(i, 1, strMsg);
	}

	CString strName(g_CurUserInfo.szUserName);
	m_nItem = g_sys.GetIndexByName(strName);
	m_UserNameList.SetItemState(m_nItem, LVIS_SELECTED, LVIS_SELECTED);
	SetRightItemStatus(g_CurUserInfo);
	UpdateView();
	
	return TRUE;
}

//
void CUserRightCfgMng::SetRightItemStatus(USERINFO &UserInfo) 
{	
	if (UserInfo.bIsDefaultUser)
	{
		((CButton*)GetDlgItem(IDC_CHECK_SET_DEFAULTUSER))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_SET_DEFAULTUSER))->SetCheck(FALSE);
	}
	
	if (UserInfo.dwRight1 & 1)
	{
		((CButton*)GetDlgItem(IDC_CHECK_MNG))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_MNG))->SetCheck(FALSE);
	}
	
	if (UserInfo.dwRight1 & 2)
	{
		((CButton*)GetDlgItem(IDC_CHECK_MARKNORMAL))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_MARKNORMAL))->SetCheck(FALSE);
	}

	if (UserInfo.dwRight1 & 4)
	{
		((CButton*)GetDlgItem(IDC_CHECK_MARKMULDOC))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_MARKMULDOC))->SetCheck(FALSE);
	}
	
	if (UserInfo.dwRight1 & 8)
	{
		((CButton*)GetDlgItem(IDC_CHECK_CREATEOBJ))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_CREATEOBJ))->SetCheck(FALSE);
	}

    if (UserInfo.dwRight1 & 16)
	{
		((CButton*)GetDlgItem(IDC_CHECK_OBJEDIT))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_OBJEDIT))->SetCheck(FALSE);
	}

    if (UserInfo.dwRight1 & 32)
	{
		((CButton*)GetDlgItem(IDC_CHECK_SYSTEMSET))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_SYSTEMSET))->SetCheck(FALSE);
	}

    if (UserInfo.dwRight1 & 64)
	{
		((CButton*)GetDlgItem(IDC_CHECK_CALIBSET))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_CALIBSET))->SetCheck(FALSE);
	}

    if (UserInfo.dwRight1 & 128)
	{
		((CButton*)GetDlgItem(IDC_CHECK_FILESAVE))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_FILESAVE))->SetCheck(FALSE);
	}

    if (UserInfo.dwRight1 & 256)
	{
		((CButton*)GetDlgItem(IDC_CHECK_OPENFILE))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_OPENFILE))->SetCheck(FALSE);
	}

    if (UserInfo.dwRight1 & 512)
	{
		((CButton*)GetDlgItem(IDC_CHECK_LAYMARKPARA))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_LAYMARKPARA))->SetCheck(FALSE);
	}

    if (UserInfo.dwRight1 & 1024)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ADV_PARA))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_ADV_PARA))->SetCheck(FALSE);
	}
}


void CUserRightCfgMng::GetRightItemStatus(USERINFO &UserInfo) 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SET_DEFAULTUSER))->GetCheck();
	if (bRet)
	{
		UserInfo.bIsDefaultUser = TRUE;
	}
	else
	{
		UserInfo.bIsDefaultUser = FALSE;
	}

	bRet = ((CButton*)GetDlgItem(IDC_CHECK_MNG))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  1;
	}
	else
	{
		UserInfo.dwRight1  &=  ~1;
	}
	
	bRet = ((CButton*)GetDlgItem(IDC_CHECK_MARKNORMAL))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  2;
	}
	else
	{
		UserInfo.dwRight1  &=  ~2;
	}

	bRet = ((CButton*)GetDlgItem(IDC_CHECK_MARKMULDOC))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  4;
	}
	else
	{
		UserInfo.dwRight1  &=  ~4;
	}

	bRet = ((CButton*)GetDlgItem(IDC_CHECK_CREATEOBJ))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  8;
	}
	else
	{
		UserInfo.dwRight1  &=  ~8;
	}

	bRet = ((CButton*)GetDlgItem(IDC_CHECK_OBJEDIT))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  16;
	}
	else
	{
		UserInfo.dwRight1  &=  ~16;
	}

    bRet = ((CButton*)GetDlgItem(IDC_CHECK_SYSTEMSET))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  32;
	}
	else
	{
		UserInfo.dwRight1  &=  ~32;
	}

    bRet = ((CButton*)GetDlgItem(IDC_CHECK_CALIBSET))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  64;
	}
	else
	{
		UserInfo.dwRight1  &=  ~64;
	}
	
	bRet = ((CButton*)GetDlgItem(IDC_CHECK_FILESAVE))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  128;
	}
	else
	{
		UserInfo.dwRight1  &=  ~128;
	}

    bRet = ((CButton*)GetDlgItem(IDC_CHECK_OPENFILE))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  256;
	}
	else
	{
		UserInfo.dwRight1  &=  ~256;
	}

    bRet = ((CButton*)GetDlgItem(IDC_CHECK_LAYMARKPARA))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  512;
	}
	else
	{
		UserInfo.dwRight1  &=  ~512;
	}

	bRet = ((CButton*)GetDlgItem(IDC_CHECK_ADV_PARA))->GetCheck();
	if (bRet)
	{
		UserInfo.dwRight1  |=  1024;
	}
	else
	{
		UserInfo.dwRight1  &=  ~1024;
	}
}

BOOL CUserRightCfgMng::HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem, int &nSubItem)
{
    if (pCtrl  == NULL) return FALSE ;
	if (pNMHDR == NULL) return FALSE;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	nItem    = pCtrl->HitTest( point, NULL);
	nSubItem = -1;

	return TRUE;
}

void CUserRightCfgMng::OnClickListUsernamelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HitTestEx(&m_UserNameList, pNMHDR, m_nItem, m_nSubItem);
	if (-1  !=  m_nItem)
	{
		USERINFO userInfo = g_RightInfoList.GetAt(m_nItem);
	    SetRightItemStatus(userInfo);
	}
	UpdateView();
	
	*pResult = 0;
}

void CUserRightCfgMng::OnRclickListUsernamelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HitTestEx(&m_UserNameList, pNMHDR, m_nItem, m_nSubItem);

	CMenu menu,*pMenu;
	menu.LoadMenu(IDR_POP);
	pMenu = menu.GetSubMenu(8);
	if ( pMenu )
	{
		POINT point;
		NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	    point = pNMListView->ptAction;
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	
	*pResult = 0;
}

void CUserRightCfgMng::OnAddUser() 
{
	CAddUserDlg AddUserDlg;
	AddUserDlg.DoModal();

	if (AddUserDlg.m_bIsOk) 
	{
		USERINFO userInfo;
		_tcsncpy(userInfo.szUserName, AddUserDlg.m_strUserName, 64);
		_tcsncpy(userInfo.szPassWord, AddUserDlg.m_strPassword1, 64);
		int nCount = m_UserNameList.GetItemCount();
		if ( g_sys.AddUser(userInfo) )
		{
			m_UserNameList.InsertItem(nCount, userInfo.szUserName, 0);
			m_nItem = nCount;
		}
	}
}

void CUserRightCfgMng::OnDelUser() 
{
	if ( m_nItem > 0 )
	{
		USERINFO userInfo = g_RightInfoList.GetAt(m_nItem);
		CString strName(_T(""));
		strName = m_UserNameList.GetItemText(m_nItem, 0);
		
		strName.TrimRight(_T(" *"));
	    g_sys.DelUser(strName);
		m_UserNameList.DeleteItem(m_nItem);
		m_nItem = -1;
	}
}

void CUserRightCfgMng::OnEidtUserName() 
{
	if ( m_nItem > 0 )
	{
		CEditUserNameDlg EditUserNameDlg;
		EditUserNameDlg.m_strOldUserName = m_UserNameList.GetItemText(m_nItem, 0);
		EditUserNameDlg.m_strOldUserName.TrimRight(_T(" *"));
		EditUserNameDlg.DoModal();
		if (EditUserNameDlg.m_bIsOk)
		{
			USERINFO stUserInfo;
			g_sys.GetUserInfoByName(EditUserNameDlg.m_strOldUserName, stUserInfo);
			_tcsncpy(stUserInfo.szUserName, EditUserNameDlg.m_strNewUserName, 64);
			g_sys.EditUserInfoByUserName(stUserInfo, EditUserNameDlg.m_strOldUserName);
			if (stUserInfo.bIsDefaultUser) 
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("%s *"), stUserInfo.szUserName);
				m_UserNameList.SetItemText(m_nItem, 0, strMsg);
			}
			else
			{
				m_UserNameList.SetItemText(m_nItem, 0, stUserInfo.szUserName);
			}
		}
	}
}

void CUserRightCfgMng::OnEidtUserPassword() 
{
	if (-1  !=  m_nItem)
	{
		CEditUserRightPassword EditUserPasswordDlg;
		EditUserPasswordDlg.m_strUserName = m_UserNameList.GetItemText(m_nItem, 0);
		EditUserPasswordDlg.m_strUserName.TrimRight(_T(" *"));
		USERINFO stUserInfo;
		BOOL bRet = g_sys.GetUserInfoByName(EditUserPasswordDlg.m_strUserName, stUserInfo);
		if (!bRet)
		{
			AfxMessageBox(IDS_USERNAMEISNOEXIST);
			return;
		}

		EditUserPasswordDlg.DoModal();
	}
}


void CUserRightCfgMng::OnSaveRightChange() 
{
	if ( m_nItem > 0 ) 
	{
		CString strName = m_UserNameList.GetItemText(m_nItem, 0);
		strName.TrimRight(_T(" *"));
		USERINFO UserInfo;
		BOOL bRet = ::g_sys.GetUserInfoByName(strName, UserInfo);
		if ( bRet )
		{
			GetRightItemStatus(UserInfo);
			::g_sys.EditUserInfo(UserInfo);

			USERINFO userInfo = g_RightInfoList.GetAt(m_nItem);
			CString strMsg(_T(""));
			if (userInfo.dwRight1 & 1)
			{
				strMsg.LoadString(IDS_MANAGER);
			}
			else
			{
				strMsg.LoadString(IDS_USER);
			}
			m_UserNameList.SetItemText(m_nItem, 1, strMsg);

			if (0 == strName.CompareNoCase(g_CurUserInfo.szUserName))
			{
				g_CurUserInfo = userInfo;
			}
		}
	} 
}

void CUserRightCfgMng::OnSetDefautUser(BOOL bSel)
{
	if ( -1  !=  m_nItem ) 
	{
		CString strName = m_UserNameList.GetItemText(m_nItem, 0);
		strName.TrimRight(_T(" *"));
		g_sys.OnSetDefautUser(strName, bSel);
		
		int nCount = g_RightInfoList.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			USERINFO userInfo = g_RightInfoList.GetAt(i);
			CString strMsg(_T(""));
			if (userInfo.bIsDefaultUser) 
			{
				strMsg.Format(_T("%s *"), userInfo.szUserName);
			}
			else
			{
				strMsg.Format(_T("%s"), userInfo.szUserName);
			}
			m_UserNameList.SetItemText(i, 0, strMsg);
		}
	}
}

void CUserRightCfgMng::OnCheckMarknormal() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckMarkmuldoc() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckCreateobj() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckObjedit() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckSystemset() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckCalibset() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckFilesave() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckOpenfile() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckLaymarkpara() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckADVpara() 
{
	// TODO: Add your control notification handler code here
	OnSaveRightChange();

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if ( !pFrame )
		return;

	pFrame->m_PropDlg.SetParaListShow();
}

void CUserRightCfgMng::OnCheckMng() 
{
	OnSaveRightChange();
}

void CUserRightCfgMng::OnCheckSetDefaultuser() 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SET_DEFAULTUSER))->GetCheck();
	OnSetDefautUser(bRet);
}

void CUserRightCfgMng::FlashRightInfo()
{
	m_UserNameList.DeleteAllItems();
	int nCount = g_RightInfoList.GetSize();
	for (int i  = 0; i < nCount; i++)
	{
		USERINFO userInfo = g_RightInfoList.GetAt(i);
		CString strUserName(_T(""));
		if (userInfo.bIsDefaultUser) 
		{
			strUserName.Format(_T("%s *"), userInfo.szUserName);
		}
		else
		{
			strUserName.Format(_T("%s"), userInfo.szUserName);
		}

		m_UserNameList.InsertItem(i, strUserName, 0);
		CString strMsg(_T(""));
		if (userInfo.dwRight1 & 1)
		{
			strMsg.LoadString(IDS_MANAGER);
		}
		else
		{
			strMsg.LoadString(IDS_USER);
		}
		m_UserNameList.SetItemText(i, 1, strMsg);
	}

	CString strName(g_CurUserInfo.szUserName);
	m_nItem = g_sys.GetIndexByName(strName);
	m_UserNameList.SetItemState(m_nItem, LVIS_SELECTED, LVIS_SELECTED);
	SetRightItemStatus(g_CurUserInfo);
}

void CUserRightCfgMng::OnButtonImport() 
{
	static TCHAR szFilter[] = _T("Right File(*.DAT)|*.DAT||");
	CFileDialog dlg(TRUE,NULL, NULL, OFN_HIDEREADONLY, szFilter);
	////////////
	if ( dlg.DoModal()  !=  IDOK )
		return;

	CString strFileName = dlg.GetPathName();
	::g_sys.ReadRightInfo(strFileName);

	FlashRightInfo();
}

//
void CUserRightCfgMng::OnButtonExport() 
{
	CString strExportFileName("");
	static TCHAR szFilter[] = _T("Right File(*.DAT)|*.dat||");
	CFileDialog dlg(FALSE,NULL,_T(""),OFN_HIDEREADONLY,szFilter);
	if (dlg.DoModal() == IDOK)
	{
		strExportFileName = dlg.GetPathName();
		if (0 == strExportFileName.CompareNoCase(_T(""))) 
			return;

		int nIndex = strExportFileName.ReverseFind(_T('.'));
		if (-1 == nIndex) 
		{
			strExportFileName  +=  _T(".DAT");
		}
		else
		{
			CString strFileName = strExportFileName.Mid(0, nIndex);
			strFileName  +=  _T(".DAT");
			strExportFileName = strFileName;
		}
		g_sys.WriteRightInfo(strExportFileName);
	}
}

void CUserRightCfgMng::UpdateView()
{
	GetDlgItem(IDC_CHECK_MARKNORMAL)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_MARKMULDOC)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_CREATEOBJ)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_OBJEDIT)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_SYSTEMSET)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_CALIBSET)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_FILESAVE)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_OPENFILE)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_LAYMARKPARA)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_ADV_PARA)->EnableWindow( m_nItem > 0 );
	GetDlgItem(IDC_CHECK_MNG)->EnableWindow( m_nItem > 0 );
	
}
