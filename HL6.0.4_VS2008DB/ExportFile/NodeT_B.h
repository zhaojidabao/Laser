#pragma once

#include "Node.h"
#include "TextB.h"
#include "MgrText.h"
#include "ChainBH.h"

// CNodeT_B ����Ŀ��

class AFX_EXT_CLASS CNodeT_B : public CNode
{
public:
	CNodeT_B();
	virtual ~CNodeT_B();

public:
	BOOL		GetCopySts();
	BOOL		AddText(CTextB *pText);	// ���һ��CTextB����
	CTextB*		GetText(UINT nIndex);	// ��ȡ��Ӧ����λ��CTextB����
	CTextB*		GetTextByID(UINT nID);	// ͨ��ID��ȡ��Ӧ��CTextB����
	BOOL		AddTextAll_NewID(CTextB* pText);	//��MgrText���к�����������һ��CTextB����,TEXTID�ᱻ���·���
	//////////////////
	UINT		GetTextCount();			// ��ȡ������CTextB����
	void		ClearAllText();			// ������е�CTextB����
	void		DelTextByID(UINT nID);	// ���ָ��ID��CTextB����
	BOOL		IsBarText();			// �Ƿ��������������ı�
	BOOL		IsFixSerial();			// �Ƿ�̶�����
	BOOL		IsExistText(UINT nTextID);// ��鴫��ID��Ӧ��CTextB�ڲ��Ƿ����

	virtual		void SetTextData(CString& strTxt);
	virtual		BOOL GetTextValue(CString& strTxt);
	virtual		BOOL GetTextString(DWORD dwStyle,BOOL &bMarkOver,CString &strRet);// ˢ���һ�ȡ����
	virtual		BOOL GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);
	virtual		BOOL SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);
	virtual		BOOL CheckBarCode(CUIntArray &arrayBarID,CUIntArray &arrayValidID);

protected:
	CString	m_strText;	//���մ�����ı�����
	/////
	BOOL    LoadTextByID( int nID, CUIntArray& arryMap);
	BOOL	CopyText(CNodeT_B *pSrc,DWORD dwStyle, int nJumpCnt = -1);
	void	ResetTextID(int nMinID, int nOpType,CUIntArray& arryMap);
	void	FlashText(DWORD dwStyle);
	virtual	BOOL GetSerializeString(CString &strRet);	
	BOOL	UpdateMgrText(BOOL bUpdate = TRUE);

// ���±����ϸ�ܿأ��������������
private:	
	CMgrText*  m_pMgrText;		// CTextBȫ�ֶ���
	CUIntArray	 m_ArrayTextID;	// �ı�ID����
};


