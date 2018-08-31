// CmdAddNode.cpp: implementation of the CCmdAddNode class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "CmdAddNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/*
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdAddNode::CCmdAddNode(CHLDoc *pDoc,CTree *pTree)
{
	commandName.LoadString(IDS_COMMAND_ADDNODE);
	m_pDoc = pDoc;
	if( pTree )
	{
		m_pTree = new CTree();
		m_pTree->CopyToMe(pTree,0x01);
	}
	m_pNode = NULL;
	m_pFlag = NULL;
	m_pFlag2 = NULL;
	m_bFirst = TRUE;
	if ( pDoc != NULL )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->AddNodeCmd(pTree);
		m_pFlag2 = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag2);
	}
}



CCmdAddNode::CCmdAddNode(CHLDoc *pDoc,CNode *pNode)
{
	commandName.LoadString(IDS_COMMAND_ADDNODE);
	m_pDoc = pDoc;
	m_pTree = NULL;
	if( pNode )
	{
		m_pNode = pNode->CreateNewNode();
		m_pNode->CopyToMe(pNode,0x01);
	}
	m_pFlag = NULL;
	m_pFlag2 = NULL;
	m_bFirst = TRUE;
	if ( pDoc != NULL )
	{
		m_pFlag = new BOOL[m_pDoc->m_chain.GetCount()];
		m_pDoc->m_chain.GetSelectFlag(m_pFlag);
		m_pDoc->AddNodeCmd(pNode);
 		m_pFlag2 = new BOOL[m_pDoc->m_chain.GetCount()];
 		m_pDoc->m_chain.GetSelectFlag(m_pFlag2);
	}
}


CCmdAddNode::~CCmdAddNode()
{
	if ( m_pFlag )
	{
		delete[] m_pFlag;
		m_pFlag = NULL;
	}
	////////
	if ( m_pFlag2 )
	{
		delete[] m_pFlag2;
		m_pFlag2 = NULL;
	}

	if( m_pTree )
	{
		delete m_pTree;
		m_pTree = NULL;
	}

	if( m_pNode )
	{
		delete m_pNode;
		m_pNode = NULL;
	}
}


BOOL CCmdAddNode::Do( )
{
	if ( m_bFirst )
	{
		m_bFirst = FALSE;
	}
	else if ( m_pDoc )
	{
		if( m_pTree )
			m_pDoc->AddNodeCmd(m_pTree);
		else if( m_pNode )
			m_pDoc->AddNodeCmd(m_pNode);

		if( m_pTree )
		{
			CTree* pTree = new CTree();
			pTree->CopyToMe(m_pTree,0x01);
			m_pTree = pTree;
		}
		if( m_pNode )
		{
			CNode* pNode = m_pNode->CreateNewNode();
			//pNode->SetParent(&m_pDoc->m_chain);
			pNode->CopyToMe(m_pNode,0x01);
			m_pNode = pNode;
		}
	}

	return TRUE;
}

BOOL CCmdAddNode::Undo( )
{
	if ( m_pDoc )
	{
		m_pDoc->m_chain.SetSelectFlag(m_pFlag2);
		m_pDoc->m_chain.Delete();
		m_pDoc->m_chain.SetSelectFlag(m_pFlag);
	}
	return TRUE;
}
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCmdAddNode::CCmdAddNode(CHLDoc *pDoc,CTree *pTree)
{
	commandName.LoadString(IDS_COMMAND_ADDNODE);
	m_pDoc=pDoc;
	m_chainOld=pDoc->GetChain();
	m_pDoc->AddNodeCmd(pTree);
	m_chainNew=pDoc->GetChain();
	m_bFirst=TRUE;
}



CCmdAddNode::CCmdAddNode(CHLDoc *pDoc,CNode *pNode)
{
	commandName.LoadString(IDS_COMMAND_ADDNODE);
	m_pDoc=pDoc;
	m_chainOld.StoreChain(pDoc->GetChain());
	m_pDoc->AddNodeCmd(pNode);
	m_chainNew.StoreChain(pDoc->GetChain());
	m_bFirst=TRUE;
}


CCmdAddNode::~CCmdAddNode()
{

}


BOOL CCmdAddNode::Do()
{
	if( m_bFirst )
		m_bFirst=FALSE;
	else
	{
		m_pDoc->SetChain(m_chainNew);
		m_pDoc->ReLoadStrokeList();
	}
	return TRUE;
}

BOOL CCmdAddNode::Undo()
{
	m_pDoc->SetChain(m_chainOld);
	m_pDoc->ReLoadStrokeList();
	return TRUE;
}
