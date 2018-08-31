// DlgChangeCard.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "System.h"
#include "DlgChangeCard.h"


// CDlgChangeCard 对话框
IMPLEMENT_DYNAMIC(CDlgChangeCard, CDialog)

CDlgChangeCard::CDlgChangeCard(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgChangeCard::IDD, pParent)
{
	m_nCardIndex = 0;
}

CDlgChangeCard::~CDlgChangeCard()
{
}

void CDlgChangeCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CARDINFO, m_comboCard);
}


BEGIN_MESSAGE_MAP(CDlgChangeCard, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_CARDINFO, &CDlgChangeCard::OnCbnSelchangeComboCardinfo)
	ON_BN_CLICKED(IDOK, &CDlgChangeCard::OnBnClickedOk)
END_MESSAGE_MAP()




BOOL CDlgChangeCard::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString strDogId;
	TCHAR szCardAlias[50] = {0};
	m_comboCard.ResetContent();
	/////////////////
	int nSize = g_DogCheck.m_arrDogValid.GetCount();
	for(int i = 0 ; i < nSize; i++ )
	{
		strDogId = g_DogCheck.m_arrDogValid[i];
		::GetPrivateProfileString(_T("CardName"), strDogId, strDogId,szCardAlias, 50, g_sys.m_strCardFile);

		m_comboCard.AddString(szCardAlias);
	}
	m_comboCard.SetCurSel(m_nCardIndex);

 	UpdateData(FALSE);
	
	return TRUE;
}

// CDlgSelCard 消息处理程序
void CDlgChangeCard::OnCbnSelchangeComboCardinfo()
{
	UpdateData(TRUE);
	//
	int nSel = m_comboCard.GetCurSel();
	if ( nSel == LB_ERR )
		return;

}


void CDlgChangeCard::OnBnClickedOk()
{
	UpdateData(TRUE);
	int nSel = m_comboCard.GetCurSel();
	if ( nSel == LB_ERR )
		return;
	//
	CString strSelDog,strOrgDog;
	m_comboCard.GetLBText(nSel,strSelDog);
	///////////
	int  nCardCount = g_DAT.GetCardNumber();
	if ( nCardCount > 1 )
	{
		TCHAR  szCardAlias[50] = { 0 };
		TCHAR  szCardKey[10][50] =  { 0 };
		int     nSize = 10;
		g_DAT.EnumCardKey(szCardKey,nSize);
		/////////
		for ( int nIndex = 0; nIndex < nCardCount; nIndex++ )
		{
			CString strKey = szCardKey[nIndex];
			////////
			memset(szCardAlias, 0, 50);
			::GetPrivateProfileString(_T("CardName"), strKey, strKey, szCardAlias, 50, g_sys.m_strCardFile);

			if ( 0 == strSelDog.CompareNoCase(szCardAlias) )
			{
				strOrgDog = szCardKey[nIndex];
				m_nCardIndex = nIndex;
				if ( !g_DogCheck.__SwitchDog(strOrgDog) )
					OutputDebugString(_T("SwitchDog failure...\n"));
				break;
			}
		}
	}
	else
	{
		m_nCardIndex = 0;
	}

	OnOK();
}
