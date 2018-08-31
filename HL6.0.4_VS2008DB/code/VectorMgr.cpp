// VectorMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "VectorMgr.h"

#include "Nodeplt.h"
#include "NodeDxf_GP.h"
#include "NodeNC.h"
#include "NodeAI.h"
#include "NodeDst.h"

CVectorMgr g_VectorMgr;

// CVectorMgr
CVectorMgr::CVectorMgr()
{
}


CVectorMgr::~CVectorMgr()
{
	m_nAiType = 0;
	m_nAiFillType = 0;
	m_dbAiFillDis = 0.05;
}


// CVectorMgr ��Ա����
BOOL CVectorMgr::Decode(CString strFileName, CString strFileExt, CTree *pTree, CProperty pty, BOOL bCenter, BOOL bBetter)
{
	if ( pTree == NULL )
		return FALSE;

	CArray<CNodeLine *, CNodeLine *>  arrayNode;
	if ( FALSE == Decode(strFileName, strFileExt, arrayNode, pty, bCenter, bBetter) )
		return FALSE;

	CNodeLine *pNode = NULL;
	for( int i = 0; i < arrayNode.GetSize(); i++ )
	{
		pNode = arrayNode.GetAt(i);
				
		if ( pNode )
		{
			pTree->Add(pNode);
//			CTree treeTemp(pNode);
//			pTree->Group(&treeTemp);
			pNode = NULL;
		}
	}
	
	return TRUE;
}


BOOL CVectorMgr::Decode(CString strFileName, CString strFileExt, CArray<CNodeLine *, CNodeLine *> &arrayNode, CProperty pty, BOOL bCenter, BOOL bBetter)
{
	BOOL bRet = TRUE;
	strFileExt.MakeUpper();
	
	CNodeLine *pNodeTemp = NULL;
	if( 0 == strFileExt.CompareNoCase(_T("PLT") ) )
	{
		CNodePlt *pNode = new CNodePlt();
		if ( NULL == pNode )
			return FALSE;
		
		pNode->SetProperty(PTY_INIT,pty);
		if( pNode->DeCode(strFileName, bCenter, bBetter) )
		{
			while( 1 )
			{
				pNodeTemp  = 	pNode->UnGroupplt();
				if( pNodeTemp == NULL )
				{
					break;
				}
				else
				{
					pNodeTemp->InitQuadRange();
					arrayNode.Add(pNodeTemp);
					pNodeTemp = NULL;
				}
			}
			delete pNode;
		}
		else
		{
			delete pNode;
		}
	}
	else if ( 0 == strFileExt.CompareNoCase(_T("DXF") ) )
	{
		CNodeDxf_GP *pNode = new CNodeDxf_GP();
		if ( NULL == pNode )
			return FALSE;
		
		pNode->SetProperty(PTY_INIT,pty);
		if ( pNode->DeCode(strFileName, bCenter, bBetter))
		{
			while( 1 )
			{
				pNodeTemp  =	pNode->UnGroupDxf_GP();
				if ( pNodeTemp == NULL )
				{
					break;
				}
				else
				{
					pNodeTemp->InitQuadRange();
					arrayNode.Add(pNodeTemp);
				}
			}
			delete pNode;
		}
		else
		{
			delete pNode;
		}
	}
	else if ( 0 == strFileExt.CompareNoCase(_T("NC") ) )
	{
		CNodeNC *pNode = new CNodeNC();
		if ( pNode == NULL )
			return FALSE;
		
		pNode->SetProperty(PTY_INIT,pty);
		if ( pNode->DeCode(strFileName, bCenter, bBetter) )
		{
			while(1)
			{
				pNodeTemp  = 	pNode->UnGroupplt();
				if (pNodeTemp == NULL)
				{
					break;
				}
				else
				{
					pNodeTemp->InitQuadRange();
					arrayNode.Add(pNodeTemp);
					pNodeTemp = NULL;
				}
			}
			delete pNode;
		}
		else
		{
			delete pNode;
		}
	}
	else if( 0 == strFileExt.CompareNoCase(_T("AI")) ) 
	{
		CNodeAI *pNode = new CNodeAI();
		if( pNode == NULL )
			return FALSE;

		pNode->SetProperty(PTY_INIT,pty);
		pNode->m_nAiType = m_nAiType;
		pNode->m_nAiFillType = m_nAiFillType;
		pNode->m_dbAiFillDis = m_dbAiFillDis;
		if( pNode->DeCode(strFileName, bCenter, bBetter) )
		{
			while(1)
			{
				pNodeTemp =	pNode->UnGroupAI();
				if( pNodeTemp == NULL )
				{
					break;
				}
				else
				{
					pNodeTemp->InitQuadRange();
					arrayNode.Add(pNodeTemp);
					pNodeTemp = NULL;
				}
			}
			delete pNode;
		}
		else
		{
			delete pNode;
		}
	}
	else if( 0 == strFileExt.CompareNoCase(_T("DST")) ) 
	{
		CNodeDst *pNode = new CNodeDst();
		if( pNode == NULL )
			return FALSE;

		pNode->SetProperty(PTY_INIT,pty);
		if( pNode->DeCode(strFileName, bCenter, bBetter) )
		{
			while(1)
			{
				pNodeTemp =	pNode->UnGroupDST();
				if( pNodeTemp == NULL )
				{
					break;
				}
				else
				{
					pNodeTemp->InitQuadRange();
					arrayNode.Add(pNodeTemp);
					pNodeTemp = NULL;
				}
			}
			delete pNode;
		}
		else
		{
			delete pNode;
		}
	}
	else 
	{
		bRet = FALSE;
	}
	
	return bRet;
}


BOOL CVectorMgr::SaveAs(CChain *pChain, int nVectorType, int nFlag )
{
	return TRUE;
}