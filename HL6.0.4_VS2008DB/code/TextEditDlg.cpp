// TextEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextEditDlg.h"
#include "TextSerialDlg.h"
#include "FntHzk.h"
#include "baseMacro.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg dialog

CTextEditDlg::CTextEditDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CTextEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextEditDlg)
	m_strFontCHS = _T("");
	m_strFontEN = _T("");
	m_strFontNUM = _T("");
	m_strFontUser = _T("");
	m_nTextType = 0;
	m_nFontType = 0;
	m_strDate = _T("");
	m_strFix  = _T("");
	m_bModify = FALSE;
	m_dbFontHeight = 0.0;
	m_strHzkType = _T("");
	m_strAscType = _T("");
	m_pNodeText   = NULL;
	m_pCurSelText = NULL;
	m_strModelName = _T("");
	//}}AFX_DATA_INIT
}


void CTextEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextEditDlg)
	DDX_Control(pDX, IDC_COMBO_MATRIXENN, m_cbxAscType);
	DDX_Control(pDX, IDC_COMBO_MATRIXCHS, m_cbxHzkType);
	DDX_Control(pDX, IDC_COMBO_FontTTF, m_comFontTTF);
	DDX_Control(pDX, IDC_COMBO_USER, m_comUserFont);
	DDX_Control(pDX, IDC_COMBO_CHS, m_comShxChs);
	DDX_Control(pDX, IDC_COMBO_EN, m_comShxEn);
	DDX_Control(pDX, IDC_COMBO_NUM, m_comShxNum);
	DDX_Control(pDX, IDC_COMBO_FontType, m_comFontType);
	DDX_Control(pDX, IDC_COMBO_TextType, m_comTextType);
	DDX_CBString(pDX, IDC_COMBO_CHS, m_strFontCHS);
	DDX_CBString(pDX, IDC_COMBO_EN, m_strFontEN);
	DDX_CBString(pDX, IDC_COMBO_NUM, m_strFontNUM);
	DDX_CBString(pDX,IDC_COMBO_USER,m_strFontUser);
	DDX_CBIndex(pDX, IDC_COMBO_TextType, m_nTextType);
	DDX_CBIndex(pDX, IDC_COMBO_FontType, m_nFontType);
	DDX_Control(pDX, IDC_LIST_EDITDATA, m_listData);
	DDX_Text(pDX, IDC_DATE_EDIT, m_strDate);
	DDX_Text(pDX, IDC_FIX_EDIT, m_strFix);
	DDX_Check(pDX, IDC_CHECK_MODIFY, m_bModify);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dbFontHeight);
	DDV_MinMaxDouble(pDX, m_dbFontHeight, 0., 999.);
	DDX_CBString(pDX, IDC_COMBO_MATRIXCHS, m_strHzkType);
	DDX_CBString(pDX, IDC_COMBO_MATRIXENN, m_strAscType);
	DDX_Text(pDX, IDC_EDIT_TEXTID, m_strModelName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextEditDlg, CDialog)
	//{{AFX_MSG_MAP(CTextEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL, OnButtonSerial)
    ON_WM_TIMER()
	ON_EN_CHANGE(IDC_DATE_EDIT, OnChangeDateEdit)
	ON_BN_CLICKED(IDC_BUTTON_TEXTADD, OnButtonTextadd)
	ON_BN_CLICKED(IDC_BUTTON_TEXTDEL, OnButtonTextdel)
	ON_BN_CLICKED(IDC_BUTTON_TEXTEDIT, OnButtonTextedit)
	ON_LBN_SELCHANGE(IDC_LIST_EDITDATA, OnSelchangeListEditdata)
	ON_WM_DESTROY()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, OnButtonAdvance)
	ON_CBN_SELCHANGE(IDC_COMBO_FontType, OnSelchangeCOMBOFontType)
	ON_CBN_SELCHANGE(IDC_COMBO_TextType, OnSelchangeCOMBOTextType)
	ON_CBN_SELCHANGE(IDC_COMBO_FontTTF, OnSelchangeCOMBOFontTTF)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_FIX_EDIT, &CTextEditDlg::OnEnKillfocusFixEdit)
	ON_BN_CLICKED(IDC_CHECK_MODIFY, &CTextEditDlg::OnBnClickedCheckModify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg message handlers

BOOL CTextEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	Layout();
	
	// TODO: Add extra initialization here
	UpdateDataView();
	
	m_pCurSelText = new CText();
	InitComBox();
	//
	m_comFontTTF.SetType(TYPE_TTF,_T("TEXT"));
	m_comUserFont.SetType(TYPE_USER,_T("TEXT"));
	m_comShxChs.SetType(TYPE_SHX,_T("大族激光"));
	m_comShxEn.SetType(TYPE_SHX,_T("TEXT"));
	m_comShxNum.SetType(TYPE_SHX,_T("1234"));
	
	UpdateDataView();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTextEditDlg::OnButtonSerial() 
{
	CTextSerialDlg dlg;
	dlg.m_nJumpStart		 =  m_pCurSelText->m_nJumpStart;
	dlg.m_nJumpEnd		 =  m_pCurSelText->m_nJumpEnd  ;
	dlg.m_strLeft			 =  m_pCurSelText->m_strLeft	 ;
	dlg.m_strRight			 =  m_pCurSelText->m_strRight  ;
	dlg.m_nJumpLen		 =  m_pCurSelText->m_nJumpLen  ;
	dlg.m_strPrefix			 =  m_pCurSelText->m_strPrefix ;
	dlg.m_nJumpStep		 =  m_pCurSelText->m_nJumpStep ;
	// 循环跳号
	dlg.m_bJumpLoop      = m_pCurSelText->m_bJumpLoop;
	dlg.m_nJumpInterval	 =  m_pCurSelText->m_nJumpDis  ;
	dlg.m_bFill				 =  m_pCurSelText->m_bFill     ;
	dlg.m_nFromFile		 =  m_pCurSelText->m_nFromFile ;
	dlg.m_strFromFile		 =  m_pCurSelText->m_strFromFile;
	
	dlg.m_pSerial			 =  &(m_pCurSelText->m_saSerial);   //
	dlg.m_pCur				 =  &(m_pCurSelText->m_nMarkTimes); //
	dlg.m_strLocal			 =  m_pCurSelText->m_strLocal;
	
	dlg.m_strBinText		 =  m_pCurSelText->m_strBinText;
	g_sys.m_bEnableUpdate = FALSE;
	
	if ( dlg.DoModal() == IDOK )
	{	
		m_pCurSelText->m_nText			 =  TEXT_SERIAL;
		m_pCurSelText->m_nJumpStart	 =  dlg.m_nJumpStart;
		m_pCurSelText->m_nJumpEnd		 =  dlg.m_nJumpEnd;
		m_pCurSelText->m_strLeft			 =  dlg.m_strLeft;
		m_pCurSelText->m_strRight		 =  dlg.m_strRight;
		m_pCurSelText->m_strPrefix		 =  dlg.m_strPrefix;
		m_pCurSelText->m_nJumpLen		 =  dlg.m_nJumpLen;
		m_pCurSelText->m_nJumpStep	 =  dlg.m_nJumpStep;
		// 循环跳号
		m_pCurSelText->m_bJumpLoop   =  dlg.m_bJumpLoop;
		m_pCurSelText->m_bFill				 =  dlg.m_bFill;
		m_pCurSelText->m_nFromFile		 =  dlg.m_nFromFile;
		m_pCurSelText->m_strFromFile	 =  dlg.m_strFromFile;
		m_pCurSelText->m_strBinText	 =  dlg.m_strBinText;
		m_pCurSelText->m_nJumpDis		 =  dlg.m_nJumpInterval;
		m_pCurSelText->m_nMarkTimes	 =  *(dlg.m_pCur);
		m_pCurSelText->m_strLocal		 =  dlg.m_strLocal;
		
		CString strSerial = GetSerialString();
		SetDlgItemText(IDC_SERIAL_EDIT, strSerial);
		m_comFontTTF.ResetString(strSerial);
	}
}


//用于配合RADIO刷新界面的
void CTextEditDlg::UpdateDataView()
{
	GetDlgItem(IDC_FIX_EDIT)->EnableWindow(0 == m_nTextType);
	GetDlgItem(IDC_SERIAL_EDIT)->EnableWindow(1 == m_nTextType);
	GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(1 == m_nTextType);
	GetDlgItem(IDC_DATE_EDIT)->EnableWindow(2 == m_nTextType);
	
	if ( m_pCurSelText  !=  NULL )
	{
		GetDlgItem(IDC_BUTTON_TEXTDEL)->EnableWindow(!(m_pCurSelText->m_bBarCode));
		GetDlgItem(IDC_BUTTON_TEXTADD)->EnableWindow(!(m_pCurSelText->m_bBarCode));
		GetDlgItem(IDC_COMBO_TextType)->EnableWindow(!(m_pCurSelText->m_bBarCode));
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(!(m_pCurSelText->m_bBarCode));
	}

	UpdateDlgTextItemView();
	UpdateDlgFontItemView();

	SetDlgItemText(IDC_SERIAL_EDIT,GetSerialString());
}


CString CTextEditDlg::GetSerialString()
{
	CString strSerial;
	strSerial.Empty();

	if ( NULL == m_pCurSelText )
		return strSerial;
	
	m_pCurSelText->GenString();
	strSerial = m_pCurSelText->GetString();

	return strSerial;
}


void CTextEditDlg::OnTimer(UINT nIDEvent) 
{
	if ( NULL == m_pCurSelText ) 
		return;
	
	CString strText = GenTimeString(VerifyTimeFormat(m_pCurSelText->m_strDate));
	SetDlgItemText(IDC_TIME_INFO,strText);
}



void CTextEditDlg::OnChangeDateEdit() 
{
	GetDlgItemText(IDC_DATE_EDIT,m_strDate);
	CString strText = GenTimeString(VerifyTimeFormat(m_strDate));
	m_comFontTTF.ResetString(strText);
}



void CTextEditDlg::SetTextInfo(CNode *pNode)
{
	if ( NULL == pNode || NODE_HLTEXT  !=  pNode->GetType()) 
		return;
	
	m_pNodeText = (CNodeText*)pNode;
	
	m_listData.ResetContent();
	m_TextList.RemoveAll();
	
	CString str;
	int  nCount = m_pNodeText->GetTextCount();
	for ( int i = 0; i < nCount; i++ )
	{
		CText *pText = (CText*)m_pNodeText->GetText(i);
		if ( NULL == pText )
			continue;
		
		pText->FlashText(0x00);
		str = pText->GetString();

		m_listData.InsertString(i, str);
		
		TEXTINFO TextInfo;
		TextInfo.nTexID = pText->GetTextID();
		TextInfo.nType  = pText->m_nText;
		
		m_TextList.Add(TextInfo);
		
		if ( 0 == i )
		{
			m_pCurSelText->CopyAll(pText);
		}
	}
	
	if ( m_pCurSelText  !=  NULL )
	{
		m_listData.SetCurSel(0);
		m_nTextType = m_pCurSelText->m_nText;
		m_strFix = m_pCurSelText->m_strFix;
		
		if ( TEXT_SERIAL == m_nTextType )
		{
			SetDlgItemText(IDC_SERIAL_EDIT, m_pCurSelText->GetString());
			m_comFontTTF.ResetString(m_pCurSelText->GetString());
		}
		m_strDate = m_pCurSelText->m_strDate;
		m_bModify = m_pCurSelText->m_bModel;
		m_strModelName = m_pCurSelText->m_strModelName;
		
		//
		m_strFontUser	 =  m_pNodeText->m_strFontUser;
		m_strFontCHS	 =  m_pNodeText->m_strFontCHS;
		m_strFontEN	 =  m_pNodeText->m_strFontEN;
		m_strFontNUM	 =  m_pNodeText->m_strFontNUM;
		m_nFontType	 =  m_pNodeText->m_nFont;
		m_dbFontHeight	 =  MyPrecision(m_pNodeText->m_dbHeight);
		m_strHzkType	 =  m_pNodeText->m_strHzkType;
		m_strAscType	 =  m_pNodeText->m_strAscType;
		
		m_lf = m_pNodeText->m_lf;
		m_comFontTTF.SetCurSelFont(m_lf);
	}

	UpdateData(FALSE);
	UpdateDataView();
}




BOOL CTextEditDlg::FindTextInfo(int nIndex, TEXTINFO &TextInfo) 
{
	int nCount = m_TextList.GetSize();
	if ( nIndex  >=  nCount )
	{
		return FALSE;
	}
	TextInfo = m_TextList[nIndex];
	
	return TRUE;
}



void CTextEditDlg::OnButtonTextadd()
{
	UpdateData(TRUE);
	/////
	if ( m_strFix.IsEmpty() )
		return;

	if ( !m_pNodeText || !m_pNodeText->IsSelect() || 0 == m_pNodeText->GetTextCount() )
		return;
	
	CText* pText = new CText();
	if ( NULL == pText )
		return;
	
	pText->m_nText   = m_nTextType;
	switch( m_nTextType ) 
	{
	case 0:
		{
			pText->m_strFix  = m_strFix;
		    break;
		}
	case 1:
		{
			pText->m_nJumpStart	 =  m_pCurSelText->m_nJumpStart;
			pText->m_nJumpEnd		 =  m_pCurSelText->m_nJumpEnd;
			pText->m_strLeft			 =  m_pCurSelText->m_strLeft;
			pText->m_strRight		 =  m_pCurSelText->m_strRight;
			pText->m_nJumpLen     = m_pCurSelText->m_nJumpLen;
			pText->m_strPrefix		 =  m_pCurSelText->m_strPrefix;
			pText->m_nJumpStep	 =  m_pCurSelText->m_nJumpStep;
			// 循环跳号
			pText->m_bJumpLoop	 =  m_pCurSelText->m_bJumpLoop;
			pText->m_nJumpDis		 =  m_pCurSelText->m_nJumpDis;
			pText->m_bFill				 =  m_pCurSelText->m_bFill;
			pText->m_nFromFile		 =  m_pCurSelText->m_nFromFile;
			pText->m_strFromFile	 =  m_pCurSelText->m_strFromFile;

			CString strInfo;
			int nCount = m_pCurSelText->m_saSerial.GetSize();
			for( int i = 0; i < nCount; i++ )
			{
				strInfo = m_pCurSelText->m_saSerial.GetAt(i);
				pText->m_saSerial.Add(strInfo);
			}
			
			pText->m_nMarkTimes  = m_pCurSelText->m_nMarkTimes;
			pText->m_strBinText    = m_pCurSelText->m_strBinText;
		    break;
		}

	default:
		break;
	}
	m_pNodeText->AddTextAll_NewID(pText);
	
	TEXTINFO textInfo;
	textInfo.nTexID = pText->GetTextID();
	textInfo.nType  = m_nTextType;
	m_TextList.Add(textInfo);
	
	m_pNodeText->CreateString(FALSE);
	
	ListFlash();
}

//
void CTextEditDlg::OnButtonTextdel()
{
	if ( m_listData.GetCount()  <=  1 )
		return;

	if ( !m_pNodeText || !m_pNodeText->IsSelect() )
		return;
	
	int nCurSel = m_listData.GetCurSel();
	if ( LB_ERR == nCurSel ) 
		return;

	TEXTINFO TextInfo;
	if ( !FindTextInfo(nCurSel, TextInfo) )
		return;
	
	int nCount = m_TextList.GetSize();
	for( int i = 0 ; i < nCount; i++ )
	{
		UINT nTextID = m_TextList[i].nTexID;
		if (nTextID == TextInfo.nTexID ) 
		{
			m_TextList.RemoveAt(i);
			break;
		}
	}
	
	m_pNodeText->DelTextByID(TextInfo.nTexID);
	if ( m_pNodeText->GetTextCount() > 0  ) 
		m_pNodeText->CreateString(FALSE);
	
	if ( 0 == m_TextList.GetSize() ) 
	{
		CHLDoc *pDoc = GetCurActiveDocument();
		if ( pDoc )	pDoc->Delete();
	}
	
	ListFlash();
}


void CTextEditDlg::OnButtonTextedit()
{
	if ( !m_pNodeText || !m_pNodeText->IsSelect() )
		return;
	
	int nCurSel = m_listData.GetCurSel();
	if ( LB_ERR == nCurSel )
		return;

	TEXTINFO TextInfo;
	if ( !FindTextInfo(nCurSel, TextInfo) )
		return;

	CText* pText = (CText*)m_pNodeText->GetTextByID(TextInfo.nTexID);
	if ( NULL == pText )
		return;
	
	UpdateData(TRUE);
	switch(m_nTextType) 
	{
	case 0://固定文本
		{
			pText->m_strFix	 =  m_strFix;
			break;
		}
	case 1://跳号
		{
			pText->CopyExceptID(m_pCurSelText);
			break;
		}
	case 2://日期
		{
			pText->m_strDate = m_strDate;
			break;
		}
	default:
		{
			break;
		}
	}
	pText->m_nText  = m_nTextType;
	pText->m_bModel = m_bModify;
	pText->m_strModelName = m_strModelName;
	m_pNodeText->CreateString(FALSE);
	
	ListFlash();
}


//
void CTextEditDlg::Apply()
{
	UpdateData(TRUE);
	//
	if ( !m_pNodeText || !m_pNodeText->IsSelect() )
		return;

	if ( m_pNodeText->GetLockFlg() == TRUE )
		return;
	
	int nCurSel = m_listData.GetCurSel();
	if ( LB_ERR == nCurSel ) 
		return;

	TEXTINFO textInfo;
	if ( !FindTextInfo(nCurSel, textInfo) )
		return;

	CHLView *pView = GetCurActiveView();
	if ( pView == NULL )
		return;
	
	CHLDoc *pDoc = pView->GetDocument();
	if ( NULL == pDoc )
		return;

	CText* pText = (CText*)m_pNodeText->GetTextByID(textInfo.nTexID);
	if ( NULL == pText )
		return;
	
	switch(m_nTextType)
	{
	case 0:
		{
			pText->m_nText  = m_nTextType;
			pText->m_strFix	 =  m_strFix;
			break;
		}
	case 1:
		{
			pText->CopyAll(m_pCurSelText);
			pText->m_nText  = m_nTextType;
			break;
		}
	case 2:
		{
			pText->m_strDate = m_strDate;
			pText->m_nText   = m_nTextType;
			
			m_strDate = VerifyTimeFormat(m_strDate);
			CString strText = GenTimeString(m_strDate);
			SetDlgItemText(IDC_TIME_INFO,strText);
			UpdateData(FALSE);
			break;
		}
	default:
		{
			break;
		}
	}
	
	pText->m_bModel = m_bModify;
	pText->m_strModelName = m_strModelName;
	
	m_pNodeText->m_strFontUser	 =  m_strFontUser;
	m_pNodeText->m_strFontCHS	 =  m_strFontCHS;
	m_pNodeText->m_strFontEN		 =  m_strFontEN;
	m_pNodeText->m_strFontNUM	 =  m_strFontNUM;
	m_pNodeText->m_nFont			 =  m_nFontType;
	m_pNodeText->m_dbHeight		 =  m_dbFontHeight;
	m_pNodeText->m_lf					 =  m_lf;
	m_pNodeText->m_strHzkType		 =  m_strHzkType;
	m_pNodeText->m_strAscType	 =  m_strAscType;
	
	m_pNodeText->CreateString(FALSE);
	
	pDoc->m_chain.RefreshBarText(pView,textInfo.nTexID);

	CTree *pTree = NULL;
	pTree  = (CTree*) m_pNodeText->GetParent();
	if ( pTree  !=  NULL  )
	{
		pTree->CalLimit();
		pTree->Fill();
	}
}

//
void CTextEditDlg::ListFlash(int nSel)
{
	m_listData.ResetContent();
	/////
	int nCount = m_TextList.GetSize();
	if ( nCount < 1 )
		return;

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	for ( int i = 0; i < nCount; i++ )
	{
		UINT nTextID = m_TextList[i].nTexID;
		CText* pText = (CText*)pDoc->m_chain.m_MgrText.GetTextByID(nTextID);
		if ( pText  !=  NULL )
		{
			m_listData.InsertString(i, pText->GetString());
		}
	}
	m_listData.SetCurSel(nSel);
	OnSelchangeListEditdata();
}

//
void CTextEditDlg::OnSelchangeListEditdata()
{
	int nCurSel = m_listData.GetCurSel();
	if ( nCurSel < 0 )
		return;
	
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return;

	UINT nTextID = m_TextList[nCurSel].nTexID;
	CText* pText = (CText*)pDoc->m_chain.m_MgrText.GetTextByID(nTextID);
	if ( pText == NULL )
		return;

	m_pCurSelText->CopyAll(pText);
	m_nTextType = m_TextList[nCurSel].nType;
	
	switch(m_nTextType)
	{
	case 0:
		{
			m_strFix = pText->GetString();
			break;
		}
	default:
		{
			break;
		}
	}
	
	m_strModelName = pText->m_strModelName;
	m_bModify = pText->m_bModel;

	UpdateData(FALSE);
	UpdateDataView();
}



BOOL CTextEditDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( ((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message) ) 
		&& (GetKeyState(VK_F10) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL , NULL);
		return TRUE;
	}
	
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F7) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
		return TRUE;
	}
	
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F11) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_DWONLOAD_MARKDATA , NULL);
		return TRUE;
	}

	if ( (WM_KEYDOWN == pMsg->message ) && (GetKeyState(VK_RETURN) & 0x8000) )
	{
		Apply();
		////////
		CHLDoc *pDoc = GetCurActiveDocument();
		if ( pDoc )
			pDoc->SetTrackerQuad(pDoc->GetLimit());
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}




void CTextEditDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	if ( m_pCurSelText )
	{
		delete m_pCurSelText;
		m_pCurSelText = NULL;
	}
}



void CTextEditDlg::OnCancelMode()
{
	CDialog::OnCancelMode();
}


void CTextEditDlg::OnNewDocment()
{
	m_pNodeText = NULL;
    KillTimer(1);
	
	int nCount = m_listData.GetCount();
	while( nCount-- )
	{
		m_listData.DeleteString(nCount);
	}
	m_listData.ResetContent();
	m_TextList.RemoveAll();
	
	m_strFix = _T("");
	UpdateData(FALSE);
}



BOOL CTextEditDlg::IsCreateDlg()
{
	return (NULL  !=  m_pCurSelText);
}



void CTextEditDlg::OnButtonAdvance()
{
	g_sys.m_bEnableUpdate = FALSE;

	if ( !m_pNodeText )
		return;
	
	CHLDoc* pDoc = GetCurActiveDocument();
	if ( NULL == pDoc ) 
		return;
	
	pDoc->EditTextNode(m_pNodeText);
}



//
void CTextEditDlg::InitComBox()
{
	//初始化字体信息
	CString path = g_sys.GetInstallPath();
	
	CString searchFile;
	CString font;
	CFileFind finder;
    BOOL bWorking;
	
	//shx 字体
	searchFile = path+_T("\\pfonts\\*.shx");
	bWorking = finder.FindFile(searchFile);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-4);
		m_comShxChs.AddString(font);
	}
	m_comShxChs.SelectString(-1,m_strFontCHS);
	
	searchFile = path+_T("\\efonts\\*.shx");
	bWorking = finder.FindFile(searchFile);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-4);
		m_comShxEn.AddString(font);
	}
	m_comShxEn.SelectString(-1,m_strFontEN);	
	
	searchFile = path+_T("\\efonts\\*.shx");
	bWorking = finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-4);
		m_comShxNum.AddString(font);
	}
	m_comShxNum.SelectString(-1,m_strFontNUM);
	
	//大族字体
	searchFile = path+_T("\\Font\\*.font");
	bWorking = finder.FindFile(searchFile);
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		font = finder.GetFileName();
		font = font.Left(font.GetLength()-5);
		m_comUserFont.AddString(font);
	}
	m_comUserFont.SelectString(-1, m_strFontUser);
	
	//TTF字体
	m_comFontTTF.InitFontComboBox();
	
	//文本类型
	CString strTextType = _T("");
	m_comTextType.ResetContent();
	
	strTextType.LoadString(IDS_TEXT_TYPE_FIX);
	m_comTextType.AddString(strTextType);

	strTextType.LoadString(IDS_TEXT_TYPE_SERIAL);
	m_comTextType.AddString(strTextType);
	
	strTextType.LoadString(IDS_TEXT_TYPE_TIME);
	m_comTextType.AddString(strTextType);

	m_comTextType.SetCurSel(-1);
	
	//字体类型
	CString strFontType = _T("");
	m_comFontType.ResetContent();
	
	strFontType.LoadString(IDS_FONT_TYPE_TTF);
	m_comFontType.AddString(strFontType);
	
	strFontType.LoadString(IDS_FONT_TYPE_SHX);
	m_comFontType.AddString(strFontType);
	
	strFontType.LoadString(IDS_FONT_TYPE_USER);
	m_comFontType.AddString(strFontType);
	
	strFontType.LoadString(IDS_FONT_TYPE_MATRIX);
	m_comFontType.AddString(strFontType);	
	
	m_comFontType.SetCurSel(-1);
	
	//Matrix
	m_cbxHzkType.AddString(FONT_HZK_12);
	m_cbxHzkType.AddString(FONT_HZK_14);
	m_cbxHzkType.AddString(FONT_HZK_16);
	m_cbxHzkType.SelectString(-1,m_strHzkType);
	m_cbxAscType.AddString(FONT_ASC_5_7);
	m_cbxAscType.AddString(FONT_ASC_8_16);
	m_cbxAscType.SelectString(-1,m_strAscType);
}




//
void CTextEditDlg::GetItemMovePos(int res, int obj,int &nX,int &nY)
{
	CWnd *hWin = NULL;
	
	RECT rectRes;
	RECT rectObj;
	
	hWin = GetDlgItem(obj);
	if (hWin  !=  NULL)
	{	
		hWin->GetWindowRect(&rectObj);
	}
	else
	{
		nX = nX = 0;
		return;
	}
	
	hWin = GetDlgItem(res);
	if (hWin  !=  NULL)
	{
		hWin->GetWindowRect(&rectRes);
	}
	else
	{
		nX = nX = 0;
		return;
	}
	
	nX = rectObj.left - rectRes.left;
	nY = rectObj.top - rectRes.top;
}




void CTextEditDlg::MoveDlgItem(int nItem, int nX,int nY)
{
	CWnd *pWnd = GetDlgItem(nItem);
	if ( pWnd == NULL)
		return;
	
	RECT rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	rect.left  +=  nX;
	rect.right  +=  nX;
	rect.top  +=  nY;
	rect.bottom  +=  nY;
	
	pWnd->MoveWindow(&rect);
}




//
void CTextEditDlg::UpdateDlgTextItemView()
{
	GetDlgItem(IDC_FIX_EDIT)->ShowWindow(0 == m_nTextType);
	GetDlgItem(IDC_CHECK_MODIFY)->EnableWindow(0 == m_nTextType);
	GetDlgItem(IDC_EDIT_TEXTID)->EnableWindow(0 == m_nTextType);
	
	GetDlgItem(IDC_BUTTON_SERIAL)->ShowWindow(1 == m_nTextType);
	GetDlgItem(IDC_SERIAL_EDIT)->ShowWindow(1 == m_nTextType);
	
	GetDlgItem(IDC_TIME_INFO)->ShowWindow(2 == m_nTextType);
	GetDlgItem(IDC_DATE_EDIT)->ShowWindow(2 == m_nTextType);
	
	GetDlgItem(IDC_STATIC_FontType)->ShowWindow(0 == m_nTextType||1 == m_nTextType||2 == m_nTextType);
	GetDlgItem(IDC_COMBO_FontType)->ShowWindow(0 == m_nTextType||1 == m_nTextType||2 == m_nTextType);
	GetDlgItem(IDC_STATIC_FontHeight)->ShowWindow(0 == m_nTextType||1 == m_nTextType||2 == m_nTextType);
	GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(0 == m_nTextType||1 == m_nTextType||2 == m_nTextType);
}


//
void CTextEditDlg::UpdateDlgFontItemView()
{
	GetDlgItem(IDC_STATIC_FontTTF)->ShowWindow(0 == m_nFontType);
	GetDlgItem(IDC_COMBO_FontTTF)->ShowWindow(0 == m_nFontType);
	GetDlgItem(IDC_BUTTON_TTF)->ShowWindow(0 == m_nFontType);
	GetDlgItem(IDC_STATIC_CHS)->ShowWindow(1 == m_nFontType);				
	GetDlgItem(IDC_COMBO_CHS)->ShowWindow(1 == m_nFontType);
	GetDlgItem(IDC_STATIC_EN)->ShowWindow(1 == m_nFontType);		
	GetDlgItem(IDC_COMBO_EN)->ShowWindow(1 == m_nFontType);
	GetDlgItem(IDC_STATIC_NUM)->ShowWindow(1 == m_nFontType);
	GetDlgItem(IDC_COMBO_NUM)->ShowWindow(1 == m_nFontType);
	GetDlgItem(IDC_STATIC_USER)->ShowWindow(2 == m_nFontType);
	GetDlgItem(IDC_COMBO_USER)->ShowWindow(2 == m_nFontType);
	GetDlgItem(IDC_STATIC_CHSM)->ShowWindow(3 == m_nFontType);
	GetDlgItem(IDC_COMBO_MATRIXCHS)->ShowWindow(3 == m_nFontType);
	GetDlgItem(IDC_STATIC_ENNM)->ShowWindow(3 == m_nFontType);
	GetDlgItem(IDC_COMBO_MATRIXENN)->ShowWindow(3 == m_nFontType);
}


//
void CTextEditDlg::OnSelchangeCOMBOFontType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateDataView();
}


//
void CTextEditDlg::OnSelchangeCOMBOTextType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CString strMsg;
	if (m_nTextType == 0 || m_nTextType == 1)
	{
		KillTimer(1);
		GetDlgItemText( (m_nTextType == 0 ? IDC_FIX_EDIT : IDC_SERIAL_EDIT),strMsg);
		m_comFontTTF.ResetString(strMsg);
	}
	else
	{
		OnChangeDateEdit();
		SetTimer(1,1000,NULL);
	}
	UpdateDataView();
}


//
void CTextEditDlg::OnSelchangeCOMBOFontTTF() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	m_lf = m_comFontTTF.GetCurSelFont();
}



void CTextEditDlg::Layout()
{
	int nMoveX = 0, nMoveY = 0;
	
	GetItemMovePos(IDC_FIX_EDIT,IDC_STATIC_RECT1,nMoveX,nMoveY);		
	MoveDlgItem(IDC_FIX_EDIT,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_STATIC_FontType,IDC_STATIC_RECT2,nMoveX,nMoveY);		
	MoveDlgItem(IDC_STATIC_FontType,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_FontType,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_STATIC_FontHeight,IDC_STATIC_RECT4,nMoveX,nMoveY);		
	MoveDlgItem(IDC_STATIC_FontHeight,nMoveX,nMoveY);
	MoveDlgItem(IDC_EDIT_HEIGHT,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_BUTTON_SERIAL,IDC_STATIC_RECT1,nMoveX,nMoveY);		
	MoveDlgItem(IDC_BUTTON_SERIAL,nMoveX,nMoveY);
	MoveDlgItem(IDC_SERIAL_EDIT,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_TIME_INFO,IDC_STATIC_RECT1,nMoveX,nMoveY);		
	MoveDlgItem(IDC_TIME_INFO,nMoveX,nMoveY);
	MoveDlgItem(IDC_DATE_EDIT,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_STATIC_FontTTF,IDC_STATIC_RECT3,nMoveX,nMoveY);		
	MoveDlgItem(IDC_STATIC_FontTTF,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_FontTTF,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_STATIC_CHS,IDC_STATIC_RECT3,nMoveX,nMoveY);		
	MoveDlgItem(IDC_STATIC_CHS,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_CHS,nMoveX,nMoveY);
	MoveDlgItem(IDC_STATIC_EN,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_EN,nMoveX,nMoveY);
	MoveDlgItem(IDC_STATIC_NUM,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_NUM,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_STATIC_USER,IDC_STATIC_RECT3,nMoveX,nMoveY);		
	MoveDlgItem(IDC_STATIC_USER,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_USER,nMoveX,nMoveY);
	
	GetItemMovePos(IDC_STATIC_CHSM,IDC_STATIC_RECT3,nMoveX,nMoveY);		
	MoveDlgItem(IDC_STATIC_CHSM,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_MATRIXCHS,nMoveX,nMoveY);
	MoveDlgItem(IDC_STATIC_ENNM,nMoveX,nMoveY);
	MoveDlgItem(IDC_COMBO_MATRIXENN,nMoveX,nMoveY);
}


void CTextEditDlg::OnEnKillfocusFixEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strData;
	GetDlgItemText(IDC_FIX_EDIT,strData);
	//
	m_comFontTTF.ResetString(strData);
}


void CTextEditDlg::OnBnClickedCheckModify()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}



