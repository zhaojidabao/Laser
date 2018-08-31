// MgrText.h: interface for the CMgrText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MGRTEXT_H__A0FF7157_D1AA_4973_80BF_4D7BCE1763A2__INCLUDED_)
#define AFX_MGRTEXT_H__A0FF7157_D1AA_4973_80BF_4D7BCE1763A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "TextB.h"

//////////////////////////////////////////////////////////////////////////
//����ר�����������ı����ݵ����ݱ�
//�ڱ���ʱͨ��ID�ҵ���ص�TEXT����
//////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CMgrText
{
public:
	CMgrText();
	virtual ~CMgrText();
	
public:
	void		Clear();		 //����Ѿ�ʧЧ��CTextB����
	void		DeleteAll();	//ȫ�����
	void		FlashText(UINT nTextID,DWORD nStyle);
	BOOL		FlashAllText(DWORD nStyle);
	BOOL		AddText(CTextB* pTextNew);//����һ���ⲿ������Text����ID���ֲ���,����ɹ����ڲ��ͷ��ڴ�
	BOOL		AddText_NewID(CTextB* pTextNew);//����һ���ⲿ������Text����ID�ᱻ�Զ�����������ɹ����ڲ��ͷ��ڴ�

	void		AddBatch(CMgrText* pMgrText);	//������MgrText�����е�Text��ӵ����ж�����
	void		Retain(CUIntArray* pArryID);	//���pArrID����ID֮���Text����
	BOOL		GetCopySts();
	void		SetCopySts(BOOL bFlag);
	void		SaveText(FILE* fp,BOOL bFlag,BOOL bUnicode);
	BOOL		IsExist(UINT nID);
	CTextB*	GetTextByID(UINT nID);

	//nStyle=0x00ʱ��������һ���ŵĴ�����MARKTIMES����1
	BOOL		IsOverMark(UINT nID,DWORD nStyle=0x00);
	void		GetString(UINT nID,CString& strInfo,DWORD nStyle=0x00);
	UINT		GetNextID();
	UINT		GetLastID();
	BOOL		ResetBaseID(UINT nBase);
	void		ResetValidID();
	void		DeleteByID(UINT nTextID);
	void		ResetTextID( UINT nMinID,CUIntArray &arrIDMap,int nOpType = 0 );
	
	void operator=(const CMgrText &src);
		
	BOOL		TextReplace(CString& strTxt, CString& strAliases);//ͨ�����������ı��滻
	BOOL		GetTextByAliases(CString& strTxt, CString& strAliases);//ͨ��������ȡ�ı�����
	BOOL		IsHaveSerialObj();
	BOOL		GetValidID(UINT& nMinID,UINT &nMaxID);
	
	BOOL		GetTextExtInfo( int nTextId, LP_TEXTEXINFO lpTextExtInfo);
	BOOL		SetTextExtInfo( int nTextId, LP_TEXTEXINFO lpTextExtInfo);

protected:
	UINT		m_nTextNextID;
	BOOL		m_bIsCopy;	

private:
	CTypedPtrList<CObList,CTextB*>m_list;		//�ı������б�
	/////

};

#endif // !defined(AFX_MGRTEXT_H__A0FF7157_D1AA_4973_80BF_4D7BCE1763A2__INCLUDED_)
