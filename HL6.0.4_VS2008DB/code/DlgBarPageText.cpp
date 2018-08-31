// DlgBarPageText.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgBarPageText.h"
#include "TextSerialDlg.h"
#include "Nodecodebar.h"
#include "baseMacro.h"

// CDlgBarPageText 对话框

IMPLEMENT_DYNAMIC(CDlgBarPageText, CDialog)

CDlgBarPageText::CDlgBarPageText(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgBarPageText::IDD, pParent)
{
	m_pText = NULL;
	m_pNodeCodeBar = NULL;
	m_nCurTextSel = -1;
	m_pSerial = NULL;
	m_pText = new CText();
}


CDlgBarPageText::~CDlgBarPageText()
{
	if ( m_pText  !=  NULL )
	{
		delete m_pText;
		m_pText = NULL;
	}

	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		CText *pText = m_list.GetNext(pos);
		if ( pText  !=  NULL )
		{
			delete pText;
			pText = NULL;
		}
	}
	m_list.RemoveAll();
}

void CDlgBarPageText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Text(pDX, IDC_DATE_EDIT, m_strDate);
	DDX_Text(pDX, IDC_FIX_EDIT, m_strFix);
	DDX_Radio(pDX, IDC_RADIO_FIX, m_nText);
}


BEGIN_MESSAGE_MAP(CDlgBarPageText, CDialog)
	ON_BN_CLICKED(IDC_BUT_ADD, &CDlgBarPageText::OnButAdd)
	ON_BN_CLICKED(IDC_BUT_DEL, &CDlgBarPageText::OnButDel)
	ON_BN_CLICKED(IDC_BUT_MODIFY, &CDlgBarPageText::OnButModify)
	ON_LBN_SELCHANGE(IDC_LIST_DATA, &CDlgBarPageText::OnSelchangeListData)
	ON_BN_CLICKED(IDC_RADIO_FIX, &CDlgBarPageText::OnRadioFix)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, &CDlgBarPageText::OnRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_TIME, &CDlgBarPageText::OnRadioTime)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL, &CDlgBarPageText::OnButtonSerial)
	ON_EN_CHANGE(IDC_DATE_EDIT, &CDlgBarPageText::OnChangeDateEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CDlgBarPageText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetTextInfo(m_pNodeCodeBar);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//CDlgBarPageText 消息处理程序
void CDlgBarPageText::OnButAdd()
{	
	UpdateData(TRUE);
	/////
	m_pText->m_nText  = m_nText;
	m_pText->m_strFix  = m_strFix;
	m_pText->m_strDate = m_strDate;
		
	CText* pText = new CText();
	if ( NULL == pText ) 
		return;

	pText->CopyExceptID(m_pText);
	m_list.AddTail(pText);

	m_nCurTextSel = m_list.GetSize()-1;
	ListFlash();
}


//
void CDlgBarPageText::OnButDel()
{
	if ( m_listData.GetCount()  <= 1 )
		return;

	int nIndex = m_listData.GetCurSel();
	if (  LB_ERR == nIndex ) 
		return;

	m_listData.DeleteString(nIndex);

	CText *pText = NULL;
	POSITION pos = m_list.FindIndex(nIndex);
	if ( pos  !=  NULL )
	{
		pText = m_list.GetAt(pos);
		if ( pText  !=  NULL )
			delete pText;
		m_list.RemoveAt(pos);
	}
	
	if ( m_nCurTextSel  >=  m_list.GetSize() )
		m_nCurTextSel = m_list.GetSize()-1;

	Update_Data(FALSE);
	ListFlash();
}

//
void CDlgBarPageText::OnButModify()
{
	Update_Data(TRUE);
	/////
	ListFlash();
}


void CDlgBarPageText::OnSelchangeListData() 
{
	m_nCurTextSel = m_listData.GetCurSel();

	Update_Data(FALSE);
	UpdateDlgView();
}


void CDlgBarPageText::ListFlash()
{
	m_listData.ResetContent();
	
	int nPos = 0;
	CText* pText = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pText = m_list.GetNext(pos);
		if ( NULL == pText ) 
			continue;

		m_listData.InsertString(nPos, pText->GetInfo());
		nPos++;
	}
	
	m_listData.SetCurSel(m_nCurTextSel);
}

//
void CDlgBarPageText::Update_Data (BOOL bSaveAndValidate)
{
	int nSize = m_list.GetSize();
	if ( nSize < 1 || m_nCurTextSel < 0 || m_nCurTextSel  >=  nSize)
		return;
	
	CText *pText = m_list.GetAt(m_list.FindIndex(m_nCurTextSel));
	if ( pText == NULL )
		return;
	
	if ( TRUE == bSaveAndValidate )
	{
		UpdateData( TRUE );

		m_pText->m_nText  = m_nText;
		m_pText->m_strFix  = m_strFix;
		m_pText->m_strDate = m_strDate;
		pText->CopyExceptID(m_pText);
	}
	else
	{
		m_pText->CopyExceptID(pText);
		m_nText	  = m_pText->m_nText;
		m_strFix  = m_pText->m_strFix;
		m_strDate = m_pText->m_strDate;
		
		UpdateData( FALSE );
	}
}


void CDlgBarPageText::OnRadioFix()
{
	SetDlgItemText(IDC_TIME_INFO,_T(""));

	KillTimer(1);
	UpdateData();
	UpdateDlgView();
}


void CDlgBarPageText::OnRadioSerial() 
{
	SetDlgItemText(IDC_TIME_INFO,_T(""));
	KillTimer(1);
	UpdateData();
	UpdateDlgView();
}


void CDlgBarPageText::OnRadioTime()
{
	UpdateData();
	SetTimer(1,1000,NULL);
	UpdateDlgView();
}


//
void CDlgBarPageText::UpdateDlgView()
{
	BOOL bFlag;
	switch(m_nText) 
	{
	case 0:
		//固定文本
		bFlag = (m_nText == 0);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(!bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(!bFlag);
		break;

	case 1:
		//跳号部分
		bFlag = (m_nText == 1);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(!bFlag);
		break;

	case 2:
		//时间日期
		bFlag = (m_nText == 2);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(!bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(bFlag);
		break;
	}

	//
	if (m_pNodeCodeBar  !=  NULL)
	{
		GetDlgItem(IDC_BUT_ADD)->EnableWindow( FALSE == m_pNodeCodeBar->m_bTextBar );
		GetDlgItem(IDC_BUT_DEL)->EnableWindow( FALSE == m_pNodeCodeBar->m_bTextBar );
	}
}


//
void CDlgBarPageText::OnButtonSerial()
{
	CTextSerialDlg dlg;
	
	dlg.m_nJumpStart	 =  m_pText->m_nJumpStart;
	dlg.m_nJumpEnd	 =  m_pText->m_nJumpEnd;
	dlg.m_strLeft		 =  m_pText->m_strLeft;
	dlg.m_strRight		 =  m_pText->m_strRight;
	dlg.m_nJumpLen	 =  m_pText->m_nJumpLen;
	dlg.m_strPrefix		 =  m_pText->m_strPrefix;
	dlg.m_nJumpStep	 =  m_pText->m_nJumpStep;
	// 循环跳号
	dlg.m_bJumpLoop = m_pText->m_bJumpLoop;
	dlg.m_nJumpInterval	 =  m_pText->m_nJumpDis;
	dlg.m_bFill			 =  m_pText->m_bFill;
	dlg.m_nFromFile	 =  m_pText->m_nFromFile;
	dlg.m_strFromFile	 =  m_pText->m_strFromFile;
	dlg.m_strBinText	 =  m_pText->m_strBinText;
	
	dlg.m_pSerial	    = &(m_pText->m_saSerial);
	dlg.m_pCur		    = &(m_pText->m_nMarkTimes);
	dlg.m_strLocal		 =  m_pText->m_strLocal;

	if ( dlg.DoModal() == IDOK )
	{
		m_pText->m_nJumpStart	 =  dlg.m_nJumpStart;
		m_pText->m_nJumpEnd		 =  dlg.m_nJumpEnd;
		m_pText->m_strLeft			 =  dlg.m_strLeft;
		m_pText->m_strRight		 =  dlg.m_strRight;
		m_pText->m_strPrefix		 =  dlg.m_strPrefix;
		m_pText->m_nJumpLen		 =  dlg.m_nJumpLen;
		m_pText->m_nJumpStep	 =  dlg.m_nJumpStep;
		// 循环跳号
		m_pText->m_bJumpLoop = dlg.m_bJumpLoop;
		m_pText->m_bFill				 =  dlg.m_bFill;
		m_pText->m_nFromFile		 =  dlg.m_nFromFile;
		m_pText->m_strFromFile	 =  dlg.m_strFromFile;
		m_pText->m_strBinText		 =  dlg.m_strBinText;
		m_pText->m_nJumpDis		 =  dlg.m_nJumpInterval;
		m_pText->m_nText			 =  1;
		m_pText->m_strLocal		 =  dlg.m_strLocal;
		
		SetDlgItemText(IDC_SERIAL_EDIT,GetSerialString());
	}
}


//
CString CDlgBarPageText::GetSerialString()
{
	CString strSerial = _T("");
	
	if ( m_pText  !=  NULL )
	{
		m_pText->GenString();
		strSerial = m_pText->GetString();
	}

	return strSerial;
}

//
CString CDlgBarPageText::GetFileString(int nCur)//取自于文件
{
	CStdioFile fp;
	int nCount = 0;
	
	CString str = _T("");
	if ( fp.Open(m_strFromFile,CFile::modeRead|CFile::typeText) )
	{
		while( fp.ReadString(str) )
		{
			if ( nCount  >=  nCur )
				break;

			nCount++;
		}
	}
	fp.Close();

	return str;
}

//
void CDlgBarPageText::OnTimer(UINT nIDEvent) 
{
	CString strText = GenTimeString(VerifyTimeFormat(m_strDate));
	SetDlgItemText(IDC_TIME_INFO,strText);
}

void CDlgBarPageText::OnChangeDateEdit() 
{
	GetDlgItemText(IDC_DATE_EDIT,m_strDate);
}


void CDlgBarPageText::SetTextInfo(CNode *pNode)
{
	if ( NULL == pNode || NODE_CODEBAR  !=  pNode->GetType() )
		return;

	CText *pText = NULL;
	CNodeCodebar* pNodeBar = (CNodeCodebar*)pNode;
	for( int i = 0; i < pNodeBar->GetTextCount(); i++)
	{
		pText = (CText*)pNodeBar->GetText(i);
		if ( pText == NULL )
			continue;

		CText *pNew = new CText();
		if ( pNew  !=  NULL )
		{
			pNew->CopyExceptID(pText);
			m_list.AddTail(pNew);
		}
	}
	ListFlash();

	pText = m_list.GetHead();
	if ( NULL == pText )
		return;

	m_pText->CopyExceptID(pText);
	
	m_nCurTextSel = 0;
	m_listData.SetCurSel(0);
	m_nText	  = m_pText->m_nText;
	m_strFix   = m_pText->m_strFix;
	m_strDate = m_pText->m_strDate;
	
	KillTimer(1);
	if ( TEXT_SERIAL == m_nText )
	{
		m_pText->GenString();
		SetDlgItemText(IDC_SERIAL_EDIT, m_pText->GetString());
	}
	else if ( TEXT_DATE == m_nText )
	{
		SetTimer(1,1000,NULL);
	}
		
	UpdateData(FALSE);
	UpdateDlgView();
}