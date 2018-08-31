// NodePlt.cpp: implementation of the CNodePlt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodePlt.h"
#include "NodeLine.h"
#include "Tree.h"
#include "HLDoc.h"
#include "OptStrokeList.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define  PLT_READSIZE      20971520	//20M
#define  PLT_SECSIZE		1000
#define  PLT_CALFACTOR   40.0


CNodePlt::CNodePlt()
{
	m_type = NODE_PLT;
	m_quadRangle = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
	m_pStroke = NULL;
	m_bFloat = FALSE;
	
	//
	m_nBufSaveSize = PLT_SECSIZE;
	m_pBufSave = new char[m_nBufSaveSize];
	memset( m_pBufSave, 0, m_nBufSaveSize*sizeof(char) );
}

CNodePlt::~CNodePlt()
{
	if ( m_pBufSave )
	{
		delete []m_pBufSave;
		m_pBufSave = NULL;
	}
}


CTree *CNodePlt::UnGroup()
{
	CStroke *pStroke = (CStroke *)m_list.GetHead();
	if ( pStroke == NULL )
		return NULL;
	
	CNodeLine *pNode = new CNodeLine(pStroke);
	if ( pNode == NULL )
		return NULL;

	CTree *pTree = new CTree(pNode);
	if ( pTree == NULL )
	{
		delete pNode;
		return NULL;
	}

	m_list.RemoveHead();

	return pTree;
	
}


//按图层分解导入的线条，每组相同图层的线条作为一个PLT对象。
CNodeLine *CNodePlt::UnGroupplt()
{
	POSITION prev = NULL;
	POSITION pos  = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	int nLay = -1;
	BOOL bFirst = TRUE;
	CNodeLine *pNode = NULL;
	
	while( pos  !=  NULL )
	{
		prev = pos;
		pStroke = (CStroke *)m_list.GetNext(pos);

		if (TRUE == bFirst )
		{
			if ( pStroke != NULL )
			{
				pNode = new CNodeLine();
				if ( pNode == NULL )
					return NULL;

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
			if ( pStroke->m_property.m_nLayer == nLay )
			{
				pNode->m_list.AddTail(pStroke);
				m_list.RemoveAt(prev);
			}
		}
	}

	return pNode;
}



void CNodePlt::AddStroke()
{
	if ( m_pStroke != NULL )
	{
		if ( m_pStroke->GetSize() > 1 )
			m_list.AddTail(m_pStroke);
		else
		{
			delete m_pStroke;
			m_pStroke = NULL;
		}
		m_pStroke = NULL;
	}
}


void CNodePlt::CreatePen(int pen)
{
	CMDIFrameWnd *pFrame  = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	if ( !pFrame  )
		return;

	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	if ( !pChild  )
		return;

	CHLDoc *pDoc = (CHLDoc *) pChild->GetActiveDocument();
	if ( pDoc == NULL )
		return;
	
	CLayer ly   = pDoc->GetLayer(pen);
	m_property = ly.GetProperty();
}


void CNodePlt::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if (bFlag)
	{
		//写入头信息
		fs << _T("NPLT_B");
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag,bUnicode);
		Serialize_list(fp,bFlag,bUnicode);
		//写入尾信息
		fs << _T("NPLT_E");
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
			else if ( str == _T("NPLT_E") )
				break;
		}
	}
}



void CNodePlt::CreateCircle(CDot dotCenter, double dbRadius)
{
	if ( dbRadius <= 0.001 )
		return;

	//根据弦长来计算圆的信息
	double dbAngle = 0.05/dbRadius; 
	double dbCurAngle;

	int  nCount = (int)(2*PAI/dbAngle+0.5);
	dbAngle  = 2*PAI/nCount;

	CStroke *pStroke = new CStroke(m_property);
	if ( pStroke == NULL )
		return;

	CDot dot;	
	for( int i = 0; i < nCount; i++ )
	{
		dbCurAngle = i*dbAngle;
		dot.x = dbRadius*cos(dbCurAngle);
		dot.y = dbRadius*sin(dbCurAngle);
		pStroke->Add(dot);
	}
	dot.x = dbRadius;
	dot.y = 0;
	pStroke->Add(dot);
	
	pStroke->Move(dotCenter.x,dotCenter.y);
	m_list.AddTail(pStroke);
}


void CNodePlt::LoadWheelList(int nType, StrokeList *pListStroke)
{
	if ( m_list.GetCount() <= 0 )
		return;

	POSITION  pos = m_list.GetHeadPosition();
	CStrokeB *pStroke = NULL;
	while( pos  !=  NULL )
	{
		pStroke = m_list.GetNext(pos);
		if ( pStroke == NULL )
			continue;

		CStrokeB *pNew = new CStrokeB(pStroke->m_property);
		if ( NULL == pNew )
			continue;

		pNew->Append(*pStroke);
		pListStroke->AddTail(pNew);
	}
}


BOOL CNodePlt::CanUnGroup()
{
	if ( m_list.GetCount() > 0 )
		return TRUE;
	else
		return FALSE;
}



//
BOOL CNodePlt::SetLayerSelect(int nItem, BOOL bFlag)
{
	if ( m_list.GetCount() > 0 )
	{
		CStrokeB *pStroke = NULL;
		POSITION pos  = m_list.GetHeadPosition();
		while( pos != NULL )
		{
			pStroke = m_list.GetNext(pos);			
			if ( pStroke->m_property.m_nLayer == nItem )
				return TRUE;
			else
				return FALSE;
		}
	}
	
	return FALSE;
}

//////////////////////////////////////////
BOOL CNodePlt::DeCode(CString file,BOOL bCenter,BOOL bBetter)  
{
	if (  file.IsEmpty() || !m_file.Open(file,CFile::modeRead) )
	{
		AfxMessageBox(IDS_ERROR_OPENFILE);
		return FALSE;
	}
	
	//
	char cmd[100] = { 0 };
	m_dotCur.x = CENTER;
	m_dotCur.y = CENTER;
	m_pStroke  = NULL;

	m_bPen = FALSE;
	m_bCircle = FALSE;
	m_bPenUp = TRUE;
	m_bCoorA = TRUE;
	m_pStroke = NULL;

	//------------------------------------------------------
	ULONGLONG FileSize = 0;
	ULONGLONG leftSize = 0;
	BYTE *pFileBuffer = NULL;

	FileSize = m_file.GetLength();
	int nCount = (int)(FileSize/PLT_READSIZE);
	leftSize = FileSize - PLT_READSIZE*nCount;
	UINT  nAlloc = (UINT)(FileSize+1);
	if ( nCount > 0 )//超级大文件
		nAlloc = PLT_READSIZE;

	pFileBuffer = new BYTE[nAlloc];
	if ( pFileBuffer == NULL )
	{
		OutputDebugString(_T("CNodePlt::Decode() alloc memory error!\n"));
		return FALSE;
	}

	m_nSaveIndex = 0;
	memset( m_pBufSave, 0, m_nBufSaveSize*sizeof(char) );
	UINT  nReaded = 0; 
	for( int i = 0; i < nCount; i++ )
	{
		memset( pFileBuffer, 0, nAlloc );
		nReaded = m_file.Read( pFileBuffer, nAlloc );
		DecodeBuf( pFileBuffer, nReaded );
	}

	if ( leftSize > 0 )
	{
		memset( pFileBuffer, 0, nAlloc );
		nReaded = (UINT)(m_file.Read( pFileBuffer,(UINT)leftSize ));
		DecodeBuf( pFileBuffer, nReaded );
	}

	delete []pFileBuffer;

	//------------------------------------------------------
	AddStroke();
	m_file.Close();
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



void CNodePlt::DecodeBuf( BYTE *pBuff, UINT nSize )
{
	if ( !pBuff || nSize  <=  0 )
		return;
	
	UINT nIndex = 0;
	CString strPack;
	CString strCmd,strValue;
	while( HandleBuffSeg( pBuff, nIndex, nSize, strPack ) )
	{
		CString strData = strPack;
		ProCommand(strData);
		ProCoord(strData);
	}
}



void CNodePlt::ProCommand( CString &strData )
{
	CString strCmd = _T("");
	BOOL bCommand = TRUE;
	while( TRUE )
	{
		strCmd = strData.Left(2);
		if ( strCmd == _T("PD") )
		{
			if ( m_bPenUp  )
			{
				AddStroke();
				m_pStroke = new CStroke(m_property);
				if ( m_pStroke )
					m_pStroke->Add(m_dotCur);
			}
			m_bPenUp = FALSE;
		}
		else if ( strCmd == _T("PU") )//结束一个STROKE
		{
			m_bPenUp = TRUE;
		}
		else if ( strCmd == _T("PA") )//转换为绝对坐标
		{
			m_bCoorA = TRUE;
		}
		else if ( strCmd == _T("PR") )//转换为相对坐标
		{
			m_bCoorA = FALSE;
		}
		else if ( strCmd == _T("SP") )//换笔
		{
			m_bPen = TRUE;
		}
		else if ( strCmd == _T("CI") )
		{
			m_bCircle = TRUE;
		}
		else
		{
			bCommand = FALSE;
		}

		//
		if ( !bCommand )
		{
			break;
		}
		else
		{
			strData = strData.Mid(2);
		}
	}
}



void CNodePlt::ProCoord( CString &strData )
{
	double dbValue = 0;
	int nCount = -1;
	
	while(TRUE)
	{
		if ( ReadValue(strData, dbValue)  !=  0 )
		{
			break;
		}
		nCount++;

		//
		if ( m_bPen )
		{
			m_bPen = FALSE;
			int nPen = (int)(dbValue);
			CreatePen( nPen );

			//
			if ( FALSE == m_bPenUp )
			{
				AddStroke();
				m_pStroke = new CStroke(m_property);
				if ( m_pStroke )
					m_pStroke->Add(m_dotCur);
			}
		}
		
		//
		if ( m_bCircle )
		{
			m_bCircle = FALSE;
			CreateCircle(m_dotCur,dbValue);
		}
		
		//
		if ( nCount%2 == 0 )
		{
			if ( m_bCoorA )
			{
				m_dotCur.x = dbValue;
			}
			else
			{
				m_dotCur.x  +=  dbValue;
			}
		}
		else
		{
			if (m_bCoorA)
			{
				m_dotCur.y = dbValue;
			}
			else
			{
				m_dotCur.y  +=  dbValue;
			}
			
			//根据实际情况加入到STROKE
			if ( !m_bPenUp && m_pStroke )
			{
				m_pStroke->Add(m_dotCur);
			}
		}
	}
}



//0：坐标
//1: 读到";"或为空
//2: 读到字母
int CNodePlt::ReadValue( CString &strData, double &dbValue )
{
	if ( strData.IsEmpty() )
		return 1;
	
	double dbRead = 0;
	TCHAR chBuf[20] = {0};
	TCHAR chBufY[20] = {0};
	TCHAR chValue;
	CString strValue = _T("");
	
	for( int i = 0; i<strData.GetLength(); i++ )
	{
		chValue = strData.GetAt(i);
		
		if ( chValue == ',' || chValue == ' ' || chValue == ';')
		{
			if ( strData.GetLength() == 1 )
			{
				return 1;
			}
			
			dbRead = _tstof(strValue);
			strValue = _T("");
			
			strData = strData.Mid(i+1);
			break;
		}
		
		//
		if ( (chValue >= '0'&& chValue <= '9')|| chValue == '.'||chValue == '+'||chValue == '-' )
		{
			strValue  +=  chValue;
		}
		else
		{
			return 2;
		}
	}
	
	if ( m_bPen || m_bFloat )
	{
		dbValue = dbRead;
	}
	else
	{
		dbValue = dbRead/PLT_CALFACTOR;
	}
	
	return 0;
}









//从大缓冲区中提取小段数据
BOOL CNodePlt::HandleBuffSeg( BYTE *pBuff, UINT &nIndex, UINT nSize, CString &strPack )
{
	if ( !pBuff || nIndex  >=  nSize )
		return FALSE;
	
	strPack.Empty();
	while( nIndex < nSize  )
	{
		if ( pBuff[nIndex] == '\0' )
			return FALSE;
		
		*(m_pBufSave + m_nSaveIndex) = pBuff[nIndex];

		m_nSaveIndex++;
		if ( m_nSaveIndex  >=  m_nBufSaveSize )
		{
			UINT nBufSizeOld = m_nBufSaveSize;
			m_nBufSaveSize  +=  PLT_SECSIZE;

			char *pDataNew = new char[m_nBufSaveSize];
			memset( pDataNew, 0, m_nBufSaveSize*sizeof(char) );
			memcpy( pDataNew, m_pBufSave, nBufSizeOld*sizeof(char) );
			
			delete []m_pBufSave;
			m_pBufSave = pDataNew;
		}
		
		if ( pBuff[nIndex] == ';' )
		{
		#ifdef _UNICODE
			TCHAR *pWData = new TCHAR[m_nBufSaveSize];
			memset( pWData, 0, m_nBufSaveSize*sizeof(TCHAR) );

			MultiByteToWideChar( GetACP(), 0, m_pBufSave, strlen(m_pBufSave), pWData, m_nBufSaveSize );
			strPack = pWData;
			delete []pWData;
		#else
			strPack = m_pBufSave;
		#endif
			
			nIndex++;
			m_nSaveIndex = 0;
			memset( m_pBufSave, 0, m_nBufSaveSize*sizeof(char) );
			break;
		}

		nIndex++;
	}
	
	if ( m_nSaveIndex > 0 )
		return FALSE;

	//
	strPack.MakeUpper();
	strPack.Remove(0x0A);
	strPack.Remove(0x0D);

	return TRUE;
}



