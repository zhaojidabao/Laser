// HLView.cpp : implementation of the CHLView class
//
#include "stdafx.h"
#include "HL.h"
#include "MainFrm.h"
#include "EllipSetDlg.h"
#include "HLDoc.h"
#include "HLView.h"
#include "NodePlt.h"
#include "NodeLine.h"
#include "NodeRect.h"
#include "NodeEllipse.h"
#include "NodeGrid.h"
#include "NodePolygon.h"
#include "NodeText.h"
#include "NodeDib.h"
#include "DrawPolygonDlg.h"
#include "NodeCodebar.h"
#include "DrawGridDlg.h"
#include "SetRectangleDlg.h"
#include "TextBaseDlg.h"
#include "DlgImport.h"
#include "DlgHelpPath.h"
#include "MoveAccuDlg.h"
#include "CmdMAlign.h"
#include "KBChar.h"
#include "Nodedxf_Gp.h"
#include "NodeDelay.h"
#include "NodeInput.h"
#include "NodeOutput.h"
#include "NodeSpline.h"
#include "NodeArc3Pt.h"
#include "NodeCircle.h"
#include "NodeDot.h"
#include "NodeNC.h"
#include "NodeMotorPos.h"
#include "NodeCondition.h"
#include "NodeBufferLaser.h"
#include "NodeCCD.h"
#include "NodePosCali.h"
#include "DlgSetDelay.h"
#include "DlgSetInput.h"
#include "DlgSetOutput.h"
#include "DlgObjPrecisionSet.h"

#include "DlgMotionInfoSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString g_OpenFileName;

void GenImportFullName(CString& strFile)
{
	if ( -1  !=  strFile.Find(_T(':')) )
		return;
	
	CString strDocName;
	CMDIFrameWnd *pMainWnd = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	BOOL bFindDocument = FALSE;
	
	if ( pMainWnd )
	{
		CMDIChildWnd *pChildWnd = pMainWnd->MDIGetActive();
		if ( pChildWnd )
		{
			CHLDoc* pDoc = (CHLDoc*) pChildWnd->GetActiveDocument();
			if ( pDoc )
			{
				strDocName    = pDoc->GetPathName();
				bFindDocument =  TRUE;
			}
		}
	}
	
	if ( !bFindDocument )
	{
		strDocName = g_OpenFileName;
	}
	
	CString strImportPath;	
	CString strPath = strDocName.Left(strDocName.ReverseFind(_T('\\')));
	strImportPath   = strPath+strDocName.Mid(strDocName.ReverseFind(_T('\\')),strDocName.ReverseFind(_T('.'))-strDocName.ReverseFind(_T('\\')));
	strFile = strImportPath + _T("\\") + strFile;
}


/////////////////////////////////////////////////////////////////////////////
// CHLView
IMPLEMENT_DYNCREATE(CHLView, CZoomView)

BEGIN_MESSAGE_MAP(CHLView, CZoomView)
	//{{AFX_MSG_MAP(CHLView)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS,OnUpdatePos)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_SELECT, OnZoomSelect)
	ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(ID_ZOOM_PAGE, OnZoomPage)
	ON_COMMAND(ID_ZOOMOPER, OnZoomoper)
	ON_COMMAND(ID_ZOOMOFF, OnZoomoff)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_SELECT, OnUpdateZoomSelect)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_ALL, OnUpdateZoomAll)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_PAGE, OnUpdateZoomPage)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOPER, OnUpdateZoomoper)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOFF, OnUpdateZoomoff)

	ON_COMMAND(ID_DRAW_CLICK, OnDrawClick)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CLICK, OnUpdateDrawClick)
	ON_COMMAND(ID_DRAW_HAND, OnDrawHand)
	ON_UPDATE_COMMAND_UI(ID_DRAW_HAND, OnUpdateDrawHand)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_COMMAND(ID_DRAW_RECTANGLE, OnDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, OnUpdateDrawRectangle)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_DRAW_TEXT, OnDrawText)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TEXT, OnUpdateDrawText)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, OnUpdateDrawEllipse)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_IMPORT_VECTOR, OnFileImportVector)
	ON_COMMAND(ID_FILE_IMPORT_BMP, OnFileImportBmp)
	ON_COMMAND(ID_FILE_IMPORT_NC, OnFileImportNc)
	ON_COMMAND(ID_LOAD_OBJLIST, OnLoadObjList)
	ON_COMMAND(ID_DRAW_GRID, OnDrawGrid)
	ON_UPDATE_COMMAND_UI(ID_DRAW_GRID, OnUpdateDrawGrid)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, OnUpdateDrawPolygon)
	ON_COMMAND(ID_DRAW_CODEBAR, OnDrawCodebar)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CODEBAR, OnUpdateDrawCodebar)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDM_HELPPATH, OnHelppath)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_DRAW_DRAG, OnDrawDrag)
	ON_UPDATE_COMMAND_UI(ID_DRAW_DRAG, OnUpdateDrawDrag)
	ON_COMMAND(IDD_MOVEACCU, OnMoveaccu)
	ON_COMMAND(IDD_CONVERT, OnConvert)
	ON_COMMAND(ID_DELDOT, OnDeldot)
	ON_COMMAND(ID_JOINDOT, OnJoindot)
	ON_COMMAND(ID_SPLITDOT, OnSplitdot)
    ON_COMMAND(ID_SEL_ALLSTROKE, OnSelAllStroke)
	ON_COMMAND(ID_SET_CURPOINT_BEGIN, OnSetCurDotToStart)
	ON_COMMAND(ID_FILTER_POINT, OnFilterPoint)
	ON_COMMAND(ID_REVERRIS_STROKE, OnReverseStroke)
	ON_COMMAND(ID_FILTER_CURDOC_POINT, OnFilterCurDocPoint)
    ON_COMMAND(ID_KILL_ANGLE, OnKillAngle)
	ON_COMMAND(ID_SORTOBJ, OnSortObj)
	ON_UPDATE_COMMAND_UI(ID_SORTOBJ, OnUpdateSortObj)
	ON_COMMAND(ID_AUTOSORTOBJ, OnAutoSortObj)
	ON_UPDATE_COMMAND_UI(ID_AUTOSORTOBJ, OnUpdateAutoSortObj)
	ON_COMMAND(ID_SELSORT, OnSelSortObj)
	ON_UPDATE_COMMAND_UI(ID_SELSORT, OnUpdateSelSortObj)
	ON_COMMAND(ID_ALIGN_BOTTOM, OnAlignBottom)
	ON_COMMAND(ID_ALIGN_LEFT, OnAlignLeft)
	ON_COMMAND(ID_ALIGN_RIGHT, OnAlignRight)
	ON_COMMAND(ID_ALIGN_TOP, OnAlignTop)
	ON_COMMAND(ID_DISTANCE_X, OnDistanceX)
	ON_COMMAND(ID_DISTANCE_Y, OnDistanceY)
	ON_COMMAND(ID_EQUAL_HEIGHT, OnEqualHeight)
	ON_COMMAND(ID_EQUAL_WIDTH, OnEqualWidth)
	ON_COMMAND(ID_EQUAL_WH, OnEqualWh)
	ON_COMMAND(ID_ALIGN_X, OnAlignX)
	ON_COMMAND(ID_ALIGN_Y, OnAlignY)
	ON_COMMAND(ID_JOINTSTROKE_CURDOC, OnJointstrokeCurdoc)
	ON_COMMAND(ID_SET_USERFONT, OnSetUserfont)
	ON_COMMAND(ID_DRAW_SPLINE, OnDrawSpline)
	ON_COMMAND(ID_SPLINE_EDIT, OnSplineEdit)
	ON_COMMAND(ID_DRAW_ARC_3PT, OnDrawArc3pt)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARC_3PT, OnUpdateDrawArc3pt)
	ON_COMMAND(ID_DRAW_ARC_CENTER, OnDrawArcCenter)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ARC_CENTER, OnUpdateDrawArcCenter)
	ON_COMMAND(ID_DRAW_CIRCLE, OnDrawCircle)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, OnUpdateDrawCircle)
	ON_COMMAND(ID_ARC_EDIT, OnArcEdit)
	ON_COMMAND(ID_CIRCLE_EDIT, OnCircleEdit)
	ON_COMMAND(ID_PRECISION_SET, OnPrecisionSet)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SPLINE, OnUpdateDrawSpline)	
	ON_COMMAND(ID_DRAW_DOT, OnDrawDot)
	ON_UPDATE_COMMAND_UI(ID_DRAW_DOT, OnUpdateDrawDot)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TIMER, OnUpdateDrawTimer)
	ON_UPDATE_COMMAND_UI(ID_DRAW_INPUT, OnUpdateDrawInput)
	ON_UPDATE_COMMAND_UI(ID_DRAW_OUTPUT, OnUpdateDrawOutput)
	ON_UPDATE_COMMAND_UI(ID_DRAW_FILL, OnUpdateDrawFill)
	ON_UPDATE_COMMAND_UI(ID_DRAW_BMP, OnUpdateDrawBmp)
	ON_UPDATE_COMMAND_UI(ID_EQUAL_WIDTH, OnUpdateEqualWidth)
	ON_UPDATE_COMMAND_UI(ID_EQUAL_WH, OnUpdateEqualWh)
	ON_UPDATE_COMMAND_UI(ID_EQUAL_HEIGHT, OnUpdateEqualHeight)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_LEFT, OnUpdateAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_RIGHT, OnUpdateAlignRight)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_TOP, OnUpdateAlignTop)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_X, OnUpdateAlignX)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_Y, OnUpdateAlignY)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BOTTOM, OnUpdateAlignBottom)
	ON_UPDATE_COMMAND_UI(ID_DISTANCE_X, OnUpdateDistanceX)
	ON_UPDATE_COMMAND_UI(ID_DISTANCE_Y, OnUpdateDistanceY)
	ON_UPDATE_COMMAND_UI(IDD_CONVERT, OnUpdateConvert)
	ON_UPDATE_COMMAND_UI(ID_FILTER_CURDOC_POINT, OnUpdateFilterCurdocPoint)
	ON_UPDATE_COMMAND_UI(ID_PATH_NORMAL, OnUpdatePathNormal)
	ON_UPDATE_COMMAND_UI(ID_SET_USERFONT, OnUpdateSetUserfont)
	ON_COMMAND(ID_DRAW_MOTORMOVE, OnDrawMotormove)
	ON_UPDATE_COMMAND_UI(ID_DRAW_MOTORMOVE, OnUpdateDrawMotormove)
	ON_COMMAND(ID_DRAW_SETPOS, OnDrawSetpos)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SETPOS, OnUpdateDrawSetpos)
	ON_COMMAND(ID_MOVEACCU, OnMoveaccu)
	ON_COMMAND(ID_DRAW_CONDITION, OnDrawCondition)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CONDITION, OnUpdateDrawCondition)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CZoomView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CZoomView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CZoomView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_DRAW_TIMER, ID_DRAW_OUTPUT, OnDrawRange)
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_COMMAND(ID_DRAW_BUFFERLASER, &CHLView::OnDrawBufferlaser)
	ON_UPDATE_COMMAND_UI(ID_DRAW_BUFFERLASER, &CHLView::OnUpdateDrawBufferlaser)
	ON_COMMAND(ID_DRAW_CCD, &CHLView::OnDrawCcd)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CCD, &CHLView::OnUpdateDrawCcd)
	ON_COMMAND(ID_DRAW_POSCALI, &CHLView::OnDrawPoscali)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POSCALI, &CHLView::OnUpdateDrawPoscali)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLView construction/destruction

CHLView::CHLView()
{
	m_hCursorClick			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CLICK));
	m_hCursorShape		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_SHAPE));
	m_hCursorCross		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CROSS));
	m_hCursorCrossRot	 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CROSSIN));
	
	m_hCursorText			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_TEXT));
	m_hCursorBar			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_CODEBAR));
	m_hCursorEllipse		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_ELLIPSE));
	m_hCursorPoly			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_POLY));
	m_hCursorRect			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_RECT));
	m_hCursorGrid			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_GRID));
	m_hCursorDot			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_DOT));
	m_hCursorSpline		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_SPLINE));
	m_hCursorArc			 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_ARC));
	
	m_hCursorHorDrag	 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_HORDRAG));
	m_hCursorVerDrag	 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_VERDRAG));
	m_hCursorHVDrag1	 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_HVDRAG1));
	m_hCursorHVDrag2	 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_HVDRAG2));
	
	m_hCursorTBDrag		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_TB_DRAG));
	m_hCursorLRDrag		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_LR_DRAG));
	m_hDrag					 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_OBJMOVE));
	m_hCursorDoxMove	 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MOVE_BOX));
    m_hCursorZoomIn		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_ZOOMIN));
    m_hCursorMLineV		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MLINE_V));
	m_hCursorMLineH		 = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MLINE_H));

	m_hCursorRotateDrag  = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_MOUSEROTATE));
	m_hCursorViewRotate = ::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR_VIEWROTE));

	m_hCursorCur			 =  m_hCursorClick;
	m_nDrawTool			 =  DRAWMODEL_CLICK;
	m_bRotate	 =  FALSE;
	m_bDrag		 =  FALSE;
	m_bLBDown	 =  FALSE;
	m_nResult	 =  9;
	
	m_strAutoSortInfo.Empty();
	m_bActive = FALSE;
	
	InitIcon();
	CMainFrame *pMFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMFrame->OpenDoor();
}


//
CHLView::~CHLView()
{
	//删除光标
	if (m_hCursorClick)		::DestroyCursor(m_hCursorClick);
	if (m_hCursorShape)		::DestroyCursor(m_hCursorShape);
	if (m_hCursorCross)		::DestroyCursor(m_hCursorCross);
	if (m_hCursorCrossRot)	::DestroyCursor(m_hCursorCrossRot);


	if (m_hCursorText   )		::DestroyCursor(m_hCursorText);  
	if (m_hCursorBar   )		::DestroyCursor(m_hCursorBar);     
	if (m_hCursorEllipse)		::DestroyCursor(m_hCursorEllipse);  
	if (m_hCursorPoly  )		::DestroyCursor(m_hCursorPoly);   
	if (m_hCursorRect  )		::DestroyCursor(m_hCursorRect);    
	if (m_hCursorGrid  )		::DestroyCursor(m_hCursorGrid);    
	if (m_hCursorDot   )		::DestroyCursor(m_hCursorDot);     
	if (m_hCursorSpline)		::DestroyCursor(m_hCursorSpline);  
	if (m_hCursorArc   )		::DestroyCursor(m_hCursorArc);

	if (m_hCursorHorDrag)	::DestroyCursor(m_hCursorHorDrag);
	if (m_hCursorVerDrag)	::DestroyCursor(m_hCursorVerDrag);
	if (m_hCursorHVDrag1)  ::DestroyCursor(m_hCursorHVDrag1);
	if (m_hCursorHVDrag2)  ::DestroyCursor(m_hCursorHVDrag2);

	if (m_hCursorTBDrag)	::DestroyCursor(m_hCursorTBDrag);
	if (m_hCursorLRDrag)		::DestroyCursor(m_hCursorLRDrag);
	if (m_hDrag)				::DestroyCursor(m_hDrag);
	if (m_hCursorDoxMove)  ::DestroyCursor(m_hCursorDoxMove);
    if (m_hCursorZoomIn)	::DestroyCursor(m_hCursorZoomIn);
    if (m_hCursorMLineH)	::DestroyCursor(m_hCursorMLineH);
    if (m_hCursorMLineV)	::DestroyCursor(m_hCursorMLineV);

	if (m_hCursorRotateDrag )	::DestroyCursor(m_hCursorRotateDrag);
	if (m_hCursorViewRotate )	::DestroyCursor(m_hCursorViewRotate);
	
	m_track.ClearAll();
}

BOOL CHLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CZoomView::PreCreateWindow(cs);
}

void CHLView::OnNcPaint( )
{

}
/////////////////////////////////////////////////////////////////////////////
// CHLView drawing

void CHLView::OnDraw(CDC* pDC)
{
	CHLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ( pDoc->m_b3D )
	{
		pDoc->InitChain3D();
		pDoc->m_chain3d.Draw();
		return;
	}

	CDC *pwDC = GetWindowDC();
	DrawRuler(pwDC);
	ReleaseDC(pwDC);
	
	/*********************************************************************/
	CRect rcClient;
	GetClientRect(&rcClient);
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());
	CBitmap *pOldBmp = (CBitmap*)memDC.SelectObject(&bm);
	memDC.FillSolidRect(rcClient,RGB(255,255,255));
	/*********************************************************************/

	XDC *pXDC = PrepareDC(&memDC);	
	pDoc->m_pTracker = &m_track;
	pDoc->Draw(pXDC);

	DrawMeasureLine(&memDC);
	DrawBox(pXDC);
	m_pDC = &memDC;

	//
	if ( DRAWMODEL_SORT_OBJECT_LIST == m_nDrawTool )  //手动排序
	{	
		DrawMark(&memDC);
	}
	else if ( DRAWMODEL_ASORT_OBJECT_LIST == m_nDrawTool ) //自动排序
	{
		AutoSortDraw(&memDC);
	}
	else if ( DRAWMODEL_DISTORTION == m_nDrawTool ) 
	{
		DrawDistortion(pXDC);
	}
	else if (DRAWMODEL_CLICKFRAME == m_nDrawTool) 
	{
		DrawFrame(pXDC);
	}
	else if ( DRAWMODEL_DRAG  !=  m_nDrawTool && DRAWMODEL_OBJ_EDIT  !=  m_nDrawTool )
	{
		DrawTracker(pXDC);
	}
	
	//
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBmp);
	bm.DeleteObject();
	memDC.DeleteDC();

	if ( DRAWMODEL_DRAG == m_nDrawTool || DRAWMODEL_OBJ_EDIT == m_nDrawTool )
	{
		m_pDC = pDC;
		DrawRect();
	}
}


//
void CHLView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
	OnZoomPage();
	
	//设置UNDO
	GetDocument()->GetCommandProcessor().SetFrame((CMainFrame *)AfxGetMainWnd());
	PostMessage(WM_COMMAND, ID_ZOOM_PAGE & 0x0FFFF, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CHLView printing
BOOL CHLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->m_bPreview = TRUE;
	return DoPreparePrinting(pInfo);
}

void CHLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{

}

void CHLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{

}

/////////////////////////////////////////////////////////////////////////////
// CHLView diagnostics
#ifdef _DEBUG
void CHLView::AssertValid() const
{
	CZoomView::AssertValid();
}

void CHLView::Dump(CDumpContext& dc) const
{
	CZoomView::Dump(dc);
}

CHLDoc* CHLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHLDoc)));
	return (CHLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHLView message handlers

void CHLView::OnZoomIn() 
{
	CZoomView::ZoomIn();
}

void CHLView::OnZoomOut() 
{
	CZoomView::ZoomOut();
}

void CHLView::OnZoomPage() 
{
	double x(0.0),y(0.0);
	g_DAT.GetMarkRect(x,y);
	DoZoomIn(x*1.2,y*1.2);
}


void CHLView::NotifyZoom(void)
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc  !=  NULL )
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
}

void CHLView::DrawBox(XDC *pDC)
{
	//取BOX大小
	double x(0.0),y(0.0);
	g_DAT.GetMarkRect(x,y);

	HPEN  hPenOld,hPen1,hPen2;
	HBRUSH hBrshOld,hBrsh2;
	hPen1 = CreatePen(PS_SOLID,1,BLACK);
	hPen2 = CreatePen(PS_DOT,1,RED);
	hBrsh2 =  (HBRUSH)GetStockObject(GRAY_BRUSH);

#ifdef _SOFTMODE_EDITOR_
	double xLog = -x/2+0.5;
	double yLog =  y/2-0.5;
	pDC->PhysicToLogic(xLog,yLog);
	pDC->m_pDC->TextOut((int)xLog,(int)yLog,_T("Demo version, only for editor"));
#else
	double xLog = -x/2+0.5;
	double yLog =  y/2+7.5;
	pDC->PhysicToLogic(xLog,yLog);
	CString strTm1,strTm2;
	g_DogCheck.__GetTimeInfo2(strTm1,strTm2);
	
	if ( !strTm2.IsEmpty() )
	{
		CString strYear = strTm2.Left(4);
		if ( _ttoi(strYear) < 2999 )
		{
			strTm2 = strTm2.Left(10);
			pDC->m_pDC->TextOut( (int)xLog,(int)yLog,_T("Valid date: ")+strTm2 );
		}
	}
	
#endif

	//画方框
	pDC->m_pDC->SetBkMode(TRANSPARENT);
	hPenOld = (HPEN)pDC->SelectObject(hPen1);
	pDC->MoveTo(-x/2,y/2);
	pDC->LineTo(x/2,y/2);
	pDC->LineTo(x/2,-y/2);
	pDC->LineTo(-x/2,-y/2);
	pDC->LineTo(-x/2,y/2);
	
	//画阴影
	hBrshOld = (HBRUSH)pDC->SelectObject(hBrsh2);

	CQuad quad1(-x/2+1.5,-y/2,x/2+1.5,-y/2-1.5);
	CQuad quad2(x/2,y/2-2,x/2+1.5,-y/2);
	pDC->FillSolidQuad(quad1,GRAY);
	pDC->FillSolidQuad(quad2,GRAY);
	
	//画标准线
	if ( TRUE == g_sys.m_bDrawCrossLine )
	{
		pDC->SelectObject(hPen2);
		pDC->m_pDC->SetBkMode(TRANSPARENT);
		pDC->MoveTo(-2000000,0);
		pDC->LineTo(2000000,0);
		pDC->MoveTo(0,-2000000);
		pDC->LineTo(0,2000000);
	}
	
	//恢复GDI对象
	pDC->SelectObject(hPenOld);
	pDC->SelectObject(hBrshOld);
	
	//删除自定义GDI对象
	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hBrsh2);
}

//
void CHLView::OnDrawDrag()
{
	m_nDrawTool = DRAWMODEL_DRAG;
	m_hCursorCur =  m_hCursorClick;
	
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc &&  pDoc->IsHaveTextOrCodeBar() )
	{
		if ( IDNO == AfxMessageBox(IDS_CONVERTTOLINE, MB_YESNO))
			return;
		
		pDoc->Changeconvert(FALSE);
	}
	
	if ( m_track.HaveObject() )
	{
		Invalidate();
	}
}


//
void CHLView::OnUpdateDrawDrag(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc )	
	{
		pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
		pCmdUI->SetCheck((m_nDrawTool == DRAWMODEL_DRAG));
	}
	else
		pCmdUI->Enable(FALSE);
}



//
void CHLView::OnDrawClick()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
		return;
	
	if ( m_nDrawTool == DRAWMODEL_DRAG )
		pDoc->m_chain.InitQuadRange();

	m_nDrawTool  = DRAWMODEL_CLICK;
	m_hCursorCur = m_hCursorClick;
	m_track.ClearDot();
	pDoc->SetTrackerQuad(pDoc->GetLimit());
	
	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	}
}



void CHLView::OnUpdateDrawClick(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
		pCmdUI->Enable(FALSE);
	else
	{
		pCmdUI->Enable(!pDoc->m_b3D);
		pCmdUI->SetCheck(m_nDrawTool == DRAWMODEL_CLICK);
	}
}



void CHLView::OnDrawHand()
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	if ( GetZoomMode()  !=  MODE_ZOOMOFF )
		SetZoomMode(MODE_ZOOMOFF);

	m_nDrawTool = DRAWMODEL_HAND;
	m_hCursorCur =  m_hCursorCross;

	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		m_track.SetPhysicQuad( this, pDoc->GetTrackerQuad() );
	}

	if ( g_bEnableAssist ) // 启动铺助点
	{
		pDoc->LoadAssistantPoints();	//加载点
		g_ObjDrawAssist.m_anchorPoint.bEnable = FALSE; // 锚点没选中
	}
}



//
void CHLView::OnUpdateDrawHand(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	
	if ( m_nDrawTool == DRAWMODEL_HAND )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}



//
BOOL CHLView::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message ) 
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc && pDoc->m_b3D )
	{
		SetCursor(m_hCursorClick);
		return TRUE;
	}

	if ( GetZoomMode() != MODE_ZOOMOFF || message == WM_LBUTTONDOWN )
		return CZoomView::OnSetCursor(pWnd, nHitTest, message);

	return TRUE;
}




//
void CHLView::OnLButtonDown( UINT nFlags, CPoint point )
{
	CHLDoc *pDoc  = GetDocument();
	if ( pDoc == NULL )
		return;

	if (  pDoc->m_b3D )
	{
		SetCursor( m_hCursorDoxMove );
		Track3D( point );
		SetCursor( m_hCursorClick );
		return;
	}
	
	pDoc->m_pTracker = &m_track;
	m_track.m_ptMeasureLine = point;
	CLayer ly      = pDoc->GetCurLayer();
	CProperty pty  = ly.GetProperty();
	
	CZoomView::OnLButtonDown(nFlags,point);
	if ( m_bCanMoveMeasureLine )
	{
		m_bCanMoveMeasureLine = FALSE;
		return;
	}
	
	if ( point.x == 0 && point.y == 0 )
		return;
	
	CQuad quad;
	SetCursor( m_hCursorCur );
	
	switch( m_nDrawTool )
	{
	case DRAWMODEL_CLICK:
		{
			quad = pDoc->GetLimit();
			if ( pDoc->HaveSelectObject() )
			{
				m_nResult = m_track.GetResult(point,quad,this);
				
				CDot dot;
				double dbRatioX(0),dbRatioY(0),dbMoveX(0),dbMoveY(0);
				m_track.GetDragPara(m_nResult,quad,dot,dbRatioX,dbRatioY,dbMoveX,dbMoveY,this);
				
				if ( m_nResult >= 0 && m_nResult <= 8 )
				{
					pDoc->Drag(dot,dbRatioX,dbRatioY,dbMoveX,dbMoveY);
					Invalidate();
					break;
				}
			}
			
			if ( HitOtherObject(point) )
			{
				pDoc->SetObjSelInQuadList();
				Invalidate();
			}
			else
			{
				BOOL bUniSel = FALSE;
				if ( !(GetKeyState(VK_SHIFT) & SHIFTED) )
				{
					CDot dt = point;
					LogicToPhysic(dt.x,dt.y);
					if ( quad.DotIn(dt) )
					{
						m_nDrawTool = DRAWMODEL_CLICKFRAME;
						Invalidate();
						break;
					}
					quad.SetEmpty();
				}
				else
				{
					bUniSel = TRUE;
				}
				
				CQuad quadSel;
				m_track.Track(point,this);
				quadSel = (CQuad)m_track.m_rectOut;
				LogicToPhysic(quadSel);
				
				quadSel.Normal();
				pDoc->MultiSelect(quadSel, bUniSel);
			}
		}
		break;
		
	case DRAWMODEL_CLICKFRAME:
		{
			CDot   dot;
			double dbRatioX(0), dbRatioY(0), dbMoveX(0), dbMoveY(0);
			
			CQuadRangle frame = pDoc->GetQuadRangle();
			m_nResult = m_track.GetResult(point,frame,this);
			m_track.GetDragFrame(m_nResult,frame,dot,dbRatioX,dbRatioY,dbMoveX,dbMoveY,this);
			
			if ( m_nResult >= 0 && m_nResult <= 8 )
			{
				pDoc->DragFrame(frame, dot, dbRatioX, dbRatioY, dbMoveX, dbMoveY);
				frame = pDoc->GetQuadRangle();
			    Invalidate();
				break;
			}
			
			if ( !HitOtherObject(point) ) 
			{
				quad = pDoc->GetLimit();
				CDot dt = point;
				LogicToPhysic(dt.x,dt.y);
				if (quad.DotIn(dt))
				{
					m_nDrawTool  = DRAWMODEL_DISTORTION;
					Invalidate();
					break;
				}
				
				m_track.Track(point,this);
				quad = (CQuad)m_track.m_rectOut;
				LogicToPhysic(quad);
				quad.Normal();
				pDoc->MultiSelect(quad);// 多选操作
				
				if ( !pDoc->HaveSelectObject() )
				{
					m_nDrawTool  = DRAWMODEL_CLICK;
					m_hCursorCur = m_hCursorClick;
				}
			}
		}
		break;
		
	case DRAWMODEL_DISTORTION:
		{
			CQuad quad = pDoc->GetLimit();
			int nRst = m_track.GetDistortionPos(point,quad,this);
			if ( nRst >= 0 && nRst <= 8 ) 
			{
				CDot dot(point);
				LogicToPhysic(dot.x, dot.y);
				double dbRotateA(0.0), dbShearXA(0.0), dbShearYA(0.0);
				CQuadRangle qd = pDoc->GetQuadRangle();
				m_track.GetDistortion(nRst, qd, dot, dbRotateA, dbShearXA, dbShearYA, this);
				
				switch(nRst)
				{
					case 0:
					case 1:
					case 2:
					case 3:
						pDoc->Rotate(quad.GetBaseDot(BASE_MM), dbRotateA);
						break;
					case 4:
						pDoc->Shear(dot,dbShearXA,0);
						break;
					case 5:
						pDoc->Shear(dot,0,dbShearYA);
						break;
					case 6:
						pDoc->Shear(dot,dbShearXA,0);
						break;
					case 7:
						pDoc->Shear(dot,0,dbShearYA);
						break;
				}
			}
			else
			{
				if ( !HitOtherObject(point) ) 
				{
					CDot dt = point;
					LogicToPhysic(dt.x,dt.y);
					if (quad.DotIn(dt)) 
					{
						m_nDrawTool  = DRAWMODEL_CLICK;
						Invalidate();
						break;
					}

					m_track.Track(point,this);
					quad = (CQuad)m_track.m_rectOut;
					LogicToPhysic(quad);
					quad.Normal();
					pDoc->MultiSelect(quad);// 多选操作
				
					if (!pDoc->HaveSelectObject())
					{
						m_nDrawTool  = DRAWMODEL_CLICK;
						m_hCursorCur = m_hCursorClick;
					}
				}
			}			
		}
		break;
	//
	case DRAWMODEL_DRAG:
		{
			if ( !m_track.HaveObject() )
			{
				OnDrawClick();
			}
			SetFocus();
			if ( !m_track.ListEmpty() )
			{
				//是否存在选中点，如果有选中点进入
				if (m_track.HitSelectDot(point,this))
				{
					//对选中点进行编辑
					pDoc->DotEdit(&m_track,point,this);
				}
				else 
				{
					//当前末选中，但MOUSE已选中其它点时进入
					if (m_track.HitOtherDot(point,this))
					{
						m_track.ClearAndStore(point,this);
						pDoc->DotEdit(&m_track,point,this);
					}
					else
					{
						m_track.Track(point,this);
						quad = (CQuad)m_track.m_rectOut;
						LogicToPhysic(quad);
						quad.Normal();
						if (m_track.HasDot(quad))
						{
							m_track.ClearAndStore(quad,this);
						}
						else 
						{
							m_track.DelRect(this);
							m_track.InverDrawDot(this);
							m_track.ClearDot();
						}
					}
				}
			}
			else  
			{
				//进行选点操作
				if ( m_track.IsOnDot(point,this) )
				{
					//选中后对选中的点进行操作
					m_track.Edit(point,this);
					pDoc->DotEdit(&m_track,point,this);
				}
				else 
				{
					//用框选来选点
					m_track.Track(point,this);
					quad = (CQuad)m_track.m_rectOut;
					LogicToPhysic(quad);
					quad.Normal();
					if ( m_track.HasDot(quad) )
					{
					m_track.Edit(quad,this);
					}
					else
					{
						m_track.DelRect(this);
					}
				}
			}
			
			CDot dot(point);
			LogicToPhysic(dot.x, dot.y);
			
			CQuad qdNew(dot,50,50);
			PhysicToLogic(qdNew);
			InvalidateRect(&(CRect)qdNew);
			break;
		}
	    //////////////////////////
		case DRAWMODEL_HAND:
		{
			ClearMark();
			
			//铺助点
			CDot dot = (CDot)point;
			LogicToPhysic(dot.x, dot.y);
			if ( g_bEnableAssist )
			{
				CDC* pDC = GetDC();
				g_ObjDrawAssist.DrawAnchor(pDC);

				double dbX, dbY;
				if ( g_ObjDrawAssist.LookUp(dot.x, dot.y, dbX, dbY) )
				{
					double adjustX = dbX;
					double adjustY = dbY;
					PhysicToLogic(adjustX, adjustY);
					
					point.x = (int)adjustX;
					point.y = (int)adjustY;
					dot.x = dbX;
					dot.y = dbY;
					
					g_ObjDrawAssist.m_anchorPoint.bEnable = FALSE;
				}
			}
			
			CNode *pNode = new CNodeLine();
			if ( !pNode  )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if ( pNode->Track(this,point) )
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_LINE);

				CStrokeB *pStroke = pNode->m_list.GetHead();
				if ( pStroke )
				{
					if ( pStroke->GetSize() > 0 )
					{
						pStroke->m_dotArray.SetAt( 0, dot );
					}
				}
			}
			else
			{
				delete pNode;
			}
			break;
		}
	case DRAWMODEL_SPLINE:
		{
			ClearMark();
			CNode *pNode = new CNodeSpline();
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if ( pNode->Track(this,point) )
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_SPLINE);
			}
			else
			{
				delete pNode;
			}
			break;
		}
	case DRAWMODEL_ARC_3PT:
		{
			ClearMark();
			CNode *pNode = new CNodeArc3Pt(1);
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if (pNode->Track(this,point))
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_ARC);
			}
			else
			{
				delete pNode;
			}
			break;
		}

	case DRAWMODEL_ARC_CENTER:
		{
			ClearMark();
			CNode *pNode = new CNodeArc3Pt(2);
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);			
			if (pNode->Track(this,point))
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_ARC);
			}
			else
			{
				delete pNode;
			}
			break;
		}

	case DRAWMODEL_CIRCLE:
		{
			ClearMark();
			CNode *pNode = new CNodeCircle();
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if (pNode->Track(this,point))
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_CIRCLE);
			}
			else
			{
				delete pNode;
			}
			break;
		}

	case DRAWMODEL_RECTANGLE:
		{
			ClearMark();
			CNode *pNode = new CNodeRect();
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if ( pNode->Track(this,point) )
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_RECT);
			}
			else
			{
				delete pNode;
			}
			break;
		}

	case DRAWMODEL_ELLIPSE:
		{
			ClearMark();
			CNode *pNode = new CNodeEllipse();
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if (pNode->Track(this, point))
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_ELLIPSE);
			}
			else
			{
				delete pNode;
			}
			break;
		}

	case DRAWMODEL_POLYGON:
		{
			ClearMark();
			CNode *pNode = new CNodePolygon();
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if (pNode->Track(this,point))
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_POLYGON);
			}
			else
			{
				delete pNode;
			}
			break;
		}

	case DRAWMODEL_GRID:
		{	
			ClearMark();
			CNode *pNode = new CNodeGrid();
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);
			if ( pNode->Track(this,point) )
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode, NODE_GRID);
			}
			else
			{
				delete pNode;
			}
			break;
		}

	case DRAWMODEL_TEXT:
		{
			ClearMark();
			///		
			CText* pText = new CText();
			if ( pText == NULL )
				return;

			CNodeText *pNodeText = new CNodeText();
			if ( pNodeText == NULL )
			{
				delete pText;
				return;
			}

			pNodeText->SetParent(&pDoc->m_chain);//注意：此条代码非常重要
			pNodeText->ClearAllText();
			pNodeText->AddTextAll_NewID(pText);
			
			ViewDPtoLP((LPPOINT)(&point));
			
			CDot dot(point);
			pNodeText->m_dotBase = dot;
			pNodeText->SetProperty(PTY_INIT,pty);
			pNodeText->CreateString(FALSE);
			pNodeText->InitQuadRange();
			pDoc->AddNode(pNodeText);
			
			m_nDrawTool = DRAWMODEL_CLICK;
			m_hCursorCur = m_hCursorClick;
			break;
		}
		
	case DRAWMODEL_CODEBAR:
		{
			ClearMark();
			///		
			CText* pText = new CText();
			if ( pText == NULL )
				return;

			pText->m_bBarCode = TRUE;
			pText->m_nBarType = EAN128;
			pText->FlashText(0x00);	//得到字串数据

			CNodeCodebar *pNode = new CNodeCodebar();
			if ( NULL == pNode )
			{
				delete pText;
				return;
			}

			pNode->SetParent(&pDoc->GetChain());
			pNode->SetProperty(PTY_INIT,pty);
			pNode->ClearAllText();
			pNode->AddTextAll_NewID(pText);
			pNode->m_BarType = EAN128;

			ViewDPtoLP((LPPOINT)(&point));
			CDot dot(point);
			pNode->m_dotBase = dot;
			pNode->GenBarCode(0x00);
			pNode->CalLimit();
			pNode->InitQuadRange();
			pDoc->AddNode(pNode);
			
			m_nDrawTool = DRAWMODEL_CLICK;
			m_hCursorCur = m_hCursorClick;
			break;
		}

	case DRAWMODEL_DOT:
		{
			ViewDPtoLP((LPPOINT)(&point));
			CNodeDot *pNode = new CNodeDot();
			if ( pNode == NULL )
				return;

			pNode->SetProperty(PTY_INIT,pty);	
			if (pNode->Create(point)) 
			{
				pNode->InitQuadRange();
				pDoc->AddNode(pNode);
			}
			else
			{
				delete pNode;
			}		
		}
		break;

	default:
		{
			break;
		}
	}

	// 显示当前选中对象的属性
	DispSelObjProp();
}



void CHLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc && pDoc->m_b3D )
		return;
	
	SetCursor(m_hCursorCur);
	CZoomView::OnLButtonUp(nFlags,point);
}

BOOL CHLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc && pDoc->m_b3D)
	{
		pDoc->m_chain3d.Zoom(zDelta>0);
		pDoc->m_chain3d.Draw();
		return TRUE;
	}

	CZoomView::OnMouseWheel(nFlags, zDelta, pt);

	return TRUE;
}

//
void CHLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CHLDoc *pDoc  = GetDocument();
	if ( pDoc == NULL || pDoc->m_b3D )
		return;
	
	m_bCanMoveMeasureLine = FALSE;
	m_bCanSetLinePos = FALSE;
	CZoomView::OnMouseMove(nFlags,point);
	SetCursor(m_hCursorCur);
	
	if ( DRAWMODEL_CLICK == m_nDrawTool || DRAWMODEL_CLICKFRAME == m_nDrawTool )
	{
		if ( pDoc->HaveSelectObject() )
		{
			if ( DRAWMODEL_CLICK == m_nDrawTool )
			{
				CQuad quad = pDoc->GetLimit();
				m_nResult = m_track.GetResult(point,quad,this);//得到HIT点
			}
			else
			{
				CQuadRangle quadRange = pDoc->GetQuadRangle();
				m_nResult = m_track.GetResult(point,quadRange,this);//得到HIT点
			}
			
			if (8 == m_nResult)
			{
				SetCursor(m_hDrag);
			}
			else if (m_nResult  >=  0 && m_nResult < 8)
			{
				switch ( m_nResult )
				{
				case 0:
				case 2:
					SetCursor(m_hCursorHVDrag2);
					break;
				case 1:
				case 3:
					SetCursor(m_hCursorHVDrag1);
					break;
				case 4:
				case 6:
					SetCursor(m_hCursorVerDrag);
					break;
				case 5:
				case 7:
					SetCursor(m_hCursorHorDrag);
					break;
				}
			}
			else
			{
				SetCursor(m_hCursorClick);
			}
			goto END_MOUSE_MOVE;
		}
		
		for(int i = 0; i < m_nMeasureLineCount; i++)
		{
			CMeasureLine line = m_arrayMeasureLine[i];
			if ( line.m_nType == CMeasureLine::LEFT_TO_RIGHT )
			{
				double dbx = 0;
				double dby = line.m_dbPos;
				PhysicToLogic(dbx, dby);
				LONG ly = (LONG)dby;
				if ( point.y > (ly-5) && point.y < (ly+5) )
				{
					SetCursor(m_hCursorMLineV);
					m_bCanMoveMeasureLine = TRUE;
					m_bCanSetLinePos = TRUE;
					m_nMeasureLineIndex = i;
				}
			}
			else
			{
				double dbx = line.m_dbPos;
				double dby = 0;
				PhysicToLogic(dbx, dby);
				LONG lx = (LONG)dbx;
				if ( point.x > (lx-5) && point.x < (lx+5) )
				{
					SetCursor(m_hCursorMLineH);
					m_bCanMoveMeasureLine = TRUE;
					m_bCanSetLinePos = TRUE;
					m_nMeasureLineIndex = i;
				}
			}
		}
	}
	else if  ( m_nDrawTool == DRAWMODEL_DISTORTION )
	{
		if ( pDoc->HaveSelectObject() )
		{
			CQuad quad = pDoc->GetLimit();
			int nResult = m_track.GetDistortionPos(point,quad,this);
			switch(nResult) 
			{
				case 0:
				case 1:
				case 2:
				case 3:
					SetCursor(m_hCursorRotateDrag);
					break;
				case 4:
				case 6:
					SetCursor(m_hCursorLRDrag);
					break;
				case 5:
				case 7:
					SetCursor(m_hCursorTBDrag);
					break;
			}
		}
	}

END_MOUSE_MOVE:
	double x = point.x;
	double y = point.y;
	LogicToPhysic(x, y);

	// 铺助点
	if ( g_bEnableAssist && m_nDrawTool == DRAWMODEL_HAND  )
	{
		CDC* pDC = GetDC();
		double dbX, dbY;
		g_ObjDrawAssist.DrawAnchor(pDC);
		g_ObjDrawAssist.m_anchorPoint.bEnable = FALSE;
		if ( g_ObjDrawAssist.LookUp(x, y, dbX, dbY))
		{
			double adjustX = dbX;
			double adjustY = dbY;
			PhysicToLogic(adjustX, adjustY);
			g_ObjDrawAssist.m_anchorPoint.x = int(adjustX);
			g_ObjDrawAssist.m_anchorPoint.y = int(adjustY);
			g_ObjDrawAssist.DrawAnchor(pDC);
			x = dbX;
			y = dbY;
		}
	}
	((CMainFrame*)AfxGetApp()->GetMainWnd())->SetMousePosInfo(x, y);
}


void CHLView::OnDrawRectangle() 
{
	// TODO: Add your command handler code here
	if ( m_nDrawTool == DRAWMODEL_RECTANGLE )
	{
		CSetRectangleDlg dlg;

		dlg.m_nType	 =  ::g_sys.GetProfileInt(_T("RECTANGLE"),_T("TYPE"),0);
		dlg.m_dbValue	 =  ::g_sys.GetProfileDouble(_T("RECTANGLE"),_T("VALUE"),1.0);

		if ( dlg.DoModal() == IDOK )
		{
			::g_sys.WriteProfileInt(_T("RECTANGLE"),_T("TYPE"),dlg.m_nType);
			::g_sys.WriteProfileDouble(_T("RECTANGLE"),_T("VALUE"),dlg.m_dbValue);
		}
	}

	m_nDrawTool  = DRAWMODEL_RECTANGLE;
	m_hCursorCur = m_hCursorRect;
    
	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		if ( pDoc )
			m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	}
}


void CHLView::OnUpdateDrawRectangle(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc  !=  NULL )
	{
		pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetCheck(m_nDrawTool == DRAWMODEL_RECTANGLE);
}

void CHLView::OnUpdatePos(CCmdUI *pCmdUI)
{
	CRect rect;
	GetClientRect(&rect);
	if ( !rect.PtInRect(m_ptCur) )
		return;

 	ViewDPtoLP(&m_ptCur);
	CDot dot(m_ptCur);
	
	CString str;
	str.Format(_T("%.3f,%.3f"),dot.x,dot.y);
	pCmdUI->Enable(TRUE);
	pCmdUI->SetText(str);
}

void CHLView::DrawTracker(XDC *pDC)
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	CQuad quad  = pDoc->GetTrackerQuad();
	BOOL  bLock = pDoc->GetSelObjLockFlg();
	if ( quad.IsEmpty() )
	{
		pDoc->m_QuadList.RemoveAll();
		return;
	}

	m_track.DrawSelTraker(pDC, quad, this, pDoc->m_QuadList,bLock);
}

void CHLView::DrawFrame(XDC *pDC)
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	BOOL  bLock = pDoc->GetSelObjLockFlg();
	CQuadRangle quad  = pDoc->GetQuadRangle();
	m_track.DrawFrame(pDC, quad, this, bLock);
}

void CHLView::DrawDistortion (XDC *pDC)
{
	CHLDoc *pDoc =  GetDocument();
	if ( pDoc == NULL )
		return;

	CQuad quad  = pDoc->GetTrackerQuad();
	BOOL  bLock = pDoc->GetSelObjLockFlg();

	if ( quad.IsEmpty() )
		return;
	
	if ( bLock )
	{ 
		m_nDrawTool  = DRAWMODEL_CLICK;
		m_hCursorCur = m_hCursorClick;
		m_track.DrawSelTraker(pDC, quad, this, pDoc->m_QuadList,bLock);
		return;
	}

	m_track.DrawDistortion(pDC, quad, this);
}

void CHLView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch ( lHint )
	{
	case 0:
		{
			//无任何参数时，更新全部
			InvalidateRect(NULL);
			break;
		}
	case UPDATE_TRACKER_VIEW:
		{
			if ( DRAWMODEL_CLICKFRAME == m_nDrawTool ) 
			{
				Invalidate();
			}
			else
			{
				//更新显示跟踪框部分
				CRect rectTrue;
				m_track.GetTrueRect(rectTrue);
				InvalidateRect((CRect)rectTrue);
			}
			break;
		}
	case UPDATE_TRACKER_PROPERTY:
		{
			//改变跟踪器显示风格
			break;
		}
	case UPDATE_QUAD:
		{
			CQuad quad = *((CQuad*)pHint);
			PhysicToLogic(quad);
			InvalidateRect((CRect*)&quad);//no use
			break;
		}
	case UPDATE_LOGIC:
		{			
			//更新显示指定逻辑区域,并设置跟踪框
			CQuad quad = *((CQuad*)pHint);
			m_track.SetPhysicQuad(this,quad);

			CRect rectTrue;
			m_track.GetTrueRect(rectTrue);
			InvalidateRect(rectTrue);
			break;
		}
	case UPDATE_DEVICE:
		{
			//更新显示指定设备区域
			InvalidateRect((CRect*)pHint);
			break;
		}
	}
}

void CHLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	switch(nChar)
	{
		case VK_DELETE:
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				if ( pDoc->m_b3D )
				{
					pDoc->m_chain3d.Delete();
					pDoc->SetModifiedFlag();
					return;
				}
				
				if ( m_nDrawTool == DRAWMODEL_CLICK || 
					m_nDrawTool == DRAWMODEL_CLICKFRAME ||
					m_nDrawTool == DRAWMODEL_DISTORTION )
				{
					//删除对象
					pDoc->Delete();
					m_track.ClearAll();
					pDoc->ReLoadStrokeList();
					m_nDrawTool = DRAWMODEL_CLICK;
					m_hCursorCur = m_hCursorClick;
				}
				else if ( m_nDrawTool == DRAWMODEL_DRAG )
				{
					short  nVKey = GetKeyState(VK_SHIFT);//short = 2 * BYTE
					if ( nVKey & SHIFTED )
					{
						//将结点断开一分为二
						double dbPoint = g_SysCfg.m_dbOneToTwo;
						pDoc->SplitDot(this,dbPoint);
						Invalidate();
					}
					else
					{
						//删结点
						pDoc->DelDot(this);
						m_track.ClearDot();
						Invalidate();
					}
					break;
				}
				break;
			}
		case VK_DOWN:
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;

				double micro = g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro = quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,0,-micro);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_UP:
			{	
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				double micro = g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro = quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,0,micro);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_LEFT:
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;

				double micro = g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro = quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,-micro,0);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_RIGHT:
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				double micro = g_SysCfg.m_dbMicro;
				CQuad quad(CDot(0,0),micro,micro);
				micro = quad.Height();
				pDoc->Drag(CDot(0,0),1.0,1.0,micro,0);
				pDoc->SetTrackerQuad(pDoc->GetLimit());
				break;
			}
		case VK_F5:
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) )
					break;
				
				CHLApp* pApp = (CHLApp*)AfxGetApp();
				if ( g_bIsRedPreview )
				{
					g_bIsRedPreview  =  FALSE;
					pApp->m_RedPreView.StopPreview();
				}
				
				if ( g_bIsSelMark ) 
				{
					pApp->m_SelMark.StopSelMark();
				}
				
				if (g_bIsFastMark) 
				{
					pApp->m_FastMark.StopFastMark();
				}
				
				//设置等待光标
				AfxGetApp()->BeginWaitCursor();
				
				//刷新外部参数
				g_sys.FlashCardPara();
				
				Sleep(100);
				//设置等待光标
				AfxGetApp()->EndWaitCursor();
				break;
			}
		case VK_J:
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				short  nVKey = GetKeyState(VK_CONTROL);//short = 2 * BYTE
				if (nVKey & SHIFTED)
				{
					pDoc->JoinPoint(this);
					Invalidate();
				}
				break;
			}
		case VK_A:
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				short  nVKey = GetKeyState(VK_SHIFT);
				if ( nVKey & SHIFTED )
				{
					m_track.InverDrawDot(this);
					double dbMoveX = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
					pDoc->MoveAccurate(-dbMoveX, 0);
					pDoc->SetTrackerQuad(pDoc->GetLimit());
				}
				else
				{
					OnSelAllStroke();
				}
				break;
			}
		case VK_W: // 上移
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				short  nVKey = GetKeyState(VK_SHIFT);
				if (nVKey & SHIFTED)
				{
					m_track.InverDrawDot(this);
					double dbMoveY = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
					pDoc->MoveAccurate(0, dbMoveY);
					pDoc->SetTrackerQuad(pDoc->GetLimit());
				}
				break;
			}
		case VK_S: // 下移
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				short  nVKey = GetKeyState(VK_SHIFT);
				if ( nVKey & SHIFTED )
				{
					m_track.InverDrawDot(this);
					double dbMoveY = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
					pDoc->MoveAccurate(0, -dbMoveY);
					pDoc->SetTrackerQuad(pDoc->GetLimit());
				}
				else
				{
					// 设置为曲线的起点
					OnSetCurDotToStart();
				}
				break;
			}
		case VK_D: // 右移
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				short  nVKey = GetKeyState(VK_SHIFT);
				if ( nVKey & SHIFTED )
				{
					m_track.InverDrawDot(this);
					double dbMoveX = MyPrecision(g_SysCfg.m_dbDotMoveStepLen);
					pDoc->MoveAccurate(dbMoveX, 0);
					pDoc->SetTrackerQuad(pDoc->GetLimit());
				}
				else
				{
					// 删除选中节点
					OnDeldot();
				}
				break;
			}
		case VK_C: // 连接选中节点
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				OnJoindot();
				break;
			}
		case VK_X: // 断点
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;
				
				OnSplitdot();
				break;
			}
		case VK_V: // 导角
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;

				OnKillAngle();
				break;
			}
		case VK_F: // 滤点
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;

				OnFilterPoint();
				break;
			}
		case VK_R: // 反序
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;

				OnReverseStroke();
				break;
			}
		case VK_M: // 精确移动
			{
				if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
					break;

				OnMoveaccu();
				break;
			}
		case VK_0 :
		case VK_1 :
		case VK_2 :
		case VK_3 :
		case VK_4 :
		case VK_5 :
		case VK_6 :
		case VK_7 :
		case VK_8 :
		case VK_9 :
			{
				short  nVKey = GetKeyState(VK_CONTROL);
				if (nVKey & SHIFTED)
				{
					pDoc->SetGroupNum(nChar);
				}
				else
				{
					pDoc->SelGroupByNum(nChar, this);
				}
				break;
			}
		case VK_TAB:
			if ( pDoc->m_b3D )
			{
				short  nVKey = GetKeyState(VK_SHIFT);
				pDoc->m_chain3d.Select(!(nVKey&SHIFTED));
				pDoc->m_chain3d.Draw();
				return;
			}

		case VK_N: pDoc->SelSwitich(TRUE); break;
		case VK_L: pDoc->SelSwitich(FALSE); break;

		default:
			{
				break;
			}
	}
	
	CZoomView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//
void CHLView::OnZoomSelect() 
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	CQuad quad   = pDoc->GetLimit();
	if ( quad.IsNull() )
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		MessageBox(str,NULL,MB_ICONWARNING);
	}
	else
	{	
		double x,y,ratiox,ratioy;
		x = (quad.left+quad.right)/2;
		y = (quad.top+quad.bottom)/2;
		ratiox = (quad.right-quad.left);
		ratioy = (quad.top-quad.bottom);
		DoZoomIn(ratiox,ratioy,x,y);
	}
}

//
void CHLView::OnZoomAll() 
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	CQuad quad = pDoc->GetAllLimit();
	if (quad.IsNull())
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		MessageBox(str,NULL,MB_ICONWARNING);
	}
	else
	{
		double x,y,ratiox,ratioy;
		x = (quad.right+quad.left)/2;
		y = (quad.top+quad.bottom)/2;
		ratiox = quad.right-quad.left;
		ratioy = quad.top-quad.bottom;
 		DoZoomIn(ratiox,ratioy,x,y);
	}
}

void CHLView::OnDrawText() 
{
	m_nDrawTool  = DRAWMODEL_TEXT;
	m_hCursorCur = m_hCursorText;	
	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		m_track.m_rect = (CRect)pDoc->GetTrackerQuad();
	}
}


void CHLView::OnUpdateDrawText(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	if (m_nDrawTool == DRAWMODEL_TEXT)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

//
void CHLView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	NotifyZoom();
	CZoomView::OnHScroll(nSBCode, nPos, pScrollBar);
}

//
void CHLView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	NotifyZoom();
	CZoomView::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CHLView::OpenSelectFile(CHLDoc *pDoc,TCHAR* pszFilter,CString &strPath, CString& strFileExt,BOOL &bCenter,BOOL &bOptimize )
{
	if ( pDoc == NULL )
		return FALSE;

	pDoc->m_pTracker = &m_track;

	CString strTitle;
	strTitle.LoadString(IDS_TITLE_FILEIMPORT);
	CString strLoad = ::g_sys.GetIniString(_T("FILEIMPORT"),_T("PATH"),_T("C:\\"));

	CDlgImport dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,pszFilter);
	dlg.m_ofn.lpstrInitialDir = strLoad;
	dlg.m_ofn.lpstrTitle = strTitle;

	if ( dlg.DoModal() != IDOK )
		return FALSE;
	
	strLoad = dlg.GetPathName();
	int nFind = strLoad.ReverseFind(_T('\\'));
	strLoad = strLoad.Left(nFind+1);
	g_sys.WriteIniString(_T("FILEIMPORT"),_T("PATH"),strLoad.GetBuffer(strLoad.GetLength()));
	strLoad.ReleaseBuffer();

	strPath = dlg.GetPathName();
	strFileExt = dlg.GetFileExt();
	strFileExt.MakeUpper();	
	bCenter = dlg.m_bCenter;
	bOptimize = dlg.m_bPltBetter;

	g_VectorMgr.m_nAiType = dlg.m_nAiType;
	g_VectorMgr.m_nAiFillType = dlg.m_nAiFillType;
	g_VectorMgr.m_dbAiFillDis = dlg.m_dbAiFillDis;
	
	return TRUE;
}


void CHLView::OnFileImport() 
{
	TCHAR szFilter[] = 
		_T("PLT File(*.plt)|*.plt|DXF File(*.dxf)|*.dxf|GCode File(*.NC)|*.nc|Adobe Illustrator 8.0(*.AI)|*.ai|JPG File(*.jpg/*.jpeg)|*.jpg;*.jpeg|BMP File(*.bmp)|*.bmp|GIF File(*.gif)|*.gif|PNG File(*.png)|*.png|ICO File(*.ico)|*.ico|TIF File(*.tif)|*.tif|TGA File(*.tga)|*.tga|PCX File(*.pcx)|*.pcx|DST File(*.dst)|*.dst|All Files(*.*)|*.*||");
	
	CString strFileExt;
	CString strFileName;
	BOOL bCenter = FALSE;
	BOOL bOptimize = FALSE;
	
	CHLDoc* pDoc = GetDocument();
	if ( !OpenSelectFile(pDoc,szFilter,strFileName,strFileExt,bCenter,bOptimize) )
		return;

	AfxGetApp()->BeginWaitCursor();//设置等待光标
	CLay lay  = pDoc->GetLay();
	CLayer ly = lay.GetCurLayer();
	CProperty pty = ly.GetProperty();

	if ( 0 == strFileExt.CompareNoCase(_T("PLT")) || 0 == strFileExt.CompareNoCase(_T("DXF")) || 0 == strFileExt.CompareNoCase(_T("NC")) || 0 == strFileExt.CompareNoCase(_T("AI")) || 0 == strFileExt.CompareNoCase(_T("DST")))
	{	
		CTree *pTreeAdd = new CTree();
		if ( NULL == pTreeAdd )
			return;

		if ( g_VectorMgr.Decode( strFileName, strFileExt, pTreeAdd, pty, bCenter, bOptimize ) )
		{
			pDoc->AddTree(pTreeAdd);
		}
		else
		{
			delete pTreeAdd;
		}
	}
	else
	{
		CNode *pNode = new CNodeDib(strFileName);
		if ( pNode == NULL )
			return;

		pNode->SetProperty(PTY_INIT,pty, &m_DC);		
		if (pNode->Track(NULL,CPoint(0,0)))
		{
			((CNodeDib*)pNode)->m_nDibType = 0;
			((CNodeDib*)pNode)->DataChange();
			pNode->InitQuadRange();
			pDoc->AddNode(pNode, NODE_BMP);
		}
		else
		{
			delete pNode;
			pNode = NULL;
		}
	}
	//结束等待光标
	AfxGetApp()->EndWaitCursor();
	pDoc->SetTrackerQuad(pDoc->GetLimit());
	Invalidate();

	pDoc->SetObjProp();
	DispSelObjProp();
}

//
void CHLView::OnFileImportVector() 
{
	CString strFileExt;
	CString strFileName;
	BOOL bCenter = FALSE;
	BOOL bOptimize = FALSE;
	TCHAR szFilter[] = _T("PLT File(*.PLT)|*.PLT|DXF File(*.dxf)|*.DXF|Adobe Illustrator(*.AI)|*.AI|DST File(*.dst)|*.dst||");
	
	CHLDoc* pDoc = GetDocument();
	if ( !OpenSelectFile(pDoc,szFilter,strFileName,strFileExt,bCenter,bOptimize) )
		return;

	//设置等待光标
	AfxGetApp()->BeginWaitCursor();
	CLay lay  = pDoc->GetLay();
	CLayer ly = lay.GetCurLayer();
	CProperty pty = ly.GetProperty();
	pDoc->m_pTracker = &m_track;

	if ( 0 == strFileExt.CompareNoCase(_T("PLT")) || 0 == strFileExt.CompareNoCase(_T("DXF")) || 0 == strFileExt.CompareNoCase(_T("AI")) || 0 == strFileExt.CompareNoCase(_T("DST")) )
	{	
		CTree *pTreeAdd = new CTree();
		if ( NULL == pTreeAdd )
			return;

		if ( g_VectorMgr.Decode(strFileName, strFileExt, pTreeAdd, pty, bCenter, bOptimize) )
		{
			pDoc->AddTree(pTreeAdd);
		}
		else
		{
			delete pTreeAdd;
		}
	}
	else
	{
		AfxMessageBox(IDS_FILE_FORMAT);
	}

	//结束等待光标
	AfxGetApp()->EndWaitCursor();
	pDoc->SetTrackerQuad(pDoc->GetLimit());
	Invalidate();

	pDoc->SetObjProp();
	DispSelObjProp();
}


void CHLView::OnFileImportBmp() 
{
	CString strFileExt;
	CString strFileName;
	BOOL bCenter = FALSE;
	BOOL bOptimize = FALSE;
	TCHAR szFilter[] =  _T("JPG File(*.jpg/*.jpeg)|*.jpg;*.jpeg|BMP File(*.bmp)|*.bmp|GIF File(*.gif)|*.gif|PNG File(*.png)|*.png|ICO File(*.ico)|*.ico|TIF File(*.tif)|*.tif|TGA File(*.tga)|*.tga|PCX File(*.pcx)|*.pcx|");
	
	CHLDoc* pDoc = GetDocument();
	if ( !OpenSelectFile(pDoc,szFilter,strFileName,strFileExt,bCenter,bOptimize) )
		return;

	//设置等待光标
	AfxGetApp()->BeginWaitCursor();
	CLay lay  = pDoc->GetLay();
	CLayer ly = lay.GetCurLayer();
	CProperty pty = ly.GetProperty();
	
	CNode *pNode = new CNodeDib(strFileName);
	if ( pNode == NULL )
		return;

	pNode->SetProperty(PTY_INIT,pty, &m_DC);		
	if (pNode->Track(NULL,CPoint(0,0)))
	{
		((CNodeDib*)pNode)->m_nDibType = 0;
		((CNodeDib*)pNode)->DataChange();
		pNode->InitQuadRange();
		pDoc->AddNode(pNode, NODE_BMP);
	}
	else
	{
		delete pNode;
		AfxMessageBox(IDS_FILE_FORMAT);
	}

	//结束等待光标
	AfxGetApp()->EndWaitCursor();
	pDoc->SetTrackerQuad(pDoc->GetLimit());
	Invalidate();

	pDoc->SetObjProp();
	DispSelObjProp();
}


void CHLView::OnFileImportNc() 
{
	CString strFileExt;
	CString strFileName;
	BOOL bCenter = FALSE;
	BOOL bOptimize = FALSE;
	TCHAR szFilter[] = _T("GCode File(*.NC)|*.nc|");
	
	CHLDoc* pDoc = GetDocument();
	if ( !OpenSelectFile(pDoc,szFilter,strFileName,strFileExt,bCenter,bOptimize) )
		return;

	//设置等待光标
	AfxGetApp()->BeginWaitCursor();
	CLay lay  = pDoc->GetLay();
	CLayer ly = lay.GetCurLayer();
	CProperty pty = ly.GetProperty();

	if ( 0 == strFileExt.CompareNoCase(_T("NC")) )
	{
		CTree *pTreeAdd = new CTree();
		if ( NULL == pTreeAdd )
			return;

		if ( g_VectorMgr.Decode(strFileName, strFileExt, pTreeAdd, pty, bCenter, bOptimize ) )
		{
			pDoc->AddTree(pTreeAdd);
		}
		else
		{
			delete pTreeAdd;
			pTreeAdd = NULL;
		}
	}
	else
	{
		AfxMessageBox(IDS_FILE_FORMAT);
	}

	//结束等待光标
	AfxGetApp()->EndWaitCursor();
	pDoc->SetTrackerQuad(pDoc->GetLimit());
	Invalidate();

	pDoc->SetObjProp();
	DispSelObjProp();
}


void CHLView::OnDrawGrid()
{
	if ( m_nDrawTool == DRAWMODEL_GRID )
	{
		CDrawGridDlg dlg;
		dlg.m_nCol = g_SysCfg.m_nGridCol;
		dlg.m_nRow = g_SysCfg.m_nGridRow;
		if ( dlg.DoModal() == IDOK )
		{
			 g_SysCfg.m_nGridCol = dlg.m_nCol;
		     g_SysCfg.m_nGridRow = dlg.m_nRow;
			
			 g_sys.WriteSysCfg();
		}
	}
	
	m_nDrawTool = DRAWMODEL_GRID;
	m_hCursorCur = m_hCursorGrid;
	
	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		if ( pDoc )
			m_track.m_rect = (CRect)pDoc->GetTrackerQuad();
	}
}


void CHLView::OnUpdateDrawGrid(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = GetDocument();
	if ( ! pDoc  )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	if ( m_nDrawTool == DRAWMODEL_GRID )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CHLView::OnDrawPolygon() 
{
	if ( m_nDrawTool == DRAWMODEL_POLYGON )
	{
		CDrawPolygonDlg dlg;
		/////////
		dlg.m_nPolygon = g_SysCfg.m_nPolygon;
		dlg.m_nNum     = g_SysCfg.m_nPolyNum;
		dlg.m_nCross   = g_SysCfg.m_nPolyCross;
		dlg.m_nStar    = g_SysCfg.m_nPolyStar;
		if ( dlg.DoModal() == IDOK )
		{
			g_SysCfg.m_nPolygon   = dlg.m_nPolygon;
		    g_SysCfg.m_nPolyNum   = dlg.m_nNum;
		    g_SysCfg.m_nPolyCross = dlg.m_nCross;
		    g_SysCfg.m_nPolyStar  = dlg.m_nStar;
			/////
			g_sys.WriteSysCfg();
		}
	}
	
	m_nDrawTool = DRAWMODEL_POLYGON;
	m_hCursorCur = m_hCursorPoly;
	
	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();

		if ( pDoc != NULL )
			m_track.m_rect = (CRect)pDoc->GetTrackerQuad();
	}
}


void CHLView::OnUpdateDrawPolygon(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = GetDocument();
	if ( ! pDoc  )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	if ( m_nDrawTool == DRAWMODEL_POLYGON )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}




void CHLView::OnDrawEllipse()
{
	if ( m_nDrawTool == DRAWMODEL_ELLIPSE )
	{
		CEllipSetDlg dlg;
		dlg.m_dbRuler = g_SysCfg.m_dbDrawEllipSet;
		dlg.DoModal();
	}
	
	m_nDrawTool  = DRAWMODEL_ELLIPSE;
	m_hCursorCur = m_hCursorEllipse;
	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		if ( pDoc != NULL )
			m_track.m_rect = (CRect)pDoc->GetTrackerQuad();
	}
}


void CHLView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = GetDocument();
	if ( ! pDoc  )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	pCmdUI->SetCheck(m_nDrawTool == DRAWMODEL_ELLIPSE);
}


void CHLView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if ( bActivate )
	{
		CMainFrame *pMainWnd = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		if ( pMainWnd )
		{
			pMainWnd->m_PropDlg.OnDocChange();
			pMainWnd->m_PropDlg.UpdateProperty();
			pMainWnd->m_ObjectListDlg.ObjListLoad();
			
			if ( g_sys.m_bEnableUpdate == TRUE )
				((CHLView*)pActivateView)->DispSelObjProp();
			else
				g_sys.m_bEnableUpdate = TRUE;
		}
	}
	else
	{
		CMainFrame *pMainWnd = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		if ( pMainWnd )
		{
			pMainWnd->m_FillEditDlg.m_TextEditDlg.OnNewDocment();
			pMainWnd->m_ObjectListDlg.ObjListLoad();
		}
	}
	
	CZoomView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CHLView::OnDrawCodebar() 
{
	m_nDrawTool = DRAWMODEL_CODEBAR;
	m_hCursorCur = m_hCursorBar;
	if (m_bRotate)
	{
		m_bRotate = FALSE;
		m_track.m_rect = (CRect)GetDocument()->GetTrackerQuad();
	}
}


void CHLView::OnUpdateDrawCodebar(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = GetDocument();
	if ( ! pDoc  )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	if ( m_nDrawTool == DRAWMODEL_CODEBAR )
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


//
void CHLView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CHLDoc *pDoc = GetDocument();
	if ( ! pDoc  )
		return;

	if ( pDoc->m_b3D )
	{
		SetCursor(m_hCursorViewRotate);
		Track3D(point);
		SetCursor(m_hCursorClick);
		return;
	}

	if ( GetZoomMode() == MODE_ZOOMOPER )
	{
		OnDrawClick();
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = DRAWMODEL_CLICK;
		m_hCursorCur = m_hCursorClick;
		Invalidate();
		return;
	}
	
	if ( GetZoomMode() == MODE_ZOOMSEL )
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = DRAWMODEL_CLICK;
		m_hCursorCur = m_hCursorClick;
		Invalidate();
		return;
	}
	
	if ( m_nDrawTool  !=  DRAWMODEL_CLICK && m_nDrawTool  !=  DRAWMODEL_DRAG )
	{
		m_nDrawTool	 =  DRAWMODEL_CLICK;
		m_hCursorCur =  m_hCursorClick;
		Invalidate();
		CZoomView::OnRButtonDown(nFlags, point);
		return;
	}
	
	if ( m_nDrawTool == DRAWMODEL_CLICK && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		if ( pDoc->GetSelObjLockFlg() )
			return;
		
		int index = pDoc->GetMenuIndex();
		CMenu menu,*pMenu=NULL;
		menu.LoadMenu(IDR_POP);
		pMenu = menu.GetSubMenu(index);
	
		if ( pMenu )
		{
			HCURSOR hcursor;
			hcursor = m_hCursorCur;
			m_hCursorCur = ::LoadCursor(NULL,IDC_ARROW);
			
			ClientToScreen(&point);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
			m_hCursorCur = hcursor;
			SetFocus();
			return;
		}
	}

	if ( m_nDrawTool == DRAWMODEL_DRAG && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		if (!m_track.ListEmpty())
		{
			CMenu menu;
			menu.LoadMenu(IDR_POP);
			CMenu* pMenu = menu.GetSubMenu(7);
			if ( pMenu )
			{
				HCURSOR hcursor;
				hcursor = m_hCursorCur;
				m_hCursorCur = ::LoadCursor(NULL,IDC_ARROW);
				
				ClientToScreen(&point);
				pMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
				m_hCursorCur = hcursor;
				SetFocus();
				return;
			}
		}
	}
	
	CZoomView::OnRButtonDown(nFlags, point);
}


void CHLView::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	if ( pDoc->m_b3D )
	{
		pDoc->m_chain3d.RawView(0);
		pDoc->m_chain3d.Draw();
		return;
	}
	
	if ( GetZoomMode() == MODE_ZOOMOPER )
	{
		OnDrawClick();
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = DRAWMODEL_CLICK;
		m_hCursorCur = m_hCursorClick;
		return;
	}
	
	if ( GetZoomMode() == MODE_ZOOMSEL )
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = DRAWMODEL_CLICK;
		m_hCursorCur = m_hCursorClick;
		return;
	}
	
	switch(m_nDrawTool)
	{
	case DRAWMODEL_CLICK:
		{
			CZoomView::OnLButtonDblClk(nFlags, point);
			
			if (m_bCanSetLinePos)
			{
				Invalidate();
				m_bCanSetLinePos = FALSE;
				break;
			}
			
			if ( pDoc->IsSelectedObj()) {
				m_nDrawTool = DRAWMODEL_DISTORTION;
				Invalidate();
			}
			break;
		}
	case DRAWMODEL_DRAG:
		{
			if ( m_track.HitValidPoint(point,this) )
			{
				pDoc->AddPoint(point,this);
				Invalidate();
			}
			else
			{
				OnDrawClick();
			}
			break;
		}
	case DRAWMODEL_SORT_OBJECT_LIST:
		{
			m_nDrawTool = DRAWMODEL_CLICK;
			break;
		}
	default:
		{
			break;
		}
	}
}



void CHLView::TrackRotate(CPoint OrgPoint, BOOL bShift, BOOL bControl)
{
	CWnd *pWin = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CDC *pDC   = GetDC();
    CDot pointLT,pointRB;

    double dbRotateAngle(0);
	double dbOrgAngle(0.0),dbNewAngle(0.0);
	int cx(0),cy(0);
	CPoint CurMousePoint;
	StrokeList *pPreviewStrokeList = NULL;
	
	pPreviewStrokeList = new StrokeList;
	if ( NULL == pPreviewStrokeList ) 
	{
		return;
	}
	
	CHLDoc *pDoc    = GetDocument();
	CQuad TrackRect = pDoc->GetQuadTracker();
	
	TrackRect.Normal();
	PhysicToLogic(TrackRect.left, TrackRect.top);
	PhysicToLogic(TrackRect.right, TrackRect.bottom);
	CDot CenterPoint = TrackRect.GetBaseDot(BASE_MM);
	PhysicToLogic(CenterPoint.x, CenterPoint.y);
	CPoint pointCC((int)CenterPoint.x,(int)CenterPoint.y);
	
    cx = OrgPoint.x-pointCC.x;
	cy = OrgPoint.y-pointCC.y;

	dbOrgAngle = CalAngle(cx,cy);
	dbNewAngle = dbOrgAngle;

	CStroke stroke,strokeTemp;
	stroke.Add(CDot(TrackRect.left,TrackRect.top));
	stroke.Add(CDot(TrackRect.right,TrackRect.top));
	stroke.Add(CDot(TrackRect.right,TrackRect.bottom));
	stroke.Add(CDot(TrackRect.left,TrackRect.bottom));
	stroke.Add(CDot(TrackRect.left,TrackRect.top));
	
	SetCapture();
	
	BOOL bExit = FALSE;
	while (!bExit)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		switch (msg.message)
		{
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			bExit = TRUE;
			m_RotateRectStroke.RemoveAllPoint();
			break;
		case WM_LBUTTONDBLCLK:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			break;
		case WM_RBUTTONDBLCLK:
			break;
		case WM_MOUSEWHEEL:
            break;
		case WM_MOUSEMOVE:
			{
				CQuad FlashRect;
				FlashRect = pDoc->GetQuadTracker();
				PhysicToLogic(FlashRect.left, FlashRect.top);
				PhysicToLogic(FlashRect.right, FlashRect.bottom);

		        PreviewRotateTracker(FlashRect);
				PreviewStroke(pPreviewStrokeList);
			
				EmptyStroke(pPreviewStrokeList);
				if (bShift)
					pDoc->GetSelectedStrokeList(pPreviewStrokeList);
			
				CurMousePoint.x = (int)(short)LOWORD(msg.lParam);
				CurMousePoint.y = (int)(short)HIWORD(msg.lParam);
				cx = CurMousePoint.x-pointCC.x;
				cy = CurMousePoint.y-pointCC.y;

				dbNewAngle = CalAngle(cx,cy);
				strokeTemp = stroke;
				strokeTemp.Rotate(CenterPoint,(dbNewAngle-dbOrgAngle));
				RotatePreviewStrokeList(pPreviewStrokeList,CenterPoint,dbNewAngle-dbOrgAngle);
				m_RotateRectStroke.CopyStroke(&strokeTemp);
				
				CQuad dbRect = strokeTemp.CalLimit();
			    pDoc->SetTrackerQuad(dbRect); 
				PreviewRotateTracker(dbRect);
				PreviewStroke(pPreviewStrokeList);

                
                CString strInfo,strFormat;
				double dbAngleTemp;
				dbAngleTemp = dbNewAngle-dbOrgAngle;
				if (dbAngleTemp<-180)
					dbAngleTemp += 360;
				if (dbAngleTemp >= 180)
					dbAngleTemp -= 360;
							
				break;
			}
		default:
			break;
		}
	}

	ReleaseDC(pDC);
	ReleaseCapture();

	TrackRect.Union(pDoc->GetQuadTracker());
	CRect rectflash((int)TrackRect.left,(int)TrackRect.top,(int)TrackRect.right,(int)TrackRect.bottom);
	rectflash.NormalizeRect();
	rectflash.InflateRect(14,14);

    InvalidateRect(rectflash);
	
	dbRotateAngle = dbNewAngle-dbOrgAngle;
	
	if (dbRotateAngle<-180)
	{
		dbRotateAngle += 360;
	}
	
	if (dbRotateAngle >= 180)
	{
		dbRotateAngle -= 360;
	}
	
	pDoc->Rotate(CenterPoint,dbRotateAngle);

	this->EmptyStroke(pPreviewStrokeList);

	if (pPreviewStrokeList)
	{
		delete pPreviewStrokeList;
		pPreviewStrokeList = NULL;
	}

	pWin->SendMessage(WM_SETMESSAGESTRING,0,(LPARAM)(LPCSTR)"");
}


double CHLView::CalAngle(double cx, double cy)
{
	double r;
	double angle;
    
	if (cx == 0&&cy == 0)
		return 0;

	if (cy == 0)
	{
		if (cx>0)
			return 0;
		else
			return 180;
	}
	else if (cx == 0)
	{
		if (cy>0)
			return 90;
		else
			return 270;
	}

	r = sqrt(cx*cx+cy*cy);
	angle = 180.0*asin(cy/r)/PAI;
    
	
	if (cx<0&&cy>0)
	{//第二象限
      ASSERT(angle>0);
	  angle = 180-angle;
	}
    else if (cx<0&&cy<0)
	{//第三象限
      ASSERT(angle<0);
      angle = fabs(angle)+180;
	}
	else if (cx>0&&cy<0)
	{//第四象限
      ASSERT(angle<0);
	  angle = 360+angle;
	}
	
	return angle;
}

void CHLView::PreviewRotateTracker(CQuad rect)
{
    CDC *pDC = GetDC();
	if ( pDC == NULL )
		return;

	int mode = pDC->SetROP2(R2_NOTXORPEN);
	CQuad rectTracker = rect;
	CRect r((int)(rectTracker.left),(int)(rectTracker.top),(int)(rectTracker.right),(int)(rectTracker.bottom));
	r.NormalizeRect();

	CBrush brush;
	CPen pen,*poldpen;

	m_track.m_rect.left = m_track.m_rect.right = m_track.m_rect.top = m_track.m_rect.bottom = 0;
	r.InflateRect(8,8);
	
	CSize size(8,4);

	pen.CreatePen(PS_DOT,1,RGB(0,0,255));
	poldpen = pDC->SelectObject(&pen);

	pDC->MoveTo(r.left+size.cx,r.top+size.cx);
	pDC->LineTo(r.right-size.cx,r.top+size.cx);
	pDC->LineTo(r.right-size.cx,r.bottom-size.cx);
	pDC->LineTo(r.left+size.cx,r.bottom-size.cx);
	pDC->LineTo(r.left+size.cx,r.top+size.cx);
	pDC->SelectObject(poldpen);

	pen.DeleteObject();

	//画拖动旋转时的当前转动到的位置预缆框
    if (m_RotateRectStroke.GetSize() != 0)
	{
		CPoint point;
		CDot pointTemp;
		pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
		poldpen = pDC->SelectObject(&pen);
		for(int k = 0; k<4; k++)
		{
			pointTemp = m_RotateRectStroke.GetAt(k);
			point.x = (int)pointTemp.x;point.y = (int)pointTemp.y;
			if (k == 0)
			{
		        pDC->MoveTo(point);
			}
			else
			{
				pDC->LineTo(point);
			}
		}
		
		pointTemp = m_RotateRectStroke.GetAt(0);
		point.x = (int)pointTemp.x;point.y = (int)pointTemp.y;
		pDC->LineTo(point);
		pDC->SelectObject(poldpen);
		pen.DeleteObject();
	}

	pDC->SetROP2(mode);
	ReleaseDC(pDC);
}



void CHLView::InitIcon()
{
    //左上
	m_DragRotateIcon[0][0] = 0x7f;  
	m_DragRotateIcon[0][1] = 0x3f;
	m_DragRotateIcon[0][2] = 0x7e;
	m_DragRotateIcon[0][3] = 0x1fc;
	m_DragRotateIcon[0][4] = 0x398;
	m_DragRotateIcon[0][5] = 0x610;
	m_DragRotateIcon[0][6] = 0x600;
	m_DragRotateIcon[0][7] = 0x2c00;
	m_DragRotateIcon[0][8] = 0x3c00;
	m_DragRotateIcon[0][9] = 0x3f00;
	m_DragRotateIcon[0][10] = 0x3e00;
	m_DragRotateIcon[0][11] = 0x3c00;
	m_DragRotateIcon[0][12] = 0x3800;
	m_DragRotateIcon[0][13] = 0x3000;
	
	//上
	m_DragRotateIcon[1][0] = 0x00;
	m_DragRotateIcon[1][1] = 0x00;
	m_DragRotateIcon[1][2] = 0x00;
	m_DragRotateIcon[1][3] = 0x00;
	m_DragRotateIcon[1][4] = 0x618;
	m_DragRotateIcon[1][5] = 0x1e1e;
	m_DragRotateIcon[1][6] = 0x3fff;
	m_DragRotateIcon[1][7] = 0x1e1e;
	m_DragRotateIcon[1][8] = 0x618;
	m_DragRotateIcon[1][9] = 0x00;
	m_DragRotateIcon[1][10] = 0x00;
	m_DragRotateIcon[1][11] = 0x00;
	m_DragRotateIcon[1][12] = 0x00;
	m_DragRotateIcon[1][13] = 0x00;
	
	//右上
	m_DragRotateIcon[2][0] = 0x3f80;
	m_DragRotateIcon[2][1] = 0x3f00;
	m_DragRotateIcon[2][2] = 0x1f80;
	m_DragRotateIcon[2][3] = 0xfe0;
	m_DragRotateIcon[2][4] = 0x670;
	m_DragRotateIcon[2][5] = 0x218;
	m_DragRotateIcon[2][6] = 0x18;
	m_DragRotateIcon[2][7] = 0x0d;
	m_DragRotateIcon[2][8] = 0x0f;
	m_DragRotateIcon[2][9] = 0x03f;
	m_DragRotateIcon[2][10] = 0x1f;
	m_DragRotateIcon[2][11] = 0x0f;
	m_DragRotateIcon[2][12] = 0x07;
	m_DragRotateIcon[2][13] = 0x03;
	
	//右
	m_DragRotateIcon[3][0] = 0x80;
	m_DragRotateIcon[3][1] = 0x1c0;
	m_DragRotateIcon[3][2] = 0x1c0;
	m_DragRotateIcon[3][3] = 0x3e0;
	m_DragRotateIcon[3][4] = 0x3e0;
	m_DragRotateIcon[3][5] = 0x80;
	m_DragRotateIcon[3][6] = 0x80;
	m_DragRotateIcon[3][7] = 0x80;
	m_DragRotateIcon[3][8] = 0x80;
	m_DragRotateIcon[3][9] = 0x3e0;
	m_DragRotateIcon[3][10] = 0x3e0;
	m_DragRotateIcon[3][11] = 0x1c0;
	m_DragRotateIcon[3][12] = 0x1c0;
	m_DragRotateIcon[3][13] = 0x80;
	
	//右下
	m_DragRotateIcon[4][0] = 0x03;
	m_DragRotateIcon[4][1] = 0x07;
	m_DragRotateIcon[4][2] = 0x0f;
	m_DragRotateIcon[4][3] = 0x1f;
	m_DragRotateIcon[4][4] = 0x3f;
	m_DragRotateIcon[4][5] = 0x0f;
	m_DragRotateIcon[4][6] = 0x0d;
	m_DragRotateIcon[4][7] = 0x18;
	m_DragRotateIcon[4][8] = 0x218;
	m_DragRotateIcon[4][9] = 0x670;
	m_DragRotateIcon[4][10] = 0xfe0;
	m_DragRotateIcon[4][11] = 0x1f80;
	m_DragRotateIcon[4][12] = 0x3f00;
	m_DragRotateIcon[4][13] = 0x3f80;
	
	//下
	m_DragRotateIcon[5][0] = 0x00;
	m_DragRotateIcon[5][1] = 0x00;
	m_DragRotateIcon[5][2] = 0x00;
	m_DragRotateIcon[5][3] = 0x00;
	m_DragRotateIcon[5][4] = 0x618;
	m_DragRotateIcon[5][5] = 0x1e1e;
	m_DragRotateIcon[5][6] = 0x3fff;
	m_DragRotateIcon[5][7] = 0x1e1e;
	m_DragRotateIcon[5][8] = 0x618;
	m_DragRotateIcon[5][9] = 0x00;
	m_DragRotateIcon[5][10] = 0x00;
	m_DragRotateIcon[5][11] = 0x00;
	m_DragRotateIcon[5][12] = 0x00;
	m_DragRotateIcon[5][13] = 0x00;
	
	//左下
	m_DragRotateIcon[6][0] = 0x3000;
	m_DragRotateIcon[6][1] = 0x3800;
	m_DragRotateIcon[6][2] = 0x3c00;
	m_DragRotateIcon[6][3] = 0x3e00;
	m_DragRotateIcon[6][4] = 0x3f00;
	m_DragRotateIcon[6][5] = 0x3c00;
	m_DragRotateIcon[6][6] = 0x2c00;
	m_DragRotateIcon[6][7] = 0x600;
	m_DragRotateIcon[6][8] = 0x610;
	m_DragRotateIcon[6][9] = 0x398;
	m_DragRotateIcon[6][10] = 0x1fc;
	m_DragRotateIcon[6][11] = 0x7e;
	m_DragRotateIcon[6][12] = 0x3f;
	m_DragRotateIcon[6][13] = 0x7f;
	
	//左
	m_DragRotateIcon[7][0] = 0x80;
	m_DragRotateIcon[7][1] = 0x1c0;
	m_DragRotateIcon[7][2] = 0x1c0;
	m_DragRotateIcon[7][3] = 0x3e0;
	m_DragRotateIcon[7][4] = 0x3e0;
	m_DragRotateIcon[7][5] = 0x80;
	m_DragRotateIcon[7][6] = 0x80;
	m_DragRotateIcon[7][7] = 0x80;
	m_DragRotateIcon[7][8] = 0x80;
	m_DragRotateIcon[7][9] = 0x3e0;
	m_DragRotateIcon[7][10] = 0x3e0;
	m_DragRotateIcon[7][11] = 0x1c0;
	m_DragRotateIcon[7][12] = 0x1c0;
	m_DragRotateIcon[7][13] = 0x80;
}



void CHLView::PreviewStroke(StrokeList *pStrokeList)
{
	CStrokeB *pStroke = NULL;
	CPoint   point;
	CDot     pointTemp;
	CDC     *pDC = GetDC();

	int mode = pDC->SetROP2(R2_NOTXORPEN);
	POSITION pos = pStrokeList->GetHeadPosition();
	while(pos != NULL)
	{
		pStroke = pStrokeList->GetNext(pos);
		for(int k = 0;k<pStroke->GetSize();k++)
		{
			pointTemp = pStroke->GetAt(k);
			point.x = (int)pointTemp.x;
			point.y = (int)pointTemp.y;
			if (0 == k)
			{
				pDC->MoveTo(point);
			}
			else
			{
				pDC->LineTo(point);
			}
		}
	}
	
	pDC->SetROP2(mode);
	ReleaseDC(pDC);
}



void CHLView::EmptyStroke(StrokeList *pStrokeList)
{
	CStrokeB *pStroke = NULL;
	POSITION pos = pStrokeList->GetHeadPosition();

	while( pos != NULL )
	{
		pStroke = pStrokeList->GetNext(pos);
		if ( pStroke )
		{
			delete pStroke;
			pStroke = NULL;
		}
	}

	pStrokeList->RemoveAll();
}



void CHLView::RotatePreviewStrokeList(StrokeList *pStrokeList, CDot CenterPoint, double dbAngle)
{
	CStrokeB *pStroke = NULL;
	POSITION  pos = pStrokeList->GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = pStrokeList->GetNext(pos);
		pStroke->Rotate(CenterPoint,dbAngle);
	}
}



void CHLView::DrawIcon(CDC *pDC, int x, int y, unsigned short *pIconMap)
{
	unsigned short *pTemp = pIconMap;
	if ( pTemp == NULL )
		return;

	for( int i = 0;i<14;i++ )
	{
		for(int k = 0;k<14;k++)
		{
			if ( IsBitSigned(*pTemp,13-k) )
			{
			    pDC->SetPixel(x+k,y+i,RGB(0,0,0));
			}
		}
		pTemp++;
	}
}

BOOL CHLView::IsBitSigned(unsigned short n, unsigned int nBit)
{
    n = n >> nBit;
	n = n & 0x01;

	return ( n  !=  0 ) ? TRUE : FALSE;
}

void CHLView::DrawRotateTrack(XDC *pDC)
{
}

void CHLView::OnHelppath() 
{
	CDlgHelpPath dlg;
	dlg.DoModal();
}

void CHLView::OnZoomoff() 
{
	if (GetZoomMode() == MODE_ZOOMSEL)
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = DRAWMODEL_CLICK;
		SetCursor(m_hCursorClick);
		m_hCursorCur = m_hCursorClick;
	}
	else
	{
		SetZoomMode(MODE_ZOOMSEL);
		SetCursor(m_hCursorZoomIn);
		m_hCursorCur = m_hCursorZoomIn;
		m_nDrawTool = DRAWMODEL_NONE;
	}
}

void CHLView::OnZoomoper()
{
	if (MODE_ZOOMOPER == GetZoomMode())
	{
		SetZoomMode(MODE_ZOOMOFF);
		m_nDrawTool = DRAWMODEL_CLICK;
		SetCursor(m_hCursorClick);
		m_hCursorCur = m_hCursorClick;
	}
	else
	{
		SetZoomMode(MODE_ZOOMOPER);	
		m_nDrawTool = DRAWMODEL_NONE;
		CZoomView::m_bLBDowm = FALSE;
		SetCursor(m_hCursorDoxMove);
		m_hCursorCur = m_hCursorDoxMove;
	}
}

void CHLView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	pCmdUI->Enable(!pDoc->m_b3D);
}

void CHLView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	pCmdUI->Enable(!pDoc->m_b3D);
}

void CHLView::OnUpdateZoomSelect(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	pCmdUI->Enable(!pDoc->m_b3D);
}

void CHLView::OnUpdateZoomAll(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	pCmdUI->Enable(!pDoc->m_b3D);
}

void CHLView::OnUpdateZoomPage(CCmdUI* pCmdUI)
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	pCmdUI->Enable(!pDoc->m_b3D);
}

void CHLView::OnUpdateZoomoper(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	pCmdUI->Enable(!pDoc->m_b3D);
	pCmdUI->SetCheck(GetZoomMode() == MODE_ZOOMOPER );
}

void CHLView::OnUpdateZoomoff(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	pCmdUI->Enable(!pDoc->m_b3D);
	pCmdUI->SetCheck(GetZoomMode() == MODE_ZOOMSEL);

	if (m_nDrawTool)
	{
		SetZoomMode(MODE_ZOOMOFF);
	}
}

void CHLView::ReplaceDot()
{
	m_track.ReplaceDot();
}

void CHLView::DelDot()
{
	m_track.DelDot();
}

BOOL CHLView::HitOtherObject(CPoint &point)//选中其它对象
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return FALSE;

	return pDoc->HitOtherObject(point,this);
}

// 画线条上的标示点
void CHLView::DrawRect()
{
	m_track.AddRect(this);
	m_track.FillRect(this);
}

void CHLView::AddPoint(CPoint &point)
{
	m_track.AddPoint(point,this);
}


//断点操作
void CHLView::SplitPoint(double dbPoint)
{
	m_track.SplitPoint(dbPoint);
	m_track.ClearAll();
	
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->ReLoadStrokeList();
}

void CHLView::JoinPoint()
{
	m_track.JoinPoint();	
	m_track.ClearAll();

	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->ReLoadStrokeList();
}
//
void CHLView::OnMoveaccu()
{
	CMoveAccuDlg dlg;
	dlg.DoModal();
}

//
void CHLView::OnChangeconvert()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->Changeconvert();
}

void CHLView::OnConvert() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->Changeconvert();
}


//
void CHLView::OnDeldot()
{	
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->DelDot(this);
	m_track.ClearDot();
	Invalidate();
}


void CHLView::OnJoindot()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->JoinPoint(this);
	Invalidate();
}


void CHLView::OnSplitdot() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	if ( g_SysCfg.m_dbOneToTwo  <=  0 )
		return;

	pDoc->SplitDot(this, g_SysCfg.m_dbOneToTwo);
	Invalidate();
}

void CHLView::OnSelAllStroke() 
{
	m_track.SelectStrokeBySelPt(this);
}

void CHLView::OnSetCurDotToStart()
{
	m_track.SetDotToStart(this);
}

//
void CHLView::OnFilterPoint()
{
	m_track.FilterPoint(this, g_SysCfg.m_dbDotFilterPrecis, g_SysCfg.m_dbDotFilterAngle);
	m_track.ClearDot();
	Invalidate();
}


//
void CHLView::OnReverseStroke() 
{
	m_track.ReverseStroke(this);
}

void CHLView::OnFilterCurDocPoint() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->FilterPointCmd();
	Invalidate();	
}

void CHLView::OnKillAngle() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc  !=  NULL )
	{
		pDoc->OnKillAngle();
		Invalidate();	
	}
}

//
void CHLView::OnSortObj() 
{
	//判断是否有功能对象
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	if ( pDoc->m_chain.HaveFunctionNode())
	{
		AfxMessageBox(IDS_ERROR_OBJPRO);
		return;
	}
	//
	if (DRAWMODEL_SORT_OBJECT_LIST  !=  m_nDrawTool)
	{
		g_bIsSort = TRUE;
		m_nDrawTool = DRAWMODEL_SORT_OBJECT_LIST;
		SetZoomMode(MODE_ZOOMOFF);
		m_hCursorCur = m_hCursorClick;
		
		CHLDoc *pDoc = (CHLDoc*)GetDocument();
		pDoc->IniSort(this);
		SortPro();
		m_nDrawTool = DRAWMODEL_CLICK;
		g_bIsSort = FALSE;
		Invalidate();
	}
	else
	{
		g_bIsSort = FALSE;
		m_nDrawTool = DRAWMODEL_CLICK;
		Invalidate();
	}
}

void CHLView::OnUpdateSortObj(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
	if ( DRAWMODEL_SORT_OBJECT_LIST == m_nDrawTool )
	{
		g_bIsSort = TRUE;
		pCmdUI->SetCheck(1);
	}
	else
	{
		g_bIsSort = FALSE;
		pCmdUI->SetCheck(0);
	}
}

void CHLView::DrawMark(CDC* pDC) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->DrawMark(pDC, this);
}

void CHLView::SortPro()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	BOOL bRet = pDoc->m_SortListObj.Track(this);
	if (bRet)
	{
		if (IDYES == AfxMessageBox(IDS_APPLYALLOBJECT, MB_YESNO))
		{
			pDoc->SortAllObj();
		}
	}
}

void CHLView::OnAutoSortObj()
{
	//判断是否有功能对象
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	if ( pDoc->m_chain.HaveFunctionNode() )
	{
		AfxMessageBox(IDS_ERROR_OBJPRO);
		return;
	}

	m_nDrawTool = DRAWMODEL_ASORT_OBJECT_LIST;
	m_hCursorCur = m_hCursorClick;
	SetZoomMode(MODE_ZOOMOFF);

	if ( pDoc->InitASort(this) )
		pDoc->ExitASort();

	Invalidate();
	m_nDrawTool = DRAWMODEL_CLICK;
}

void CHLView::OnUpdateAutoSortObj(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);

	if ( DRAWMODEL_ASORT_OBJECT_LIST == m_nDrawTool )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}



void CHLView::OnSelSortObj() 
{
	//判断是否有功能对象
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	if ( pDoc->m_chain.HaveFunctionNode() )
	{
		AfxMessageBox(IDS_ERROR_OBJPRO);
		return;
	}
	
	m_nDrawTool = ID_SEL_OBJECT_LIST;
	SetZoomMode(MODE_ZOOMOFF);

	pDoc->InitSelSort(this);
	m_nDrawTool = DRAWMODEL_CLICK;
}

void CHLView::OnUpdateSelSortObj(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
	
	if ( ID_SEL_OBJECT_LIST == m_nDrawTool )
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CHLView::AutoSortPro()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	BOOL bRet = pDoc->m_ASortListObj.Track(this);
	if ( bRet )
	{
		if ( IDYES == AfxMessageBox(IDS_APPLYALLOBJECT, MB_YESNO) )
		{
			pDoc->SortAllObj();
		}
	}

	pDoc->m_Sort.RemoveAll();
}

void CHLView::AutoSortDraw(CDC *pDc) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( !pDoc )
		return;

	pDoc->m_ASortListObj.GetResultMsg();

	CPen   pen(PS_SOLID, 1, RGB(0,0,255));
	CPen  *pOldPen = pDc->SelectObject(&pen);

	int nSize = pDoc->m_ASort.GetSize();
	ADRAWNODE *pNl = new ADRAWNODE[nSize];
	if ( !pNl )
		return;

	for(int i = 0; i < nSize; i++)
	{
		ASNODE nd = pDoc->m_ASort[i];
		int nSort = nd.m_nSort;
		ASNODE nd1 = pDoc->m_ASort[nSort];
		pNl[i].m_SDt = nd1.m_SDt;
		pNl[i].m_EDt = nd1.m_EDt;
	}

	for(int j = 0; j < nSize; j++)
	{
		double sx = pNl[j].m_SDt.x;
		double sy = pNl[j].m_SDt.y;
		PhysicToLogic(sx, sy);
		double ex = pNl[j].m_EDt.x;
		double ey = pNl[j].m_EDt.y;
		PhysicToLogic(ex, ey);

		CRect rts((int)(sx - 3), (int)(sy - 3)
			     ,(int)(sx + 3), (int)(sy + 3));
		pDc->FillSolidRect(&rts, RGB(255,0,0));

		CRect rte((int)(ex - 3), (int)(ey - 3)
			     ,(int)(ex + 3), (int)(ey + 3));
		pDc->FillSolidRect(&rte, RGB(255,0,0));

		if (j == (nSize - 1)) 
		{
			CRect rts((int)(sx - 3), (int)(sy - 3)
					 ,(int)(sx + 3), (int)(sy + 3));
			pDc->FillSolidRect(&rts, RGB(255, 0, 0));

			CRect rte((int)(ex - 3), (int)(ey - 3)
			     ,(int)(ex + 3), (int)(ey + 3));
		    pDc->FillSolidRect(&rte, RGB(0, 255, 0));
		}
	}

	if ( nSize > 0 )
	{
		double endx,endy;
		for( int k = 0; k < nSize; k++)
		{
			double sx = pNl[k].m_SDt.x;
			double sy = pNl[k].m_SDt.y;
			PhysicToLogic(sx, sy);
			double ex = pNl[k].m_EDt.x;
			double ey = pNl[k].m_EDt.y;
			PhysicToLogic(ex, ey);
			if (k == 0)
			{
				endx = sx;
				endy = sy;
				pDc->MoveTo((int)sx, (int)sy);
				pDc->LineTo((int)ex, (int)ey);
			}
			else
			{
				pDc->LineTo((int)sx, (int)sy);
				pDc->LineTo((int)ex, (int)ey);
			}
		}// end for
		pDc->LineTo((int)endx, (int)endy);
	}// end if (size > 0)

	pDc->SelectObject(pOldPen);

	pen.DeleteObject();
	if ( pNl )
	{
		delete[] pNl;
		pNl = NULL;
	}
}


void CHLView::InvalidateSort()
{
	CRect rt;
	GetClientRect(&rt);

	rt.top = rt.bottom - 20;
	InvalidateRect(rt);
}


//
void CHLView::SetGroupNum(int nNum) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	
	pDoc->SetGroupNum(nNum);
}


//
void CHLView::DispSelObjProp()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	if ( NULL == pFrame )
		return;
	
	CHLDoc *pDoc = (CHLDoc*)m_pDocument;
	if ( NULL == pDoc )
		return;
	
	//
	CNode *pNodeBmp = pDoc->GetSelNode(NODE_BMP);
	if ( pNodeBmp  !=  NULL )
	{
		pFrame->m_FillEditDlg.GeneralPage(pNodeBmp);
		//////
		CQuad quad(0,0,0,0);
		DOUBLEOUTPARA mulMarkPara;
		pDoc->m_chain.GetAllSelTreeProp(mulMarkPara, quad);
		pDoc->RefreshSelectQuadList();
		pFrame->m_PSDlg.UpdateProp(quad);
		return;
	}
	
	//
	int nStatus = pDoc->m_chain.GetStatus();
	if ( (nStatus & CChain::selectMul) || (nStatus & CChain::selectUnion) )
	{
		if ( pDoc->GetSelObjLockFlg() )
		{
			pFrame->m_FillEditDlg.GeneralPage(NULL);
		}
		else
		{
			CNode *pNode = new CNodeV();
			if ( pNode )
			{
				pFrame->m_FillEditDlg.GeneralPage(pNode);
				delete pNode;
			}
		}
	}
	else
		pFrame->m_FillEditDlg.GeneralPage(pDoc->m_chain.GetNode());
	
	CQuad quad(0,0,0,0);
	DOUBLEOUTPARA mulMarkPara;
	pDoc->m_chain.GetAllSelTreeProp(mulMarkPara, quad);
	int nLayer = pDoc->m_chain.GetSelObjLayerNo();
	
	FILLDATA fillOld;
	pDoc->GetFillProp(fillOld);
	pFrame->m_FillEditDlg.m_ObjFillEdit.DisplyFillProp(fillOld);
	
#ifdef _SPECIALPROP_EDIT
	OBJMARKSET ObjMarkSet;
    pDoc->GetObjMarkSet(ObjMarkSet);
	pFrame->m_FillEditDlg.m_FillObjMarkSet.DisplayMarkSet(ObjMarkSet);
#endif
	
	pDoc->RefreshSelectQuadList();

	if ( nLayer  >=  0 )
		pFrame->m_PropDlg.ClickColorButton(nLayer);

	pFrame->m_PSDlg.UpdateProp(quad);
}


//
void CHLView::OnLoadObjList()
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;
	
    pDoc->ObjListUnLoad();
	pDoc->ObjListLoad();
}

//
void CHLView::OnAlignBottom()
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if (NULL == pDoc) return;
	
	CRect qdRect;
	m_track.GetTrueRect(qdRect);
	pDoc->OnMAlignCmd(CmdAlignBottom);

	InvalidateRect(&qdRect);
}

//
void CHLView::OnAlignLeft()
{
	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc) 
		return;
	
	CRect qdRect;
	m_track.GetTrueRect(qdRect);

	pDoc->OnMAlignCmd(CmdAlignLeft);
	InvalidateRect(&qdRect);
}

//
void CHLView::OnAlignRight()
{
	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc) 
		return;
	
	CRect qdRect;
	m_track.GetTrueRect(qdRect);
	pDoc->OnMAlignCmd(CmdAlignRight);

	InvalidateRect(&qdRect);
}



//
void CHLView::OnAlignTop() 
{
	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc) 
		return;

	CRect qdRect;
	m_track.GetTrueRect(qdRect);

	pDoc->OnMAlignCmd(CmdAlignTop);
	
	InvalidateRect(&qdRect);
}



void CHLView::OnDistanceX() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	CRect qdRect;
	m_track.GetTrueRect(qdRect);

	pDoc->OnMAlignCmd(CmdDistanceX);
	
	InvalidateRect(&qdRect);
}



void CHLView::OnDistanceY() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	CRect qdRect;
	m_track.GetTrueRect(qdRect);

	pDoc->OnMAlignCmd(CmdDistanceY);
	
	InvalidateRect(&qdRect);
}


void CHLView::OnEqualHeight() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc ) 
		return;

	CRect qdRect;
	m_track.GetTrueRect(qdRect);

	pDoc->OnMAlignCmd(CmdEqualHeight);
	
	InvalidateRect(&qdRect);
}


void CHLView::OnEqualWidth() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	CRect qdRect;
	m_track.GetTrueRect(qdRect);

	pDoc->OnMAlignCmd(CmdEqualWidth);
	
	InvalidateRect(&qdRect);
}

void CHLView::OnEqualWh() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	CRect qdRect;
	m_track.GetTrueRect(qdRect);

	pDoc->OnMAlignCmd(CmdEqualWh);
	
	InvalidateRect(&qdRect);
}

void CHLView::ClearMark() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;
	
	m_track.ClearAll();
    pDoc->m_QuadList.RemoveAll();
	pDoc->SetSelect(FALSE);
	pDoc->m_quadTracker.SetEmpty();
}

BOOL CHLView::PreTranslateMessage(MSG* pMsg) 
{	
	return CZoomView::PreTranslateMessage(pMsg);
}

BOOL CHLView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;  	
}


void CHLView::OnAlignX() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->OnMAlignCmd(CmdAlignX);
	Invalidate();
}

void CHLView::OnAlignY() 
{
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc )
		return;

	pDoc->OnMAlignCmd(CmdAlignY);
	Invalidate();
}

void CHLView::OnJointstrokeCurdoc() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = GetDocument();
	if ( NULL == pDoc ) 
		return;
	
	pDoc->JoinStrokeCmd();
}


//
void CHLView::OnSetUserfont()
{
	CString strWorkFolder = ::g_sys.GetInstallPath();
	CString strCmdLine = strWorkFolder+_T("\\ES.exe");
		
	STARTUPINFO si = {0};
	si.cb  = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi = {0};

	::CreateProcess(NULL,strCmdLine.GetBuffer(),NULL,NULL,FALSE,NULL,NULL,strWorkFolder.GetBuffer(),&si,&pi);
	strCmdLine.ReleaseBuffer();
	strWorkFolder.ReleaseBuffer();
}

void CHLView::OnDrawRange(UINT nID) 
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL ) 
		return;

	if ( pDoc->m_bPathOptimize == TRUE )
		return;
	
	if ( nID == ID_DRAW_TIMER )
	{
		CNodeDelay* pNode = new CNodeDelay();
		pDoc->AddNode(pNode);
	}
	else if (nID == ID_DRAW_INPUT)
	{
		CNodeInput* pNode = new CNodeInput();
		pDoc->AddNode(pNode);
	}
	else if (nID == ID_DRAW_OUTPUT)
	{
		CNodeOutput* pNode = new CNodeOutput();
		pDoc->AddNode(pNode);
	}
	else
	{
		return;
	}
	
	DispSelObjProp();
}


void CHLView::OnDrawSpline() 
{
	// TODO: Add your command handler code here
	if ( m_nDrawTool == DRAWMODEL_SPLINE )
	{
		CDlgObjPrecisionSet dlg;
		dlg.m_dbPrecision = g_SysCfg.m_dbPrecision;
		dlg.m_nModelNo	 =  g_SysCfg.m_nSampleCount;
		if ( IDOK == dlg.DoModal() )
		{
			g_SysCfg.m_dbPrecision    = dlg.m_dbPrecision;
			g_SysCfg.m_nSampleCount = dlg.m_nModelNo;

			g_sys.WriteSysCfg();
		}
	}

	if ( GetZoomMode()  !=  MODE_ZOOMOFF )
	{
		SetZoomMode(MODE_ZOOMOFF);
	}
	
	m_nDrawTool = DRAWMODEL_SPLINE;
	m_hCursorCur = m_hCursorSpline;
	
	if ( m_bRotate )
	{	
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	}
}

void CHLView::OnUpdateDrawSpline(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable( g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D );
	pCmdUI->SetCheck( m_nDrawTool == DRAWMODEL_SPLINE );
}

//
void CHLView::OnDrawArc3pt() 
{
	// TODO: Add your command handler code here	if ( m_nDrawTool == DRAWMODEL_ELLIPSE )
	if ( m_nDrawTool == DRAWMODEL_ARC_3PT )
	{
		CDlgObjPrecisionSet dlg;
		dlg.m_dbPrecision = g_SysCfg.m_dbPrecision;
		dlg.m_nModelNo	 =  g_SysCfg.m_nSampleCount;
		if ( IDOK == dlg.DoModal() )
		{
			g_SysCfg.m_dbPrecision    = dlg.m_dbPrecision;
			g_SysCfg.m_nSampleCount = dlg.m_nModelNo;
			g_sys.WriteSysCfg();
		}
	}

	if ( GetZoomMode()  !=  MODE_ZOOMOFF )
	{
		SetZoomMode(MODE_ZOOMOFF);
	}
	
	m_nDrawTool = DRAWMODEL_ARC_3PT;
	m_hCursorCur =  m_hCursorArc;
	
	if ( m_bRotate )
	{	
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	}
}

void CHLView::OnUpdateDrawArc3pt(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	pCmdUI->SetCheck(m_nDrawTool == DRAWMODEL_ARC_3PT);
}



void CHLView::OnDrawArcCenter()
{
	// TODO: Add your command handler code here
	if ( m_nDrawTool == DRAWMODEL_ARC_CENTER )
	{
		CDlgObjPrecisionSet dlg;
		dlg.m_dbPrecision = g_SysCfg.m_dbPrecision;
		dlg.m_nModelNo	 =  g_SysCfg.m_nSampleCount;
		if ( IDOK == dlg.DoModal() )
		{
			g_SysCfg.m_dbPrecision  = dlg.m_dbPrecision;
			g_SysCfg.m_nSampleCount = dlg.m_nModelNo;
			g_sys.WriteSysCfg();
		}
	}
	
	if ( GetZoomMode()  !=  MODE_ZOOMOFF )
	{
		SetZoomMode(MODE_ZOOMOFF);
	}
	
	m_nDrawTool = DRAWMODEL_ARC_CENTER;
	m_hCursorCur =  m_hCursorArc;
	
	if ( m_bRotate )
	{
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	}
}





void CHLView::OnUpdateDrawArcCenter(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	pCmdUI->SetCheck(m_nDrawTool == DRAWMODEL_ARC_CENTER);
}

void CHLView::OnDrawCircle() 
{
	if ( m_nDrawTool == DRAWMODEL_CIRCLE )
	{
		CDlgObjPrecisionSet dlg;
		dlg.m_dbPrecision = g_SysCfg.m_dbPrecision;
		dlg.m_nModelNo	 =  g_SysCfg.m_nSampleCount;
		if ( IDOK == dlg.DoModal() )
		{
			g_SysCfg.m_dbPrecision    = dlg.m_dbPrecision;
			g_SysCfg.m_nSampleCount = dlg.m_nModelNo;
			g_sys.WriteSysCfg();
		}
	}

	if ( GetZoomMode()  !=  MODE_ZOOMOFF )
	{
		SetZoomMode(MODE_ZOOMOFF);
	}
	
	m_nDrawTool = DRAWMODEL_CIRCLE;
	m_hCursorCur =  m_hCursorEllipse;
	
	if ( m_bRotate )
	{	
		m_bRotate = FALSE;
		CHLDoc *pDoc = GetDocument();
		m_track.SetPhysicQuad(this,pDoc->GetTrackerQuad());
	}
}

void CHLView::OnUpdateDrawCircle(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	pCmdUI->SetCheck(m_nDrawTool == DRAWMODEL_CIRCLE);
}

//
void CHLView::OnSplineEdit()
{	
	// TODO: Add your command handler code here
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	
	CNodeSpline *pNode = NULL;
	pNode = (CNodeSpline *)pDoc->GetSelNode(NODE_SPLINE);
	if ( pNode == NULL )
		return;
	
	if ( pNode->GetLockFlg() == TRUE )
		return;

	m_hCursorCur = m_hCursorSpline;
	SetCursor(m_hCursorCur);
	
	m_nDrawTool = DRAWMODEL_OBJ_EDIT;
	ClearMark();
	pNode->TrackEdit(this);
	CTree *pTree = (CTree*)pNode->GetParent();
	if ( pTree  !=  NULL  )
		pTree->Fill();

	m_nDrawTool = DRAWMODEL_CLICK;
	m_hCursorCur = m_hCursorClick;
	
	Invalidate();
}


void CHLView::OnArcEdit() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	
	CNodeArc3Pt *pNode = NULL;
	pNode = (CNodeArc3Pt *)pDoc->GetSelNode(NODE_ARC);
		
	if ( pNode  !=  NULL )
	{	
		if ( pNode->GetLockFlg() == TRUE )
			return;

		m_hCursorCur = m_hCursorArc;
		SetCursor(m_hCursorCur);
		
		m_nDrawTool = DRAWMODEL_OBJ_EDIT;
		ClearMark();
		pNode->TrackEdit(this);
		CTree *pTree = (CTree*)pNode->GetParent();
		if ( pTree  !=  NULL )
			pTree->Fill();
		
	}
	m_nDrawTool = DRAWMODEL_CLICK;
	m_hCursorCur =  m_hCursorClick;

	Invalidate();
}

//
void CHLView::OnCircleEdit() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;
	
	CNodeCircle *pNode = NULL;
	pNode = (CNodeCircle *)pDoc->GetSelNode(NODE_CIRCLE);
	
	if ( pNode  !=  NULL )
	{	
		if ( pNode->GetLockFlg() == TRUE )
			return;

		m_hCursorCur = m_hCursorEllipse;
		SetCursor(m_hCursorCur);
		
		m_nDrawTool = DRAWMODEL_OBJ_EDIT;
		ClearMark();
		pNode->TrackEdit(this);
		CTree *pTree = (CTree*)pNode->GetParent();
		if ( pTree  !=  NULL )
			pTree->Fill();
		
	}
	m_nDrawTool = DRAWMODEL_CLICK;
	m_hCursorCur = m_hCursorClick;

	Invalidate();
}

//
void CHLView::OnPrecisionSet() 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( NULL == pDoc )
		return;

	CNode *pNode = pDoc->GetSelNode(-1);
	if ( NULL == pNode )
		return;

	CNodeArc3Pt *pNodeArc3Pt = NULL;
	CNodeCircle *pNodeCircle = NULL;
	CNodeSpline *pNodeSpline = NULL;
	CDlgObjPrecisionSet dlg;
	
	if ( pNode->GetType() == NODE_ARC )
	{
		pNodeArc3Pt = (CNodeArc3Pt *)pNode;
		dlg.m_dbPrecision = pNodeArc3Pt->m_dbPrecision;
		dlg.m_nModelNo = pNodeArc3Pt->m_nSampleCount;
	}
	else if ( pNode->GetType() == NODE_CIRCLE )
	{
		pNodeCircle = (CNodeCircle *)pNode;
		dlg.m_dbPrecision = pNodeCircle->m_dbPrecision;
		dlg.m_nModelNo = pNodeCircle->m_nSampleCount;
	}
	else if ( pNode->GetType() == NODE_SPLINE )
	{
		pNodeSpline = (CNodeSpline *)pNode;
		dlg.m_dbPrecision = pNodeSpline->m_dbPrecision;
		dlg.m_nModelNo = pNodeSpline->m_nSampleCount;
	}
	
	if ( dlg.DoModal() == IDOK )
	{
		if ( pNode->GetType() == NODE_ARC )
		{
			pNodeArc3Pt->PrecisionSet(dlg.m_dbPrecision, dlg.m_nModelNo);
			pNodeArc3Pt->CreateStroke();
		}
		else if ( pNode->GetType() == NODE_CIRCLE )
		{
			pNodeCircle->PrecisionSet(dlg.m_dbPrecision, dlg.m_nModelNo);
			pNodeCircle->CreateStroke();
		}
		else if ( pNode->GetType() == NODE_SPLINE )
		{
			pNodeSpline->PrecisionSet(dlg.m_dbPrecision, dlg.m_nModelNo);
			pNodeSpline->CreateStroke();
		}
		
		Invalidate();
	}
}

void CHLView::OnDrawDot() 
{
	if ( GetZoomMode()  !=  MODE_ZOOMOFF )
	{
		SetZoomMode(MODE_ZOOMOFF);
	}
	
	m_nDrawTool = DRAWMODEL_DOT;
	m_hCursorCur =  m_hCursorDot;	
}

void CHLView::OnUpdateDrawDot(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
	pCmdUI->SetCheck(m_nDrawTool == DRAWMODEL_DOT);
}


void CHLView::OnUpdateDrawTimer(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
}

void CHLView::OnUpdateDrawInput(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
}

void CHLView::OnUpdateDrawOutput(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !pDoc->m_b3D);
}

void CHLView::OnUpdateDrawFill(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
}

void CHLView::OnUpdateDrawBmp(CCmdUI* pCmdUI) 
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
}

void CHLView::OnUpdateEqualWidth(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateEqualWh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateEqualHeight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateAlignLeft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateAlignRight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateAlignTop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateAlignX(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateAlignY(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateAlignBottom(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateDistanceX(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateDistanceY(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}

void CHLView::OnUpdateConvert(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CHLDoc* pDoc = (CHLDoc*)GetDocument();	
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
}

void CHLView::OnUpdateFilterCurdocPoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CHLDoc* pDoc = (CHLDoc*)GetDocument();	
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
}

void CHLView::OnUpdatePathNormal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CHLDoc* pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
}

void CHLView::OnUpdateSetUserfont(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}



void CHLView::OnDrawMotormove() 
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;
	
	if ( pDoc->m_bPathOptimize == TRUE )
		return;

	CNodeMotorMove* pNode = new CNodeMotorMove();
	if ( pNode  !=  NULL )
		pDoc->AddNode(pNode);
	
	DispSelObjProp();	
}

void CHLView::OnUpdateDrawMotormove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !pDoc->m_b3D);
}

void CHLView::OnDrawSetpos() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	if (pDoc->m_bPathOptimize == TRUE)
		return;
	
	CNodeMotorPos* pNode = new CNodeMotorPos();
	if ( pNode  !=  NULL )
		pDoc->AddNode(pNode);
	
	DispSelObjProp();
}

void CHLView::OnUpdateDrawSetpos(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !pDoc->m_b3D);
}

void CHLView::OnDrawCondition() 
{
	// TODO: Add your command handler code here
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;
	
	if ( pDoc->m_bPathOptimize == TRUE )
		return;
	
	CNodeConditionS* pNodeS = new CNodeConditionS();
	if ( !pNodeS )
		return;

	CNodeConditionE* pNodeE = new CNodeConditionE();
	if ( !pNodeE )
	{
		delete pNodeS;
		return;
	}

	pDoc->AddNode(pNodeS);
	pDoc->AddNode(pNodeE);
	
	DispSelObjProp();
}

void CHLView::OnUpdateDrawCondition(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !pDoc->m_b3D);
}


void CHLView::OnSize(UINT nType, int cx, int cy)
{
	CZoomView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CHLDoc* pDoc = (CHLDoc*)GetDocument();
	if ( pDoc )
		pDoc->m_chain3d.InitOpenGL(m_hWnd);
}

void CHLView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_pDocument )
	{
		CHLDoc* pDoc = (CHLDoc*)GetDocument();
		if (pDoc && !pDoc->m_b3D)
		{
			lpncsp->rgrc[0].left += m_nRulerWidth;
			lpncsp->rgrc[0].top  += m_nRulerWidth;
 		}
	}
	CView::OnNcCalcSize(bCalcValidRects, lpncsp);
}

//
void CHLView::Track3D(CPoint point)
{
	CHLDoc* pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
		return;

	pDoc->m_chain3d.Convert(point.x, point.y);
	
	AfxLockTempMaps();
	SetCapture();
	
	CPoint ptLaset = point;
	MSG msg;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			{	
				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{	
				point.x = (int)(short)LOWORD(msg.lParam);
				point.y = (int)(short)HIWORD(msg.lParam);

				if (msg.wParam&MK_LBUTTON)
				{
					pDoc->m_chain3d.Drag(point.x-ptLaset.x, point.y-ptLaset.y);
				}else if (msg.wParam&MK_RBUTTON)
				{
					if (point.x != ptLaset.x)	{
						pDoc->m_chain3d.SightH(point.x<ptLaset.x);
					}
					if (point.y != ptLaset.y)	{
						pDoc->m_chain3d.SightV(point.y>ptLaset.y);
					}
				}
				ptLaset  = point;		
				pDoc->m_chain3d.Draw();
				break;
			}
		}
	}
ExitLoop:
	{	
		ReleaseCapture();
		AfxUnlockTempMaps();
	}
}

void CHLView::OnDrawBufferlaser()
{
	// TODO: 在此添加命令处理程序代码
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	CNodeBufferLaser* pNode = new CNodeBufferLaser();
	if ( pNode )
		pDoc->AddNode(pNode);

	DispSelObjProp();	
}

void CHLView::OnUpdateDrawBufferlaser(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK && !pDoc->m_b3D);
}


void CHLView::OnDrawCcd()
{
	// TODO: 在此添加命令处理程序代码
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;

	CNodeCCD* pNode = new CNodeCCD();
	if ( pNode )
		pDoc->AddNode(pNode);

	DispSelObjProp();	
}

void CHLView::OnUpdateDrawCcd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK && !pDoc->m_b3D);
}


void CHLView::OnDrawPoscali()
{
	CHLDoc *pDoc = GetDocument();
	if ( pDoc == NULL )
		return;
	
	CNodePosCali* pNode = new CNodePosCali();
	if ( pNode )
		pDoc->AddNode(pNode);
	
	DispSelObjProp();
}


void CHLView::OnUpdateDrawPoscali(CCmdUI *pCmdUI)
{
	CHLDoc *pDoc = (CHLDoc*)GetDocument();
	if ( pDoc == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK && !pDoc->m_b3D);
}
