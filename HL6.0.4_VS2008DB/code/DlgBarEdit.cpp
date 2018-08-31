// DlgBarEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgBarEdit.h"
#include "TextSerialDlg.h"
#include "DataMatrixDlg.h"
#include "SetQRCodebar.h"
#include "Pdf417Dlg.h"
#include "baseMacro.h"
#include "DlgSetHanXinCode.h"

// CDlgBarEdit 对话框
IMPLEMENT_DYNAMIC(CDlgBarEdit, CDialog)

CDlgBarEdit::CDlgBarEdit(CWnd* pParent  /*= NULL*/)
: CDialog(CDlgBarEdit::IDD, pParent)
, m_bModel(FALSE)
, m_strModelName(_T(""))
{
	m_pText		 =  NULL;
	m_nCurSel  = -1;
	m_pNodeBar =  NULL;
	m_pSerial = NULL;
	m_nVersion = 0;
}

CDlgBarEdit::~CDlgBarEdit()
{
}

void CDlgBarEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Text(pDX, IDC_DATE_EDIT, m_strDate);
	DDX_Text(pDX, IDC_FIX_EDIT, m_strFix);
	DDX_Control(pDX, IDC_COMBO_BARCODETYPE, m_ctrlType);
	DDX_Check(pDX, IDC_CHECK_MODIFY, m_bModel);
	DDX_Text(pDX, IDC_EDIT_TEXTID, m_strModelName);
	DDX_Control(pDX, IDC_COMBO_TextType, m_comTextType);
	DDX_CBIndex(pDX, IDC_COMBO_TextType, m_nText);
}


BEGIN_MESSAGE_MAP(CDlgBarEdit, CDialog)
	ON_BN_CLICKED(IDC_BUT_BARCODESET, &CDlgBarEdit::OnBnClickedButBarcodeset)
	ON_BN_CLICKED(IDC_BUTTON_TEXTADD, &CDlgBarEdit::OnBnClickedButtonTextadd)
	ON_BN_CLICKED(IDC_BUTTON_TEXTDEL, &CDlgBarEdit::OnBnClickedButtonTextdel)
	ON_BN_CLICKED(IDC_BUTTON_TEXTEDIT, &CDlgBarEdit::OnBnClickedButtonTextedit)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, &CDlgBarEdit::OnBnClickedButtonAdvance)
	ON_LBN_SELCHANGE(IDC_LIST_DATA, &CDlgBarEdit::OnLbnSelchangeListData)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL, &CDlgBarEdit::OnBnClickedButtonSerial)
	ON_EN_CHANGE(IDC_DATE_EDIT, &CDlgBarEdit::OnEnChangeDateEdit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_MODIFY, &CDlgBarEdit::OnBnClickedCheckModify)
	ON_CBN_SELCHANGE(IDC_COMBO_BARCODETYPE, &CDlgBarEdit::OnCbnSelchangeComboBarcodetype)
	ON_CBN_SELCHANGE(IDC_COMBO_TextType, &CDlgBarEdit::OnCbnSelchangeComboTexttype)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgBarEdit 消息处理程序
BOOL CDlgBarEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	/////
	CString strName("");
	int nCount = g_BarCodeMgr.m_BarNameArray.GetSize();
	for( int i = 0; i<nCount; i++ )
	{
		strName = g_BarCodeMgr.m_BarNameArray.GetAt(i);
		m_ctrlType.AddString(strName);
	}
	m_ctrlType.SetCurSel(m_nBarType);
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow( m_nBarType >= PDF417 && m_nBarType  <=  MicroQR || m_nBarType == HANXIN );

	//
	m_pText = new CText();
	Layout();
	UpdateDlgView();

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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgBarEdit::OnBnClickedButBarcodeset()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	g_sys.m_bEnableUpdate = FALSE;
	m_nBarType = m_ctrlType.GetCurSel();
	switch( m_nBarType )
	{
	case PDF417:
		{
			CPdf417Dlg dlg;
			dlg.m_nLineNum = m_nOutline;
			dlg.m_nErrorLevel = m_nErrLev;
			if ( dlg.DoModal() == IDOK )
			{
				m_nOutline = dlg.m_nLineNum;
				m_nErrLev = dlg.m_nErrorLevel;
			}
			break;
		}
	case DATAMATRIX:
		{
			CDataMatrixDlg dlg;
			dlg.m_nSymbol = m_nSymbolNo+1;
			dlg.m_nShape	 = m_nShapeNo+1;
			dlg.m_nErrLev  = m_nErrLev;

			if ( dlg.DoModal() == IDOK )
			{
				m_nSymbolNo = dlg.m_nSymbol-1;
				m_nShapeNo  = dlg.m_nShape-1;
				m_nErrLev	   = dlg.m_nErrLev;

				if ( m_pNodeBar )
				{
					m_pNodeBar->m_nSymbolNo = m_nSymbolNo;
					m_pNodeBar->m_nShapeNo  = m_nShapeNo;
				}
			}
			break;
		}
	case QR:
		{
			CSetQRCodebar dlg;
			dlg.m_bMicroQR	 =  FALSE;
			dlg.m_nErrorValue	 =  m_nErrLev-1;
			dlg.m_nSize			 =  m_nHPoint-1;//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号-1
			dlg.m_nCodeType	 =  m_nCodeType;

			if ( dlg.DoModal() == IDOK )
			{
				m_nErrLev	 =  dlg.m_nErrorValue+1;//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号+1
				m_nHPoint	 =  dlg.m_nSize+1;
				m_nCodeType	 =  dlg.m_nCodeType;

				if ( m_pNodeBar )
				{
					if ( TRUE == CheckBarText() )
					{
						m_pNodeBar->GenBarCode(0x00);
						GetCurActiveView()->Invalidate();
					}
					else
					{
						m_nErrLev	 =  m_pNodeBar->m_nErrLev;
						m_nHPoint	 =  m_pNodeBar->m_nHPoint;
						m_nCodeType	 =  m_pNodeBar->m_nCodeType;
					}
				}
			}
			break;
		}
	case MicroQR:
		{
			CSetQRCodebar dlg;
			dlg.m_bMicroQR		 =  TRUE;
			dlg.m_nErrorValue		 =  m_nErrLev-1;
			dlg.m_nSize				 =  m_nHPoint-1;	//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号-1
			dlg.m_nCodeType		 =  m_nCodeType;

			if ( dlg.DoModal() == IDOK )
			{
				m_nErrLev	 =  dlg.m_nErrorValue+1;//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号+1
				m_nHPoint	 =  dlg.m_nSize+1;
				m_nCodeType	 =  dlg.m_nCodeType;

				if ( m_pNodeBar )
				{
					if ( TRUE == CheckBarText() )
					{
						m_pNodeBar->GenBarCode(0x00);
						GetCurActiveView()->Invalidate();
					}
					else
					{
						m_nErrLev	 =  m_pNodeBar->m_nErrLev;
						m_nHPoint	 =  m_pNodeBar->m_nHPoint;
						m_nCodeType	 =  m_pNodeBar->m_nCodeType;
					}
				}
			}
			break;
		}
	case HANXIN:
		{
			CDlgSetHanXinCode dlg;
			dlg.m_nErrorLevel = m_nErrLev;
			dlg.m_nVersion = m_nVersion;

			if ( dlg.DoModal() == IDOK )
			{
				m_nErrLev = dlg.m_nErrorLevel;
				m_nVersion = dlg.m_nVersion;

				if ( TRUE == CheckBarText() )
				{
					m_pNodeBar->GenBarCode(0x00);
					GetCurActiveView()->Invalidate();
				}
				else
				{
					m_nErrLev = m_pNodeBar->m_nErrLev;
					m_nVersion = m_pNodeBar->m_nVersion;
				}
			}
			break;
		}
	}
}

//
void CDlgBarEdit::OnBnClickedButtonTextadd()
{
	//TODO: 在此添加控件通知处理程序代码
	if ( !m_pNodeBar->IsSelect()  )
		return;

	UpdateData(TRUE);
	//////////
	m_pText->m_bModel	 =  m_bModel;
	m_pText->m_nText	 =  m_nText;
	m_pText->m_strFix	 =  m_strFix;
	m_pText->m_strDate = m_strDate;
	m_pText->m_strModelName = m_strModelName;

	CText* pText = new CText();
	if ( NULL == pText )
		return;

	pText->CopyExceptID(m_pText);
	m_list.AddTail(pText);
	////////
	m_nCurSel = m_list.GetSize()-1;

	Update_Data(FALSE);
	ListFlash();
}

void CDlgBarEdit::OnBnClickedButtonTextdel()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_listData.GetCount()  <= 1 )
		return;

	if ( m_nCurSel < 0 || m_nCurSel  >=  m_listData.GetCount() ) 
		return;

	m_listData.DeleteString(m_nCurSel);
	/////////
	CText *pText = NULL;
	POSITION pos = m_list.FindIndex(m_nCurSel);
	if ( pos  !=  NULL )
	{
		pText = m_list.GetAt(pos);
		if ( pText  !=  NULL )
		{
			delete pText;
			pText = NULL;
		}
		m_list.RemoveAt(pos);
	}

	if ( m_nCurSel  >=  m_listData.GetCount() )
		m_nCurSel = m_listData.GetCount()-1;

	Update_Data(FALSE);
	ListFlash();
}


void CDlgBarEdit::OnBnClickedButtonTextedit()
{
	// TODO: 在此添加控件通知处理程序代码
	Update_Data(TRUE);
	ListFlash();
}

void CDlgBarEdit::OnBnClickedButtonAdvance()
{
	// TODO: 在此添加控件通知处理程序代码
	CHLDoc* pDoc = GetCurActiveDocument();
	if ( NULL == pDoc || NULL == m_pNodeBar ) 
		return;

	g_sys.m_bEnableUpdate = FALSE;
	pDoc->EditBarNode(m_pNodeBar);
}



void CDlgBarEdit::OnLbnSelchangeListData()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_nCurSel = m_listData.GetCurSel();

	Update_Data(FALSE);
	UpdateDlgView();
}


void CDlgBarEdit::OnBnClickedButtonSerial()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( NULL == m_pText )
		return;

	CTextSerialDlg dlg;
	dlg.m_nJumpStart	 =  m_pText->m_nJumpStart;
	dlg.m_nJumpEnd	 =  m_pText->m_nJumpEnd;
	dlg.m_strLeft		 =  m_pText->m_strLeft;
	dlg.m_strRight		 =  m_pText->m_strRight;
	dlg.m_nJumpLen	 =  m_pText->m_nJumpLen;
	dlg.m_strPrefix		 =  m_pText->m_strPrefix;
	dlg.m_nJumpStep	 =  m_pText->m_nJumpStep;
	// 循环跳号
	dlg.m_bJumpLoop	 =  m_pText->m_bJumpLoop;

	dlg.m_nJumpInterval	 =  m_pText->m_nJumpDis;
	dlg.m_bFill			 =  m_pText->m_bFill;
	dlg.m_nFromFile	 =  m_pText->m_nFromFile;
	dlg.m_strFromFile	 =  m_pText->m_strFromFile;
	dlg.m_strBinText	 =  m_pText->m_strBinText;

	dlg.m_pSerial	    = &(m_pText->m_saSerial);
	dlg.m_pCur		    = &(m_pText->m_nMarkTimes);
	dlg.m_strLocal		 =  m_pText->m_strLocal;

	g_sys.m_bEnableUpdate = FALSE;
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
		m_pText->m_bJumpLoop     =  dlg.m_bJumpLoop;
		m_pText->m_bFill				 =  dlg.m_bFill;
		m_pText->m_nFromFile		 =  dlg.m_nFromFile;
		m_pText->m_strFromFile	 =  dlg.m_strFromFile;
		m_pText->m_strBinText		 =  dlg.m_strBinText;
		m_pText->m_nJumpDis     = dlg.m_nJumpInterval;
		m_pText->m_nText			 =  1;
		m_pText->m_strLocal		 =  dlg.m_strLocal;


		SetDlgItemText(IDC_SERIAL_EDIT,GetSerialString());
	}
}


void CDlgBarEdit::ListFlash()
{
	m_listData.ResetContent();
	////
	int i = 0;
	CText* pText = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pText = m_list.GetNext(pos);
		if ( NULL == pText ) 
			continue;
		
		pText->FlashText(0x00);
		m_listData.InsertString(i, pText->GetInfo());
		i++;
	}

	m_listData.SetCurSel(m_nCurSel);
}


//
BOOL CDlgBarEdit::Update_Data(BOOL bSaveAndValidate)
{
	int nSize = m_list.GetSize();
	if ( nSize < 1 || m_nCurSel < 0 || m_nCurSel  >=  nSize )
		return FALSE;

	CText *pText = m_list.GetAt(m_list.FindIndex(m_nCurSel));
	if ( pText == NULL )
		return FALSE;

	BOOL bRet = TRUE;
	if ( TRUE == bSaveAndValidate )
	{
		CText* pTextTemp = new CText();
		pTextTemp->CopyExceptID(m_pText);

		UpdateData(TRUE);

		m_pText->m_bModel    = m_bModel;
		m_pText->m_strModelName = m_strModelName;
		m_pText->m_nText	   = m_nText;
		m_pText->m_strFix     = m_strFix;
		m_pText->m_strDate  = m_strDate;
		pText->CopyExceptID(m_pText);

		if ( FALSE == CheckBarText() )
		{
			pText->CopyExceptID(pTextTemp);
			m_pText->CopyExceptID(pTextTemp);

			m_bModel	 =  m_pText->m_bModel;
			m_nText		 =  m_pText->m_nText;
			m_strFix		 =  m_pText->m_strFix;
			m_strDate	 =  m_pText->m_strDate;
			m_strModelName = m_pText->m_strModelName;

			UpdateData(FALSE);
			bRet = FALSE;
		}

		delete pTextTemp;
	}
	else
	{
		m_pText->CopyExceptID(pText);

		m_bModel	 =  m_pText->m_bModel;
		m_nText		 =  m_pText->m_nText;
		m_strFix	 =  m_pText->m_strFix;
		m_strDate	 =  m_pText->m_strDate;
		m_strModelName = m_pText->m_strModelName;

		UpdateData(FALSE);
	}

	KillTimer(1);
	if ( TEXT_SERIAL == m_nText )
	{
		SetDlgItemText(IDC_SERIAL_EDIT, m_pText->GetString());
	}
	else if ( TEXT_DATE == m_nText )
	{
		SetTimer(1,1000,NULL);
	}

	return bRet;
}


//
CString CDlgBarEdit::GetSerialString()
{
	CString strSerial = _T("");

	if ( m_pText  !=  NULL )
	{
		m_pText->GenString();
		strSerial = m_pText->GetString();
	}

	return strSerial;
}


void CDlgBarEdit::OnEnChangeDateEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_DATE_EDIT,m_strDate);
}

void CDlgBarEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strText = GenTimeString(VerifyTimeFormat(m_strDate));
	SetDlgItemText(IDC_TIME_INFO,strText);

	CDialog::OnTimer(nIDEvent);
}


//
void CDlgBarEdit::SetTextInfo( CNode *pNode )
{
	if ( NULL == pNode || NODE_CODEBAR  !=  pNode->GetType() )
		return;

	ResetData();
	////
	CText *pText = NULL;
	m_pNodeBar = (CNodeCodebar*)pNode;
	for(int i = 0; i < m_pNodeBar->GetTextCount(); i++)
	{
		pText = (CText*)m_pNodeBar->GetText(i);
		if ( pText  !=  NULL )
		{
			CText *pNew = new CText();
			if ( pNew  !=  NULL )
			{
				pNew->CopyExceptID(pText);
				m_list.AddTail(pNew);
			}
		}
	}
	ListFlash();

	if ( m_list.GetCount() < 1 )
		return;

	pText = m_list.GetHead();
	if ( NULL == pText )
		return;
	else
		m_pText->CopyExceptID(pText);

	m_nCurSel = 0;
	m_listData.SetCurSel(0);
	m_bModel = m_pText->m_bModel;
	m_strModelName = m_pText->m_strModelName;
	m_nText	  = m_pText->m_nText;
	m_strFix  = m_pText->m_strFix;
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

	m_nSymbolNo = m_pNodeBar->m_nSymbolNo;
	m_nShapeNo  = m_pNodeBar->m_nShapeNo;
	m_nHPoint	   = m_pNodeBar->m_nHPoint;
	m_nWPoint    = m_pNodeBar->m_nWPoint;
	m_bAutoSet   = m_pNodeBar->m_bAutoSet;
	m_nErrLev     = m_pNodeBar->m_nErrLev;
	m_nCodeType =  m_pNodeBar->m_nCodeType;
	m_nOutline    = m_pNodeBar->m_nOutline;
	m_nVersion	  = m_pNodeBar->m_nVersion;

	m_nBarType = (int)(m_pNodeBar->m_BarType);
	m_ctrlType.SetCurSel(m_nBarType);

	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow( m_nBarType >= PDF417 && m_nBarType  <=  MicroQR || m_nBarType == HANXIN );

	UpdateData(FALSE);
	UpdateDlgView();
}


//
void CDlgBarEdit::Apply()
{
	if ( !m_pNodeBar || !m_pNodeBar->IsSelect() )
		return;

	if ( m_pNodeBar->GetLockFlg() == TRUE )
		return;
	//
	if ( FALSE == Update_Data(TRUE) )
		return;

	ListFlash();

	//
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( 0 == m_listData.GetCount() )
	{
		if ( pDoc  !=  NULL )
		{	
			pDoc->Delete();
			return;
		}
	}

	m_pNodeBar->GenBarCode(0x00);
	m_pNodeBar->SetSelect(TRUE);
	m_pNodeBar->CalLimit();

	CHLView *pView = GetCurActiveView();
	if ( NULL == pView )
		return;

	CText *pText = (CText*)m_pNodeBar->GetText(0);

	pDoc->m_chain.RefreshBarText( pView,pText->GetTextID() );
}


void CDlgBarEdit::OnBnClickedCheckModify()
{
	//TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CDlgBarEdit::OnCbnSelchangeComboBarcodetype()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	int nCurSel = m_ctrlType.GetCurSel();
	m_nBarType  = nCurSel;
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow( nCurSel >= PDF417 && nCurSel  <=  MicroQR || nCurSel == HANXIN );
	FlashBarDefaultProperty();
	Update_Data(TRUE);

}


void CDlgBarEdit::FlashBarDefaultProperty()
{
	switch(m_nBarType) 
	{
	case PDF417:
		m_nOutline = 12;
		break;
	case DATAMATRIX:
		m_nHPoint	 = 18;
		m_nWPoint	 = 18;
		m_bAutoSet	 = FALSE;
		m_nErrLev	 = 5;
		m_nOutline	 = 0;
		break;
	case QR:
		m_nErrLev = 2;
		m_nHPoint = 1;
		m_nCodeType = 5;
		break;
	case MicroQR:
		m_nErrLev = 1;
		m_nHPoint = 4;
		m_nCodeType = 1;
		break;
	case HANXIN:
		m_nErrLev = 3;
		m_nVersion = 0;
		break;
	}
}




void CDlgBarEdit::GetItemMovePos(int res, int obj,int &nX,int &nY)
{
	CWnd *hWin = NULL;

	RECT rectRes;
	RECT rectObj;

	hWin = GetDlgItem(obj);
	if ( hWin  !=  NULL )
	{
		hWin->GetWindowRect(&rectObj);
	}
	else
	{
		nX = nX = 0;
		return;
	}

	hWin = GetDlgItem(res);
	if ( hWin  !=  NULL )
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


void CDlgBarEdit::MoveDlgItem(int nItem, int nX,int nY)
{
	CWnd *hWin = NULL;
	RECT rect;

	hWin = GetDlgItem(nItem);
	if (hWin  !=  NULL)
	{	
		hWin->GetWindowRect(&rect);
	}
	else
		return;

	ScreenToClient(&rect);

	rect.left  +=  nX;
	rect.right  +=  nX;
	rect.top  +=  nY;
	rect.bottom  +=  nY;

	hWin->MoveWindow(&rect);
}




void CDlgBarEdit::Layout()
{
	int nMoveX = 0, nMoveY = 0;

	GetItemMovePos(IDC_FIX_EDIT,IDC_STATIC_RECT1,nMoveX,nMoveY);		
	MoveDlgItem(IDC_FIX_EDIT,nMoveX,nMoveY);

	GetItemMovePos(IDC_BUTTON_SERIAL,IDC_STATIC_RECT1,nMoveX,nMoveY);		
	MoveDlgItem(IDC_BUTTON_SERIAL,nMoveX,nMoveY);
	MoveDlgItem(IDC_SERIAL_EDIT,nMoveX,nMoveY);

	GetItemMovePos(IDC_TIME_INFO,IDC_STATIC_RECT1,nMoveX,nMoveY);
	MoveDlgItem(IDC_TIME_INFO,nMoveX,nMoveY);
	MoveDlgItem(IDC_DATE_EDIT,nMoveX,nMoveY);
}


void CDlgBarEdit::UpdateDlgView()
{
	GetDlgItem(IDC_FIX_EDIT)->ShowWindow(0 == m_nText);
	GetDlgItem(IDC_CHECK_MODIFY)->EnableWindow(0 == m_nText);
	GetDlgItem(IDC_EDIT_TEXTID)->EnableWindow(0 == m_nText);

	GetDlgItem(IDC_BUTTON_SERIAL)->ShowWindow(1 == m_nText);
	GetDlgItem(IDC_SERIAL_EDIT)->ShowWindow(1 == m_nText);

	GetDlgItem(IDC_TIME_INFO)->ShowWindow(2 == m_nText);
	GetDlgItem(IDC_DATE_EDIT)->ShowWindow(2 == m_nText);

	if (m_pNodeBar  !=  NULL)
	{
		GetDlgItem(IDC_BUTTON_TEXTDEL)->EnableWindow( FALSE == m_pNodeBar->m_bTextBar );
		GetDlgItem(IDC_BUTTON_TEXTADD)->EnableWindow( FALSE == m_pNodeBar->m_bTextBar );
		GetDlgItem(IDC_COMBO_BARCODETYPE)->EnableWindow( FALSE == m_pNodeBar->m_bTextBar );
	}
}



void CDlgBarEdit::OnCbnSelchangeComboTexttype()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	UpdateDlgView();	
}


//
void CDlgBarEdit::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	ResetData();
	/////
	if ( m_pText  !=  NULL )
		delete m_pText;
}

//
void CDlgBarEdit::ResetData()
{
	m_nCurSel = -1;
	m_listData.ResetContent();
	//////////
	POSITION pos = m_list.GetHeadPosition();
	while ( pos  !=  NULL )
	{
		CText *pText = (CText*)m_list.GetNext(pos);
		if ( pText  !=  NULL )
		{
			delete pText;
			pText = NULL;
		}
	}
	////
	m_list.RemoveAll();
}



BOOL CDlgBarEdit::CheckBarText()
{
	if ( !m_pNodeBar || !m_pNodeBar->IsSelect() )
		return FALSE;

	if ( m_pNodeBar->GetLockFlg() == TRUE )
		return FALSE;
	
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( 0 == m_listData.GetCount() && pDoc  !=  NULL )
	{
		pDoc->Delete();
		return FALSE;
	}

	CNodeCodebar *pNodeTmp = new CNodeCodebar();
	if ( pNodeTmp == NULL )
		return FALSE;

	pNodeTmp->SetParent(&pDoc->m_chain);
	pNodeTmp->CopyToMe(m_pNodeBar,0x02);
	pNodeTmp->ClearAllText();

	CText *pText = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pText = m_list.GetNext(pos);
		if ( pText  !=  NULL )
		{
			CText* pNew = new CText();
			if ( pNew  !=  NULL )
			{
				pNew->CopyExceptID(pText);
				pNodeTmp->AddTextAll_NewID(pNew);
			}
		}
	}
	//
	m_nBarType	 =  m_ctrlType.GetCurSel();

	pNodeTmp->m_BarType		 =  (BARTYPE)m_nBarType;
	pNodeTmp->m_nHPoint		 =  m_nHPoint;
	pNodeTmp->m_nWPoint		 =  m_nWPoint;
	pNodeTmp->m_bAutoSet	 =  m_bAutoSet;
	pNodeTmp->m_nErrLev		 =  m_nErrLev;
	pNodeTmp->m_nOutline		 =  m_nOutline;
	pNodeTmp->m_nErrLev		 =  m_nErrLev;
	pNodeTmp->m_nSymbolNo = m_nSymbolNo;
	pNodeTmp->m_nShapeNo  = m_nShapeNo;
	pNodeTmp->m_nHPoint		 =  m_nHPoint;
	pNodeTmp->m_nCodeType	 =  m_nCodeType;
	pNodeTmp->m_nVersion	 =  m_nVersion;

	//条码类型切换后镂空判断
	if ( m_nBarType != QR && m_nBarType != DATAMATRIX && m_nBarType != MicroQR && m_nBarType != HANXIN )
		pNodeTmp->m_bHollow = FALSE;

	BOOL bRet = pNodeTmp->GenBarCode(0x00);
	if ( TRUE == bRet )
	{
		m_pNodeBar->CopyToMe(pNodeTmp,0x03);//原Text ID保留，仅改变数据
	}

	delete pNodeTmp;
	pNodeTmp = NULL;

	return bRet;
}

BOOL CDlgBarEdit::PreTranslateMessage(MSG* pMsg)
{
	if ( (WM_KEYDOWN == pMsg->message ) && (GetKeyState(VK_RETURN) & 0x8000) )
	{
		Apply();
		////////
		CHLDoc *pDoc = GetCurActiveDocument();
		if ( pDoc )
			pDoc->SetTrackerQuad(pDoc->GetLimit());
	}
	return 0;
}
