#pragma once

#include "Node.h"
#include "TextB.h"
#include "MgrText.h"
#include "ChainBH.h"

// CNodeT_B 命令目标

class AFX_EXT_CLASS CNodeT_B : public CNode
{
public:
	CNodeT_B();
	virtual ~CNodeT_B();

public:
	BOOL		GetCopySts();
	BOOL		AddText(CTextB *pText);	// 添加一个CTextB对象
	CTextB*		GetText(UINT nIndex);	// 获取对应索引位的CTextB对象
	CTextB*		GetTextByID(UINT nID);	// 通过ID获取对应的CTextB对象
	BOOL		AddTextAll_NewID(CTextB* pText);	//向MgrText队列和自身新增加一个CTextB对象,TEXTID会被重新分配
	//////////////////
	UINT		GetTextCount();			// 获取对象中CTextB个数
	void		ClearAllText();			// 清除所有的CTextB对象
	void		DelTextByID(UINT nID);	// 清除指定ID的CTextB对象
	BOOL		IsBarText();			// 是否条码或条码关联文本
	BOOL		IsFixSerial();			// 是否固定内容
	BOOL		IsExistText(UINT nTextID);// 检查传入ID对应的CTextB内部是否存在

	virtual		void SetTextData(CString& strTxt);
	virtual		BOOL GetTextValue(CString& strTxt);
	virtual		BOOL GetTextString(DWORD dwStyle,BOOL &bMarkOver,CString &strRet);// 刷新且获取数据
	virtual		BOOL GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);
	virtual		BOOL SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);
	virtual		BOOL CheckBarCode(CUIntArray &arrayBarID,CUIntArray &arrayValidID);

protected:
	CString	m_strText;	//最终处理的文本内容
	/////
	BOOL    LoadTextByID( int nID, CUIntArray& arryMap);
	BOOL	CopyText(CNodeT_B *pSrc,DWORD dwStyle, int nJumpCnt = -1);
	void	ResetTextID(int nMinID, int nOpType,CUIntArray& arryMap);
	void	FlashText(DWORD dwStyle);
	virtual	BOOL GetSerializeString(CString &strRet);	
	BOOL	UpdateMgrText(BOOL bUpdate = TRUE);

// 以下变量严格管控，不允许随意访问
private:	
	CMgrText*  m_pMgrText;		// CTextB全局队列
	CUIntArray	 m_ArrayTextID;	// 文本ID数组
};


