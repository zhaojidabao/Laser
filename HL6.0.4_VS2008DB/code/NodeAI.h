#if !defined(AFX_NODEAI_H__33ABD830_1CFB_4b3c_AB4A_DEECA3FCDDA7__INCLUDED_)
#define AFX_NODEAI_H__33ABD830_1CFB_4b3c_AB4A_DEECA3FCDDA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeV.h"
#include "Nodeline.h"
#include "Property.h"	// Added by ClassView

class CNodeAI : public CNodeV
{
public:
	CNodeAI();
	virtual ~CNodeAI();

public:
	virtual	BOOL SetLayerSelect(int nItem, BOOL bFlag);
	virtual	void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual	BOOL CanUnGroup();
	virtual	void LoadWheelList(int nType, StrokeList *pListStroke);
	virtual	CTree * UnGroup();
	CNodeLine* UnGroupAI(); 

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeAI();
		return pNode;
	};

private:
	void	CreatePen(int pen);
	//----------------------------------------------------
public:
	BOOL  DeCode(const CString& file,BOOL bCenter,BOOL bBetter);
	BOOL DeCodeNormalLine(const CString& file,BOOL bCenter,BOOL bBetter);
	BOOL DeCodeFillLine(const CString& file,BOOL bCenter,BOOL bBetter);

	int m_nAiType;
	int m_nAiFillType;
	double m_dbAiFillDis;
};

#endif // AFX_NODEAI_H__33ABD830_1CFB_4b3c_AB4A_DEECA3FCDDA7__INCLUDED_