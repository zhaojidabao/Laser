// BarCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "BarCodeDlg.h"

#include "DataMatrixDlg.h"
#include "SetQRCodebar.h"
#include "Pdf417Dlg.h"
#include "DlgSetHanXinCode.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW 
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarCodeDlg dialog


CBarCodeDlg::CBarCodeDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CBarCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarCodeDlg)
	
	//}}AFX_DATA_INIT

	m_nVersion = 0;
}


void CBarCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarCodeDlg)
	DDX_Control(pDX, IDC_TAB_TEXT, m_tab);
	DDX_Control(pDX, IDC_COMBO_BARCODETYPE, m_ctrlType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBarCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CBarCodeDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TEXT, OnSelchangeTabText)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_TEXT, OnSelchangingTabText)
	ON_CBN_SELCHANGE(IDC_COMBO_BARCODETYPE, OnSelchangeComboBarcodetype)
	ON_BN_CLICKED(IDC_BUT_BARCODESET, OnButBarcodeset)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CBarCodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarCodeDlg message handlers

BOOL CBarCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strName;
	int nCount = g_BarCodeMgr.m_BarNameArray.GetSize();
	for( int i = 0;i < nCount; i++ )
	{
		strName = g_BarCodeMgr.m_BarNameArray.GetAt(i);
		m_ctrlType.AddString(strName);
	}
	m_ctrlType.SetCurSel(m_nBarType);
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow( m_nBarType >= PDF417 && m_nBarType  <=  MicroQR || m_nBarType == HANXIN );

	//生成标签
	TC_ITEM tabItem;
	tabItem.mask = TCIF_PARAM|TCIF_TEXT;
	tabItem.pszText = new TCHAR[255];

	if ( tabItem.pszText == NULL )
		return FALSE;

	CString str;
	tabItem.lParam = (LPARAM)&m_TextPage;
	VERIFY(m_TextPage.Create(CDlgBarPageText::IDD,&m_tab));
	str.LoadString(IDS_TEXTTITLE);
	_tcscpy_s(tabItem.pszText,255,str);
	m_tab.InsertItem(0,&tabItem);
	m_TextPage.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_TextPage.ShowWindow(SW_SHOW);
	m_TextPage.EnableWindow(TRUE);

	tabItem.lParam = (LPARAM)&m_BarPage;
	VERIFY(m_BarPage.Create(CBarPage::IDD,&m_tab));
	str.LoadString(IDS_BARTITLE);
	_tcscpy_s(tabItem.pszText,255,str);

	m_tab.InsertItem(1,&tabItem);
	m_BarPage.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BarPage.ShowWindow(SW_HIDE);
	m_BarPage.EnableWindow(FALSE);
	m_BarPage.UpdateData(FALSE);
	
	delete tabItem.pszText;

	BOOL bParity = ( m_nBarType == CODE39 || m_nBarType == INT25 || m_nBarType == EXT39 );
	if ( !bParity )
		m_BarPage.GetDlgItem(IDC_CHECK_PARITY)->ShowWindow(SW_HIDE);

	//qr microqr可镂空
	m_BarPage.GetDlgItem(IDC_CHECK_HOLLOW)->EnableWindow(m_nBarType == QR || m_nBarType == DATAMATRIX || m_nBarType == MicroQR || m_nBarType == HANXIN);
	if( m_nBarType != QR && m_nBarType != DATAMATRIX && m_nBarType != MicroQR && m_nBarType != HANXIN )
		m_BarPage.m_bHollow = FALSE;
	
	GetDlgItem(IDC_COMBO_BARCODETYPE)->EnableWindow( FALSE == m_TextPage.m_pNodeCodeBar->m_bTextBar );
		
	return TRUE;
}

void CBarCodeDlg::OnSelchangeTabText(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab = m_tab.GetCurSel();
	if ( iTab < 0 )
		return;

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	if ( pWnd )
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->EnableWindow(TRUE);
	}
	*pResult = 0;
}

void CBarCodeDlg::OnSelchangingTabText(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab = m_tab.GetCurSel();
	if ( iTab < 0 )
		return;

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd = (CWnd *)tci.lParam;
	if ( pWnd )
	{
		pWnd->ShowWindow(SW_HIDE);
		pWnd->EnableWindow(FALSE);
	}

	*pResult = 0;
}

void CBarCodeDlg::OnOK() 
{
	m_nBarType = m_ctrlType.GetCurSel();
	m_TextPage.Update_Data(TRUE);
	m_BarPage.UpdateData();
	
	if ( m_BarPage.m_nSingleLineMarkCount < 1 )
	{
		m_BarPage.m_nSingleLineMarkCount = 1;
	}
	
    if ( m_BarPage.m_nNarrowLineNum < 2 )
	{
		m_BarPage.m_nNarrowLineNum = 2;
	}
	
	BOOL bParity = (m_nBarType == CODE39 || m_nBarType == INT25 || m_nBarType == EXT39);
	if ( !bParity )
		m_BarPage.m_bParity = FALSE;
	
	BOOL bCreate = CreateCodeBarStatus(m_nBarType);
	if ( TRUE == bCreate || m_TextPage.m_list.GetCount() == 0 )
	{
		CDialog::OnOK();
	}
}


void CBarCodeDlg::OnSelchangeComboBarcodetype() 
{
	UpdateData();

	int nCurSel = m_ctrlType.GetCurSel();
	m_nBarType = nCurSel;
	
	GetDlgItem(IDC_BUT_BARCODESET)->EnableWindow( nCurSel >= PDF417 && nCurSel  <=  MicroQR || nCurSel == HANXIN );
	FlashBarDefaultProperty();
  
	BOOL bParity = (m_nBarType == CODE39 || m_nBarType == INT25 || m_nBarType == EXT39);

	// 是否可镂空
	m_BarPage.GetDlgItem(IDC_CHECK_HOLLOW)->EnableWindow(m_nBarType == QR || m_nBarType == DATAMATRIX || m_nBarType == MicroQR || m_nBarType == HANXIN);
	if( m_nBarType != QR && m_nBarType != DATAMATRIX && m_nBarType != MicroQR && m_nBarType != HANXIN )
	{
		m_BarPage.m_bHollow = FALSE;
		m_BarPage.UpdateData(FALSE);
	}

	m_BarPage.GetDlgItem(IDC_EDIT_HOLLOW_WIDTH)->EnableWindow( (m_nBarType == QR || m_nBarType == DATAMATRIX || m_nBarType == MicroQR || m_nBarType == HANXIN) && m_BarPage.m_bHollow );
	m_BarPage.GetDlgItem(IDC_EDIT_HOLLOW_HEIGHT)->EnableWindow( (m_nBarType == QR || m_nBarType == DATAMATRIX || m_nBarType == MicroQR || m_nBarType == HANXIN) && m_BarPage.m_bHollow );

	m_BarPage.GetDlgItem(IDC_CHECK_PARITY)->ShowWindow(bParity?SW_SHOW:SW_HIDE);
}



void CBarCodeDlg::OnButBarcodeset() 
{
	UpdateData();

	switch(m_nBarType)
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
			}
			break;
		}
	case QR:
		{
			CSetQRCodebar dlg;
			dlg.m_bMicroQR	 =  FALSE;
			dlg.m_nErrorValue	 =  m_nErrLev-1;
			dlg.m_nSize			 =  m_nHPoint-1;//QR码的矩阵大小序号从1开始
			dlg.m_nCodeType	 =  m_nCodeType;

			if ( dlg.DoModal() == IDOK )
			{
				m_nErrLev		 =  dlg.m_nErrorValue+1;//QR码的矩阵大小序号从1开始
				m_nHPoint		 =  dlg.m_nSize+1;
				m_nCodeType	 =  dlg.m_nCodeType;
			}
			break;
		}
	case MicroQR:
		{
			CSetQRCodebar dlg;
			dlg.m_bMicroQR		 =  TRUE;
			dlg.m_nErrorValue		 =  m_nErrLev-1;
			dlg.m_nSize				 =  m_nHPoint-1; //QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号-1
			dlg.m_nCodeType		 =  m_nCodeType;
			
			if ( dlg.DoModal() == IDOK )
			{
				m_nErrLev		 =  dlg.m_nErrorValue+1;//QR码的矩阵大小序号从1开始，因此要把COMBO控件的当前选择项的序号+1
				m_nHPoint		 =  dlg.m_nSize+1;
				m_nCodeType = dlg.m_nCodeType;
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
			}
			break;
		}
	}
}


void CBarCodeDlg::FlashBarDefaultProperty()
{
	switch(m_nBarType) 
	{
	case PDF417:
		m_nOutline = 12;
		break;
	case DATAMATRIX:
		m_nHPoint	 = 18;
		m_nWPoint	 = 18;
		m_bAutoSet = FALSE;
		m_nErrLev	 = 5;
		m_nOutline	 = 0;
		break;
	case QR:
		m_nErrLev	 =  2;
		m_nHPoint	 =  1;
		m_nCodeType = 5;
		break;
	case MicroQR:
		m_nErrLev	 =  1;
		m_nHPoint	 =  4;
		m_nCodeType = 1;
		break;
	case HANXIN:
		m_nErrLev = 3;
		m_nVersion = 0;
		break;
	}
}


BOOL CBarCodeDlg::CreateCodeBarStatus( long nType )
{
	CNodeCodebar *pNode = new CNodeCodebar();
	if ( !pNode  )
		return FALSE;

	m_TextPage.UpdateData(TRUE);

	CString strText = GetBarString();
	pNode->m_BarType		 =  (BARTYPE)nType;
	pNode->m_nOutline		 =  m_nOutline;
	pNode->m_nErrLev		 =  m_nErrLev;
	pNode->m_nHPoint		 =  m_nHPoint;
	pNode->m_nWPoint		 =  m_nWPoint;
	pNode->m_nSymbolNo	 =  m_nSymbolNo;
	pNode->m_nShapeNo	 =  m_nShapeNo;
	pNode->m_nCodeType	 =  m_nCodeType;
	pNode->m_bAutoSet		 =  m_bAutoSet;
	pNode->m_nVersion		 =  m_nVersion;
	
	BOOL bRet = g_BarCodeMgr.GenBarCode(pNode, strText,pNode->m_nCodePage);
	delete pNode;

	return bRet;
}

//
CString CBarCodeDlg::GetBarString()
{
	CString str, strText;
	CText *pText = NULL;

	strText.Empty();
	POSITION pos = m_TextPage.m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pText = m_TextPage.m_list.GetNext(pos);
		//////
		if ( pText  !=  NULL )
		{
			pText->FlashText(0x00);
			str = pText->GetString();
			strText  +=  str;
		}
	}

	return strText;
}


void CBarCodeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
