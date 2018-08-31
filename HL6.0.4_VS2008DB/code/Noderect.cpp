// NodeRect.cpp: implementation of the CNodeRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "NodeRect.h"
#include "HLView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeRect::CNodeRect()
{
	m_type = NODE_RECT;
	m_GArc.m_dbPrecision = 0.1;
	m_GArc.m_nSampleCount = 100;
}

CNodeRect::~CNodeRect()
{
}

BOOL CNodeRect::Track(CWnd* pWnd, CPoint ptOrg)
{
	// A NULL return value means the current thread has not captured the mouse. However,
	// it is possible that another thread or process has captured the mouse.
	if ( !pWnd || ::GetCapture()  !=  NULL)
	{
		return FALSE;
	}

	BOOL bResult = TRUE;
	CZoomView *pView = (CZoomView*)pWnd;
	AfxLockTempMaps();

	pWnd->SetCapture();
	ASSERT( pWnd == CWnd::GetCapture() );
	pWnd->UpdateWindow();

	CDC* pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);
	XDC *pXDC = pView->PrepareDC(pDC);
	
	CPoint pointNow = ptOrg;
	BOOL bMoved = FALSE;

	while ( TRUE )
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		if ( CWnd::GetCapture()  !=  pWnd )
			break;
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				if ( bMoved )
				{
					bResult = TRUE;
				}
				else
				{
					bResult = FALSE;
				}
				goto ExitLoop;
				
				break;
			}
		case WM_MOUSEMOVE:
			{
				if ( !bMoved )
				{
					bMoved = TRUE;
				}
			
				Adjust(msg,ptOrg);
				
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);

				int mode = pDC->SetROP2(R2_NOTXORPEN);
				CDot dot1,dot2;
				dot1.x = ptOrg.x;
				dot1.y = ptOrg.y;
				dot2.x = pointNow.x;
				dot2.y = pointNow.y;

				pView->LogicToPhysic(dot1.x,dot1.y);
				pView->LogicToPhysic(dot2.x,dot2.y);

				Draw(pXDC);
				Create(dot1,dot2);
				Draw(pXDC);
				pDC->SetROP2(mode);
				break;
			}
		default:
			{
				DispatchMessage(&msg);
		        break;
			}
		}// end switch
	}// end for

ExitLoop:
	pWnd->ReleaseDC(pDC);
	ReleaseCapture();
	AfxUnlockTempMaps(FALSE);

	return bResult;
}

BOOL CNodeRect::Create(CDot dot1,CDot dot2)
{
	int       nMode  = ::g_sys.GetProfileInt(_T("RECTANGLE"),_T("TYPE"),0);;
	double dbValue = ::g_sys.GetProfileDouble(_T("RECTANGLE"),_T("VALUE"),1.0);
	
	return CreateStroke( dot1, dot2, nMode, dbValue);
}

//
BOOL CNodeRect::CreateStroke(CDot dt1,CDot dt2,int nMode,double dbValue)
{
	Delete();
	/////
	//生成四个点
	CStroke *pStroke  = new CStroke(m_property);
	if ( NULL == pStroke )
		return FALSE;

	CDot dot0,dot1,dot2,dot3,dot4;
	CDot dot5,dot6,dot7,dot;
	CQuad rect;

	rect.left	=  dt1.x;
	rect.right=  dt2.x;
	rect.bottom=  dt1.y;
	rect.top    = dt2.y;
	rect.Normal();
	
	if ( 0 == nMode || FALSE == (rect.Width() >= 2*dbValue && rect.Height() >= 2*dbValue) || dbValue < 0.005 )
	{
		dot0.x = rect.left;
		dot0.y = rect.top;
		dot1.x = rect.left;
		dot1.y = rect.bottom;
		dot2.x = rect.right;
		dot2.y = rect.bottom;
		dot3.x = rect.right;
		dot3.y = rect.top;
		
		pStroke->Add(dot0);
		pStroke->Add(dot1);
		pStroke->Add(dot2);
		pStroke->Add(dot3);
		pStroke->Add(dot0);
	}
	else if ( 1 == nMode )  //倒圆角
	{
		if ( dbValue < 0.005 )
			dbValue = 0.005;

		dot0.x = rect.left + dbValue;
		dot0.y = rect.top;
		dot1.x = rect.left;
		dot1.y = rect.top-dbValue;
		dot.x	 = dot0.x;
		dot.y	 = dot1.y;	
		AddCircleStroke(pStroke, dot, dot0, dot1);

		dot2.x = rect.left;
		dot2.y = rect.bottom + dbValue;
		dot3.x = rect.left + dbValue;
		dot3.y = rect.bottom;
		dot.x  = dot3.x;
		dot.y  = dot2.y;
		AddCircleStroke(pStroke, dot, dot2, dot3);
		
		dot4.x = rect.right - dbValue;
		dot4.y = rect.bottom;
		dot5.x = rect.right;
		dot5.y = rect.bottom + dbValue;
		dot.x  = dot4.x;
		dot.y  = dot5.y;
		AddCircleStroke(pStroke, dot, dot4, dot5);

		dot6.x = rect.right;
		dot6.y = rect.top - dbValue;
		dot7.x = rect.right - dbValue;
		dot7.y = rect.top;
		dot.x  = dot7.x;
		dot.y  = dot6.y;
		AddCircleStroke(pStroke, dot, dot6, dot7);

		pStroke->Add(dot0);
	}
	else if ( 2 == nMode )  //倒角
	{
		dot0.x = rect.left + dbValue;
		dot0.y = rect.top;
		dot1.x = rect.left;
		dot1.y = rect.top-dbValue;
		dot2.x = rect.left;
		dot2.y = rect.bottom + dbValue;
		dot3.x = rect.left + dbValue;
		dot3.y = rect.bottom;
		dot4.x = rect.right - dbValue;
		dot4.y = rect.bottom;
		dot5.x = rect.right;
		dot5.y = rect.bottom + dbValue;
		dot6.x = rect.right;
		dot6.y = rect.top - dbValue;
		dot7.x = rect.right - dbValue;
		dot7.y = rect.top;
		
		pStroke->Add(dot0);
		pStroke->Add(dot0);
		pStroke->Add(dot1);
		pStroke->Add(dot2);
		pStroke->Add(dot3);
		pStroke->Add(dot4);
		pStroke->Add(dot5);
		pStroke->Add(dot6);
		pStroke->Add(dot7);
		pStroke->Add(dot0);
	}
	
	m_list.AddTail(pStroke);

	return TRUE;
}

void CNodeRect::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if ( bFlag )
	{
		//写入头信息
		fs <<  _T("NRCT_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		Serialize_list(fp,bFlag,bUnicode);
		
		//写入尾信息
		fs << _T("NRCT_E");
	}
	else
	{
		while( fs >> str )
		{
			if ( str == _T("QUAD_RANG_B") )
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("PRO_B") )
				m_property.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("SL_B") )
				Serialize_list(fp,bFlag,bUnicode);
			else if ( str == _T("NRCT_E") )
				break;
		}
	}
}



//
void CNodeRect::AddCircleStroke(CStroke *pStroke, CDot dotCenter, CDot dotStart, CDot dotEnd)
{
	m_GArc.CreateArcline(dotCenter, dotStart, dotEnd, pStroke, 2);
}

