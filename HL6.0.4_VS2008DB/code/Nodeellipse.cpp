// NodeEllipse.cpp: implementation of the CNodeEllipse class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "hl.h"
#include "NodeEllipse.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeEllipse::CNodeEllipse()
{
	m_type		 =  NODE_ELLIPSE;
	m_nCount	 =  0;
	m_dbARC    = g_SysCfg.m_dbDrawEllipSet;
	m_pStrkTemp = NULL;
	//////////////////////////////////////////////////////////////////////////
	m_quadRangle = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
}
 
CNodeEllipse::~CNodeEllipse()
{

}


BOOL CNodeEllipse::Track(CWnd* pWnd, CPoint ptOrg)
{
	if ( m_pStrkTemp )
	{
		delete m_pStrkTemp;
		m_pStrkTemp = NULL;
	}
	
	if (::GetCapture()  !=  NULL) //A NULL return value means the current thread has not captured the mouse. However, it is possible that another thread or process has captured the mouse.
		return FALSE;

	CHLView *pHLView = (CHLView*)pWnd;
	
	BOOL bResult = TRUE;

	AfxLockTempMaps();
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	CDC* pDC;
	pDC = pWnd->GetDC();
	XDC *pXDC = pHLView->PrepareDC(pDC);
	ASSERT_VALID(pDC);

	CPoint pointNow;
	pointNow = ptOrg;
	BOOL bMoved = FALSE;

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_RECT);
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture()  !=  pWnd)
			break;

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			if (bMoved)
				bResult = TRUE;
			else
				bResult = FALSE;
			goto ExitLoop;
			break;
		case WM_MOUSEMOVE:
			{
				if ( !bMoved )
					bMoved = TRUE;
				
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);

				int mode = pDC->SetROP2(R2_NOTXORPEN);
				Draw(pXDC);
				CDot dot1,dot2;
				dot1.x = ptOrg.x;
				dot1.y = ptOrg.y;
				dot2.x = pointNow.x;
				dot2.y = pointNow.y;
				pHLView->LogicToPhysic(dot1.x,dot1.y);
				pHLView->LogicToPhysic(dot2.x,dot2.y);
				DOT Center;
				Center.x = (dot1.x+dot2.x)/2;
				Center.y = (dot1.y+dot2.y)/2;
				Create(dot1,dot2,Center,msg);
				Draw(pXDC);
				pDC->SetROP2(mode);
				break;
			}
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:

	pWnd->ReleaseDC(pDC);
	ReleaseCapture();
	AfxUnlockTempMaps(FALSE);

	if ( !bResult )
		return FALSE;

	return TRUE;
}


void CNodeEllipse::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if ( bFlag )
	{
		//写入头信息
		fs << _T("NEPS_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		//实际坐标转为13个控点
		PointToCtrPoint();
		SerializePoint(fp,bFlag,bUnicode);
		//写入尾信息
		fs << _T("NEPS_E");
	}
	else
	{
		while(fs >> str)
		{
			if ( str == _T("QUAD_RANG_B") )
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("PRO_B") )
				m_property.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("EPS_B") )
				SerializePoint(fp,bFlag,bUnicode);
			else if ( str == _T("NEPS_E") )
				break;
		}
		//由13个控点生成实际点
		CtrlPointToPoint();
	}
}



void CNodeEllipse::SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos != NULL)
	{	
		pStroke = (CStroke *)m_list.GetNext(pos);
		pStroke->SetLayColor(nLayer, color);
	}
	m_property.m_color  = color;
	m_property.m_nLayer  = nLayer;
}

UINT CNodeEllipse::Mark(HWND hWnd)
{
	if ( m_bPathOptimize == TRUE )
		return HLOK;

	PreprocessMark(0x00);
	if ( m_list.GetCount()  <=  0 )
		return HLOK;

	CLay* pLay = g_DAT.GetCurLay();
	CStroke *pStroke = NULL;
	int nNum =  pLay->GetRepeat(m_property.m_nLayer);
	for(int i =  0;i<nNum;i++)
	{
		POSITION pos = m_list.GetHeadPosition();
		while( pos != NULL )	
		{	
			pStroke = (CStroke *)m_list.GetNext(pos);
			UINT nRet = g_DAT.Mark(pStroke,hWnd);
			if ( HLERROR == nRet )
				return HLERROR;
		}
	}

	return HLOK;
}

UINT CNodeEllipse::SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark)
{
	PreprocessMark(0x00);
	if ( m_list.GetCount() <= 0 )
		return HLOK;

	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)	
	{	
		pStroke = (CStroke *)m_list.GetNext(pos);
		if  ( !pStroke )
			continue;

		UINT nRet = g_DAT.SelMark(pStroke, nLayer, hWnd, hSelStop, LINE_MARK, bIsRevMark);
		if ( HLOK  !=  nRet)
			return nRet;
	}

	return HLOK;
}


void CNodeEllipse::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if (m_list.GetCount() <= 0)
		return;

	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )	
	{	
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->ExportPlt(file,bUnExportFill,bScall100);
	}
}


//
BOOL CNodeEllipse::Create(CDot dt1,CDot dt2,DOT Center,MSG msg)
{
	if ( m_pStrkTemp )
	{
		m_list.RemoveAll();
		delete m_pStrkTemp;
		m_pStrkTemp = NULL;
	}
	
	double dbLen = fabs(dt2.x-dt1.x)/2;
	double dbWidth = fabs(dt2.y-dt1.y)/2;
	if ( msg.wParam & MK_CONTROL )
	{
		dbLen = dbWidth = max(dbLen,dbWidth);
	}

	if ( dbLen  >=  dbWidth )
	{
		CStroke *pStroke = new CStroke(m_property);
		if ( NULL == pStroke )
			return FALSE;

		m_pStrkTemp = pStroke;

		double r(0.0);
		if ( 2*dbLen > m_dbARC )
		{
			double dbTemp = (2*dbLen*dbLen-m_dbARC*m_dbARC)/(2*dbLen*dbLen);
			double dbAraf = acos(dbTemp);
			m_nCount = (int)( 2*PAI/dbAraf + 0.5);

			dbAraf = 2*PAI/m_nCount;
			CDot   dot;
			double dbArc(0);
			for(int i = 0;i<m_nCount;i++)
			{
				dbArc = i*dbAraf;
				r = sqrt((dbLen*dbLen*dbWidth*dbWidth)/(dbLen*dbLen*sin(dbArc)*sin(dbArc)+dbWidth*dbWidth*cos(dbArc)*cos(dbArc)));
				dot.x = r*cos(dbArc);
				dot.y = r*sin(dbArc);
				dot.x += Center.x;
				dot.y += Center.y;
				pStroke->Add(dot);
			}
			dot = pStroke->m_dotArray[0];
			pStroke->Add(dot);
			m_list.AddTail(m_pStrkTemp);
		}
	}
	else
	{
		CStroke *pStroke = new CStroke(m_property);
		if ( NULL == pStroke )
			return FALSE;
	
		m_pStrkTemp = pStroke;
		
		double r = dbLen;
		dbLen = dbWidth;
		dbWidth = r;
		if ( 2*dbLen > m_dbARC )
		{
			double  dbAraf = acos((2*dbLen*dbLen-m_dbARC*m_dbARC)/(2*dbLen*dbLen));
			m_nCount = (int)( 2.0*PAI/dbAraf + 0.5 );

			dbAraf = 2*PAI/m_nCount;
			CDot dot;
			double dbArc(0);
			for(int i = 0;i<m_nCount;i++)
			{
				dbArc = i*dbAraf;
				r = sqrt((dbLen*dbLen*dbWidth*dbWidth)/(dbLen*dbLen*cos(dbArc)*cos(dbArc)+dbWidth*dbWidth*sin(dbArc)*sin(dbArc)));
				dot.x = r*cos(dbArc);
				dot.y = r*sin(dbArc);
				dot.x += Center.x;
				dot.y += Center.y;
				pStroke->Add(dot);
			}
			dot = pStroke->m_dotArray[0];
			pStroke->Add(dot);
			m_list.AddTail(m_pStrkTemp);
		}
	}
	return TRUE;
}

void CNodeEllipse::SerializePoint(FILE *fp, BOOL bFlag, BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if ( bFlag )
	{
		fs << _T("EPS_B");
		for( int i = 0; i<13; i++ )
		{
			fs << 130;
			fs << m_dotArray[i].x;
			fs << 131;
			fs << m_dotArray[i].y;
		}
		fs << _T("EPS_E");
	}
	else
	{
		for( int i = 0; i<13; i++ )
		{
			fs >> str;
			if ( 130 == _ttoi(str) )
				fs >> m_dotArray[i].x;
			fs >> str;
			if ( 131 == _ttoi(str) )
				fs >> m_dotArray[i].y;
		}
		while( fs >> str )
		{
			if ( str == _T("EPS_E") )
				break;
		}
	}
}

void CNodeEllipse::PointToCtrPoint()
{
	CQuad quad = CalLimit();
	quad.TrueToLog();
	CDot center;
	center.x = (quad.left+quad.right)/2;
	center.y = (quad.top+quad.bottom)/2;
	double EtoBConst;
	EtoBConst = 2.0/3.0*(sqrt(2.0)-1);
	double offsetX,offsetY;
	offsetX = quad.Width()*EtoBConst;
	offsetY = quad.Height()*EtoBConst;
	
	//////////////////////////////
	m_dotArray[0].x = 
	m_dotArray[1].x = 
	m_dotArray[11].x = 
	m_dotArray[12].x = quad.left;

	m_dotArray[5].x = 
	m_dotArray[6].x = 
	m_dotArray[7].x = quad.right;

	m_dotArray[2].x = 
	m_dotArray[10].x = center.x-offsetX;

	m_dotArray[4].x = 
	m_dotArray[8].x = center.x+offsetX;

	m_dotArray[3].x = 
	m_dotArray[9].x = center.x;

	/////////////////////////////
	m_dotArray[2].y = 
	m_dotArray[3].y = 
	m_dotArray[4].y = quad.bottom;
	
	m_dotArray[8].y = 
	m_dotArray[9].y = 
	m_dotArray[10].y = quad.top;

	m_dotArray[7].y = 
	m_dotArray[11].y = center.y+offsetY;

	m_dotArray[1].y = 
	m_dotArray[5].y = center.y-offsetY;

	m_dotArray[0].y = 
	m_dotArray[12].y = 
	m_dotArray[6].y = center.y;
}

void CNodeEllipse::CtrlPointToPoint()
{
	CDC dc,dcscreen;
	dcscreen.CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
	dc.CreateCompatibleDC(&dcscreen);
	if (dc.GetMapMode() != MM_TEXT)
		return ;

	POINT lpPoints[13];
	int i = 0;
	for(;i<13;i++)
	{
		lpPoints[i] = (POINT)m_dotArray[i];
	}
	dc.BeginPath();
	dc.PolyBezier(lpPoints,13);
	dc.EndPath();
	dc.FlattenPath();
	
	//检取路径数据
	int nCount = dc.GetPath(NULL,NULL,0);
	CPoint* points = new CPoint[nCount];
	BYTE* bytes = new BYTE[nCount];
	dc.GetPath(points,bytes,nCount);
	
	CStroke *pStroke = new CStroke(m_property);
	for(i = 0;i<nCount;i++)
	{
		CDot dot;
		dot.x = points[i].x;
		dot.y = points[i].y;
		
		dot.LogToTrue();
		pStroke->Add(dot);
	}
	delete []points;
	delete []bytes;
	
	pStroke->SetProperty(PTY_INIT, m_property);
	m_list.AddTail(pStroke);
}


BOOL CNodeEllipse::Shear(double dbBaseX, double dbBaseY, double angleX, double angleY)
{
	return FALSE;
}


void CNodeEllipse::Rotate(CDot dot, double angle)
{
	return;
}
