// DlgIntensity.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgIntensity.h"
#include "LocNukeForHL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity dialog
CDlgIntensity::CDlgIntensity(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgIntensity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIntensity)
	m_iLowGray = 0;
	m_iUpGray = 0;
	//}}AFX_DATA_INIT
	m_nListType = 1;
}


void CDlgIntensity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIntensity)
	DDX_Control(pDX, IDC_LIST_GRAYLIST, m_listCtrGray);
	DDX_Text(pDX, IDC_EDIT_LowGray, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UpGray, m_iUpGray);
	DDV_MinMaxInt(pDX, m_iUpGray, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIntensity, CDialog)
	//{{AFX_MSG_MAP(CDlgIntensity)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_EDIT_LowGray, OnKillfocusEDITLowGray)
	ON_EN_KILLFOCUS(IDC_EDIT_UpGray, OnKillfocusEDITUpGray)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_GRAYLIST, OnColumnclickListGraylist)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgIntensity::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity message handlers
//
BOOL CDlgIntensity::OnInitDialog() 
{
	unsigned char * lpSrc;
	
	LONG i;
	LONG j;
	
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	//
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	//
	m_MouseRect.top  -=  rect.top;
	m_MouseRect.left  -=  rect.left;
	
	m_MouseRect.top  +=  25;
	m_MouseRect.left  +=  10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	for (i = 0; i < 256; i ++)
	{
		m_lCount[i] = 0;
	}	
	
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(m_lWidth * 8);
	
	//
	for (i = 0; i < m_lHeight; i ++)
	{
		for (j = 0; j < m_lWidth; j ++)
		{
			lpSrc = (unsigned char *)m_lpDIBBits + lLineBytes * i + j;
			m_lCount[*(lpSrc)]++;
		}
	}
	m_iIsDraging = 0;
	
	//初始化灰度统计列表
	InitGrayList();
	
	return TRUE;
}





//
void CDlgIntensity::OnKillfocusEDITLowGray()
{
	UpdateData(TRUE);
	
	// 
	if (m_iLowGray > m_iUpGray)
	{	
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		UpdateData(FALSE);
	}
	
	InvalidateRect(m_MouseRect, TRUE);
}	




//
void CDlgIntensity::OnKillfocusEDITUpGray()
{
	UpdateData(TRUE);
	
	if (m_iLowGray > m_iUpGray)
	{
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		UpdateData(FALSE);
	}
	
	InvalidateRect(m_MouseRect, TRUE);
}



//
void CDlgIntensity::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_iLowGray))
		{
			m_iIsDraging = 1;
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_iUpGray))
		{
			m_iIsDraging = 2;
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}



//
void CDlgIntensity::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (m_MouseRect.PtInRect(point))
	{
		if (m_iIsDraging  !=  0)
		{
			if (m_iIsDraging == 1)
			{
				if (point.x - m_MouseRect.left < m_iUpGray)
				{
					m_iLowGray = point.x - m_MouseRect.left;
				}
				else
				{
					m_iLowGray = m_iUpGray - 1;
					point.x = m_MouseRect.left + m_iUpGray - 1;
				}
			}
			else
			{
				if (point.x - m_MouseRect.left > m_iLowGray)
				{
					m_iUpGray = point.x - m_MouseRect.left;
				}
				else
				{
					m_iUpGray = m_iLowGray + 1;
					point.x = m_MouseRect.left + m_iLowGray + 1;
				}
			}

			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_iLowGray) || point.x == (m_MouseRect.left + m_iUpGray))
		{
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}



//
void CDlgIntensity::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_iIsDraging  !=  0)
	{
		m_iIsDraging = 0;
	}
	
	UpdateGrayList();
	CDialog::OnLButtonUp(nFlags, point);
}



//
void CDlgIntensity::OnPaint() 
{	
	CString str;
	LONG i;
	
	LONG lMaxCount = 0;
	CPaintDC dc(this);
	
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
	CPen* pPenRed = new CPen;
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	
	CPen* pPenBlue = new CPen;
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	CPen* pPenGreen = new CPen;
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	pDC->MoveTo(10,10);
	pDC->LineTo(10,280);
	pDC->LineTo(320,280);
	
	//
	pDC->TextOut(10,   283, _T("0"));
	pDC->TextOut(60,   283, _T("50"));
	pDC->TextOut(110, 283, _T("100"));
	pDC->TextOut(160, 283, _T("150"));
	pDC->TextOut(210, 283, _T("200"));
	pDC->TextOut(265, 283, _T("255"));
	
	//
	for (i = 0; i < 256; i  +=  5)
	{
		if ((i & 1) == 0)
		{
			//
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			//
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	
	//
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);
	
	//
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	//
	for (i = m_iLowGray; i  <=  m_iUpGray; i ++)
	{
		if ( m_lCount[i] > lMaxCount )
		{
			lMaxCount = m_lCount[i];
		}	
	}		
	
	//
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format(_T("%d"), lMaxCount);
	pDC->TextOut(11, 26, str);
	//
	pDC->SelectObject(pPenGreen);	
	//
	pDC->MoveTo(m_iLowGray + 10, 25);
	pDC->LineTo(m_iLowGray + 10, 280);
	
	pDC->MoveTo(m_iUpGray + 10, 25);
	pDC->LineTo(m_iUpGray + 10, 280);
	pDC->SelectObject(pPenBlue);	
	//
	if (lMaxCount > 0)
	{
		for (i = m_iLowGray; i  <=  m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_lCount[i] * 256 / lMaxCount));
		}
	}
	
	//
	pDC->SelectObject(pOldPen);	
	
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;	
}



//
void CDlgIntensity::OnOK() 
{
	if ( m_iLowGray > m_iUpGray )
	{
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
	}
	//
	CDialog::OnOK();
}


void CDlgIntensity::InitGrayList()
{
	m_listCtrGray.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CString str;
	str.LoadString(IDS_INTENSITY_GRAY);
	m_listCtrGray.InsertColumn(0,str,LVCFMT_LEFT,60);
	str.LoadString(IDS_INTENSITY_COUNT);
	m_listCtrGray.InsertColumn(1,str,  LVCFMT_LEFT,60);
	
	//设定统计框大小
	CWnd* pWnd = NULL;
	CRect rect;
	pWnd = GetDlgItem(IDC_LIST_GRAYLIST);
	pWnd->GetWindowRect(&rect);
	rect.top = m_MouseRect.top - 25;
	rect.bottom  = rect.top + 300;
	pWnd->MoveWindow(&rect);
	
	UpdateGrayList();
}


//
void CDlgIntensity::UpdateGrayList()
{
	CArray<SORTOBJ,SORTOBJ> arraySort;

	int i = 0;
	for(i = m_iLowGray; i  <=  m_iUpGray; i ++)
	{
		SORTOBJ sorObj;
		sorObj.m_dbValue = (double)m_lCount[i];
		sorObj.m_nFlag = i;
		
		arraySort.Add(sorObj);
	}
	
	//
	int nCount = arraySort.GetSize();
	if (m_nListType == 1)
	{
		BOOL bStop = FALSE;
		g_dataMng.QuickSort(arraySort, 0, nCount-1, bStop, FALSE);
	}
	
	//
	m_arrayStringGray.RemoveAll();
	m_arrayStringCount.RemoveAll();
	int nSize = nCount;
	for( i = 0; i<nSize; i++ )
	{
		SORTOBJ sotObjTemp = arraySort.GetAt(i);
		CString strGray, strCount;
		
		strGray.Format( _T("%d"), sotObjTemp.m_nFlag );
		strCount.Format( _T("%d"), (int)sotObjTemp.m_dbValue );
		
		m_arrayStringGray.Add(strGray);
		m_arrayStringCount.Add(strCount);
	}
	
	//
	m_listCtrGray.DeleteAllItems();
	for( i = 0; i < nSize; i++ )
	{
		m_listCtrGray.InsertItem(i, m_arrayStringGray.GetAt(i), 0);
		m_listCtrGray.SetItemText(i, 1, m_arrayStringCount.GetAt(i));
	}
}


void CDlgIntensity::OnColumnclickListGraylist(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//TODO: Add your control notification handler code here
	m_nListType = pNMListView->iSubItem;
	UpdateGrayList();
	
	*pResult = 0;
}


void CDlgIntensity::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
