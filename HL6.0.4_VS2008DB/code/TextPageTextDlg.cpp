#include "stdafx.h"
#include "hl.h"
#include "TextPageTextDlg.h"
#include "TextSerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextPageTextDlg property page
IMPLEMENT_DYNCREATE(CTextPageTextDlg, CPropertyPage)

CTextPageTextDlg::CTextPageTextDlg():CPropertyPage(CTextPageTextDlg::IDD)
{
	//{{AFX_DATA_INIT(CTextPageRoundDlg)
	// NOTE: the ClassWizard will add member initialization here
	m_strDate = _T("");
	m_strFix = _T("");
	m_nJumpInterval = 1;
	m_nText = 0;
	//}}AFX_DATA_INIT

	m_bJumpLoop = FALSE;
	m_strFromFile = _T("");
	m_strLocal = _T("chs");
	m_pSerial = NULL;
	m_pCur = NULL;
}

CTextPageTextDlg::~CTextPageTextDlg()
{
}

void CTextPageTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextPageTextDlg)
	DDX_Text(pDX, IDC_DATE_EDIT, m_strDate);
	DDX_Text(pDX, IDC_FIX_EDIT, m_strFix);
	DDX_Radio(pDX, IDC_RADIO_FIX, m_nText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextPageTextDlg, CPropertyPage)
//{{AFX_MSG_MAP(CTextPageTextDlg)
	ON_BN_CLICKED(IDC_RADIO_FIX, OnRadioFix)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, OnRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_TIME, OnRadioTime)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL, OnButtonSerial)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_DATE_EDIT, OnChangeDateEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextPageTextDlg message handlers

void CTextPageTextDlg::OnRadioFix() 
{
	SetDlgItemText(IDC_TIME_INFO,_T(""));
	KillTimer(1);
	UpdateData();
	UpdateDataView();
}

void CTextPageTextDlg::OnRadioSerial() 
{
	SetDlgItemText(IDC_TIME_INFO,_T(""));
	KillTimer(1);
	UpdateData();
	UpdateDataView();
}

void CTextPageTextDlg::OnRadioTime() 
{
	UpdateData();
	SetTimer(1,1000,NULL);
	UpdateDataView();
}

//用于配合RADIO刷新界面的
void CTextPageTextDlg::UpdateDataView()
{
	BOOL bFlag;
	switch(m_nText) {
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
}



//
BOOL CTextPageTextDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	UpdateDataView();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




//
void CTextPageTextDlg::OnButtonSerial() 
{
	CTextSerialDlg dlg;
	
	dlg.m_nJumpStart		 =  m_nJumpStart;
	dlg.m_nJumpEnd			 =  m_nJumpEnd;
	dlg.m_strLeft			 =  m_strLeft;
	dlg.m_strRight			 =  m_strRight;
	dlg.m_nJumpLen			 =  m_nJumpLen;
	dlg.m_strPrefix			 =  m_strPrefix;
	dlg.m_nJumpStep			 =  m_nJumpStep;
	// 循环跳号
	dlg.m_bJumpLoop			 =  m_bJumpLoop;
	dlg.m_nJumpInterval		 =  m_nJumpInterval;
	dlg.m_bFill				 =  m_bFill;
	dlg.m_nFromFile			 =  m_nFromFile;
	dlg.m_strFromFile		 =  m_strFromFile;
	
	dlg.m_pCur				 =  m_pCur;
	dlg.m_pSerial			 =  m_pSerial;
	dlg.m_strFromFile		 =  m_strFromFile;
	dlg.m_strBinText		 =  m_strBinText;
	dlg.m_strLocal			 =  m_strLocal;
	
	if (dlg.DoModal() == IDOK)
	{
		m_nJumpStart	 =  dlg.m_nJumpStart;
		m_nJumpEnd		 =  dlg.m_nJumpEnd;
		m_strLeft		 =  dlg.m_strLeft;
		m_strRight		 =  dlg.m_strRight;
		m_strPrefix		 =  dlg.m_strPrefix;
		m_nJumpLen		 =  dlg.m_nJumpLen;
		m_nJumpStep		 =  dlg.m_nJumpStep;
		// 循环跳号
		m_bJumpLoop		 =  dlg.m_bJumpLoop;
		m_bFill			 =  dlg.m_bFill;
		m_nFromFile		 =  dlg.m_nFromFile;
		m_strFromFile	 =  dlg.m_strFromFile;
		m_strBinText	 =  dlg.m_strBinText;
		m_nJumpInterval	 =  dlg.m_nJumpInterval;
		m_strFromFile	 =  dlg.m_strFromFile;
		m_strLocal		 =  dlg.m_strLocal;
		
		SetDlgItemText(IDC_SERIAL_EDIT,GetSerialString());
	}
}


//
CString CTextPageTextDlg::GetSerialString()
{
	CString  str;
	LONGLONG nPos = *m_pCur;
	if ( m_nText == TEXT_SERIAL )
	{
		if ( m_nFromFile )//取于文件
		{
			str = GetFileString( (int)nPos );
		}
		else
		{
			//自定义跳号
			LONGLONG nItems = 0;
			if ( m_nJumpStep  )
				nItems = (m_nJumpEnd-m_nJumpStart)/m_nJumpStep+1;
			else
				nItems = 999999999999;//无穷大

			if ( nPos < nItems )
			{	
				LONGLONG nVal = m_nJumpStart+m_nJumpStep*nPos;
				//计算进制类型
				int  nCount = m_strBinText.GetLength();
				TCHAR szInfo[256] = { 0 };
				
				int  nPos = 254;
				UINT nPow = (UINT)( pow(nCount*1.0,1.0) + 0.5 );
				
				int nTmp(0),i(1);
				while( TRUE )
				{
					nTmp = nVal%nPow;
					szInfo[nPos] = m_strBinText.GetAt(nTmp);
					i++;
					if ( nPos == 0 ) 
						break;
					
					nPos--;
					if ( nVal < nPow )
						break;
					nVal = nVal/nPow;
				}
				
				str.Format(_T("%s"),szInfo+nPos+1);
				
				if ( m_bFill )
				{	
					int n = m_nJumpLen-str.GetLength();
					for( int j = 0; j<n; j++ )
						str = m_strPrefix + str;
				}
				str = m_strLeft + str + m_strRight;
			}
		}
	}

	return str;
}


//
CString CTextPageTextDlg::GetFileString(int nCur)
{
	//取自于文件
	CStdioFile fp;
	CString str = _T("");

	if ( !fp.Open(m_strFromFile,CFile::modeRead|CFile::typeText) )
		return str;

	int nCount = 0;
	while(fp.ReadString(str))
	{
		if ( nCount  >=  nCur )
			break;
		nCount++;
	}
	fp.Close();
	
	return str;
}




//
void CTextPageTextDlg::OnTimer(UINT nIDEvent) 
{
	m_strDate = VerifyTimeFormat(m_strDate);
	CString strText = GenTimeString(m_strDate);
	SetDlgItemText(IDC_TIME_INFO,strText);

	CPropertyPage::OnTimer(nIDEvent);
}

void CTextPageTextDlg::OnChangeDateEdit() 
{
	GetDlgItemText(IDC_DATE_EDIT,m_strDate);
}
