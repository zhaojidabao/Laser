#pragma once
#include "node.h"
#include "Property.h"

class CNodeDst :public CNodeV
{
public:
	CNodeDst(void);
	~CNodeDst(void);
	BOOL DeCode(CString file,BOOL bCenter,BOOL bBetter);

public:
	virtual	BOOL SetLayerSelect(int nItem, BOOL bFlag);
	virtual	void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual	BOOL CanUnGroup();
	virtual	void LoadWheelList(int nType, StrokeList *pListStroke);
	virtual	CTree * UnGroup();
	CNodeLine* UnGroupDST(); 

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeDst();
		return pNode;
	};

protected:
	CFile		*m_pFile;
	CDot		m_dotCur;
	CStroke		*m_pStroke;
};
