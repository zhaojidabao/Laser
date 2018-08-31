// NodeLine.cpp: implementation of the CNodeLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "NodeLine.h"
#include "HLView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeLine::CNodeLine()
{
	m_type = NODE_LINE;

	m_quadRangle = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
}

CNodeLine::~CNodeLine()
{
}

CNodeLine::CNodeLine(CStroke *pStroke)
{
	m_type = NODE_LINE;

	Delete();

	m_list.AddTail(pStroke);
	m_property = pStroke->m_property;
	
	CalLimit();
	InitQuadRange();
}

void CNodeLine::Adjust(MSG &msg,const CPoint &ptOrg)
{
	//CTRL控制
	if ( MK_CONTROL & msg.wParam )
	{
		CPoint pointNow;
		pointNow.x = (int)(short)LOWORD(msg.lParam);
		pointNow.y = (int)(short)HIWORD(msg.lParam);

		int x = pointNow.x-ptOrg.x;
		int y = pointNow.y-ptOrg.y;
		if ( abs(x) > abs(y) )
			y = 0;
		else
			x = 0;

		msg.lParam = ptOrg.y+y;
		msg.lParam <<=  16;
		msg.lParam  +=  (ptOrg.x+x);
	}

}
void CNodeLine::Adjust(MSG msg, CPoint pointOld,CPoint &pointNew)
{
	int cx = 0,cy = 0;
	if ( MK_CONTROL & msg.wParam )
	{
		cx = pointNew.x-pointOld.x;
		cy = pointNew.y-pointOld.y;
		if ( abs(cx) > abs(cy) )
			pointNew.y = pointOld.y;
		else
			pointNew.x = pointOld.x;
	}
	
}


BOOL CNodeLine::Track(CWnd* pWnd, CPoint ptOrg)
{
	if ( pWnd == NULL || ::GetCapture()  !=  NULL )// 如果已经有窗口捕获了鼠标则退出
		return FALSE;

	CStroke *pStroke = new CStroke(m_property);
	if ( pStroke == NULL )
		return FALSE;

	pWnd->UpdateWindow();
	CHLView* pView = (CHLView*)pWnd;
	CHLDoc* pDoc = pView->GetDocument();
	if ( g_bEnableAssist && pDoc ) // 启动铺助点
	{
		pDoc->LoadAssistantPoints();	//加载点
	}

	g_ObjDrawAssist.m_anchorPoint.bEnable = FALSE; // 锚点没选中

	AfxLockTempMaps();  // 在循环过程中保持映射模式

	//捕获鼠标
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());

	//取设备上下文
	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);
	CPen pen(PS_SOLID,1,RGB(0,0,0));  
	CPen *pOldPen = pDC->SelectObject(&pen);

	CPoint pointOld;
	CDot  dotTrack;
	BOOL bLine = TRUE; 

	WORD wStatu(0); 
	BOOL bEnd = FALSE;
	BOOL bLButtonDownPrev(FALSE);
	int    mode(0);
	 
	CPoint pointNow = ptOrg;
	dotTrack = CDot(pointNow);
	pView->LogicToPhysic(dotTrack.x, dotTrack.y);
	
	//取消息，直到焦点丢失或结束绘图过程,存储的坐标都是逻辑坐标
	while ( !bEnd )
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if ( CWnd::GetCapture()  !=  pWnd )
			break;

		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
		case WM_LBUTTONDBLCLK: //双击退出至选择状态
			break;

		case WM_LBUTTONDOWN:
			bLButtonDownPrev = TRUE;
			break;

		case WM_LBUTTONUP:
			if ( bLine )
			{
				pStroke->Add(dotTrack); 
				if ( g_bEnableAssist )
					g_ObjDrawAssist.SetPointsFromStroke(pStroke);

				pointOld = pointNow;
			}
			else
			{
				pStroke->Add(dotTrack);
				if ( g_bEnableAssist )
					g_ObjDrawAssist.SetPointsFromStroke(pStroke);

				pointOld = pointNow;
				bLine = TRUE;
			}
			break;

		case WM_RBUTTONDOWN:
			bEnd = TRUE; 
			mode = pDC->SetROP2(R2_NOTXORPEN);
			pDC->MoveTo(pointOld);
			pDC->LineTo(pointNow); 
			pDC->SetROP2(mode);
			break;

		case WM_MOUSEMOVE:
			wStatu = msg.wParam; 
			if ( wStatu & MK_LBUTTON )
			{
				if ( bLButtonDownPrev || (!bLButtonDownPrev&&bLine) )
				{
					pStroke->Add(dotTrack);
					if ( g_bEnableAssist ) //启动了铺助对象
						g_ObjDrawAssist.SetPointsFromStroke(pStroke);

					bLButtonDownPrev = FALSE;
				}

				bLine = FALSE;
				pointOld = pointNow;
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				dotTrack = (CDot)pointNow;
				pView->LogicToPhysic(dotTrack.x, dotTrack.y);

				double dbX = 0, dbY = 0;
				if ( g_bEnableAssist && g_ObjDrawAssist.LookUp(dotTrack.x, dotTrack.y, dbX, dbY))
				{
					pDC->MoveTo(pointOld);
					double adjustX = dbX;
					double adjustY = dbY;
					pView->PhysicToLogic(adjustX, adjustY);
					pointNow.x = (int)adjustX;
					pointNow.y = (int)adjustY;
					pDC->LineTo(pointNow);

					g_ObjDrawAssist.m_anchorPoint.x = pointNow.x;
					g_ObjDrawAssist.m_anchorPoint.y = pointNow.y;
					g_ObjDrawAssist.DrawAnchor(pDC);	// 绘锚点
					
					dotTrack.x = dbX;
					dotTrack.y = dbY;
					pStroke->Add(dotTrack); 

					if ( g_bEnableAssist )
						g_ObjDrawAssist.SetPointsFromStroke(pStroke);
				}
				else
				{
					pDC->MoveTo(pointOld);
					pDC->LineTo(pointNow);

					dotTrack = (CDot)pointNow;
					pView->LogicToPhysic(dotTrack.x, dotTrack.y);
					pStroke->Add(dotTrack); 

					if ( g_bEnableAssist )
						g_ObjDrawAssist.SetPointsFromStroke(pStroke);
				}
			}
			else
			{
				mode = pDC->SetROP2(R2_NOTXORPEN);
				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow); 
				
				bLine = TRUE;
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				Adjust(msg,pointOld,pointNow);
				dotTrack = (CDot)pointNow;
				pView->LogicToPhysic(dotTrack.x, dotTrack.y);				

				g_ObjDrawAssist.DrawAnchor(pDC);
				g_ObjDrawAssist.m_anchorPoint.bEnable = FALSE;

				double dbX = 0,dbY = 0;
				if ( g_bEnableAssist && g_ObjDrawAssist.LookUp(dotTrack.x, dotTrack.y, dbX, dbY))
				{
					double adjustX = dbX;
					double adjustY = dbY;
					pView->PhysicToLogic(adjustX, adjustY);
					
					pointNow.x = (int)adjustX;
					pointNow.y = (int)adjustY;
					
 					dotTrack.x = dbX;
 					dotTrack.y = dbY;
					
					g_ObjDrawAssist.m_anchorPoint.x = pointNow.x;
					g_ObjDrawAssist.m_anchorPoint.y = pointNow.y;
					g_ObjDrawAssist.DrawAnchor(pDC);
				}

				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow); 
				pDC->SetROP2(mode);
			}
			DispatchMessage(&msg);
			break;

		default:
			DispatchMessage(&msg);
			break;
		}
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pView->ReleaseDC(pDC);
	ReleaseCapture();
	
	AfxUnlockTempMaps(FALSE);

	if ( pStroke->GetSize()  <=  0 )
	{
		delete pStroke;
		return FALSE;
	}

	m_list.AddTail(pStroke);
	return TRUE;
}


 void CNodeLine::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
 {
 	CMySerialize fs(fp,bUnicode);
 	CString str;
 	
 	if ( bFlag )
 	{
 		//写入头信息
		fs << _T("NLIN_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
 		m_property.Serialize(fp,bFlag,bUnicode);
 		Serialize_list(fp,bFlag,bUnicode);
 		//写入尾信息
 		fs << _T("NLIN_E");
 	}
 	else
 	{
 		while( fs >> str )
		{		
			if ( str == _T("QUAD_RANG_B") )
			{
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
				continue;
			}
 			else if ( str == _T("PRO_B") )
			{
 				m_property.Serialize(fp,bFlag,bUnicode);
 				continue;
 			}
 			else if ( str == _T("SL_B") )
			{
 				Serialize_list(fp,bFlag,bUnicode);
 				continue;
 			}
 			else if ( str == _T("NLIN_E") )
 				break;
 		}
 	}
 }


void CNodeLine::AddRect(CWnd *pWnd)
{
	if ( pWnd == NULL || m_list.GetCount()  <=  0 )
		return;

	CZoomView *pView = (CZoomView*)pWnd;
	CDC *pDC = pView->GetDC();
	if ( pDC == NULL  )
		return;

	CStroke *pStroke = (CStroke*)m_list.GetHead();
	if ( pStroke == NULL )
		return;

	CQuad quad;
	CDot   dot;
	CRect  rect;

	double dbR = 3.0;
	pView->ActualLen(dbR);
	CPen redPen(PS_SOLID,1,RED);
	CPen *pOldPen = pDC->SelectObject(&redPen);
	int  nCount = pStroke->m_dotArray.GetSize();
	/////////
	for( int i = 0; i < nCount; i++ )
	{
		dot = pStroke->m_dotArray[i];
		quad.left = dot.x-dbR;
		quad.right = dot.x+dbR;
		quad.bottom = dot.y-dbR;
		quad.top = dot.y+dbR;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		rect = (CRect)quad;
		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);
	}
	pDC->SelectObject(pOldPen);
	pView->ReleaseDC(pDC);
}

void CNodeLine::DelRect(CWnd *pWnd)
{
	if ( pWnd == NULL  || m_list.GetCount()  <=  0 )
		return;

	CHLView *pView = (CHLView*)pWnd;
	CDC *pDC = pView->GetDC();
	if ( pDC == NULL )
		return;

	CStroke *pStroke = (CStroke*)m_list.GetHead();
	if ( pStroke == NULL )
		return;

	CQuad quad;
	CDot   dot;
	CRect  rect;

	double dbR = 3.0;
	pView->ActualLen(dbR);
	CPen  redPen(PS_SOLID,1,RED);
	CPen *pOldPen = pDC->SelectObject(&redPen);
	int nCount = pStroke->m_dotArray.GetSize();
	int mode = pDC->SetROP2(R2_NOTXORPEN);

	for( int i = 0; i < nCount; i++ )
	{
		dot = pStroke->m_dotArray[i];
		quad.left = dot.x-dbR;
		quad.right = dot.x+dbR;
		quad.bottom = dot.y-dbR;
		quad.top = dot.y+dbR;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		rect = (CRect)quad;

		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);
	}
	pDC->SetROP2(mode);
	pDC->SelectObject(pOldPen);
	pView->ReleaseDC(pDC);
}


BOOL CNodeLine::GetRulerPos(CDot &dot)
{
	CalLimit();

	if ( m_quadLimit.IsEmpty() )
		return FALSE;

	dot = m_quadLimit.GetBaseDot(BASE_TM);
	return TRUE;
}


CTree *CNodeLine::UnGroup()
{
	CStroke *pStroke = (CStroke *)m_list.GetHead();
	if ( pStroke == NULL )
		return NULL;
	
	CNodeLine *pNode = new CNodeLine(pStroke);
	if ( pNode == NULL )
		return NULL;

	CTree *pTree = new CTree(pNode);
	if ( pTree == NULL )
	{
		delete pNode;
		return NULL;
	}
	m_list.RemoveHead();
	return pTree;
}



BOOL CNodeLine::CanUnGroup()
{
	if ( m_list.GetCount() > 1 )
		return TRUE;
	else
		return FALSE;
}
