// NodeGrid.cpp: implementation of the CNodeGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeGrid.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeGrid::CNodeGrid()
{
	m_type = NODE_GRID;

	m_nCol = g_SysCfg.m_nGridCol;
	m_nRow =  g_SysCfg.m_nGridRow;

	if ( m_nCol <= 0 )
		m_nCol = 1;

	if ( m_nRow <= 0 )
		m_nRow = 1;
}

CNodeGrid::~CNodeGrid()
{
}

BOOL CNodeGrid::Track(CWnd* pWnd, CPoint ptOrg)
{
	if ( !pWnd || ::GetCapture()  !=  NULL)
		return FALSE;

	BOOL bResult = FALSE;

	AfxLockTempMaps();
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);

	CZoomView* pView =  (CZoomView*)pWnd;
	XDC *pXDC = pView->PrepareDC(pDC);

	CPoint pointNow = ptOrg;
	BOOL  bMoved = FALSE;

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_GRID);
	while ( TRUE )
	{
		MSG msg;
		VERIFY( ::GetMessage(&msg, NULL, 0, 0) );

		if ( CWnd::GetCapture()  !=  pWnd )
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

				Adjust(msg,ptOrg);
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);

				int mode = pDC->SetROP2(R2_NOTXORPEN);

				CDot dt = (CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);

				CDot odt = (CDot)ptOrg;
				pView->LogicToPhysic(odt.x,odt.y);

				CQuad quad = CQuad(odt,dt);

				Draw(pXDC);
				Create(quad);

				Draw(pXDC);
				pDC->SetROP2(mode);

				//状态行提示
				str.Format(prompt,quad.Width(),quad.Height());
				if ( pFrame )
					pFrame->PostMessage(WM_SETMESSAGESTRING, 0,(LPARAM)(LPCTSTR)str);
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

	//生成最终数据
	CDot dt = (CDot)pointNow;
	pView->LogicToPhysic(dt.x,dt.y);
	CDot odt = (CDot)ptOrg;
	pView->LogicToPhysic(odt.x,odt.y);

	CQuad quad= CQuad(odt,dt);

    BOOL bRet = Create(quad);
	return bRet;
}

BOOL CNodeGrid::Create(CQuad &quad)
{
	Delete();
	//重建立
	CStroke *pStroke = NULL;
	double col  = quad.Width()  / m_nCol;
	double row = quad.Height() / m_nRow;
	CDot dot1,dot2;

	int i = 0;
	for( ; i  <=  m_nCol; i++)
	{
		dot1.x = quad.left+i*col; 
		dot1.y = quad.top;
		dot2.x = dot1.x;
		dot2.y = quad.bottom;

		pStroke = new CStroke(m_property);
		if ( pStroke == NULL )
			return FALSE;

		pStroke->Add(dot1);
		pStroke->Add(dot2);
		m_list.AddTail(pStroke);
	}

	for( i = 0; i  <=  m_nRow; i++)
	{
		dot1.x = quad.left; 
		dot1.y = quad.top-i*row;
		dot2.x = quad.right;
		dot2.y = dot1.y;

		pStroke = new CStroke(m_property);
		if ( pStroke == NULL )
			return FALSE;

		pStroke->Add(dot1);
		pStroke->Add(dot2);
		m_list.AddTail(pStroke);
	}

	return TRUE;
}


void CNodeGrid::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	//把它当成PLT图存笔画信息
	CMySerialize fs(fp,bUnicode);
	
	if ( bFlag )
	{
		//写入头信息
		fs << _T("NGRID_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		Serialize_list(fp,bFlag,bUnicode);
		//写入尾信息
		fs << _T("NGRID_E");
	}
	else
	{
		CString str;
		while( fs >>  str )
		{	
			if ( str == _T("QUAD_RANG_B")  )
			{
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
			}
			else if ( str == _T("PRO_B")  )
			{
				m_property.Serialize(fp,bFlag,bUnicode);
			}
			else if ( str == _T("SL_B")  )
			{
				Serialize_list(fp,bFlag,bUnicode);
			}
			else if ( str == _T("NGRID_E") )
				break;
		}
	}
}
