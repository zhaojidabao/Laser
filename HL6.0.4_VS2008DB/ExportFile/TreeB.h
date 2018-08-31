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
	BOOL  m_bSelect;      // ѡ�б�־ TRUE: ѡ�У�FALSE: ��ѡ��
	BOOL  m_bMainSelect;
	int   m_nMarkUnit;
	StrokeList m_listFill; //�������
	FILLDATA   m_Fill;
	CProperty  m_property;	    //����ߴ������
	CProperty  m_propertyScal;	//���������������
	BOOL       m_bIsCurSortTree;  //�Ƿ�Ϊ��ǰ����������ѡ�е�TREE
	BOOL       m_bIsUpdate;   //�ñ���ֻ����Do��Undo����ʱ���ã�TRUE:��Do��Undo����ʱ���Tree��FALSE:�����
	DWORD64    m_ID;
	int        m_nGroupNum;
	OBJMARKSET m_MarkSet;
};

#endif // !defined(AFX_TREEB_H__A78FFD84_9C2F_4E19_A9B8_24472D5F97B8__INCLUDED_)
