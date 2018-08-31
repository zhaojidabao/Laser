// Chain.cpp: implementation of the CChain class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "Chain.h"
#include "Tree.h"
#include "HLView.h"
#include "KBChar.h"
#include "NodePlt.h"
#include "Mainfrm.h"
#include "MonitorQuad.h"
#include "MgrWheelStroke.h"
#include "DlgProWait.h"
#include "Nodeline.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CChain::selectNull	 = 1;
int CChain::selectSingle	 = 2;
int CChain::selectMul	 = 4;
int CChain::selectGroup	 = 8;
int CChain::selectUnion  = 16;

CUIntArray g_arryIDMap;
extern  CHLApp  theApp;

IMPLEMENT_DYNAMIC(CChain, CChainBH)  

CChain::CChain()
{
	m_bSelMark = FALSE;
	m_bPathOptimize = FALSE;
	m_nPathOptimizeType = 0;
	m_bWheelProStop = FALSE;
	/////////
	::InitializeCriticalSection(&m_lockData);
}

//
CChain::~CChain()
{
	Lock tmp(&m_lockData);

	DeleteAll();
	////
	::DeleteCriticalSection(&m_lockData);
}

//
void CChain::operator = (const CChain& src)
{
	Lock tmp(&m_lockData);
	/////
	DeleteAll();
	m_MgrText = src.m_MgrText;

	CGroupObj *pGroupObj = NULL;
	POSITION pos = src.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)(src.m_listGroup.GetNext(pos));
		if ( !pGroupObj )
			continue;
		
		CGroupObj *pGroupObjNew = new CGroupObj();
		if ( pGroupObjNew  !=  NULL )
		{
			Add( pGroupObjNew );
			pGroupObjNew->CopyToMe(pGroupObj,0x01,TRUE);//两个chain之间拷贝dwStyle用0x01
		}
	}
}

//
void CChain::Add( CGroupObj *pGroupObj )
{
	Lock tmp(&m_lockData);
	
	if ( !pGroupObj )
		return;
	
	pGroupObj->SetParent(this);
	m_listGroup.AddTail(pGroupObj);
}

//
void CChain::Draw( XDC *pDC )
{
	Lock tmp(&m_lockData);
	//
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext( pos );
		if ( !pGroupObj )
			continue;
		
		pGroupObj->Draw( pDC );
	}
}

//
void CChain::Rotate(CDot &dot, double angle, int nPlane)
{
	Lock tmp(&m_lockData);
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		pGroupObj->Rotate(dot,angle,nPlane);
	}
}


//
void CChain::Shear( CDot dot, double dbAngleX, double dbAngleY )
{
	Lock tmp(&m_lockData);
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;

		pGroupObj->Shear(dot, dbAngleX, dbAngleY);
	}
}


//
void CChain::Drag( CDot dot, double ratioX, double ratioY, double moveX, double moveY, double moveZ )
{
	Lock tmp(&m_lockData);

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect())
			continue;
		
		pGroupObj->Drag(dot,ratioX,ratioY,moveX,moveY,moveZ);
	}
}

//
void CChain::Move( double moveX, double moveY, double moveZ )
{
	Lock tmp(&m_lockData);

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		pGroupObj->Move(moveX,moveY,moveZ);
	}
}


//
void CChain::Scale(CDot dot, double ratioX, double ratioY)
{
	Lock tmp(&m_lockData);
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		pGroupObj->Scale(dot,ratioX,ratioY);
	}
}

//
CQuad CChain::Include( CQuad quad,BOOL bMulSelect )
{
	if ( bMulSelect == FALSE )
		SetSelect(FALSE);
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->Include(quad) )
			pGroupObj->InvertSelect();
	}
	
	m_quadLimit = GetLimit();
	return m_quadLimit;
}

//
void CChain::SetSelect(BOOL bFlag)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		pGroupObj->SetSelect(bFlag);
	}
}

//
void CChain::Serialize( FILE* fp, BOOL bFlag, BOOL bUnicode )
{
	Lock tmp(&m_lockData);
	/////////
	CMySerialize sa(fp,bUnicode);
	
	CString str;
	CTree *pTree = NULL;
	CGroupObj *pGroupObj = NULL;
	
	if ( bFlag )
	{
		//写入头信息
		sa << _T("D_B");
		m_MgrText.SaveText(fp,bFlag,bUnicode);
		
		POSITION pos = m_listGroup.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			pGroupObj->Serialize(fp,bFlag,bUnicode);
		}
		
		//写入尾信息
		sa << _T("D_E");
	}
	else
	{
		UINT nIndex = 1;
		g_arryIDMap.RemoveAll();
		///////
		while( sa >> str )
		{
			if ( str == _T("TXT_B") )
			{
				CText* pText = new CText();
				if ( pText == NULL )
				{
					OutputDebugString(_T("CChain::Serialize() new CText() failure!\n"));
					return;
				}
				if ( !SerializeTxT(pText,fp,bUnicode,g_arryIDMap) )
					return;
			}
			else if ( str == _T("GROUP_B") )
			{
				pGroupObj = new CGroupObj();
				if ( pGroupObj == NULL )
				{
					::OutputDebugString(_T("CChain::Serialize() Create CGroupObj error!\n"));
					return;
				}
				pGroupObj->SetParent(this);
				pGroupObj->Serialize(fp, bFlag, bUnicode);
				
				CQuad quad = pGroupObj->GetLimit();				
				if ( quad.IsEmpty() )
				{
					delete pGroupObj;
					pGroupObj = NULL;
				}
				else
				{
					Add(pGroupObj);
				}
			}
			else if ( str == _T("D_E") )//数据结束
				break;
		}
		
		if ( !m_MgrText.GetCopySts() )//普通序列化，非复制粘贴
		{
			m_MgrText.ResetValidID();
			ResetMgrText();
		}
	}
}


void CChain::ResetMgrText()
{
	Lock tmp(&m_lockData);
	//
	UINT   nMinID(0),nMaxID(0);
	BOOL  bValid = m_MgrText.GetValidID(nMinID,nMaxID);
	
	if ( bValid )//需要重新分配ID
	{
		UINT nOffset = nMaxID - nMinID;
		if ( nOffset < 5000 ) //最小ID和最大ID差别不大时简易处理
		{
			m_MgrText.ResetTextID(nMinID,g_arryIDMap,0);//Text队列重新分配
			ResetTextID( nMinID,0 );//NodeText & NodeCodeBar重新分配
		}
		else
		{
			g_arryIDMap.RemoveAll();
			m_MgrText.ResetTextID(nMinID,g_arryIDMap,1);//同上
			ResetTextID( nMinID,1 );
		}
	}
}

void CChain::ResetTextID( int nMinID, int nOpType )
{
	Lock tmp(&m_lockData);
	//
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGrouObj  = NULL;
	while( pos )
	{
		pGrouObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGrouObj )
			continue;

		pGrouObj->ResetTextID( nMinID,nOpType );
	}
}


//
void CChain::SerializeOld( FILE* fp,BOOL bFlag,BOOL bUnicode )
{
	if ( bFlag )	
		return;

	Lock tmp(&m_lockData);
	
	CString   str;
	CMySerialize  sa(fp,bUnicode);
	g_arryIDMap.RemoveAll();
	while( sa >> str )
	{
		str.TrimRight();
		if ( str == _T("TXT_B") )//Text对象
		{
			CText* pText = new CText();
			if ( pText == NULL )
			{
				OutputDebugString(_T("CChain::Serialize_old() new CText() failure!\n"));
				return;
			}
			if ( !SerializeTxT(pText,fp,bUnicode,g_arryIDMap) )
				return;
		}
		else if ( str == _T("T_B") ) //树开始
		{
			CTree *pTree = new CTree();
			if ( pTree == NULL )
				return;
			
			pTree->SetParent(this);
			pTree->Serialize(fp,bFlag,bUnicode);
			CQuad quad = pTree->GetLimit();				
			if ( quad.IsEmpty() )
			{
				delete pTree;
				pTree = NULL;
			}
			else
			{
				CGroupObj *pGroupObj = new CGroupObj();
				if ( pGroupObj  !=  NULL )
				{
					pGroupObj->Add(pTree);
					Add(pGroupObj);
				}
				else
				{
					delete pTree;
					pTree = NULL;
				}
			}
		}
		else if ( str == _T("D_E") ) //数据结束
			break;
	}

	if ( !m_MgrText.GetCopySts() )//普通序列化，非复制粘贴
	{
		m_MgrText.ResetValidID();
		ResetMgrText();
	}
}


//
void CChain::Delete()
{
	Lock tmp(&m_lockData);
	//
	POSITION posPrev = NULL;
	POSITION pos  = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj  = NULL;
	while( pos  !=  NULL )
	{
		posPrev = pos;
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;

		if ( !pGroupObj->GetLockFlg()  )
		{
			m_listGroup.RemoveAt(posPrev);
			//
			delete pGroupObj;
			pGroupObj = NULL;
		}
	}
}

//
void CChain::DeleteAll()
{
	Lock tmp(&m_lockData);

	CGroupObj *pGroupObj  = NULL;
	POSITION pos  = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		delete pGroupObj;
		pGroupObj = NULL;
	}
	
	m_listGroup.RemoveAll();
}


void CChain::UnionGroup()
{
	Lock tmp(&m_lockData);
	
	CGroupObj *pGroupObj = NULL;
	CGroupObj *pGroupObjNew = NULL;
	
	POSITION prev = NULL;
	POSITION pos   = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		prev  = pos;
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		if (  pGroupObj->GetLockFlg() == FALSE  )
		{
			if ( pGroupObjNew == NULL )
			{
				pGroupObjNew = new CGroupObj();
				if ( !pGroupObjNew )
					return;

				pGroupObjNew->SetParent(this);
				m_listGroup.InsertBefore( pos, pGroupObjNew );
			}
			m_listGroup.RemoveAt(prev);
			pGroupObjNew->UnionGroup(pGroupObj);
			
			delete pGroupObj;
			pGroupObj = NULL;
		}
	}
	
	if ( pGroupObjNew )
	{
		pGroupObjNew->SetParent(this);
		pGroupObjNew->SetSelect();
		pGroupObjNew->GetLimit();
	}
}

//
void CChain::UnUnionGroup()
{
	Lock tmp(&m_lockData);
	
	CGroupObj *pGroupObj = NULL;
	POSITION prev = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		prev = pos;
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		if ( !pGroupObj->GetLockFlg() && pGroupObj->GetTreeCount() > 1 )
		{
			pGroupObj->UnUnionGroup(m_listGroup);
			m_listGroup.RemoveAt(prev);
			delete pGroupObj;
		}
	}
}


//
void CChain::Group()
{
	UnionGroup();
	Lock tmp(&m_lockData);
	//
	CGroupObj *pGroupObj = NULL;
	CGroupObj *pGroupObjNew = NULL;
	
	POSITION prev = NULL;
	POSITION pos   = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		prev  = pos;
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE) )
		{
			pGroupObjNew = new CGroupObj();
			pGroupObjNew->SetParent(this);
			m_listGroup.InsertBefore(pos,pGroupObjNew);
			m_listGroup.RemoveAt(prev);
			pGroupObjNew->Group(pGroupObj);

			delete pGroupObj;
			pGroupObj = NULL;
			break;
		}
	}
	
	if ( pGroupObjNew != NULL )
	{
		pGroupObjNew->SetSelect();
		pGroupObjNew->GetLimit();
	}
}

//
void CChain::UnGroup()
{
	Lock tmp(&m_lockData);
	
	CGroupObj *pGroupObj = NULL;
	POSITION prev = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		prev = pos;
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		if ( pGroupObj->GetLockFlg() == FALSE )
		{
			pGroupObj->UnGroup(m_listGroup);
			m_listGroup.RemoveAt(prev);
			delete pGroupObj;
			pGroupObj = NULL;
			break;
		}
	}
}

//
int CChain::GetStatus()
{
	int style = 0;
	int count = 0;
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;

		if ( pGroupObj->GetType() == NODE_COMBIN )
			style  |=  selectGroup;

		if ( pGroupObj->GetType() == NODE_UNION )
			style  |=  selectUnion;

		count++;
		if ( count > 1 )
			return selectMul;

	}
	
	if ( count )
		style |= selectSingle;
	else
		style |= selectNull;
	
	return style;
}


//
CQuad CChain::GetAllLimit()
{
	CQuad quad;
	quad.Initial();
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		quad.Compose(pGroupObj->GetLimit());
	}
	
	return quad;
}


//
BOOL CChain::Mark(HWND hWnd)
{
	if ( m_bPathOptimize )//快速排序打标
	{	
		if ( m_nPathOptimizeType == 0 )
		{
			if ( MarkPathOptimizeByStroke(hWnd) == FALSE )
				return FALSE;
		}
		else if ( m_nPathOptimizeType == 1 )
		{
			if ( MarkPathOptimizeByLay(hWnd) == FALSE )
				return FALSE;
		}
	}

	//所有对象扫描打标，如果是快速排序状态，会自动忽略排序处理对象
	int    nCurPos = 0;
	DWORD nPos  = 0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		//进度条显示处理
		nCurPos++;
		nPos = nCurPos*10/CMarkMgr::m_dwMarkCount;
		if (  !m_bPathOptimize && nPos  !=  CMarkMgr::m_dwCurPos )//非快速排序状态下刷新进度条
		{
			CMarkMgr::m_dwCurPos = nPos;
			::SendMessage( hWnd, WM_MARK_END, MARK_INFO, nPos );
		}
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || ( m_bSelMark && !pGroupObj->IsSelect() ) )
			continue;

		/////标准打标流程
		if ( !pGroupObj->Mark(hWnd) )
			return FALSE;

		/***********************************************************\
		以下为测试代码，检验定制安全性及可靠性
		测试内容：每次打标之前重新构建新临时对象进行标记，标记完成后删除该临时对象
		           重构临时对象时CopyToMe()函数dwStyle此处只允许使用0x00和0x02,不得使
				   用其它值，0x00会重新生成Text对象，0x02复用原Text对象，两种方法都很
				   安全，经过长时间压力测试，可放心根据需要使用。
		\***********************************************************/
		/*
		CGroupObj *pObjNew = new CGroupObj();
		if ( pObjNew  !=  NULL )
		{
			pObjNew->SetParent(this);
			pObjNew->CopyToMe(pGroupObj,0x02,TRUE);//此处dwStyle只允许用0x00,0x02
			pObjNew->Mark(hWnd);
			////////
			delete pObjNew; //使用完毕删除
		}
		*/
	}
	return TRUE;
}


//
BOOL CChain::MarkPathOptimizeByStroke( HWND hWnd )
{
	StrokeList listMark;
	listMark.AddTail(&m_listStroke);

	int    nCurPos = 0;
	DWORD  nPos = 0;
	CStroke *pStroke = NULL;
	POSITION pos = listMark.GetHeadPosition();

	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)listMark.GetNext(pos);
		if ( !pStroke )
			continue;

		CLay* pLay = g_DAT.GetCurLay();
		int  nCount = pLay->GetRepeat(pStroke->m_property.m_nLayer);

		for( int i = 0; i<nCount; i++ )
		{
			if ( HLERROR == g_DAT.Mark(pStroke,hWnd) )
				return FALSE;
			//
			if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
				return FALSE;
		}

		//
		nCurPos++;
		nPos = nCurPos*10/listMark.GetCount();
		::SendMessage(hWnd,WM_MARK_END,MARK_INFO,nPos);
	}

	return TRUE;
}

//
BOOL CChain::MarkPathOptimizeByLay( HWND hWnd )
{
	StrokeList listMark;
	listMark.AddTail(&m_listStroke);
	//
	int nLayCount = g_DAT.GetCurLay()->GetSize();
	int nCurPos = 0;
	DWORD nPos = 0;

	for( int i = 0; i < nLayCount; i++ )
	{
		int nMarkTimes = g_DAT.GetRepeat(i);
		for( int j = 0; j<nMarkTimes; j++ )
		{
			CStroke *pStroke = NULL;
			POSITION pos = listMark.GetHeadPosition();
			while( pos  !=  NULL )
			{
				pStroke = (CStroke *)listMark.GetNext(pos);
				if ( !pStroke )
					continue;

				if ( pStroke->m_property.m_nLayer == i )
				{
					if ( HLERROR == g_DAT.Mark(pStroke,hWnd) )
						return FALSE;
					//
					if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
						return FALSE;
					//
					if ( j == 0 )
					{
						nCurPos++;
						nPos = nCurPos*10/listMark.GetCount();
						::SendMessage(hWnd,WM_MARK_END,MARK_INFO,nPos);
					}
				}
			}
		}
	}
	return TRUE;
}


BOOL CChain::FastMarkPathOptimizeByStroke(HWND hWnd)
{
	StrokeList listMark;
	listMark.AddTail(&m_listStroke);

	int nCurPos = 0;
	DWORD  nPos = 0;
	CStroke *pStroke = NULL;
	POSITION pos = listMark.GetHeadPosition();

	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)listMark.GetNext(pos);
		if ( !pStroke )
			continue;
		
		if ( pStroke->m_bSelect )
		{
			if ( HLERROR == g_DAT.Mark(pStroke,hWnd) )
				return FALSE;		
			
			if ( WAIT_OBJECT_0 == ::WaitForSingleObject(hStopWork,0) )
				return FALSE;
			
			nCurPos++;
			nPos = nCurPos*10/listMark.GetCount();
			::SendMessage(hWnd,WM_FASTMARK_PRO,MARK_INFO,nPos);
		}
	}
	
	return TRUE;
}

//
BOOL CChain::FastMarkPathOptimizeByLay(HWND hWnd)
{
	StrokeList listMark;
	listMark.AddTail(&m_listStroke);
	//
	int nLayCount = g_DAT.GetCurLay()->GetSize();
	int nCurPos = 0;
	DWORD nPos = 0;
	for( int i = 0; i<nLayCount; i++ )
	{
		for( int j = 0; j < g_DAT.GetRepeat(i); j++ )
		{
			CStroke *pStroke = NULL;
			POSITION pos = listMark.GetHeadPosition();
			while( pos  !=  NULL )
			{
				pStroke = (CStroke *)listMark.GetNext(pos);
				if ( !pStroke )
					continue;

				if ( pStroke->m_property.m_nLayer == i && pStroke->m_bSelect)
				{
					if ( HLERROR == g_DAT.Mark(pStroke,hWnd) )
						return FALSE;
					//
					if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
						return FALSE;
					//
					if ( j == 0 )
					{
						nCurPos++;
						nPos = nCurPos*10/listMark.GetCount();
						::SendMessage(hWnd,WM_FASTMARK_PRO,MARK_INFO,nPos);
					}
				}
			}
		}
	}
	return TRUE;
}

//
void CChain::Copy()
{
	m_MgrText.SetCopySts(TRUE);
	//////////
	CTypedPtrList<CObList,CGroupObj *> listGroup;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos )
	{
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		pGroupObj->SetSelect(FALSE);
		CGroupObj *pGroupObjNew = new CGroupObj();
		if ( !pGroupObjNew )
			break;

		pGroupObjNew->SetParent(this);
		listGroup.AddTail(pGroupObjNew);
		pGroupObjNew->CopyToMe(pGroupObj,0x00,TRUE);
		pGroupObjNew->SetSelect(TRUE);
	}

	Lock tmp(&m_lockData);
	pos = listGroup.GetHeadPosition();
	while( pos )
	{
		pGroupObj = (CGroupObj*)listGroup.GetNext(pos);
		Add(pGroupObj);
	}

	//////////
	m_MgrText.SetCopySts(FALSE);
}

//
void CChain::Clipboard( FILE* fp,BOOL bFlag )
{
	Lock tmp(&m_lockData);
	//
	POSITION pos = NULL;	
	CTree  *pTree = NULL;
	CGroupObj *pGroupObj = NULL;
	CChain *pChain = new CChain();
	if ( pChain == NULL )
	{
		AfxMessageBox(_T("Not enough memory,Clipboard stopped!"));
		return;
	}
	
	if ( bFlag )
	{		
		CUIntArray arrayBarID,arrayValidID;
		pos = m_listGroup.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
			if ( !pGroupObj || !pGroupObj->IsSelect() )
				continue;

			pGroupObj->CheckBarCode(arrayBarID,arrayValidID);//获取ID
		}
		//
		pos = m_listGroup.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj || pGroupObj->IsSelect() )
				continue;
			//将相关联未被选中的Group重设状态
			pGroupObj->SelectBarCode(arrayBarID);
		}
		//
		pChain->m_MgrText = m_MgrText;

		pos = m_listGroup.GetHeadPosition();//重新复制需要的对象
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
			if ( !pGroupObj || !pGroupObj->IsSelect() )
				continue;

			CGroupObj *pNew = new CGroupObj();
			if ( pNew  !=  NULL  )
			{
				pChain->Add(pNew);
				pNew->CopyToMe(pGroupObj,0x01,FALSE);//不同chain之间拷贝用0x01
			}
		}
		pChain->Serialize( fp,bFlag, TRUE );
	}
	else
	{
		UINT nBaseID = m_MgrText.GetLastID();
		//将得到当前文档的基本ID传到复制对象中的ID中去
		pChain->m_MgrText.ResetBaseID(nBaseID);
		pChain->m_MgrText.SetCopySts(TRUE);
		pChain->Serialize( fp,bFlag,TRUE );
		pChain->m_MgrText.SetCopySts(FALSE);
		pChain->SetSelect();

		m_MgrText.AddBatch( &(pChain->m_MgrText) );
		SetSelect(FALSE);
		
		pos = pChain->m_listGroup.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj*)pChain->m_listGroup.GetNext(pos);
			if ( pGroupObj == NULL )
				continue;

			CGroupObj * pNewGroup = new CGroupObj();
			if ( pNewGroup  !=  NULL )
			{
				Add(pNewGroup);
				pNewGroup->CopyToMe(pGroupObj,0x01,TRUE);
				pNewGroup->SetSelect(TRUE);
			}
		}
	}

	delete pChain;
}

//
void CChain::ExtendObj( double dbSpace, BOOL bIsKillOldObj, XTracker *pTrack, BOOL bOpt )
{
	Lock tmp(&m_lockData);
	/////////
	CTypedPtrList<CObList,CGroupObj *> listGroup;
	POSITION pos = m_listGroup.GetHeadPosition();
	POSITION prev = NULL;
	while( pos  !=  NULL )
	{
		prev = pos;
		CGroupObj *pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		CGroupObj *pRetGroupObj = pGroupObj->ExtendObj(dbSpace,bOpt);
		if ( pRetGroupObj )
		{
			pRetGroupObj->SetSelect(FALSE);
			pRetGroupObj->SetStrokeList(&pTrack->m_StrokeList);
			listGroup.AddTail(pRetGroupObj);

			if ( bIsKillOldObj )
			{
				m_listGroup.RemoveAt(prev);
				delete pGroupObj;
				pGroupObj = NULL;
			}
		}
	}

	pos = listGroup.GetHeadPosition();
	while( pos )
	{
		CGroupObj *pGroupObj = (CGroupObj*)listGroup.GetNext(pos);
		Add(pGroupObj);
	}
}


//
void CChain::SetFillStyle(FILLDATA& fill,XTracker *pTrack)
{
	Lock tmp(&m_lockData);
	//
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while( pos != NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE))
		{
			pGroupObj->SetFillStyle(fill);
			pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
		}
	}
}


//
void CChain::GetFillStyle(FILLDATA& fill)
{
	CGroupObj *pGroupObj = NULL;
	if ( selectMul & GetStatus() )
	{
		pGroupObj = GetMainSelGroupObj();
		
		if ( pGroupObj  !=  NULL )
		{
			pGroupObj->GetFillStyle(fill);
		}
		return;
	}
	//
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() == TRUE )
		{
			pGroupObj->GetFillStyle(fill);
			return;
		}
	}
}

void CChain::SetProperty(int mode,CProperty pty)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->SetProperty(mode,pty);
	}
}


BOOL CChain::PreprocessMark(DWORD nStyle)
{	
	if ( nStyle == 0x06 )//检查是否有跳号
	{
		return m_MgrText.IsHaveSerialObj();
	}
	else if ( nStyle == 0x05  )//检查跳号是否超出范围
	{
		if ( !m_MgrText.FlashAllText(0x05)  )
		{
			CString str;
			str.LoadString(IDS_PROMPT_JUMP_OVERLAP);
			CWnd *pWnd = ((CWinApp *) AfxGetApp())->m_pMainWnd;
			pWnd->SendMessage(WM_SETMESSAGESTRING, 0,(LPARAM)(LPCTSTR)str);
			AfxMessageBox(str);
			return FALSE;
		}
		return TRUE;
	}
	else 
	{
		m_MgrText.FlashAllText(nStyle);
		///////
		Lock tmp(&m_lockData);
		/////////
		POSITION pos = m_listGroup.GetHeadPosition();
		CGroupObj *pGroupObj = NULL;
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			pGroupObj->PreprocessMark(0x00);
		}
	}
	

	return TRUE;
}


CNode * CChain::GetNode()
{
	int nStyle = GetStatus();
	
	if ( nStyle & selectSingle )
	{
		CGroupObj *pGroupObj = NULL;
		CNode *pNode = NULL;
		POSITION pos = m_listGroup.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			if ( pGroupObj->IsSelect() )
			{
				pNode = pGroupObj->GetNode();
				return pNode;
			}
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
	
	return NULL;
}

//
int CChain::GetMenuIndex()
{
	//根据选择状态确定相应菜单
	int type(0);
	int status = GetStatus();
	if ( status & selectNull )
	{
		type = -1;
	}
	else
	{
		type = NODE_BASE;
		
		if ( status & selectSingle )
		{
			POSITION pos = m_listGroup.GetHeadPosition();
			CGroupObj *pGroupObj = NULL;
			while( pos  !=  NULL )
			{
				pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
				if ( !pGroupObj )
					continue;

				if ( pGroupObj->IsSelect() )
				{
					type = pGroupObj->GetType();
					break;
				}
			}
		}
		
		switch(type)
		{
		case NODE_SERIAL:
			type = 1;
			break;
		case NODE_CODEBAR:
			type = 2;
			break;
		case NODE_BMP:
			type = 3;
			break;
		case NODE_HLTEXT:
			type = 4;
			break;
		case NODE_SPLINE:
			type = 9;
			break;
		case NODE_ARC:
			type = 10;
			break;
		case NODE_CIRCLE:
			type = 11;
			break;
		default:
			type = 0;
			break;
		}
	}
	
	return type;
}

//
void CChain::GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray)
{
	dtArray.RemoveAll();
	
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	CQuad  quad;
	while( pos )
	{	
		pGroupObj   = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		pGroupObj->GetArrayDot(dtArray);
	}
}


//
int CChain::GetCount()
{
	return m_listGroup.GetCount();
}


//
void CChain::GetSelectFlag(BOOL *pFlag)
{
	int i = 0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pFlag[i] = pGroupObj->IsSelect();
		i++;
	}
}

//
CQuad CChain::SetSelectFlag( BOOL *pFlag )
{	
	SetSelect(FALSE);
	
	int i = 0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while(pos)
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->SetSelect(pFlag[i]);
		i++;
	}
	
	m_quadLimit = GetLimit();
	
	return m_quadLimit;
}

//
void CChain::CalLimit(CDot dot,BOOL bAll,double dbXScale,double dbYScale )
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		pGroupObj->GetLimit();
	}
}

//
CQuad CChain::GetLimit()
{
	CQuad quad;
	quad.SetEmpty();
	
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
			quad.Compose(pGroupObj->GetLimit());
	}
	
	return quad;
}

CQuadRangle CChain::GetQuadRangle()
{	
	m_quadLimit = GetLimit();
	CQuadRangle quad(	m_quadLimit.GetBaseDot(BASE_BL),m_quadLimit.GetBaseDot(BASE_TL),
						m_quadLimit.GetBaseDot(BASE_TR),m_quadLimit.GetBaseDot(BASE_BR) );
	
	int nStatus = GetStatus();
	if ( ( nStatus&selectMul ) || ( nStatus&selectGroup ) || ( nStatus&selectUnion ) )
		return quad;
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
			return pGroupObj->GetQuadRangle();
	}
	
	return quad;
}

BOOL CChain::GetFrameDots(CArray<CDot,CDot>& frameDots)
{
	m_quadLimit = GetLimit();
	frameDots.RemoveAll();
	int nStatus = GetStatus();
	if ( ( nStatus&selectMul ) || ( nStatus&selectGroup ) || ( nStatus&selectUnion ) )
	{
		frameDots.Add(m_quadLimit.GetBaseDot(BASE_BL));
		frameDots.Add(m_quadLimit.GetBaseDot(BASE_TL));
		frameDots.Add(m_quadLimit.GetBaseDot(BASE_TR));
		frameDots.Add(m_quadLimit.GetBaseDot(BASE_BR));
		return TRUE;
	}

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() )
		{
			if( pGroupObj->GetFrameDots(frameDots) )
				return TRUE;
		}
	}

	frameDots.Add(m_quadLimit.GetBaseDot(BASE_BL));
	frameDots.Add(m_quadLimit.GetBaseDot(BASE_TL));
	frameDots.Add(m_quadLimit.GetBaseDot(BASE_TR));
	frameDots.Add(m_quadLimit.GetBaseDot(BASE_BR));
	return TRUE;
}

//
void CChain::Array(CArray<OBJPOS, OBJPOS> &dtArray)
{
	int nAyCnt = dtArray.GetSize();
	if ( nAyCnt  <=  0 ) 
		return;

	Lock tmp(&m_lockData);
	/////////
	OBJPOS fDot =  dtArray[0];
	CTypedPtrList<CObList,CGroupObj *>listGroupObj;
	CGroupObj *pGroupObj = NULL;
	
	CQuad quadSel;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
		{
			listGroupObj.AddTail(pGroupObj);
			quadSel.Compose(pGroupObj->GetLimit());
			pGroupObj->SetMainSelect(FALSE);
		}
	}
	
	if ( quadSel.IsEmpty() )
		return;
	
	CDot dotBase = quadSel.GetBaseDot();
	for(int i = 1; i < nAyCnt; i++)
	{
		m_MgrText.SetCopySts(TRUE);
		
		OBJPOS dotNew = dtArray[i];
		pos = listGroupObj.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pGroupObj  = listGroupObj.GetNext(pos);
			if ( !pGroupObj )
				continue;
			
			if ( g_dwJumpCnt  !=  -1 )
			{
				g_dwJumpCnt = i;
			}
			
			CGroupObj *pGroupObjNew = new CGroupObj();
			if ( pGroupObjNew == NULL )
				return;

			Add(pGroupObjNew);
			pGroupObjNew->CopyToMe(pGroupObj,0x00,TRUE);
			pGroupObjNew->Move(dotNew.x - dotBase.x, dotNew.y - dotBase.y,0);
			CDot dotCenter(dotNew.x, dotNew.y);
			pGroupObjNew->Rotate(dotCenter, dotNew.z,0);
		}
	}
	
	pos = listGroupObj.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = listGroupObj.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		pGroupObj->Move(fDot.x - dotBase.x, fDot.y - dotBase.y,0);
		CDot dotCenter(fDot.x, fDot.y);
		pGroupObj->Rotate(dotCenter, fDot.z,0);
		pGroupObj->SetMainSelect(TRUE);
	}
}


//
void CChain::SetLayColor(int nLayer,int nFillLayer, int nFillScalLayer)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
			pGroupObj->SetLayColor(nLayer,nFillLayer,nFillScalLayer);
	}
}


void CChain::SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
			pGroupObj->SetObjColor(nObjIndex, nObjColor, nFillIndex, nFillColor);
	}
}


//
void CChain::GetLayColor(int &nLayer,int &nFillLayer, int &nFillScalLayer)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
		{
			pGroupObj->GetLayColor(nLayer,nFillLayer,nFillScalLayer);
			return;
		}
	}
}

void CChain::GetAllGroupLayColor(int* pLayer,int* pFillLayer, int* pFillScalLayer)
{
	int i = 0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->GetLayColor(pLayer[i],pFillLayer[i],pFillScalLayer[i]);
		i++;
	}
}


void CChain::SetAllGroupLayColor(int* pLayer,int* pFillLayer, int* pFillScalLayer)
{
	int i = 0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->SetLayColor(pLayer[i],pFillLayer[i],pFillScalLayer[i]);
		i++;
	}
}

//
CQuad CChain::GetReFurbishQuad()
{
	CQuad quad;
	quad.SetEmpty();
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() )
			quad.Union(pGroupObj->GetReFurbishQuad());
	}
	
	return quad;
}


//
void CChain::GetSelectedStrokeList(StrokeList *pStrokeList)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
			pGroupObj->GetStrokeList(pStrokeList);
	}
}


//
CQuad CChain::GetMarkRange()
{
    CQuad quad;
	quad.SetEmpty();
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	if ( m_bSelMark )
	{
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			if ( pGroupObj->IsSelect() )
				quad.Union(pGroupObj->GetMarkRange());
		}
	}
	else
	{
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;
			
			quad.Union(pGroupObj->GetMarkRange());
		}
	}
	return quad;
}


UINT CChain::PreTrack()
{
	CStroke Stroke;
	CArray<CDot,CDot>	dotArray;
	GetFrameDots(dotArray);
	for(int i = 0; i<dotArray.GetSize(); i++)
		Stroke.Add(dotArray.GetAt(i));

	while(1)
	{
		if ( HLERROR == g_DAT.Mark(&Stroke,NULL,LINE_MARK)) 
			return HLERROR;

		if ( WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
			break;
	}

	return HLOK;
}

//
BOOL CChain::GetFirstMarkCurrent(double & dbCurrent)
{
	if ( GetCount()  !=  0 )
	{
		POSITION pos = m_listGroup.GetHeadPosition();
		CGroupObj *pGroupObj = NULL;		
		
		while(pos  !=  NULL)
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			if ( m_bSelMark )
			{
				if ( pGroupObj->IsSelect() )
				{
					pGroupObj->GetFirstMarkCurrent(dbCurrent);
					return TRUE;
				}
			}
			else
			{
				pGroupObj->GetFirstMarkCurrent(dbCurrent);		
				return TRUE;
			}
		}
	}
	return FALSE;
}




//
BOOL CChain::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{	
	CGroupObj *pGroupObj = NULL;
	BOOL brtn = TRUE;
	POSITION pos = m_listGroup.GetHeadPosition();
	while(pos  !=  NULL)
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		brtn = brtn && ( pGroupObj->ExportPlt(file,bUnExportFill,bScall100) );
	}
	return brtn;
}

//
void CChain::AddRect(CQuad &quad,CWnd *pWnd)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->Include(quad) )
		{
			pGroupObj->AddRect(pWnd);
		}
	}
}	


//
void CChain::SelectDraw(CGroupObj *pGroupObj,XTracker *pTrack)
{
	pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
}


BOOL CChain::SetStrokeList(StrokeList *pStrokeList, CPoint &point, CWnd *pWnd)
{
	Lock tmp(&m_lockData);
	/////////
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if (pGroupObj->Click(point,pWnd))
		{
			if (!pGroupObj->IsSelect())
			{		
				//取消其它树的选中状态
				SetSelect(FALSE);
				pGroupObj->SetSelect(TRUE);
				pGroupObj->SetStrokeList(pStrokeList);
								
				return FALSE;
			}
			else
			{
				pGroupObj->SetStrokeList(pStrokeList);
				return TRUE;
			}
		}
	}

	return FALSE;
}

//
void CChain::CaptureObject(CQuad &quad, XTracker *pTrack, BOOL bUniSel)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	
	if ( FALSE == bUniSel )
		SetSelect(FALSE);

	pTrack->ClearStrokeList();
	while(pos  !=  NULL)
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->CaptureObject(quad,pTrack);
	}
}


BOOL CChain::DragSelectGroupObj(CDot &dot, double ratioX, double ratioY, double moveX, double moveY, double moveZ)
{
	Lock tmp(&m_lockData);
	/////////
	BOOL bOK = FALSE;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() )
		{
			bOK = TRUE;
			pGroupObj->Drag(dot,ratioX,ratioY,moveX,moveY,moveZ);
		}
	}

	return bOK;
}

//
BOOL CChain::HaveSelectObject()
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() )
			return TRUE;
	}

	return FALSE;
}


//
BOOL CChain::HitOtherObject(CPoint &point,CWnd *pWnd)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( !pGroupObj->IsSelect() && pGroupObj->Click(point,pWnd) )
		{
			return TRUE;
		}
	}
	return FALSE;
}




//bFlag为FALSE时不装载文本与条码数据
//为TRUE时装载文本与条码数据
void CChain::ReLoadStrokeList(StrokeList *pStrokeList,BOOL bFlag)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() )
		{
			pGroupObj->SetStrokeList(pStrokeList,bFlag);
		}
	}
}


// add by zhq
void CChain::LoadAssistantPoints(CArray<DOT,DOT> &pointArray)
{
	pointArray.RemoveAll();
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->LoadAssistantPoints(pointArray);
	}
}


//
void CChain::LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->LoadStrokeList(pStrokeList,bOptimize,nLayNo);
	}
}


//nType: 0对象切分，1轮廓切分， 2综合轮廓， 3综合填充 == == == == == == == == == 8导入选择链表
void CChain::LoadWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( 8 == nType  )
		{
			if ( pGroupObj->IsSelect() )
			{
				pGroupObj->LoadWheelList(arrayList, nType);
			}
			else
			{
				continue;
			}
		}
		else
		{
			pGroupObj->LoadWheelList(arrayList, nType);
		}
	}
}


//
void CChain::ChangeQuad( CPoint &point, XTracker *pTrack,CWnd *pWnd, CArray<SELQUAD ,SELQUAD> &QuadList )
{
	ClearALLMainSelect();

	double  dbMinArea = 100000;
	CGroupObj *pGroupSel = NULL;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos != NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj == NULL )
			continue;

		if ( !pGroupObj->IsSelect() && pGroupObj->Click(point,pWnd) )
		{
			CQuad quad = pGroupObj->GetLimit();
			double dbArea = quad.Height()*quad.Width();
			if ( dbArea < dbMinArea )
			{
				dbMinArea = dbArea;
				pGroupSel = pGroupObj;
			}
		}
	}

	if ( pGroupSel != NULL )
	{
		short  nVKey = GetKeyState(VK_SHIFT);
		if( nVKey & SHIFTED )
		{
			pGroupSel->SetSelect(TRUE);
			pGroupSel->SetStrokeList(&pTrack->m_StrokeList);
			SELQUAD Quad;
			Quad.m_QD   = pGroupSel->GetLimit();
			if( Quad.m_QD.IsEmpty() )
				return;
			Quad.m_bSel = TRUE;
			Quad.m_pGroupObj = (DWORD)pGroupSel;
			QuadList.Add(Quad);
			SetDrawFill(QuadList, -1);
		}
		else
		{
			SetSelect(FALSE);
			pGroupSel->SetSelect(TRUE);
			pTrack->ClearStrokeList();
			QuadList.RemoveAll();
			pGroupSel->SetStrokeList(&pTrack->m_StrokeList);
			SELQUAD Quad;
			Quad.m_QD   = pGroupSel->GetLimit();
			if( Quad.m_QD.IsEmpty() )
				return;
			Quad.m_bSel = TRUE;
			Quad.m_pGroupObj = (DWORD)pGroupSel;
			QuadList.Add(Quad);
			SetDrawFill(QuadList, -1);

			CMainFrame *pMainFram = (CMainFrame*)theApp.m_pMainWnd;
			if( pMainFram )
			{
				pMainFram->SetAllObjecUnSel();
				pMainFram->SetObjecSel(pGroupSel);
			}
		}
	}//
}



//
void CChain::SetDrawFill(CArray<SELQUAD ,SELQUAD> &QuadList, int nIndex)
{
	int nCount = QuadList.GetSize();
	if ( 0  >=  nCount )
		return;
	
	if ((nIndex > (nCount - 1)) && (nCount > 0))
	{
		return;
	}
	
	for(int i = 0; i < nCount; i++)
	{
		QuadList[i].m_bFill = FALSE;
		((CGroupObj*)QuadList[i].m_pGroupObj)->SetMainSelect(FALSE);
	}
	
	if (-1 == nIndex)
	{
		QuadList[nCount - 1].m_bFill = TRUE;
		((CGroupObj*)QuadList[nCount - 1].m_pGroupObj)->SetMainSelect(TRUE);
	}
	else
	{
		QuadList[nIndex].m_bFill = TRUE;
		QuadList[nIndex].m_bSel = TRUE;
		((CGroupObj*)QuadList[nIndex].m_pGroupObj)->SetMainSelect(TRUE);
	}
}


void CChain::SetSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd )
{
	int nCount = QuadList.GetSize();
	if ( 0  >=  nCount )
		return;
	
	for( int i = 0; i < nCount; i++ )
	{
		CGroupObj *pGroupObj = (CGroupObj *)(QuadList[i].m_pGroupObj);
		if ( pGroupObj->Click(point,pWnd) )
		{
			SetDrawFill(QuadList, i);
			break;
		}
	}
}




void CChain::SetUnSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd )
{
	int nCount = QuadList.GetSize();
	if ( 0  >=  nCount )
		return;
	
	for( int i = 0; i < nCount; i++ )
	{
		CGroupObj *pGroupObj = (CGroupObj *)(QuadList[i].m_pGroupObj);
		if ( pGroupObj->Click(point,pWnd) ) 
		{
			pGroupObj->SetSelect(FALSE);
			break;
		}
	}
}




void CChain::GetQuadListInQuad(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &TrakerQD)
{
	CQuad quad;
	quad.SetEmpty();
	
	CMainFrame *pMainFram = (CMainFrame*)theApp.m_pMainWnd;
	if ( pMainFram )
		pMainFram->SetAllObjecUnSel();
	
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		quad  = pGroupObj->GetLimit();
		if ( quad.IsEmpty() ) 
			continue;
		
		if ( TrakerQD.Include(quad) )
		{
			if (!IsInSelQuadList(QuadList, quad))
			{
				SELQUAD QD;
				QD.m_bFill = FALSE;
				QD.m_bSel  = TRUE;
				QD.m_QD    = quad;
				QD.m_pGroupObj = (DWORD)pGroupObj;
				QuadList.Add(QD);
				
				if ( pMainFram )
					pMainFram->SetObjecSel(pGroupObj);
			}
		}
	}
}




BOOL CChain::IsInSelQuadList(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &QD)
{
	int nCount = QuadList.GetSize();
	for( int i = 0; i < nCount; i++ )
	{
		if ( QuadList[i].m_QD.Equal(QD) )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}




//
void CChain::Convert( BOOL bPopMsg )
{
	BOOL bConvertFill = TRUE;
	if ( bPopMsg )
	{
		if ( IDNO == AfxMessageBox(IDS_CONVERTTOLINE, MB_YESNO) )
			return;
	}

	Lock tmp(&m_lockData);
	/////////
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	POSITION prev = NULL;
	while( pos  !=  NULL )
	{
		prev = pos;
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect()  && (pGroupObj->GetLockFlg() == FALSE))
		{
			UINT nResult = pGroupObj->CanConvert();
			if ( nResult  !=  0 )
			{
				CGroupObj *pGroupObjNew = pGroupObj->ConvertPLT();				
				
				m_listGroup.RemoveAt(prev);
				if (pGroupObj  !=  NULL)
				{
					delete pGroupObj;
					pGroupObj = NULL;
				}
				
				if (pGroupObjNew  !=  NULL)
				{
					Add(pGroupObjNew);
				    pGroupObjNew->SetSelect(TRUE);
				}
			}
		} 
	} 
}




//
void CChain::JoinStroke(BOOL bPopMsg,double dbMinDistance)
{
	Lock tmp(&m_lockData);
	/////////

	BOOL bJoinStroke = TRUE;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE))
			pGroupObj->JoinStroke(dbMinDistance);
	}
}



//
void CChain::FilterPoint(double dbPrecis, double dbAngle)
{
	Lock tmp(&m_lockData);
	/////////
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		pGroupObj->FilterPoint(dbPrecis, dbAngle);
	}
}


BOOL CChain::InitSort(CArray<SORTNODE,SORTNODE> &Sort, CWnd *pWnd)
{
	if ( NULL == pWnd )
		return FALSE;
	
	Sort.RemoveAll();
	
	POSITION pos  = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj  = NULL;
	int    nIndex = 0;
	while( pos != NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		CDot dt;
		if (pGroupObj->GetFirstDot(dt))
		{
			SORTNODE SortNode;
			SortNode.m_SPt   = dt;
			SortNode.m_nSort  = nIndex;
            SortNode.m_Color = RGB(128, 128, 128);
			SortNode.m_pGroupObj = (DWORD)pGroupObj;
			Sort.Add(SortNode);
			nIndex++;
		}
	}// end while
	
	CalcMarkDot(Sort);

	return TRUE;
}



BOOL CChain::InitASort(CArray<ASNODE,ASNODE> &Sort)
{
	Sort.RemoveAll();
	
	POSITION pos  = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj  = NULL;
	int    nIndex = 0;
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		CDot sDt;
		if ( pGroupObj->GetFirstDot(sDt) )
		{
			CDot eDt;
			pGroupObj->GetLastDot(eDt);
			ASNODE sNode;
			sNode.m_SDt    = sDt;
			sNode.m_EDt    = eDt;
			sNode.m_nSort  = nIndex;
			sNode.m_pGroupObj  = (DWORD)pGroupObj;
			Sort.Add(sNode);
			nIndex++;
		}
	}
	
	return TRUE;
}



//
void CChain::SetGroupNum(int nNum)
{
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() )
		{
			pGroupObj->SetGroupNum(nNum);
		}
	} 
}

//
BOOL CChain::InitSelSort(CArray<GNODE,GNODE> &Sort)
{	
	Sort.RemoveAll();
	
	POSITION pos  = m_listGroup.GetHeadPosition();
	POSITION prev  = pos;
	CGroupObj *pGroupObj  = NULL;
	int    nIndex = 0;
	while(pos  !=  NULL)
	{
		prev = pos;
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;
		
		CDot dt;
		if ( pGroupObj->GetFirstDot(dt) )
		{	
			GNODE gNode;
			gNode.m_nNum  = nIndex;
			gNode.m_pGroupObj = (DWORD)pGroupObj;
			CQuad qd = pGroupObj->GetLimit();
			gNode.m_rt.left   = qd.left;
			gNode.m_rt.top    = qd.top;
			gNode.m_rt.right  = qd.right;
			gNode.m_rt.bottom = qd.bottom;
			Sort.Add(gNode);
			nIndex++;
		}
	}// end while

	return TRUE;
}

//
void CChain::ConverStrokeCmd()
{
	Lock tmp(&m_lockData);
	/////////
	CNodePlt *pNodePlt = new CNodePlt();
	if ( NULL == pNodePlt )
		return;
	
	int nLayer = 0;
	CGroupObj* pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() || pGroupObj->GetLockFlg() )
			continue;
		//
		if ( pGroupObj->m_bMainSelect )
		{
			nLayer = pGroupObj->GetSelObjLayerNo();
		}
		pGroupObj->ComToOneGroupObj(pNodePlt);
	}
	
	//
	if ( pNodePlt->CalLimit().IsEmpty() )
	{
		delete pNodePlt;
		pNodePlt = NULL;
	}
	else
	{
		ConvertToOneGroupObjDelete();
		CTree *pTree = new CTree(pNodePlt);
		if ( NULL == pTree )
		{
			delete pNodePlt;
			return;
		}
		CGroupObj *pGroupObjNew = new CGroupObj();
		if ( pGroupObjNew )
		{
			pGroupObjNew->Add(pTree);
			pGroupObjNew->SetSelect(TRUE);
			pGroupObjNew->SetLayColor(nLayer, nLayer, nLayer);
			Add(pGroupObjNew);
		}
	}
}

//
void CChain::SelGroupByNum(int nNum, XTracker *pTrack,CWnd *pWnd)
{
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	
	short  nVKey = GetKeyState(VK_SHIFT);
	if (nVKey & SHIFTED)
	{
		while(pos)
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			if ( nNum == pGroupObj->GetGroupNum() )
			{
				pGroupObj->SetSelect(TRUE);
				pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
			}
		}
	}
	else
	{
		SetSelect(FALSE);
		pTrack->ClearStrokeList();
		while( pos )
		{
			pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			if ( nNum == pGroupObj->GetGroupNum() )
			{
				pGroupObj->SetSelect(TRUE);
				pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
			}
		}
	}
}



void CChain::SetAllSelTreeDoubleOutParaAdd(double dbLeftXAdd, double dbLeftYAdd, double dbRightXAdd, double dbRightYAdd)
{
	
}

void CChain::GetAllSelTreeProp(DOUBLEOUTPARA &DbPara, CQuad &QD)
{
	QD = GetLimit();
}

void CChain::SetAllSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns)
{
}

void CChain::GetSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns)
{
}


//
void CChain::DelAllObjInList()
{
	Lock tmp(&m_lockData);
	/////////
	CMainFrame *pMainFram = (CMainFrame*)theApp.m_pMainWnd;
	if ( NULL == pMainFram )
		return;

	pMainFram->m_ObjectListDlg.DelAllClmItem();
}



void CChain::SetGroupObjSel( CGroupObj *pGroupObj, XTracker* pTracker )
{
	if ( ( pGroupObj == NULL) || ( pTracker == NULL ) )
		return;
	
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		CGroupObj * pGroupObjTemp = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObjTemp )
			continue;
		
		if ( pGroupObjTemp == pGroupObj )
		{
			pGroupObj->SetSelect(TRUE);
			pTracker->ClearStrokeList();
			pGroupObj->SetStrokeList(&pTracker->m_StrokeList);
			break;
		}
	}
}

//
void CChain::DelObj( DWORD dwGroupObj )
{
	Lock tmp(&m_lockData);
	/////////
	POSITION prev =  NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	
	CGroupObj *pGroupObj = NULL;
	while( pos  !=  NULL )
	{	
		prev = pos;
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( (DWORD)pGroupObj == dwGroupObj )
		{
			m_listGroup.RemoveAt(prev);
			delete pGroupObj;
		}
	}
}


//
void CChain::ObjListLoad()
{	
	CMainFrame *pMainFram = (CMainFrame*)theApp.m_pMainWnd;
	if ( NULL == pMainFram )
		return;
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while(pos  !=  NULL)
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
 
		pMainFram->m_ObjectListDlg.AddClmItem(pGroupObj);
	}
	pMainFram->m_ObjectListDlg.m_DocObjList.UpdateListCtrl();
}

//
void CChain::ObjListUnLoad() 
{
	CMainFrame *pMainFram = (CMainFrame*)theApp.m_pMainWnd;
	if ( NULL == pMainFram )
		return;

	pMainFram->m_ObjectListDlg.ObjListUnLoad();
}


//
void CChain::AjustTreeForQuad(CArray<SELQUAD,SELQUAD> &quadList)
{
	int j = 0;
	int count = quadList.GetSize();
	for(int i = 0; i<count; i++)
	{
		SELQUAD selQuad = quadList.GetAt(i);
		
		CGroupObj *pGroupObj = (CGroupObj *)selQuad.m_pGroupObj;
		CQuad quad = pGroupObj->GetLimit();
		if ( quad.Equal(selQuad.m_QD) )
			continue;

		double rationX = selQuad.m_QD.Width()/quad.Width();
		double rationY = selQuad.m_QD.Height()/quad.Height();
		double selMidX = (selQuad.m_QD.left + selQuad.m_QD.right)/2;
		double selMidY = (selQuad.m_QD.top + selQuad.m_QD.bottom)/2;
		double midX = (quad.left + quad.right)/2;
		double midY = (quad.top + quad.bottom)/2;
		double moveX = selMidX - midX;
		double moveY = selMidY - midY;
		
		if (fabs(rationX - 1.0) > 0.0001 || fabs(rationY - 1.0) > 0.0001)
		{	
			CDot dot;
			dot.x = selMidX;
			dot.y = selMidY;
			pGroupObj->Drag(dot, rationX, rationY, moveX, moveY,0);
		}
		else
		{
			pGroupObj->Move(moveX,moveY,0);
		}
		j++;
	}
}


//
void CChain::RefreshSelectQuadList(CArray<SELQUAD,SELQUAD> &quadList)
{
	CMainFrame *pMainFram = (CMainFrame*)theApp.m_pMainWnd;
	if ( pMainFram )
	{
		pMainFram->SetAllObjecUnSel();
		pMainFram->m_ObjectListDlg.ObjListLoad();
	}
		
	CQuad quad;
	int  nIndex = -1;

	quadList.RemoveAll();
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while(pos  !=  NULL)
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() )
		{
			SELQUAD QD;
			QD.m_bFill	 =  pGroupObj->IsMainSelect();
			QD.m_bSel  = TRUE;
			QD.m_QD    = pGroupObj->GetLimit();
			quad.Compose(QD.m_QD);
			if ( QD.m_QD.IsEmpty() )
				continue;
			
			QD.m_pGroupObj = (DWORD)pGroupObj;
			quadList.Add(QD);
			
			int nId = pMainFram->SetObjecSel(pGroupObj);
			if ( -1 == nIndex )
			{
				nIndex = nId;
			}
		}
	}
	
	if (-1  !=  nIndex && pMainFram)
	{
		pMainFram->m_ObjectListDlg.m_DocObjList.ScrollToItem(nIndex);
	}
	
	theApp.m_RedPreView.SetQuadArray(quadList);
	m_quadLimit = quad;
}


//
double CChain::GetTotalLength()
{
	double total = 0;
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while(pos  !=  NULL)
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		total  +=  pGroupObj->GetLength();
	}
	
	return total;
}


//
void CChain::OnMAlign(CArray<SELQUAD,SELQUAD> &quadList, int nType)
{	
	CMonitorQuad monitorQuad(this);
	monitorQuad.DoQuadCmd(nType, quadList);
}


//
BOOL CChain::IsHaveTextOrCodeBar()
{	
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() &&  pGroupObj->IsHaveTxtOrCodeBar() && (pGroupObj->GetLockFlg() == FALSE) )
			return TRUE;
	}
	
	return FALSE;
}


//
void CChain::ConvertToOneGroupObjDelete()
{
	Lock tmp(&m_lockData);
	/////////
	POSITION prev = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while(pos  !=  NULL)
	{
		prev = pos;
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( (pGroupObj->IsSelect() == TRUE) && pGroupObj->GetType()  !=  NODE_DOT && pGroupObj->GetType()  !=  NODE_BMP )
		{
			m_listGroup.RemoveAt(prev);
			delete pGroupObj;
		}
	}
}

//
void CChain::UpdateObjMarkSet(OBJMARKSET &ObjMarkSet)
{
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	while(pos  !=  NULL)
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;
		
		if ( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE) )
		{
			pGroupObj->m_MarkSet = ObjMarkSet;
			pGroupObj->UpdateObjMarkSet(ObjMarkSet);
		}
	}
}


//
void CChain::GetObjMarkSet(OBJMARKSET &ObjMarkSet)
{	
	CGroupObj *pGroupObj = NULL;
	if ( selectMul )
	{
		pGroupObj = GetMainSelGroupObj();
		if ( pGroupObj  !=  NULL )
		{
			ObjMarkSet = pGroupObj->m_MarkSet;
		}
		return;
	}
	
	POSITION pos = m_listGroup.GetHeadPosition();
	while(pos  !=  NULL)
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() )
		{
			ObjMarkSet = pGroupObj->m_MarkSet;
			return;
		}
	}
}


//
void CChain::StoreChain(const CChain& chain)
{
	Lock tmp(&m_lockData);
	//文本内容的复制
	DeleteAll();
	m_MgrText = chain.m_MgrText;

	CGroupObj *pGroupObj = NULL;
	POSITION pos = chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)chain.m_listGroup.GetNext(pos);
		if ( pGroupObj == NULL )
			continue;

		CGroupObj *pGroupNew = new CGroupObj();
		if (  pGroupNew )
		{		
			Add(pGroupNew);
			pGroupNew->CopyToMe(pGroupObj,0x01,FALSE);
		}
	}
}


void CChain::ReStoreChain(const CChain& chain)
{
	Lock tmp(&m_lockData);

	//文本内容的复制
	DeleteAll();
	m_MgrText = chain.m_MgrText;

	CGroupObj *pGroupObj = NULL;
	POSITION pos = chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)chain.m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		CGroupObj *pGroupNew = new CGroupObj();
		if ( pGroupNew )
		{
			Add(pGroupNew);
			pGroupNew->CopyToMe(pGroupObj,0x01,TRUE);
		}
	}
}

// 选中打标对象数据
void CChain::ReStoreSelChain(const CChain& chain)
{
	Lock tmp(&m_lockData);
	//文本内容的复制
	DeleteAll();
	m_MgrText = chain.m_MgrText;

	CGroupObj *pGroupObj = NULL;
	POSITION pos = chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)chain.m_listGroup.GetNext(pos);
		if ( !pGroupObj || (chain.m_bSelMark && !pGroupObj->IsSelect()) )
			continue;

		CGroupObj *pGroupNew = new CGroupObj();
		if ( pGroupNew )
		{
			Add(pGroupNew);
			pGroupNew->CopyToMe(pGroupObj,0x01,TRUE);
		}
	}
}

//
void CChain::SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub)
{
	TREELIST listTree;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;

		pGroupObj->LoadTreeList(listTree);
	}

	//
	CTree *pTree = NULL;
	pos = listTree.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)listTree.GetNext(pos);
		if ( pTree  )
		{
			if ( HLERROR == pTree->SelMark(hWnd, hStop, stSub) ) 
				break;
		}
		
		if ( WAIT_OBJECT_0 == ::WaitForSingleObject(hStop,0) )
			break;
	}
}


//
BOOL CChain::FastMark(HWND hWnd)
{
	if ( m_bPathOptimize )
	{
		if ( m_nPathOptimizeType == 0 )
		{
			if ( FastMarkPathOptimizeByStroke(hWnd) == FALSE )
				return FALSE;
		}
		else
		{
			if ( FastMarkPathOptimizeByLay(hWnd) == FALSE )
				return FALSE;
		}
	}
	
	//
	TREELIST listTree;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj  || !pGroupObj->IsSelect() )
			continue;
		
		pGroupObj->LoadTreeList(listTree);
	}

	//
	CTree  *pTree = NULL;
	int nObjCnt = listTree.GetCount();	
	pos = listTree.GetHeadPosition();

	int   nCurPos  = 0;
	DWORD nPos  = 0;
	while( pos  !=  NULL )
	{
		pTree = (CTree*)listTree.GetNext(pos);
		if ( pTree && HLERROR == pTree->Mark(hWnd) )
			return FALSE;

		if ( WAIT_OBJECT_0 == ::WaitForSingleObject(hStopWork,0) )
			return FALSE;

		//进度条显示处理
		if ( m_listStroke.GetCount() < 1 )//快速排序队列无数据
		{
			nCurPos++;
			nPos = nCurPos*100/nObjCnt;
			::SendMessage( hWnd, WM_FASTMARK_PRO,MARK_INFO, nPos );
		}
	}
	
	return TRUE;
}


void CChain::ClearALLMainSelect()
{	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->SetMainSelect(FALSE);
	}
}

//
BOOL CChain::TextReplace(CString& strTxt,CString& strModelName)
{	
	if ( !m_MgrText.TextReplace(strTxt,strModelName) )
		return FALSE;
	//////
	PreprocessMark(0x00);
	return TRUE;
}

//
void CChain::SetLayerSelect(int nItem,BOOL bFlag)
{	
	POSITION pos = m_listGroup.GetHeadPosition();
	CGroupObj *pGroupObj = NULL;
	
	while(pos  !=  NULL)
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		pGroupObj->SetLayerSelect(nItem,bFlag);
	}
}


//
BOOL CChain::HasObjForProcess()
{	
	return ( m_bSelMark ? HaveSelectObject() : (m_listGroup.GetCount() > 0) ) ;
}


void CChain::AdjustObjectOrder(int nFrom, int nTo)
{
	if ( !( nFrom >= 0 && nFrom < m_listGroup.GetCount() ) ) 
		return;

	if ( !( nTo >= 0 && nTo < m_listGroup.GetCount() ) ) 
		return;

	if ( nFrom == nTo ) 
		return;

	CGroupObj *pFrom = (CGroupObj *)m_listGroup.GetAt(m_listGroup.FindIndex(nFrom));
	POSITION pos = m_listGroup.FindIndex(nTo);
	
	if ( nFrom < nTo ) {
		m_listGroup.InsertAfter(pos, pFrom);
	}
	else{
		m_listGroup.InsertBefore(pos, pFrom);
		nFrom++;
	}

	pos = m_listGroup.FindIndex(nFrom);
	m_listGroup.RemoveAt(pos);
}

void CChain::RefreshBarText(CWnd* pWnd,UINT nTextID)
{
	PreprocessMark(0x00);
	//
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj  !=  NULL )
			pGroupObj->RefreshBarText(pWnd,nTextID);
	}
}

//
void CChain::SetPathOptimizeState(BOOL bState)
{	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj  !=  NULL )
			pGroupObj->SetPathOptimizeState(bState);
	}
}

//
BOOL CChain::GetSelObjLockFlg()
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj && pGroupObj->IsSelect() && pGroupObj->GetLockFlg() )
			return TRUE;
	}
	
	return FALSE;
}

CGroupObj *CChain::GetMainSelGroupObj()
{
	CGroupObj *pGrouObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGrouObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGrouObj && pGrouObj->IsSelect() && pGrouObj->IsMainSelect() )
			return pGrouObj;
	}
	
	if ( pGrouObj  !=  NULL )//如果没有，取最后一个
	{
		pGrouObj->SetMainSelect(TRUE);
		return pGrouObj;
	}
	
	return NULL;
}

//返回值 0正常， 1出错或停止 ，2对象宽度超限
int CChain::CreateWheelStroke(int nProcType,double dbRadius,double dbDivAngle,int nFillNu)
{
	if ( nProcType < -1 || nProcType > 2 )
		return FALSE;
	
	CQuad  quadAll    = GetAllLimit();
	double dbStartPos = quadAll.left;
	CMgrWheelStroke mgrWheelStroke;
	int nRet = 0;
	
	if ( nProcType == -1 )//自动按对象切分
	{
		CArray<SORTOBJ,SORTOBJ> arraySrotObj;
		SORTOBJ sortObj;
		CGroupObj *pGroupObj  = NULL;
		POSITION pos = NULL;
		int i = 0, k = 0;
		
		//
		pos = m_listGroup.GetHeadPosition();
		while( pos  !=  NULL )
		{
			if ( m_bWheelProStop ) 
				return 1;
			
			pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
			if ( !pGroupObj )
				continue;

			SORTOBJ sortObjNew;
			sortObjNew.m_pObj = (DWORD)pGroupObj;
			
			CQuadB quad = pGroupObj->GetLimit();
			CDot dot = quad.GetBaseDot(BASE_ML);
			sortObjNew.m_dbValue = dot.x;
			sortObjNew.m_quad = quad;
			arraySrotObj.Add(sortObjNew);
		}
		
		//数据处理
		int nCount = arraySrotObj.GetSize(), nSize = 0;
		g_dataMng.QuickSort(arraySrotObj, 0, nCount-1, m_bWheelProStop);
		if ( m_bWheelProStop )
			return 1;
		
		nRet = mgrWheelStroke.CreateWheelObj( dbRadius, arraySrotObj, nSize );
		if ( nRet == 1 )
			return 1;
		
		//
		i = 0;
		pos = m_listGroup.GetHeadPosition();
		while(pos  !=  NULL)
		{
			if ( m_bWheelProStop )
				return 1;
			
			sortObj = arraySrotObj.GetAt(i);		
			m_listGroup.SetAt(pos, (CGroupObj *)sortObj.m_pObj);
			
			i++;
			m_listGroup.GetNext(pos);
		}
		
		//重新组合
		k = 0;
		for(k = 0; k<nSize; k++)
		{
			if ( m_bWheelProStop )
				return 1;
			
			SetSelect(FALSE);			
			for(i = 0; i<arraySrotObj.GetSize(); i++)
			{
				if  ( m_bWheelProStop ) 
					return 1;
				
				sortObj = arraySrotObj.GetAt(i);
				if (sortObj.m_nFlag == k)
				{
					pGroupObj = (CGroupObj *)sortObj.m_pObj;
					pGroupObj->SetSelect(TRUE);
				}
			}
			
			Group();
		}
		
		//排序
		SortWheelObj();
		if ( m_bWheelProStop ) 
			return 1;
		
		return nRet;
	}
	else if ( nProcType == 0 )//无损切分
	{
		SortWheelObj();
		ProWheelObj(dbStartPos, dbRadius);
	}
	else if (nProcType == 1) //等角度轮廓切分
	{
		CArray<StrokeList*, StrokeList*>arrayList, arrayListSource;
		LoadWheelList(arrayListSource, 1);
		DeleteAll();
		
		if ( mgrWheelStroke.CreateWheelStroke(arrayList, dbDivAngle, dbRadius, arrayListSource, nProcType, nFillNu)  !=  0 )
		{
			g_dataMng.DeleteArrayList(arrayList);
			g_dataMng.DeleteArrayList(arrayListSource);
			return FALSE;
		}
		
		AddArrayList(arrayList);
		g_dataMng.DeleteArrayList(arrayList);
		g_dataMng.DeleteArrayList(arrayListSource);
		
		ProWheelObj(dbStartPos, dbRadius);		
	}
	else if (nProcType == 2)  //等角度综合切分
	{
		BOOL bRet1 = TRUE, bRet2 = TRUE;
		CArray<StrokeList*, StrokeList*>arrayListContour, arrayListSourceContour;
		CArray<StrokeList*, StrokeList*>arrayListFill, arrayListSourceFill;
		LoadWheelList(arrayListSourceContour, 2);
		LoadWheelList(arrayListSourceFill, 3);
		DeleteAll();
		
		if ( mgrWheelStroke.CreateWheelStroke(arrayListContour, dbDivAngle, dbRadius, arrayListSourceContour, 1, nFillNu)  !=  0 )
		{
			g_dataMng.DeleteArrayList(arrayListContour);
			g_dataMng.DeleteArrayList(arrayListSourceContour);
			bRet1 = FALSE;
		}
		
		if ( mgrWheelStroke.CreateWheelStroke(arrayListFill, dbDivAngle, dbRadius, arrayListSourceFill, 2, nFillNu)  !=  0 )
		{
			g_dataMng.DeleteArrayList(arrayListFill);
			g_dataMng.DeleteArrayList(arrayListSourceFill);
			bRet2 = FALSE;
		}
		
		if ( bRet1 == FALSE && bRet2 == FALSE )
		{
			return FALSE;
		}
		
		if ( bRet1 == TRUE )
		{
			AddArrayList(arrayListContour);
			g_dataMng.DeleteArrayList(arrayListContour);
			g_dataMng.DeleteArrayList(arrayListSourceContour);
		}
		
		if ( bRet2 == TRUE )
		{
			AddArrayList(arrayListFill);
			g_dataMng.DeleteArrayList(arrayListFill);
			g_dataMng.DeleteArrayList(arrayListSourceFill);
		}
		
		ProWheelObj(dbStartPos, dbRadius);
	}
	
	if (m_bWheelProStop == TRUE)
	{
		DeleteAll();
		nRet = 1;
	}
	
	return nRet;
}


//
BOOL CChain::HandleAutoSegment(double dbRadius)
{
	if ( AfxMessageBox(IDS_MAKE_WHEELSTROKE, MB_YESNO)  !=  IDYES )
		return FALSE;
	
	Lock tmp(&m_lockData);
	/////////
	BOOL bRet = FALSE;
	CChain *pChainWheel = new CChain();
	if ( !pChainWheel )
		return FALSE;
	
	CDlgProWait dlg;
	dlg.m_nProcessType = -1;
	dlg.SetChain(pChainWheel, this);
	dlg.DoModal();
	
	if ( dlg.m_nProState == 1 )
	{
		bRet = FALSE;
	}
	else
	{
		if ( dlg.m_nProState == 2 )
		{
			AfxMessageBox(IDS_ERROR_SEGMENT,MB_OK|MB_ICONQUESTION);
		}
		
		DeleteAll();
		ReStoreChain(*pChainWheel);
		bRet = TRUE;
	}
	
	delete pChainWheel;
	
	return bRet;
}



void CChain::AddArrayList(CArray<StrokeList*, StrokeList*> &arrayList)
{
	Lock tmp(&m_lockData);

	CNodePlt *pNode = NULL;
	CGroupObj *pGroupObj = NULL;
	StrokeList *pListStroke = NULL;
	POSITION pos = NULL;
	
	BOOL bFirst = TRUE;
	CProperty propertyTemp;
	int nCount = arrayList.GetSize();
	for(int i = 0; i<nCount; i++)
	{
		pListStroke = arrayList.GetAt(i);
		if ( pListStroke == NULL )
			continue;
		
		pNode = new CNodePlt();
		if ( pNode == NULL )
			return;

		bFirst = TRUE;
		pos = pListStroke->GetHeadPosition();
		CStroke *pStroke = NULL;
		CStrokeB *pStrokeB = NULL;
		while( pos  !=  NULL )
		{
			pStrokeB = pListStroke->GetNext(pos);	
			if ( pStrokeB == NULL )
				continue;
			
			if ( bFirst )
			{
				propertyTemp = pStrokeB->m_property;
				bFirst = FALSE;
			}
			
			pStroke = new CStroke();
			if ( pStroke )
			{
				(*pStroke) = (*pStrokeB);
				pStroke->SetParent((LPVOID*)pNode);
				pNode->m_list.AddTail(pStroke);
				//////	
				delete pStrokeB;
				pStrokeB = NULL;
			}
		}
		//
		if ( pNode->m_list.GetCount() < 1 )
		{
			delete pNode;
			pNode = NULL;
		}
		else
		{
			CTree *pTree = new CTree(pNode);
			if ( pTree == NULL )
			{
				delete pNode;
				return;
			}
			pGroupObj = new CGroupObj();
			if ( pGroupObj == NULL )
			{
				delete pTree;
				return;
			}
			pGroupObj->Add(pTree);
			pGroupObj->SetProperty(PTY_INIT,propertyTemp);
			pGroupObj->SetSelect(TRUE);
			/////
			Add(pGroupObj);
		}
		
		pListStroke->RemoveAll();
		delete pListStroke;
		pListStroke = NULL;
		
		arrayList.SetAt(i, pListStroke);
	}
}

//
void CChain::SortWheelObj()
{
	Lock tmp(&m_lockData);
	/////////
	CArray<SORTOBJ,SORTOBJ> arraySrotObj;
	CGroupObj *pGroupObj  = NULL;
	//
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		if ( m_bWheelProStop ) 
			return;
		
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( !pGroupObj )
			continue;

		SORTOBJ sortObjNew;
		sortObjNew.m_pObj = (DWORD)pGroupObj;

		CQuadB quad = pGroupObj->GetLimit();
		CDot dot = quad.GetBaseDot();
		sortObjNew.m_dbValue = dot.x;

		arraySrotObj.Add(sortObjNew);
	}
	//
	int nCount = arraySrotObj.GetSize();
	g_dataMng.QuickSort(arraySrotObj, 0, nCount-1, m_bWheelProStop);
	//
	int i = 0;
	pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		if ( m_bWheelProStop ) 
			return;
		
		SORTOBJ sortObj = arraySrotObj.GetAt(i);
		m_listGroup.SetAt(pos, (CGroupObj *)sortObj.m_pObj);
		
		i++;
		m_listGroup.GetNext(pos);
	}
}

void CChain::ProWheelObj(double dbStartPos, double dbRadius)
{
	Lock tmp(&m_lockData);

	CGroupObj *pGroupObj  = NULL;
	POSITION pos = NULL, posPre = NULL;
	pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		if ( m_bWheelProStop == TRUE )
			break;
		
		pGroupObj = (CGroupObj*)m_listGroup.GetNext(pos);
		if ( pGroupObj == NULL )
			continue;

		pGroupObj->SetSelect(TRUE);
		CQuad quad = pGroupObj->GetLimit();
		CDot  dotCenter = quad.Center();

		double dbMoveX  = -dotCenter.x;
		//X方向进行缩放
		double dbLen  = 0.5*quad.Width();
		double ratioX = 1;

		if ( dbLen > 0.0001 )
			ratioX = ( 2*dbRadius*sin( dbLen/(2*dbRadius) ) )/dbLen;

		pGroupObj->Scale(dotCenter,ratioX,1.0);
		pGroupObj->Move(dbMoveX,0,0);

		CNodeMotorMove *pNodeMotor = new CNodeMotorMove();
		if ( pNodeMotor )
		{
			pNodeMotor->m_para.nAxis = 0;
			pNodeMotor->m_nMarkMode  = 1;
			pNodeMotor->m_dbMoveAngle = ( (dotCenter.x)/dbRadius )*180.0/PAI;

			CTree *pTreeMove = new CTree(pNodeMotor);
			if ( pTreeMove )
				pGroupObj->InsertHead(pTreeMove);
			else
				delete pNodeMotor;
		}
	}
	
	//电机位置清零
	pos = m_listGroup.GetHeadPosition();
	if ( pos  !=  NULL && m_bWheelProStop == FALSE )
	{
		CNodeMotorPos *pNodeMotorPos = new CNodeMotorPos();
		if ( !pNodeMotorPos )
			return;

		for( int i = 0; i < 4; i++ )
		{
			pNodeMotorPos->m_arrayAxisPos[i] = 0;
		}
		CTree *pTreeMovePos = new CTree(pNodeMotorPos);
		if ( !pTreeMovePos )
		{
			delete pNodeMotorPos;
			return;
		}

		CGroupObj *pGroupObj = new CGroupObj();
		if ( pGroupObj )
		{
			pGroupObj->Add(pTreeMovePos);
			m_listGroup.InsertBefore(pos, pGroupObj);
		}
		else
		{
			delete pNodeMotorPos;
			delete pTreeMovePos;
		}
	}
}

BOOL CChain::HaveFunctionNode()
{
	if ( m_listGroup.GetCount() < 1 )
		return FALSE;
	/////////
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj && pGroupObj->HaveFunctionNode() )
			return TRUE;
	}
	return FALSE;
}


int CChain::GetSelObjLayerNo()
{
	if ( m_listGroup.GetCount() < 1 )
		return -1;
	
	if ( !(GetStatus() & selectSingle) )
		return -1;
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;

		return pGroupObj->GetSelObjLayerNo();
	}
	
	return -1;
}


void CChain::InitQuadRange()
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj  !=  NULL )
		{
			pGroupObj->InitQuadRange();
		}
	}
}


//
BOOL CChain::GetTextExtInfo( CString strObjName, LP_TEXTEXINFO lpTextExtInfo)
{
	if ( strObjName.IsEmpty()  || lpTextExtInfo == NULL )
		return FALSE;
	
	BOOL bRet = FALSE;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL && strObjName  !=  _T("") )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj &&  pGroupObj->m_strObjName == strObjName )
		{
			bRet = pGroupObj->GetTextExtInfo(lpTextExtInfo);
			break;
		}
	}
	
	if ( FALSE == bRet )
	{
		lpTextExtInfo->InitData();
	}
	
	return bRet;
}


BOOL CChain::SetTextExtInfo( CString strObjName, LP_TEXTEXINFO lpTextExtInfo )
{
	if ( lpTextExtInfo == NULL || strObjName == _T("") )
		return FALSE;
	
	BOOL bRet = FALSE;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( pGroupObj && pGroupObj->m_strObjName == strObjName )
		{
			bRet = pGroupObj->SetTextExtInfo(lpTextExtInfo);
			break;
		}
	}
	
	return bRet;
}


BOOL CChain::IsEnable3DEdit()
{
	BOOL bRet = TRUE;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_listGroup.GetNext(pos);
		if ( !pGroupObj || !pGroupObj->IsSelect() )
			continue;

		if ( !pGroupObj->IsEnable3DEdit() )
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

