// ZoomView.cpp : implementation file
//

#include "stdafx.h"
#include "ZoomView.h"
#include "Resource.h"
#include "DlgSetLinePos.h"

#include "ArrayDlg.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
  
/////////////////////////////////////////////////////////////////////////////
// CZoomView
#define MAX_MEASURE_LINE_SIZE 256

IMPLEMENT_DYNCREATE(CZoomView, CZoomViewB)
CZoomView::CZoomView()
{ 
	m_pArrayWnd = NULL;
}

CZoomView::~CZoomView()
{
	if ( m_arrayMeasureLine )
	{
		delete[] m_arrayMeasureLine;
		m_arrayMeasureLine = NULL;
	}
}

BEGIN_MESSAGE_MAP(CZoomView, CZoomViewB)
	//{{AFX_MSG_MAP(CZoomView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomView drawing

void CZoomView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CZoomView diagnostics

#ifdef _DEBUG
void CZoomView::AssertValid() const
{
	CZoomViewB::AssertValid();
}

void CZoomView::Dump(CDumpContext& dc) const
{
	CZoomViewB::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZoomView message handlers

void CZoomView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bRButDown = FALSE;
	SendMessage(WM_NCPAINT);
	ReleaseCapture();
	
	if (m_bCanMoveMeasureLine)
	{
		//移动测量线
		MoveMeasureLine(point);
		return;
	}
	if (point.x == 0 && point.y == 0)
	{
		//添加测量线条
		DoMeasureLine();
		return;
	}

	if ((m_ModeSelect == MODE_ZOOMOPER)||(m_ModeSelect == MODE_ZOOMSEL))//drag mode
	{
		m_bLBDowm = TRUE;
		m_ptPrev = m_ptNow = point;	
		m_ptResev = point;
	}
	
	OnArrayPro(point);
	
	CView::OnLButtonDown(nFlags, point);
}


void CZoomView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ( (m_ModeSelect == MODE_ZOOMOPER)||(m_ModeSelect == MODE_ZOOMSEL) )
	{
		if ( m_bLBDowm && !m_bDrag )
		{
			CDC *pDC = GetDC();
			if ( pDC == NULL )
				return;

			//clear the last rect
			int mod = pDC->SetROP2(R2_NOTXORPEN);
			CPen penWhite(PS_DOT,1,RGB(0,0,255));
			CPen *pOldPen = pDC->SelectObject(&penWhite);

			pDC->MoveTo(m_ptPrev);
			pDC->LineTo(m_ptNow.x,m_ptPrev.y);
			pDC->LineTo(m_ptNow);
			pDC->LineTo(m_ptPrev.x,m_ptNow.y);
			pDC->LineTo(m_ptPrev);
			pDC->SelectObject(pOldPen);
			penWhite.DeleteObject();
			
			double tx = m_ptPrev.x;
			double ty = m_ptPrev.y;
			m_DC.LogicToPhysic(tx,ty);

			double dbx = point.x;
			double dby = point.y;
			m_DC.LogicToPhysic(dbx,dby);

			m_DC.m_dbPhysicX = (tx + dbx)/2;
			m_DC.m_dbPhysicY = (ty + dby)/2;

			double dbWidth = abs(m_ptPrev.x-point.x);
			double dbHei = abs(m_ptPrev.y-point.y);
			
			if ( dbWidth*dbHei > 0 )
			{
				if ( m_rect.Width()/dbWidth>m_rect.Height()/dbHei )
				{
					double dbVal = m_DC.GetScale();
					dbVal *= m_rect.Height()/dbHei;
					m_DC.SetScale(dbVal);
				}
				else
				{
					double dbVal = m_DC.GetScale();
					dbVal *= m_rect.Width()/dbWidth;
					m_DC.SetScale(dbVal);
				}
				Invalidate();
				ReleaseDC(pDC);
			}
			pDC->SetROP2(mod);
		}
		m_bLBDowm = FALSE;
		SendMessage(WM_NCPAINT);
	}
	CView::OnLButtonUp(nFlags, point);
}




void CZoomView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	FlashCurPos(m_ptCurPos);
	m_ptCurPos = point;
	FlashCurPos(m_ptCurPos);
	
	CView::OnMouseMove(nFlags,point);
	if (MODE_ZOOMSEL == m_ModeSelect)
	{
		if (m_bLBDowm && !m_bDrag)
		{
			CDC *pDC = GetDC();
			if ( pDC == NULL )
				return;

			int mod = pDC->SetROP2(R2_NOTXORPEN);//反色掉
			CPen penBlack(PS_DOT,1,RGB(0,0,255));

			CPen *pOldPen = pDC->SelectObject(&penBlack);
			pDC->MoveTo(m_ptPrev);
			pDC->LineTo(m_ptNow.x,m_ptPrev.y);
			pDC->LineTo(m_ptNow);
			pDC->LineTo(m_ptPrev.x,m_ptNow.y);
			pDC->LineTo(m_ptPrev);
			pDC->SelectObject(pOldPen);
			
			//draw new rect
			pOldPen = pDC->SelectObject(&penBlack);
			pDC->MoveTo(m_ptPrev);
			pDC->LineTo(point.x,m_ptPrev.y);
			pDC->LineTo(point);
			pDC->LineTo(m_ptPrev.x,point.y);
			pDC->LineTo(m_ptPrev);
			pDC->SelectObject(pOldPen);

			penBlack.DeleteObject();
			m_ptNow = point;
			pDC->SetROP2(mod);
			ReleaseDC(pDC);

			SendMessage(WM_NCPAINT);
		}
	}
	if ( (MODE_ZOOMOPER == m_ModeSelect)|m_bRButDown )
	{	
		if ((m_bLBDowm && m_bDrag)|m_bRButDown)
		{
			double x,dx,y,dy;
			x = m_ptResev.x;
			y = m_ptResev.y;
			dx = point.x;
			dy = point.y;
			m_DC.LogicToPhysic(x,y);
			m_DC.LogicToPhysic(dx,dy);
			m_DC.m_dbPhysicX  -=  (dx-x);
			m_DC.m_dbPhysicY  -=  (dy-y);
			Invalidate();
			SendMessage(WM_NCPAINT);
			m_ptResev = point;
		}
	}
}

void CZoomView::OnSize(UINT nType, int cx, int cy) 
{	
	CView::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&m_rect);

	double dbWidth,dbHeight;
	dbWidth = m_rect.Width();
	dbHeight = m_rect.Height();
	m_DC.m_ptCenter.x = (int)dbWidth/2;
	m_DC.m_ptCenter.y = (int)dbHeight/2;
	
	m_DC.SetViewRect(dbWidth,dbHeight);
	SendMessage(WM_NCPAINT);
}


//
XDC *CZoomView:: PrepareDC(CDC *pDC)
{
	m_DC.m_pDC = pDC;

	return &m_DC;
}


//
BOOL CZoomView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if ( zDelta > 0 )
	{
		ZoomIn();
	}
	else
	{
		ZoomOut();
	}
	
	return TRUE;
}


void CZoomView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bRButDown = TRUE;
	m_ptPrev = m_ptNow = point;	
	m_ptResev = point;
	SetCapture();

	CView::OnMButtonDown(nFlags, point);
}

void CZoomView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bRButDown = FALSE;
	SendMessage(WM_NCPAINT);
	ReleaseCapture();

	CView::OnMButtonUp(nFlags, point);
}


void CZoomView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnRButtonDown(nFlags, point);
}

void CZoomView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CView::OnRButtonUp(nFlags, point);
}


BOOL CZoomView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if ( message == WM_LBUTTONDOWN && nHitTest == HTNOWHERE )
	{
		SendMessage(WM_LBUTTONDOWN, 0, 0);
	}
	
	return CZoomViewB::OnSetCursor(pWnd, nHitTest, message);
}

void CZoomView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCanSetLinePos )
	{
		CMeasureLine &line = m_arrayMeasureLine[m_nMeasureLineIndex];
		CDlgSetLinePos dlg;
		CString strTemp;
		strTemp.Format(_T("%.3f"), line.m_dbPos);
		dlg.m_dbLinePos = _tstof(strTemp);
		if ( dlg.DoModal() == IDOK )
		{
			line.m_dbPos = dlg.m_dbLinePos;
			CRect rect((int)(line.m_dbPos-5), 100, (int)(line.m_dbPos+5), -100);
			InvalidateRect(&rect);
		}
	}
	
	CView::OnLButtonDblClk(nFlags, point);
}

////////////////////////
void CZoomView::SetArrayDlg(CWnd *pWnd)
{
	m_pArrayWnd = pWnd;
}


BOOL CZoomView::Track(CPoint &point, CRect &rect, CWnd *pWnd)//是否选中对象
{
	BOOL bFlag = FALSE;
	
	AfxLockTempMaps();
	pWnd->SetCapture();

	if ( CWnd::GetCapture()  !=  pWnd )
	{
		return FALSE;
	}
	
	CPoint ptthis	 =  point;
	m_ptSPrev		 =  point;
	m_ptSNow		 =  m_ptSPrev;

	MSG msg;
	while( TRUE )
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				if (bFlag)
				{
					LastBrush(pWnd, m_ptSPrev, ptthis);
				}

				rect.left   = point.x;
				rect.right  = ptthis.x;
				rect.top    = point.y;
				rect.bottom = ptthis.y;

				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				bFlag    = TRUE;
				ptthis.x = (int)(short)LOWORD(msg.lParam);
				ptthis.y = (int)(short)HIWORD(msg.lParam);
				DrawRect(pWnd,m_ptSPrev,m_ptSNow,ptthis);
				m_ptSNow = ptthis;
				
				break;
			}
		default:
			{
				break;
			}
		} // end switch
	} // end for

ExitLoop:
	{
		ReleaseCapture();
		AfxUnlockTempMaps(FALSE);	
		return TRUE;
	}
	
}


void CZoomView::DrawSelQuadList()
{
	if ( m_pArrayWnd )
	{
		CArrayDlg *pArrayDlg = (CArrayDlg*)m_pArrayWnd;

		CDC *pDC = this->GetDC();
		XDC* pXDc = PrepareDC(pDC);

		if ( m_SelQuad.IsEmpty() )
		{
			int nCnt = pArrayDlg->m_QuadList.GetSize();
			for (int i = 0; i < nCnt; i++)
			{
				pArrayDlg->m_QuadList[i].m_bSel = FALSE;
			}
			return;
		}

		pArrayDlg->DrawSelTraker(pXDc, m_SelQuad, this, pArrayDlg->m_QuadList);
	}
}

void CZoomView::UpdateSelQuad()
{
	m_SelQuad.SetEmpty();

	if (m_pArrayWnd)
	{
		CArrayDlg *pArrayDlg = (CArrayDlg*)m_pArrayWnd;
		int nCnt = pArrayDlg->m_QuadList.GetSize();
		for (int j = 0; j < nCnt; j++)
		{
			if (TRUE == pArrayDlg->m_QuadList[j].m_bSel)
			{
				m_SelQuad.Compose(pArrayDlg->m_QuadList[j].m_QD);
			}
		}
	}
}

void CZoomView::OnArrayPro(CPoint &point) 
{
	if ( m_pArrayWnd )
	{
		CArrayDlg *pArrayDlg = (CArrayDlg*)m_pArrayWnd;

		CRect rect;
		pArrayDlg->MulObjSel(point, rect, this);
		CQuad quad;
		(CQuadB)quad = rect;
		LogicToPhysic(quad);
		quad.Normal();
		
		int nCnt = pArrayDlg->m_QuadList.GetSize();
		for (int i = 0; i < nCnt; i++)
		{
			pArrayDlg->m_QuadList[i].m_bSel = FALSE;
		}
		m_SelQuad.SetEmpty();
		
		int nSel = -1;
		for( int j = 0; j < nCnt; j++ )
		{
			if ( quad.Include(pArrayDlg->m_QuadList[j].m_QD) )
			{
				pArrayDlg->m_QuadList[j].m_bSel = TRUE;
				SELQUAD selQuad;
				selQuad = pArrayDlg->m_QuadList[j];
				m_SelQuad.Compose(selQuad.m_QD);
				nSel = j;
			}
		}

		pArrayDlg->DispSelObjProp(nSel);
		pArrayDlg->SetRedMarkQuadlist(pArrayDlg->m_QuadList);

		Invalidate();
	}
}
