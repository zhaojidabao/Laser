#pragma once

#include "TextB.h"
// CText ����Ŀ��

class CText : public CTextB
{
public:
	CText();
	virtual ~CText();

public:
	virtual	void		Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual    CTextB*  NewCopy();

public:
	BOOL m_bJumpLoop;	// ѭ������ add by zhq

	virtual	BOOL		FlashText(DWORD nStyle);
//	virtual	void		GenString();
//	virtual	CString	GetInfo();
	virtual	void		CopyExceptID(CTextB *pSrc);
	virtual	void		CopyAll(CTextB *pSrc);		//CopyExceptID �� CopyAll ������
																//CopyExceptID��ID�����븴�ƣ���CopyAll�����帴���Ұ���ID
//	virtual	CString	GetFileString(LONGLONG nCur);

protected:
	// Add new variable below here

};


