// TextObject.h: interface for the CTextObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTOBJECT_H__8BCD93FF_5479_494B_8727_968BBAD39E7E__INCLUDED_)
#define AFX_TEXTOBJECT_H__8BCD93FF_5479_494B_8727_968BBAD39E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�����������ı��������������Եļ��ϣ�ֻ�������ı�����ʱ�õ�
//�������
//CTextObject 
// |____________________| ......
//CTextSet             CTextSet  ��ʱ���ǿ���һ�����壬���˶Ի�������óɸ��Զ����Ķ���
// |_________|          |_________|
//CNodeText CText      CNodeText CText

#include <afxtempl.h>
#include "TextSet.h"

class CTextObject  
{
public:
	CTextObject();
	virtual ~CTextObject();

public:
	CTextSet* GetItem(UINT nIndex);
	void Delete();
	void DeleteItem(UINT nIndex);
	void AddText(CTextSet* pTextSet);
	CTypedPtrList<CObList,CTextSet*>m_list;		//�ı������б�
};

#endif // !defined(AFX_TEXTOBJECT_H__8BCD93FF_5479_494B_8727_968BBAD39E7E__INCLUDED_)
