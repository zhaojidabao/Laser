#pragma once

#include "TextB.h"
// CText 命令目标

class CText : public CTextB
{
public:
	CText();
	virtual ~CText();

public:
	virtual	void		Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual    CTextB*  NewCopy();

public:
	BOOL m_bJumpLoop;	// 循环跳号 add by zhq

	virtual	BOOL		FlashText(DWORD nStyle);
//	virtual	void		GenString();
//	virtual	CString	GetInfo();
	virtual	void		CopyExceptID(CTextB *pSrc);
	virtual	void		CopyAll(CTextB *pSrc);		//CopyExceptID 与 CopyAll 的区别
																//CopyExceptID中ID不参与复制，而CopyAll是整体复制且包括ID
//	virtual	CString	GetFileString(LONGLONG nCur);

protected:
	// Add new variable below here

};


