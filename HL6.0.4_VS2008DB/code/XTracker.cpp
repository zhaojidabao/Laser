// XTracker.cpp: implementation of the XTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "XTracker.h"
#include "HLView.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif 
#define HITBORDER 3
#define MIN 1e-15
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern HICON   g_hIcoList[20];
extern HBITMAP g_hBmpDtt[7];

void XTracker::SelectStrokeBySelPt(CWnd *pWnd)
{
	CDot selDot;
	if ( !FindSelListFirstPt(selDot) )
	{
		return;
	}
	
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	BOOL     bFind   = FALSE;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke )
		{
			if ((pStroke->DotIsInStroke(selDot)) && (pStroke->m_bTag))
			{
				bFind = TRUE;
				break;
			}
		}
	}// end while
	
	if ( !bFind )
	{
		return;
	}
	
	InverDrawDot(pWnd);
	ClearDot();
	
	AddStrokeToEditList(pStroke);
	FillRect(pWnd);
	DataEXChange();
}


//
void XTracker::SetDotToStart(CWnd *pWnd)
{
	CDot SelDt;
	int nCount = FindSelListFirstPt(SelDt);
	if ( nCount > 1 )
	{
		AfxMessageBox(IDS_SELONEDOT_ONLY);
		return;
	}

	InverDrawDot(pWnd);
	ClearDot();
	
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if (pStroke)
		{
			if (pStroke->m_bTag)
			{
				pStroke->SetDotToStart(SelDt);
				break;
			}
		}
	}// end while
	
	AddStrokeToEditList(pStroke);
	FillRect(pWnd);
	DataEXChange();
}


void XTracker::FilterPoint(CWnd *pWnd, double dbPresic, double dbAngle)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{	
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->m_bTag )
		{
			pStroke->FilterPoint(dbPresic, dbAngle);
		}
	}// end while
}

//
void XTracker::ReverseStroke(CWnd *pWnd)
{
	CDot SelDt;
	if ( !FindSelListFirstPt(SelDt) )
		return;

	InverDrawDot(pWnd);
	ClearDot();
	
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke*)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->m_bTag )
		{
			pStroke->SetStrokeReverse(SelDt);
			break;
		}
	}
	
	AddStrokeToEditList(pStroke);
	FillRect(pWnd);
	DataEXChange();
}


void XTracker::DotHead::InverDrawDot(CWnd *pWnd)//反画
{
	if ( NULL == m_pHead )
		return;
	
	CZoomView *pView = (CZoomView*)pWnd;
	CDC *pDC       = pView->GetDC();
	CRect rect;
	CQuad quad;
	CDot  dot;
	DotSelectList *pList = m_pHead;
	double dbData = 2;
	pView->ActualLen(dbData);
	while(pList)
	{
		dot = pList->m_Node;
		quad.left = dot.x-dbData;
		quad.right = dot.x+1.75*dbData;
		quad.top = dot.y+dbData;
		quad.bottom = dot.y-1.75*dbData;
		pView->PhysicToLogic(quad);
		rect = (CRect)quad;
		pDC->FillSolidRect(&rect,RGB(255,255,255));
		pList = pList->m_pNext;
	}
	pView->ReleaseDC(pDC);
}



XTracker::XTracker()
{
	m_quadSave.left = m_quadSave.right = m_quadSave.top = m_quadSave.bottom = 0;
	m_ptNow = m_ptPrev = CPoint(0,0);
	m_HITBORDER = 0;
	m_pDotHead = m_pDotTail = NULL;
}


XTracker::~XTracker()
{
}


int XTracker:: HitPoint(CTree *pTree,CPoint &point,CWnd *pWnd)
{
	for(int i = 0;i<8;i++)
	{
		if (CtrlPoint(i,point,pTree,pWnd))
		{
			return i;
		}
	}
	
	if (InRect(point,pTree,pWnd))
	{
		return 8;
	}

	return 9;
}




BOOL XTracker::CtrlPoint(int i, CPoint &point,CTree *pTree,CWnd *pWnd)
{
	if (IsPtInRect(i,point,pTree,pWnd))
	{
		return TRUE;
	}
	
	return FALSE;
}


//
BOOL XTracker::InRect(CPoint &point,CTree *pTree,CWnd *pWnd)
{
	double dx = point.x;
	double dy = point.y;
	m_quadSave  = pTree->GetLimit();
	CZoomView *pView = (CZoomView*)pWnd;
	pView->LogicToPhysic(dx,dy);
	
	if (dx >= m_quadSave.left && dx <= m_quadSave.right && dy <= m_quadSave.top && dy >= m_quadSave.bottom)
	{
		return TRUE;
	}
	
	return FALSE;
}


void XTracker:: UpdateRectCtrl()
{
	(m_quad[0]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[0]).right = m_quadSave.left;
	(m_quad[0]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[0]).bottom =  m_quadSave.top;

	(m_quad[1]).left  = m_quadSave.right;
	(m_quad[1]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[1]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[1]).bottom =  m_quadSave.top;

	(m_quad[2]).left  = m_quadSave.right;
	(m_quad[2]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[2]).top   = m_quadSave.bottom;
	(m_quad[2]).bottom =  m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[3]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[3]).right = m_quadSave.left;
	(m_quad[3]).top   = m_quadSave.bottom;
	(m_quad[3]).bottom =  m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[4]).left  = (m_quadSave.left+m_quadSave.right)/2-m_HITBORDER;
	(m_quad[4]).right = (m_quadSave.left+m_quadSave.right)/2+m_HITBORDER;
	(m_quad[4]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[4]).bottom =  m_quadSave.top;

	(m_quad[5]).left  = m_quadSave.right;
	(m_quad[5]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[5]).top   = (m_quadSave.top+m_quadSave.bottom)/2+m_HITBORDER;
	(m_quad[5]).bottom =  (m_quadSave.top+m_quadSave.bottom)/2-m_HITBORDER;

	(m_quad[6]).left  = (m_quadSave.left+m_quadSave.right)/2-m_HITBORDER;
	(m_quad[6]).right = (m_quadSave.left+m_quadSave.right)/2+m_HITBORDER;
	(m_quad[6]).top   = m_quadSave.bottom;
	(m_quad[6]).bottom =  m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[7]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[7]).right = m_quadSave.left;
	(m_quad[7]).top   = (m_quadSave.top+m_quadSave.bottom)/2+m_HITBORDER;
	(m_quad[7]).bottom =  (m_quadSave.top+m_quadSave.bottom)/2-m_HITBORDER;

	return;
}


BOOL XTracker::IsPtInRect(int i, CPoint &point,CTree *pTree,CWnd *pWnd)
{
	if ( !pTree || !pWnd  )
		return FALSE;

	double dx = point.x;
	double dy = point.y;

	m_quadSave  = pTree->GetLimit();
	CZoomView *pView =  (CZoomView*)pWnd;
	pView->LogicToPhysic(dx,dy);

	m_HITBORDER = HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();

	CQuad quad = m_quad[i];
	if ( dx >= (m_quad[i]).left && dx <= (m_quad[i]).right && dy >= (m_quad[i]).bottom && dy <= (m_quad[i]).top )
	{
		return TRUE;
	}
	
	return FALSE;
}



//////////////////////////////////////////////////////////////////////////
//                0 _____4____  1				
//				   |				 |
//				  7|		 8      | 5
//				   |				 |
//                 |___________| 
//                3      6       2
//////////////////////////////////////////////////////////////////////////
void XTracker::FromHandle(int nResult, double &ratioX, double &ratioY, double &MoveX, double &MoveY, CPoint ptOrg, 
						  CPoint &point,CQuad &quad,CDot &dot,CWnd *pWnd, BOOL bCtrlKey)
{
	if ( !pWnd  )
		return;

	CZoomView *pView = (CZoomView*)pWnd;
	CDot dot2;
	dot2.x = point.x;
	dot2.y = point.y;
	double dx,dy;
	double Lnx = 1, LnY = 1;
	pView->LogicToPhysic(dot2.x,dot2.y);
	if (pView->m_bMeasureLineX)
	{
		dot2.x = pView->m_dbMeasureLineX;
	}
	if (pView->m_bMeasureLineY)
	{
		dot2.y = pView->m_dbMeasureLineY;
	}
	switch(nResult)
	{
	case 0:
		{
			dx = dot2.x-dot.x;
			dy = dot2.y-dot.y;
			
			Lnx = (quad.left-dot.x);
			LnY = (quad.top-dot.y);

			ratioX = dx/Lnx;
			ratioY = dy/LnY;
			
			MoveX = MoveY = 0;			
			break;
		}
	case 1:
		{
			dx = dot2.x-dot.x;
			dy = dot2.y-dot.y;
			
			Lnx = (quad.right-dot.x);
			LnY = (quad.top-dot.y);

			ratioX = dx/Lnx;
			ratioY = dy/LnY;
			
			MoveX = MoveY = 0;			
			break;
		}
	case 2:
		{
			dx = dot2.x-dot.x;
			dy = dot2.y-dot.y;
			
			Lnx = (quad.right-dot.x);
			LnY = (quad.bottom-dot.y);

			ratioX = dx/Lnx;
			ratioY = dy/LnY;

			MoveX = MoveY = 0;
			break;
		}
	case 3:
		{
			dx = dot2.x-dot.x;
			dy = dot2.y-dot.y;
			
			Lnx = (quad.left-dot.x);
			LnY = (quad.bottom-dot.y);

			ratioX = dx/Lnx;
			ratioY = dy/LnY;
			
			MoveX = MoveY = 0;
			break;
		}
	case 4:
		{	
			dx = dot2.x-dot.x;
			dy = dot2.y-dot.y;
			
			Lnx = dot2.x-dot.x;
			LnY = (quad.top-dot.y);

			ratioX = 1;
			ratioY = dy/LnY;

			MoveX = MoveY = 0;
			break;
		}
	case 5:
		{
			ratioX = (dot2.x-dot.x)/(quad.right-dot.x);
			ratioY = 1;

			MoveX = MoveY = 0;
			break;
		}
	case 6:
		{	
			
			ratioY = (dot2.y-dot.y)/(quad.bottom-dot.y);
			ratioX = 1;

			MoveX = MoveY = 0;
			break;
		}
	case 7:
		{
			ratioX = (dot2.x-dot.x)/(quad.left-dot.x);
			ratioY = 1;
			
			MoveX = MoveY = 0;
			break;
		}
	case 8:
		{	
			MoveX = point.x-ptOrg.x;
			MoveY = point.y-ptOrg.y;
			pView->ActualLen(MoveX);
			pView->ActualLen(MoveY);
			MoveY *= -1;
			ratioX = ratioY = 1;
			break;
		}
	case 9:
		{
			break;
		}
	}

	if (bCtrlKey && nResult < 4)
	{
		ratioX<ratioY ? ratioY = ratioX : ratioX = ratioY;
		
		CDot dotTemp;
		dotTemp.x = dot.x + ratioX * Lnx;
		dotTemp.y = dot.y + ratioY * LnY;
		
		pView->PhysicToLogic(dotTemp.x, dotTemp.y);
		
		point.x = (int)(dotTemp.x);
		point.y = (int)(dotTemp.y);
	}
}

//
int XTracker::PtHitQuad(CPoint &pt, CQuad &quad,CWnd *pWnd)
{
	if ( !pWnd  )
		return -1;

	CZoomView *pView = (CZoomView *)pWnd;
	m_quadSave = quad;
	m_HITBORDER = HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();

	CDot dot, dt;
	dot.x = pt.x;
	dot.y = pt.y;
	dt = dot;
	pView->LogicToPhysic(dot.x,dot.y);
	for(int i = 0; i < 8; i++)
	{
		CDot mDot = dt;
		CQuad  mQuad = m_quad[i];
		pView->PhysicToLogic(mQuad.left, mQuad.top);
		pView->PhysicToLogic(mQuad.right, mQuad.bottom);
		switch(i)
		{
		case 0:
			{
				mDot.SetOffset(10, 10);
				break;
			}
		case 1:
			{
				mDot.SetOffset(-10, 10);
				break;
			}
		case 2:
			{
				mDot.SetOffset(-10, -10);
				break;
			}
		case 3:
			{
				mDot.SetOffset(10, -10);
				break;
			}
		case 4:
			{
				mDot.SetOffset(0, 10);
				break;
			}
		case 5:
			{
				mDot.SetOffset(-10, 0);
				break;
			}
		case 6:
			{
				mDot.SetOffset(0, -10);
				break;
			}
		case 7:
			{
				mDot.SetOffset(10, 0);
				break;
			}
		default:
			{
				break;
			}
		}
		
		if (mQuad.DotIn(mDot))
		{
			switch(i)
			{
			case 0:
				{
					mDot.SetOffset(-10, -10);
					break;
				}
			case 1:
				{
					mDot.SetOffset(10, -10);
					break;
				}
			case 2:
				{
					mDot.SetOffset(10, 10);
					break;
				}
			case 3:
				{
					mDot.SetOffset(-10, 10);
					break;
				}
			case 4:
				{
					mDot.SetOffset(0, -10);
					break;
				}
			case 5:
				{
					mDot.SetOffset(10, 0);
					break;
				}
			case 6:
				{
					mDot.SetOffset(0, 10);
					break;
				}
			case 7:
				{
					mDot.SetOffset(-10, 0);
					break;
				}
			default:
				{
					break;
				}
			}

			pt.x = (long)(mDot.x);
			pt.y = (long)(mDot.y);

			return i;
		}
	}

	double dbScale = HITBORDER;
	pView->ActualLen(dbScale);
	CDot dtC = quad.Center();
	CQuad mQD;
	double dblen = 1.5 * dbScale;
	mQD.left   = dtC.x - dblen;
	mQD.top    = dtC.y + dblen;
	mQD.right  = dtC.x + dblen;
	mQD.bottom = dtC.y - dblen;
	if (mQD.DotIn(dot))
	{
		return 8;
	}

	return 9;
}

int XTracker::PtHitQuad(CPoint &pt,CQuadRangle &frame,CWnd *pWnd)
{
	if ( !pWnd )
		return -1;

	CZoomView *pView = (CZoomView *)pWnd;
	CQuadRangle qdSel = frame;

	double dbExtend = 16;
	pView->ActualLen(dbExtend);
	qdSel.Move(-dbExtend, dbExtend);
	qdSel.Inflate(dbExtend);

	CDot d0;
	int i = 0;
	for(i = 0; i < 9; i++)
	{	
		switch(i)
		{
			case 0: d0 = qdSel.GetBaseDot(BASE_TL); break;
			case 1:	d0 = qdSel.GetBaseDot(BASE_TR); break;
			case 2:	d0 = qdSel.GetBaseDot(BASE_BR); break;
			case 3:	d0 = qdSel.GetBaseDot(BASE_BL); break;
			case 4:	d0 = qdSel.GetBaseDot(BASE_TM); break;
			case 5:	d0 = qdSel.GetBaseDot(BASE_MR); break;
			case 6:	d0 = qdSel.GetBaseDot(BASE_BM); break;
			case 7:	d0 = qdSel.GetBaseDot(BASE_ML); break;
			case 8:	d0 = qdSel.GetBaseDot(BASE_MM); break;
		}
		
		pView->PhysicToLogic(d0.x, d0.y);
		d0.Move(16, 16);
		CQuad  mQuad(d0, HITBORDER*2, HITBORDER*2);		
		if (mQuad.DotIn(pt))
		{
			break;
		}
	}

	return i;
}

//
void XTracker::DrawQuadInvert(CQuadRangle quad, XDC *pXDC)
{
	if ( NULL == pXDC )
		return;
	
	int nMod = pXDC->SetROP2(R2_XORPEN);
	
	pXDC->MoveTo(quad.GetBaseDot(BASE_BL).x, quad.GetBaseDot(BASE_BL).y);
	pXDC->LineTo(quad.GetBaseDot(BASE_TL).x, quad.GetBaseDot(BASE_TL).y);
	pXDC->LineTo(quad.GetBaseDot(BASE_TR).x, quad.GetBaseDot(BASE_TR).y);
	pXDC->LineTo(quad.GetBaseDot(BASE_BR).x, quad.GetBaseDot(BASE_BR).y);
	pXDC->LineTo(quad.GetBaseDot(BASE_BL).x, quad.GetBaseDot(BASE_BL).y);
	
	pXDC->SetROP2(nMod);
}


BOOL XTracker::GetDistortion(int nRst,CQuadRangle quad,CDot &dot,double &dbRotateA,double &dbShearXA,double &dbShearYA,CWnd *pWnd)
{
	if ( !( nRst >= 0 && nRst <= 7 ) || NULL == pWnd ) 
		return FALSE;

	CZoomView *pView = (CZoomView*)pWnd; 
	CDC     *pDC   = pView->GetDC();
	XDC     *pXDC  = pView->PrepareDC(pDC);
	MSG msg;
	CDot dot1(0,0);
	CQuadRangle quadTmp = quad;
	double dbAngel0(0.0), dbAngel1(0.0);
	if ( nRst<4 ) 
	{
		dbAngel0 = quad.GetBaseDot(BASE_MM).GetVectorAngle(dot);
	}
	else if (4 == nRst) //顶部横切
	{
		dot = quad.GetShearBaseDot(0);
		
		dbAngel0 = 90;
	}
	else if ( 5 == nRst ) {//右侧竖切
		dot = quad.GetShearBaseDot(1);
				
		dbAngel0 = 0;
	}
	else if (6 == nRst) {//底部横切
		dot = quad.GetShearBaseDot(2);
		
		dbAngel0 = 270;
	}
	else if (7 == nRst) {//左侧竖切
		dot = quad.GetShearBaseDot(3);
		
		dbAngel0 = 180;
	}

	AfxLockTempMaps();
	pWnd->SetCapture();
	if (CWnd::GetCapture()  !=  pWnd) 
	{
		AfxUnlockTempMaps();
		return FALSE;
	}

	CPen pen(PS_DOT, 1, BLUE);
	CPen *pOldPen = pXDC->SelectObject(&pen);

	for (;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if ( WM_LBUTTONUP == msg.message ) 
		{
			break;
		}
		else if (WM_MOUSEMOVE == msg.message) 
		{
			dot1.x = (int)(short)LOWORD(msg.lParam);	
			dot1.y = (int)(short)HIWORD(msg.lParam);
			pView->LogicToPhysic(dot1.x, dot1.y);
			
			DrawQuadInvert(quadTmp, pXDC);
			dbAngel1 = quad.GetBaseDot(BASE_MM).GetVectorAngle(dot1);
			switch(nRst) 
			{
				case 0:
				case 1:
				case 2:
				case 3:
					{
						dbRotateA = dbAngel1-dbAngel0;
						quadTmp = quad;
						quadTmp.Rotate(quadTmp.GetBaseDot(BASE_MM), dbRotateA);
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					{
						quadTmp = quad;
						if ( 4 == nRst||6 == nRst )  {
							dbShearXA = dbAngel0-dbAngel1;
							quadTmp.Shear(dot.x, dot.y, dbShearXA, 0);
						}
						else{
							dbShearYA = dbAngel0-dbAngel1;
							dbShearYA  *=  -1;
							quadTmp.Shear(dot.x, dot.y, 0, dbShearYA);
						}			
					}
					break;
			}
			DrawQuadInvert(quadTmp, pXDC);
		}
	}
	DrawQuadInvert(quadTmp, pXDC);

	pXDC->SelectObject(&pOldPen);

	ReleaseCapture();
	AfxUnlockTempMaps();
	return TRUE;
}

//
BOOL XTracker::GetDragPara(int &nResult,CQuad &quad,CDot &dot, double &ratioX, double &ratioY, double &MoveX, double &MoveY,CWnd *pWnd)
{	
	if ( 9 == nResult || !pWnd ) 
		return FALSE;
	
	AfxLockTempMaps();
	pWnd->SetCapture();
	if ( CWnd::GetCapture() != pWnd )
		return FALSE;
	
	CPoint point = m_ptDown;
	m_OldQuad.Initial();
	m_NewQuad     = quad;
	CQuad quadTemp =  quad;
	ModifyDot(nResult,dot,quad);
	CDot dotTemp = dot;
	
	for(;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{	
				//最后一次修改得到最终结果				
				if (nResult >= 0 && nResult <= 8)
				{	
					if (msg.wParam & MK_CONTROL)
					{
						Handle(nResult,ratioX,ratioY,MoveX,MoveY,dot,point,quad,pWnd,TRUE);
					}
					else
					{
						Handle(nResult,ratioX,ratioY,MoveX,MoveY,dot,point,quad,pWnd,FALSE);
					}		
				}
				
				m_NewQuad.Initial();
				if ( m_OldQuad.IsEmpty() ) 
				{
					ratioX = 1.0;
					ratioY = 1.0;
				}
				else
				{
					DrawQuad(nResult,dotTemp,ratioX,ratioY,MoveX,MoveY,m_OldQuad,m_NewQuad,pWnd);
				}
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{	
				point.x = (int)(short)LOWORD(msg.lParam);
				point.y = (int)(short)HIWORD(msg.lParam);
				CZoomView *pView = (CZoomView*)pWnd;
				pView->AlignMeasureLine(nResult, point, m_ptMeasureLine, quad);
				
				if ( msg.wParam & MK_CONTROL )
				{
					FromHandle(nResult,ratioX,ratioY,MoveX,MoveY,m_ptDown,point,quadTemp,dotTemp,pWnd,TRUE);
				}
				else
				{
					FromHandle(nResult,ratioX,ratioY,MoveX,MoveY,m_ptDown,point,quadTemp,dotTemp,pWnd,FALSE);
				}				
				
				DrawQuad(nResult,dotTemp,ratioX,ratioY,MoveX,MoveY,m_OldQuad,m_NewQuad,pWnd);
				
				if ( nResult == 8 )
					m_OldQuad = m_NewQuad;

				m_ptDown  = point;
				break;
			}
		}
	}
ExitLoop:
	{	
		ReleaseCapture();
		AfxUnlockTempMaps();
		return TRUE;
	}
}

BOOL XTracker::GetDragFrame(int nResult,CQuadRangle &frame,CDot &dot,double &ratioX,double &ratioY,double &MoveX,double &MoveY,CWnd *pWnd)
{
	if ( !( nResult >= 0 && nResult <= 8 ) || NULL == pWnd ) 
		return FALSE;

	CZoomView *pView = (CZoomView*)pWnd;
	CDC     *pDC   = pView->GetDC();
	XDC     *pXDC  = pView->PrepareDC(pDC);
	double dbRA0 = frame.CalRotateAngle();
	double dbSA0 = frame.CalShearAngle();
	CDot d1, d0,dtOrg = frame.GetBaseDot(BASE_MM);
	CQuadRangle f1;

	f1 = frame;
	f1.Rotate(f1.GetBaseDot(BASE_MM), -dbRA0);
	f1.Shear(f1.GetBaseDot(BASE_MM).x, f1.GetBaseDot(BASE_BM).y, -dbSA0, 0);
	ModifyDot(nResult,dot,f1);
	GetDot(nResult, f1, d0);

	f1 = frame;
	ratioX = ratioY = 1.0;

	AfxLockTempMaps();
	pWnd->SetCapture();
	if (CWnd::GetCapture()  !=  pWnd)
	{
		return FALSE;
	}

	CPen pen(PS_DOT, 1, RED);
	CPen *pOldPen = pXDC->SelectObject(&pen);

	CPoint point;
	MSG msg;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{	
				DrawQuadInvert(f1, pXDC);
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{	
				point.x = (int)(short)LOWORD(msg.lParam);
				point.y = (int)(short)HIWORD(msg.lParam);
				d1 = CDot(point.x, point.y);
				pView->LogicToPhysic(d1.x, d1.y);
				if ( 8  !=  nResult )
				{
					d1.Rotate(frame.GetBaseDot(BASE_MM), -dbRA0);
					d1.Shear(frame.GetBaseDot(BASE_MM).x, frame.GetBaseDot(BASE_MM).y, -dbSA0, 0);
					if (5 == nResult || 7 == nResult) {
						ratioX = (d1.x-dot.x)/(d0.x-dot.x);
						ratioY = 1;
					}else if (4 == nResult || 6 == nResult) {
						ratioY = (d1.y-dot.y)/(d0.y-dot.y);
						ratioX = 1;
					}else {
						ratioX = (d1.x-dot.x)/(d0.x-dot.x);
						ratioY = (d1.y-dot.y)/(d0.y-dot.y);
						(ratioX>ratioY) ? (ratioX = ratioY) : (ratioY = ratioX);
					}	
					
					DrawQuadInvert(f1, pXDC);
					f1 = frame;
					f1.Rotate(frame.GetBaseDot(BASE_MM), -dbRA0);
					f1.Shear(f1.GetBaseDot(BASE_MM).x, f1.GetBaseDot(BASE_MM).y, -dbSA0, 0);
					f1.Scale(dot, ratioX, ratioY);				
					f1.Shear(f1.GetBaseDot(BASE_MM).x, f1.GetBaseDot(BASE_MM).y, dbSA0, 0);	
					f1.Rotate(frame.GetBaseDot(BASE_MM), dbRA0);
					if (dbSA0 > 1e-3) 
					{
						CDot d2;
						if (0 == nResult || 1 == nResult || 2 == nResult || 3 == nResult) {
							ModifyDot(nResult, d1, f1);
							ModifyDot(nResult, d2, frame);
						}else if (4 == nResult) {
							GetDot(2, f1, d1);
							GetDot(2, frame, d2);
						}else if (6 == nResult) {
							GetDot(0, f1, d1);
							GetDot(0, frame, d2);
						}else {
							d1 = d2;
						}
						f1.Move(d2.x-d1.x, d2.y-d1.y);
					}
				}				
				else{
					ratioX = ratioY = 1;
					MoveX = d1.x - dtOrg.x;
					MoveY = d1.y - dtOrg.y;

					DrawQuadInvert(f1, pXDC);
					f1 = frame;
					f1.Move(MoveX, MoveY);
				}
				DrawQuadInvert(f1, pXDC);
				break;
			}
		}
	}
ExitLoop:
	{	
		pXDC->SelectObject(&pOldPen);

		ReleaseCapture();
		AfxUnlockTempMaps();
		return TRUE;
	}
}

//
void XTracker::DrawQuad(int nResult,CDot &dot, double &ratioX, double &ratioY, double &MoveX, double &MoveY,CQuad &quad1,CQuad &quad2,CWnd *pWnd)
{
	if ( !pWnd  )
		return;

	//dot 为不动点,画虚线外包框
	CZoomView *pView = (CZoomView*)pWnd;
	CDC   *pDC   = pView->GetDC();
	ASSERT_VALID(pDC);

	XDC    *pXDC  = pView->PrepareDC(pDC);
	CPen  penBlack(PS_DOT,1,RGB(0,0,0));
	CPen *pOldPen   = pDC->SelectObject(&penBlack);
	CQuad oldQuad  = quad1;
	CQuad qdSel     = quad2;
	
	CDot Dot;
	double tx(0),ty(0);
	double dx(0),dy(0);
	dx = dot.x;
	dy = dot.y;
	
	// 反色掉
	int mod = pDC->SetROP2(R2_NOTXORPEN);
	//draw qdSel
	{	
		pXDC->MoveTo(oldQuad.left,oldQuad.top);
		pXDC->LineTo(oldQuad.right,oldQuad.top);
		pXDC->LineTo(oldQuad.right,oldQuad.bottom);
		pXDC->LineTo(oldQuad.left,oldQuad.bottom);
		pXDC->LineTo(oldQuad.left,oldQuad.top);
		
        //比例缩放加平移
		Dot.x = quad2.left;
		Dot.y = quad2.top;
		tx    = Dot.x-dx;
		Dot.x = ratioX*tx+dx;
		ty    = Dot.y-dy;
		Dot.y = ratioY*ty+dy;
		
		qdSel.left = Dot.x;
		qdSel.top  = Dot.y;
		qdSel.left +=  MoveX;
		qdSel.top  +=  MoveY;

		Dot.x = quad2.right;
		Dot.y = quad2.top;
		tx    = Dot.x-dx;
		Dot.x = ratioX*tx+dx;
		ty    = Dot.y-dy;
		Dot.y = ratioY*ty+dy;

		qdSel.right = Dot.x;
		qdSel.top   = Dot.y;
		qdSel.right +=  MoveX;
		qdSel.top   +=  MoveY;

		Dot.x = quad2.right;
		Dot.y = quad2.bottom;
		tx    = Dot.x - dx;
		Dot.x = ratioX * tx + dx;
		ty    = Dot.y - dy;
		Dot.y = ratioY * ty + dy;

		qdSel.right   = Dot.x;
		qdSel.bottom  = Dot.y;
		qdSel.right   +=  MoveX;
		qdSel.bottom  +=  MoveY;

		Dot.x = quad2.left;
		Dot.y = quad2.bottom;
		tx    = Dot.x - dx;
		Dot.x = ratioX * tx + dx;
		ty    = Dot.y - dy;
		Dot.y = ratioY * ty+dy;

		qdSel.left   = Dot.x;
		qdSel.bottom = Dot.y;
		qdSel.left    +=  MoveX;
		qdSel.bottom  +=  MoveY;

        //重绘		
		pXDC->MoveTo(qdSel.left,qdSel.top);
		pXDC->LineTo(qdSel.right,qdSel.top);
		pXDC->LineTo(qdSel.right,qdSel.bottom);
		pXDC->LineTo(qdSel.left,qdSel.bottom);
		pXDC->LineTo(qdSel.left,qdSel.top);

		if (nResult == 8)
		{
			quad2 = qdSel;
		}
		else
		{
			quad1 = qdSel;
		}
	}

	pDC->SetROP2(mod);
	pDC->SelectObject(pOldPen);
	penBlack.DeleteObject();
	pView->ReleaseDC(pDC);

	return;
}



void XTracker::ModifyDot(int nResult,CDot &dot,CQuad &quad)
{
	switch(nResult)
	{
	case 0:
		{
			dot.x = quad.right;
			dot.y = quad.bottom;
			break;
		}
	case 1:
		{
			dot.x = quad.left;
			dot.y = quad.bottom;
			break;
		}
	case 2:
		{
			dot.x = quad.left;
			dot.y = quad.top;
			break;
		}
	case 3:
		{
			dot.x = quad.right;
			dot.y = quad.top;
			break;
		}
	case 4:
		{
			dot.x = (quad.right+quad.left)/2;
			dot.y = quad.bottom;
			break;
		}
	case 5:
		{
			dot.x = quad.left;
			dot.y = (quad.top+quad.bottom)/2;
			break;
		}
	case 6:
		{	
			dot.x = (quad.left+quad.right)/2;
			dot.y = quad.top;
			break;
		}
	case 7:
		{
			dot.x = quad.right;
			dot.y = (quad.top+quad.bottom)/2;
			break;
		}
	case 8:
		{	
			dot.x = m_ptDown.x;
			dot.y = m_ptDown.y;
			m_dtQuadCenter.x = (quad.left+quad.right)/2;
			m_dtQuadCenter.y = (quad.top+quad.bottom)/2;
			break;
		}
	case 9:
		{
			break;
		}
	}
}


void XTracker::ModifyDot(int nResult,CDot &dot,CQuadRangle& frame)
{
	switch(nResult)
	{
		case 0:	dot = frame.GetBaseDot(BASE_BR);break;
		case 1:	dot = frame.GetBaseDot(BASE_BL);break;
		case 2:	dot = frame.GetBaseDot(BASE_TL);break;
		case 3:	dot = frame.GetBaseDot(BASE_TR);break;
		case 4:	dot = frame.GetBaseDot(BASE_BM);break;
		case 5:	dot = frame.GetBaseDot(BASE_ML);break;
		case 6:	dot = frame.GetBaseDot(BASE_TM);break;
		case 7:	dot = frame.GetBaseDot(BASE_MR);break;
		case 8:	dot = frame.GetBaseDot(BASE_MM);break;
		case 9:	break;
	}
}

void XTracker::GetDot(int nResult,CQuadRangle &frame,CDot &dot)
{
	switch(nResult)
	{
		case 0:	dot = frame.GetBaseDot(BASE_TL);break;
		case 1:	dot = frame.GetBaseDot(BASE_TR);break;
		case 2:	dot = frame.GetBaseDot(BASE_BR);break;
		case 3:	dot = frame.GetBaseDot(BASE_BL);break;
		case 4:	dot = frame.GetBaseDot(BASE_TM);break;
		case 5:	dot = frame.GetBaseDot(BASE_MR);break;
		case 6:	dot = frame.GetBaseDot(BASE_BM);break;
		case 7:	dot = frame.GetBaseDot(BASE_ML);break;
		case 8:	dot = frame.GetBaseDot(BASE_MM);break;
		case 9:	break;
	}
}

//
void XTracker::Handle(int nResult, double &ratioX, double &ratioY, double &MoveX, double &MoveY, CPoint dot, 
					  CPoint &point,CQuad &quad, CWnd *pWnd, BOOL bCtrlKey)
{
	if ( !pWnd )
		return;

	CZoomView *pView = (CZoomView*)pWnd;
	CDot dot2;
	dot2.x = point.x;
	dot2.y = point.y;
	double dx,dy;
	pView->LogicToPhysic(dot2.x,dot2.y);
	if (pView->m_bMeasureLineX)
	{
		dot2.x = pView->m_dbMeasureLineX;
	}
	if (pView->m_bMeasureLineY)
	{
		dot2.y = pView->m_dbMeasureLineY;
	}
	
	CDot dotTemp;
	dotTemp = dot2;
	switch(nResult)
	{	
	case 0:
		{	
			dx = dotTemp.x-dot.x;
			dy = dotTemp.y-dot.y;
			ratioX = dx/(quad.left-dot.x);
			ratioY = dy/(quad.top-dot.y);
			
			if (bCtrlKey == TRUE)
			{
				ratioX<ratioY ? ratioY = ratioX : ratioX = ratioY;
				dotTemp.x = (quad.left - dot.x)*ratioX + dot.x;
				dotTemp.y = (quad.top - dot.y)*ratioY + dot.y;
			}			

			MoveX = dotTemp.x-quad.left;
			MoveY = dotTemp.y-quad.top;
			MoveX *= 0.5;
			MoveY *= 0.5;
			break;
		}
	case 1:
		{	
			dx = dotTemp.x-dot.x;
			dy = dotTemp.y-dot.y;
			
			ratioX = dx/(quad.right-dot.x);
			ratioY = dy/(quad.top-dot.y);
			
			if (bCtrlKey == TRUE)
			{
				ratioX<ratioY ? ratioY = ratioX : ratioX = ratioY;
				dotTemp.x = (quad.right - dot.x)*ratioX + dot.x;
				dotTemp.y = (quad.top - dot.y)*ratioY + dot.y;
			}	
			
			MoveX = dotTemp.x-quad.right;
			MoveY = dotTemp.y-quad.top;
			MoveX *= 0.5;
			MoveY *= 0.5;
			break;
		}
	case 2:
		{	
			dx = dotTemp.x-dot.x;
			dy = dotTemp.y-dot.y;
			
			ratioX = dx/(quad.right-dot.x);
			ratioY = dy/(quad.bottom-dot.y);

			if (bCtrlKey == TRUE)
			{
				ratioX<ratioY ? ratioY = ratioX : ratioX = ratioY;
				dotTemp.x = (quad.right - dot.x)*ratioX + dot.x;
				dotTemp.y = (quad.bottom - dot.y)*ratioY + dot.y;
			}	
			
			MoveX = dotTemp.x-quad.right;
			MoveY = dotTemp.y-quad.bottom;
			MoveX *= 0.5;
			MoveY *= 0.5;			
			break;
		}
	case 3:
		{	
			dx = dotTemp.x-dot.x;
			dy = dotTemp.y-dot.y;
			
			ratioX = dx/(quad.left-dot.x);
			ratioY = dy/(quad.bottom-dot.y);
			
			if (bCtrlKey == TRUE)
			{
				ratioX<ratioY ? ratioY = ratioX : ratioX = ratioY;
				dotTemp.x = (quad.left - dot.x)*ratioX + dot.x;
				dotTemp.y = (quad.bottom - dot.y)*ratioY + dot.y;
			}	

			MoveX = dotTemp.x-quad.bottom;
			MoveY = dotTemp.y-quad.left;
			MoveX *= 0.5;
			MoveY *= 0.5;
			break;
		}
	case 4:
		{			
			ratioY = (dot2.y-dot.y)/(quad.top-dot.y);
			ratioX = 1;
			
			MoveY = dot2.y-quad.top;
			MoveX = 0;
			MoveY *= 0.5;
			break;
		}
	case 5:
		{	
			ratioX = (dot2.x-dot.x)/(quad.right-dot.x);
			ratioY = 1;
			
			MoveX = dot2.x-quad.right;
			MoveY = 0;
			MoveX *= 0.5;
			break;
		}
	case 6:
		{				
			ratioY = (dot2.y-dot.y)/(quad.bottom-dot.y);
			ratioX = 1;
			
			MoveY = dot2.y-quad.bottom;
			MoveX = 0;
			MoveY *= 0.5;
			break;
		}
	case 7:
		{
			ratioX = (dot2.x-dot.x)/(quad.left-dot.x);
			ratioY = 1;
			
			MoveX = dot2.x-quad.left;
			MoveY = 0;
			MoveX *= 0.5;
			break;
		}
	case 8:
		{	
			MoveX = point.x-dot.x;
			MoveY = point.y-dot.y;
			pView->ActualLen(MoveX);
			pView->ActualLen(MoveY);
			MoveY *= -1;
			if (pView->m_bMeasureLineX)
			{
				MoveX = dot2.x - m_dtQuadCenter.x;
			}
			if (pView->m_bMeasureLineY)
			{
				MoveY = dot2.y - m_dtQuadCenter.y;
			}
			ratioX = ratioY = 1;
			break;
		}
	case 9:
		{
			break;
		}
	}
}

//
int XTracker::GetResult(CPoint &point, CQuad &quad,CWnd *pWnd)
{
	int nResult = PtHitQuad(point,quad,pWnd);

	m_ptDown.x = point.x;
	m_ptDown.y = point.y;

	return nResult;
}



int XTracker::GetResult(CPoint &point ,CQuadRangle &quad,CWnd *pWnd)
{
	int nResult =PtHitQuad(point,quad,pWnd);

	m_ptDown.x = point.x;
	m_ptDown.y = point.y;

	return nResult;
}



//
void XTracker::DrawSelTraker(XDC *pDC, CQuad &qdSel,CWnd *pWnd, CArray<SELQUAD, SELQUAD> &QuadList,BOOL bLock)
{
	DrawTracker(pDC,qdSel,pWnd,bLock);
	
	int nCount = QuadList.GetSize();
	if ( nCount  <=  1 )
		return;
	
	BOOL bFill  = FALSE;
	int  nIndex = -1;
	
	for(int i  = 0; i < nCount; i++) 
	{
		if (!QuadList[i].m_bSel)
		{
			continue;
		}
		
		if (bFill)
		{
			QuadList[i].m_bFill = FALSE;
			CGroupObj *pGroupObj = ((CGroupObj *)QuadList[i].m_pGroupObj);
			pGroupObj->SetMainSelect(FALSE);
		}
		
		if (QuadList[i].m_bFill) 
		{
			DrawSelObjTracker(pDC, QuadList[i].m_QD, TRUE, pWnd);
			bFill = TRUE;
		}
		else
		{
			DrawSelObjTracker(pDC, QuadList[i].m_QD, FALSE, pWnd);
		}
		
		nIndex = i;
	}
	
	if ((!bFill) && (nIndex  >=  0))
	{
		QuadList[nIndex].m_bFill = TRUE;
		CGroupObj *pGroupObj = ((CGroupObj *)QuadList[nIndex].m_pGroupObj);
		pGroupObj->SetMainSelect(TRUE);
		DrawSelObjTracker(pDC, QuadList[nIndex].m_QD, TRUE, pWnd);
	}
}



void XTracker::DrawSelObjTracker(XDC *pDC, CQuad &qdSel, BOOL bFillQuad, CWnd *pWnd)
{
	if ( !pDC || !pWnd )
		return;

	CPen penBlue(PS_SOLID, 0, BLUE);
	CPen *pOldPen = pDC->SelectObject(&penBlue);
	double x,y;
	x = y = 1.5;

	CZoomView *pView = (CZoomView*)pWnd;
	CQuad quad = qdSel;
	pView->ActualLen(x);
	pView->ActualLen(y);
	quad.Inflate(x,y);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);

	//draw handle
	m_quadSave = quad;
	m_HITBORDER = HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	for(int i = 0;i<8;i++)
	{
		if ((7 == i) || (6 == i) || (5 == i) || (8 == i) || (4 == i))
		{
			continue;
		}
		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		if (bFillQuad)
		{
			pDC->FillSolidQuad(m_quad[i],BLUE);
		}
	}
	pDC->SelectObject(pOldPen);
	penBlue.DeleteObject();

	return;
}





int XTracker::GetDistortionPos(CPoint point ,CQuad &quad,CWnd *pWnd)
{
	if (NULL == pWnd) {
		return -1;
	}

	m_quadSave = quad;
	UpdateRectCtrl();
	CZoomView *pView = (CZoomView*)pWnd;
	for(int i = 0; i<8; i++)
	{
		CQuad  quad = m_quad[i];
		CDot dot  = quad.GetBaseDot(BASE_MM);
		pView->PhysicToLogic(dot.x, dot.y);
		switch(i)
		{
			case 0:
				dot.SetOffset(-16, -16);	
				break;
			case 1:
				dot.SetOffset(  0, -16);
				break;
			case 2:
				dot.SetOffset(  0,   0);
				break;
			case 3:
				dot.SetOffset(-16,   0);
				break;
			case 4:
				dot.SetOffset( -8, -16);
				break;
			case 5:
				dot.SetOffset(  0,  -8);
				break;
			case 6:
				dot.SetOffset( -8,   0);
				break;
			case 7:
				dot.SetOffset(-16,  -8);
				break;
		}

		quad = CQuad(BASE_TL, dot, 16, -16);
		if ( quad.DotIn(point) ) {
			return i;
		}
	}

	return -1;
}

void XTracker::DrawDistortion (XDC *pDC, CQuad quad, CWnd *pWnd)
{
	if (NULL == pDC || NULL == pWnd) {
		return;
	}

	CPen penBlue(PS_DOT,0,BLUE);
	CPen *pOldPen = pDC->SelectObject(&penBlue);

	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);

	pDC->SelectObject(pOldPen);
	penBlue.DeleteObject();
	
	m_quadSave = quad;
	UpdateRectCtrl();

	CZoomView *pView = (CZoomView*)pWnd;
	CDC *pMDC = pDC->m_pDC;
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC->m_pDC);
	dcMem.SetBkMode(TRANSPARENT);

	CDot dot = quad.GetBaseDot(BASE_MM);
	pView->PhysicToLogic(dot.x, dot.y);
	dcMem.SelectObject(g_hBmpDtt[6]);
	pMDC->BitBlt((int)(dot.x-8),(int)(dot.y-8),16,16,&dcMem,0,0,SRCCOPY);
	
	for(int i = 0; i<8; i++)
	{
		quad = m_quad[i];
		dot  = quad.GetBaseDot(BASE_MM);
		pView->PhysicToLogic(dot.x, dot.y);
		switch(i)
		{
			case 0:
				dot.SetOffset(-16, -16);
				dcMem.SelectObject(g_hBmpDtt[i]);	
				break;
			case 1:
				dot.SetOffset(  0, -16);
				dcMem.SelectObject(g_hBmpDtt[i]);	
				break;
			case 2:
				dot.SetOffset(  0,   0);
				dcMem.SelectObject(g_hBmpDtt[i]);
				break;
			case 3:
				dot.SetOffset(-16,   0);
				dcMem.SelectObject(g_hBmpDtt[i]);				
				break;
			case 4:
				dot.SetOffset( -8, -16);
				dcMem.SelectObject(g_hBmpDtt[4]);	
				break;
			case 5:
				dot.SetOffset(  0,  -8);
				dcMem.SelectObject(g_hBmpDtt[5]);
				break;
			case 6:
				dot.SetOffset( -8,   0);
				dcMem.SelectObject(g_hBmpDtt[4]);	
				break;
			case 7:
				dot.SetOffset(-16,  -8);
				dcMem.SelectObject(g_hBmpDtt[5]);
				break;
		}
		pMDC->BitBlt((int)(dot.x),(int)(dot.y),16,16,&dcMem,0,0,SRCCOPY);
	}

}

void XTracker::DrawTracker(XDC *pDC, CQuad &qdSel,CWnd *pWnd,BOOL bLock)
{	
	if ( qdSel.Width() < 0.0000015 && qdSel.Height() < 0.0000015)
		return;

	//draw outline
	CPen penBlue(PS_DOT,0,BLUE);
	CPen *pOldPen = pDC->SelectObject(&penBlue);
	double x,y;
	x = y = 1.5;
	CZoomView *pView = (CZoomView*)pWnd;
	CQuad quad = qdSel;
	pView->ActualLen(x);
	pView->ActualLen(y);
	quad.Inflate(x,y);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave = quad;
	m_HITBORDER = HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();

	CDC *pMDC = pDC->m_pDC;
	for(int i = 0; i<8; i++)
	{
		CQuad  mQuad = m_quad[i];
		switch(i)
		{
		case 0:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, bLock ? g_hIcoList[5] : g_hIcoList[3]);
				break;
			}
		case 1:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16,  bLock ? g_hIcoList[5] : g_hIcoList[2]);
				break;
			}
		case 2:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16,  bLock ? g_hIcoList[5] : g_hIcoList[3]);
				break;
			}
		case 3:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16,  bLock ? g_hIcoList[5] : g_hIcoList[2]);
				break;
			}
		case 4:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(0, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16,  bLock ? g_hIcoList[5] : g_hIcoList[1]);
				break;
			}
		case 5:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, 0);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16,  bLock ? g_hIcoList[5] : g_hIcoList[4]);
				break;
			}
		case 6:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(0, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16,  bLock ? g_hIcoList[5] : g_hIcoList[1]);
				break;
			}
		case 7:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, 0);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16,  bLock ? g_hIcoList[5] : g_hIcoList[4]);
				break;
			}
		default:
			{
				break;
			}
		}
	}
	
	CDot dot  = qdSel.Center();
	pView->PhysicToLogic(dot.x, dot.y);
	pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[0]);
	
	pDC->SelectObject(pOldPen);
	penBlue.DeleteObject();
}






void XTracker::DrawFrame(XDC *pDC,CQuadRangle &qdSel,CWnd *pWnd,BOOL bLock)
{
	if ( qdSel.GetWidth() < 0.0000015 && qdSel.GetHeight() < 0.0000015)
		return;
	
	//draw outline
	CPen redPen(PS_DOT,0,RED);
	CPen *pOldPen = pDC->SelectObject(&redPen);
	double dbExtend = 1.5;
	CZoomView *pView = (CZoomView*)pWnd;
	CQuadRangle quad = qdSel;
	pView->ActualLen(dbExtend);
	quad.Inflate(dbExtend);
	pDC->MoveTo(quad.GetBaseDot(BASE_TL).x, quad.GetBaseDot(BASE_TL).y);
	pDC->LineTo(quad.GetBaseDot(BASE_TR).x, quad.GetBaseDot(BASE_TR).y);
	pDC->LineTo(quad.GetBaseDot(BASE_BR).x, quad.GetBaseDot(BASE_BR).y);
	pDC->LineTo(quad.GetBaseDot(BASE_BL).x, quad.GetBaseDot(BASE_BL).y);
	pDC->LineTo(quad.GetBaseDot(BASE_TL).x, quad.GetBaseDot(BASE_TL).y);
	dbExtend = 16;
	pView->ActualLen(dbExtend);
	quad = qdSel;
	quad.Move(-dbExtend, dbExtend);
	quad.Inflate(dbExtend);
		
	CDC *pMDC = pDC->m_pDC;	
	HICON pHicon = NULL;
	CDot dot;
	for(int i = 0; i<8; i++)
	{
		switch(i)
		{
			case 0:	dot  = quad.GetBaseDot(BASE_TL);pHicon = g_hIcoList[3];break;
			case 1: dot  = quad.GetBaseDot(BASE_TR);pHicon = g_hIcoList[2];break;
			case 2:	dot  = quad.GetBaseDot(BASE_BR);pHicon = g_hIcoList[3];break;
			case 3: dot  = quad.GetBaseDot(BASE_BL);pHicon = g_hIcoList[2];break;
			case 4: dot  = quad.GetBaseDot(BASE_TM);pHicon = g_hIcoList[1];break;
			case 5: dot  = quad.GetBaseDot(BASE_MR);pHicon = g_hIcoList[4];break;
			case 6: dot  = quad.GetBaseDot(BASE_BM);pHicon = g_hIcoList[1];break;
			case 7: dot  = quad.GetBaseDot(BASE_ML);pHicon = g_hIcoList[4];break;
		}
		pView->PhysicToLogic(dot.x, dot.y);
		pMDC->DrawIcon((int)(dot.x), (int)(dot.y), bLock ? g_hIcoList[5] : pHicon);
	}

	dot  = quad.GetBaseDot(BASE_MM);
	pView->PhysicToLogic(dot.x, dot.y);
	pMDC->DrawIcon((int)(dot.x), (int)(dot.y), g_hIcoList[0]);

	pDC->SelectObject(pOldPen);
	redPen.DeleteObject();
}

void XTracker::Draw(CQuad &qdSel, CWnd *pWnd)
{
	CPen penBlue(PS_DOT,0,BLUE);
	double x,y;
	x = y = 1;
	CZoomView *pView = (CZoomView*)pWnd;
	pView->ActualLen(x);
	pView->ActualLen(y);
	CQuad quad = qdSel;
	quad.Inflate(x,y);
	CDC *pdc = pView->GetDC();
	XDC *pDC = pView->PrepareDC(pdc);
	CPen *pOldPen = pDC->SelectObject(&penBlue);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave = quad;
	m_HITBORDER = HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	for(int i = 0;i<8;i++)
	{
		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		pDC->FillSolidQuad(m_quad[i],BLUE);
	}
	pDC->SelectObject(pOldPen);
	penBlue.DeleteObject();
	pView->ReleaseDC(pdc);
}

void XTracker::InverDraw(CQuad &qdSel, CWnd *pWnd)
{
	if ( !pWnd )
		return;

	CPen penBlue(PS_DOT,0,BLUE);
	double x,y;
	x = y = 1;
	CZoomView *pView = (CZoomView*)pWnd;
	pView->ActualLen(x);
	pView->ActualLen(y);
	CQuad quad = qdSel;
	quad.Inflate(x,y);
	CDC *pdc = pView->GetDC();
	XDC *pDC = pView->PrepareDC(pdc);
	CPen *pOldPen = pDC->SelectObject(&penBlue);
	int mod = pDC->SetROP2(R2_NOTXORPEN);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave = quad;
	m_HITBORDER = HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	for(int i = 0;i<8;i++)
	{
		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		pDC->FillSolidQuad(m_quad[i],BLUE);
	}
	pDC->SetROP2(mod);
	pDC->SelectObject(pOldPen);
	penBlue.DeleteObject();
	pView->ReleaseDC(pdc);
}

void XTracker::GetTrueRect(CRect &rect)
{
	rect = m_rect;
	rect.InflateRect(9,9);
}

void XTracker::SetPhysicQuad(CWnd *pWnd, CQuad &quad)
{
	if ( !pWnd  )
		return;

	CZoomView *pView = (CZoomView*)pWnd;
	CQuad quadTemp = quad;
	pView->PhysicToLogic(quadTemp);
	m_rect = (CRect)quadTemp;
}

void XTracker::SingleModify(CPoint &point,CWnd *pWnd)
{
	if ( !pWnd  )  return;

	AfxLockTempMaps();
	pWnd->SetCapture();
	if ( CWnd::GetCapture()  !=  pWnd )
		return;

	
	CPoint point1 = point;
	CPoint point2;
	while( TRUE )
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				point2.x = (int)(short)LOWORD(msg.lParam);
				point2.y = (int)(short)HIWORD(msg.lParam);
				DrawNode(point1,point2,pWnd);
				point1 = point2;
			}
		default:
			{
				break;
			}
		}
	}
ExitLoop:
	{ 
		ReleaseCapture();
		AfxUnlockTempMaps(FALSE);
	}
}

void XTracker::DrawNode(CPoint &pt1,CPoint &pt2, CWnd *pWnd)//pt1为旧坐标，pt2为新坐标
{
	if ( m_pDotHead == NULL || pWnd == NULL )
		return;
	
	DotHead *phd = m_pDotHead;
	CZoomView *pView = (CZoomView*)pWnd;
	CDot dot1,dot2;
	CDC *pDC = pView->GetDC();
	CPen penGreen(PS_DOT,1,GREEN);
	CPen *pOldPen = pDC->SelectObject(&penGreen);
	int mode = pDC->SetROP2(R2_NOTXORPEN);
	XDC *pXDC = pView->PrepareDC(pDC);
	DotSelectList *pList = NULL;
	CDot dt;		
	while(phd)
	{
		dot1.x = pt1.x;
		dot1.y = pt1.y;
		dot2.x = pt2.x;
		dot2.y = pt2.y;
		pView->LogicToPhysic(dot1.x,dot1.y);
		pView->LogicToPhysic(dot2.x,dot2.y);
		pList = phd->m_pHead;
		
		while(pList)
		{
			//先画右边
			dt = pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if (pList->bRight && !pList->bRightDraw)
				pXDC->LineTo(pList->m_pRight->x,pList->m_pRight->y);
			
			pList = pList->m_pNext;
		}
		pList = phd->m_pTail;
		while(pList)
		{
			//再画左边
			dt = pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if (!pList->bLeftDraw)
				pXDC->LineTo(pList->m_pLeft->x,pList->m_pLeft->y);
			pList = pList->m_pFront;
		}
		//数据转换
		pList = phd->m_pHead;
		while(pList)
		{
			dt = pList->m_Node;
			dt.x += dot2.x-dot1.x;
			dt.y += dot2.y-dot1.y;
			pList->m_Node = dt;
			pList = pList->m_pNext;
		}
		//重绘
		pList = phd->m_pHead;
		while(pList)
		{
			//先画右边
			dt = pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if (pList->bRight && !pList->bRightDraw)
				pXDC->LineTo(pList->m_pRight->x,pList->m_pRight->y);
			pList = pList->m_pNext;
		}
		pList = phd->m_pTail;
		while(pList)
		{
			//再画左边
			dt = pList->m_Node;
			pXDC->MoveTo(dt.x,dt.y);
			if (!pList->bLeftDraw)
				pXDC->LineTo(pList->m_pLeft->x,pList->m_pLeft->y);
			pList = pList->m_pFront;
		}
			phd = phd->m_pNext;
	}
	pDC->SetROP2(mode);
	pDC->SelectObject(pOldPen);
	penGreen.DeleteObject();
	pView->ReleaseDC(pDC);
}


void XTracker::ClearStrokeList()
{
	m_StrokeList.RemoveAll();
}

BOOL XTracker::HaveObject()
{
	if ( m_StrokeList.GetCount() > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL XTracker::HitSelectDot(CPoint &point, CWnd *pWnd)
{
	if ( !pWnd )
		return FALSE;

	CDot dotTest;
	CZoomView *pView = (CZoomView*)pWnd;
	dotTest.x = point.x;
	dotTest.y = point.y;
	pView->LogicToPhysic(dotTest.x,dotTest.y);
	double dbData = 3;
	pView->ActualLen(dbData);
	BOOL bRet =  HitSelectDotB(dotTest, dbData);
	return bRet;
}


BOOL XTracker::HitOtherDot(CPoint &point, CWnd *pWnd)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->IsOnDot(point,pWnd) && !HitSelectDot(point,pWnd))
			return TRUE;
	}
	return FALSE;
}

void XTracker::GetDot(CPoint &point,CWnd *pWnd)
{
	if ( !pWnd )
		return;

	CDot dot;
	dot.x = point.x;
	dot.y = point.y;

	CZoomView *pView = (CZoomView*)pWnd;
	pView->LogicToPhysic(dot.x,dot.y);

	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->m_bTag)
		{
			AddDotHead(pStroke,dot,pWnd);
		}
	}
}

void XTracker::GetDot(CQuad &quad)
{
	CQuadB quadb= quad;
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->m_bTag)
		{
			AddDotHeadB(pStroke,quadb);
		}
	}
}

//
void XTracker::SearchDot(CPoint &point,CWnd *pWnd)
{
	if ( !pWnd )
		return;

	CStroke *pStroke = NULL;
	POSITION pos = m_StrokeList.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke )
			pStroke->SetFlag(FALSE);
	}
	
	
	CDot dot;
	dot.x = point.x;
	dot.y = point.y;
	
	CZoomView *pView = (CZoomView*)pWnd;
	pView->LogicToPhysic(dot.x,dot.y);
	pos = m_StrokeList.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if (pStroke && pStroke->DotRight(dot,pWnd))
		{
			pStroke->SetFlag(TRUE);
			break;
		}
	}
}

//
void XTracker::SearchDot(CQuad &quad)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_StrokeList.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke )
			pStroke->SetFlag(FALSE);
	}

	pos = m_StrokeList.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->DotInQuad(quad) )
		{
			pStroke->SetFlag(TRUE);
		}
	}
}

//
void XTracker::AddDotHead(CStroke *pStroke,CDot &dotIndex,CWnd *pWnd)
{
	if ( !pWnd || !pStroke )
		return;

	CZoomView *pView = (CZoomView*)pWnd;
	double dbData = 3;
	pView->ActualLen(dbData);
	AddDotToHead(pStroke, dotIndex, dbData);
}

//
DWORD XTracker::FillRect(CWnd *pWnd)
{
	if ( !m_pDotHead || !pWnd )
		return 0;
	
	CZoomView *pView = (CZoomView*)pWnd;
	CDC  *pDC   = pView->GetDC();
	ASSERT_VALID(pDC);
	
	CRect rect;
	CQuad quad;
	CDot  dot;
	DotSelectList *pList = NULL;
	double dbData = 2;
	DWORD  dwDumpPt = 0;

	pView->ActualLen(dbData);
	CDot dt;
	dt.x = -9999999999999;
	dt.y = -9999999999999;
	DotHead *pHead = m_pDotHead;
	BOOL bIsFirstDot = FALSE;
	while(pHead)
	{
		pList = pHead->m_pHead;
		while( pList )
		{
			dot  = pList->m_Node;
			
			double dbDs = Distance(dt, dot);
			if (dbDs < g_SysCfg.m_dbDotFilterPrecis)
			{
				dwDumpPt = 1;
			}
			
			dt = dot;
			quad.left   = dot.x-dbData;
			quad.right  = dot.x+1.75*dbData;
			quad.top    = dot.y+dbData;
			quad.bottom = dot.y-1.75*dbData;
			
			pView->PhysicToLogic(quad);
			rect = (CRect)quad;
			if ( bIsFirstDot )
			{	
				bIsFirstDot = FALSE;
				pDC->FillSolidRect(&rect,RED);
			}
			else
			{
				pDC->FillSolidRect(&rect,BLACK);
			}
			pList = pList->m_pNext;
		}
		pHead = pHead->m_pNext;
	}
	
	pView->ReleaseDC(pDC);
	
	return dwDumpPt;
}


void XTracker::ClearAndStore(CPoint &point, CWnd *pWnd)
{	
	InverDrawDot(pWnd);
	SearchDot(point,pWnd);
	ClearDot();
	GetDot(point,pWnd);
	FillRect(pWnd);
	DataEXChange();
}

BOOL XTracker::HasDot(CQuad &quad)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->HasDot(quad))
			return TRUE;
	}
	return FALSE;
}

void XTracker::ClearAndStore(CQuad &quad, CWnd *pWnd)
{
	InverDrawDot(pWnd);
	SearchDot(quad);
	ClearDot();
	GetDot(quad);
	FillRect(pWnd);
	DataEXChange();
}

void XTracker::DelRect(CWnd *pWnd)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke=NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke )
			pStroke->DelRect(pWnd);
	}
}

BOOL XTracker::IsOnDot(CPoint &point, CWnd *pWnd)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->IsOnDot(point,pWnd))
			return TRUE;
	}
	return FALSE;
}

void XTracker::Edit(CPoint &point, CWnd *pWnd)
{
	SearchDot(point,pWnd);
	GetDot(point,pWnd);
	FillRect(pWnd);
	DataEXChange();
}

void XTracker::SetFlag(BOOL bflag)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke )
			pStroke->SetFlag(bflag);
	}
}

void XTracker::Edit(CQuad &quad, CWnd *pWnd)
{
	SearchDot(quad);
	GetDot(quad);

	DataEXChange();
}

//
void XTracker::AddRect(CWnd *pWnd)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke )
			pStroke->AddRect(pWnd);
	}
}


//
void XTracker::AddPoint(CPoint &point,CWnd *pWnd)
{
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pStroke && pStroke->AddPoint(point,pWnd))
			break;
	}
}


void XTracker::DelDot()
{
	if ( m_pDotHead == NULL )
		return;
	
	DotHead *pList = m_pDotHead;
	DotHead  *pDotNext = NULL;
	DotSelectList *pDotList = NULL;
	CStroke  *pStroke  = NULL;
	
	int nIndex = 0;
	int nCount = 0;
	while( pList )
	{	
		pDotNext = pList->m_pNext;
		pStroke  = (CStroke*)(pList->m_pAdd);
		pDotList = pList->m_pHead;
		nCount   = 0;
		
		while( pDotList )
		{
			nIndex = pDotList->m_nNum;
			pStroke->RemoveDot(nIndex);
			if (DotListSub(&pList, nCount))
			{
				ModifyStrokeList(pStroke);
			}

			if (pList  !=  NULL)
			{
				pDotList = pList->m_pHead;
			}
			else
			{
				break;
			}
		}// end while
		
		if (pList != NULL)
		{
			pList = pList->m_pNext;
		}
		else
		{
			pList = pDotNext;
		}
	}// end while
}


//
void XTracker::ModifyStrokeList(CStroke *pStroke)
{	
	if ( NULL == pStroke )
		return;
	
	POSITION pos   = m_StrokeList.GetHeadPosition();
	CStroke *pTStroke = NULL;
	POSITION prev   = NULL;
	
	while( pos )
	{
		prev = pos;
		pTStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( pTStroke == pStroke )
		{
			m_StrokeList.RemoveAt(prev);
		}
	}
}


//
BOOL XTracker::HitValidPoint(CPoint &point, CWnd *pWnd)
{	
	POSITION pos = m_StrokeList.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{	
		pStroke = (CStroke *)m_StrokeList.GetNext(pos);
		if ( !pStroke || pStroke->IsFillStyle())
			continue;
		
		if ( pStroke->Click(point,pWnd) )
			return TRUE;
	}

	return FALSE;
}


//
void XTracker::ClearAll()
{	
	ClearDot();
	ClearStrokeList();
}

void XTracker::SplitPoint(double dbPoint)//断点操作
{	
	if ( m_pDotHead == NULL )
		return;
	
	DotHead *pDotHead = m_pDotHead;
	DotSelectList *pList = NULL;
	DotSelectList *pStore = NULL;
	CStroke *pStroke = NULL;
	int nCount;
	while(pDotHead)
	{	
		pList = pDotHead->m_pTail;
		pStroke = (CStroke*)(pDotHead->m_pAdd);
		if (NULL == pStroke->GetParent())
		{	
			return;
		}
		
		nCount = pStroke->GetSize();
		BOOL bFirst = TRUE;
		CDot dt1,dt2,dt3,dt4;
		int nBegin,nEnd,nTemp;
		while(pList)
		{
			//从后往前进行
			if (pList == pDotHead->m_pTail)
			{
				if (0<pList->m_nNum &&pList->m_nNum<nCount-1)
				{
					dt3 = pStroke->m_dotArray[pList->m_nNum];
					dt1 = pStroke->m_dotArray[pList->m_nNum-1];
					dt2 = pStroke->m_dotArray[pList->m_nNum+1];
					dt1 = pStroke->GetDot(dt1,dt3,dbPoint);
					dt2 = pStroke->GetDot(dt2,dt3,dbPoint);
					nBegin = pList->m_nNum+1;
					nEnd = pStroke->m_dotArray.GetUpperBound();
					pStroke->SplitDot(nBegin,nEnd,dt2,dt2);
					dt4 = dt1;
					nTemp = pList->m_nNum;
					pStore = pList;
					pList = pList->m_pFront;
				}
				else
				{
					pStore = pList->m_pFront;
					if (pStore)
					{
						if (pList->bLeft)
						{
							delete pList->m_pLeft;
							pList->bLeft = FALSE;
						}
						if (pList->bRight)
						{
							delete pList->m_pRight;
							pList->bRight = FALSE;
						}
						delete pList;
						pList = pStore;
						pList->m_pNext = NULL;
						pDotHead->m_pTail = pList;
					}
					else 
					{
						break;
					}
				}
			}
			else
			{
				if (0<pList->m_nNum &&pList->m_nNum<nCount-1)
				{
					dt3 = pStroke->m_dotArray[pList->m_nNum];
					dt1 = pStroke->m_dotArray[pList->m_nNum-1];
					dt2 = pStroke->m_dotArray[pList->m_nNum+1];
					dt1 = pStroke->GetDot(dt1,dt3,dbPoint);
					dt2 = pStroke->GetDot(dt2,dt3,dbPoint);
					nBegin = pList->m_nNum+1;
					nEnd = nTemp-1;
					pStroke->SplitDot(nBegin,nEnd,dt2,dt4);
					dt4 = dt1;
					nTemp = pList->m_nNum;
				}
				pStore = pList;
				pList = pList->m_pFront;
			}	
		}
		if (pStore  != NULL)
		{
			if (pStore == pDotHead->m_pHead)
			{
				pStroke->m_dotArray[nTemp] = dt4;
				pStroke->m_dotArray.SetSize(nTemp+1);
			}
		}
		pDotHead = pDotHead->m_pNext;
	}
}

void XTracker::JoinPoint()//联点操作
{
	if ( m_pDotHead == NULL )
		return;
	
	DotHead *pHead = m_pDotHead;
	int nCount = 0;
	while( pHead )
	{
		nCount++;
		pHead = pHead->m_pNext;
	}

	if ( 1 == nCount )
	{
		CStroke *pStroke = (CStroke*)(m_pDotHead->m_pAdd);
		if ( pStroke )  
		{
			int nCount = pStroke->m_dotArray.GetSize();
			CDot sdot = (pStroke->m_dotArray)[0];
			CDot edot = (pStroke->m_dotArray)[nCount - 1];
			double dbD = pStroke->Distance(sdot, edot);
			if (dbD > g_SysCfg.m_dbDotFilterPrecis)
			{
				(pStroke->m_dotArray).Add(sdot);
			}
			else
			{
				AfxMessageBox(IDS_THETWOPOINTTOOSMALL);
			}
		}
		return;
	}
	else if (2 == nCount)
	{
		//两点在两个不同的stroke中
		if (!IsDiffStroke())
		{
			return;
		}

		CStroke *pStroke1 = NULL;
		CStroke *pStroke2 = NULL;
		pHead = m_pDotHead;
		pStroke1 = (CStroke*)(pHead->m_pAdd);
		pHead = pHead->m_pNext;
		pStroke2 = (CStroke*)(pHead->m_pAdd);
		//两个Stroke不为填充线
		if (IsFillStyle(pStroke1)||IsFillStyle(pStroke2))
		{
			return;
		}

		//两点在两个不同的NODE中
		if ( !IsSameNode(pStroke1, pStroke2) )
			return;
		
		int nJudge1 = 0;
		int nJudge2 = 0;
		pHead = m_pDotHead;
		nJudge1 = HeadOrTail(pHead);
		pHead = pHead->m_pNext;
		nJudge2 = HeadOrTail(pHead);
		if ((0 == nJudge1) && (1 == nJudge2))
		{
			pStroke2->AddStroke(pStroke1);
		}
		if (nJudge2 == 0 && nJudge1 == 1)
		{
			pStroke1->AddStroke(pStroke2);
		}
		if (nJudge2 == 0 && nJudge1 == 0)
		{
			pStroke1->ConnectHeadToHead(pStroke2);
		}
		if (nJudge2 == 1 && nJudge1 == 1)
		{
			pStroke1->ConnectTailToTail(pStroke2);
		}
	}
	else
	{
		return;
	}
	
	return;
}

int XTracker::GetSelCnt()
{
	if (m_pDotHead == NULL)
		return 0;

	int nCnt(0);
	DotHead *pHead = m_pDotHead;		
	DotSelectList *pList = NULL;
	while( pHead )
	{
		pList = pHead->m_pHead;
		while( pList )
		{
			nCnt++;
			pList = pList->m_pNext;
		}
		pHead = pHead->m_pNext;
	}

	return nCnt;
}

void XTracker::GetSelDot(double& dbX, double& dbY, double& dbZ)
{
	dbX = dbY = dbZ = 0;
	if ( m_pDotHead == NULL )
		return ;

	int nCnt = GetSelCnt();
	if ( nCnt <= 0 )
		return ;

	BOOL bFirst(TRUE);
	DotHead *pHead = m_pDotHead;	
	CStroke *pStroke = NULL;
	DotSelectList *pList = NULL;
	while( pHead )
	{
		pList = pHead->m_pHead;
		pStroke = (CStroke*)(pHead->m_pAdd);
		while( pList )
		{
			if ( bFirst )
			{
				if (1 == nCnt)
				{
					dbX = pStroke->m_dotArray[pList->m_nNum].x;
					dbY = pStroke->m_dotArray[pList->m_nNum].y;
					dbZ = pStroke->m_dotArray[pList->m_nNum].z;
					return;
				}
				else
				{
					dbZ = pStroke->m_dotArray[pList->m_nNum].z;
				}
				bFirst = FALSE;
			}
			else
			{
				if ( dbZ != pStroke->m_dotArray[pList->m_nNum].z )
				{
					dbZ = 0;
					return;
				}
			}
			pList = pList->m_pNext;
		}
		pHead = pHead->m_pNext;
	}
}

//
void XTracker::ModifyDot(double dbX, double dbY, double dbZ)
{
	if ( m_pDotHead == NULL )
		return;

	int nCnt = GetSelCnt();
	DotHead *pHead = m_pDotHead;		
	CStroke *pStroke = NULL;
	DotSelectList *pList  = NULL;
	while( pHead )
	{
		pStroke = (CStroke*)(pHead->m_pAdd);
		if ( pStroke == NULL )
			continue;
		
		pList = pHead->m_pHead;
		while( pList )
		{
			if (1 == nCnt)
			{
				pStroke->m_dotArray[pList->m_nNum].x = dbX;
				pStroke->m_dotArray[pList->m_nNum].y = dbY;
				pList->m_Node.x = dbX;
				pList->m_Node.y = dbY;
			}
			pStroke->m_dotArray[pList->m_nNum].z = dbZ;
			pList = pList->m_pNext;
		}
		pHead = pHead->m_pNext;
	}
}


void XTracker::MoveDot(double MoveX, double MoveY)
{
	if ( m_pDotHead == NULL )
		return;

	DotHead *pHead = m_pDotHead;		
	CStroke *pStroke = NULL;
	DotSelectList *pList = NULL;
	while( pHead )
	{
		pStroke = (CStroke*)(pHead->m_pAdd);
		if ( pStroke == NULL )
			continue;

		pList = pHead->m_pHead;
		while ( pList )
		{
			pStroke->MoveDot(pList->m_nNum,MoveX,MoveY);
			pList->m_Node.x += MoveX;
			pList->m_Node.y += MoveY;
			pList = pList->m_pNext;
		}
		pHead = pHead->m_pNext;
	}
}


CStroke* XTracker::FindSelListFirstDot(int &nNum, int &nCount) 
{
	DotHead *pHead = m_pDotHead;
	if ( !pHead )
		return NULL;
	
	DotSelectList *pList = pHead->m_pHead;
	if ( pList == NULL )
	{
		AfxMessageBox(IDS_NO_SEL_EDITPOINT);
		return NULL;
	}

	nNum   = pList->m_nNum;
	nCount = 1;
	pList = pList->m_pNext;
	while ( pList )
	{
		pList = pList->m_pNext;
		nCount ++;
	}
	return (CStroke*)(pHead->m_pAdd);
}


void XTracker::KillAngle(CWnd *pWnd)
{
	int nNum   = -1;
	int nCount = 0;
	CStroke *pStroke = FindSelListFirstDot(nNum, nCount);
	if ( nCount > 1 )
	{
		AfxMessageBox(IDS_SELONEDOT_ONLY);
		return;
	}

	if ( pStroke == NULL ||  nNum < 0 ) 
		return;

	InverDrawDot(pWnd);
	ClearDot();

	pStroke->KillAngle(nNum, g_SysCfg.m_dbKillAngleSpace);
	AddStrokeToEditList(pStroke);

	FillRect(pWnd);
	DataEXChange();
}

BOOL XTracker::IsSameNode(CStroke *pStroke1, CStroke *pStroke2)
{
	if ( pStroke1 == NULL || pStroke2 == NULL )
		return FALSE;

	CNode *pNode1 = (CNode*)pStroke1->GetParent();
	CNode *pNode2 = (CNode*)pStroke2->GetParent();
	if ( pNode1 == pNode2 )
		return TRUE;

	return FALSE;
}

void XTracker::InverDrawDot(CWnd *pWnd)
{
	if ( pWnd == NULL || m_pDotHead == NULL )
		return;

	DotHead *pHead = m_pDotHead;
	while( pHead )
	{
		pHead->InverDrawDot(pWnd);
		pHead = pHead->m_pNext;
	}
}
