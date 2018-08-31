// TextSerialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextSerialDlg.h"
#include "SetLangRegionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextSerialDlg dialog


CTextSerialDlg::CTextSerialDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CTextSerialDlg::IDD, pParent)
	, m_bChageMarkCnt(FALSE)
	, m_bJumpLoop(FALSE)
{
	//{{AFX_DATA_INIT(CTextSerialDlg)
	m_strPrefix = _T("");
	m_strLeft = _T("");
	m_strRight = _T("");
	m_nJumpLen = 0;
	m_bFill = FALSE;
	m_nFromFile = -1;
	m_nJumpStep = 0;
	m_nJumpInterval = 1;
	m_strBinText = _T("");
	m_bChageMarkCnt = FALSE;
	m_strJumpEnd = _T("");
	m_strJumpStart = _T("");
	//}}AFX_DATA_INIT
	m_nJumpStart = 0;
	m_nJumpEnd = 0;
	m_pSerial = NULL;
	m_strLocal = _T("chs");
}


void CTextSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextSerialDlg)
	DDX_Control(pDX, IDC_COMBO_SERIAL, m_comboSerial);
	DDX_Control(pDX, IDC_EDIT_PREFIX, m_editPrefix);
	DDX_Text(pDX, IDC_EDIT_PREFIX, m_strPrefix);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_strLeft);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_strRight);
	DDX_Text(pDX, IDC_EDIT_LEN, m_nJumpLen);
	DDX_Check(pDX, IDC_CHECK_FILL, m_bFill);
	DDX_Text(pDX, IDC_EDIT_START, m_strJumpStart);
	DDX_Text(pDX,IDC_EDIT_CURRENT,*m_pCur);
	DDX_Text(pDX, IDC_EDIT_END, m_strJumpEnd);
	DDX_Radio(pDX, IDC_RADIO_CUSTOM, m_nFromFile);
	DDX_Text(pDX, IDC_EDIT_STEP, m_nJumpStep);
	DDX_Text(pDX,IDC_EDIT_INTERVAL,m_nJumpInterval);
	DDX_Text(pDX, IDC_EDIT_BINTEXT, m_strBinText);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_bChageMarkCnt);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bJumpLoop);
}


BEGIN_MESSAGE_MAP(CTextSerialDlg, CDialog)
	//{{AFX_MSG_MAP(CTextSerialDlg)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	ON_BN_CLICKED(IDC_RADIO_FROM, OnRadioFrom)
	ON_BN_CLICKED(IDC_CHECK_FILL, OnCheckFill)
	ON_EN_KILLFOCUS(IDC_EDIT_LEN, OnKillfocusEditLen)
	ON_EN_KILLFOCUS(IDC_EDIT_START, OnKillfocusEditStart)
	ON_EN_KILLFOCUS(IDC_EDIT_END, OnKillfocusEditEnd)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIAL, OnSelchangeComboSerial)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDOK, &CTextSerialDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextSerialDlg message handlers

void CTextSerialDlg::OnRadioCustom() 
{
	// TODO: Add your control notification handler code here
	m_nFromFile = 0;
	UpdateView();
}

void CTextSerialDlg::OnRadioFrom() 
{
	// TODO: Add your control notification handler code here
	m_nFromFile = 1;
	UpdateView();
	
}

void CTextSerialDlg::UpdateView()
{
	if (m_nFromFile)
	{
		GetDlgItem(IDC_COMBO_SERIAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_CHECK_FILL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PREFIX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_SERIAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FILL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PREFIX)->EnableWindow(TRUE);
		
		OnCheckFill();
	}
}



//
void CTextSerialDlg::OnCheckFill() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_PREFIX)->EnableWindow(m_bFill);
	GetDlgItem(IDC_EDIT_LEN)->EnableWindow(m_bFill);
	UpdateData(FALSE);	
}


//
BOOL CTextSerialDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editPrefix.SetLimitText(1);
	
	if ( !m_strFromFile.IsEmpty() && m_pCur )
	{
		SetSerial_FromFile(m_strFromFile, *m_pCur, TRUE);
	}
	UpdateView();

	m_strJumpStart = TransFormat(m_nJumpStart);
	m_strJumpEnd   = TransFormat(m_nJumpEnd);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextSerialDlg::OnKillfocusEditLen() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	UINT lenMax = max(m_strJumpStart.GetLength(), m_strJumpEnd.GetLength());
	if ( m_nJumpLen < lenMax )
	{
		m_nJumpLen = lenMax;
		UpdateData(FALSE);
	}	
}

void CTextSerialDlg::OnKillfocusEditStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	UINT lenMax = max(m_strJumpStart.GetLength(), m_strJumpEnd.GetLength());
	if ( m_nJumpLen<lenMax )
	{
		m_nJumpLen = lenMax;
		UpdateData(FALSE);
	}	
}

void CTextSerialDlg::OnKillfocusEditEnd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	UINT lenMax = max(m_strJumpStart.GetLength(), m_strJumpEnd.GetLength());
	if ( m_nJumpLen < lenMax )
	{
		m_nJumpLen = lenMax;
		//////
		UpdateData(FALSE);
	}	
}



int CTextSerialDlg::GetLength(LONGLONG value)
{
	int i = 1;
	while( value /= 10 )
		i++;
	
	return i;
}


void CTextSerialDlg::OnButtonSet()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,NULL,m_strFromFile,OFN_HIDEREADONLY,_T("TEXT File(*.txt)|*.txt|All Files(*.*)|*.*||"));
	
	dlg.m_ofn.lpstrInitialDir = m_strFromFile;
	if ( dlg.DoModal() == IDOK )
	{
		m_strFromFile = dlg.GetPathName();
		SetSerial_FromFile(m_strFromFile, 0,FALSE);
	}
}


void CTextSerialDlg::SetSerial_FromFile(CString strFileName, int nCurSel,BOOL bDefault)
{
	//加入到列表中
	m_comboSerial.ResetContent();
	if ( strFileName.IsEmpty() )
		return;
	
	CStdioFileEx fp;	//取自于文件
	BOOL  bUnicode = CStdioFileEx::IsFileUnicode(strFileName);
	if ( !fp.Open( strFileName,CFile::modeRead|CFile::typeText) )
	{
		AfxMessageBox(_T("Can't open file:") + strFileName);
		return;
	}

	int       nItems(0);	//文件行数
	CString  strData;

	while( fp.ReadString(strData) )
		nItems++;

	if ( nItems > 32767 ){
		AfxMessageBox(_T("The file data is too large,does not allow more than 32767!"));
		fp.Close();
		return;
	}

	m_pSerial->SetSize(nItems);
	fp.SeekToBegin(); 

#ifdef _UNICODE
	char* old_locale = NULL;
	if ( !bUnicode )//unicode下读取Ansi文件
	{
		if ( bDefault )
		{
			char  szLocal[50] = {0};
			CString  strLocal = ::g_sys.GetIniString(_T("CHARSET"),_T("REGION_LANG"),_T("chs"));
			CStdioFileEx::GetMultiByteFromUnicode(strLocal.GetBuffer(),szLocal,50);
			strLocal.ReleaseBuffer();
			old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
			setlocale( LC_CTYPE,szLocal);
			m_strLocal = szLocal;
		}
		else
		{
			CSetLangRegionDlg dlg;
			if ( IDOK == dlg.DoModal() )
			{
				old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
				setlocale( LC_CTYPE,dlg.m_szLang);
				m_strLocal = dlg.m_szLang;
			}
			else
			{
				fp.Close();
				return ;
			}
		}
	}
#endif

	while( fp.ReadString(strData) )
		GetSerial(strData);

	fp.Close();
	
#ifdef _UNICODE
	if ( !bUnicode )//ANSI文件
	{
		setlocale( LC_CTYPE, old_locale ); //还原语言区域 
		free( old_locale );
	}
#endif

	m_comboSerial.SetCurSel(nCurSel);
	*m_pCur = m_comboSerial.GetCurSel();
	
	UpdateData(FALSE);
}


//
void CTextSerialDlg::GetSerial(CString strString)
{
	int index = m_comboSerial.AddString(strString);
	if ( index  >=  0 )
	{
		m_comboSerial.SetCurSel(index);
		m_pSerial->SetAt(index,strString);
	}
}

void CTextSerialDlg::SetSerial()
{
	if ( !m_pSerial || !m_pCur )
		return;

	int count = m_pSerial->GetSize();
	for(int i = 0;i < count;i++)
	{
		m_comboSerial.AddString(m_pSerial->GetAt(i));
		m_comboSerial.SetCurSel(i);
	}
	m_comboSerial.SetCurSel(*m_pCur);
}


//
void CTextSerialDlg::OnSelchangeComboSerial() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if ( m_pCur )
		*m_pCur = m_comboSerial.GetCurSel();
	/////
	UpdateData(FALSE);
}

void CTextSerialDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(m_bChageMarkCnt);
}



void CTextSerialDlg::OnOK()
{
	UpdateData();
	
	int nLen = m_strBinText.GetLength();
	if ( nLen  <=  0 )
	{
		AfxMessageBox(IDS_INVALIDINPUT);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_BINTEXT));
		return;
	}
	int i = 0;
	for(; i<m_strJumpStart.GetLength(); ++i ) 
	{
		if ( -1 == m_strBinText.Find(m_strJumpStart[i], 0) ) 
		{
			AfxMessageBox(IDS_INVALIDINPUT);
			GotoDlgCtrl(GetDlgItem(IDC_EDIT_START));
			return;
		}
	}

	for( i = 0; i<m_strJumpEnd.GetLength(); ++i )
	{
		if (-1 == m_strBinText.Find(m_strJumpEnd[i], 0))
		{
			AfxMessageBox(IDS_INVALIDINPUT);
			GotoDlgCtrl(GetDlgItem(IDC_EDIT_END));
			return;
		}
	}
	
	if ( 0 == m_nFromFile )
	{
		m_nJumpStart = TransFormat(m_strJumpStart);
		m_nJumpEnd   = TransFormat(m_strJumpEnd);
	}
	else if ( 1 == m_nFromFile )
	{
		m_nJumpStart = 0;
		m_nJumpEnd   = m_pSerial->GetSize()-1;
	}
	
	if ( ( m_nJumpStep  >=  0  && m_nJumpStart > m_nJumpEnd ) ||  ( m_nJumpStep <0  && m_nJumpStart < m_nJumpEnd ) )
	{
		AfxMessageBox(IDS_ERRORRANGE);
		GotoDlgCtrl(GetDlgItem(IDC_EDIT_START));
		return;
	}
	
	CDialog::OnOK();
}


LONGLONG CTextSerialDlg::TransFormat(CString strInput)
{
	int  nPos(0);
	int  nBase = m_strBinText.GetLength();
	LONGLONG nRet(0);
	///////
	for( int  i =  0; i < strInput.GetLength(); ++i ) 
	{
		nRet  *=  nBase;
		
		nPos = m_strBinText.Find(strInput[i],0);
		if ( nPos  >=  0 )
		{
			nRet  +=  nPos;
		}
	}

	return nRet;
}

CString CTextSerialDlg::TransFormat(LONGLONG nValue)
{
	int  nBase = m_strBinText.GetLength();
	if ( 0 == nBase ) 
		return _T("");
 
	CString strRet = _T("");

	int  nPos(0);
	while( TRUE )
	{
		nPos = (int)( nValue % nBase );
		strRet.Insert(0, m_strBinText.GetAt(nPos) );
			
		if ( nValue < nBase )
			break;

		if ( 1 == nBase ) 
			nValue--;
		else 
			nValue = nValue/nBase;
	}

	if ( nValue < 0 )
		strRet.Insert(0, '-');

	return strRet;
}




void CTextSerialDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

