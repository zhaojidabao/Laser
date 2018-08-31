// SelSort.cpp: implementation of the CSelSort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "SelSort.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelSort::CSelSort()
{

}

CSelSort::~CSelSort()
{

}


BOOL CSelSort::Track(CWnd* pWnd, CArray<GNODE,GNODE> &ObjList)
{
	// ����Ѿ��д��ڲ�����������˳�
	if (::GetCapture()  !=  NULL)
	{
		return FALSE;
	}
	
	// ��ѭ�������б���ӳ��ģʽ
	AfxLockTempMaps();
	
	//�������
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	CHLView* pView = (CHLView*)pWnd;
	if (!pView)
	{
		return FALSE;
	}
	
	// ȡ�豸������
	CDC* pDC;
	pDC = pWnd->GetDC();
	ASSERT_VALID(pDC);
	CPen pen(PS_SOLID,1,RGB(255,0,0));  
	CPen *pOldPen = pDC->SelectObject(&pen);
	
	CPoint pointNow;
	CPoint pointOld;
	CPoint pointTemp;
	BOOL bLine = TRUE;
	
	BOOL bRet   = FALSE;
	BOOL bFirst = TRUE;
	
	CProperty propty;
	propty.m_color = RGB(255, 0, 0);
	CStroke *pStroke = new CStroke(propty);
	WORD wStatu;
	BOOL bEnd = FALSE;
	BOOL bLButtonDownPrev(FALSE);
	int mode;
	
	CString strMsg;
	strMsg.LoadString(IDS_SELSORTMSG);
	CString strPopMsg;
	
	//ȡ��Ϣ��ֱ�����㶪ʧ�������ͼ����,�洢�����궼���߼�����
	CWnd *pFrame = ((CWinApp *) AfxGetApp())->m_pMainWnd;
	while (!bEnd)
	{	
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (CWnd::GetCapture()  !=  pWnd)
		{
			bRet = FALSE;
			break;
		}
		
		switch (msg.message)
		{
		case WM_MOUSEWHEEL:
 		case WM_LBUTTONDBLCLK: //˫���˳���ѡ��״̬
			bRet = TRUE;
 			break;
		case WM_LBUTTONDOWN:
			bLButtonDownPrev = TRUE;
			if (bFirst) 
			{
				pointNow.x = (int)(short)LOWORD(msg.lParam);
				pointNow.y = (int)(short)HIWORD(msg.lParam);
				bFirst = FALSE;
			}
			
			bRet = TRUE;
			break;
		case WM_LBUTTONUP:
			if (bLine)
			{
				CDot dt = (CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);
				pStroke->Add(dt);
				pointOld = pointNow;
			}
			else
			{
				CDot dt = (CDot)pointNow;
				pView->LogicToPhysic(dt.x,dt.y);
				pStroke->Add(dt);
				pointOld = pointNow;
				bLine = TRUE;
			}
			bRet = TRUE;
			break;
		case WM_RBUTTONDOWN:
			{
				bEnd = TRUE; 
				mode = pDC->SetROP2(R2_NOTXORPEN);
				pDC->MoveTo(pointOld);
				pDC->LineTo(pointNow); 
				pDC->SetROP2(mode);
				
				int nCunt = ObjList.GetSize();
				DWORD dwCount = GetSort(ObjList, pStroke);
				
				if ( 0  <=  dwCount && (int)dwCount  <=  nCunt )
				{
					strPopMsg.Format(strMsg, nCunt, nCunt - (int)dwCount);
					if (IDYES == AfxMessageBox(strPopMsg, MB_YESNO)) 
					{
						bRet = TRUE;
					}
					else
					{
						bRet = FALSE;
					}
				}
				else
					bRet = FALSE;
				
				break;
			}
		case WM_MOUSEMOVE:

			if (!bFirst) 
			{
				wStatu = msg.wParam; 
				if (wStatu&MK_LBUTTON)
				{
					if (bLButtonDownPrev||(!bLButtonDownPrev&&bLine))
					{
						CDot dt = (CDot)pointNow;
						pView->LogicToPhysic(dt.x,dt.y);
						pStroke->Add(dt); 
						bLButtonDownPrev = FALSE;
					}
					bLine = FALSE;
					pointOld = pointNow;
					pointNow.x = (int)(short)LOWORD(msg.lParam);
					pointNow.y = (int)(short)HIWORD(msg.lParam);
					
					pDC->MoveTo(pointOld);
					pDC->LineTo(pointNow);
					
					CDot dt = (CDot)pointNow;
					pView->LogicToPhysic(dt.x,dt.y);
					pStroke->Add(dt); 
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
					pDC->MoveTo(pointOld);
					pDC->LineTo(pointNow); 
					
					pDC->SetROP2(mode);	
				}
			}// end if (!bFirst) 
			
			DispatchMessage(&msg);
			bRet = TRUE;
			break;

		default:
			DispatchMessage(&msg);
			bRet = TRUE;
			break;
		}
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	pWnd->ReleaseDC(pDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);
	
	if (NULL  !=  pStroke)
	{
		pStroke->RemoveAllPoint();
		delete pStroke;
		pStroke = NULL;
	}
	
	return bRet;
}
