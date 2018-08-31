
// OptStrokeList.cpp: implementation of the COptStrokeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "OptStrokeList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptStrokeList::COptStrokeList()
{
	m_pListSrc = NULL;
}

COptStrokeList::~COptStrokeList()
{
	ASSERT( NULL == m_pListSrc );
}

void COptStrokeList::Attach(StrokeList *pListSrc, double dbOptDist, BOOL bFull)
{
	ASSERT( NULL != pListSrc );
	////
	m_pListSrc = pListSrc;
	m_dbOptDist = dbOptDist;
	m_bOptFull = bFull;
}

void COptStrokeList::Detach()
{
	m_pListSrc = NULL;
}

//��ȫ�Ż�����
void COptStrokeList::FullOpt()
{
	ASSERT( TRUE == m_bOptFull && NULL != m_pListSrc );
	////
	StrokeList tmpList;
	tmpList.RemoveAll();
	////
	CStroke* pS;
	POSITION posSrc = m_pListSrc->GetHeadPosition();
	while( NULL != posSrc ){
		pS = (CStroke *)m_pListSrc->GetNext(posSrc);
		tmpList.AddTail(pS);
	}
	m_pListSrc->RemoveAll();
	////
	posSrc = tmpList.GetHeadPosition();
	while( NULL != posSrc ){
		pS = (CStroke *)tmpList.GetNext(posSrc);
		//����һ�����������Ż�
		InsertStrokeIntoList2(pS);
//		//�����Ż����Դ��Ч��������Ӱ��
//		InsertStrokeIntoList(pS);
	}
	tmpList.RemoveAll();
}

//�����Ż��������������
void COptStrokeList::PartOptEmptyStrokeList(BOOL bDestroy)
{
	ASSERT( FALSE == m_bOptFull && NULL != m_pListSrc );
	////
}

//�����Ż�����������������������
void COptStrokeList::PartOptInsertStroke(CStroke *pStroke)
{
	ASSERT( FALSE == m_bOptFull && NULL != m_pListSrc );
	////
}

void COptStrokeList::InsertStrokeIntoList(CStroke *pStroke)
{
	BOOL bOpted;
	long i,size,layerS;
	double dbTmp,dbDist;
	CStroke* pS = NULL;
	POSITION posCur,posPrev;
	CDot dotS,dotE,dotS2,dotE2;
	////
	size = pStroke->m_dotArray.GetSize();
	if ( NULL == pStroke || size < 1 )
	{
		m_pListSrc->AddTail(pStroke);
	}
	else{
		bOpted = FALSE;
		layerS = pStroke->GetOptLayer();
		dotS = pStroke->m_dotArray[0];
		dotE = pStroke->m_dotArray[size-1];
		dotS.LogToTrue();
		dotE.LogToTrue();
		posCur = m_pListSrc->GetHeadPosition();
		while( !bOpted && NULL != posCur ){
			posPrev = posCur;
			pS = (CStroke *)m_pListSrc->GetNext(posCur);
			if ( !pS )		continue;
			size = pS->m_dotArray.GetSize();
			if ( NULL != pS && size>0 && 
				layerS == pS->GetOptLayer() && 
				pStroke->IsInDots() == pS->IsInDots() ){
				////���2�������Ƿ�����ϲ�����
				dotS2 = pS->m_dotArray[0];
				dotE2 = pS->m_dotArray[size-1];
				dotS2.LogToTrue();
				dotE2.LogToTrue();
				////
				dbTmp = dotS.x-dotS2.x;
				dbDist = dotS.y-dotS2.y;
				dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
				if ( dbDist<m_dbOptDist ){//�׵��غ����׵�
					bOpted = TRUE;
					size = pStroke->m_dotArray.GetSize();
					for( i = 1;i<size;i++ ){
						pS->m_dotArray.InsertAt(0,pStroke->m_dotArray[i]);
					}
				}
				else{
					dbTmp = dotE.x-dotS2.x;
					dbDist = dotE.y-dotS2.y;
					dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
					if ( dbDist<m_dbOptDist ){//β���غ����׵�
						bOpted = TRUE;
						size = pStroke->m_dotArray.GetSize();
						for( i = size-2;i >= 0;i-- ){
							pS->m_dotArray.InsertAt(0,pStroke->m_dotArray[i]);
						}
					}
					else{
						dbTmp = dotS.x-dotE2.x;
						dbDist = dotS.y-dotE2.y;
						dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
						if ( dbDist<m_dbOptDist ){//�׵��غ���β��
							bOpted = TRUE;
							size = pStroke->m_dotArray.GetSize();
							for( i = 1;i<size;i++ ){
								pS->m_dotArray.Add(pStroke->m_dotArray[i]);
							}
						}
						else{
							dbTmp = dotE.x-dotE2.x;
							dbDist = dotE.y-dotE2.y;
							dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
							if ( dbDist<m_dbOptDist ){//β���غ���β��
								bOpted = TRUE;
								size = pStroke->m_dotArray.GetSize();
								for( i = size-2;i >= 0;i-- ){
									pS->m_dotArray.Add(pStroke->m_dotArray[i]);
								}
							}
						}
					}
				}
			}
		}
		////
		if ( bOpted ){
			pStroke->m_dotArray.RemoveAll();
		}
		m_pListSrc->AddTail(pStroke);
	}
}

void COptStrokeList::InsertStrokeIntoList2(CStroke *pStroke)
{
	BOOL bOpted;
	long i,size,layerS;
	double dbTmp,dbDist;
	CStroke* pS;
	POSITION posCur;
	CDot dotS,dotE,dotS2,dotE2;
	////
	size = pStroke->m_dotArray.GetSize();
	if ( NULL == pStroke || size<1 ){
		m_pListSrc->AddHead(pStroke);
	}
	else{
		bOpted = FALSE;
		layerS = pStroke->GetOptLayer();
		dotS = pStroke->m_dotArray[0];
		dotE = pStroke->m_dotArray[size-1];
		dotS.LogToTrue();
		dotE.LogToTrue();
		posCur = m_pListSrc->GetTailPosition();
		if (posCur != NULL)
		{
			pS = (CStroke *)m_pListSrc->GetNext(posCur);
			size = pS->m_dotArray.GetSize();
			if ( NULL != pS && size>0 && 
				layerS == pS->GetOptLayer() && 
				pStroke->IsInDots() == pS->IsInDots() ){
				////���2�������Ƿ�����ϲ�����
				dotS2 = pS->m_dotArray[0];
				dotE2 = pS->m_dotArray[size-1];
				dotS2.LogToTrue();
				dotE2.LogToTrue();
				////
				dbTmp = dotS.x-dotS2.x;
				dbDist = dotS.y-dotS2.y;
				dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
				if ( dbDist<m_dbOptDist ){//�׵��غ����׵�
					bOpted = TRUE;
					size = pStroke->m_dotArray.GetSize();
					for( i = 1;i<size;i++ ){
						pS->m_dotArray.InsertAt(0,pStroke->m_dotArray[i]);
					}
				}
				else{
					dbTmp = dotE.x-dotS2.x;
					dbDist = dotE.y-dotS2.y;
					dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
					if ( dbDist<m_dbOptDist ){//β���غ����׵�
						bOpted = TRUE;
						size = pStroke->m_dotArray.GetSize();
						for( i = size-2;i >= 0;i-- ){
							pS->m_dotArray.InsertAt(0,pStroke->m_dotArray[i]);
						}
					}
					else{
						dbTmp = dotS.x-dotE2.x;
						dbDist = dotS.y-dotE2.y;
						dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
						if ( dbDist<m_dbOptDist ){//�׵��غ���β��
							bOpted = TRUE;
							size = pStroke->m_dotArray.GetSize();
							for( i = 1;i<size;i++ ){
								pS->m_dotArray.Add(pStroke->m_dotArray[i]);
							}
						}
						else{
							dbTmp = dotE.x-dotE2.x;
							dbDist = dotE.y-dotE2.y;
							dbDist = sqrt(dbTmp*dbTmp+dbDist*dbDist);
							if ( dbDist<m_dbOptDist ){//β���غ���β��
								bOpted = TRUE;
								size = pStroke->m_dotArray.GetSize();
								for( i = size-2;i >= 0;i-- ){
									pS->m_dotArray.Add(pStroke->m_dotArray[i]);
								}
							}
						}
					}
				}
			}
			////
			if ( bOpted ){
				pStroke->m_dotArray.RemoveAll();
			}
		}
		m_pListSrc->AddTail(pStroke);
	}
}
