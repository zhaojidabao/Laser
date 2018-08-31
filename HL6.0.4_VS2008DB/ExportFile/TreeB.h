// TreeB.h: interface for the CTreeB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREEB_H__A78FFD84_9C2F_4E19_A9B8_24472D5F97B8__INCLUDED_)
#define AFX_TREEB_H__A78FFD84_9C2F_4E19_A9B8_24472D5F97B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataBase.h"
#include "StrokeB.h"

class CTreeB;
typedef CTypedPtrList<CObList,CTreeB *> TREELIST;

class AFX_EXT_CLASS CTreeB : public CDataBase  
{	
public:
	CTreeB();
	virtual ~CTreeB();
	
public:
	BOOL IsMainSelect();
	void SetMainSelect(BOOL bSelect=TRUE);
	CDataBase* GetTopNode();
	
public:
	BOOL  m_bSelect;      // 选中标志 TRUE: 选中；FALSE: 不选中
	BOOL  m_bMainSelect;
	int   m_nMarkUnit;
	StrokeList m_listFill; //用于填充
	FILLDATA   m_Fill;
	CProperty  m_property;	    //填充线打标属性
	CProperty  m_propertyScal;	//缩进轮廓打标属性
	BOOL       m_bIsCurSortTree;  //是否为当前正在排序所选中的TREE
	BOOL       m_bIsUpdate;   //该变量只有在Do与Undo操作时有用，TRUE:在Do与Undo操作时填充Tree，FALSE:不填充
	DWORD64    m_ID;
	int        m_nGroupNum;
	OBJMARKSET m_MarkSet;
};

#endif // !defined(AFX_TREEB_H__A78FFD84_9C2F_4E19_A9B8_24472D5F97B8__INCLUDED_)
