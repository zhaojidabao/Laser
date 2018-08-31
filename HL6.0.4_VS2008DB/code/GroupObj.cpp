// GroupObj.cpp: implementation of the CGroupObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupObj.h"
#include "baseMacro.h"
#include "Nodetext.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupObj::CGroupObj()
{
	m_bSelect = FALSE;
	m_bMainSelect = FALSE;
	
	m_dwObjId = (DWORD)(++g_dwLastObjId);//目前停用
}


CGroupObj::~CGroupObj()
{
	DeleteAll();
}

BOOL CGroupObj::CopyToMe( CGroupObj *pSrc, DWORD dwStyle,BOOL bGenFill )
{
	if ( pSrc == NULL )
		return FALSE;

	DeleteAll();
	/////
	m_strObjName = pSrc->m_strObjName;
	m_bSelect = pSrc->m_bSelect;
	
	CTree *pTree = NULL;
	POSITION pos = pSrc->m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)(pSrc->m_list.GetNext(pos));
		if ( pTree  !=  NULL )
		{
			CTree *pTreeNew = new CTree();
			if ( pTreeNew  !=  NULL )
			{
				Add(pTreeNew) ;
				pTreeNew->CopyToMe(pTree,dwStyle);	
				if ( bGenFill ) //拷贝时需要生成填充线
					pTreeNew->Fill();
			}
		}
	}
	SetSelect(m_bSelect);

	return TRUE;
}

//
CQuad CGroupObj::GetLimit()
{
	CQuad quad;
	quad.Initial();
	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			quad.Compose( pTree->CalLimit() );
	}
	
	return quad;
}

BOOL CGroupObj::Mark( HWND hWnd )
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( !pTree || HLOK  !=  pTree->Mark(hWnd) )
			return FALSE;			
	}

	return TRUE;
}


void CGroupObj::Rotate(CDot &dot, double angle, int nPlane)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Rotate(dot,angle,nPlane);
	}
}


void CGroupObj::UnGroup( CTypedPtrList<CObList,CGroupB *> &listGroupObj )
{
	CTree *pTree = NULL;
	POSITION prev = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		prev = pos;
		pTree = (CTree*)m_list.GetNext( pos );
		
		if ( pTree )
		{
			CTree *pTreeNew = NULL;
			CTree *pTreePre = NULL;
			//////////
			while( pTreeNew = pTree->UnGroup() )
			{
				pTreeNew->SetSelect();
				Add(pTreeNew);
				pTreePre = pTreeNew;
			}
			
			if ( pTreePre )
			{
				delete pTree;
				pTree = NULL;	
				m_list.RemoveAt(prev);
			}
		}
	}
	//
	UnUnionGroup(listGroupObj);
}

//
void CGroupObj::Group(CGroupObj *pGroupObj)
{
	if ( pGroupObj == NULL )
		return;
	
	UnionGroup(pGroupObj);
	//
	CTree *pTree    = NULL;
	CTree *pTreeNew  = NULL;
	
	BOOL  bCreate = FALSE;
	POSITION prev = NULL;
	POSITION pos   = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		prev   = pos;
		pTree = (CTree*)m_list.GetNext(pos);
		
		if ( pTree &&  pTree->CanGroup() )
		{
			if ( !bCreate )//stroke未创建
			{
				pTreeNew = new CTree();
				if ( pTreeNew == NULL )
				{
					OutputDebugString(_T("CGroupObj::Group() new CTree failure!"));
					return;
				}
				bCreate = TRUE;
				pTreeNew->SetParent(this);
				m_list.InsertBefore(pos,pTreeNew);
			}
			if ( pTreeNew )
			{
				m_list.RemoveAt(prev);
				pTreeNew->Group(pTree);
				delete pTree;
				pTree = NULL;
			}
		}
	}
}

//
void CGroupObj::DeleteAll()
{	
	POSITION pos  = m_list.GetHeadPosition();
	CTree *pTree  = NULL;
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{	
			delete pTree;
			pTree = NULL;
		}
	}

	m_list.RemoveAll();
}



void CGroupObj::Serialize(FILE* fp,BOOL bFlag, BOOL bUnicode)
{
	CMySerialize sa(fp,bUnicode);

	CTree *pTree = NULL;
	
	if ( bFlag )
	{	
		sa << _T("GROUP_B");
		
		sa << 100;
		sa << m_bSelect;
		
		sa << 101;
		sa << m_strObjName;
		
		POSITION pos = m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{	
			pTree = (CTree*)m_list.GetNext(pos);
			if( pTree )
				pTree->Serialize(fp,bFlag,bUnicode);
		}
		sa << _T("GROUP_E");
	}
	else
	{	
		CString strRead;
		while( sa >> strRead )
		{
			if ( strRead == _T("GROUP_E") ){
				break;			
			}
			else if ( strRead == _T("T_B") )			
			{
				pTree = new CTree();
				if ( pTree == NULL )
				{
					OutputDebugString(_T("CGroupObj::Serialize() new CTree error!\n"));
					return;
				}
				pTree->SetParent(this);
				pTree->Serialize(fp,bFlag,bUnicode);
				
				//扩展功能限制
				int nType = pTree->GetType();
				if ( (nType == NODE_MOTORMOVE || nType == NODE_MOTORPOS)  && FALSE == g_DAT.m_btExtendInfo.FunData.bMotorMark )
				{
					delete pTree;
				}
				else
				{
					Add(pTree);
				}
			}

			switch ( _ttoi(strRead) )
			{
			case 100:
				sa >> m_bSelect;
				break;

			case 101:
				sa >> m_strObjName;
				break;
			}
		}
		SetSelect(m_bSelect);
	}
}

void CGroupObj::SetSelect(BOOL bFlag)
{	
	if ( FALSE == bFlag && TRUE == m_bSelect)
	{
		CHLView *pView = (CHLView*)GetCurActiveView();
		if ( pView  !=  NULL )
		{
			if ( pView->m_nDrawTool == DRAWMODEL_DRAG )
				InitQuadRange();
		}		
	}
	////////
	m_bSelect = bFlag;	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->SetSelect(m_bSelect);
	}
}


void CGroupObj::Scale(CDot dot,double ratioX,double ratioY)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Scale(dot,ratioX,ratioY);
	}
}


//
void CGroupObj::Drag(CDot dot, double ratioX, double ratioY, double moveX, double moveY,double moveZ)
{	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Drag(dot,ratioX,ratioY,moveX,moveY,moveZ);
	}
}


void CGroupObj::Move(double moveX,double moveY,double moveZ)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Move(moveX,moveY,moveZ);
	}
}

void CGroupObj::Draw( XDC *pDC )
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		///////
		if ( pTree  !=  NULL  )
			pTree->Draw( pDC );
	}
}

//
BOOL CGroupObj::Add( CTree *pTree )
{
	if ( pTree == NULL )
		return FALSE;
	
	pTree->SetParent(this);
	m_list.AddTail(pTree);
	/////
	return TRUE;
}

//
void CGroupObj::InsertHead(CTree *pTree)
{
	if ( pTree == NULL )
		return;
	
	pTree->SetParent(this);
	m_list.AddHead(pTree);	
}


//
BOOL CGroupObj::IsSelect()
{
	return m_bSelect;
}


BOOL CGroupObj::Include(CQuad quad)
{
	CQuad quadTemp = GetLimit();
	return quad.Include(quadTemp);
}

//
void CGroupObj::InvertSelect()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);		
		if ( pTree  !=  NULL )
			pTree->InvertSelect();
	}
	
	m_bSelect = !m_bSelect;
}



//
void CGroupObj::SetStrokeList( StrokeList *pStrokeList )
{
	CTree *pTree = NULL;
	POSITION pos =  m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			if ( !pTree->m_Fill.bFillFirst )
				pTree->SetStrokeList(pStrokeList);
			///////
			pTree->AddFillToStrokeList(pStrokeList);
			///////
			if ( pTree->m_Fill.bFillFirst )
				pTree->SetStrokeList(pStrokeList);
		}
	}
}



void CGroupObj::Shear(CDot dot, double dbAngleX, double dbAngleY)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Shear(dot, dbAngleX, dbAngleY);
	}
}


BOOL CGroupObj::GetLockFlg()
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			if ( pTree->GetLockFlg() == TRUE )//只要任意Tree被锁定，整个Group锁定
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


void CGroupObj::UnionGroup(CGroupObj *pGroupObj)
{
	if ( pGroupObj == NULL )
		return;
	
	POSITION pos = pGroupObj->m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)(pGroupObj->m_list.GetNext(pos));
		Add(pTree);
	}
	
	pGroupObj->m_list.RemoveAll();
}

void CGroupObj::UnUnionGroup( CTypedPtrList<CObList,CGroupB *> &listGroupObj )
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL)
		{	
			CGroupObj *pGroupObj = new CGroupObj();
			if ( pGroupObj  !=  NULL )
			{
				pGroupObj->SetParent(GetParent());
				pGroupObj->Add(pTree);
				pGroupObj->SetSelect(TRUE);
				listGroupObj.AddTail(pGroupObj);
			}
		}
	}
	
	m_list.RemoveAll();
}


//
BOOL CGroupObj::CheckBarCode(CUIntArray &arrayTextID,CUIntArray &arrayValidID)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree *)m_list.GetNext(pos);	
		if ( pTree  !=  NULL )
		{
			pTree->CheckBarCode(pTree->GetRoot(),arrayTextID,arrayValidID);
		}
	}
	
	return TRUE;
}



void CGroupObj::SelectBarCode(CUIntArray& arrayTextID)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree *)m_list.GetNext(pos);
		////////
		if ( pTree  !=  NULL )
		{
			if ( pTree->SelectBarCode(arrayTextID) == TRUE )
			{
				SetSelect(TRUE);
				break;
			}
		}
	}
}


CGroupObj*  CGroupObj::ExtendObj(double dbSpace, BOOL bOpt)
{
	CGroupObj *pGroupObj = new CGroupObj();
	if ( pGroupObj == NULL )
		return NULL;

	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pTree = (CTree *)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			CTree *pTreeNew = pTree->ExtendObj(dbSpace, bOpt);
			if (pTreeNew  !=  NULL)
			{
				pGroupObj->Add(pTreeNew);
			}
		}
	}

	if ( pGroupObj->m_list.GetCount()  <=  0 )
	{
		delete pGroupObj;
		return NULL;
	}
	else
		return pGroupObj;
}


void CGroupObj::SetStrokeList(StrokeList *pStrokeList, BOOL bFlag)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )		
		{
			if ( !pTree->m_Fill.bFillFirst )
				pTree->SetStrokeList(pStrokeList, bFlag);
			
			pTree->AddFillToStrokeList(pStrokeList);

			if ( pTree->m_Fill.bFillFirst )
				pTree->SetStrokeList(pStrokeList, bFlag);
		}
	}
}



void CGroupObj::SetFillStyle(FILLDATA& fill)
{
	CTree *pTree = NULL;
	POSITION pos =  m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pTree = (CTree *)m_list.GetNext(pos);
		if ( pTree != NULL )
		{
			pTree->SetFillStyle(fill);
		}
	}
}


void CGroupObj::GetFillStyle(FILLDATA& fill)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL)
		{
			pTree->GetFillStyle(fill);
			return;
		}
	}
}


BOOL CGroupObj::IsMainSelect()
{
	return m_bMainSelect;
}

//
void CGroupObj::SetMainSelect(BOOL bSelect)
{
	m_bMainSelect = bSelect;
}


void CGroupObj::SetProperty(int mode,CProperty pty)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree	 = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->SetProperty(mode,pty);
	}
}


BOOL CGroupObj::PreprocessMark(DWORD nStyle)
{		
	CTree *pTree = NULL;	
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->PreprocessMark(nStyle);
	}

	return TRUE;
}

CNode * CGroupObj::GetNode()
{
	CTree *pTree = NULL;	
	CNode *pNode = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			pNode = pTree->GetRoot();
			if ( pNode && !pNode->GetLockFlg() )
				return pNode;
			else
				return NULL;
		}
	}
	
	return NULL;
}



void CGroupObj::GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray)
{	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	CQuad  quad;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{
			quad  = pTree->GetLimit();
			CDot dt =  quad.GetBaseDot();

			OBJPOS dt6;
			dt6.x = dt.x;
			dt6.y = dt.y;
			/////
			dtArray.Add(dt6);
		}
	}
}


void CGroupObj::CalLimit(CDot dot,BOOL bAll,double dbXScale,double dbYScale )
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->CalLimit();
	}
}



CQuadRangle CGroupObj::GetQuadRangle()
{
	CQuad quadLimit = GetLimit();
	CQuadRangle quad(	quadLimit.GetBaseDot(BASE_BL),quadLimit.GetBaseDot(BASE_TL),
								quadLimit.GetBaseDot(BASE_TR),quadLimit.GetBaseDot(BASE_BR) );

	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		//////
		if ( pTree && pTree->GetRoot()  !=  NULL )
			return pTree->GetRoot()->GetQuadRangle();
	}

	return quad;
}

BOOL CGroupObj::GetFrameDots(CArray<CDot,CDot>& frameDots)
{
	frameDots.RemoveAll();
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		//////
		if ( pTree && pTree->GetRoot()  !=  NULL )
		{
			CNode* pRoot = (CNode*)pTree->GetRoot();
			if( pRoot->GetType() == NODE_HLTEXT )
			{
				CNodeText* pNodeText = (CNodeText*)pRoot;
				if(pNodeText != NULL && pNodeText->GetFrameDots(frameDots) )
				{
					return TRUE;
				}
				else
				{
					CQuadRangle qd = pRoot->GetQuadRangle();
					frameDots.Add(qd.GetBaseDot(BASE_BL));
					frameDots.Add(qd.GetBaseDot(BASE_TL));
					frameDots.Add(qd.GetBaseDot(BASE_TR));
					frameDots.Add(qd.GetBaseDot(BASE_BR));
					return TRUE;
				}
			}
			else
			{
				CQuadRangle qd = pRoot->GetQuadRangle();
				frameDots.Add(qd.GetBaseDot(BASE_BL));
				frameDots.Add(qd.GetBaseDot(BASE_TL));
				frameDots.Add(qd.GetBaseDot(BASE_TR));
				frameDots.Add(qd.GetBaseDot(BASE_BR));
				return TRUE;
			}
		}
	}

	CQuadRangle quadLimit = GetQuadRangle();
	frameDots.Add(quadLimit.GetBaseDot(BASE_BL));
	frameDots.Add(quadLimit.GetBaseDot(BASE_TL));
	frameDots.Add(quadLimit.GetBaseDot(BASE_TR));
	frameDots.Add(quadLimit.GetBaseDot(BASE_BR));
	return TRUE;
}
//
void CGroupObj::SetLayColor(int nLayer,int nFillLayer, int nFillScalLayer)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();

	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->SetLayColor(nLayer,nFillLayer,nFillScalLayer);
	}
}



void CGroupObj::SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{
			pTree->SetObjColor(nObjIndex, nObjColor, nFillIndex, nFillColor);
		}
	}
}


void CGroupObj::GetLayColor(int &nLayer,int &nFillLayer, int &nFillScalLayer)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{
			pTree->GetLayColor(nLayer,nFillLayer, nFillScalLayer);
			return;
		}
	}
}


CQuad CGroupObj::GetReFurbishQuad()
{
	CQuad quad;
	quad.SetEmpty();
	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			quad.Union(pTree->GetReFurbishQuad());
	}

	return quad;	
}

//
void CGroupObj::GetSelectedStrokeList(StrokeList *pStrokeList)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		  pTree->GetStrokeList(pStrokeList);
	}
}

CQuad CGroupObj::GetMarkRange()
{
    CQuad quad;
	quad.SetEmpty();

	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			quad.Union(pTree->GetMarkRange());
	}
	
	return quad;
}


BOOL CGroupObj::GetFirstMarkCurrent(double & dbCurrent)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;		
	while( pos != NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL )
		{
			pTree->GetFirstMarkCurrent(dbCurrent);		
			return TRUE;
		}
	}
	
	return FALSE;
}

//
BOOL CGroupObj::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{	
	CTree *pTree = NULL;
	BOOL  bRtn = TRUE;
	POSITION pos = m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		bRtn  &=  pTree->ExportPlt(file,bUnExportFill,bScall100);
	}
	return bRtn;
}

//
void CGroupObj::AddRect(CWnd *pWnd)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			pTree->AddRect(pWnd);
		}
	}
}	


void CGroupObj::CaptureObject(CQuad &quad,XTracker *pTrack)
{
	if ( Include(quad) == FALSE )
		return;
	
	BOOL bCapture = FALSE;
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		
		if ( pTree  !=  NULL )
		{
			if (    ( pTree->GetType() == NODE_DELAY ) 
				|| ( pTree->GetType() == NODE_INPUT ) 
				|| ( pTree->GetType() == NODE_OUTPUT ) 
				|| ( pTree->GetType() == NODE_MOTORMOVE )
				|| ( pTree->GetType() == NODE_MOTORPOS ) 
				|| ( pTree->GetType() == NODE_CONDITIONS )
				|| ( pTree->GetType() == NODE_CONDITIONE ) 
				|| ( pTree->GetType() == NODE_BUFFERLASER ) 
				|| ( pTree->GetType() == NODE_CCD ) 
				|| ( pTree->GetType() == NODE_POSCALI ) 
				)
				continue;
			
			pTree->SetStrokeList(&pTrack->m_StrokeList);
			bCapture = TRUE;
		}
	}
	
	if  ( bCapture == TRUE )
		SetSelect(TRUE);
}

//
BOOL CGroupObj::Click(CPoint &point,CWnd *pWnd)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->Click(point,pWnd) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CGroupObj::LoadAssistantPoints(CArray<DOT,DOT> &pointArray)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			pTree->LoadAssistantPoints(pointArray);
		}
	}
}

//
void CGroupObj::LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL )
		{
			if ( TRUE == pTree->m_Fill.bFillFirst )
			{
				pTree->LoadFillStrokeList(pStrokeList,bOptimize,nLayNo);
				pTree->LoadStrokeList(pStrokeList,bOptimize,nLayNo);
			}
			else
			{
				pTree->LoadStrokeList(pStrokeList,bOptimize,nLayNo);
				pTree->LoadFillStrokeList(pStrokeList,bOptimize,nLayNo);				
			}			
		}
	}
}


void CGroupObj::LoadWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			pTree->LoadWheelList(arrayList, nType);
			pTree->LoadFillWheelList(arrayList, nType);
		}
	}
}


void CGroupObj::LabelTo3D(LABEL_DATA &data)
{
	StrokeList *pListContour = new StrokeList();
	if ( pListContour == NULL )
		return;

	StrokeList *pListFill = new StrokeList();
	if ( pListFill == NULL )
	{
		delete pListContour;
		return;
	}

	StrokeList *pListFillScal = new StrokeList();
	if ( pListFillScal == NULL )
	{
		delete pListContour;
		delete pListFill;
		return;
	}

	LABEL_NODE *pLabelNodeContour = NULL;
	LABEL_NODE *pLabelNodeFill = NULL;
	LABEL_NODE *pLabelNodeFillScal = NULL;

	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			int nLayer, nFillLayer, nFillScalLayer;
			pTree->GetLayColor(nLayer, nFillLayer, nFillScalLayer);
			
			pListFill->RemoveAll();
			pTree->LoadFillLine(pListFill);
			if ( pListFill->GetSize()  >=  1 )
			{
				pLabelNodeFill = new LABEL_NODE();
				if ( pLabelNodeFill  !=  NULL )
				{
					pLabelNodeFill->bSelected = m_bSelect;
					pLabelNodeFill->iLayer = nFillLayer;
					pLabelNodeFill->nMarkNum = g_DAT.GetRepeat(nFillLayer);
					Create3dDataFromHS(pListFill, pLabelNodeFill);
				}
			}
			
			pListFillScal->RemoveAll();
			pTree->LoadFillContour(pListFillScal);
			if ( pListFillScal->GetSize()  >=  1 )
			{
				pLabelNodeFillScal = new LABEL_NODE();
				if ( pLabelNodeFillScal  !=  NULL )
				{
					pLabelNodeFillScal->bSelected = m_bSelect;
					pLabelNodeFillScal->iLayer = nFillScalLayer;
					pLabelNodeFillScal->nMarkNum = g_DAT.GetRepeat(nFillScalLayer);
					Create3dDataFromHS(pListFillScal, pLabelNodeFillScal);
				}
			}
			
			pListContour->RemoveAll();
			pTree->LoadStrokeList(pListContour,FALSE,-1);
			if ( pListContour->GetSize()  >=  1 )
			{
				pLabelNodeContour = new LABEL_NODE();
				if ( pLabelNodeContour )
				{
					pLabelNodeContour->bSelected = m_bSelect;
					pLabelNodeContour->iLayer = nLayer;
					pLabelNodeContour->nMarkNum = g_DAT.GetRepeat(nLayer);
					Create3dDataFromHS(pListContour, pLabelNodeContour);
				}
			}
			
			if ( TRUE == pTree->m_Fill.bFillFirst )
			{
				if ( pLabelNodeFill  !=  NULL )
				{
					data.Add(pLabelNodeFill);
				}
				
				if ( pLabelNodeFillScal  !=  NULL )
				{
					data.Add(pLabelNodeFillScal);
				}

				if ( pLabelNodeContour  !=  NULL )
				{
					data.Add(pLabelNodeContour);
				}
			}
			else
			{
				if ( pLabelNodeContour  !=  NULL )
				{
					data.Add(pLabelNodeContour);
				}
				
				if ( pLabelNodeFillScal  !=  NULL )
				{
					data.Add(pLabelNodeFillScal);
				}

				if ( pLabelNodeFill  !=  NULL )
				{
					data.Add(pLabelNodeFill);
				}				
			}
			
			pLabelNodeFill = NULL;
			pLabelNodeContour = NULL;
			pLabelNodeFillScal = NULL;
		}
	}

	pListFill->RemoveAll();
	delete pListFill;
	
	pListFillScal->RemoveAll();
	delete pListFillScal;

	pListContour->RemoveAll();
	delete pListContour;
}


void CGroupObj::Create3dDataFromHS( StrokeList *pListStroke, LABEL_NODE *pLabelNode )
{
	if ( pListStroke == NULL || pLabelNode == NULL )
		return;
	
	LABEL_DOT dot;
	LABEL_STROKE *pLabelStroke = NULL;
	CStrokeB *pStroke = NULL;
	int i = 0, j = 0, nCount = 0;
	POSITION pos = NULL;

	pos = pListStroke->GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = pListStroke->GetNext(pos);
		if ( pStroke  !=  NULL )
		{
			nCount = pStroke->m_dotArray.GetSize();
			if ( nCount  <=  0 )
				continue;
			
			pLabelStroke = new LABEL_STROKE();
			if ( !pLabelStroke )
				continue;
			
			for( j = 0; j<nCount; j++ )
			{
				dot.x = pStroke->m_dotArray[j].x;
				dot.y = pStroke->m_dotArray[j].y;
				dot.z = pStroke->m_dotArray[j].z;
				pLabelStroke->Add(dot);
			}
			pLabelNode->list.Add(pLabelStroke);
		}
	}
}



int CGroupObj::GetType()
{	
	int nType = NODE_BASE;
	if ( m_list.GetCount() > 1 )
	{
		nType = NODE_UNION;
	}
	else if (m_list.GetCount() == 1)
	{
		CTree *pTree = (CTree *)m_list.GetHead();
		if ( pTree->IsGroup() == TRUE )
		{
			nType = NODE_COMBIN;
		}
		else
		{
			nType = pTree->GetType();
		}
	}
	
	return nType;
}

//
void CGroupObj::RefreshBarText(CWnd* pWnd,UINT nTextID)
{
	PreprocessMark(0x00);
	//
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL && pTree->GetRoot()  )
			pTree->RefreshBarText(pTree->GetRoot(),pWnd,nTextID);
	}
}

//
void CGroupObj::SetPathOptimizeState(BOOL bState)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			pTree->SetPathOptimizeState(bState);
	}
}

BOOL CGroupObj::GetSelObjLockFlg()
{	
	BOOL bLock = FALSE;
	BOOL bFirst = TRUE;
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree == NULL) 
			continue;
		
		if ( bFirst )
			bLock = pTree->GetLockFlg();
		else
			bLock  |=  pTree->GetLockFlg();
	}

	return bLock;
}

int CGroupObj::GetSelObjLayerNo()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if (  pTree != NULL && pTree->GetRoot() )
			return pTree->GetRoot()->m_property.m_nLayer;
	}
	/////
	return 0;
}

//
void CGroupObj::SetLayerSelect(int nItem,BOOL bFlag)
{	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->SetLayerSelect(nItem,bFlag) == TRUE ) 
		{
			SetSelect(bFlag);
			break;
		}
	}
}

void CGroupObj::DrawExtern(XDC *pDC)
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos  !=  NULL)
	{
		CTree *pTree = (CTree*)m_list.GetNext(pos);
		//////
		if ( pTree != NULL)
			pTree->DrawExtern(pDC);
	}
}

void CGroupObj::UpdateObjMarkSet(OBJMARKSET &ObjMarkSet)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL )
			pTree->m_MarkSet = ObjMarkSet;
	}
}

BOOL CGroupObj::IsBmpNode()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->IsBmpNode() )
			return TRUE;
	}
	return FALSE;
}

BOOL CGroupObj::IsHaveTextOrCodeBar()
{	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->IsHaveTxtOrCodeBar() )
			return TRUE;
	}
	
	return FALSE;
}

double CGroupObj::GetLength()
{
	double dbTotal = 0;
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL )
			dbTotal  +=  pTree->GetLength();
	}
	
	return dbTotal;
}

void CGroupObj::SetGroupNum(int nNum)
{
	m_nGroupNum = nNum;
}

int CGroupObj::GetGroupNum()
{
	return m_nGroupNum;
}

//
BOOL CGroupObj::ComToOneGroupObj(CNode *pNode)
{
	if ( NULL == pNode )
		return FALSE;
	
	BOOL bIsHaveData = FALSE;
	CTree* pTree = NULL;
	BOOL bRet = TRUE;
	
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		//
		if (  !pTree->ComToOneNode(pNode)  )
		{
			bRet = FALSE;
			break;
		}
	}
	
	return bRet;
}



BOOL CGroupObj::GetFirstDot(CDot &dt)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  && pTree->GetFirstDot(dt) )
			return TRUE;
	}
	
	return FALSE;
}

BOOL CGroupObj::GetLastDot(CDot &dt)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  && pTree->GetLastDot(dt) )
			return TRUE;
	}
	
	return FALSE;
}


//
void CGroupObj::FilterPoint(double dbPrecis, double dbAngle)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree) 
			continue;

		pTree->FilterPoint(dbPrecis, dbAngle);
	}
}


void CGroupObj::JoinStroke(double dbMinDistance)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		
		pTree->JoinStroke(dbMinDistance);
	}
}

void CGroupObj::Convert()
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	POSITION prev =  NULL;
	while( pos )
	{
		prev = pos;
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		
		if ( pTree != NULL && pTree->CanConvert() )//转为曲线
		{	
			CTree *pNewTree = pTree->ConvertPLT();				
			CTree *pNewFillTree = pTree->ConvertFillToPLT();
	
			if ( pNewTree )
			{	
				Add(pNewTree);
				pNewTree->SetSelect(TRUE);
			}

			if ( pNewFillTree )
			{
				Add(pNewFillTree);
				pNewFillTree->SetSelect(TRUE);
			}
			
			delete pTree;
			m_list.RemoveAt(prev);
		} 
	} 
}



UINT CGroupObj::CanConvert()
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		
		if ( pTree->CanConvert() == 0 )
			return 0;
	}

	return 1;
}



CGroupObj * CGroupObj::ConvertPLT()
{
	CGroupObj *pGroupObj = new CGroupObj();
	if ( pGroupObj == NULL )
		return NULL;

	POSITION pos = m_list.GetHeadPosition();
	while (pos != NULL)
	{
		CTree *pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree  )
			continue;

		CTree *pTreeNew = pTree->ConvertPLT();
		CTree *pFillTreeNew = pTree->ConvertFillToPLT();

		if ( pTreeNew  !=  NULL )
		{
			pGroupObj->Add(pTreeNew);
		}
		if ( pFillTreeNew  !=  NULL )
		{
			pGroupObj->Add(pFillTreeNew);
		}
	}
	
	return pGroupObj;
}

void CGroupObj::LoadTreeList(TREELIST &listTree)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{	
			listTree.AddTail(pTree);
		}
	}
}

void CGroupObj::GetStrokeList(StrokeList *pStrokeList)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		  pTree->GetStrokeList(pStrokeList);
	}
}

BOOL CGroupObj::IsHaveTxtOrCodeBar()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->IsHaveTxtOrCodeBar() )
			return TRUE;
	}
	
	return FALSE;
}


void CGroupObj::SetSortStatus(BOOL bIsSort)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		
		if ( pTree != NULL)
		{
			pTree->SetSortStatus(bIsSort);
		}
	}
}


BOOL CGroupObj::HaveFunctionNode()
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL && pTree->HaveFunctionNode() )
			return TRUE;
	}
	return FALSE;
}

BOOL CGroupObj::GetRulerPos(CDot &dot, int nAlignType)
{
	CQuad quad;
	quad.SetEmpty();	
	//////
	CTree *pTree =  NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && !pTree->HaveNode(NODE_HLTEXT) )
		{
			quad.Compose(pTree->CalLimit());
		}			
	}

	if ( quad.IsEmpty() )
		return FALSE;

	if ( nAlignType == 0 )
		dot = quad.GetBaseDot(BASE_TM);
	else
		dot = quad.GetBaseDot(BASE_BM);

	return TRUE;
}

//
void CGroupObj::InitQuadRange()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos  !=  NULL)
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			pTree->InitQuadRange(pTree->GetRoot());
		}
	}
}


//
BOOL CGroupObj::GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree == NULL )
			continue;
	
		return pTree->GetTextExtInfo(lpTextExtInfo);
	}

	return FALSE;
}


BOOL CGroupObj::SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree == NULL )
			continue;
		
		return pTree->SetTextExtInfo(lpTextExtInfo);

	}

	return FALSE;
}


void CGroupObj::ResetTextID( int nMinID, int nOpType )
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( !pTree )
			continue;

		pTree->ResetTextID(nMinID, nOpType);
	}
}


BOOL CGroupObj::IsEnable3DEdit()
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && !pTree->IsEnable3DEdit() )
			return FALSE;
	}

	return TRUE;
}


void  CGroupObj::SetTextData(CString &strTxt)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( !pTree )
			continue;

		pTree->ChangeText(pTree->GetRoot(),strTxt);
	}
}

void  CGroupObj::GetTextValue(CString& strTxt)
{
	CString  strTmp;
	CTree  *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( !pTree )
			continue;

		pTree->GetTextValue(pTree->GetRoot(),strTmp);
		if ( !strTmp.IsEmpty() )
			strTxt  +=  strTmp;
	}
}

