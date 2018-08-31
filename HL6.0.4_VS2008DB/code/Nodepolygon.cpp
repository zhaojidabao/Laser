// NodePolygon.cpp: implementation of the CNodePolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodePolygon.h"
#include "HLView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodePolygon::CNodePolygon()
{
	m_type = NODE_POLYGON;

	m_nPolygon = g_SysCfg.m_nPolygon;
	m_nNum     = g_SysCfg.m_nPolyNum;
	m_nCross   = g_SysCfg.m_nPolyCross;
	m_nStar    = g_SysCfg.m_nPolyStar;

	m_quadRangle = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
}

CNodePolygon::~CNodePolygon()
{
}



BOOL CNodePolygon::Track(CWnd* pWnd, CPoint ptOrg)
{
	if (::GetCapture()  !=  NULL)
		return FALSE;

	BOOL bResult = TRUE;
	AfxLockTempMaps();

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	CDC* pDC;
	pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);

	CDot dtOrg,dtNow;
	dtOrg = ptOrg;
	((CHLView *)pWnd)->LogicToPhysic(dtOrg.x,dtOrg.y);
	dtNow = dtOrg;

	BOOL bMoved = FALSE;

	//消息处理
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	CString str,prompt;
	prompt.LoadString(IDS_PROMPT_POLYGON);
	for(;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		
		if (CWnd::GetCapture()  !=  pWnd)
			break;
		
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				if (bMoved)
					bResult = TRUE;
				else
					bResult = FALSE;

				goto ExitLoop;
				break;
			}
		case WM_MOUSEMOVE:
			{
				if (!bMoved)
					bMoved = TRUE;
				
				Adjust(msg,ptOrg);
				CPoint pointNow;
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				dtNow = pointNow;
				((CHLView *)pWnd)->LogicToPhysic(dtNow.x,dtNow.y);
				
				int mode = pDC->SetROP2(R2_NOTXORPEN);
				CQuad quad;
				quad = CQuad(dtOrg,dtNow);
				XDC	*pXDC = ((CHLView *)pWnd)->PrepareDC(pDC);
				Draw(pXDC);
				Create(quad);
				Draw(pXDC);
				
				pDC->SetROP2(mode);

				//状态行提示
				quad.Normal();

				str.Format(prompt,quad.Width(),quad.Height());
				pFrame->SendMessage(WM_SETMESSAGESTRING, 0,(LPARAM)(LPCTSTR)str);
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

	if (!bResult)
	{
		return FALSE;
	}

	//生成最终对象
	CQuad quad;
	quad = CQuad(dtOrg,dtNow);
	BOOL bRet = Create(quad);

	return bRet;
}

BOOL CNodePolygon::Create(CQuad &rect)
{
	//删除可能存在的数据
	Delete();

	//确定中心，及半径，XY方向缩放比例等
	CStroke *pStroke = NULL;
	CDot center;
	double r(0.0);
	double ratioX(0.0),ratioY(0.0);
	CDot dot;
	double step(0.0);
	step = 360.0 / m_nNum;

	center.x = (rect.left + rect.right) / 2.0;
	center.y = (rect.bottom + rect.top) / 2.0;

	r = min(rect.Height(),rect.Width());
	if (fabs(r) < EPSILON)
	{
		return FALSE;
	}
	int i = 0;
	ratioX = rect.Width()  / r;
	ratioY = rect.Height() / r;
	r = r / 2;
	
	switch(m_nPolygon)
	{
	case POLYGON_LINE:
		{
			pStroke = new CStroke(m_property);
			for( i = 0; i  <=  m_nNum; i++)
			{
				dot.x =  r * cos(PAI * (90 + i * step) / 180) + center.x;
				dot.y =  r * sin(PAI * (90 + i * step) / 180) + center.y;
				pStroke->Add(dot);
			}
			m_list.AddTail(pStroke);
			break;
		}
	case POLYGON_STAR:
		{
			double r1 = r*cos(PAI*step/360)*m_nStar/100;
			
			pStroke = new CStroke(m_property);
			for( i = 0;i<m_nNum;i++)
			{
				dot.x = r*cos(PAI*(90+i*step)/180)+center.x;
				dot.y = r*sin(PAI*(90+i*step)/180)+center.y;
				pStroke->Add(dot);
				
				dot.x = r1*cos(PAI*(90+(i+.5)*step)/180)+center.x;
				dot.y = r1*sin(PAI*(90+(i+.5)*step)/180)+center.y;
				pStroke->Add(dot);
			}
			
			dot.x = r * cos(PAI/2)+center.x;
			dot.y = r * sin(PAI/2)+center.y;
			pStroke->Add(dot);
			
			m_list.AddTail(pStroke);
			break;
		}
	case POLYGON_CROSS:
		{
			BOOL *pList = new BOOL[m_nNum];
			for(i = 0; i<m_nNum; i++)
			{
				pList[i] = TRUE;
			}
			
			for(i = 0;i<m_nNum;i++)
			{
				if (pList[i])
				{
					pStroke = new CStroke(m_property);
					for(int j = i;;j = j+m_nCross+1)
					{
						int k;
						dot.x = r*cos(PAI*(90+j*step)/180)+center.x;
						dot.y = r*sin(PAI*(90+j*step)/180)+center.y;
						pStroke->Add(dot);
						
						k = (j%m_nNum);
						if (pList[k])
							pList[k] = FALSE;
						else
							break;
					}
					m_list.AddTail(pStroke);
				}
			}
			
			delete []pList;
			break;
		}
	}

	Scale(center,ratioX,ratioY);
	return TRUE;
}




void CNodePolygon::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	//把它当成PLT图存笔画信息
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if ( bFlag )
	{
		//写入头信息
		fs << _T("NPOLYGON_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		Serialize_list(fp,bFlag,bUnicode);
		//写入尾信息
		fs << _T("NPOLYGON_E");
	}
	else
	{
		while( fs >> str )
		{
			if ( str == _T("QUAD_RANG_B") )
			{
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
			}
			else if ( str == _T("PRO_B") )
			{
				m_property.Serialize(fp,bFlag,bUnicode);
			}
			else if ( str == _T("SL_B") )
			{
				Serialize_list(fp,bFlag,bUnicode);
			}
			else if ( str == _T("NPOLYGON_E") )
				break;
		}
	}
}




void CNodePolygon::AddRect(CWnd *pWnd)
{
	CHLView *pView =  (CHLView*)pWnd;
	CDC *pDC = pView->GetDC();
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = (CStroke*)m_list.GetNext(pos);
	int nCount = pStroke->m_dotArray.GetSize();
	nCount--;
	CQuad quad;
	CDot dot;
	double dbR = 3;
	pView->ActualLen(dbR);
	CPen RedPen(PS_SOLID,1,RED);
	CPen *pOldPen = pDC->SelectObject(&RedPen);
	CRect rect;
	for(int i = 0;i<nCount;i++)
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
	RedPen.DeleteObject();
	pView->ReleaseDC(pDC);
}

void CNodePolygon::DelRect(CWnd *pWnd)
{
	CHLView *pView = (CHLView*)pWnd;
	CDC *pDC = pView->GetDC();
	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = (CStroke*)m_list.GetNext(pos);
	int nCount = pStroke->m_dotArray.GetSize();
	nCount--;
	CQuad quad;
	CDot dot;
	double dbR = 3;
	pView->ActualLen(dbR);
	CPen RedPen(PS_SOLID,1,RED);
	CPen *pOldPen = pDC->SelectObject(&RedPen);
	int mode = pDC->SetROP2(R2_NOTXORPEN);
	CRect rect;
	for(int i = 0;i<nCount;i++)
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
	RedPen.DeleteObject();
	pView->ReleaseDC(pDC);
}

