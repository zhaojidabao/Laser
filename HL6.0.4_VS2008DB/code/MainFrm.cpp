// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "HL.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "HLView.h"
#include "htmlhelp.h"
#include "resource.h"
#include "UserRightCfgMng.h"
#include "UserLogin.h"
#include "baseMacro.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//CMainFrame
HICON		g_hIcoList[20];
HBITMAP		g_hBmpDtt[7];

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_COMMAND_EX(ID_VIEW_CHANGE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHANGE, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_DRAW, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAW, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_ZOOM, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_PROPERTY, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTY, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	ON_COMMAND(ID_DISPLAY, OnDisplay)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY, OnUpdateDisplay)
	ON_COMMAND(ID_HIDE, OnHide)
	ON_UPDATE_COMMAND_UI(ID_HIDE, OnUpdateHide)
	ON_COMMAND(IDM_KEYBIND_SET, OnKeybindSet)
	ON_WM_DESTROY()
	ON_COMMAND(ID_SET_CALIBRATE, OnSetCalibrate)
	ON_COMMAND(ID_SET_LASERPARA, OnSetLaserPara)
	ON_COMMAND(ID_ADJUST_M, OnAdjustM)
	ON_UPDATE_COMMAND_UI(ID_SET_CALIBRATE, OnUpdateSetCalibrate)
	ON_UPDATE_COMMAND_UI(ID_SET_LASERPARA, OnUpdateSetLaserpara)
	ON_UPDATE_COMMAND_UI(ID_ADJUST_M, OnUpdateAdjustM)
	ON_UPDATE_COMMAND_UI(IDM_KEYBIND_SET, OnUpdateKeybindSet)
	ON_COMMAND(IDM_CHANGEUSER, OnChangeuser)
	ON_COMMAND(IDM_RIGHTMNG, OnRightmng)
	ON_COMMAND(ID_LOGOUT_USER, OnLogoutUser)
	ON_MESSAGE(WM_DISPLAY_INFOR,OnDisplayInfor)
	ON_MESSAGE(WM_START_MSG,OnStartMachine)
	ON_MESSAGE(WM_DISPLAY_CARD_STATUS,OnDispCardStatus)
	ON_WM_TIMER()
	ON_COMMAND(ID_LANG_ENGLISH, &CMainFrame::OnLangEnglish)
	ON_UPDATE_COMMAND_UI(ID_LANG_ENGLISH, &CMainFrame::OnUpdateLangEnglish)
	ON_COMMAND(ID_LANG_CHINESES, &CMainFrame::OnLangChineses)
	ON_UPDATE_COMMAND_UI(ID_LANG_CHINESES, &CMainFrame::OnUpdateLangChineses)
	ON_COMMAND(ID_LANG_CHINESET, &CMainFrame::OnLangChineset)
	ON_UPDATE_COMMAND_UI(ID_LANG_CHINESET, &CMainFrame::OnUpdateLangChineset)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_RUNHLEX, &CMainFrame::OnRunHlex)
	ON_WM_NCPAINT()
	ON_WM_QUERYENDSESSION()
END_MESSAGE_MAP()

static UINT indicators[]  = 
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CALIBOX,
	ID_INDICATOR_MARKTIME,
	ID_INDICATOR_MOTORPOS,
	ID_INDICATOR_MOTORLEFT,
	ID_INDICATOR_MOTORRIGHT,
	ID_INDICATOR_USER,
//	ID_INDICATOR_LENGTH,
	ID_INDICATOR_POS,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pView			=  NULL;
	m_bActivate		=  TRUE;
	m_pWndButton	=  NULL;
	
	m_bStartCalibrate		 =  FALSE;
	m_strProcessName	 =  _T("");
	
	//
	m_hicRed	 =  (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_RED), IMAGE_ICON, 16, 16, 0);
	m_hicGreen = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_GREEN), IMAGE_ICON, 16, 16, 0);
}

CMainFrame::~CMainFrame()
{
	if ( m_hicRed )
	{
		::DestroyIcon(m_hicRed);
		m_hicRed = NULL;
	}
	if ( m_hicGreen )
	{
		::DestroyIcon(m_hicGreen);
		m_hicGreen = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CMDIFrameWnd::OnCreate(lpCreateStruct) == -1 )
		return -1;

	if ( !m_wndStatusBar.Create(this) ||
		 !m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	UINT nID   = 0, nStyle =  0;
	int nIndex = 0, cxWidth = 0;
	CStatusBarCtrl& sb = m_wndStatusBar.GetStatusBarCtrl();
	
	m_wndStatusBar.GetPaneInfo(0,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(0,nID,nStyle,150); //限制空白区大小
	
	//BOX
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CALIBOX);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,100);
	
	//单个打标时间
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MARKTIME);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,110);
	
	//运动轴位置
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MOTORPOS);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,125);
	
	//左限位
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MOTORLEFT);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,70);
	sb.SetIcon(nIndex, (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_GREEN), IMAGE_ICON, 16, 16, 0));
	
	//右限位
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MOTORRIGHT);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,70);
	sb.SetIcon(nIndex, (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_GREEN), IMAGE_ICON, 16, 16, 0));
	
	//用户信息
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_USER);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,100);
	
	//长度信息
//	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_LENGTH);
//	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
//	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,115);
	
	//位置信息
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_POS);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,cxWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,150);
	
	//建立常用工具条,设定常用工具栏的放置特性及风格
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
#ifdef _SOFTMODE_EDITOR_
		!m_wndToolBar.LoadToolBar(IDR_EDITFRAME))
#else
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
#endif
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	//建立缩放工具条,设定缩放工具栏的放置特性及风格
	if (!m_wndZoomBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_VIEW_ZOOM) ||
		!m_wndZoomBar.LoadToolBar(IDR_ZOOM))
	{
		TRACE0("Failed to create zoombar\n");
		return -1;      // fail to create
	}
	
	//建立变换工具条,设定变换工具栏的放置特性及风格
	if (!m_wndChangeBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_VIEW_CHANGE) ||
		!m_wndChangeBar.LoadToolBar(IDR_OBJCHANGE))
	{
		TRACE0("Failed to create changebar\n");
		return -1;      // fail to create
	}
	
	//建立运动控制工具条,设定运动控制工具栏的放置特性及风格
	if (!m_wndMotion.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_VIEW_CHANGE) ||
		!m_wndMotion.LoadToolBar(IDR_MOTION))
	{
		TRACE0("Failed to create changebar\n");
		return -1;      // fail to create
	}

	//建立绘图工具条,设定绘图工具栏的放置特性及风格
	if (!m_wndDrawBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_VIEW_DRAW) ||
		!m_wndDrawBar.LoadToolBar(IDR_DRAW))
	{
		TRACE0("Failed to create drawbar\n");
		return -1;      // fail to create
	}
	//建立显示工具条
	if ( !m_wndDisplay.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_VIEW_DRAW) ||
		!m_wndDisplay.LoadToolBar(IDR_DISPLAY))
	{
		TRACE0("Failed to create drawbar\n");
		return -1;      // fail to create
	}
	
	//建立排序工具条
	if ( !m_wndSort.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_VIEW_DRAW) ||
		!m_wndSort.LoadToolBar(IDR_SORTOBJ))
	{
		TRACE0("Failed to create drawbar\n");
		return -1;      // fail to create
	}
	
	//建立对齐排列工具条
	if ( !m_wndArrangeBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),ID_VIEW_DRAW) ||
		!m_wndArrangeBar.LoadToolBar(IDR_ARRANGE))
	{
		TRACE0("Failed to create arrangebar\n");
		return -1;      // fail to create
	}
	
	CString str;
	str.LoadString(IDS_TITLE_MAINFRAME);
	m_wndToolBar.SetWindowText(str);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	str.LoadString(IDS_TITLE_ZOOM);
	m_wndZoomBar.SetWindowText(str);
	m_wndZoomBar.EnableDocking(CBRS_ALIGN_ANY);
	
	m_wndMotion.SetWindowText(_T("Motion"));
	m_wndMotion.EnableDocking(CBRS_ALIGN_ANY);
	
	str.LoadString(IDS_TITLE_CHANGE);
	m_wndChangeBar.SetWindowText(str);
	m_wndChangeBar.EnableDocking(CBRS_ALIGN_ANY);
	
	str.LoadString(IDS_DISPLAY);
	m_wndDisplay.SetWindowText(str);
	m_wndDisplay.EnableDocking(CBRS_ALIGN_ANY);
	
    str.LoadString(IDS_SORTOBJ);
	m_wndSort.SetWindowText(str);
	m_wndSort.EnableDocking(CBRS_ALIGN_ANY);
	
	str.LoadString(IDS_TITLE_DRAW);
	m_wndDrawBar.SetWindowText(str);
	m_wndDrawBar.EnableDocking(CBRS_ALIGN_ANY);
	
	str.LoadString(IDS_DISPLAY);
	m_wndArrangeBar.SetWindowText(str);
	m_wndArrangeBar.EnableDocking(CBRS_ALIGN_ANY);
	
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	DockControlBarLeftOf(&m_wndDisplay,&m_wndToolBar);
	DockControlBarLeftOf(&m_wndSort,&m_wndDisplay);
	DockControlBarLeftOf(&m_wndMotion, &m_wndSort);
	DockControlBarLeftOf(&m_wndZoomBar,&m_wndMotion);
	
	CRect rect;
	DockControlBar(&m_wndChangeBar,AFX_IDW_DOCKBAR_RIGHT);  //
	m_wndChangeBar.GetWindowRect(&rect);
	rect.OffsetRect(0, 1);
	DockControlBar(&m_wndArrangeBar, AFX_IDW_DOCKBAR_RIGHT, rect);
	
	BOOL bHighMetrics = (GetSystemMetrics(SM_CYSCREEN)  >=  900);
	//建立属性条
	str.LoadString(IDS_OBJECTPROPERTY);
	m_XBarPos.CreateEx(str,this,1000,bHighMetrics ? CSize(217,250) : CSize(217,200));//217,200
	m_XBarPos.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY);
 	m_XBarPos.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT|CBRS_ALIGN_TOP);
	
	//建立打标参数条
	str.LoadString(IDS_MARKPARA);
	m_XBarProperty.CreateEx(str,this,1001,bHighMetrics ? CSize(217,700) : CSize(217,550));//217,620 590 
//	m_XBarProperty.CreateEx(str,this,1001,bHighMetrics ? CSize(217,1000) : CSize(217,590));//217,620 590 
	m_XBarProperty.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY);
	m_XBarProperty.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT|CBRS_ALIGN_BOTTOM);
	
 	DockControlBar(&m_XBarPos,AFX_IDW_DOCKBAR_RIGHT);
	RecalcLayout();
	m_PSDlg.Create(CChangDlg::IDD,&m_XBarPos);
	m_PSDlg.CloseDoor();
	m_PropDlg.Create(CProperDlg::IDD,&m_XBarProperty);
    RecalcLayout();
	
    m_XBarPos.GetWindowRect(&rect);
    rect.OffsetRect(0,1);
	DockControlBar(&m_XBarProperty,AFX_IDW_DOCKBAR_RIGHT,&rect);
	
	//
#ifdef _SPECIALPROP_EDIT
	//对象列表
	str.LoadString(IDS_OBJLIST);
	m_XObjectlist.CreateEx(str,this,1002, bHighMetrics ? CSize(188,260) : CSize(188,190));//188,210
	m_XObjectlist.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY);
 	m_XObjectlist.EnableDocking(CBRS_ALIGN_LEFT);

	//特性列表
	str.LoadString(IDS_SPECIALLIST);
	m_XDBSpecialty.CreateEx(str,this,1004,bHighMetrics ? CSize(188,730) : CSize(188,600));//188,600
	m_XDBSpecialty.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY);
 	m_XDBSpecialty.EnableDocking(CBRS_ALIGN_LEFT);
	
	DockControlBar(&m_XObjectlist,AFX_IDW_DOCKBAR_LEFT);
	RecalcLayout();
	m_ObjectListDlg.Create(CObjectListDlg::IDD,&m_XObjectlist);
	
	m_FillEditDlg.Create(CFillEditDlg::IDD,&m_XDBSpecialty);
    RecalcLayout();
	CRect rect1;
    m_XObjectlist.GetWindowRect(&rect1);
    rect1.OffsetRect(0,1);
	DockControlBar(&m_XDBSpecialty,AFX_IDW_DOCKBAR_LEFT,&rect1);
#endif
	
	//
	DockControlBar(&m_wndDrawBar,AFX_IDW_DOCKBAR_LEFT);
	RecalcLayout();
	
	g_hIcoList[0] = AfxGetApp()->LoadIcon(IDI_MOVE_MARK);
	g_hIcoList[1] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_HIGTH);
	g_hIcoList[2] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_45_ANGLE);
	g_hIcoList[3] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_135_ANGLE);
	g_hIcoList[4] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_WITH);
	g_hIcoList[5] = AfxGetApp()->LoadIcon(IDI_ICON_SCALE_LOCK);
	
	g_hBmpDtt[0] = LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_MOUSEROTATE_LT));
	g_hBmpDtt[1] = LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_MOUSEROTATE_RT));
	g_hBmpDtt[2] = LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_MOUSEROTATE_RB));
	g_hBmpDtt[3] = LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_MOUSEROTATE_LB));
	g_hBmpDtt[4] = LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_SHEARX));
	g_hBmpDtt[5] = LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_SHEARY));
	g_hBmpDtt[6] = LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_ROTATE_CENTER));
	
	m_PSDlg.ShowWindow(FALSE);
	m_PropDlg.ShowWindow(FALSE);
	
#ifdef _SPECIALPROP_EDIT
    m_ObjectListDlg.ShowWindow(FALSE);
    m_FillEditDlg.ShowWindow(FALSE);
#endif
	
	AddStatusButton();
	
	//定义计时器
	if ( !SetTimer(0,500,NULL) )
	{
		AfxMessageBox(IDS_ERROR_TIMER);
	}

	return 0;
}




BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	//  the CREATESTRUCT cs
	
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::DockControlBarLeftOf(CToolBar *Bar, CToolBar *LeftOf)
{
    CRect rect;
    DWORD dw;
    UINT n;
	
    RecalcLayout();
    LeftOf->GetWindowRect(&rect);
    rect.OffsetRect(1,0);
    dw = LeftOf->GetBarStyle();
    n = 0;
    n = (dw & CBRS_ALIGN_TOP)            ? AFX_IDW_DOCKBAR_TOP    :n;
    n = (dw & CBRS_ALIGN_BOTTOM && n == 0) ? AFX_IDW_DOCKBAR_BOTTOM :n;
    n = (dw & CBRS_ALIGN_LEFT   && n == 0) ? AFX_IDW_DOCKBAR_LEFT   :n;
    n = (dw & CBRS_ALIGN_RIGHT  && n == 0) ? AFX_IDW_DOCKBAR_RIGHT  :n;

    DockControlBar(Bar,n,&rect);
}

void CMainFrame::OnClose()
{
	SaveBarState(_T("MyBarState"));
	
	CMDIFrameWnd::OnClose();
}


//
void CMainFrame::OnDestroy()
{
	StartProcess(m_strProcessName);
	
	StopAvi();
	CMDIFrameWnd::OnDestroy();
}

//
void CMainFrame::StopAvi()
{
	SaveBarState(_T("MyBarState"));
	
	if (m_pWndButton)
	{
		m_pWndButton->stop();
		m_pWndButton->CloseWindow();
		delete m_pWndButton;
		m_pWndButton = NULL;
	}
}


void CMainFrame::RecalcLayout(BOOL bNotify) 
{
 	CMDIFrameWnd::RecalcLayout(bNotify);
	CMDIFrameWnd::RecalcLayout(bNotify);
}

void CMainFrame::UpdateProperty(BOOL bNoDoc)
{
	static BOOL b3DLast = FALSE;
	CHLDoc *pDoc = (CHLDoc *)GetCurActiveDocument();
	
	if ( !pDoc )
	{
        m_PSDlg.ShowWindow(SW_HIDE);
        m_PropDlg.ShowWindow(SW_HIDE);
		m_ObjectListDlg.ShowWindow(SW_HIDE);
		m_FillEditDlg.ShowWindow(SW_HIDE);
	}
	else
	{
		m_PSDlg.ShowWindow(SW_SHOW);
	    m_PropDlg.ShowWindow(SW_SHOW);
		m_ObjectListDlg.ShowWindow( pDoc->m_b3D ? SW_HIDE : SW_SHOW );
		m_FillEditDlg.ShowWindow( pDoc->m_b3D ? SW_HIDE : SW_SHOW );
		
		if ( pDoc->m_b3D  !=  b3DLast )
		{
			b3DLast = pDoc->m_b3D;
			m_wndSort.EnableWindow(!b3DLast);
			m_wndZoomBar.EnableWindow(!b3DLast);
			m_wndDrawBar.EnableWindow(!b3DLast);
			PostMessage(WM_COMMAND, ID_DISPLAY);
			
			if ( b3DLast )
				SetMousePosInfo(0, 0);
		}
	}
	
	m_PropDlg.UpdateProperty();
	SetUserInfo();
}





BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//消除：打标参数修改后直接按打标工具时，最后修改的参数无效
	CWnd *pWnd = GetFocus();
	if ( pWnd )
	{
		HWND hWnd = pWnd->GetSafeHwnd();
		::SendMessage(hWnd,WM_KILLFOCUS,NULL,NULL);
	}
	
	return CMDIFrameWnd::OnCommand(wParam, lParam);
}




CDocument* CMainFrame::GetActiveDocument()
{
	CDocument* pDoc = CMDIFrameWnd::GetActiveDocument();
	CMDIChildWnd*  pChild = MDIGetActive();
	if ( pChild )
	{
		UpdateProperty(TRUE);
	}
	else
	{
		UpdateProperty();
	}
	
	return pDoc;
}




BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CMainFrame::OnDisplay() 
{
#ifdef _SPECIALPROP_EDIT
	BOOL bShow = m_XDBSpecialty.IsVisible();
	if (bShow)
	{
		ShowControlBar(&m_XDBSpecialty,FALSE,FALSE);
		ShowControlBar(&m_XObjectlist,FALSE,FALSE);
	}
	else
	{
		m_XObjectlist.SetSize(CSize(110,150));
		ShowControlBar(&m_XObjectlist,TRUE,FALSE);
	    ShowControlBar(&m_XDBSpecialty,TRUE,FALSE);
	}
#endif
}

void CMainFrame::OnUpdateDisplay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SPECIALPROP_EDIT
	pCmdUI->SetCheck(m_XObjectlist.IsVisible());
#endif
}

void CMainFrame::OnHide() 
{
	BOOL bShow = m_XBarProperty.IsVisible();
	if (bShow)
	{
		ShowControlBar(&m_XBarProperty,FALSE,FALSE);
		ShowControlBar(&m_XBarPos,FALSE,FALSE);
	}
	else
	{
		m_XBarPos.SetSize(CSize(173,210));
		ShowControlBar(&m_XBarPos,     TRUE,FALSE);
		ShowControlBar(&m_XBarProperty,TRUE,FALSE);
	}
}

void CMainFrame::OnUpdateHide(CCmdUI* pCmdUI) 
{
	BOOL bShow = m_XBarProperty.IsVisible();
	pCmdUI->SetCheck(bShow);
}

void CMainFrame::SetPos(double dbX,double dbY,double dbZ)
{
	m_PSDlg.SetPos(dbX,dbY,dbZ);
}

void CMainFrame::SetSize(double dbSizeX,double dbSizeY)
{
	m_PSDlg.SetSize(dbSizeX,dbSizeY);
}

void CMainFrame::OpenDoor()
{
	m_PSDlg.OpenDoor();
}

LRESULT CMainFrame::OnDisplayInfor(WPARAM wpara/*  = 0 */,LPARAM lpara/*  = 0 */)
{
	TCHAR szMsg[256] = {0};
	UINT nRet = g_DAT.GetErrInfo(wpara, szMsg, 255);
	if ( nRet == HLERROR ) 
		return 1;

	CString strTitle = GetTitle() + _T("  (") + szMsg +  _T(")  ");
	SetWindowText(strTitle);
	
	if ( lpara == 2 && g_DAT.m_hMarkWnd ) //Reconnect
	{
		g_DAT.EnterMarkEnv();
		g_DAT.OutStatus(O_ENT_MARK_ENV,O_ENT_MARK_ENV);
	}

	return 0;
}

LRESULT CMainFrame::OnStartMachine(WPARAM wpara,LPARAM lpara)
{
	CHLApp* pApp = (CHLApp*)AfxGetApp();
#ifndef _SOFTMODE_EDITOR_
	if( pApp->m_pdlgSplash )
		pApp->m_pdlgSplash->DestroyWindow();
#endif
	if( wpara == 0 )//SUCCESS
	{
		ShowWindow(SW_SHOWMAXIMIZED);
		ShowWindow(SW_SHOW);
		UpdateWindow();
		//
		TCHAR szPath[MAX_PATH] = { 0 };
		TCHAR szFileName[MAX_PATH] = { 0 };

		GetExepath(szPath,MAX_PATH);
		_stprintf_s(szFileName,MAX_PATH, _T("%s%s"), szPath, _T("SysCfg.ini"));

		// 第一次运行程序弹出新功能推介向导
		BOOL bFirstRun = ReadIniInt(szFileName,_T("SysCfg"), _T("AUTOGUIDE"), _T("1") );
		if( bFirstRun )
			pApp->ShowNewFlag();
	}
	else if( wpara == 2 ) // 没有校正文件
	{
		AfxMessageBox( IDS_OPENCALIFILE_ERROR );
		SetCalibrate();

	}
	else if( wpara == 3 ) // 卡初始化失败
	{
		AfxMessageBox(_T("Can't initial control card,System will exit!"));
		OnClose();
	}
	else			// 狗检测失败
	{
		OnClose();
	}

	return 0;
}

LRESULT CMainFrame::OnDispCardStatus(WPARAM wpara/*  = 0 */,LPARAM lpara/*  = 0 */)
{
	TCHAR szMsg[255] = {0};
	UINT nRet = g_DAT.GetErrInfo(wpara, szMsg, 255);
	if ( nRet == HLERROR ) 
		return 1;
	
	m_wndStatusBar.SetPaneText( 0, szMsg );
	
	return 0;
}


void CMainFrame::SetMousePosInfo( double x, double y )
{
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_POS);
	CString strTmp1;
	strTmp1.LoadString(ID_INDICATOR_POS);
	CString strTmp2;
	strTmp2.Format(_T("%0.3f, %0.3f"), x, y);
	m_wndStatusBar.SetPaneText(index, strTmp1+strTmp2);
}



void CMainFrame::SetLengthInfo(double length)
{
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_LENGTH);
	CString strTemp1;
	strTemp1.LoadString(ID_INDICATOR_LENGTH);
	CString strTemp2;
	strTemp2.Format(_T("%0.3f"), length);
	strTemp1  +=  strTemp2;
	m_wndStatusBar.SetPaneText(index, strTemp1);
}


void CMainFrame::SetUserInfo()
{
	CString strMsg;
	strMsg.LoadString(ID_INDICATOR_USER);
	strMsg  +=  g_CurUserInfo.szUserName;

	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_USER);
	m_wndStatusBar.SetPaneText(index, strMsg);

	strMsg.LoadString(ID_INDICATOR_CALIBOX);
	strMsg  +=  g_DAT.GetCaliBoxName();
	index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_CALIBOX);
	m_wndStatusBar.SetPaneText(index,strMsg);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_SYSCOMMAND )
	{
		if ( pMsg->wParam  == SC_MINIMIZE )
		{
			ShowWindow(SW_SHOWMINIMIZED);
			return TRUE;
		}
		else if ( pMsg->wParam  == SC_RESTORE )
		{
			ShowWindow(SW_SHOWMAXIMIZED);
			return TRUE;
		}
	}

	if (pMsg->message  >=  WM_KEYFIRST && pMsg->message  <=  WM_KEYLAST) 
	{ 
		if ( m_keyBind.m_hAccel  !=  NULL )
		{
			if (::TranslateAccelerator(m_hWnd, m_keyBind.m_hAccel, pMsg)  !=  0)
			{
				return TRUE;
			}
		}
	}

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnKeybindSet() 
{
	// TODO: Add your command handler code here
	m_keyBind.DoModal();
}



void CMainFrame::AddStatusButton()
{
    m_pWndButton = new CAniButton;
	if ( !m_pWndButton )
		return;

	m_pWndButton->LoadAVI(IDR_DCP_AVI);

	if ( !m_pWndButton->Create(		_T(""),
									WS_CHILD|WS_VISIBLE|WS_TABSTOP,
									CRect(0,0,0,0), 
									&m_wndStatusBar, 
									IDC_STATUS_BAR_BUTTON )		)
	{
		return;
	}
	
	m_pWndButton->SetFont(	CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT))  );
	
	BOOL bRet = m_wndStatusBar.AddPane(IDS_PANE_BUTTON, 8);
	if ( !bRet )	
		return;
	
	int nIndex = m_wndStatusBar.CommandToIndex(IDS_PANE_BUTTON);
	if ( nIndex == -1 )
		return;
	
	m_wndStatusBar.SetPaneWidth(nIndex, 25);
	m_wndStatusBar.AddPaneControl(m_pWndButton, IDS_PANE_BUTTON, TRUE);

	m_pWndButton->InitAviItem();
	m_pWndButton->play();
	m_wndStatusBar.SetITCursor(IDC_CURSOR_HAND);
}



void CMainFrame::RefreshCmdSet()
{
	CString strFilePath = ::g_sys.GetInstallPath();

	try
	{
		UINT nId,nStyle;
		int iImage;
		int i = 0;
		CStdioFile file(strFilePath + _T("cmd.set"), CFile::modeCreate|CFile::modeWrite);
		int count = m_wndDrawBar.GetCount();
		for (i = 0; i<count; i++)
		{
			m_wndDrawBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle  !=  TBBS_SEPARATOR && nId  !=  0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find(_T('\n'));
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format(_T("%d,绘图,%s\r\n"), nId, strDesc);
				file.WriteString(strLine);
			}
		}

		count = m_wndChangeBar.GetCount();
		for (i = 0; i<count; i++)
		{
			m_wndChangeBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle  !=  TBBS_SEPARATOR && nId  !=  0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find(_T('\n'));
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format(_T("%d,变换对象,%s\r\n"), nId, strDesc);
				file.WriteString(strLine);
			}
		}

		count = m_wndZoomBar.GetCount();
		for (i = 0; i<count; i++)
		{
			m_wndZoomBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle  !=  TBBS_SEPARATOR && nId  !=  0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find(_T('\n'));
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format(_T("%d,缩放排序,%s\r\n"), nId, strDesc);
				file.WriteString(strLine);
			}
		}

		count = m_wndArrangeBar.GetCount();
		for (i = 0; i<count; i++)
		{
			m_wndArrangeBar.GetButtonInfo(i, nId, nStyle, iImage);
			if (nStyle  !=  TBBS_SEPARATOR && nId  !=  0)
			{
				CString strDesc;
				strDesc.LoadString(nId);
				int index = strDesc.Find(_T('\n'));
				strDesc = strDesc.Left(index);
				CString strLine;
				strLine.Format(_T("%d,排列对齐,%s\r\n"), nId, strDesc);
				file.WriteString(strLine);
			}
		}

		file.Flush();
		file.Close();
		AfxMessageBox(IDS_CREATSUCCSED);
	}
	catch (CMemoryException* e){e;}
	catch (CFileException* e){e;}
	catch (CException* e){e;}
}

int CMainFrame::SetObjecSel(CGroupObj *pGroupObj)
{
	if ( NULL == pGroupObj )
		return -1;
	
	return m_ObjectListDlg.SetSelByGroupObj(pGroupObj);
}

void CMainFrame::SetAllObjecUnSel()
{
	m_ObjectListDlg.SetObjUnSel();
}

//
BOOL CMainFrame::StartWorkConsoleDlg()
{	
	CString strCmdLine = _T("");	
	GetModuleFileName(NULL, strCmdLine.GetBuffer(MAX_PATH), MAX_PATH);
	strCmdLine.ReleaseBuffer();
	strCmdLine = strCmdLine.Left(strCmdLine.ReverseFind('\\'));

	CString strWorkFolder = strCmdLine+_T("\\emcc\\");
	strCmdLine  +=  _T("\\emcc\\WorkConsole.exe");
	
	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	TCHAR szCmdLine[MAX_PATH];
	_stprintf_s(szCmdLine, MAX_PATH, _T("%s"), strCmdLine);
	
	::CreateProcess(NULL,szCmdLine,NULL,NULL,FALSE,NULL,NULL,strWorkFolder,&si,&pi);
	
	return TRUE;
}


//
void CMainFrame::StartProcess(CString strProcessName)
{
	if ( m_bStartCalibrate )
		StartWorkConsoleDlg(); 
	else
		return;
	
	Sleep(10);
	CWnd *pWnd = NULL;
	LONG lWaitTime = 0;
	while( pWnd == NULL )
	{
		Sleep(1);
		lWaitTime++;
		pWnd = CWnd::FindWindow( NULL,_T("WorkConsole"));
		
		if ((pWnd  !=  NULL) || (lWaitTime > 500))
			break;
	}
	
	if ( pWnd == NULL )
	{
		AfxMessageBox(_T("The process does not exist!") );
	}
	else
	{
		if ( strProcessName.Compare(_T("Cali") ) == 0 )
			pWnd->PostMessage(WM_SETDLG_TYPE, 0, 0);
		else if ( strProcessName.Compare(_T("LaserSet") ) == 0)
			pWnd->PostMessage(WM_SETDLG_TYPE, 0, 1);
		else if ( strProcessName.Compare(_T("AdjustM") ) == 0)
			pWnd->PostMessage(WM_SETDLG_TYPE, 0, 2);
		else if ( strProcessName.Compare(_T("HLEX") ) == 0)
			pWnd->PostMessage(WM_SETDLG_TYPE, 0, 3);
	}
}



//
void CMainFrame::OnSetCalibrate()
{
	if (IDYES == AfxMessageBox(IDS_START_CALIBRATEDLG, MB_YESNO))
	{	
		SetCalibrate();
	}
}


//
void CMainFrame::SetCalibrate()
{
	m_bStartCalibrate = TRUE;
	m_strProcessName = _T("Cali");
	OnClose();
	
	//
	CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
	if ( pFWnd  !=  NULL )
	{
		m_bStartCalibrate = FALSE;
		m_strProcessName = _T("");
	}
}



//
void CMainFrame::OnSetLaserPara()
{	
	if (IDYES == AfxMessageBox(IDS_START_LASERPARA, MB_YESNO))
	{	
		m_bStartCalibrate = TRUE;
		m_strProcessName = _T("LaserSet");
		OnClose();
		
		//
		CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
		if (pFWnd  !=  NULL)
		{	
			m_bStartCalibrate = FALSE;
			m_strProcessName = _T("");
		}
	}
}


//
void CMainFrame::OnAdjustM()
{
	if ( IDYES == AfxMessageBox(IDS_START_ADJUSTM, MB_YESNO) )
	{
		m_bStartCalibrate = TRUE;
		m_strProcessName = _T("AdjustM");
		OnClose();
		
		//
		CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
		if ( pFWnd  !=  NULL )
		{
			m_bStartCalibrate = FALSE;
			m_strProcessName = _T("");
		}
	}
}


void CMainFrame::OnUpdateSetCalibrate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CALI);
#endif
}

void CMainFrame::OnUpdateSetLaserpara(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET);
#endif
}

void CMainFrame::OnUpdateAdjustM(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET);
#endif
}

void CMainFrame::OnUpdateKeybindSet(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET);
}

void CMainFrame::OnChangeuser() 
{
	// TODO: Add your command handler code here
	CUserLogin loginDlg;
	if ( IDCANCEL == loginDlg.DoModal() )
		return;
	
	CHLView *pView = GetCurActiveView();
	if ( pView && pView->m_nDrawTool  !=  DRAWMODEL_CLICK )
	{
		pView->m_nDrawTool = DRAWMODEL_CLICK;
		pView->SetCurCursorClick();
	}

	UpdateProperty();
}

void CMainFrame::OnRightmng() 
{
	// TODO: Add your command handler code here
	BOOL bMng = ::g_CurUserInfo.dwRight1 & USER_RIGHT_ADMIN;
	if ( bMng ) 
	{
		CUserRightCfgMng RightMngDlg;
		RightMngDlg.DoModal();
	}
	else
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
	}
}

void CMainFrame::OnLogoutUser() 
{
	// TODO: Add your command handler code here
	if ( IDNO == AfxMessageBox(IDS_LOGOUT_PROPT, MB_YESNO) )
		return;
	memset(&g_CurUserInfo,0,sizeof(USERINFO));
	//
	UpdateProperty();	
}

//
void CMainFrame::SetMotorPos( int nAxis, LONGLONG nPos)
{
	double dbPos = g_Motion.PosTranslate(nAxis, nPos);
	
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MOTORPOS);
	CString strTemp1;
	strTemp1.LoadString(ID_INDICATOR_MOTORPOS);
	CString strTemp2;
	strTemp2.Format(_T("%0.3f"), dbPos);
	strTemp1  +=  strTemp2;
	m_wndStatusBar.SetPaneText(index, strTemp1);
}



void CMainFrame::SetMotorLeftFlag(BOOL bFlag)
{
	CStatusBarCtrl& sb = m_wndStatusBar.GetStatusBarCtrl();
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MOTORLEFT);
	
	if ( TRUE == bFlag )
	{
		sb.SetIcon(nIndex, m_hicRed);
	}
	else
	{
		sb.SetIcon(nIndex, m_hicGreen);
	}
}




void CMainFrame::SetMotorRightFlag(BOOL bFlag)
{
	CStatusBarCtrl& sb = m_wndStatusBar.GetStatusBarCtrl();
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MOTORRIGHT);
	
	if ( TRUE == bFlag )
	{
		sb.SetIcon(nIndex, m_hicRed);
	}
	else
	{
		sb.SetIcon(nIndex, m_hicGreen);
	}
}


void CMainFrame::SetMarkTime( double dbMarkTime )
{
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_MARKTIME);
	CString strTemp1;
	strTemp1.LoadString(ID_INDICATOR_MARKTIME);
	CString strTemp2;
	strTemp2.Format( _T(" %.3fs" ), dbMarkTime/1000);
	strTemp1  +=  strTemp2;
	m_wndStatusBar.SetPaneText(index, strTemp1);
}



void CMainFrame::OnTimer( UINT_PTR nIDEvent )
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( g_DAT.IsInit() == FALSE || FALSE == g_DAT.m_btExtendInfo.FunData.bMotorMark )
		return;
	
	CheckMotorStatus();
	CMDIFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::CheckMotorStatus()
{
	LONGLONG nPos(0);
	g_DAT.GetCurMotorPos(0, nPos);
	SetMotorPos(0, nPos);
	
	UINT uIO(0);
	g_DAT.GetInput(&uIO);
	SetMotorLeftFlag( (uIO&0x400)>0 );
	
	//
	int nLaserType = eType_IPG;
	UINT nRet = g_DAT.GetLayerParaType(nLaserType);
	if ( nRet  !=  HLOK )
		return;
	
	//
	if ( eType_IPGM == nLaserType 
		|| eType_IPGM_GENERAL == nLaserType 
		|| eType_YLPP_CLASSIC == nLaserType 
		|| eType_YLPP_GENERAL == nLaserType
		|| eType_HFMA_CLASSIC == nLaserType 
		|| eType_HFMA_GENERAL == nLaserType )
	{
		SetMotorRightFlag( (uIO&0x004)>0 );
	}
	else
	{
		SetMotorRightFlag( (uIO&0x800)>0 );
	}
}



void CMainFrame::OnLangEnglish()
{
	// TODO: 在此添加命令处理程序代码
	g_LangID = 0x409;
	LanguageChange();
}


void CMainFrame::OnUpdateLangEnglish(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_LangID == 0x409);
}

void CMainFrame::OnLangChineses()
{
	// TODO: 在此添加命令处理程序代码
	g_LangID = 0x804;
	LanguageChange();
}

void CMainFrame::OnUpdateLangChineses(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_LangID == 0x804);
}

void CMainFrame::OnLangChineset()
{
	// TODO: 在此添加命令处理程序代码
	g_LangID = 0x404;
	LanguageChange();
}

void CMainFrame::OnUpdateLangChineset(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_LangID == 0x404);
}


void CMainFrame::LanguageChange()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szFileName[MAX_PATH] = { 0 };
	int nLangFlag = 0;

	if ( g_LangID == 0x804 )
		nLangFlag = 0;
	else if (g_LangID == 0x404)
		nLangFlag = 1;
	else if (g_LangID == 0x409)
		nLangFlag = 2;
	
	//
	GetExepath(szPath,MAX_PATH);
	
	_stprintf_s(szFileName,  MAX_PATH,_T("%s%s"), szPath, _T("SysCfg.ini"));		
	WriteIniInt(szFileName, _T("LANG"), _T("LANG"), nLangFlag);
	
	_stprintf_s(szFileName,  MAX_PATH,_T("%s%s"), szPath, _T("Emcc\\SystemSet.ini"));		
	WriteIniInt(szFileName, _T("Lang"), _T("LANG"), nLangFlag);
	
	AfxMessageBox(IDS_LANGUAGE_CHANGE, MB_OK);
}



void CMainFrame::OnRunHlex()
{
	// TODO: 在此添加命令处理程序代码
	if ( IDYES == AfxMessageBox(IDS_START_HLEX, MB_YESNO) )
	{
		m_bStartCalibrate = TRUE;
		m_strProcessName = _T("HLEX");
		OnClose();
		
		//
		CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
		if ( pFWnd  !=  NULL )
		{
			m_bStartCalibrate = FALSE;
			m_strProcessName = _T("");
		}
	}
}



//WM_QUERYENDSESSION
BOOL CMainFrame::OnQueryEndSession()
{
	{
		if ( WaitForSingleObject(hMarkSts, 0)  !=  WAIT_OBJECT_0 )
			return TRUE;
		
		::SetEvent(hStopWork);
		::Sleep(60);
		
		g_DAT.StopMark();
	}
	
	//
	if ( !CMDIFrameWnd::OnQueryEndSession() )
		return FALSE;
	
	// TODO:  在此添加专用的查询结束会话代码
	
	return TRUE;
}
