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

typedef  CBaseNode* (*GENSTRING)(	char *  pFileName,			//pFileNameΪȫ·��
													char *  pText,					//pTextΪ�û�������ַ���
													BOOL    bFix,					//bFix��ʾ�Ƿ����ɹ̶����dis�ַ���
													double  dbDist,					//dbDist��ʾ�ַ����ľ���
													double  dbFontHeight,		//dbFontHeightΪ������Ҫ��������ָ�
													double  dbHeightRatio,		//dbHeightRatioΪ�߶�ϵ��
													double  dbWidthRatio,		//dbWidthRatioΪ���ϵ��
													double  dbRowWidth			//dbRowWidthΪ�м��
													);	

//����ֵΪCBaseNodeָ�룬ʹ����Ϻ��ɵ����߽����ͷ�

typedef  UserFONT* (*GETCHARPROPERTY)(	char *pFileName,		//pFileNameΪȫ·��
															unsigned char ch);	//chΪ����ѯ���ַ�
//����ֵΪUserFONT�ṹ��ָ�룬ʹ����Ϻ��ɵ����߽����ͷ�

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
