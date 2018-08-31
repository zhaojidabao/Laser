// DlgRulerEdit.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgRulerEdit.h"
#include "DlgRulerObjSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRulerEdit dialog

CDlgRulerEdit::CDlgRulerEdit(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgRulerEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRulerEdit)
	m_nRulerType = 0;
	m_dbDiameter = 100.0;
	m_dbLineDist = 1.0;
	m_dbCircleDis = 1.0;
	m_dbStart = 0.0;
	m_nLineCount = 101;
	m_dbAdd = 1.0;
	m_bRemoveLastLine = FALSE;
	m_nAlignType = 0;
	//}}AFX_DATA_INIT
	
	m_nCurObjNu = 0;
	m_ptySel.m_nLayer = 1;
	m_ptySel.m_color = RGB(255,0,0);
}


void CDlgRulerEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRulerEdit)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_objView);
	DDX_Control(pDX, IDC_LIST_OBJ, m_listCtrObj);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_nRulerType);
	DDX_Text(pDX, IDC_EDIT_DIA, m_dbDiameter);
	DDV_MinMaxDouble(pDX, m_dbDiameter, 0., 9999999.);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_LINE, m_dbLineDist);
	DDV_MinMaxDouble(pDX, m_dbLineDist, 0., 9999999.);
	DDX_Text(pDX, IDC_EDIT_DISTANCE_CIRCLE, m_dbCircleDis);
	DDV_MinMaxDouble(pDX, m_dbCircleDis, 0., 999999.);
	DDX_Text(pDX, IDC_EDIT_START, m_dbStart);
	DDV_MinMaxDouble(pDX, m_dbStart, -9999999., 99999999.);
	DDX_Text(pDX, IDC_EDIT_LINECOUNT, m_nLineCount);
	DDV_MinMaxInt(pDX, m_nLineCount, 0, 99999999);
	DDX_Text(pDX, IDC_EDIT_ADD, m_dbAdd);
	DDV_MinMaxDouble(pDX, m_dbAdd, -999999., 999999.);
	DDX_Check(pDX, IDC_CHECK_REMOVELASTLINE, m_bRemoveLastLine);
	DDX_Radio(pDX, IDC_RADIO_ALIGNTOP, m_nAlignType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_ALIGN, m_btnView);
}


BEGIN_MESSAGE_MAP(CDlgRulerEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgRulerEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OBJ, OnClickListObj)
	ON_BN_CLICKED(IDC_CHECK_REMOVELASTLINE, OnCheckRemovelastline)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_RADIO_TYPE, OnRadioType)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OBJ, OnDblclkListObj)
	ON_BN_CLICKED(IDC_RADIO_ALIGNDOWN, OnRadioAligndown)
	ON_BN_CLICKED(IDC_RADIO_ALIGNTOP, OnRadioAligntop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRulerEdit message handlers
BOOL CDlgRulerEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitZoomView();
	InitListCtrl();
	UpdateRulerTypeInfo();

	if (m_nAlignType == 0)
		m_btnView.SetIcon(IDI_ICON_RULER_ALIGNTOP);
	else
		m_btnView.SetIcon(IDI_ICON_RULER_ALIGNDOWN);
	m_btnView.DrawBorder(FALSE);
	
	m_dbDiameter = g_SysCfg.m_dbDiameter;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgRulerEdit::InitListCtrl()
{
	m_listCtrObj.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CString str = _T("");

	str.LoadString(IDS_RULER_OBJECT);
	m_listCtrObj.InsertColumn(0,str,LVCFMT_LEFT,38);
	
	str.LoadString(IDS_RULER_JUMPCOUNT);
	m_listCtrObj.InsertColumn(1,str,  LVCFMT_LEFT,50);

	str.LoadString(IDS_RULER_VISIBLEMARK);
	m_listCtrObj.InsertColumn(2,str,  LVCFMT_LEFT,60);

	str.LoadString(IDS_RULER_DECIMALDIGITS);
	m_listCtrObj.InsertColumn(3,str,  LVCFMT_LEFT,60);

	str.LoadString(IDS_RULER_PRIORITY);
	m_listCtrObj.InsertColumn(4,str,  LVCFMT_LEFT,50);

	UpdateListObjView();
}



//
void CDlgRulerEdit::UpdateListObjView()
{
	m_listCtrObj.DeleteAllItems();
	
	int i = 0;
	CString str = _T("");
	CGroupObj *pGroupObj = NULL;
	RULEROBJINFO rulerObjInfo;
	for(i = 0; i<m_arrayRulerObjInfo.GetSize(); i++)
	{	
		rulerObjInfo = m_arrayRulerObjInfo.GetAt(i);
		pGroupObj = (CGroupObj *)rulerObjInfo.m_pObj;

		m_listCtrObj.InsertItem(i, pGroupObj->m_strObjName, 0);

		str.Format(_T("%d"), rulerObjInfo.m_nJumpCount);
		m_listCtrObj.SetItemText(i, 1, str);
		
		if (rulerObjInfo.m_bViewNumber)
			str = _T("Yes");
		else
			str = _T("No");
		m_listCtrObj.SetItemText(i, 2, str);
		
		str.Format(_T("%d"), rulerObjInfo.m_nDecimalDigits);
		m_listCtrObj.SetItemText(i, 3, str);

		str.Format(_T("%d"), i);
		m_listCtrObj.SetItemText(i, 4, str);
	}
}



void CDlgRulerEdit::UpdateObjInfo()
{
	if (m_nCurObjNu == -1 || m_nCurObjNu  >=  m_arrayRulerObjInfo.GetSize())
		return;
	
	RULEROBJINFO rulerObjInfo;
	CGroupObj *pGroup = NULL;
	int i = 0;
	for(i = 0; i<m_arrayRulerObjInfo.GetSize(); i++)
	{
		rulerObjInfo = m_arrayRulerObjInfo.GetAt(i);
		pGroup = (CGroupObj *)rulerObjInfo.m_pObj;

		if (i == m_nCurObjNu)
			pGroup->SetProperty(PTY_INIT, m_ptySel);
		else
			pGroup->SetProperty(PTY_INIT, m_ptyInit);
	}

	UpdateZoomList();
}






BOOL CDlgRulerEdit::HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem)
{
    if (pCtrl  == NULL) return FALSE;
	if (pNMHDR == NULL) return FALSE;
	
	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	nItem    = pCtrl->HitTest( point, NULL );
	
	return TRUE;
}




void CDlgRulerEdit::OnClickListObj(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HitTestEx(&m_listCtrObj, pNMHDR, m_nCurObjNu);
	if (-1  !=  m_nCurObjNu)
	{
		UpdateObjInfo();
		UpdateData(TRUE);
	}
	
	*pResult = 0;
}


void CDlgRulerEdit::AddObj(CGroupObj *pGroup)
{
	if ( pGroup == NULL)
		return;
	
	CGroupObj *pGroupNew = new CGroupObj();//pGroup, 0x01);
	if ( pGroupNew  !=  NULL)
	{
		pGroupNew->SetParent(pGroup->GetParent());
		pGroupNew->CopyToMe(pGroup,0x00);
		pGroupNew->SetProperty(PTY_INIT, m_ptyInit);
		m_listGroupSource.AddTail(pGroupNew);
		
		RULEROBJINFO rulerObjInfo;
		rulerObjInfo.m_pObj = (DWORD)pGroupNew;
		rulerObjInfo.m_nPriority = m_arrayRulerObjInfo.GetSize();
		m_arrayRulerObjInfo.Add(rulerObjInfo);
	}
}

void CDlgRulerEdit::OnCheckRemovelastline() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CDlgRulerEdit::OnButtonUp()
{
	// TODO: Add your control notification handler code here
	if (m_nCurObjNu < 0)
		return;
	
	if (m_nCurObjNu > 0)
	{
		RULEROBJINFO ruler1, ruler2;
		ruler1 = m_arrayRulerObjInfo.GetAt(m_nCurObjNu - 1);
		ruler2 = m_arrayRulerObjInfo.GetAt(m_nCurObjNu);
		
		m_arrayRulerObjInfo.SetAt(m_nCurObjNu, ruler1);
		m_arrayRulerObjInfo.SetAt(m_nCurObjNu-1, ruler2);
		
		m_nCurObjNu = m_nCurObjNu-1;
		UpdateListObjView();
	}
	
	m_listCtrObj.SetSelectionMark(m_nCurObjNu);
	m_listCtrObj.SetItemState(m_nCurObjNu,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}



void CDlgRulerEdit::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurObjNu > m_arrayRulerObjInfo.GetSize()-1)
		return;
	
	if (m_nCurObjNu < m_arrayRulerObjInfo.GetSize()-1)
	{
		RULEROBJINFO ruler1, ruler2;
		ruler1 = m_arrayRulerObjInfo.GetAt(m_nCurObjNu + 1);
		ruler2 = m_arrayRulerObjInfo.GetAt(m_nCurObjNu);
		
		m_arrayRulerObjInfo.SetAt(m_nCurObjNu, ruler1);
		m_arrayRulerObjInfo.SetAt(m_nCurObjNu+1, ruler2);
		
		m_nCurObjNu = m_nCurObjNu+1;
		UpdateListObjView();
	}
	
	m_listCtrObj.SetSelectionMark(m_nCurObjNu);
	m_listCtrObj.SetItemState(m_nCurObjNu,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}

void CDlgRulerEdit::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurObjNu < 0 )
		return;
	
	CDlgRulerObjSet dlg;
	
	RULEROBJINFO rulerObjInfo = m_arrayRulerObjInfo.GetAt(m_nCurObjNu);
	dlg.m_nJumpCount = rulerObjInfo.m_nJumpCount;
	dlg.m_bViewNumber = rulerObjInfo.m_bViewNumber;
	dlg.m_nDecimalDigits = rulerObjInfo.m_nDecimalDigits;

	if (dlg.DoModal() == IDOK)
	{
		rulerObjInfo.m_nJumpCount = dlg.m_nJumpCount;
		rulerObjInfo.m_bViewNumber = dlg.m_bViewNumber;
		rulerObjInfo.m_nDecimalDigits = dlg.m_nDecimalDigits;
	}
	m_arrayRulerObjInfo.SetAt(m_nCurObjNu, rulerObjInfo);
	
	UpdateListObjView();
	m_listCtrObj.SetSelectionMark(m_nCurObjNu);
}

void CDlgRulerEdit::InitZoomView()
{
	CHLApp *pApp = (CHLApp *)AfxGetApp();
	if (g_bIsRedPreview)
	{	
		g_bIsRedPreview = FALSE;
		pApp->m_RedPreView.StopPreview();
	}
	
	if (g_bIsSelMark) 
	{
		pApp->m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		pApp->m_FastMark.StopFastMark();
	}
	
	CRect rt;
	m_objView.GetClientRect(&rt);
	double x(0.0), y(0.0);
	g_DAT.GetMarkRect(x,y);
	
	m_xzoom.CreateZoomEx(rt,this,ID_ZOOM_WND,x,y);
	m_xzoom.SetZoomScale(g_SysCfg.m_dbPreViewZoomScale);
	
	CWnd *hWin = GetDlgItem(IDC_STATIC_VIEW);
	RECT rectObj;
	hWin->GetWindowRect(&rectObj);
	ScreenToClient(&rectObj);
	m_xzoom.MoveWindow(&rectObj);
		
	UpdateZoomList();
}

void CDlgRulerEdit::UpdateZoomList()
{
	StrokeList  list;
	
	int i = 0;
	CGroupObj *pGroupObj = NULL;
	RULEROBJINFO rulerObjInfo;
	for(i = 0; i<m_arrayRulerObjInfo.GetSize(); i++)
	{	
		rulerObjInfo = m_arrayRulerObjInfo.GetAt(i);
		pGroupObj = (CGroupObj *)rulerObjInfo.m_pObj;
		
		if ( pGroupObj  !=  NULL )
		{
			pGroupObj->LoadStrokeList(&list, FALSE, -1);
		}
	}
	
	m_xzoom.SetStrokeList(&list);
	m_xzoom.Invalidate();
}



void CDlgRulerEdit::OnRadioType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateRulerTypeInfo();
}

void CDlgRulerEdit::OnRadio2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateRulerTypeInfo();
}



void CDlgRulerEdit::UpdateRulerTypeInfo()
{
	GetDlgItem(IDC_EDIT_DIA)->EnableWindow(m_nRulerType == 0);	
	GetDlgItem(IDC_EDIT_DISTANCE_CIRCLE)->EnableWindow(m_nRulerType == 0);
	GetDlgItem(IDC_EDIT_DISTANCE_LINE)->EnableWindow(m_nRulerType == 1);
}




void CDlgRulerEdit::CreateRulerData()
{
	UpdateData(TRUE);
	
	CGroupObj *pGroup = NULL;
	RULEROBJINFO rulerObj;
	int i = 0, j = 0;
	for(i = 0; i<m_nLineCount; i++)
	{
		//忽略最后一根线
		if (m_bRemoveLastLine && i == m_nLineCount - 1)
			break;
		
		for(j = 0; j<m_arrayRulerObjInfo.GetSize(); j++)
		{
			rulerObj = m_arrayRulerObjInfo.GetAt(j);
			
			if (i%rulerObj.m_nJumpCount == 0)
			{
				pGroup = CreateRulerLine(i, rulerObj);
				
				if (pGroup  !=  NULL)
				{
					m_listGroup.AddTail(pGroup);	
					break;
				}
			}//
		}//
	}
}


CGroupObj* CDlgRulerEdit::CreateRulerLine(int i, RULEROBJINFO &rulerObj)
{
	CGroupObj *pOrg = (CGroupObj *)rulerObj.m_pObj;
	if ( pOrg == NULL)
		return NULL;

	CGroupObj *pGroup = new CGroupObj();
	if ( pGroup == NULL )
		return NULL;

	pGroup->SetParent(pOrg->GetParent());
	pGroup->CopyToMe(pOrg,0x00);
	pGroup->SetProperty(PTY_INIT, m_ptyInit);
	
	//刻度值处理
	if (rulerObj.m_bViewNumber)
	{
		CString strMark;
		double dbMark = m_dbStart + i*m_dbAdd;
		strMark.Format(_T("%f"), dbMark);
		
		//小数位数处理
		int nPos = 0, nSize = 0, k = 0, nCount = 0;

		nSize = strMark.GetLength();
		nPos = strMark.Find(_T("."));
		if (nPos < 0)
		{
			strMark  +=  _T(".");
			nCount = rulerObj.m_nDecimalDigits;
		}
		else
		{
			if (rulerObj.m_nDecimalDigits == 0)
			{
				strMark = strMark.Left(nPos);
				nCount = 0;
			}
			else
			{
				nCount = rulerObj.m_nDecimalDigits - ((nSize - 1) - nPos);

				if ( nCount < 0 )
					strMark = strMark.Left(nPos + rulerObj.m_nDecimalDigits + 1);
			}
		}
		
		for(k = 0; k<nCount; k++)
		{
			strMark  +=  _T("0");
		}
		
		//字符串替换
		CString strName;
		pGroup->SetTextData(strMark);
	}
	
	//位置处理
	double dbMoveX = 0, dbMoveY = 0;
	double dbDisTance = 0;
	if ( m_nRulerType == 0 ) //圆周
	{
		dbDisTance = (m_dbDiameter * 0.5) * (m_dbCircleDis*PAI/180);
	}
	else //直线
	{
		dbDisTance = m_dbLineDist;
	}
	
	CDot  dotPos;
	if (pGroup->GetRulerPos(dotPos, m_nAlignType) == FALSE)
	{
		CQuad quad = pGroup->GetLimit();

		if (m_nAlignType == 0)
			dotPos = quad.GetBaseDot(BASE_TM);
		else
			dotPos = quad.GetBaseDot(BASE_BM);
	}
	
	dbMoveX = i*dbDisTance - dotPos.x;
	dbMoveY = -dotPos.y;
	pGroup->Move(dbMoveX, dbMoveY,0);
	
	//
	return pGroup;
}


//
void CDlgRulerEdit::OnOK()
{
	// TODO: Add extra validation here
	CreateRulerData();
	DeleteSourceObjList();

	g_SysCfg.m_dbDiameter = m_dbDiameter;
	g_sys.WriteSysCfg();
	
	CDialog::OnOK();
}


void CDlgRulerEdit::OnCancel()
{
	// TODO: Add extra cleanup here
	DeleteDataObjList();
	DeleteSourceObjList();
	
	CDialog::OnCancel();
}



void CDlgRulerEdit::DeleteDataObjList()
{
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		CGroupObj *pGroup = (CGroupObj *)m_listGroup.GetNext(pos);

		if (pGroup  !=  NULL)
		{
			delete pGroup;
		}
	}

	m_listGroup.RemoveAll();
}




void CDlgRulerEdit::DeleteSourceObjList()
{
	POSITION pos = m_listGroupSource.GetHeadPosition();
	while( pos  !=  NULL )
	{
		CGroupObj *pGroup = (CGroupObj *)m_listGroupSource.GetNext(pos);
		
		if (pGroup  !=  NULL)
		{
			delete pGroup;
		}
	}
	
	m_listGroupSource.RemoveAll();
}

void CDlgRulerEdit::OnDblclkListObj(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonSet();
	*pResult = 0;
}

void CDlgRulerEdit::OnRadioAligndown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_btnView.SetIcon(IDI_ICON_RULER_ALIGNDOWN);
}

void CDlgRulerEdit::OnRadioAligntop() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_btnView.SetIcon(IDI_ICON_RULER_ALIGNTOP);
}

