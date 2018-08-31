// FontUser.h: interface for the CFontUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTUSER_H__6067BDDF_EECB_4749_B922_30BFCDC526A7__INCLUDED_)
#define AFX_FONTUSER_H__6067BDDF_EECB_4749_B922_30BFCDC526A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Fnt.h"
#include "UserFont.h"

typedef  CBaseNode* (*GENSTRING)(	char *  pFileName,			//pFileName为全路径
													char *  pText,					//pText为用户输入的字符串
													BOOL    bFix,					//bFix表示是否生成固定间距dis字符串
													double  dbDist,					//dbDist表示字符中心距离
													double  dbFontHeight,		//dbFontHeight为调用者要求的最终字高
													double  dbHeightRatio,		//dbHeightRatio为高度系数
													double  dbWidthRatio,		//dbWidthRatio为宽度系数
													double  dbRowWidth			//dbRowWidth为行间距
													);	

//返回值为CBaseNode指针，使用完毕后由调用者进行释放

typedef  UserFONT* (*GETCHARPROPERTY)(	char *pFileName,		//pFileName为全路径
															unsigned char ch);	//ch为待查询的字符
//返回值为UserFONT结构的指针，使用完毕后由调用者进行释放

class CFontUser : public CFnt
{
public:
	CFontUser();
	virtual ~CFontUser();
	virtual BOOL MakeStroke();
	
	CString m_strFileName;

private:
	HMODULE	m_hModule;
	GENSTRING m_pGenString;
};

#endif // !defined(AFX_FONTUSER_H__6067BDDF_EECB_4749_B922_30BFCDC526A7__INCLUDED_)
