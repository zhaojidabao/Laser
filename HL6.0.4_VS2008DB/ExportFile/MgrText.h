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
//此类专门用来处理文本内容的数据表
//在遍例时通过ID找到相关的TEXT数据
//////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CMgrText
{
public:
	CMgrText();
	virtual ~CMgrText();
	
public:
	void		Clear();		 //清除已经失效的CTextB对象
	void		DeleteAll();	//全部清除
	void		FlashText(UINT nTextID,DWORD nStyle);
	BOOL		FlashAllText(DWORD nStyle);
	BOOL		AddText(CTextB* pTextNew);//插入一个外部创建的Text对象，ID保持不变,插入成功后内部释放内存
	BOOL		AddText_NewID(CTextB* pTextNew);//插入一个外部创建的Text对象，ID会被自动调整，插入成功后内部释放内存

	void		AddBatch(CMgrText* pMgrText);	//将传入MgrText队列中的Text添加到现有队列中
	void		Retain(CUIntArray* pArryID);	//清除pArrID数组ID之外的Text对象
	BOOL		GetCopySts();
	void		SetCopySts(BOOL bFlag);
	void		SaveText(FILE* fp,BOOL bFlag,BOOL bUnicode);
	BOOL		IsExist(UINT nID);
	CTextB*	GetTextByID(UINT nID);

	//nStyle=0x00时不进行下一个号的处理，即MARKTIMES不加1
	BOOL		IsOverMark(UINT nID,DWORD nStyle=0x00);
	void		GetString(UINT nID,CString& strInfo,DWORD nStyle=0x00);
	UINT		GetNextID();
	UINT		GetLastID();
	BOOL		ResetBaseID(UINT nBase);
	void		ResetValidID();
	void		DeleteByID(UINT nTextID);
	void		ResetTextID( UINT nMinID,CUIntArray &arrIDMap,int nOpType = 0 );
	
	void operator=(const CMgrText &src);
		
	BOOL		TextReplace(CString& strTxt, CString& strAliases);//通过别名进行文本替换
	BOOL		GetTextByAliases(CString& strTxt, CString& strAliases);//通过别名获取文本内容
	BOOL		IsHaveSerialObj();
	BOOL		GetValidID(UINT& nMinID,UINT &nMaxID);
	
	BOOL		GetTextExtInfo( int nTextId, LP_TEXTEXINFO lpTextExtInfo);
	BOOL		SetTextExtInfo( int nTextId, LP_TEXTEXINFO lpTextExtInfo);

protected:
	UINT		m_nTextNextID;
	BOOL		m_bIsCopy;	

private:
	CTypedPtrList<CObList,CTextB*>m_list;		//文本数据列表
	/////

};

#endif // !defined(AFX_MGRTEXT_H__A0FF7157_D1AA_4973_80BF_4D7BCE1763A2__INCLUDED_)
