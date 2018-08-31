
// NodeSpline.cpp: implementation of the CNodeSpline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "NodeSpline.h"
#include "HLView.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeSpline::CNodeSpline()
{
	m_type = NODE_SPLINE;
	
	m_GSpline.m_dbPrecision = m_dbPrecision = ::g_SysCfg.m_dbPrecision;    //拟合精度
	m_GSpline.m_nSampleCount = m_nSampleCount = ::g_SysCfg.m_nSampleCount;   //每段采样点数

	m_stkCtrlDot.RemoveAllPoint();
}

//
CNodeSpline::~CNodeSpline()
{

}

BOOL CNodeSpline::Track(CWnd* pWnd, CPoint ptOrg)
{
	if ( !pWnd || ::GetCapture()  !=  NULL)// 如果已经有窗口捕获了鼠标则退出
		return FALSE;
	
	AfxLockTempMaps();  // 在循环过程中保持映射模式
	
	//捕获鼠标
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	CZoomView* pView = (CZoomView*)pWnd;
	
	// 取设备上下文
	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);

	CPen   pen(PS_DOT,1,RGB(0,0,250));  
	CPen *pOldPen = pDC->SelectObject(&pen);
	
	CPoint pointNow;
	CPoint pointOld;
	CPoint pointTemp;
	pointNow = ptOrg;
	pointOld = pointNow;
	
	CStroke *pStroke = new CStroke(m_property);
	if ( !pStroke )
		return FALSE;

	BOOL bEnd = FALSE;
	BOOL bStartPrev(FALSE);
	m_stkCtrlDot.RemoveAllPoint();
	CWnd *pFrame = ((CWinApp *)AfxGetApp())->m_pMainWnd;
	while ( !bEnd )
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if ( CWnd::GetCapture()  !=  pWnd )
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
				bStartPrev = TRUE;
				int mode = pDC->SetROP2(R2_NOTXORPEN);
				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow); 
				
				pointOld  = pointNow;
				CDot dt = (CDot)pointNow;
				
				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow); 
				
				pView->LogicToPhysic(dt.x,dt.y);
				AddDotToArray(m_stkCtrlDot.m_dotArray, dt);
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
					pDC->MoveTo(pointOld);
					pDC->LineTo(pointNow); 
					
					pointNow.x = (int)(short)LOWORD(msg.lParam);
					pointNow.y = (int)(short)HIWORD(msg.lParam);
					
					pDC->MoveTo(pointOld);
					pDC->LineTo(pointNow); 
					
					if ( m_stkCtrlDot.m_dotArray.GetSize() > 1 )
					{	
						CDot dt = (CDot)pointNow;
						pView->LogicToPhysic(dt.x,dt.y);
						XDC* pXDC = ((CZoomView *)pWnd)->PrepareDC(pDC);
						
						pStroke->Draw(pXDC);
						pStroke->RemoveAllPoint();
						m_GSpline.CreatePrevSPline(m_stkCtrlDot.m_dotArray,pStroke,dt);
						pStroke->Draw(pXDC);
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
	
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	
	pWnd->ReleaseDC(pDC);
	ReleaseCapture();
	
	AfxUnlockTempMaps(FALSE);
	
	int nCount = m_stkCtrlDot.m_dotArray.GetSize();
	if ( nCount < 3 )
	{	
		delete pStroke;
		return FALSE;
	}
	else
	{	
		if ( AfxMessageBox(IDS_STRING_ISCLOSE,MB_YESNO) == IDYES )
		{	
			CDot dot = m_stkCtrlDot.m_dotArray.GetAt(0);
			AddDotToArray(m_stkCtrlDot.m_dotArray, dot);
		}
		
		pStroke->RemoveAllPoint();
		m_GSpline.CreateSPline(m_stkCtrlDot.m_dotArray,pStroke);
		m_list.AddTail(pStroke);
	}
	
	return TRUE;
}



//
void CNodeSpline::AddDotToArray(CArray<CDot,CDot> &listDot, CDot dot)
{	
	if ( listDot.GetSize() < 1 )
	{	
		listDot.Add(dot);
	}
	else
	{	
		CDot dotTemp = listDot.GetAt(listDot.GetSize() - 1);
		
		if ( sqrt((dot.x - dotTemp.x)*(dot.x - dotTemp.x) + (dot.y - dotTemp.y)*(dot.y - dotTemp.y)) > 0.1)
			listDot.Add(dot);
	}
}


//
void CNodeSpline::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
 	CMySerialize fs(fp,bUnicode);
 	CString str;
 	
 	if (bFlag)
 	{
 		fs << _T("NSPLIN_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
 		m_property.Serialize(fp,bFlag,bUnicode);
 		Serialize_DotList(fp,bFlag,bUnicode);
		fs << 201;
		fs << m_dbPrecision;
		fs << 202;
		fs << m_nSampleCount;
 		fs << _T("NSPLIN_E");
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
 			else if ( str == _T("DL_B") )
			{
 				Serialize_DotList(fp,bFlag,bUnicode);
 				continue;
 			}
			else if ( str == _T("201") )
			{
				fs >> str;
				m_dbPrecision = _tstof(str);
				continue;
			}
			else if ( str == _T("202") )
			{
				fs >> str;
				m_nSampleCount = _ttoi(str);
				continue;
			}
 			else if ( str == _T("NSPLIN_E") )
 				break;
 		}
		
		PrecisionSet(m_dbPrecision, m_nSampleCount);
		CreateStroke();
 	}
}


void CNodeSpline::Serialize_DotList(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	CDot dot;
	
	if (bFlag)
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
		fs << _T("DL_E");
	}
	else
	{
		while(fs >> str)
		{
			if (str == _T("100"))
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
void CNodeSpline::Draw(XDC *pDC)
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
void CNodeSpline::CreateStroke()
{	
	CArray<CDot, CDot> listDot;
	listDot.Append(m_stkCtrlDot.m_dotArray);

	CStroke *pStroke = NULL;
	if ( m_list.GetCount() < 1 )
	{
		pStroke = new CStroke(m_property);	
		m_GSpline.CreateSPline(listDot,pStroke);
		m_list.AddTail(pStroke);
	}
	else
	{
		POSITION pos = m_list.GetHeadPosition();		
		while( pos  !=  NULL )
		{
			pStroke = (CStroke*)m_list.GetNext(pos);
			
			pStroke->RemoveAllPoint();
			m_GSpline.CreateSPline(listDot,pStroke);
		}
	}
}

//
void CNodeSpline::Rotate(CDot dot, double angle, int nPlane)
{
	for( int i = 0; i<m_stkCtrlDot.m_dotArray.GetSize(); i++ )
	{
		m_stkCtrlDot.m_dotArray[i].Rotate(dot,angle);
	}
	m_quadRangle.Rotate(dot,angle);
	
	CreateStroke();
	CalLimit();
}

//
void CNodeSpline::Scale(CDot dot,double ratioX,double ratioY)
{	
	for( int i = 0; i<m_stkCtrlDot.m_dotArray.GetSize(); i++ )
	{
		m_stkCtrlDot.m_dotArray[i].Scale(dot, ratioX, ratioY);
	}
	m_quadRangle.Scale(dot,ratioX,ratioY);

	CreateStroke();
	CalLimit();
}

BOOL CNodeSpline::Move(double moveX, double moveY, double moveZ )
{
	for(int i = 0; i<m_stkCtrlDot.m_dotArray.GetSize(); i++)
	{
		m_stkCtrlDot.m_dotArray[i].Move( moveX, moveY, moveZ );
	}
	m_quadRangle.Move(moveX,moveY);

	CreateStroke();
	return TRUE;
}


void CNodeSpline::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if ( m_list.GetCount()  <=  0 )
		return;
	
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )	
	{	
		pStroke = (CStroke *)m_list.GetNext(pos);
		pStroke->ExportPlt(file,bUnExportFill,bScall100);
	}
}

//
BOOL CNodeSpline::TrackEdit(CWnd* pWnd)
{	
	if ( ::GetCapture()  !=  NULL )
		return FALSE;
	
	if ( m_list.GetCount() < 1 )
		return FALSE;
	
	AfxLockTempMaps();
	
	//捕获鼠标
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	CZoomView* pView = (CZoomView*)pWnd;
	
	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);
 
	XDC	*pXDC = ((CZoomView *)pWnd)->PrepareDC(pDC);
	CStroke *pStroke = (CStroke *)m_list.GetHead();
	
	m_aryTouchSts.RemoveAll();
	for(int i = 0; i<m_stkCtrlDot.GetSize(); i++)
	{
		int nStatus = 0;
		m_aryTouchSts.Add(nStatus);
	}

	double dbMove = 3;
	pView->ActualLen(dbMove);

	pStroke->Draw(pXDC);
	DrawCtrlPointStroke(pXDC, dbMove);
	
	CPoint pointNow;
	CPoint pointOld;
	CPoint pointTemp;
	CDot dot1, dot2, dot3;
	
	BOOL bEnd = FALSE;
	BOOL bTouch = FALSE;
	BOOL bStartPrev(FALSE);	
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	
	while (!bEnd)
	{	
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (CWnd::GetCapture()  !=  pWnd)
			break;
		
		pointNow.x = (int)(short)LOWORD(msg.lParam);
		pointNow.y = (int)(short)HIWORD(msg.lParam);
		pointOld = pointNow;
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
				if (bStartPrev)
				{	
					int mode = pDC->SetROP2(R2_NOTXORPEN);
										
					if (bTouch)
					{					
						pStroke->Draw(pXDC);
						DrawCtrlPointStroke(pXDC, dbMove);
						
						pStroke->RemoveAllPoint();						
						SetEditPoint(dt);
						m_GSpline.CreateSPline(m_stkCtrlDot.m_dotArray, pStroke);

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
	if (nCount < 3)
	{
		return FALSE;
	}
	else
	{	
		pStroke->RemoveAllPoint();
		m_GSpline.CreateSPline(m_stkCtrlDot.m_dotArray, pStroke);
		
		if (pStroke->m_dotArray.GetSize() < 2)
		{	
			return FALSE;
		}
	}
	
	return TRUE;
}

//
void CNodeSpline::DrawCtrlPointStroke(XDC *pXDC, double dbMove)
{
	COLORREF drawColor = 0, drawColorQuad = 0;
	drawColor = RGB(0, 0, 200);
	drawColorQuad = RGB(255,0,0);
	CPen pen(PS_DOT, 1, drawColor);
	CPen penQuad(PS_SOLID, 1, drawColorQuad);
	CPen *pOldPen = pXDC->SelectObject(&pen);
	CDot dot = m_stkCtrlDot.m_dotArray[0];
	
	double x = dot.x;
	double y = dot.y;
	
	pXDC->SelectObject(&penQuad);
	pXDC->MoveTo(x-dbMove, y+dbMove);
	pXDC->LineTo(x+dbMove, y+dbMove);	
	pXDC->LineTo(x+dbMove, y-dbMove);
	pXDC->LineTo(x-dbMove, y-dbMove);
	pXDC->LineTo(x-dbMove, y+dbMove);
	pXDC->MoveTo(x,y);
	
	for(int i = 1;i<m_stkCtrlDot.m_dotArray.GetSize();i++)	
	{
		dot = m_stkCtrlDot.m_dotArray[i];
		x   = dot.x;
		y   = dot.y;
		
		pXDC->SelectObject(&pen);
		pXDC->LineTo(x,y);
		
		pXDC->SelectObject(&penQuad);
		pXDC->MoveTo(x-dbMove, y+dbMove);
		pXDC->LineTo(x+dbMove, y+dbMove);
		pXDC->LineTo(x+dbMove, y-dbMove);
		pXDC->LineTo(x-dbMove, y-dbMove);
		pXDC->LineTo(x-dbMove, y+dbMove);

		pXDC->MoveTo(x, y);
	}
	
	pXDC->SelectObject(pOldPen);
	pen.DeleteObject();
	penQuad.DeleteObject();
}

//
void CNodeSpline::SetEditPoint(CDot dot)
{
	for(int i = 0; i<m_aryTouchSts.GetSize(); i++)
	{
		if (m_aryTouchSts[i] == 1)
		{
			m_stkCtrlDot.m_dotArray.SetAt(i,dot);
			break;
		}
	}
}

//
BOOL CNodeSpline::CheckTouchStatus(CDot dot)
{	
	int  i = 0;
	BOOL bReturn = FALSE;
	
	for( i = 0; i < m_stkCtrlDot.GetSize(); i++ )
	{
		m_aryTouchSts.SetAt(i, 0);
	}
	
	double dbMinDistance = 9999999, dbDistance = 0;
	int     nMinIndex = 0;
	for( i = 0; i < m_stkCtrlDot.GetSize(); i++ )
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


//
double CNodeSpline::GetDotDistance(CDot dot1, CDot dot2)
{
	return sqrt((dot1.x - dot2.x) * (dot1.x - dot2.x) + (dot1.y - dot2.y) * (dot1.y - dot2.y));
}


void CNodeSpline::PrecisionSet(double dbPrecision, int nSampleCount)
{	
	m_dbPrecision = dbPrecision;
	m_nSampleCount = nSampleCount;
	
	m_GSpline.m_dbPrecision = m_dbPrecision;
	m_GSpline.m_nSampleCount = m_nSampleCount;
}


//
BOOL CNodeSpline::Shear(double dbBaseX, double dbBaseY, double angleX, double angleY)
{	
	for( int i = 0 ; i < m_stkCtrlDot.m_dotArray.GetSize(); i++ )
	{	
		m_stkCtrlDot.m_dotArray[i].Shear(dbBaseX,dbBaseY, angleX, angleY);
	}

	m_quadRangle.Shear(dbBaseX,dbBaseY,angleX,angleY);
	CreateStroke();
	CalLimit();
	
	return TRUE;
}


//
BOOL CNodeSpline::CopyToMe(CNode *pSrc,DWORD dwStyle)
{	
	if ( pSrc == NULL || pSrc->GetType()  !=  NODE_SPLINE )
		return FALSE;

	CNodeSpline *pNode = (CNodeSpline*)pSrc;
	m_dbPrecision			 = pNode->m_dbPrecision;
	m_nSampleCount		 = pNode->m_nSampleCount;
	
	m_property		 =  pNode->m_property;
	m_bSelect		 =  pNode->m_bSelect;
	m_quadRangle	 =  pNode->m_quadRangle;
	//
	m_stkCtrlDot.RemoveAllPoint();
	for ( int i = 0; i < pNode->m_stkCtrlDot.m_dotArray.GetSize(); i++ )
	{
		m_stkCtrlDot.m_dotArray.Add(pNode->m_stkCtrlDot.m_dotArray[i]);
	}
	CreateStroke();
	
	return TRUE;
}

