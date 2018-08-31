// FillObjmMarkSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "FillObjmMarkSetDlg.h"
#include "LocNukeForHL.h"
#include "baseMacro.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern  CHLApp	theApp;
/////////////////////////////////////////////////////////////////////////////
// CFillObjmMarkSetDlg dialog

CFillObjmMarkSetDlg::CFillObjmMarkSetDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CFillObjmMarkSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFillObjmMarkSetDlg)
	m_IsFirstMarkFill = FALSE;
	m_IsMarkFill = FALSE;
	m_IsMarkFram = FALSE;
	m_dbMarkCurrent = 0.0;
	m_dbQFreq = 0.0;
	m_dbMarkSpeed = 0.0;
	m_nMarkCnt = 1;
	m_nCurrSelItem = 0;
	m_bIsResMark = FALSE;
	//}}AFX_DATA_INIT
}


void CFillObjmMarkSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFillObjmMarkSetDlg)
	DDX_Control(pDX, IDC_LIST_MARKINFO, m_MarkInfoList);
	DDX_Control(pDX, IDC_COMBO_MARKLAYERCOUNT, m_combLayerCnt);
	DDX_Control(pDX, IDC_COMBO_MARKPARALIST, m_spParaList);
	DDX_Control(pDX, IDC_COMBO_MARKPARA, m_MarkParaList);
	DDX_Check(pDX, IDC_CHECK_ISFIRSTMARKFILL, m_IsFirstMarkFill);
	DDX_Check(pDX, IDC_CHECK_ISMARKFILL, m_IsMarkFill);
	DDX_Check(pDX, IDC_CHECK_ISMARKFRAM, m_IsMarkFram);
	DDX_Text(pDX, IDC_EDIT_MARKCOUNT, m_nMarkCnt);
	DDV_MinMaxInt(pDX, m_nMarkCnt, 0, 256);
	DDX_Check(pDX, IDC_CHECK_ISRESMARK, m_bIsResMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillObjmMarkSetDlg, CDialog)
	//{{AFX_MSG_MAP(CFillObjmMarkSetDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKLAYERCOUNT, OnSelchangeComboMarklayercount)
	ON_BN_CLICKED(IDC_CHECK_ISMARKFRAM, OnCheckIsmarkfram)
	ON_BN_CLICKED(IDC_CHECK_ISMARKFILL, OnCheckIsmarkfill)
	ON_BN_CLICKED(IDC_CHECK_ISFIRSTMARKFILL, OnCheckIsfirstmarkfill)
	ON_EN_CHANGE(IDC_EDIT_MARKCOUNT, OnChangeEditMarkcount)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKPARA, OnSelchangeComboMarkpara)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MARKINFO, OnClickListMarkinfo)
	ON_BN_CLICKED(IDC_CHECK_SELMARK, OnCheckSelmark)
	ON_BN_CLICKED(IDC_CHECK_USEED, OnCheckUseed)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKPARALIST, OnSelchangeComboParaList)
	ON_BN_CLICKED(IDC_BTN_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_CHECK_ISRESMARK, OnCheckIsresmark)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SELMARK_END,OnSelMarkEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillObjmMarkSetDlg message handlers

BOOL CFillObjmMarkSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitMarkInfoList();
	DisplayMarkSet(m_ObjMarkSet);

	HICON hIcon = ((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON_MARKSEL);
	((CButton*)GetDlgItem(IDC_CHECK_SELMARK))->SetIcon(hIcon);

	if (m_IsMarkFill && m_IsMarkFram)
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(FALSE);
	}

	InitParaList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFillObjmMarkSetDlg::SetLayerCount(CLay &lay) 
{
	m_MarkParaList.Clear();
	m_MarkParaList.ResetContent();

	int nSize = lay.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CLayer lyr       = lay.GetLayer(i);
		m_MarkParaList.AddColor(lyr.m_color);
	}
	
	return TRUE;
}

void CFillObjmMarkSetDlg::OnSelchangeComboParaList() 
{
	UpdateData(TRUE);

	CString strPath = ::g_sys.GetInstallPath();
	int  index = m_spParaList.GetCurSel();
	if ( index  !=  CB_ERR )
	{	
		CString filename;
		m_spParaList.GetLBText(index,filename);	
		filename = strPath+filename + _T(".psd");

		g_sys.ReadSpMarkInfo(m_ObjMarkSet, filename);
		DisplayMarkSet(m_ObjMarkSet);
	}
}

void CFillObjmMarkSetDlg::InitParaList() 
{
	m_spParaList.ResetContent();

	CString searchFile =  ::g_sys.GetInstallPath() + _T("*.psd");
	
	CString   pd;
	CFileFind finder;
    BOOL bWorking = finder.FindFile(searchFile);
    while ( bWorking )
    {
        bWorking = finder.FindNextFile();
		pd = finder.GetFileName();
		pd = pd.Left(pd.GetLength()-4);
		m_spParaList.AddString(pd);
	}
	
	m_spParaList.SetCurSel(-1);
}

void CFillObjmMarkSetDlg::OnButtonExport() 
{
	static TCHAR szFilter[] = _T("Mark File(*.psd)|*.psd||");
	
	CFileDialog dlg(FALSE,NULL,_T(""),OFN_HIDEREADONLY,szFilter);
	dlg.m_ofn.lpstrInitialDir = g_sys.GetInstallPath();
	if ( dlg.DoModal()  !=  IDOK )
		return;

	
	CString strExportFileName = dlg.GetPathName();
	if ( strExportFileName.IsEmpty() )
		return;

	CString strExt = strExportFileName.Right(4);
	if (0  !=  strExt.CompareNoCase(_T(".psd"))) 
	{
		strExportFileName  +=  _T(".psd");
	}

	g_sys.WriteSpMarkInfo(m_ObjMarkSet, strExportFileName);

	CString strFile = strExportFileName;
	strFile = strFile.Left(strFile.GetLength() - 4);
	int nPos = strFile.ReverseFind(_T('\\'));
	strExportFileName = strFile.Mid(nPos + 1, strFile.GetLength() - 1);
	int nIn = m_spParaList.FindString(0, strExportFileName);
	if ( -1  !=  nIn )
	{
		m_spParaList.DeleteString(nIn);
	}
	m_spParaList.AddString(strExportFileName);

}

void CFillObjmMarkSetDlg::DisplayMarkSet(OBJMARKSET &ObjMarkSet) 
{
	// ��༭ 
	m_combLayerCnt.Clear();
	m_combLayerCnt.ResetContent();
	for (int i = 0; i < OBJ_MAX_MARKCOUNT; i++)
	{
		TCHAR szLayer[64];
		if ( 0x804 == g_LangID )
		{
			_stprintf_s(szLayer,64, _T("%d��"), i+1);
		}
		else
		{
			_stprintf_s(szLayer, 64,_T("%d"), i+1);
		}
		
		m_combLayerCnt.InsertString(i, szLayer);
	}

	if ( ObjMarkSet.nMarkCount > OBJ_MAX_MARKCOUNT ) 
	{
		ObjMarkSet.nMarkCount = OBJ_MAX_MARKCOUNT;
	}
	else if (ObjMarkSet.nMarkCount < 0) 
	{
		ObjMarkSet.nMarkCount = 0;
	}
	m_combLayerCnt.SetCurSel(ObjMarkSet.nMarkCount - 1);

	// ���������༭
	if (ObjMarkSet.nMarkCount > 0) 
	{
		m_IsFirstMarkFill = ObjMarkSet.stSub[0].bIsFillFirst;
		m_IsMarkFill = ObjMarkSet.stSub[0].bIsMarkFill;
		m_IsMarkFram = ObjMarkSet.stSub[0].bIsMarkFram;
		m_nMarkCnt   = ObjMarkSet.stSub[0].nSubCnt;

		m_bIsResMark = ObjMarkSet.stSub[0].bIsRevMark;

		// �����
		if (ObjMarkSet.stSub[0].nLayer < 0) 
		{
			ObjMarkSet.stSub[0].nLayer = 0;
		}
		m_MarkParaList.SetCurSel(ObjMarkSet.stSub[0].nLayer);

		CHLDoc *pDoc = GetCurActiveDocument();
		if ( NULL  !=  pDoc )
		{
			CLayer ly			 =  pDoc->GetLayer(ObjMarkSet.stSub[0].nLayer);
			m_dbQFreq			 =  ly.m_para.emcc.dbQFreq;
			m_dbMarkSpeed   = ly.m_para.emcc.dbMarkV;
			m_dbMarkCurrent = ly.m_para.emcc.dbCurrent;
		}
		else
		{
			m_dbQFreq			 =  0;
			m_dbMarkSpeed   = 0;
			m_dbMarkCurrent = 0;
		}
	}// end if (ObjMarkSet.nMarkCount > 0) 
	else
	{
		ObjMarkSet.nMarkCount = 0;

		m_IsFirstMarkFill = FALSE;
		m_IsMarkFill = FALSE;
		m_IsMarkFram = FALSE;
		m_nMarkCnt   = 1;

		m_bIsResMark = FALSE;

		ObjMarkSet.stSub[0].nLayer = 0;
		m_MarkParaList.SetCurSel(ObjMarkSet.stSub[0].nLayer);
		m_dbQFreq       = 0;
		m_dbMarkSpeed   = 0;
		m_dbMarkCurrent = 0;
	}

	m_nCurrSelItem = 0;

	UsedMarkSet(ObjMarkSet.nMarkCount > 0);

	if (m_IsMarkFill && m_IsMarkFram)
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(FALSE);
	}

	SetMarkObjInfo(ObjMarkSet);

	m_ObjMarkSet = ObjMarkSet;
	UpdateData(FALSE);

	return;
}

BOOL CFillObjmMarkSetDlg::SetMarkObjInfo(OBJMARKSET &ObjMarkSet) 
{
	m_MarkInfoList.DeleteAllItems();
	for (int i = 0; i < ObjMarkSet.nMarkCount; i++)
	{
		SetSubInfoToList(ObjMarkSet, i, TRUE);
	}

	m_MarkInfoList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_MarkInfoList.SetCurSel(0);

	return TRUE;
}

void CFillObjmMarkSetDlg::GetFillMarkSet(OBJMARKSET &ObjMarkSet) 
{
	int nLayerCnt = m_combLayerCnt.GetCurSel();
	if (nLayerCnt  >=  0)
	{
		m_ObjMarkSet.nMarkCount  = nLayerCnt + 1;
	}
	else
	{
		OBJMARKSET markSet;
		m_ObjMarkSet = markSet;
	}

	ObjMarkSet = m_ObjMarkSet;
}

void CFillObjmMarkSetDlg::OnSelchangeComboMarklayercount() 
{
	int nLayerCnt  = m_combLayerCnt.GetCurSel();
	if (nLayerCnt  >=  0)
	{
		m_ObjMarkSet.nMarkCount   = nLayerCnt + 1;

		// 
		m_IsFirstMarkFill	 =  m_ObjMarkSet.stSub[0].bIsFillFirst;
		m_IsMarkFill			 =  m_ObjMarkSet.stSub[0].bIsMarkFill;
		m_IsMarkFram     = m_ObjMarkSet.stSub[0].bIsMarkFram;
		m_nMarkCnt        = m_ObjMarkSet.stSub[0].nSubCnt;

		m_bIsResMark      = m_ObjMarkSet.stSub[0].bIsRevMark;

		if (m_ObjMarkSet.stSub[0].nLayer < 0) 
		{
			m_ObjMarkSet.stSub[0].nLayer = 0;
		}
		m_MarkParaList.SetCurSel(m_ObjMarkSet.stSub[0].nLayer);

		CHLDoc *pDoc = GetCurActiveDocument();
		if (NULL  !=  pDoc)
		{
			CLayer ly          = pDoc->GetLayer(m_ObjMarkSet.stSub[0].nLayer);
			m_dbQFreq        = ly.m_para.emcc.dbQFreq;
			m_dbMarkSpeed   = ly.m_para.emcc.dbMarkV;
			m_dbMarkCurrent = ly.m_para.emcc.dbCurrent;
		}
		else
		{
			m_dbQFreq       = 0;
			m_dbMarkSpeed = 0;
			m_dbMarkCurrent = 0;
		}

		// �����б��ؼ�
		m_MarkInfoList.DeleteAllItems();
		for (int j = 0; j < m_ObjMarkSet.nMarkCount; j++)
		{
			SetSubInfoToList(m_ObjMarkSet, j, TRUE);
		}
		
		if (m_ObjMarkSet.nMarkCount > 0) 
		{
			m_MarkInfoList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	        m_MarkInfoList.SetCurSel(0);
		}

		UpdateData(FALSE);
	}// end if (nLayerCnt  >=  0)

	return;
}

void CFillObjmMarkSetDlg::OnSelchangeComboMarkpara() 
{
    UpdateData(TRUE);

	// �ж��Ƿ�ѡ���ˣ��ڼ��δ��
	if (  m_nCurrSelItem < 0 ) 
		return;

	int nLayer = m_MarkParaList.GetCurSel();
	if (nLayer  >=  0)
	{
		m_ObjMarkSet.stSub[m_nCurrSelItem].nLayer       = nLayer;
	}
	else
	{
		m_ObjMarkSet.stSub[m_nCurrSelItem].nLayer       = 0;
	}
	SetSubInfoToList(m_ObjMarkSet, m_nCurrSelItem, FALSE);
}

void CFillObjmMarkSetDlg::OnCheckIsmarkfram() 
{
	UpdateData(TRUE);

	if (m_nCurrSelItem  >=  0)
	{
		m_ObjMarkSet.stSub[m_nCurrSelItem].bIsMarkFram = m_IsMarkFram;
		SetSubInfoToList(m_ObjMarkSet, m_nCurrSelItem, FALSE);
	}

	if (m_IsMarkFill && m_IsMarkFram)
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(FALSE);
	}
}

void CFillObjmMarkSetDlg::OnCheckIsmarkfill() 
{
	UpdateData(TRUE);
	if (m_nCurrSelItem  >=  0)
	{
		m_ObjMarkSet.stSub[m_nCurrSelItem].bIsMarkFill = m_IsMarkFill;
		SetSubInfoToList(m_ObjMarkSet, m_nCurrSelItem, FALSE);
	}

	if (m_IsMarkFill && m_IsMarkFram)
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(FALSE);
	}
}

void CFillObjmMarkSetDlg::OnCheckIsfirstmarkfill() 
{
	UpdateData(TRUE);

	if (m_nCurrSelItem  >=  0)
	{
		m_ObjMarkSet.stSub[m_nCurrSelItem].bIsFillFirst = m_IsFirstMarkFill;
		SetSubInfoToList(m_ObjMarkSet, m_nCurrSelItem, FALSE);
	}
}

void CFillObjmMarkSetDlg::OnChangeEditMarkcount() 
{
	UpdateData(TRUE);
	if (m_nCurrSelItem  >=  0)
	{
		m_ObjMarkSet.stSub[m_nCurrSelItem].nSubCnt = m_nMarkCnt;
	}
}

BOOL CFillObjmMarkSetDlg::SetSubInfoToList(OBJMARKSET& ObjMarkSet, int nNum, BOOL bAddFlag) 
{
	TCHAR  szNum[20] = { 0 };
	_stprintf_s(szNum,20, _T("%d"),  nNum);

	TCHAR szLayer[20] = {0};
	_stprintf_s(szLayer,20, _T("   %d"),  ObjMarkSet.stSub[nNum].nLayer);

	TCHAR szFram[20] = {0};
	if ( ObjMarkSet.stSub[nNum].bIsMarkFram ) 
	{
		_tcscpy_s(szFram,20, _T("��"));
	}
	else
	{
		_tcscpy_s(szFram,20, _T(""));
	}

	TCHAR szFill[20] = { 0 };
	if ( ObjMarkSet.stSub[nNum].bIsMarkFill ) 
	{
		_tcscpy_s(szFill,20, _T("��"));
	}
	else
	{
		_tcscpy_s(szFill, 20,_T(""));
	}

	TCHAR szFillFirst[20] = {0};
	if ( ObjMarkSet.stSub[nNum].bIsFillFirst ) 
	{
		_tcscpy_s(szFillFirst, 20, _T("��"));
	}
	else
	{
		_tcscpy_s(szFillFirst, 20, _T(""));
	}

	if ( bAddFlag ) 
	{
		m_MarkInfoList.AddItem(szNum, szLayer, szFram, szFill, szFillFirst);
	}
	else
	{
		m_MarkInfoList.SetItemText(nNum, 0, szNum);
		m_MarkInfoList.SetItemText(nNum, 1, szLayer);
		m_MarkInfoList.SetItemText(nNum, 2, szFram);
		m_MarkInfoList.SetItemText(nNum, 3, szFill);
		m_MarkInfoList.SetItemText(nNum, 4, szFillFirst);
	}

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc  !=  NULL ) 
	{
		CLayer ly = pDoc->GetLayer(ObjMarkSet.stSub[nNum].nLayer);
		m_MarkInfoList.SetItemColor(nNum, 1, 0, ly.m_color);
	}
	
	return TRUE;
}


BOOL CFillObjmMarkSetDlg::InitMarkInfoList() 
{
    m_MarkInfoList.SetExtendedStyle (	LVS_EX_FULLROWSELECT|LVS_SHAREIMAGELISTS|
													LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES );

	TCHAR  szNum[20] = {0},szLayer[20] = {0}, szFram[20] = {0};
	TCHAR  szFill[32] = {0},szFillFirst[32] = {0}, szParaHead[100] = {0};

	CString strLoad;
	strLoad.LoadString(IDS_MARKSET_NUM);
	_tcscpy_s(szNum, 20, strLoad.GetBuffer(strLoad.GetLength()));
	strLoad.ReleaseBuffer();
	
	strLoad.LoadString(IDS_MARKSET_LAYER);
	_tcscpy_s(szLayer, 20, strLoad.GetBuffer(strLoad.GetLength()));
	strLoad.ReleaseBuffer();

	strLoad.LoadString(IDS_MARKSET_FRAM);
	_tcscpy_s(szFram, 20, strLoad.GetBuffer(strLoad.GetLength()));
	strLoad.ReleaseBuffer();

	strLoad.LoadString(IDS_MARKSET_FILL);
	_tcscpy_s(szFill,32, strLoad.GetBuffer(strLoad.GetLength()));
	strLoad.ReleaseBuffer();

	strLoad.LoadString(IDS_MARKSET_FILLFIRST);
	_tcscpy_s(szFillFirst, 32, strLoad.GetBuffer(strLoad.GetLength()));
	strLoad.ReleaseBuffer();

	_stprintf_s(szParaHead,100, _T("%s,20;%s,35;%s,37;%s,37;%s,38"),szNum,szLayer,szFram,szFill,szFillFirst);
	m_MarkInfoList.SetHeadings(szParaHead);

	return TRUE;
}



void CFillObjmMarkSetDlg::OnClickListMarkinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;

	int nItem   = -1;
	int nSubItem = 0;
	HitTestEx(&m_MarkInfoList, pNMHDR, nItem, nSubItem);
	if (  nItem < 0  )
		return;

	m_nCurrSelItem = nItem;
	m_IsFirstMarkFill = m_ObjMarkSet.stSub[nItem].bIsFillFirst;
	m_IsMarkFill = m_ObjMarkSet.stSub[nItem].bIsMarkFill;
	m_IsMarkFram = m_ObjMarkSet.stSub[nItem].bIsMarkFram;
	m_nMarkCnt   = m_ObjMarkSet.stSub[nItem].nSubCnt;

	m_bIsResMark = m_ObjMarkSet.stSub[nItem].bIsRevMark;

	if (m_ObjMarkSet.stSub[nItem].nLayer < 0) 
	{
		m_ObjMarkSet.stSub[nItem].nLayer = 0;
	}

	m_MarkParaList.SetCurSel(m_ObjMarkSet.stSub[nItem].nLayer);
	/////
	CLayer ly    = pDoc->GetLayer(m_ObjMarkSet.stSub[nItem].nLayer);
	m_dbQFreq  = ly.m_para.emcc.dbQFreq;
	m_dbMarkSpeed  = ly.m_para.emcc.dbMarkV;
	m_dbMarkCurrent = ly.m_para.emcc.dbCurrent;

	SetSubInfoToList(m_ObjMarkSet, nItem, FALSE);
	m_MarkInfoList.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	m_MarkInfoList.SetCurSel(nItem);

	if (m_IsMarkFill && m_IsMarkFram)
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	
	*pResult = 0;
}

BOOL CFillObjmMarkSetDlg::HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem, int &nSubItem)
{
    if ( pCtrl  == NULL )   return FALSE ;
	if ( pNMHDR == NULL ) return FALSE;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	nItem    = pCtrl->HitTest( point, NULL);
	nSubItem = -1;

	((CSortListCtrl*)pCtrl)->SetCurSel(nItem);

	return TRUE;
}

void CFillObjmMarkSetDlg::SetSelObjColor(OBJMARKSET &ObjMarkSet, int nItem)
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	if ( pDoc->GetStatus() == CChain::selectNull )
	{
		AfxMessageBox(IDS_NOOBJECT,MB_ICONWARNING);
		return;
	}

	CLayer layer = pDoc->GetLayer(ObjMarkSet.stSub[nItem].nLayer);
	if ( ObjMarkSet.stSub[nItem].bIsMarkFram ) 
	{
		pDoc->LayChange(PTY_OUTLINE, layer);
		for (int i = 0; i < ObjMarkSet.nMarkCount; i++)
		{
			if (ObjMarkSet.stSub[i].bIsMarkFill) 
			{
				CLayer ly = pDoc->GetLayer(ObjMarkSet.stSub[i].nLayer);
				pDoc->LayChange(PTY_FILL, ly);
				break;
			}
		}
	}

	if ( ObjMarkSet.stSub[nItem].bIsMarkFill )
	{
		pDoc->LayChange(PTY_FILL, layer);
		for (int i = 0; i < ObjMarkSet.nMarkCount; i++)
		{
			if ( ObjMarkSet.stSub[i].bIsMarkFram ) 
			{
				CLayer ly = pDoc->GetLayer(ObjMarkSet.stSub[i].nLayer);
				pDoc->LayChange(PTY_OUTLINE, ly);
				break;
			}
		}
	}

	pDoc->SetTrackerQuad(pDoc->GetLimit());
}

void CFillObjmMarkSetDlg::Apply()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return;
	
	int  nLayerCnt = m_combLayerCnt.GetCurSel();
	if ( nLayerCnt  >=  0 )
	{
		SetSelObjColor(m_ObjMarkSet, 0);
		m_ObjMarkSet.nMarkCount  = nLayerCnt + 1;
	}
	else
	{
		OBJMARKSET markSet;
		m_ObjMarkSet = markSet;
	}

	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_USEED))->GetCheck();
	if ( !bRet )
	{
		m_ObjMarkSet.nMarkCount = 0;

		((CComboBox*)GetDlgItem(IDC_COMBO_MARKLAYERCOUNT))->SetCurSel(-1);
		((CListCtrl*)GetDlgItem(IDC_LIST_MARKINFO))->DeleteAllItems();
		((CComboBox*)GetDlgItem(IDC_COMBO_MARKPARA))->SetCurSel(-1);
		((CButton*)GetDlgItem(IDC_CHECK_ISMARKFRAM))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_ISMARKFILL))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_CHECK_ISRESMARK))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK_SELMARK))->SetCheck(FALSE);
	}

	pDoc->UpdateObjMarkSet(m_ObjMarkSet);
	if ( nLayerCnt  >=  0 )
	{
		SetSelObjColor(m_ObjMarkSet, 0);
	}
}

void CFillObjmMarkSetDlg::OnCheckSelmark() 
{
#ifdef _SOFTMODE_EDITOR_
	return;
#endif

	if ( !g_DAT.IsCheckStatusOK() )
		return;

	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SELMARK))->GetCheck();
	if (  bRet )
	{
		if ( g_bIsRedPreview )
		{
			theApp.m_RedPreView.StopPreview();
			g_bIsRedPreview = FALSE;
			return;
		}

		if ( g_bIsFastMark ) 
		{
			theApp.m_FastMark.StopFastMark();
			return;
		}

		if ( g_bIsSelMark )
		{
			theApp.m_SelMark.StopSelMark();
			return;
		}
		
		g_bIsSelMark = TRUE;
		SUBOBJMARKSET stSub = m_ObjMarkSet.stSub[m_nCurrSelItem];
		theApp.m_SelMark.StartSelMark(this->m_hWnd,stSub, ENU_MARKSEL);
	}
	else
	{
		theApp.m_SelMark.StopSelMark();
	}
}

LRESULT CFillObjmMarkSetDlg::OnSelMarkEnd(WPARAM wParam,LPARAM lParam)
{	
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SELMARK))->GetCheck();
	if ( bRet )
	{
		((CButton*)GetDlgItem(IDC_CHECK_SELMARK))->SetCheck(FALSE);
	}

	if ( g_bIsSelMark )
	{
		g_DAT.LeaveMarkEnv();
	    g_bIsSelMark = FALSE;
	}

	return 0;
}

void CFillObjmMarkSetDlg::OnCheckUseed()
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_USEED))->GetCheck();

	UsedMarkSet(bRet);
}

void CFillObjmMarkSetDlg::UsedMarkSet(BOOL bUsed) 
{
	if ( bUsed ) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_USEED))->SetCheck(TRUE);
		GetDlgItem(IDC_COMBO_MARKLAYERCOUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_MARKINFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MARKPARA)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ISMARKFRAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ISMARKFILL)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_ISRESMARK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MARKCOUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SELMARK)->EnableWindow(TRUE);
	}
	else
	{
		
		((CButton*)GetDlgItem(IDC_CHECK_USEED))->SetCheck(FALSE);
		GetDlgItem(IDC_COMBO_MARKLAYERCOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_MARKINFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MARKPARA)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ISMARKFRAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ISMARKFILL)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_ISRESMARK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MARKCOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SELMARK)->EnableWindow(FALSE);
	}
	
	BOOL bFrame = ((CButton*)GetDlgItem(IDC_CHECK_ISMARKFRAM))->GetCheck();
	BOOL bFill  = ((CButton*)GetDlgItem(IDC_CHECK_ISMARKFILL))->GetCheck();
	if (bFrame && bFill) 
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_ISFIRSTMARKFILL)->EnableWindow(FALSE);
	}
	
}

void CFillObjmMarkSetDlg::OnBtnDelete() 
{
	UpdateData(TRUE);
	////
	int index = m_spParaList.GetCurSel();
	if (  index == CB_ERR )
		return;

	CString filename;
	CString strPath = ::g_sys.GetInstallPath();
	m_spParaList.GetLBText(index, filename);	
	filename = strPath + filename + _T(".psd");

	if ( !DeleteFile(filename) )
	{
		AfxMessageBox(IDS_YH_DELETE);
		return;
	}

	m_spParaList.DeleteString(index);
	m_spParaList.SetCurSel(-1);

	OBJMARKSET ObjMarkSet;
	m_ObjMarkSet = ObjMarkSet;
	DisplayMarkSet(ObjMarkSet);

}

void CFillObjmMarkSetDlg::OnCheckIsresmark() 
{
	UpdateData(TRUE);
	////
	if ( m_nCurrSelItem  >=  0 )
	{
		m_ObjMarkSet.stSub[m_nCurrSelItem].bIsRevMark = m_bIsResMark;
	}
}

BOOL CFillObjmMarkSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
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

	return CDialog::PreTranslateMessage(pMsg);
}

