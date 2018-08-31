// NodeArc3Pt.cpp: implementation of the CNodeArc3Pt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeArc3Pt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeArc3Pt::CNodeArc3Pt()
{
	m_type = NODE_ARC;
	
	m_GArc.m_dbPrecision = m_dbPrecision = ::g_SysCfg.m_dbPrecision;    //拟合精度
	m_GArc.m_nSampleCount = m_nSampleCount = ::g_SysCfg.m_nSampleCount;   //每段采样点数
	
	m_nArcType = 1;
	m_stkCtrlDot.RemoveAllPoint();
}


CNodeArc3Pt::CNodeArc3Pt(int nArcType)
{	
	m_type = NODE_ARC;
	
	m_GArc.m_dbPrecision = m_dbPrecision = ::g_SysCfg.m_dbPrecision;    //拟合精度
	m_GArc.m_nSampleCount = m_nSampleCount = ::g_SysCfg.m_nSampleCount;   //每段采样点数
	
	m_nArcType = nArcType;
	m_stkCtrlDot.RemoveAllPoint();
}


CNodeArc3Pt::~CNodeArc3Pt()
{

}


//
BOOL CNodeArc3Pt::Track(CWnd* pWnd, CPoint ptOrg)
{
	if (::GetCapture()  !=  NULL)
		return FALSE;
	
	AfxLockTempMaps();
	
	//捕获鼠标
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	
	// 取设备上下文
	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);

	CZoomView* pView = (CZoomView*)pWnd;
	XDC	*pXDC = pView->PrepareDC(pDC);
	
	CPoint pointNow;
	CPoint pointOld;
	CPoint pointTemp;
	CDot dt, dot1, dot2, dot3;
	
	pointNow = ptOrg;
	pointOld = pointNow;
	
	dt = (CDot)pointNow;
	pView->LogicToPhysic(dt.x,dt.y);

	dot1 = dt;
	dot2 = dt;
	dot3 = dt;
	
	CStroke *pStroke = new CStroke(m_property);
	if ( !pStroke )
		return FALSE;

	m_stkCtrlDot.RemoveAllPoint();
	
	BOOL bEnd = FALSE;
	BOOL bStartPrev(FALSE);
	
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	while (!bEnd)
	{	
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (CWnd::GetCapture()  !=  pWnd)
			break;
		
		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
 		case WM_LBUTTONDBLCLK:
 			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			{
				if ( bStartPrev == FALSE)
				{
					bStartPrev = TRUE;
				}
								
				AddDotToArray(m_stkCtrlDot.m_dotArray, dt);
				
				int nSize = m_stkCtrlDot.m_dotArray.GetSize();
				if ( nSize == 1 )
				{
					dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
					dot2 = dot1;
					dot3 = dot1;
				}
				else if (nSize == 2)
				{
					dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
				}
				else if (nSize == 3)
				{
					dot3 = m_stkCtrlDot.m_dotArray.GetAt(2);
					bEnd = TRUE;
				}
			}
			break;
		case WM_RBUTTONDOWN:
			{
				bEnd = TRUE;
			}
			break;
		case WM_MOUSEMOVE:
			{	
				int mode = pDC->SetROP2(R2_NOTXORPEN);
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				
				dt = (CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);

				if ( bStartPrev )
				{						
					if ( m_stkCtrlDot.m_dotArray.GetSize() == 1)
					{						
						if ( m_nArcType == 1 )
						{
							DrawPrevStroke(dot1, dot2, dot2, pXDC);
							dot2 = dt;
							dot3 = dt;
							DrawPrevStroke(dot1, dot2, dot2, pXDC);
						}
						else if ( m_nArcType == 2 )
						{
							DrawPrevStroke(dot1, dot2, dot1, pXDC);
							dot2 = dt;
							dot3 = dot1;
							DrawPrevStroke(dot1, dot2, dot1, pXDC);
						}						
					}
					else if ( m_stkCtrlDot.m_dotArray.GetSize() == 2 )
					{										
						pStroke->Draw(pXDC);
						pStroke->RemoveAllPoint();
						//
						if ( m_nArcType == 1 )
						{
							DrawPrevStroke(dot2, dot2, dot3, pXDC);
							dot3 = dt;
							DrawPrevStroke(dot2, dot2, dot3, pXDC);
						}
						else if (m_nArcType == 2)
						{
							DrawPrevStroke(dot1, dot1, dot3, pXDC);
							dot3 = m_GArc.AjustDot(dot1,dot2,dt);
							DrawPrevStroke(dot1, dot1, dot3, pXDC);
						}
						
						m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
						pStroke->Draw(pXDC);
					}									
				}
				pDC->SetROP2(mode);
			}
			break; 
			
		default:
			DispatchMessage(&msg);
			break;
		}
	}
	
	pWnd->ReleaseDC(pDC);
	ReleaseCapture();
	
	AfxUnlockTempMaps(FALSE);
	
	int nCount = m_stkCtrlDot.m_dotArray.GetSize();
	if  ( nCount  !=  3 )
	{
		delete pStroke;
		return FALSE;
	}
	else
	{	
		pStroke->RemoveAllPoint();
		m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
		if ( pStroke->m_dotArray.GetSize() > 2 )
		{
			m_list.AddTail(pStroke);
		}
		else
		{
			delete pStroke;
			return FALSE;
		}
	}
	
	return TRUE;
}



void CNodeArc3Pt::DrawPrevStroke(CDot dot1, CDot dot2, CDot dot3, XDC *pXDC)
{
	int nSize = m_stkCtrlDot.GetSize();
	if ( nSize > 2 )
		return;

	COLORREF drawColor = 0;
	drawColor = RGB(0, 0, 255);
	
	CPen pen(PS_DOT, 1, drawColor);
	CPen *pOldPen = pXDC->SelectObject(&pen);

	//画线
	if (m_nArcType == 1)
	{
		pXDC->MoveTo(dot1.x, dot1.y);
		pXDC->LineTo(dot2.x, dot2.y);
		pXDC->LineTo(dot3.x, dot3.y);
	}
	else if (m_nArcType == 2)
	{
		pXDC->MoveTo(dot1.x, dot1.y);
		pXDC->LineTo(dot2.x, dot2.y);

		pXDC->MoveTo(dot1.x, dot1.y);
		pXDC->LineTo(dot3.x, dot3.y);
	}
	
	pXDC->SelectObject(pOldPen);
	pen.DeleteObject();
}



//
void CNodeArc3Pt::AddDotToArray(CArray<CDot,CDot> &listDot, CDot dot)
{	
	if ( listDot.GetSize() < 1 )
	{
		listDot.Add(dot);
	}
	else
	{
		int nSize = listDot.GetSize();
		if ( nSize == 2 && m_nArcType == 2 )
		{
			dot = m_GArc.AjustDot(listDot.GetAt(0), listDot.GetAt(1), dot);
		}

		CDot dotTemp = listDot.GetAt(nSize - 1);		
		if ( sqrt((dot.x - dotTemp.x)*(dot.x - dotTemp.x) + (dot.y - dotTemp.y)*(dot.y - dotTemp.y)) > 0.1 )
			listDot.Add(dot);
	}
}


//
void CNodeArc3Pt::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
 	CMySerialize fs(fp,bUnicode);
 	CString str;
 	
 	if (bFlag)
 	{
 		fs << _T("NARC_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
 		m_property.Serialize(fp,bFlag,bUnicode);
 		Serialize_DotList(fp,bFlag,bUnicode);
		fs << 200;
		fs << m_nArcType;
		fs << 201;
		fs << m_dbPrecision;
		fs << 202;
		fs << m_nSampleCount;
 		fs << _T("NARC_E");
 	}
 	else
 	{
 		while( fs >> str )
 		{
 			if ( str == _T("PRO_B") )
 				m_property.Serialize(fp,bFlag,bUnicode);
  			else if ( str == _T("QUAD_RANG_B") )
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
 			else if ( str == _T("DL_B") )
 				Serialize_DotList(fp,bFlag,bUnicode);
			else if ( str == _T("200") )
			{
				fs >> str;
				m_nArcType = _ttoi(str);
			}
			else if ( str == _T("201") )
			{
				fs >> str;
				m_dbPrecision   = _tstof(str);
			}
			else if ( str == _T("202") )
			{
				fs >> str;
				m_nSampleCount = _ttoi(str);
			}
 			else if ( str == _T("NARC_E") )
 				break;
 		}
		
		PrecisionSet(m_dbPrecision, m_nSampleCount);
		CreateStroke();
 	}
}



void CNodeArc3Pt::Serialize_DotList(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	CDot dot;
	
	if ( bFlag )
	{
		fs << _T("DL_B");

		int nSize = m_stkCtrlDot.m_dotArray.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			dot = m_stkCtrlDot.m_dotArray.GetAt(i);
			dot.TrueToLog();
			fs << 100;
			fs << dot.x;
			fs << 110;
			fs << dot.y;
		}
		fs  <<  _T("DL_E");
	}
	else
	{
		while( fs >> str )
		{
			if ( str == _T("100") )
			{
				fs >> str;
				dot.x = _tstof(str);
				continue;
			}
			
			if ( str == _T("110") )
			{
				fs >> str;
				dot.y = _tstof(str);
				dot.LogToTrue();
				m_stkCtrlDot.m_dotArray.Add(dot);
				continue;
			}
					
			if ( str == _T("DL_E") )
			{
				break;
			}
		}
	}
}

//
void CNodeArc3Pt::Draw(XDC *pDC)
{	
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos != NULL)
	{
		pStroke = (CStroke*)m_list.GetNext(pos);		
		pStroke->Draw(pDC, 0, m_bIsCurSortTree);
	}
}


//
void CNodeArc3Pt::CreateStroke()
{	
	if (m_stkCtrlDot.m_dotArray.GetSize()  !=  3)
		return;

	CDot dot1,dot2,dot3;
	dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
	dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
	dot3 = m_stkCtrlDot.m_dotArray.GetAt(2);
	
	CStroke *pStroke = NULL;
	if (m_list.GetCount() < 1)
	{
		pStroke = new CStroke(m_property);	
		m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
		m_list.AddTail(pStroke);
	}
	else
	{
		POSITION pos = m_list.GetHeadPosition();		
		while(pos != NULL)
		{
			pStroke = (CStroke*)m_list.GetNext(pos);
			
			pStroke->RemoveAllPoint();
			m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
		}
	}
	InitQuadRange();
}


//
void CNodeArc3Pt::Rotate(CDot dot, double angle, int nPlane)
{
	m_quadRangle.Rotate(dot,angle);
	for(int i = 0;i<m_stkCtrlDot.m_dotArray.GetSize();i++)
	{
		m_stkCtrlDot.m_dotArray[i].Rotate(dot,angle);
	}
	CreateStroke();
	CalLimit();
}



//
void CNodeArc3Pt::Scale(CDot dot,double ratioX,double ratioY)
{	
	m_quadRangle.Scale(dot,ratioX,ratioY);
	for(int i = 0;i<m_stkCtrlDot.m_dotArray.GetSize();i++)
	{
		m_stkCtrlDot.m_dotArray[i].Scale(dot, ratioX, ratioY);

		//
		if (m_nArcType == 2)
		{
			CDot dot = m_GArc.AjustDot(m_stkCtrlDot.m_dotArray[0], m_stkCtrlDot.m_dotArray[1], m_stkCtrlDot.m_dotArray[2]);
			m_stkCtrlDot.m_dotArray.SetAt(2,dot);
		}
	}
	CreateStroke();

	CalLimit();
}



BOOL CNodeArc3Pt::Move(double moveX,double moveY, double moveZ)
{
	m_quadRangle.Move(moveX,moveY);
	for(int i = 0; i<m_stkCtrlDot.m_dotArray.GetSize(); i++)
	{
		m_stkCtrlDot.m_dotArray[i].Move(moveX,moveY);
	}
	CreateStroke();
	return TRUE;
}


void CNodeArc3Pt::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if ( m_list.GetCount()  <=  0 )
		return;

	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)	
	{	
		pStroke = (CStroke *)m_list.GetNext(pos);
		pStroke->ExportPlt(file,bUnExportFill,bScall100);
	}
}


//
BOOL CNodeArc3Pt::TrackEdit(CWnd* pWnd)
{	
	if ( !pWnd || ::GetCapture()  !=  NULL)
		return FALSE;
	
	if (  m_list.GetCount() < 1 )
		return FALSE;
	
	AfxLockTempMaps();
	
	//捕获鼠标
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	
	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);

 	CZoomView* pView = (CZoomView*)pWnd;
	XDC* pXDC = pView->PrepareDC(pDC);	

	m_aryTouchSts.RemoveAll();
	for(int i = 0; i<m_stkCtrlDot.GetSize(); i++)
	{
		int nStatus = 0;
		m_aryTouchSts.Add(nStatus);
	}
	double dbMove = 3;
	pView->ActualLen(dbMove);

	CStroke *pStroke = (CStroke *)m_list.GetHead();
	if ( pStroke == NULL )
		return FALSE;

	pStroke->Draw(pXDC);
	DrawCtrlPointStroke(pXDC, dbMove);
	
	CPoint pointNow;
	CDot dot1, dot2, dot3;
	
	BOOL bEnd = FALSE;
	BOOL bTouch = FALSE;
	BOOL bStartPrev(FALSE);	
	
	while (!bEnd)
	{	
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (CWnd::GetCapture()  !=  pWnd)
			break;
		
		pointNow.x = (int)(short)LOWORD(msg.lParam);
		pointNow.y = (int)(short)HIWORD(msg.lParam);

		CDot dt = (CDot)pointNow;
		pView->LogicToPhysic(dt.x,dt.y);
		
		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
 		case WM_LBUTTONDBLCLK:
 			break;
		case WM_LBUTTONDOWN:
			{
				bStartPrev = TRUE;
				bTouch = CheckTouchStatus(dt);
				if (bTouch)
				{
					bTouch = TRUE;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				bStartPrev = FALSE;
				bTouch = FALSE;
			}
			break;
		case WM_RBUTTONDOWN:
			{
				bEnd = TRUE;
			}
			break;
		case WM_MOUSEMOVE:
			{	
				if ( bStartPrev )
				{	
					int mode = pDC->SetROP2(R2_NOTXORPEN);
					if ( bTouch )
					{							
						pStroke->Draw(pXDC);
						DrawCtrlPointStroke(pXDC, dbMove);

						pStroke->RemoveAllPoint();						
						SetEditPoint(dt);
						dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
						dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
						dot3 = m_stkCtrlDot.m_dotArray.GetAt(2);
						m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
						pStroke->Draw(pXDC);
						DrawCtrlPointStroke(pXDC, dbMove);
					}
					pDC->SetROP2(mode);
				}
			}
			break;
			
		default:
			DispatchMessage(&msg);
			break;
		}
	}
			
	pWnd->ReleaseDC(pDC);
	ReleaseCapture();
	
	AfxUnlockTempMaps(FALSE);
	
	int nCount = m_stkCtrlDot.m_dotArray.GetSize();
	if (nCount  !=  3)
	{
		return FALSE;
	}
	else
	{	
		pStroke->RemoveAllPoint();		
		dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
		dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
		dot3 = m_stkCtrlDot.m_dotArray.GetAt(2);
		m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
		
		if (pStroke->m_dotArray.GetSize() < 2)
		{	
			return FALSE;
		}
	}
	
	InitQuadRange();

	return TRUE;
}


//
void CNodeArc3Pt::DrawCtrlPointStroke(XDC *pXDC, double dbMove)
{
	if ( pXDC == NULL )
		return;

	COLORREF drawColor = 0, drawColorQuad = 0;
	drawColor = RGB(0, 0, 255);
	drawColorQuad = RGB(255,0,0);
	CPen pen(PS_DOT, 1, drawColor);
	CPen penQuad(PS_SOLID, 1, drawColorQuad);
	CPen *pOldPen = pXDC->SelectObject(&pen);

	//画红框
	CDot dot = m_stkCtrlDot.m_dotArray[0];
	double x = dot.x;
	double y = dot.y;	
	
	pXDC->SelectObject(&penQuad);
	pXDC->MoveTo(x-dbMove, y+dbMove);
	pXDC->LineTo(x+dbMove, y+dbMove);	
	pXDC->LineTo(x+dbMove, y-dbMove);
	pXDC->LineTo(x-dbMove, y-dbMove);
	pXDC->LineTo(x-dbMove, y+dbMove);

	for(int i = 1;i<m_stkCtrlDot.m_dotArray.GetSize();i++)	
	{
		dot = m_stkCtrlDot.m_dotArray[i];
		x   = dot.x;
		y   = dot.y;
				
		pXDC->SelectObject(&penQuad);
		pXDC->MoveTo(x-dbMove, y+dbMove);
		pXDC->LineTo(x+dbMove, y+dbMove);
		pXDC->LineTo(x+dbMove, y-dbMove);
		pXDC->LineTo(x-dbMove, y-dbMove);
		pXDC->LineTo(x-dbMove, y+dbMove);
	}

	//画线	
	CDot dot0, dot1,dot2;
	dot0 = m_stkCtrlDot.m_dotArray[0];
	dot1 = m_stkCtrlDot.m_dotArray[1];
	dot2 = m_stkCtrlDot.m_dotArray[2];
	
	pXDC->SelectObject(&pen);
	if (m_nArcType == 1)
	{
		pXDC->MoveTo(dot0.x, dot0.y);
		pXDC->LineTo(dot1.x, dot1.y);
		pXDC->LineTo(dot2.x, dot2.y);
	}
	else if (m_nArcType == 2)
	{
		pXDC->MoveTo(dot0.x, dot0.y);
		pXDC->LineTo(dot1.x, dot1.y);

		pXDC->MoveTo(dot0.x, dot0.y);
		pXDC->LineTo(dot2.x, dot2.y);
	}
	
	pXDC->SelectObject(pOldPen);
	pen.DeleteObject();
	penQuad.DeleteObject();
}


//
void CNodeArc3Pt::SetEditPoint(CDot dot)
{
	for(int i = 0; i<m_aryTouchSts.GetSize(); i++)
	{
		if (m_aryTouchSts[i] == 1)
		{
			m_stkCtrlDot.m_dotArray.SetAt(i,dot);
			if (m_nArcType == 2)
			{
				CDot dot1;
				if (i == 0)
				{
					dot1 = m_GArc.AjustDot(m_stkCtrlDot.m_dotArray.GetAt(0), m_stkCtrlDot.m_dotArray.GetAt(1),
						   m_stkCtrlDot.m_dotArray.GetAt(2));

					m_stkCtrlDot.m_dotArray.SetAt(2, dot1);
				}
				else if (i == 1)
				{
					dot1 = m_GArc.AjustDot(m_stkCtrlDot.m_dotArray.GetAt(0), m_stkCtrlDot.m_dotArray.GetAt(2),
						   m_stkCtrlDot.m_dotArray.GetAt(1));

					m_stkCtrlDot.m_dotArray.SetAt(1, dot1);
				}
				else if (i == 2)
				{
					dot1 = m_GArc.AjustDot(m_stkCtrlDot.m_dotArray.GetAt(0), m_stkCtrlDot.m_dotArray.GetAt(1),
						   m_stkCtrlDot.m_dotArray.GetAt(2));

					m_stkCtrlDot.m_dotArray.SetAt(2, dot1);
				}
			}
			break;
		}
	}
}

//
BOOL CNodeArc3Pt::CheckTouchStatus(CDot dot)
{	
	int i = 0;
	BOOL bReturn = FALSE;
	for( i = 0; i < m_stkCtrlDot.GetSize(); i++)
	{
		m_aryTouchSts.SetAt(i, 0);
	}
	
	double dbMinDistance = 9999999, dbDistance = 0;
	int nMinIndex = 0;
	for(i = 0; i<m_stkCtrlDot.GetSize(); i++)
	{
		CDot dotTemp = m_stkCtrlDot.m_dotArray.GetAt(i);
		dbDistance = GetDotDistance(dot,dotTemp);
		
		if ( i == 0 )
		{
			dbMinDistance = dbDistance;
			nMinIndex = i;
		}
		else
		{
			if ( dbDistance < dbMinDistance )
			{
				dbMinDistance = dbDistance;
				nMinIndex = i;
			}
		}
	}

	if ( dbMinDistance < 1 )
	{
		m_aryTouchSts.SetAt(nMinIndex, 1);
		bReturn = TRUE;
	}

	return bReturn;
}

double CNodeArc3Pt::GetDotDistance(CDot dot1, CDot dot2)
{
	return sqrt((dot1.x - dot2.x) * (dot1.x - dot2.x) + (dot1.y - dot2.y) * (dot1.y - dot2.y));
}



void CNodeArc3Pt::PrecisionSet(double dbPrecision, int nSampleCount)
{
	m_dbPrecision = dbPrecision;
	m_nSampleCount = nSampleCount;

	m_GArc.m_dbPrecision = m_dbPrecision;
	m_GArc.m_nSampleCount = m_nSampleCount;
}

BOOL CNodeArc3Pt::Shear(double dbBaseX, double dbBaseY, double angleX, double angleY)
{
	return FALSE;
}

//
BOOL CNodeArc3Pt::CopyToMe(CNode *pSrc,DWORD dwStyle)
{	
	if ( pSrc == NULL || pSrc->GetType()  !=  NODE_ARC )
		return FALSE;

	CNodeArc3Pt *pNode = (CNodeArc3Pt*)pSrc;
	///////
	m_dbPrecision = pNode->m_dbPrecision;
	m_nSampleCount = pNode->m_nSampleCount;
	m_nArcType = pNode->m_nArcType;
	m_GArc.m_dbPrecision = pNode->m_dbPrecision;
	m_GArc.m_nSampleCount = pNode->m_nSampleCount;
	
	m_property = pNode->m_property;
	m_bSelect  = pNode->m_bSelect;
	m_quadRangle = pNode->m_quadRangle;
	
	//
	m_stkCtrlDot.RemoveAllPoint();
	for( int i = 0; i < pNode->m_stkCtrlDot.m_dotArray.GetSize(); i++ )
	{
		m_stkCtrlDot.m_dotArray.Add(pNode->m_stkCtrlDot.m_dotArray[i]);
	}
	CreateStroke();

	return TRUE;
}



