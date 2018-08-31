#include "stdafx.h"
#include "hl.h"
#include "NodeLine.h"
#include "Tree.h"
#include "HLDoc.h"
#include "OptStrokeList.h"
#include "HLView.h"
#include "NodeDst.h"
#include "Resource.h"

CNodeDst::CNodeDst(void)
{
	m_type = NODE_DST;
	m_quadRangle = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
}

CNodeDst::~CNodeDst(void)
{
}

CTree *CNodeDst::UnGroup()
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


void CNodeDst::LoadWheelList(int nType, StrokeList *pListStroke)
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


BOOL CNodeDst::CanUnGroup()
{
	if ( m_list.GetCount() > 0 )
		return TRUE;
	else
		return FALSE;
}


//
BOOL CNodeDst::SetLayerSelect(int nItem, BOOL bFlag)
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


//按图层分解导入的线条
CNodeLine *CNodeDst::UnGroupDST()
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


BOOL CNodeDst::DeCode(CString file,BOOL bCenter,BOOL bBetter)
{
	m_pFile = new CFile();
	if( !m_pFile->Open(file,CFile::modeRead) )
	{
		AfxMessageBox(IDS_ERROR_OPENFILE);
		return FALSE;
	}
	DWORD nLen = m_pFile->GetLength();
	if (nLen<515)
	{
		AfxMessageBox(_T("File size error!"));
		return FALSE;
	}
	char pbuf[30] ={0};
    UINT nBytesRead = m_pFile->Read( pbuf, 10);
	if(pbuf[0] ==_T('L')&&pbuf[1]==_T('A')&&pbuf[2]==_T(':'))
	{
	}		
	else
	{
		AfxMessageBox(_T("'LA' command does not exist!"));
		return FALSE;
	}
	m_pFile->Seek( 512, CFile::begin );
	m_dotCur.x = CENTER;
	m_dotCur.y = CENTER;
	CStroke *pStroke =new CStroke(m_property);
	m_pStroke=pStroke;
	int nAddx = 0,nAddy = 0;
	m_list.AddTail(m_pStroke);
	BOOL bNewStroke = TRUE;
	while((m_pFile->GetPosition()+3)<=nLen)
    {
		byte pbum[3]={0};
		UINT nBytesRead1 = m_pFile->Read(pbum, 3);
		int u,v,w;
		u = pbum[0];
		v = pbum[1];
		w = pbum[2];	
		int X,Y;
		X=((u&1)?1:0)+((u&2)?(-1):0)+((u&4)?9:0)+((u&8)?(-9):0)+\
			((v&1)?3:0)+((v&2)?(-3):0)+((v&4)?27:0)+((v&8)?(-27):0)+\
			((w&4)?81:0)+((w&8)?(-81):0);
		Y=((u&16)?(-9):0)+((u&32)?9:0)+((u&64)?(-1):0)+((u&128)?1:0)+\
			((v&16)?(-27):0)+((v&32)?27:0)+((v&64)?(-3):0)+((v&128)?3:0)+\
			((w&16)?(-81):0)+((w&32)?81:0);
	    
		nAddx += X;
		nAddy += Y;	
		
		m_dotCur.x = (double)nAddx/10;
		m_dotCur.y = (double)nAddy/10;
		BOOL bState1,bState2,bState3,bState4;
		bState1 = ((pbum[2] & 0x03)==0x03)?TRUE:FALSE;
		bState2 = ((pbum[2] & 0x83)==0x83)?TRUE:FALSE;
		bState3 = ((pbum[2] & 0xC3)==0xC3)?TRUE:FALSE;
		bState4 = ((pbum[2] & 0xF3)==0xF3)?TRUE:FALSE;	
		if(bState4)
		{
			if(u==0&&v==0){
				//m_list.SetItemText(nPos,7,"End");
				break;
			}
			else
			{
				AfxMessageBox(_T("No end command!"));
				return FALSE;
			}
		}
		else if(bState3)
		{
			//m_list.SetItemText(nPos,7,"Ncolor");
			m_property.m_nLayer++;
		} else if(bState2)
		{
			//m_list.SetItemText(nPos,7,"Jump");
		    bNewStroke = TRUE;
		}else if(bState1)
		{
			if(bNewStroke){
				CStroke *pStroke =new CStroke(m_property);
				m_pStroke=pStroke;
				m_list.AddTail(m_pStroke);
				m_pStroke->Add(m_dotCur);
				bNewStroke = FALSE;
			}
			//m_list.SetItemText(nPos,7,"Move");
			m_pStroke->Add(m_dotCur);
		}
		else
		{
			//m_list.SetItemText(nPos,7,"error command");
		}
		//nPos++;
	}
	m_pFile->Close();
	delete m_pFile;

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


void CNodeDst::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs << _T("NDST_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		Serialize_list(fp,bFlag,bUnicode);
		//写入尾信息
		fs << _T("NDST_E");
	}
	else
	{
		while(fs >> str)
		{
			if ( str == _T("QUAD_RANG_B") )
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("PRO_B") )
				m_property.Serialize(fp,bFlag,bUnicode);
			else if ( str == _T("SL_B") )
				Serialize_list(fp,bFlag,bUnicode);
			else if ( str == _T("NDST_E") )
				break;
		}
	}
}