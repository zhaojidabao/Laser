#pragma once

// CVectorMgr ÃüÁîÄ¿±ê
#include "Nodeline.h"


enum ENU_VECTORTYPE
{
	VECTOR_PLT,
	VECTOR_DXF,
	VECTOR_NC
};

class CVectorMgr : public CObject
{
public:
	CVectorMgr();
	virtual ~CVectorMgr();
	
	BOOL Decode(CString strFileName, CString strFileExt, CTree *pTree, CProperty pty, BOOL bCenter, BOOL bBetter);//nFlag:
	BOOL Decode(CString strFileName, CString strFileExt, CArray<CNodeLine *, CNodeLine *> &arrayNode, CProperty pty, BOOL bCenter, BOOL bBetter);
	BOOL SaveAs(CChain *pChain, int nVectorType, int nFlag = VECTOR_PLT);

	CProperty m_pty;

	int m_nAiType;
	int m_nAiFillType;
	double m_dbAiFillDis;
};

extern CVectorMgr g_VectorMgr;
