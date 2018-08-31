// NodeAI.cpp: implementation of the CNodeAI class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "NodeAI.h"
#include "NodeLine.h"
#include "Tree.h"
#include "HLDoc.h"
#include "OptStrokeList.h"
#include "HLView.h"
#include "AIDecodeDefine.h"
#include "AiFillMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CNodeAI::CNodeAI(void)
{
	m_type = NODE_AI;
	m_quadRangle = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
	
	m_nAiType = 0;
	m_nAiFillType = 0;
	m_dbAiFillDis = 0.05;
}

CNodeAI::~CNodeAI(void)
{
}



CTree *CNodeAI::UnGroup()
{
	CStroke *pStroke = (CStroke *)m_list.GetHead();
	if( pStroke != NULL )
	{
		CNodeLine *pNode=new CNodeLine(pStroke);
		CTree *pTree=new CTree(pNode);

		m_list.RemoveHead();
		return pTree;
	}
	return NULL;
}


//按图层分解导入的线条
CNodeLine *CNodeAI::UnGroupAI()
{
	POSITION prev=NULL , pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	int nLay = -1;
	BOOL bFirst = TRUE;
	CNodeLine *pNode = NULL;
	
	while(pos != NULL)
	{
		prev = pos;
		pStroke = (CStroke *)m_list.GetNext(pos);

		if( TRUE == bFirst )
		{
			if( pStroke != NULL )
			{
				pNode = new CNodeLine();
				nLay = pStroke->m_property.m_nLayer;
				pNode->m_property.m_color = pStroke->m_property.m_color;
				pNode->m_property.m_nLayer = pStroke->m_property.m_nLayer;
				
				pNode->m_list.AddTail(pStroke);
				m_list.RemoveAt(prev);
				bFirst = FALSE;
			}
		}
		else
		{
			if( pStroke->m_property.m_nLayer == nLay )
			{
				pNode->m_list.AddTail(pStroke);
				m_list.RemoveAt(prev);
			}
		}
	}
	
	return pNode;
}


void CNodeAI::CreatePen(int pen)
{
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	CHLDoc *pDoc = (CHLDoc *) pChild->GetActiveDocument();
	
	CLayer ly=pDoc->GetLayer(pen);
	m_property=ly.GetProperty();
}


void CNodeAI::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if(bFlag)
	{
		//写入头信息
		fs<<_T("NAI_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		Serialize_list(fp,bFlag,bUnicode);
		//写入尾信息
		fs<<_T("NAI_E");
	}
	else
	{
		while( fs>>str )
		{
			if( str == _T("QUAD_RANG_B") )
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
			else if ( str.Left(5) == _T("PRO_B") )
				m_property.Serialize(fp,bFlag,bUnicode);
			else if ( str.Left(4) == _T("SL_B") )
				Serialize_list(fp,bFlag,bUnicode);
			else if ( str.Left(6) == _T("NAI_E") )
				break;
		}
	}
}


void CNodeAI::LoadWheelList(int nType, StrokeList *pListStroke)
{
	if( m_list.GetCount() > 0 )
	{
		POSITION  pos = m_list.GetHeadPosition();
		CStrokeB *pStroke = NULL;
		while(pos != NULL)
		{
			pStroke = m_list.GetNext(pos);

			if(pStroke != NULL)
			{
				CStrokeB *pNew = new CStrokeB(pStroke->m_property);
				if( NULL == pNew )
					continue;

				pNew->Append(*pStroke);
				pListStroke->AddTail(pNew);
			}
		}
	}
}


BOOL CNodeAI::CanUnGroup()
{
	if ( m_list.GetCount() > 0 )
		return TRUE;
	else
		return FALSE;
}



//
BOOL CNodeAI::SetLayerSelect(int nItem, BOOL bFlag)
{
	if ( m_list.GetCount() > 0 )
	{
		CStrokeB *pStroke = NULL;
		POSITION pos = m_list.GetHeadPosition();
		while( pos!=NULL )
		{
			pStroke = m_list.GetNext(pos);			
			if(pStroke->m_property.m_nLayer == nItem)
				return TRUE;
		}
	}

	return FALSE;
}



//////////////////////////////////////////
BOOL CNodeAI::DeCode( const CString& file, BOOL bCenter, BOOL bBetter )  
{
	BOOL bFlag = TRUE;
	if( 0 == m_nAiType )
		bFlag = DeCodeNormalLine( file, bCenter, bBetter );
	else
		bFlag = DeCodeFillLine( file, bCenter, bBetter );
	
	return bFlag;
}




BOOL CNodeAI::DeCodeNormalLine(const CString& file,BOOL bCenter,BOOL bBetter)  
{
	//清空Node中的线条数据
	Delete();
	
	CAITranslate aiTranslate;
	//导出AI文件图层
	if( DECODE_SUCCESS != aiTranslate.Translate( (LPTSTR)(LPCTSTR)file, 0.002) )
		return FALSE;
	
	int nLayer = 0;
	CStroke* pStroke = NULL;
	CDot dot;
	
	//
	CAILayerOut* pLayerOut;
	CAIStroke* pStrokeOut;
	POSITION pos = NULL;
	POSITION posStroke = NULL;
	pos = aiTranslate.m_layerList.GetHeadPosition();
	
	PTOUT ptCenter = aiTranslate.m_rcRange.Center();
	while( NULL != pos )
	{
		// 一个图层一种颜色
		pLayerOut = (CAILayerOut*)aiTranslate.m_layerList.GetNext(pos);
		if( pLayerOut )
		{
			CreatePen(nLayer);
			nLayer++;
			posStroke = pLayerOut->m_strokeList.GetHeadPosition();
			while( NULL != posStroke )	
			{
				// 一个路径一个Stroke
				pStrokeOut = (CAIStroke*)pLayerOut->m_strokeList.GetNext(posStroke);
				if( pStrokeOut  )
				{
					pStroke = new CStroke(m_property);
					// 点坐标转换并居中处理
					for(int i =0; i<pStrokeOut->m_ptArray.GetCount(); i++)
					{
						dot.x = pStrokeOut->m_ptArray[i].x - ptCenter.x;
						dot.y = -(pStrokeOut->m_ptArray[i].y - ptCenter.y);
						pStroke->Add(dot);
					}
					m_list.AddTail(pStroke);
				}
			}
		}
	}

	CalLimit();

	//居中
	if ( bCenter )
	{
		CDot dot = GetLimit().GetBaseDot();
		Move(-dot.x,-dot.y,0);
	}

	//优化数据
	if ( bBetter ) 
	{
		COptStrokeList tmp;
		tmp.Attach(&m_list,0.01);
		tmp.FullOpt();
		tmp.Detach();
	}

	CalLimit();

	return TRUE;

}




BOOL CNodeAI::DeCodeFillLine(const CString& file,BOOL bCenter,BOOL bBetter)  
{
	Delete();
	
	//
	CAiFillMgr aiFillMgr;
	aiFillMgr.m_nScanType = m_nAiFillType;
	aiFillMgr.m_dbScanDis = m_dbAiFillDis;
	BOOL bRet = aiFillMgr.DeCode( file, m_list );
	CalLimit();
	return bRet;
}

