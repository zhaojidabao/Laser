#pragma once

#include "ChainB.h"
#include "MgrText.h"


// CChainBH ÃüÁîÄ¿±ê
class AFX_EXT_CLASS CChainBH : public CChainB
{
	DECLARE_DYNAMIC( CChainBH )  

public:
	CChainBH();
	virtual ~CChainBH();
	
	BOOL SerializeTxT(CTextB *pText,FILE* fp, BOOL bUnicode,CUIntArray &arryID);
	//
public:
	CMgrText m_MgrText;

};


