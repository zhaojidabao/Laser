// Tree.cpp: implementation of the CTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "Text.h"
#include "Tree.h"
#include "NodeCodebar.h"
#include "NodeText.h"
#include "HLView.h"
#include "Nodeplt.h"
#include "NodeSpline.h"
#include "Nodeline.h"
#include "NodeRect.h"
#include "NodeEllipse.h"
#include "NodeGrid.h"
#include "Nodedxf_Gp.h"
#include "NodePolygon.h"
#include "NodeDib.h"
#include "NodeDelay.h"
#include "NodeInput.h"
#include "NodeOutput.h"
#include "NodeSpline.h"
#include "NodeArc3Pt.h"
#include "NodeCircle.h"
#include "NodeDot.h"
#include "NodeNC.h"
#include "NodeMotorMove.h"
#include "NodeMotorPos.h"
#include "NodeCondition.h"
#include "NodeBufferLaser.h"
#include "NodeCCD.h"
#include "NodePosCali.h"
#include "baseMacro.h"
#include "FillMaker.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CTree,CObject,2000)
CTree::CTree()
{
	m_pRoot = new CNodeV();
	//////
	m_bIsCurSortTree = FALSE;
	m_nGroupNum = -1;
	m_bMainSelect = FALSE;
	//
	m_dbAngle1_Old = 0;
	m_dbAngle2_Old = 0;
	m_dbDistance_Old = -1;
	m_dbSpace1_Old = -1;
	m_dbSpace2_Old = -1;
	m_nStyle_Old = -1;
	m_dbCircleLen_Old = 0;
	m_dbHeadKillLen_Old = 0;
	m_dbTailKillLen_Old = 0;
	m_dbFillAngle = 0;
	m_bPathOptimize = FALSE;
}

//
CTree::CTree(CNode *pNode)
{
	if ( pNode  !=  NULL )
		pNode->SetParent(this);

	m_pRoot = pNode;
	m_bIsCurSortTree = FALSE;
	m_nGroupNum = -1;
	m_bMainSelect = FALSE;
	
	//
	m_dbAngle1_Old = 0;
	m_dbAngle2_Old = 0;
	m_dbDistance_Old = -1;
	m_dbSpace1_Old = -1;
	m_dbSpace2_Old = -1;
	m_nStyle_Old = -1;
	m_dbCircleLen_Old = 0;
	m_dbHeadKillLen_Old = 0;
	m_dbTailKillLen_Old = 0;
	m_dbFillAngle = 0;
	m_bPathOptimize = FALSE;
}

//
CTree::~CTree()
{
	Delete();
}

BOOL CTree::CopyToMe(CTree *pSrc, DWORD dwStyle)
{
	if ( pSrc == NULL )
		return FALSE;

	Delete();
	/////
	m_pRoot	 =  CopyFrom(pSrc->m_pRoot,dwStyle);
	if ( m_pRoot == NULL )
		return FALSE;

	m_bSelect			 =  pSrc->m_bSelect;
	m_Fill					 =  pSrc->m_Fill;
	m_property			 =  pSrc->m_property;
	m_propertyScal	 =  pSrc->m_propertyScal;
	m_bIsCurSortTree = pSrc->m_bIsCurSortTree;
	m_nGroupNum		 =  pSrc->m_nGroupNum;
	m_bMainSelect		 =  pSrc->m_bMainSelect;
	m_MarkSet			 =  pSrc->m_MarkSet;
	m_ID					 =  pSrc->m_ID;
	m_dbFillAngle		 =  pSrc->m_dbFillAngle;
	m_bPathOptimize	 =  pSrc->m_bPathOptimize;
	
	CalLimit();

	return TRUE;
}

void CTree::Add(CNode *pNode)
{
	if ( !pNode  )
		return;

	pNode->SetParent(this);
	if ( m_pRoot->m_pLeft == NULL )
	{
		m_pRoot->m_pLeft = pNode;
	}
	else
	{
		CNode *pNext = m_pRoot->m_pLeft;
		while( pNext->m_pRight )
			pNext = pNext->m_pRight;
		
		pNext->m_pRight = pNode;
	}
}

//
void CTree::Draw( XDC *pDC )
{
	Draw(m_pRoot,pDC);
	/////////
	CStroke *pStroke = NULL;
	POSITION pos = NULL;
	
	if ( ::g_sys.m_bFastDrawEnable )
	{
		pos = m_listDraw.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pStroke = (CStroke *)m_listDraw.GetNext(pos);
			if ( pStroke )
				pStroke->Draw(pDC);
		}
	}
	else
	{
		pos = m_listFill.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pStroke = (CStroke *)m_listFill.GetNext(pos);
			if ( pStroke )
				pStroke->Draw(pDC);
		}
	}


}
	

void CTree::Draw(CNode *pNode,XDC *pDC)
{
	if ( pNode )
	{
		pNode->Draw(pDC);
		/////
		Draw(pNode->m_pLeft,pDC);
		Draw(pNode->m_pRight,pDC);
	}
}


void CTree::CalLimit(CNode *pNode)
{
	if ( pNode )
	{		
		pNode->SetParent(this);
		/////
		m_quadLimit.Compose(pNode->CalLimit());
		CalLimit(pNode->m_pLeft);
		CalLimit(pNode->m_pRight);
	}	
}

CQuad CTree::CalLimit()
{
	m_quadLimit.Initial();
	CalLimit(m_pRoot);
	
	//如果未扩大则不需计算填充线
	if ( (m_Fill.newFileData1.bLoop && m_Fill.newFileData1.dbLoopDist < 0) || (m_Fill.newFileData2.bLoop && m_Fill.newFileData2.dbLoopDist < 0) )
	{
		POSITION pos = m_listFill.GetHeadPosition();
		CStroke *pStroke = NULL;
		while(pos  !=  NULL)
		{
			pStroke = (CStroke*)m_listFill.GetNext(pos);
			if ( pStroke )
				m_quadLimit.Compose(pStroke->CalLimit());
		}
	}
		
	return m_quadLimit;
}


void CTree::Scale( CNode *pNode,CDot dot, double ratioX, double ratioY )
{
	if ( pNode )
	{
		pNode->Scale(dot,ratioX,ratioY);
		Scale(pNode->m_pLeft,dot,ratioX,ratioY);
		Scale(pNode->m_pRight,dot,ratioX,ratioY);
	}	
}



void CTree::Scale(CDot dot, double ratioX, double ratioY)
{
	if ( !m_pRoot || GetLockFlg() )
		return;

	if ( IsGroup() )
		SetSelect(m_pRoot,TRUE);

	Scale(m_pRoot,dot,ratioX,ratioY);
	
	Fill();
	CalLimit();
}



void CTree::Move(double moveX, double moveY, double moveZ)
{
	if ( GetLockFlg() || !m_pRoot )// 如果被锁定，就不移动
		return;
	
	if ( IsGroup()  )	
		SetSelect(m_pRoot,TRUE);
	
	Move(m_pRoot,moveX,moveY,moveZ);
	
	//填充线移动
	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while( pos != NULL )
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( pStroke )
			pStroke->Move(moveX,moveY,moveZ);
	}

	pos = m_listDraw.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)m_listDraw.GetNext(pos);
		if ( pStroke )
			pStroke->Move(moveX,moveY,moveZ);
	}

	CalLimit();
}


void CTree::Move(CNode *pNode,double moveX,double moveY,double moveZ)
{
	if ( pNode )
	{
		pNode->Move(moveX,moveY,moveZ);
		Move(pNode->m_pLeft,moveX,moveY,moveZ);
		Move(pNode->m_pRight,moveX,moveY,moveZ);
	}	
}


//
void CTree::Serialize( FILE* fp,BOOL bFlag,BOOL bUnicode )
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if ( bFlag )
	{
		//写入头信息
		fs  <<  _T("T_B");

		//写入节点数据
		Save(m_pRoot,fp,FALSE,bUnicode);

		//写入尾信息
		fs  <<  _T("T_E");
		fs  <<  _T("T_FILLB");
		fs  <<  100;
		fs  <<  m_dbAngle1_Old;
		fs  <<  101;
		fs << m_dbAngle2_Old;
		fs << 102;
		fs << m_dbDistance_Old;
		fs << 103;
		fs << m_dbSpace1_Old;
		fs << 104;
		fs << m_dbSpace2_Old;
		fs << 105;
		fs << -1;   //m_nStyle_Old;
		fs << 112;
		fs << m_dbCircleLen_Old;
		fs << 113;
		fs << m_MarkSet.nMarkCount;
		fs << _T("MARKSET_B");
		for (int i = 0; i < OBJ_MAX_MARKCOUNT; i++) 
		{
			fs << 114;
			fs << m_MarkSet.stSub[i].nSubCnt;
			fs << 115;
			fs << m_MarkSet.stSub[i].nLayer;
			fs << 116;
			fs << m_MarkSet.stSub[i].bIsMarkFram;
			fs << 117;
			fs << m_MarkSet.stSub[i].bIsMarkFill;
			fs << 118;
			fs << m_MarkSet.stSub[i].bIsFillFirst;
		}
		fs << _T("MARKSET_E");

		fs << 120;
		fs << m_dbHeadKillLen_Old;
		fs << 121;
		fs << m_dbTailKillLen_Old;
		fs << 122;
		fs << m_dbFillAngle;
		
		fs << _T("T_FILLE");
		
		m_property.Serialize(fp,TRUE,bUnicode);
		m_propertyScal.Serialize(fp,TRUE,bUnicode);
		Serialize_FillPara(fp, TRUE,bUnicode);
	}
	else
	{
		delete m_pRoot;
		m_pRoot = Load(fp,bUnicode);
		
		while( fs >> str )
		{
			str.TrimRight();
			if ( str == _T("T_E") || str == _T("T_FILLB") )
				break;
		}
		
		while( fs >> str )
		{
			str.TrimRight();
			if ( str == _T("T_FILLE") )
				break;
			
			switch( _ttoi(str) )
			{
			case 100:
				fs >> m_dbAngle1_Old;
				break;

			case 101:
				fs >> m_dbAngle2_Old;
				break;

			case 102:
				fs >> m_dbDistance_Old;
				break;

			case 103:
				fs >> m_dbSpace1_Old;
				break;

			case 104:
				fs >> m_dbSpace2_Old;
				break;

			case 105:
				fs >> m_nStyle_Old;
				break;

			case 112:
				fs >> m_dbCircleLen_Old;
				break;

			case 113:
				fs >> m_MarkSet.nMarkCount;
				break;
			
			case 120:
				fs >> m_dbHeadKillLen_Old;
				break;
			
			case 121:
				fs >> m_dbTailKillLen_Old;
				break;
				
			case 122:
				fs >> m_dbFillAngle;
				break;
			}
			
			///////////////////////////
			if ( 0 == str.CompareNoCase(_T("MARKSET_B")) ) 
			{
				for( int i = 0; i < OBJ_MAX_MARKCOUNT; i++ ) 
				{
					fs >> str;
					if ( 114 == _ttoi(str) ) 
					{
						fs >> m_MarkSet.stSub[i].nSubCnt;
					}
					
					fs >> str;
					if ( 115 == _ttoi(str) ) 
					{
						fs >> m_MarkSet.stSub[i].nLayer;
					}
					
					fs  >>  str;
					if ( 116 == _ttoi(str) ) 
					{
						fs  >>  m_MarkSet.stSub[i].bIsMarkFram;
					}
					
					fs >> str;
					if ( 117 == _ttoi(str) ) 
					{
						fs >> m_MarkSet.stSub[i].bIsMarkFill;
					}
					
					fs >> str;
					if ( 118 == _ttoi(str) ) 
					{
						fs >> m_MarkSet.stSub[i].bIsFillFirst;
					}

					if ( 0 == str.CompareNoCase(_T("MARKSET_E")) ) 
					{
						break;
					}
				}
			}			
		}
		
		while( fs >> str )
		{
			str.TrimRight();
			if ( str == _T("T_FILLE") )
				break;
			else if ( str == _T("PRO_B") )
				break;
		}
		m_property.Serialize(fp,FALSE,bUnicode);
		//
		if ( g_dwVersion  >=  60001 )
		{
			fs >> str;
			if ( str == _T("PRO_B") )
				m_propertyScal.Serialize(fp,FALSE,bUnicode);
		}
		
		Serialize_FillPara(fp, FALSE,bUnicode);
		
		Fill();
		CalLimit();
		SetSelect(FALSE);
	}
}



//
void CTree::Save(CNode *pNode,FILE* fp,BOOL bLeft,BOOL bUnicode)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp,bUnicode);
	
	if ( pNode )
	{
		if ( bLeft )
			fs << _T("G_B");

		if ( pNode->GetType()  !=  NODE_BASE )//处理本结点类信息
		{
			pNode->Serialize(fp,TRUE,bUnicode);
		}
		Save(pNode->m_pLeft,fp,TRUE,bUnicode);
		Save(pNode->m_pRight,fp,FALSE,bUnicode);
		if ( bLeft )
			fs << _T("G_E");
	}
	else
		fs << _T("NULL");			//告知本节点为空
}


CNode * CTree::Load(FILE* fp,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	CNode* pNode = NULL;

	while( fs >> str )
	{
		str.TrimRight();
		if ( str == _T("NULL") || str == _T("T_E")  )
			break;
	
		if ( str == _T("G_B") )//创建空结点
		{
			pNode = new CNodeV();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->m_pLeft  = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NDIB_B") )
		{
			pNode = new CNodeDib();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NLIN_B") )
		{
			pNode = new CNodeLine();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NSPLIN_B") )
		{
			pNode = new CNodeSpline();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NARC_B") )
		{
			pNode = new CNodeArc3Pt();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NCIRCLE_B") )
		{
			pNode = new CNodeCircle();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NTXT_B") )
		{
			pNode = new CNodeText();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NEPS_B") )
		{
			pNode = new CNodeEllipse();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NRCT_B") )
		{
			pNode = new CNodeRect();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NGRID_B") )
		{
			pNode = new CNodeGrid();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight =  Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NPOLYGON_B") )
		{
			pNode = new CNodePolygon();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight =  Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NPLT_B") )
		{
			pNode = new CNodePlt();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight =  Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NDXF_B") )
		{
			pNode = new CNodeDxf_GP();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NBAR_B") )
		{
			pNode = new CNodeCodebar();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NDELAY_B") )
		{
			pNode = new CNodeDelay();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NINPUT_B") )
		{
			pNode = new CNodeInput();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NOUTPUT_B") )
		{
			pNode = new CNodeOutput();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NDOT_B") )
		{
			pNode = new CNodeDot();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NMOTORMOVE_B") )
		{
			pNode = new CNodeMotorMove();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NMOTORPOS_B") )
		{
			pNode = new CNodeMotorPos();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NCONDITIONS_B") )
		{
			pNode = new CNodeConditionS();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NCONDITIONE_B") )
		{
			pNode = new CNodeConditionE();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NNC_B") )
		{
			pNode =  new CNodeNC();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NBUFFERLASER_B") )
		{
			pNode = new CNodeBufferLaser();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NCCD_B") )
		{
			pNode = new CNodeCCD();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		if ( str == _T("NPOSCALI_B") )
		{
			pNode = new CNodePosCali();
			if ( pNode == NULL )
				return pNode;

			pNode->SetParent(this);
			pNode->Serialize(fp,FALSE,bUnicode);
			pNode->m_pLeft = Load(fp,bUnicode);
			pNode->m_pRight = Load(fp,bUnicode);
			break;
		}
		//为了兼容老的文本方式所以在此处进行了修改
		if ( str == _T("TXT_B") )
		{
			CText* pText = new CText();
			pText->Serialize(fp,FALSE,bUnicode);
			
			CMgrText *pMgrObj = GetCurMgrText();
			if (  pMgrObj  !=  NULL)
			{
				pMgrObj->AddText(pText);
			}
		}
	}

	return pNode;
}

CQuad CTree::GetLimit()
{
	m_quadLimit = CalLimit();
	return m_quadLimit;
}

void CTree::Delete()
{
	Delete(m_pRoot);
	DeleteFill();
}

void CTree::Delete(CNode *pNode)
{
	if ( pNode )
	{
		Delete(pNode->m_pLeft);
		Delete(pNode->m_pRight);
		if ( pNode ) 
		{
			delete pNode;
			pNode = NULL;
		}
	}
}

//
void CTree::Group(CTree *pTree)
{
	CNode *pNode = CopyFrom(pTree->m_pRoot, 0x02);
	if ( pNode  !=  NULL )
	{
		Add(pNode);
		SetSelect(TRUE);
	}
}


//
BOOL CTree::IsGroup()
{
	if ( m_pRoot->m_pLeft  )
		return TRUE;
	else
	{
		if ( m_pRoot->CanUnGroup() )
			return TRUE;
		else
			return FALSE;
	}
}

BOOL CTree::CanGroup()
{
	if ( m_pRoot->m_pLeft )
		return TRUE;
	else
	{
		if ( m_pRoot->CanGroup() )
			return TRUE;
		else
			return FALSE;
	}
}

CTree *CTree::UnGroup()
{
	if ( !m_pRoot )
		return NULL;

	//从一棵树中分解出第一个左节点
	CNode *pNode = m_pRoot->m_pLeft;
	if ( pNode )
	{
		m_pRoot->m_pLeft = pNode->m_pRight;
		pNode->m_pRight = NULL;
		CTree *pTree = new CTree(pNode);

		if ( pTree )
			pTree->CalLimit();
		
		return pTree;
	}
	else
	{
		//如果此节点具有分组能力，则分组之
		BOOL bCanUnGroup = FALSE;

		if   (     m_pRoot->CanUnGroup() 
			   || (  m_pRoot->GetType() == NODE_LINE && m_pRoot->GetList().GetSize() > 0 )
		   )
		{	
			CTree *pTree = m_pRoot->UnGroup();
			if ( pTree )
			{
				pTree->CalLimit();
			}
			return pTree;
		}
		else
			return NULL;
	}
}


//
UINT CTree::SelMark(CNode *pNode, int nLayer, HWND hWnd, HANDLE hStop, BOOL bIsRevMark)
{
	if ( pNode )
	{	
		if ( ::WaitForSingleObject(hStop,0) == WAIT_OBJECT_0 )
			return HLERROR;
		
		if ( HLOK  !=  pNode->SelMark(hWnd,  hStop, nLayer, bIsRevMark) ) 
			return HLERROR;

		if ( HLOK  !=  SelMark(pNode->m_pLeft, nLayer, hWnd, hStop, bIsRevMark)) 
			return HLERROR;

		if ( HLOK  !=  SelMark(pNode->m_pRight, nLayer, hWnd, hStop, bIsRevMark) ) 
			return HLERROR;
	}

	return HLOK;
}

UINT CTree::Mark(CNode *pNode,HWND hWnd)
{
	UINT nRet = HLOK;
	if ( pNode )
	{	
		if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
			return HLERROR;
	
		if ( m_MarkSet.nMarkCount  <=  0 )
			pNode->SetMarkMode(ENU_MARKNORMALL);
		else
			pNode->SetMarkMode(ENU_MARKSPECPARASET);
		
		if ( HLOK  !=  pNode->Mark(hWnd) ) 
			return HLERROR;
		
		if ( HLOK  !=  Mark(pNode->m_pLeft,hWnd) ) 
			return HLERROR;

		if ( HLOK  !=  Mark(pNode->m_pRight,hWnd) ) 
			return HLERROR;
	}

	return nRet;
}

//树结构入口
UINT CTree::Mark( HWND hWnd )
{
	UINT nRet = HLOK;
	g_DAT.SetMarkLayNum(-1, FALSE);

	if ( m_MarkSet.nMarkCount > 0 )
	{
		nRet = MarkSpecParaSet(hWnd);
	}
    else
	{
		nRet = MarkNormal(hWnd);
	}

	return nRet;
}

UINT CTree::MarkSpecParaSet( HWND hWnd )
{
	UINT nRet = HLOK;
	
	if ( m_MarkSet.nMarkCount > OBJ_MAX_MARKCOUNT ) 
		m_MarkSet.nMarkCount = OBJ_MAX_MARKCOUNT;
	else if ( m_MarkSet.nMarkCount < 0 ) 
		m_MarkSet.nMarkCount = 0;

	for( int i = 0; i < m_MarkSet.nMarkCount; i++ ) 
	{
		if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
			return HLERROR;

		SUBOBJMARKSET stSub = m_MarkSet.stSub[i];
		g_DAT.SetMarkLayNum(stSub.nLayer, stSub.bIsRevMark);
		for ( int j = 0; j < stSub.nSubCnt; j++ )
		{
			if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
				return HLERROR;

			if ( stSub.bIsMarkFill && stSub.bIsMarkFram )
			{
				if ( stSub.bIsFillFirst )
				{
					if ( HLERROR == MarkFill(hWnd,stSub.bIsRevMark) )
						return HLERROR;

					if ( HLERROR == Mark(m_pRoot,hWnd) ) 
						return HLERROR;
				}
				else
				{
					if ( HLERROR == Mark(m_pRoot,hWnd) ) 
						return HLERROR;

					if ( HLERROR == MarkFill(hWnd,stSub.bIsRevMark) ) 
						return HLERROR;
				}
			}
			else if ( stSub.bIsMarkFill ) 
			{
				if ( HLERROR == MarkFill(hWnd,stSub.bIsRevMark) ) 
					return HLERROR;
			}
			else if ( stSub.bIsMarkFram )
			{
				if ( HLERROR == Mark(m_pRoot,hWnd) ) 
					return HLERROR;
			}
		}
	}
	
	return nRet;
}

UINT CTree::MarkNormal(HWND hWnd)
{
	if ( m_Fill.bFillFirst == FALSE )//轮廓优先标记
	{
		if ( HLERROR == Mark(m_pRoot,hWnd) ) 
			return HLERROR;

		if ( !m_bPathOptimize  ) //快速排序下填充线不标记
		{
			if ( HLERROR == MarkFillList(hWnd) ) 
				return HLERROR;
		}
	}
	else
	{
		if ( !m_bPathOptimize  )
		{
			if ( HLERROR == MarkFillList(hWnd) ) 
				return HLERROR;
		}
		if ( HLERROR == Mark(m_pRoot,hWnd) ) 
			return HLERROR;
	}
	
	return HLOK;
}


UINT CTree::MarkFillList(HWND hWnd)
{
	StrokeList listScal;
	StrokeList listFill;

	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( pStroke == NULL )
			continue;
		
		if ( 1 == pStroke->GetStyle() )
		{
			listFill.AddTail(pStroke);
		}
		else if ( 2 == pStroke->GetStyle() )
		{
			listScal.AddTail(pStroke);
		}
		
		pStroke = NULL;
	}
	
	if ( FALSE == m_Fill.bFillFirst )
	{
		if ( HLERROR == MarkList(hWnd, listScal) )
			return HLERROR;

		if ( HLERROR == MarkList(hWnd, listFill) )
			return HLERROR;
	}
	else
	{
		if ( HLERROR == MarkList(hWnd, listFill) )
			return HLERROR;

		if ( HLERROR == MarkList(hWnd, listScal) )
			return HLERROR;
	}
	
	return HLOK;
}

UINT CTree::MarkList(HWND hWnd, const StrokeList &list)
{
	BOOL bFirst(TRUE);
	CStroke *pStroke = NULL;
	CDot dotBase(0,0);      //对齐基点
	BOOL bAlignFlag = FALSE;//对齐标志
	if ( list.GetCount() > 0 )
	{
		int nNum  = g_DAT.GetRepeat(m_property.m_nLayer);
		for( int i = 0; i < nNum; i++ )
		{
			POSITION pos =  list.GetHeadPosition();
			while( pos  !=  NULL )
			{
				pStroke = (CStroke *)list.GetNext(pos);
				if ( pStroke == NULL )
					continue;

				if ( pStroke->GetSize() < 1 )
				{
					continue;
				}
				else if ( pStroke->GetSize() == 2 )
				{
					if ( bFirst && m_Fill.bFillAlign )
					{
						bFirst = FALSE;
						CDot dot1, dot2;
						dot1 = pStroke->GetAt(0);
						dot2 = pStroke->GetAt(1);

						dotBase.x = (dot1.x + dot2.x) / 2;
						dotBase.y = (dot1.y + dot2.y) / 2;

						bAlignFlag = TRUE;
					}

					if ( HLOK  !=  g_DAT.MarkFillStroke(pStroke, hWnd, 0, bAlignFlag, dotBase) )
						return HLERROR;						
				}
				else
				{
					if ( HLOK  !=  g_DAT.MarkFillStroke(pStroke, hWnd, 0, FALSE, dotBase) )
						return HLERROR;
				}

				if ( WAIT_OBJECT_0 == ::WaitForSingleObject(hStopWork,0) )
					break;
			}

			if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
				return HLERROR;
		}
	}

	return HLOK;
}



//
UINT CTree::MarkFill(HWND hWnd, BOOL bInverse)
{
	if ( m_bPathOptimize == TRUE )
		return HLOK;
	
	if ( m_listFill.GetCount()  <=  0 )
		return HLOK;
	
	CStroke *pStroke = NULL;
	POSITION pos = NULL;
	if ( bInverse )
		pos = m_listFill.GetTailPosition();
	else
		pos = m_listFill.GetHeadPosition();
	
	while( pos  !=  NULL )
	{
		if ( bInverse )
			pStroke = (CStroke *)m_listFill.GetPrev(pos);
		else
			pStroke = (CStroke *)m_listFill.GetNext(pos);
		
		if ( !pStroke )
			continue;
		
		if ( HLOK  !=  g_DAT.Mark(pStroke,hWnd) )
			return HLERROR;
		
		if ( ::WaitForSingleObject(hStopWork,0) == WAIT_OBJECT_0 )
			return HLERROR;
	}
	
	return HLOK;
}


//
UINT CTree::SelMarkFill(HWND hWnd, int nLayer, HANDLE hStop, BOOL bIsRevMark)
{	
	if ( m_listFill.GetCount()  <=  0 )
		return HLOK;
	
	CStroke *pStroke = NULL;
	POSITION pos = NULL;
	
	if ( bIsRevMark )
		pos = m_listFill.GetTailPosition();
	else
		pos = m_listFill.GetHeadPosition();
	
	while ( pos  !=  NULL )
	{
		if ( bIsRevMark )
			pStroke = (CStroke *)m_listFill.GetPrev(pos);
		else
			pStroke = (CStroke *)m_listFill.GetNext(pos);
		
		UINT nRet = g_DAT.SelMark(pStroke,nLayer, hWnd, hStop, LINE_MARK, bIsRevMark);
		if ( HLERROR == nRet )
			return HLERROR;
		
		if ( ::WaitForSingleObject(hStop,0) == WAIT_OBJECT_0 )
			return HLERROR;
	}
	
	return HLOK;
}



//
UINT CTree::SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub)
{	
	UINT nRet = HLOK;
	for (int j = 0; j < stSub.nSubCnt; j++)
	{
		if ( stSub.bIsMarkFill && stSub.bIsMarkFram )
		{
			if ( stSub.bIsFillFirst )
			{
				nRet = SelMarkFill(hWnd, stSub.nLayer, hStop, stSub.bIsRevMark);
				if ( HLERROR == nRet ) 
					return HLERROR;
				
				nRet = SelMark(m_pRoot,stSub.nLayer, hWnd, hStop, stSub.bIsRevMark);
				if ( HLERROR == nRet ) 
					return HLERROR;
			}
			else
			{
				nRet = SelMark(m_pRoot,stSub.nLayer, hWnd, hStop, stSub.bIsRevMark);
				if ( HLERROR == nRet ) 
				{
					return HLERROR;
				}
				nRet = SelMarkFill(hWnd, stSub.nLayer, hStop, stSub.bIsRevMark);
				if ( HLERROR == nRet ) 
				{
					return HLERROR;
				}
			}
		}
		else if (stSub.bIsMarkFill)
		{
			nRet = SelMarkFill(hWnd, stSub.nLayer, hStop, stSub.bIsRevMark);
			if ( HLERROR == nRet ) 
			{
				return HLERROR;
			}
		}
		else if ( stSub.bIsMarkFram )
		{
			nRet = SelMark(m_pRoot,stSub.nLayer, hWnd, hStop, stSub.bIsRevMark);
			if ( HLERROR == nRet ) 
			{
				return HLERROR;
			}
		}
	}
	
	return nRet;
}


//
void CTree::Rotate(CDot dot, double angle, int nPlane)
{
	if ( GetLockFlg() || fabs(angle) < 0.0001 )
		return;

	Rotate(m_pRoot,dot,angle,nPlane);
	if( m_Fill.newFileData1.bKeepAngle && m_Fill.newFileData2.bKeepAngle )
	{
		RotateFill( dot, angle, nPlane );
		m_dbFillAngle  +=  angle;
	}
	else
	{
		m_dbFillAngle  +=  angle;
		Fill();
	}

	CalLimit();
}


void CTree::RotateFill(CDot dot, double angle, int nPlane)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( !pStroke )
			continue;

		pStroke->Rotate( dot, nPlane, angle );
	}

	pos = m_listDraw.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_listDraw.GetNext(pos);
		if ( !pStroke )
			continue;

		pStroke->Rotate( dot, nPlane, angle );
	}
}

void CTree::Rotate(CNode *pNode,CDot dot, double angle, int nPlane)
{
	if ( pNode )
	{
		pNode->Rotate(dot,angle,nPlane);
		Rotate(pNode->m_pLeft,dot,angle,nPlane);
		Rotate(pNode->m_pRight,dot,angle,nPlane);
	}
}

void CTree::Shear(CDot dot, double dbAngleX, double dbAngleY)
{
	if ( GetLockFlg() )
		return;

	Shear(m_pRoot,dot,dbAngleX,dbAngleY);
	
	Fill();
	CalLimit();
}

void CTree::Shear(CNode *pNode,CDot dot, double dbAngleX, double dbAngleY)
{
	if ( pNode )
	{
		pNode->Shear(dot.x,dot.y,dbAngleX,dbAngleY);
		pNode->CalLimit();
		Shear(pNode->m_pLeft,dot,dbAngleX,dbAngleY);
		Shear(pNode->m_pRight,dot,dbAngleX,dbAngleY);
	}
}

CNode * CTree::CopyFrom(CNode *pNode,DWORD dwStyle)
{
	if ( pNode )
	{
		CNode *pNodeNew = pNode->CreateNewNode();
		if ( pNodeNew == NULL )
			return NULL;

		pNodeNew->SetParent(this);
		pNodeNew->CopyToMe(pNode,dwStyle);
		pNodeNew->m_pLeft  = CopyFrom(pNode->m_pLeft,dwStyle);
		pNodeNew->m_pRight = CopyFrom(pNode->m_pRight,dwStyle);
		return pNodeNew;
	}
	
	return NULL;
}

void CTree::TransFillLine(StrokeList& listFill,BOOL bDraw)
{
	//设填充线绘制参数
	BOOL  bFirst = TRUE;
	POSITION pos = listFill.GetHeadPosition();
	while( pos  !=  NULL )
	{
		CStrokeB *pStrokeB = listFill.GetNext(pos);
		if ( pStrokeB  !=  NULL )
		{
			CStroke* pStroke = new CStroke(m_property);
			if ( pStroke )
			{
				(*pStroke) = (*pStrokeB);
				pStroke->SetStyle(pStrokeB->GetStyle());
				if ( !bDraw )
					m_listFill.AddTail(pStroke);
				else
					m_listDraw.AddTail(pStroke);
				//
				delete pStrokeB;
				pStrokeB = NULL;
			}
			else
			{
				delete pStrokeB;
				continue;
			}
			///////
			if ( pStroke->GetSize() == 2 && TRUE == bFirst )//第一根填充线
			{
				pStroke->m_bFillFirst = TRUE;
				bFirst = FALSE;
			}
		}
	}
	listFill.RemoveAll();
}


//
void CTree::Fill()
{
	if ( m_pRoot->GetType() == NODE_BMP )
		return;

	if ( m_pRoot->GetType() == NODE_CODEBAR && ((CNodeCodebar*)m_pRoot)->m_bAutoFill )
		return;

	DeleteFill();
	BOOL bFirst = TRUE;

	if ( m_Fill.newFileData1.bEnable || m_Fill.newFileData2.bEnable )
	{
		StrokeList listData;
		Scan(m_pRoot, &listData);

		//
		StrokeList listFill;
		if ( m_Fill.newFileData1.dbSpace > FILLMINDIS && m_Fill.newFileData1.bEnable )
		{
			CFill f1(m_Fill.newFileData1, m_property);
			f1.Proc( &listData,listFill, m_dbFillAngle,m_Fill.bFillFirst );
			//设填充线绘制参数
			TransFillLine(listFill,FALSE);
			
			if ( ::g_sys.m_bFastDrawEnable )
			{
				if ( m_Fill.newFileData1.dbSpace < 0.05 )
					f1.m_newFillData.dbSpace = 0.05;

				f1.Proc( &listData,listFill, m_dbFillAngle,m_Fill.bFillFirst );
				TransFillLine(listFill,TRUE);
			}
		}
		//
		if ( m_Fill.newFileData2.dbSpace > FILLMINDIS && m_Fill.newFileData2.bEnable )
		{
			CFill f2(m_Fill.newFileData2, m_property);
			f2.Proc( &listData, listFill, m_dbFillAngle, m_Fill.bFillFirst );
			//设填充线绘制参数
			TransFillLine(listFill,FALSE);

			if ( ::g_sys.m_bFastDrawEnable )
			{
				if ( m_Fill.newFileData2.dbSpace < 0.05 )
					f2.m_newFillData.dbSpace = 0.05;

				f2.Proc( &listData,listFill, m_dbFillAngle,m_Fill.bFillFirst );
				TransFillLine(listFill,TRUE);
			}
		}
	}
	SetFillProperty();
}


//
void CTree::Scan(CNode *pNode,StrokeList* listTemp)
{	
	if (pNode)
	{
		pNode->Scan(listTemp); 
		Scan(pNode->m_pLeft,listTemp); 
		Scan(pNode->m_pRight,listTemp);
	}
}


//
void CTree::GetFillStyle(FILLDATA& fill)
{
	fill = m_Fill;
}

//
void CTree::SetFillStyle(FILLDATA& fill)
{
	m_Fill = fill;

	Fill();
	m_quadLimit = CalLimit();
}


//
void CTree::DeleteFill()
{
	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( pStroke )
		{
			delete pStroke;
		    pStroke = NULL;
		}
	}
	m_listFill.RemoveAll();

	pos = m_listDraw.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)m_listDraw.GetNext(pos);
		if ( pStroke )
		{
			delete pStroke;
			pStroke = NULL;
		}
	}
	m_listDraw.RemoveAll();
}

//
void CTree::Drag(CDot dot, double ratioX, double ratioY, double moveX, double moveY, double moveZ)
{
	// 如果被锁定，就不移动
	if ( GetLockFlg() )
		return;
	
	if ( fabs(ratioX-1) < EPSILON && fabs(ratioY-1) < EPSILON )
	{
		Move(moveX,moveY,moveZ);
	}
	else
	{
		Drag( m_pRoot,dot,ratioX,ratioY,moveX,moveY,moveZ );

		Fill();
		CalLimit();
	}
}

//
void  CTree::Drag(CNode *pNode,CDot dot, double ratioX, double ratioY, double moveX, double moveY,double moveZ)
{
	if ( pNode )
	{
		pNode->Drag(dot,ratioX,ratioY,moveX,moveY,moveZ);

		Drag(pNode->m_pLeft,dot,ratioX,ratioY,moveX,moveY,moveZ);
		Drag(pNode->m_pRight,dot,ratioX,ratioY,moveX,moveY,moveZ);
	}
}

//
void CTree::SetProperty(int mode,CProperty pty)
{
	switch(mode)
	{
	case PTY_OUTLINE:
		if ( IsSelect() )
		{
			SetProperty(m_pRoot,mode,pty);
		}
		break;
	case PTY_FILL:
		if ( IsSelect() )
		{
			m_property.Set(mode,pty);
			SetFillProperty();
		}
		break;
	case PTY_FILL_COUNTOUR:
		if ( IsSelect() )
		{
			m_propertyScal.Set(mode,pty);
			SetFillProperty();
		}
		break;
	default:
		SetProperty(m_pRoot,mode,pty);
		m_property.Set(mode,pty);
		m_propertyScal.Set(mode,pty);
		SetFillProperty();
		break;
	}
}


//
void CTree::SetProperty(CNode *pNode,int mode,CProperty pty)
{
	if ( pNode )
	{
		pNode->SetProperty(mode,pty);
		SetProperty(pNode->m_pLeft,mode,pty);
		SetProperty(pNode->m_pRight,mode,pty);
	}
}

//
void CTree::SetGroup(BOOL bGroup)
{
	SetGroup(m_pRoot, bGroup);
}

//
void CTree::SetGroup(CNode *pNode,BOOL bGroup)
{
	if ( pNode )
	{
		SetGroup(pNode->m_pLeft,bGroup);
		SetGroup(pNode->m_pRight,bGroup);
	}
}


//
BOOL CTree::PreprocessMark(CNode *pNode,DWORD nStyle)
{
	BOOL bFlag = FALSE;
	if ( pNode )
	{
		if ( pNode->PreprocessMark(nStyle) )
			bFlag = TRUE;
		if ( PreprocessMark(pNode->m_pLeft,nStyle) )
			bFlag = TRUE;
		if ( PreprocessMark(pNode->m_pRight,nStyle) )
			bFlag = TRUE;
	}

	return bFlag;
}


//
BOOL CTree::PreprocessMark(DWORD nStyle)
{	
	m_nMarkUnit = m_listFill.GetCount();
	BOOL bUpdate = PreprocessMark(m_pRoot,nStyle);

	if ( bUpdate )
		Fill();

	return TRUE;
}

//
int CTree::GetType()
{
	if ( m_pRoot )
		return m_pRoot->GetType();
	else
		return  NODE_BASE;

}

//
void CTree::SetSelect(CNode *pNode,BOOL bFlag)
{
	if ( pNode )
	{
		pNode->SetSelect(bFlag);
		//////
		SetSelect(pNode->m_pLeft,bFlag);
		SetSelect(pNode->m_pRight,bFlag);
	}
}

//
void CTree::SetSelect(BOOL bFlag)
{
	SetSelect(m_pRoot,bFlag);
	//////
	m_bSelect = bFlag;
}

//
BOOL CTree::IsSelect()
{
	if ( IsGroup() )	
		return m_bSelect;
	
	return IsSelect(m_pRoot);
}

//
BOOL CTree::IsSelect(CNode *pNode)
{
	if ( pNode )
	{
		return pNode->IsSelect();
	}
	return FALSE;
}

//
void CTree::InvertSelect(CNode *pNode)
{
	if ( pNode )
	{
		pNode->InvertSelect();
		InvertSelect(pNode->m_pLeft);
		InvertSelect(pNode->m_pRight);
	}
}

//
void CTree::InvertSelect()
{
	InvertSelect(m_pRoot);
	m_bSelect = !m_bSelect;
}

//
BOOL CTree::Include(CNode *pNode,CQuad quad)
{
	if ( pNode )
	{
		return  pNode->Include(quad);
		//////
		 Include(pNode->m_pLeft,quad);
		 Include(pNode->m_pRight,quad);
	}
	return FALSE;
}


//
BOOL CTree::Include(CQuad quad)
{		
	BOOL bInclude = FALSE;
 	bInclude = Include(m_pRoot,quad);
	if ( bInclude && IsGroup() )
	{	
		SetSelect(m_pRoot,TRUE);
	}
	return bInclude;
}


//
void CTree::SetLayColor(CNode *pNode,int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	if ( pNode )
	{
		pNode->SetLayColor(nLayer, color, nLayerFill, colorFill);
		SetLayColor(pNode->m_pLeft,nLayer, color, nLayerFill, colorFill);
		SetLayColor(pNode->m_pRight,nLayer, color, nLayerFill, colorFill);
	}
}


//
void CTree::GetLayColor(CNode *pNode,int &nLayer,int &nFillLayer)
{
	if ( pNode )
	{
		pNode->GetLayColor(nLayer, nFillLayer);
		GetLayColor(pNode->m_pLeft,nLayer, nFillLayer);
		GetLayColor(pNode->m_pRight,nLayer, nFillLayer);
	}
}

//
void CTree::GetLayColor(int &nLayer,int &nFillLayer, int &FillScalLayer)
{
	nFillLayer = m_property.m_nLayer;
	FillScalLayer = m_propertyScal.m_nLayer;
	
	GetLayColor(m_pRoot,nLayer,nFillLayer);
}


//
void CTree::SetLayColor(int nLayer, int nFillLayer, int FillScalLayer)
{	
	CLay* pLay = g_DAT.GetCurLay();
	
	CProperty layProperty;
	CProperty FillProperty;
	CProperty FillScalProperty;
	if ( nLayer > -1 )
	{
		layProperty  = pLay->GetLayer(nLayer).GetProperty();
		if ( layProperty.m_nLayer == -1 )
		{
			return;
		}
		SetLayColor(m_pRoot,nLayer,layProperty.m_color,nFillLayer,FillProperty.m_color);
	} 
	
	//
	if (nFillLayer > -1) 
	{
		FillProperty = pLay->GetLayer(nFillLayer).GetProperty();
		if ( FillProperty.m_nLayer == -1 )
		{
			return;
		}
		m_property.m_color  = FillProperty.m_color;
		m_property.m_nLayer = nFillLayer;		
		SetFillProperty();
	}

	if ( FillScalLayer > -1 ) 
	{
		FillScalProperty = pLay->GetLayer(FillScalLayer).GetProperty();
		if ( FillScalProperty.m_nLayer == -1 )
		{
			return;
		}
		m_propertyScal.m_color  = FillScalProperty.m_color;
		m_propertyScal.m_nLayer = FillScalLayer;
		SetFillProperty();
	}
}



//
void CTree::SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor)
{
	SetLayColor(m_pRoot, nObjIndex, nObjColor, nFillIndex, nFillColor);

	m_property.m_color  = nFillColor;
	m_property.m_nLayer = nFillIndex;
	m_propertyScal = m_property;

	SetFillProperty();
}

//
BOOL CTree::CheckBarCode(CNode*pNode,CUIntArray &arrayBarID,CUIntArray &arrayValidID)
{
	if ( !pNode )
		return FALSE;

	if ( pNode->GetType() == NODE_CODEBAR || pNode->GetType() == NODE_HLTEXT )
	{
		return ((CNodeT_B *) pNode)->CheckBarCode(arrayBarID,arrayValidID);
	}
	
	CheckBarCode(pNode->m_pLeft,arrayBarID,arrayValidID);
	CheckBarCode(pNode->m_pRight,arrayBarID,arrayValidID);

	return TRUE;
}

//
CQuad CTree::GetReFurbishQuad()
{
	if ( m_pRoot )
		return m_pRoot->GetLimit(TRUE);
	else
		return CQuad(0,0,0,0);
}

//
void CTree::GetStrokeList(StrokeList *pStrokeList)
{
	GetStrokeList(m_pRoot,pStrokeList);
}

void CTree::GetStrokeList(CNode *pNode, StrokeList *pStrokeList)
{
	if ( pNode )
	{
		pNode->GetStrokeList(pStrokeList);
		GetStrokeList(pNode->m_pLeft,pStrokeList);
		GetStrokeList(pNode->m_pRight,pStrokeList);
	}
}

CQuad CTree::GetMarkRange()//zg add 2005/08/26 for v4.3
{
	CQuad quad;
	quad.SetEmpty();

	if ( m_pRoot->m_pLeft == NULL&& m_pRoot->m_pRight == NULL )
	{
		quad = m_pRoot->GetLimit(TRUE);
	}
	else	//对于组合对象，应该遍历组合对象中的所有对象，获取其打标范围，
			 //以防止该组合对象中有圆弧排布的文字对象
	{
		if ( m_pRoot->m_pLeft  !=  NULL )
           GetMarkRange(m_pRoot->m_pLeft,quad);
		if ( m_pRoot->m_pRight != NULL )
	       GetMarkRange(m_pRoot->m_pRight,quad);
	}

	return quad;
}

void CTree::GetMarkRange(CNode *pNode, CQuad &quad)//zg add 2005/08/26 for v4.3
{
     ASSERT(pNode != NULL);
	////
	 quad.Union(pNode->GetLimit());

	 if ( pNode->m_pLeft  !=  NULL )
		 GetMarkRange(pNode->m_pLeft,quad);
	 if ( pNode->m_pRight  !=  NULL )
		 GetMarkRange(pNode->m_pRight,quad);
}


BOOL CTree::SelectBarCode(CUIntArray& arrayTextID)
{
	if ( !m_pRoot  )
		return FALSE;

	if ( m_pRoot->GetType() == NODE_HLTEXT || m_pRoot->GetType() == NODE_CODEBAR )
	{	
		CText *pText = NULL;
		CNodeT* pNode = (CNodeT*)m_pRoot;
		if ( pNode->IsBarText() )
		{
			for( UINT i = 0; i < pNode->GetTextCount(); i++ )
			{
				pText = (CText*)pNode->GetText(i);
				if ( pText == NULL )
					continue;

				for( int j = 0; j < arrayTextID.GetSize(); j++ ) 
				{
					if ( pText->GetTextID() == arrayTextID.GetAt(j) )
						return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}


//
BOOL CTree::GetFirstMarkCurrent(double & dbCurrent)
{
	CLay *pLay = g_DAT.GetCurLay();
	if ( pLay == NULL )
		return FALSE;

	PARA para = pLay->GetPara(m_property.m_nLayer);
	dbCurrent = para.emcc.dbCurrent;

	return TRUE;
}


BOOL CTree::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	ExportPlt(m_pRoot,file,bUnExportFill, bScall100);
	if ( !bUnExportFill )
	{
		//显示填充线
		CStroke *pStroke = NULL;
		POSITION pos = m_listFill.GetHeadPosition();
		while(pos != NULL)
		{	
			pStroke = (CStroke *)m_listFill.GetNext(pos);
			if ( pStroke )
				pStroke->ExportPlt(file,bUnExportFill, bScall100);
		}
	}	
	return TRUE;
}

void CTree::ExportPlt(CNode *pNode,CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if ( pNode )
	{
		pNode->ExportPlt(file, bUnExportFill, bScall100);
		/////
		ExportPlt(pNode->m_pLeft,file, bUnExportFill, bScall100);
		ExportPlt(pNode->m_pRight,file, bUnExportFill, bScall100);
	}	
}


BOOL CTree::PtInTree(CPoint &point,CWnd *pWnd)
{
	CZoomView *pView = (CZoomView*)pWnd;
	if ( pWnd == NULL )
		return FALSE;

	double x = 5;
	double y = 5;
	pView->ActualLen(x);
	pView->ActualLen(y);

	CQuad quad = m_quadLimit;
	quad.Inflate(x,y);

	return  quad.Include(point,pWnd);
}


BOOL CTree::Click(CPoint &point,CWnd *pWnd)//点选
{
	return Click(m_pRoot,point,pWnd);
}



BOOL CTree::Click(CNode *pNode,CPoint &point,CWnd *pWnd)
{
	return m_quadLimit.Include(point, pWnd);
}


BOOL CTree::PtInTree(CDot &dot)
{
	CQuad quad = CalLimit();
	quad.Inflate(2,2);
	//////////
	return ( quad.left  <=  dot.x && dot.x  <=  quad.right && quad.bottom  <=  dot.y && dot.y  <=  quad.top );
}



void CTree::AddRect(CWnd *pWnd)
{
	AddRect(m_pRoot,pWnd);
	//////
	POSITION pos = m_listFill.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( pStroke )
			pStroke->AddRect(pWnd);
	}
}

void CTree::AddRect(CNode *pNode,CWnd *pWnd)
{
	if ( pNode )
	{
		pNode->AddRect(pWnd);
		AddRect(pNode->m_pLeft,pWnd);
		AddRect(pNode->m_pRight,pWnd);
	}
}

void CTree::DelRect(CWnd *pWnd)
{
	DelRect( m_pRoot,pWnd );
}


void CTree::DelRect(CNode *pNode, CWnd *pWnd)
{
	if ( pNode )
	{
		pNode->DelRect(pWnd);
		DelRect(pNode->m_pLeft,pWnd);
		DelRect(pNode->m_pRight,pWnd);
	}	
}



BOOL CTree::IsEmpty()
{
	return IsEmpty(m_pRoot);
}



BOOL CTree::IsEmpty(CNode *pNode)
{
	if ( pNode )
	{
		if ( !pNode->IsEmpty() )
			return FALSE;
		if ( !IsEmpty(pNode->m_pLeft) )
			return FALSE;
		if ( !IsEmpty(pNode->m_pRight) )
			return FALSE;
	}
	return TRUE;
}

void CTree::SetStrokeList(StrokeList *pStrokeList,BOOL bFlag)
{
	// 如果层被锁定，则直接返回
	if ( GetLockFlg() ) 
		return;
	
	SetStrokeList(m_pRoot,pStrokeList,bFlag);
}

void CTree::SetStrokeList(CNode *pNode, StrokeList *pStrokeList,BOOL bFlag)
{	
	if ( pStrokeList == NULL )
		return;

	if ( pNode )
	{	
		if ( !bFlag )
		{
			if ( pNode->GetType() == NODE_CODEBAR || pNode->GetType() == NODE_HLTEXT )
				return;
		}
		pNode->SetStrokeList(pStrokeList);
		SetStrokeList(pNode->m_pLeft,pStrokeList, bFlag);
		SetStrokeList(pNode->m_pRight,pStrokeList, bFlag);
	}
}

//
void CTree::CaptureObject(CQuad &quad,StrokeList *pStrokeList)
{
	if ( m_pRoot  !=  NULL )
	{
		if  ( (  m_pRoot->GetType()  == NODE_DELAY) 
			||( m_pRoot->GetType()  == NODE_INPUT) 
			|| (m_pRoot->GetType()  == NODE_OUTPUT) 
			|| (m_pRoot->GetType()  == NODE_MOTORMOVE)
			|| (m_pRoot->GetType()  == NODE_MOTORPOS) 
			|| (m_pRoot->GetType()  == NODE_CONDITIONS)
			|| (m_pRoot->GetType()  == NODE_CONDITIONE) 
			|| (m_pRoot->GetType()  == NODE_BUFFERLASER) 
			|| (m_pRoot->GetType()  == NODE_CCD) 
			|| (m_pRoot->GetType()  == NODE_POSCALI) 
			)
			return;
	}

	CQuad quadTest = CalLimit();
	if ( quad.Include(quadTest) )
	{
		SetStrokeList(pStrokeList);
		SetSelect(TRUE);
	}
}

void CTree::AddFillToStrokeList(StrokeList *pStrokeList)
{
	if ( !pStrokeList )
		return;

	POSITION pos = m_listFill.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos)
	{	
		pStroke = (CStroke*)m_listFill.GetNext(pos);
		if ( pStroke )
			pStrokeList->AddTail(pStroke);
	}
}

UINT CTree::CanConvert()
{	
	if ( HaveNode(m_pRoot, NODE_CODEBAR) )
		return 1;
	else if ( HaveNode(m_pRoot, NODE_HLTEXT) )
		return 2;
	else if ( HaveNode(m_pRoot, NODE_ELLIPSE) )
		return 3;
	else if ( HaveNode(m_pRoot, NODE_SPLINE) )
		return 4;
	else if ( HaveNode(m_pRoot, NODE_ARC) )
		return 5;
	else if ( HaveNode(m_pRoot, NODE_CIRCLE) )
		return 6;
	else if ( m_listFill.GetCount() > 0 )
		return 3;

	return 0;
}


//
CTree* CTree::ConvertPLT()
{
	CNodePlt *pNode = new CNodePlt();
	if ( NULL == pNode )
		return NULL;

	AddToPlt(m_pRoot,pNode);
	CTree*  pTree = new CTree(pNode);
	if ( pTree == NULL )
	{
		delete pNode;
		pNode = NULL;
	}

	return pTree;
}

//
CTree* CTree::ConvertFillToPLT()
{
	if ( m_listFill.GetCount()  <=  0 )
		return NULL;

	CNodePlt *	pNode = new CNodePlt();
	if ( NULL == pNode )
		return NULL;

	CStroke *pStrokeNew = NULL;
	CStroke *pStroke = (CStroke*)m_listFill.GetHead();
	pNode->SetProperty(PTY_INIT, pStroke->m_property);

	POSITION pos = m_listFill.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		pStrokeNew = (CStroke*)pStroke->Copy();
		pNode->GetList().AddTail(pStrokeNew);
	}

	CTree *pTree = new CTree(pNode);
	if ( pTree == NULL )
	{
		delete pNode;
		pNode = NULL;
	}

	return pTree;
}

CTree*  CTree::ExtendObj(double dbSpace, BOOL bOpt)
{
	if ( m_pRoot == NULL )
		return NULL;

	if ( m_pRoot->GetType() == NODE_CODEBAR )
		return NULL;
	
	CNodePlt *pNode = new CNodePlt();
	if ( NULL == pNode ) 
		return NULL;
	
	//
	StrokeList listData, listContour, listTemp;	
	Scan(m_pRoot,&listData);	

	//
	CFillMaker fillMake;
	fillMake.ScaleProc(listData, listContour, listTemp, dbSpace, bOpt, FALSE);

	//
	POSITION pos  = listContour.GetHeadPosition();
	CStrokeB *pStrokeB = NULL;
	CStroke   *pStroke = NULL;
	while( pos )
	{
		pStrokeB = listContour.GetNext(pos);
		if ( pStrokeB  !=  NULL )
		{
			pStroke = new CStroke(pStrokeB->m_property);
			if ( pStroke )
			{
				(*pStroke) = (*pStrokeB);
				pNode->GetList().AddTail(pStroke);
			}

			delete pStrokeB;
			pStrokeB = NULL;
		}
	}
	listContour.RemoveAll();

	CTree *pTree = new CTree(pNode);
	if ( NULL == pTree )
	{
		delete pNode;
		return NULL;
	}

	pTree->SetSelect(TRUE);
	pTree->SetProperty(PTY_OUTLINE,m_pRoot->m_property);
	pTree->SetSelect(FALSE);
	pTree->CalLimit();

	return pTree;
}


//
void CTree::AddToPlt(CNode *pNode, CNode *pNodeplt)
{
	if ( pNode )
	{
		pNode->AddToPlt(pNodeplt);
		//////
		AddToPlt(pNode->m_pLeft,pNodeplt);
		AddToPlt(pNode->m_pRight,pNodeplt);
	}
}

//
BOOL CTree::AddFillToPlt(CNode *pNodeplt)
{
	if ( m_listFill.GetCount()  <=  0 )
		return TRUE;

	CStroke *pStroke    = NULL;
	CStroke *pStrokeNew = NULL;
	POSITION pos =  m_listFill.GetHeadPosition();
	while( pos )
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( !pStroke )
			continue;

		pStrokeNew = (CStroke*)pStroke->Copy();
		if ( pStrokeNew )
			pNodeplt->GetList().AddTail(pStrokeNew);
	}

	return TRUE;
}

void CTree::FilterPoint(double dbPrecis, double dbAngle)
{
	FilterPoint(m_pRoot, dbPrecis, dbAngle);
}

void CTree::FilterPoint(CNode *pNode, double dbPrecis, double dbAngle)
{
	if ( pNode)
	{
        pNode->FilterPoint(dbPrecis, dbAngle);
		////
		FilterPoint(pNode->m_pLeft,dbPrecis, dbAngle);
		FilterPoint(pNode->m_pRight,dbPrecis, dbAngle);
	}
}



BOOL CTree::GetFirstDot(CDot &dt)
{
	return GetTreeFirstdot(m_pRoot,dt);
}



BOOL CTree::GetTreeFirstdot(CNode *pNode, CDot &dt)
{
	if ( pNode )
	{
		if ( pNode->GetNodeFirstDot(dt) )
			return TRUE;

		if ( GetTreeFirstdot(pNode->m_pLeft, dt) )
			return TRUE;
		
		if ( GetTreeFirstdot(pNode->m_pRight, dt) )
			return TRUE;
	}
	return FALSE;
}

BOOL CTree::GetLastDot(CDot &dt)
{
	return GetNodeLastDot(m_pRoot,dt);
}

BOOL CTree::GetNodeLastDot(CNode *pNode, CDot &dt)
{
	if ( pNode )
	{
		pNode->GetNodeLastDot(dt);
		///
		GetNodeLastDot(pNode->m_pLeft, dt);
		GetNodeLastDot(pNode->m_pRight, dt);
	}

	return TRUE;
}



void CTree::SetSortStatus(BOOL bIsSort)
{
	m_bIsCurSortTree = bIsSort;
	SetSortStatus(m_pRoot, bIsSort);
}


void CTree::SetSortStatus(CNode *pNode, BOOL bIsSort)
{
	if ( pNode )
	{
        pNode->m_bIsCurSortTree = bIsSort;
		SetSortStatus(pNode->m_pLeft,bIsSort);
		SetSortStatus(pNode->m_pRight,bIsSort);
	}
}

void CTree::SetGroupNum(int nNum)
{
	m_nGroupNum = nNum;
}

int CTree::GetGroupNum()
{
	return m_nGroupNum;
}

CStroke* CTree::GetGoLineStroke()
{
	return GetGoLineStroke(m_pRoot);
}

CStroke* CTree::GetGoLineStroke(CNode *pNode)
{
	if ( pNode )
	{
		CStroke *pStroke = (CStroke *)pNode->GetGoLineStroke();
		if ( pStroke )
			return pStroke;

		pStroke = GetGoLineStroke(pNode->m_pLeft);
		if ( pStroke )
			return pStroke;

		pStroke =  GetGoLineStroke(pNode->m_pRight);
		if ( pStroke )
			return pStroke;
	}
	
	return NULL;
}


BOOL CTree::ComToOneNode(CNode *pNode)
{
	if ( NULL == pNode )
		return FALSE;
	
	AddToPlt(m_pRoot, pNode);
	AddFillToPlt(pNode);
	
	return TRUE;
}


BOOL CTree::GetLockFlg()
{
	CHLDoc *pDoc = GetCurActiveDocument();
	if ( NULL == pDoc )
		return FALSE;
	
	if ( pDoc->GetLockFlg(m_property.m_nLayer) )
		return TRUE;
	
	return GetLockFlg(m_pRoot);
}



BOOL CTree::GetLockFlg(CNode* pNode)
{
	CHLDoc* pDoc = GetCurActiveDocument();
	if ( pDoc == NULL )
		return FALSE;
	
	if ( pNode  !=  NULL )
	{
		if ( pDoc->GetLockFlg(pNode->m_property.m_nLayer) )
			return TRUE;
		
		if (GetLockFlg(pNode->m_pLeft))
			return TRUE;
		
		if (GetLockFlg(pNode->m_pRight))
			return TRUE;
	}
	
	return FALSE;
}



double CTree::GetLength(CNode* pNode)
{
	double total = 0;
	if (pNode  !=  NULL)
	{
		total  +=  pNode->GetLength();
		total  +=  GetLength(pNode->m_pLeft);
		total  +=  GetLength(pNode->m_pRight);
	}
	return total;
}


double CTree::GetLength()
{
	double total = 0;
	total  +=  GetLength(m_pRoot);
	
	POSITION pos =  m_listFill.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( pStroke )
			total  +=  pStroke->GetLength();
	}
	
	return total;
}



BOOL CTree::IsHaveTxtOrCodeBar()
{
	return IsHaveTxtOrCodeBar(m_pRoot);
}



BOOL CTree::IsHaveTxtOrCodeBar(CNode *pNode)
{
	if ( pNode )
	{
		if (   ( NODE_HLTEXT  == pNode->GetType() ) 
			|| (NODE_CODEBAR == pNode->GetType() )
			|| (NODE_ELLIPSE == pNode->GetType() )
			|| (NODE_SPLINE  == pNode->GetType() )
			|| (NODE_ARC	 == pNode->GetType() )
			|| (NODE_CIRCLE  == pNode->GetType() ))
		{
			return TRUE;
		}
		IsHaveTxtOrCodeBar(pNode->m_pLeft);
		IsHaveTxtOrCodeBar(pNode->m_pRight);
	}

	return FALSE;
}

BOOL CTree::IsBmpNode()
{
	return IsBmpNode(m_pRoot);
}

BOOL CTree::IsBmpNode(CNode *pNode)
{
	if ( pNode )
	{
		if ( NODE_BMP == pNode->GetType() )
			return TRUE;

		IsBmpNode(pNode->m_pLeft);
		IsBmpNode(pNode->m_pRight);
	}

	return FALSE;
}


void CTree::DrawExtern(XDC *pDC)
{
	DrawExtern(m_pRoot,pDC);
}

void CTree::DrawExtern(CNode *pNode, XDC *pDC)
{
	if ( pNode )
	{
		pNode->DrawExtern(pDC);
		DrawExtern(pNode->m_pLeft,pDC);
		DrawExtern(pNode->m_pRight,pDC);
	}	
}

CQuad& CTree::RetLimit()
{
	return m_quadLimit;
}



void CTree::JoinStroke(double dbMinDistance)
{
	JoinStroke(m_pRoot,dbMinDistance);
}



void CTree::JoinStroke(CNode *pNode, double dbMinDistance)
{
	if ( pNode == NULL )
		return;
	
	pNode->JoinStroke(dbMinDistance);
	JoinStroke(pNode->m_pLeft,dbMinDistance);
	JoinStroke(pNode->m_pRight,dbMinDistance);
}


//
BOOL CTree::SetLayerSelect(int nItem, CNode *pNode,BOOL bFlag)
{
	if ( pNode == NULL )
		return FALSE;
	
	if ( pNode->LayerSelect(nItem) == TRUE )
		return TRUE;
	
	if ( SetLayerSelect(nItem, pNode->m_pLeft, bFlag) == TRUE )
		return TRUE;
	
	if ( SetLayerSelect(nItem, pNode->m_pRight, bFlag) == TRUE )
		return TRUE;
	
	return FALSE;
}



//
BOOL CTree::SetLayerSelect(int nItem, BOOL bFlag)
{
	if ( ( m_property.m_nLayer == nItem ) && ( m_listFill.GetCount() > 0 ) )
		return TRUE;
	
	return SetLayerSelect(nItem,m_pRoot,bFlag);
}

// add by zhq
void CTree::LoadAssistantPoints(CArray<DOT,DOT> &pointArray)
{
	LoadAssistantPoints(m_pRoot, pointArray);
}

void CTree::LoadAssistantPoints(CNode *pNode, CArray<DOT,DOT> &pointArray)
{
	if ( pNode )
	{			
		pNode->LoadAssistantPoints(pointArray);

		LoadAssistantPoints(pNode->m_pLeft, pointArray);
		LoadAssistantPoints(pNode->m_pRight, pointArray);
	}
}

//
void CTree::LoadStrokeList(StrokeList *pStrokeList,BOOL bSelect,int nLayNo)
{
	LoadStrokeList(m_pRoot,pStrokeList,bSelect,nLayNo);
}


void CTree::LoadStrokeList(CNode *pNode, StrokeList *pStrokeList,BOOL bSelect,int nLayNo)
{
	if ( pNode )
	{			
		pNode->LoadStrokeList(pStrokeList,bSelect,nLayNo);
		
		LoadStrokeList(pNode->m_pLeft,pStrokeList,bSelect,nLayNo);
		LoadStrokeList(pNode->m_pRight,pStrokeList,bSelect,nLayNo);
	}
}

//
void CTree::LoadFillStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo)
{
	if ( (bOptimize == FALSE) && (m_bPathOptimize == TRUE) )
		return;
	
	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while( pos )
	{	
		pStroke = (CStroke *)m_listFill.GetNext(pos);

		if (pStroke  !=  NULL)
		{
			int nLayer = pStroke->m_property.m_nLayer;
			if ( -1 == nLayNo || nLayer == nLayNo )
			{
				pStroke->m_bSelect = m_bSelect;
				pStrokeList->AddTail(pStroke);

				m_bPathOptimize = bOptimize;
			}
		}
	}
}



void CTree::LoadFillLine(StrokeList *pStrokeList )
{
	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while( pos )
	{	
		pStroke = (CStroke *)m_listFill.GetNext(pos);
		if ( pStroke  !=  NULL )
		{
			if ( pStroke->GetStyle() == 1 )
			{
				pStroke->m_bSelect = m_bSelect;
				pStrokeList->AddTail(pStroke);
			}
		}
	}
}


void CTree::LoadFillContour(StrokeList *pStrokeList )
{
	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while( pos )
	{	
		pStroke = (CStroke *)m_listFill.GetNext(pos);

		if ( pStroke  !=  NULL )
		{
			if ( pStroke->GetStyle() == 2 )
			{
				pStroke->m_bSelect = m_bSelect;
				pStrokeList->AddTail(pStroke);
			}
		}
	}
}

//nType: 0对象切分，1轮廓切分， 2综合轮廓， 3综合填充
void CTree::LoadWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType)
{
	if ( nType == 2 && (m_Fill.newFileData1.bEnable || m_Fill.newFileData2.bEnable) )
		return;

	if ( nType == 3 || m_pRoot == NULL)
		return;
	
	StrokeList *pListStroke = new StrokeList();
	if ( !pListStroke )
		return;

	LoadWheelList(m_pRoot, nType, pListStroke);
	if ( pListStroke->GetCount() > 0 ) 
	{
		arrayList.Add(pListStroke);
	}
	else
	{
		delete pListStroke;
	}
}


//
void CTree::LoadWheelList(CNode *pNode,int nType, StrokeList *pListStroke)
{
	if ( pNode )
	{	
		pNode->LoadWheelList(nType, pListStroke);
		
		LoadWheelList(pNode->m_pLeft,nType, pListStroke);
		LoadWheelList(pNode->m_pRight,nType, pListStroke);
	}
}


//nType: 0对象切分，1轮廓切分， 2综合轮廓， 3综合填充
void CTree::LoadFillWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType)
{
	if ( nType == 1 || nType == 2 )
		return;
	else if ( nType == 3 )
	{
		BOOL bFill1 = FALSE, bFill2 = FALSE;
		//
		if (m_Fill.newFileData1.bEnable)
		{
			if (fabs(m_Fill.newFileData1.dbAngle - 90) < EPSILON && fabs(m_Fill.newFileData1.ProAngle) < EPSILON && fabs(m_Fill.newFileData1.AddDis) < EPSILON && (m_Fill.newFileData1.nFillStyle == 0 || m_Fill.newFileData1.nFillStyle == 2))
			{
				bFill1 = TRUE;
			}
			else
				bFill1 = FALSE;
		}
		else
		{
			bFill1 = TRUE;
		}

		//
		if (m_Fill.newFileData2.bEnable)
		{
			if (fabs(m_Fill.newFileData2.dbAngle - 90) < EPSILON && fabs(m_Fill.newFileData2.ProAngle) < EPSILON && fabs(m_Fill.newFileData2.AddDis) < EPSILON && (m_Fill.newFileData2.nFillStyle == 0 || m_Fill.newFileData2.nFillStyle == 2))
			{
				bFill2 = TRUE;
			}
			else
				bFill2 = FALSE;
		}
		else
		{
			bFill2 = TRUE;
		}
		
		if ( !bFill1 || !bFill2 )
		{
			return;
		}
	}
	
	//
	StrokeList *pListStroke = NULL;
	if ( m_listFill.GetCount() > 0 )
	{
		if ( nType ==  0 )
			pListStroke = arrayList.GetAt(arrayList.GetSize() - 1);
		
		if ( pListStroke == NULL )
		{
			pListStroke = new StrokeList();
			if ( pListStroke == NULL )
			{
				OutputDebugString(_T("CTree::LoadFillWheelList() alloc memory error!\n"));
				return;
			}
		}
		
		CStrokeB *pStroke = NULL;
		POSITION pos = m_listFill.GetHeadPosition();
		while( pos != NULL )
		{	
			pStroke = m_listFill.GetNext(pos);
			if ( pStroke == NULL )
				continue;
			//////
			CStrokeB *pNew = new CStrokeB(m_property);
			if ( NULL == pNew )
				continue;
			
			pNew->Append(*pStroke);			
			pListStroke->AddTail(pNew);
		}
		
		if ( nType  !=  0)
			arrayList.Add(pListStroke);
	}
}


//
void CTree::LoadOldFill()
{	
	BOOL bBack = FALSE, bCross = FALSE, bEven = FALSE; 
	BOOL bFirstFill = FALSE, bPtp = FALSE;
	BOOL bCircle = FALSE, bBetter = FALSE;
	
	bBack      = ((m_nStyle_Old & FILL_BACK) > 0  )	? 1 : 0;      //往复填充
	bCross     = ((m_nStyle_Old & FILL_CROSS) > 0 )	? 1 : 0;     //交叉填充
	bEven      = ((m_nStyle_Old & FILL_EVEN) > 0  )	? 1 : 0;      //线间距自修正
	bFirstFill = ((m_nStyle_Old & FILL_MORD) > 0  )	? 1 : 0;      //先打填充
	bPtp       = ((m_nStyle_Old & FILL_PTP) > 0   )	? 1 : 0;       //光栅
    bCircle    = ((m_nStyle_Old & FILL_CIRCLE) > 0)	? 1 : 0;    //连接
    bBetter    = ((m_nStyle_Old & FILL_BETTER) > 0)	? 1 : 0;    //优化
	
	//
	if ( m_dbSpace1_Old > 0 )
	{
		m_Fill.newFileData1.bEnable = TRUE;
		m_Fill.newFileData1.bEquaDist = bEven;
		m_Fill.newFileData1.bKeepAngle = FALSE;
		
		if ( bBack == FALSE && bCircle == FALSE )
			m_Fill.newFileData1.nFillStyle = 0;
		else if (bBack == FALSE && bCircle == TRUE && bBetter == TRUE)
			m_Fill.newFileData1.nFillStyle = 1;
		else if (bBack == TRUE && bCircle == FALSE)
			m_Fill.newFileData1.nFillStyle = 2;
		else if (bBack == TRUE && bCircle == TRUE && bBetter == TRUE)
			m_Fill.newFileData1.nFillStyle = 3;

		m_Fill.newFileData1.dbSpace = m_dbSpace1_Old;
		m_Fill.newFileData1.dbAngle = m_dbAngle1_Old;
		m_Fill.newFileData1.dbBeamComp = 0;
		m_Fill.newFileData1.dbLineReduce = m_dbDistance_Old;
		m_Fill.newFileData1.dbLineReduceEnd = m_dbDistance_Old;
		m_Fill.newFileData1.dbStartOffset = m_dbHeadKillLen_Old;
		m_Fill.newFileData1.dbEndOffset = m_dbTailKillLen_Old;
		m_Fill.newFileData1.dbMinJump = m_dbCircleLen_Old;
		
		m_Fill.newFileData1.bLoop = FALSE;
		m_Fill.newFileData1.nLoopNum = 0;
		m_Fill.newFileData1.dbLoopDist = 0;
		m_Fill.newFileData1.bRemoveLine = FALSE;
		m_Fill.newFileData1.bRemoveOrg = FALSE;	
	}

	if ( bCross == TRUE )
	{	
		if ( m_dbSpace2_Old > 0 )
			m_Fill.newFileData2.bEnable = TRUE;
		
		m_Fill.newFileData2.bEquaDist = bEven;
		m_Fill.newFileData2.bKeepAngle = FALSE;
		
		if (bBack == FALSE && bCircle == FALSE)
			m_Fill.newFileData2.nFillStyle = 0;
		else if (bBack == FALSE && bCircle == TRUE && bBetter == TRUE)
			m_Fill.newFileData2.nFillStyle = 1;
		else if (bBack == TRUE && bCircle == FALSE)
			m_Fill.newFileData2.nFillStyle = 2;
		else if (bBack == TRUE && bCircle == TRUE && bBetter == TRUE)
			m_Fill.newFileData2.nFillStyle = 3;

		m_Fill.newFileData2.dbSpace = m_dbSpace2_Old;
		m_Fill.newFileData2.dbAngle = m_dbAngle2_Old;
		m_Fill.newFileData2.dbBeamComp = 0;
		m_Fill.newFileData2.dbLineReduce = m_dbDistance_Old;
		m_Fill.newFileData1.dbLineReduceEnd = m_dbDistance_Old;
		m_Fill.newFileData2.dbStartOffset = m_dbHeadKillLen_Old;
		m_Fill.newFileData2.dbEndOffset = m_dbTailKillLen_Old;
		m_Fill.newFileData2.dbMinJump = m_dbCircleLen_Old;
		
		m_Fill.newFileData2.bLoop = FALSE;
		m_Fill.newFileData2.nLoopNum = 0;
		m_Fill.newFileData2.dbLoopDist = 0;
		m_Fill.newFileData2.bRemoveLine = FALSE;
		m_Fill.newFileData2.bRemoveOrg = FALSE;	
	}
}

//
void CTree::Serialize_FillPara(FILE* fp,BOOL bFlag,BOOL bUnicode)
{	
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if ( bFlag == TRUE ) //save
	{	
		fs  <<  "T_NEWFILLB";
		
		fs  <<  100;
		fs  <<  m_Fill.newFileData1.bEnable;
		fs  <<  101;
		fs  <<  m_Fill.newFileData1.bEquaDist;
		fs  <<  102;
		fs  <<  m_Fill.newFileData1.bKeepAngle;
		fs  <<  103;
		fs  <<  m_Fill.newFileData1.nFillStyle;
		fs  <<  104;
		fs  <<  m_Fill.newFileData1.dbSpace;
		fs  <<  105;
		fs  <<  m_Fill.newFileData1.dbAngle;
		fs  <<  106;
		fs  <<  m_Fill.newFileData1.dbBeamComp;
		fs  <<  107;
		fs  <<  m_Fill.newFileData1.dbLineReduce;
		fs  <<  108;
		fs  <<  m_Fill.newFileData1.dbStartOffset;
		fs  <<  109;
		fs  <<  m_Fill.newFileData1.dbEndOffset;
		fs  <<  110;
		fs  <<  m_Fill.newFileData1.dbMinJump;
		fs  <<  111;
		fs  <<  m_Fill.newFileData1.bLoop;
		fs  <<  112;
		fs  <<  m_Fill.newFileData1.nLoopNum;
		fs  <<  113;
		fs  <<  m_Fill.newFileData1.dbLoopDist;
		fs  <<  114;
		fs  <<  m_Fill.newFileData1.bRemoveLine;
		fs  <<  115;
		fs  <<  m_Fill.newFileData1.bRemoveOrg;
		fs  <<  116;
		fs  <<  m_Fill.newFileData1.nFillNum;
		fs  <<  117;
		fs  <<  m_Fill.newFileData1.AddDis;
		fs  <<  118;
		fs  <<  m_Fill.newFileData1.ProAngle;
		fs  <<  119;
		fs  <<  m_Fill.newFileData1.bSaveCountour;
		fs  <<  120;
		fs  <<  (m_Fill.newFileData1.dbLineReduceEnd - m_Fill.newFileData1.dbLineReduce);	// 填充线尾点缩进量，存与首点的增量
		
		fs  <<  200;
		fs  <<  m_Fill.newFileData2.bEnable;
		fs  <<  201;
		fs  <<  m_Fill.newFileData2.bEquaDist;
		fs  <<  202;
		fs  <<  m_Fill.newFileData2.bKeepAngle;
		fs  <<  203;
		fs  <<  m_Fill.newFileData2.nFillStyle;
		fs  <<  204;
		fs  <<  m_Fill.newFileData2.dbSpace;
		fs  <<  205;
		fs  <<  m_Fill.newFileData2.dbAngle;
		fs  <<  206;
		fs  <<  m_Fill.newFileData2.dbBeamComp;
		fs  <<  207;
		fs  <<  m_Fill.newFileData2.dbLineReduce;
		fs  <<  208;
		fs  <<  m_Fill.newFileData2.dbStartOffset;
		fs  <<  209;
		fs  <<  m_Fill.newFileData2.dbEndOffset;
		fs  <<  210;
		fs  <<  m_Fill.newFileData2.dbMinJump;
		fs  <<  211;
		fs  <<  m_Fill.newFileData2.bLoop;
		fs  <<  212;
		fs  <<  m_Fill.newFileData2.nLoopNum;
		fs  <<  213;
		fs  <<  m_Fill.newFileData2.dbLoopDist;
		fs  <<  214;
		fs  <<  m_Fill.newFileData2.bRemoveLine;
		fs  <<  215;
		fs  <<  m_Fill.newFileData2.bRemoveOrg;
		fs  <<  216;
		fs  <<  m_Fill.newFileData2.nFillNum;
		fs  <<  217;
		fs  <<  m_Fill.newFileData2.AddDis;
		fs  <<  218;
		fs  <<  m_Fill.newFileData2.ProAngle;
		fs  <<  219;
		fs  <<  m_Fill.newFileData2.bSaveCountour;
		fs  <<  220;
		fs  << (m_Fill.newFileData2.dbLineReduceEnd - m_Fill.newFileData2.dbLineReduce);
		
		fs  <<  300;
		fs  <<  m_Fill.bFillFirst;
		fs  <<  301;
		fs  <<  m_Fill.bFillAlign;
		
		fs  <<  "T_NEWFILLE";
	}
	else //load
	{
		if ( g_dwVersion == 52001 )
		{
			LoadOldFill();
		}
		else
		{
			while( fs >> str )
			{
				str.TrimRight();
				if ( str == "T_NEWFILLE" )
					break;

				switch( _ttoi(str) )
				{
				case 100:
					{
						fs >> m_Fill.newFileData1.bEnable;
						break;
					}
				case 101:
					{
						fs >> m_Fill.newFileData1.bEquaDist;
						break;
					}
				case 102: 
					{
						fs >> m_Fill.newFileData1.bKeepAngle;
						break;
					}
				case 103: 
					{
						fs >> m_Fill.newFileData1.nFillStyle;
						break;
					}
				case 104:
					{
						fs >> m_Fill.newFileData1.dbSpace;
						break;
					}
				case 105:
					{
						fs >> m_Fill.newFileData1.dbAngle;
						break;
					}
				case 106:
					{
						fs >> m_Fill.newFileData1.dbBeamComp;
						break;
					}
				case 107:
					{
						fs >> m_Fill.newFileData1.dbLineReduce;
						break;
					}
				case 108:
					{
						fs >> m_Fill.newFileData1.dbStartOffset;
						break;
					}
				case 109:
					{
						fs >> m_Fill.newFileData1.dbEndOffset;
						break;
					}
				case 110:
					{
						fs >> m_Fill.newFileData1.dbMinJump;
						break;
					}
				case 111:
					{
						fs >> m_Fill.newFileData1.bLoop;
						break;
					}
				case 112:
					{
						fs >> m_Fill.newFileData1.nLoopNum;
						break;
					}
				case 113:
					{
						fs >> m_Fill.newFileData1.dbLoopDist;
						break;
					}
				case 114:
					{
						fs >> m_Fill.newFileData1.bRemoveLine;
						break;
					}
				case 115: 
					{
						fs >> m_Fill.newFileData1.bRemoveOrg;
						break;
					}
				case 116: 
					{
						fs >> m_Fill.newFileData1.nFillNum;
						break;
					}
				case 117: 
					{
						fs >> m_Fill.newFileData1.AddDis;
						break;
					}
				case 118: 
					{
						fs >> m_Fill.newFileData1.ProAngle;
						break;
					}
				case 119:
					{
						fs >> m_Fill.newFileData1.bSaveCountour;
						break;
					}
				case 120:
					{
						fs >> m_Fill.newFileData1.dbLineReduceEnd;
						break;
					}
				case 200: 
					{
						fs >> m_Fill.newFileData2.bEnable;
						break;
					}
				case 201:
					{
						fs >> m_Fill.newFileData2.bEquaDist;
						break;
					}
				case 202:
					{
						fs >> m_Fill.newFileData2.bKeepAngle;
						break;
					}
				case 203: 
					{
						fs >> m_Fill.newFileData2.nFillStyle;
						break;
					}
				case 204:
					{
						fs >> m_Fill.newFileData2.dbSpace;
						break;
					}
				case 205:
					{
						fs >> m_Fill.newFileData2.dbAngle;
						break;
					}
				case 206:
					{
						fs >> m_Fill.newFileData2.dbBeamComp;
						break;
					}
				case 207:
					{
						fs >> m_Fill.newFileData2.dbLineReduce;
						break;
					}
				case 208:
					{
						fs >> m_Fill.newFileData2.dbStartOffset;
						break;
					}
				case 209:
					{
						fs >> m_Fill.newFileData2.dbEndOffset;
						break;
					}
				case 210:
					{
						fs >> m_Fill.newFileData2.dbMinJump;
						break;
					}
				case 211:
					{
						fs >> m_Fill.newFileData2.bLoop;
						break;
					}
				case 212:
					{
						fs >> m_Fill.newFileData2.nLoopNum;
						break;
					}
				case 213:
					{
						fs >> m_Fill.newFileData2.dbLoopDist;
						break;
					}
				case 214:
					{
						fs >> m_Fill.newFileData2.bRemoveLine;
						break;
					}
				case 215:
					{
						fs >> m_Fill.newFileData2.bRemoveOrg;
						break;
					}
				case 216:
					{
						fs >> m_Fill.newFileData2.nFillNum;
						break;
					}
				case 217:
					{
						fs >> m_Fill.newFileData2.AddDis;
						break;
					}
				case 218:
					{
						fs >> m_Fill.newFileData2.ProAngle;
						break;
					}
				case 219: 
					{
						fs >> m_Fill.newFileData2.bSaveCountour;
						break;
					}
				case 220:
					{
						fs >> m_Fill.newFileData2.dbLineReduceEnd;
						break;
					}
				case 300:
					{
						fs >> m_Fill.bFillFirst;
						break;
					}
				case 301:
					{
						fs >> m_Fill.bFillAlign;
						break;
					}
				default:
					break;
				}
			}

			m_Fill.newFileData1.dbLineReduceEnd += m_Fill.newFileData1.dbLineReduce;
			m_Fill.newFileData2.dbLineReduceEnd += m_Fill.newFileData2.dbLineReduce;
		}
	}
}

//
void CTree::RefreshBarText(CNode* pNode,CWnd* pWnd, int nTextID)
{
	if ( pNode )
	{
		CNodeT *pNodeT = TransNode(pNode);
		if ( pNodeT )
			pNodeT->RefreshBarText(pWnd,nTextID);

		RefreshBarText(pNode->m_pLeft,pWnd,nTextID);
		RefreshBarText(pNode->m_pRight,pWnd,nTextID);
	}
}

void CTree::SetPathOptimizeState(BOOL bState)
{	
	m_bPathOptimize = bState;
	SetPathOptimizeState(m_pRoot,bState);
}

void CTree::SetPathOptimizeState(CNode *pNode,BOOL bState)
{
	if ( pNode )
	{
		pNode->SetPathOptimizeState(bState);
		SetPathOptimizeState(pNode->m_pLeft,bState);
		SetPathOptimizeState(pNode->m_pRight,bState);
	}	
}

BOOL CTree::HaveFunctionNode()
{
	return HaveFunctionNode(m_pRoot);
}

//
BOOL CTree::HaveFunctionNode(CNode *pNode)
{
	if ( pNode )
	{
		int nType = pNode->GetType();
		if (	   ( nType == NODE_DELAY) 
			||( nType == NODE_INPUT) 
			|| (nType == NODE_OUTPUT) 
			|| (nType == NODE_MOTORMOVE)
			|| (nType == NODE_MOTORPOS) 
			|| (nType == NODE_CONDITIONS)
			|| (nType == NODE_CONDITIONE) 
			|| (nType == NODE_BUFFERLASER) 
			|| (nType == NODE_CCD) 
			|| (nType == NODE_POSCALI) 
			)
			return TRUE;

		if ( HaveFunctionNode(pNode->m_pLeft) ) 
			return TRUE;		
		if ( HaveFunctionNode(pNode->m_pRight) )
			return TRUE;
	}

	return FALSE;
}

BOOL CTree::HaveNode(int nType)
{
	return HaveNode(m_pRoot, nType);
}

//
BOOL CTree::HaveNode(CNode *pNode, int nType)
{
	if ( pNode )
	{	
		if ( pNode->GetType() == nType)
			return TRUE;

		if ( HaveNode(pNode->m_pLeft, nType) )
			return TRUE;

		if ( HaveNode(pNode->m_pRight, nType) )
			return TRUE;
	}

	return FALSE;
}

void CTree::ChangeText(CNode *pNode, CString strTxt)
{
	SetTextData(pNode,strTxt);
	PreprocessMark(0x00);
	Fill();
}


void  CTree::SetTextData(CNode *pNode, CString& strTxt)
{
	if ( pNode )
	{	
		CNodeT *pNodeT = (CNodeT*)TransNode(pNode); 
		if ( pNodeT )
			pNodeT->SetTextData(strTxt);

		SetTextData(pNode->m_pLeft, strTxt);
		SetTextData(pNode->m_pRight, strTxt);
	}
}

void  CTree::GetTextValue(CNode *pNode, CString& strTxt)
{
	if ( pNode )
	{	
		CNodeT *pNodeT = TransNode(pNode);
		if ( pNodeT  !=  NULL )
		{	
			CString strTmp;
			BOOL bOK = pNodeT->GetTextValue(strTmp);
			if ( bOK && !strTmp.IsEmpty() )
				strTxt  +=  strTmp;
		}
		GetTextValue(pNode->m_pLeft, strTxt);
		GetTextValue(pNode->m_pRight, strTxt);
	}
}

void CTree::InitQuadRange(CNode *pNode)
{
	if ( pNode )
	{
		pNode->InitQuadRange();
		InitQuadRange(pNode->m_pLeft);
		InitQuadRange(pNode->m_pRight);
	}	
}

void CTree::SetFillProperty()
{
	CStroke *pStroke = NULL;
	POSITION pos = m_listFill.GetHeadPosition();
	while( pos != NULL )
	{	
		pStroke = (CStroke *)m_listFill.GetNext(pos);

		if (1 == pStroke->GetStyle())
		{
			pStroke->SetProperty(PTY_FILL,m_property);
		}
		else if ( 2 == pStroke->GetStyle())
		{
			pStroke->SetProperty(PTY_FILL,m_propertyScal);
		}
	}
	pos = m_listDraw.GetHeadPosition();
	while( pos != NULL )
	{	
		pStroke = (CStroke *)m_listDraw.GetNext(pos);

		if (1 == pStroke->GetStyle())
		{
			pStroke->SetProperty(PTY_FILL,m_property);
		}
		else if ( 2 == pStroke->GetStyle())
		{
			pStroke->SetProperty(PTY_FILL,m_propertyScal);
		}
	}
}


BOOL CTree::GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo)
{
	CNodeT *pNodeT = (CNodeT*)TransNode(m_pRoot); 
	if ( pNodeT  !=  NULL )
	{
		return  pNodeT->GetTextExtInfo(lpTextExtInfo);
	}
	else
	{
		return FALSE;
	}
}

CNodeT* CTree::TransNode(CNode *pNode)
{
	if ( pNode == NULL )
		return NULL;

	int nType = pNode->GetType();
	if ( nType  !=  NODE_HLTEXT && nType  !=  NODE_CODEBAR )
		return NULL;

	return  (CNodeT*)pNode; 
}

BOOL CTree::SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo)
{
	CNodeT *pNodeT = (CNodeT*)TransNode(m_pRoot); 
	if ( pNodeT  !=  NULL )
	{
		return pNodeT->SetTextExtInfo(lpTextExtInfo);
	}
	else
	{
		return FALSE;
	}
}


void CTree::ResetTextID( int nMinID, int nOpType )
{
	ResetTextID( m_pRoot, nMinID, nOpType );
}

void CTree::ResetTextID( CNode *pNode, int nMinID,int nOpType )
{
	if ( pNode )
	{
		CNodeT *pNodeT = TransNode(pNode);
		if ( pNodeT )
			pNodeT->ResetTextID(nMinID,nOpType,g_arryIDMap);
		////////
		ResetTextID(pNode->m_pLeft, nMinID,nOpType);
		ResetTextID(pNode->m_pRight, nMinID,nOpType);
	}	
}


BOOL CTree::IsEnable3DEdit()
{
	if ( IsHaveTxtOrCodeBar() || m_listFill.GetCount() > 0 )
		return FALSE;

	return TRUE;
}

CMgrText*  CTree::GetCurMgrText()
{
	try
	{
		CDataBase *pChain =  GetTopNode();
		//////
		if ( pChain == NULL || !pChain->IsKindOf(RUNTIME_CLASS(CChain))  )
		{
			OutputDebugString(_T("CTree::GetCurMgrText() can't find parent!\n"));
			return NULL;
		}
		return  &(((CChain*)pChain)->m_MgrText);
	}
	catch(CException *e)
	{
		e->Delete();

		return NULL;
	}
}
