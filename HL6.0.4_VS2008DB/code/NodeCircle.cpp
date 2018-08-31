// NodeCircle.cpp: implementation of the CNodeCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeCircle.h"
#include "GArc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeCircle::CNodeCircle()
{
	m_type = NODE_CIRCLE;
	
	m_GArc.m_dbPrecision = m_dbPrecision = ::g_SysCfg.m_dbPrecision;    //��Ͼ���
	m_GArc.m_nSampleCount = m_nSampleCount = ::g_SysCfg.m_nSampleCount;  //ÿ�β�������
	
	m_nArcType = 3;
	m_stkCtrlDot.RemoveAllPoint();
}



CNodeCircle::~CNodeCircle()
{

}

//
BOOL CNodeCircle::Track(CWnd* pWnd, CPoint ptOrg)
{
	if ( !pWnd || ::GetCapture()  !=  NULL)
		return FALSE;
		
	AfxLockTempMaps();
	
	//�������
	pWnd->SetCapture();
	ASSERT( pWnd == CWnd::GetCapture() );
	pWnd->UpdateWindow();
	CZoomView* pView = (CZoomView*)pWnd;
	
	CDC* pDC = pWnd->GetDC();
	if ( pDC == NULL )
		return FALSE;

	CPen pen(PS_DOT,1,RGB(0,0,200));  
	CPen *pOldPen = pDC->SelectObject(&pen);
	
	CPoint pointNow = ptOrg;
	CPoint pointOld = pointNow;
	CPoint pointTemp;
	
	CDot dot1, dot2, dot3;
	CStroke *pStroke = new CStroke(m_property);
	if ( !pStroke )
		return FALSE;

	BOOL bEnd = FALSE;
	BOOL bStartPrev(FALSE);
	m_stkCtrlDot.RemoveAllPoint();
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
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
				pointOld = pointNow;
				CDot dt = (CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);
				AddDotToArray(m_stkCtrlDot.m_dotArray, dt);
				
				if (m_stkCtrlDot.m_dotArray.GetSize() == 2)
					bEnd = TRUE;
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
					pDC->MoveTo(pointOld);
					pDC->LineTo(pointNow); 
					
					pointNow.x = (int)(short)LOWORD(msg.lParam);
					pointNow.y = (int)(short)HIWORD(msg.lParam);
					
					pDC->MoveTo(pointOld);
					pDC->LineTo(pointNow); 
					
					if (m_stkCtrlDot.m_dotArray.GetSize() == 1)
					{
						CDot dt = (CDot)pointNow;
						pView->LogicToPhysic(dt.x,dt.y);
						XDC	*pXDC = ((CZoomView *)pWnd)->PrepareDC(pDC);
						
						pStroke->Draw(pXDC);
						pStroke->RemoveAllPoint();

						dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
						dot2 = dt;
						dot3 = dt;
						m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
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
	if ( nCount  !=  2 )
	{
		delete pStroke;
		return FALSE;
	}
	else
	{	
		pStroke->RemoveAllPoint();
		
		dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
		dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
		dot3 = dot2;
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

//
void CNodeCircle::AddDotToArray(CArray<CDot,CDot> &listDot, CDot dot)
{	
	if ( listDot.GetSize() < 1 )
	{
		listDot.Add(dot);
	}
	else
	{
		CDot dotTemp = listDot.GetAt(listDot.GetSize() - 1);
		
		if ( sqrt((dot.x - dotTemp.x)*(dot.x - dotTemp.x) + (dot.y - dotTemp.y)*(dot.y - dotTemp.y)) > 0.1 )
			listDot.Add(dot);
	}
}


//
void CNodeCircle::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
 	CMySerialize fs(fp,bUnicode);
 	CString str;
 	
 	if ( bFlag )
 	{
 		fs << _T("NCIRCLE_B");

		m_quadRangle.Serialize(fp,bFlag,bUnicode);
 		m_property.Serialize(fp,bFlag,bUnicode);

 		Serialize_DotList(fp,bFlag,bUnicode);

		fs  <<  200;
		fs  <<  m_nArcType;
		fs << 201;
		fs << m_dbPrecision;
		fs << 202;
		fs << m_nSampleCount;
 		fs << _T("NCIRCLE_E");
 	}
 	else
 	{
 		while( fs >> str )
 		{
			if ( str == _T("QUAD_RANG_B") )
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
 			else if ( str == _T("PRO_B") )
				m_property.Serialize(fp,bFlag,bUnicode);
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
				m_dbPrecision = _tstof(str);
			}
			else if ( str == _T("202") )
			{
				fs >> str;
				m_nSampleCount = _ttoi(str);
			}
 			else if ( str == _T("NCIRCLE_E") )
 				break;
 		}
		PrecisionSet(m_dbPrecision, m_nSampleCount);
		CreateStroke();
 	}
}



void CNodeCircle::Serialize_DotList(FILE* fp,BOOL bFlag,BOOL bUnicode)
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
		fs  <<  _T("DL_E") ;
	}
	else
	{
		while( fs >> str )
		{
			str.TrimRight();
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
			if ( str == _T("DL_E")  )
			{
				break;
			}
		}
	}
}


//
void CNodeCircle::Draw(XDC *pDC)
{	
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos != NULL )
	{
		pStroke = (CStroke*)m_list.GetNext(pos);	
		if ( pStroke )
			pStroke->Draw(pDC, 0, m_bIsCurSortTree);
	}
}


//
void CNodeCircle::CreateStroke()
{	
	if ( m_stkCtrlDot.m_dotArray.GetSize() < 2 )
		return;

	CDot dot1,dot2,dot3;
	dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
	dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
	dot3 = dot2;
	
	CStroke *pStroke = NULL;
	if ( m_list.GetCount() < 1 )
	{
		pStroke = new CStroke(m_property);
		if ( pStroke == NULL )
			return;

		if ( m_bSelect )
			pStroke->m_bSelect = TRUE;

		m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
		m_list.AddTail(pStroke);
	}
	else
	{
		POSITION pos = m_list.GetHeadPosition();		
		while(pos != NULL)
		{
			pStroke = (CStroke*)m_list.GetNext(pos);
			/////
			if ( pStroke )
			{
				pStroke->RemoveAllPoint();
				m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
			}
		}
	}
	
	InitQuadRange();
}


//
void CNodeCircle::Rotate(CDot dot,double angle, int nPlane)
{
	m_quadRangle.Rotate(dot,angle);
	for ( int i = 0; i < m_stkCtrlDot.m_dotArray.GetSize(); i++ )
	{
		m_stkCtrlDot.m_dotArray[i].Rotate(dot,angle);
	}
	CreateStroke();
	CalLimit();
}



//
void CNodeCircle::Scale(CDot dot,double ratioX,double ratioY)
{
	m_quadRangle.Scale(dot,ratioX,ratioY);
	for( int i = 0;i<m_stkCtrlDot.m_dotArray.GetSize();i++ )
	{
		m_stkCtrlDot.m_dotArray[i].Scale(dot, ratioX, ratioY);
	}
	CreateStroke();
	CalLimit();
}



BOOL CNodeCircle::Move(double moveX,double moveY, double moveZ)
{
	m_quadRangle.Move(moveX,moveY);
	for(int i = 0; i<m_stkCtrlDot.m_dotArray.GetSize(); i++)
	{
		m_stkCtrlDot.m_dotArray[i].Move(moveX,moveY);
	}
	CreateStroke();
	return TRUE;
}


void CNodeCircle::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if ( m_list.GetCount() <= 0 )
		return;

	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->ExportPlt(file,bUnExportFill,bScall100);
	}
}

//
BOOL CNodeCircle::TrackEdit(CWnd* pWnd)
{	
	if ( !pWnd || ::GetCapture()  !=  NULL)
		return FALSE;
	
	if ( m_list.GetCount() < 1 )
		return FALSE;
	
	AfxLockTempMaps();
	
	//�������
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	CZoomView* pView = (CZoomView*)pWnd;
	
	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);
 
	XDC  *pXDC = ((CZoomView *)pWnd)->PrepareDC(pDC);
	CStroke *pStroke = (CStroke *)m_list.GetHead();
	
	m_aryTouchSts.RemoveAll();
	for(int i = 0; i<m_stkCtrlDot.GetSize(); i++)
	{
		int nStatus = 0;
		m_aryTouchSts.Add(nStatus);
	}
	
	double dbPtMove = 3;
	pView->ActualLen(dbPtMove);
		
	pStroke->Draw(pXDC);
	DrawCtrlPointStroke(pXDC, dbPtMove);
	
	CPoint pointNow;
	CPoint pointOld;
	CPoint pointTemp;
	
	CDot dot1, dot2, dot3;
	
	BOOL bEnd = FALSE;
	BOOL bTouch = FALSE;
	BOOL bStartPrev(FALSE);	
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	
	while ( !bEnd )
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
				if ( bTouch )
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
						DrawCtrlPointStroke(pXDC, dbPtMove);

						pStroke->RemoveAllPoint();						
						SetEditPoint(dt);
						dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
						dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
						dot3 = dot2;
						m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
						
						pStroke->Draw(pXDC);
						DrawCtrlPointStroke(pXDC, dbPtMove);
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
	if ( nCount  !=  2 )
	{
		return FALSE;
	}
	else
	{
		pStroke->RemoveAllPoint();		
		dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);
		dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);
		dot3 = dot2;
		m_GArc.CreateArcline(dot1, dot2, dot3, pStroke, m_nArcType);
		
		if ( pStroke->m_dotArray.GetSize() < 2 )
			return FALSE;
	}
	InitQuadRange();
	
	return TRUE;
}




//
void CNodeCircle::DrawCtrlPointStroke(XDC *pXDC, double dbMove)
{
	if ( pXDC == NULL )
		return;

	COLORREF drawColor = RGB(0, 0, 200);
	COLORREF drawColorQuad = RGB(255,0,0);
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

	for( int i = 1; i < m_stkCtrlDot.m_dotArray.GetSize(); i++ )	
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
void CNodeCircle::SetEditPoint(CDot dot)
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


	
BOOL CNodeCircle::CheckTouchStatus(CDot dot)
{	
	int i = 0;
	BOOL bReturn = FALSE;
	
	for( i = 0; i<m_stkCtrlDot.GetSize(); i++)
	{
		m_aryTouchSts.SetAt(i, 0);
	}

	int    nMinIndex = 0;
	double dbMinDist = 9999999, dbDist = 0;
	for( i = 0; i < m_stkCtrlDot.GetSize(); i++)
	{
		CDot dotTemp = m_stkCtrlDot.m_dotArray.GetAt(i);
		dbDist = GetDotDistance(dot,dotTemp);
		if ( i == 0 )
		{
			dbMinDist = dbDist;
			nMinIndex = i;
		}
		else
		{
			if ( dbDist < dbMinDist )
			{
				dbMinDist = dbDist;
				nMinIndex = i;
			}
		}
	}
	
	if ( dbMinDist < 1 )
	{
		m_aryTouchSts.SetAt(nMinIndex, 1);
		bReturn = TRUE;
	}
	
	return bReturn;
}


double CNodeCircle::GetDotDistance(CDot dot1, CDot dot2)
{
	return sqrt((dot1.x - dot2.x) * (dot1.x - dot2.x) + (dot1.y - dot2.y) * (dot1.y - dot2.y));
}

//
void CNodeCircle::PrecisionSet(double dbPrecision, int nSampleCount)
{
	m_dbPrecision = dbPrecision;
	m_nSampleCount = nSampleCount;

	m_GArc.m_dbPrecision = m_dbPrecision;
	m_GArc.m_nSampleCount = m_nSampleCount;
}

BOOL CNodeCircle::Shear(double dbBaseX, double dbBaseY, double angleX, double angleY)
{
	return FALSE;
}


//
BOOL CNodeCircle::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_CIRCLE )
		return NULL;

	CNodeCircle *pNode = (CNodeCircle*)pSrc;
	/////////
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
	for(int i = 0; i<pNode->m_stkCtrlDot.m_dotArray.GetSize(); i++)
	{
		m_stkCtrlDot.m_dotArray.Add(pNode->m_stkCtrlDot.m_dotArray[i]);
	}
	CreateStroke();

	return TRUE;
}


void CNodeCircle::LoadAssistantPoints(CArray<DOT,DOT> &pointArray)
{
	if ( m_stkCtrlDot.m_dotArray.GetSize()  !=  2 ) // ������Բ�޿��Ƶ�
		return;

	CDot dot1 = m_stkCtrlDot.m_dotArray.GetAt(0);	// Բ��
	CDot dot2 = m_stkCtrlDot.m_dotArray.GetAt(1);	
	double dbRadius = m_GArc.CalDistance(dot1, dot2);
	DOT assistDot;
	assistDot.x = dot1.x;
	assistDot.y = dot1.y;
	assistDot.z = 2;
	pointArray.Add(assistDot);	// Բ��

	assistDot.x = dot1.x;
	assistDot.y = dot1.y + dbRadius;
	assistDot.z = 3;
	pointArray.Add(assistDot);	// ƽ��x���������е�

	assistDot.x = dot1.x;
	assistDot.y = dot1.y - dbRadius;
	assistDot.z = 3;
	pointArray.Add(assistDot);	// ƽ��x���������е�

	assistDot.x = dot1.x - dbRadius ;
	assistDot.y = dot1.y;
	assistDot.z = 3;
	pointArray.Add(assistDot);	// y���е�

	assistDot.x = dot1.x + dbRadius;
	assistDot.y = dot1.y;
	assistDot.z = 3;
	pointArray.Add(assistDot);	// y���е�
}
