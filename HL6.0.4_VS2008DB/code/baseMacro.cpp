#include "stdafx.h"
#include "baseMacro.h"


CHLView* GetCurActiveView()
{
	CWnd *pWnd = AfxGetApp()->m_pMainWnd;
	if ( pWnd == NULL || !pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
		return NULL;
 
	CHLView *pHView = NULL;
	CFrameWnd *pMainWnd = ((CFrameWnd*)pWnd)->GetActiveFrame();
	if ( pMainWnd == NULL )
		return NULL;
	
	CView *pView = pMainWnd->GetActiveView();
  	if ( NULL == pView || !pView->IsKindOf(RUNTIME_CLASS(CHLView)) )
		return NULL;

	pHView = (CHLView*)pView;
	return pHView;
 }




CHLDoc* GetCurActiveDocument()
{
	CWnd *pWnd = AfxGetApp()->m_pMainWnd;
	if ( pWnd == NULL || !pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) )
		return NULL;
	
	CDocument* pDoc = NULL;
	CMDIFrameWnd *pMainWnd	 =  (CMDIFrameWnd *)pWnd;
	CMDIChildWnd *pChildWnd = pMainWnd->MDIGetActive();
	if ( pChildWnd )
	{
		pDoc = pChildWnd->GetActiveDocument();
	}
	if ( NULL == pDoc || !pDoc->IsKindOf(RUNTIME_CLASS(CHLDoc)) )
		return NULL;
	
	CHLDoc *pHDoc = ( CHLDoc* )pDoc;

	return pHDoc;
}