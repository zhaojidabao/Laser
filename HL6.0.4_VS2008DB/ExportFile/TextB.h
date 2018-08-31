// TextB.h: interface for the CTextB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__445AF437_EBB6_49DC_9087_48900DC5F3C1__INCLUDED_)
#define AFX_TEXT_H__445AF437_EBB6_49DC_9087_48900DC5F3C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CTextB : public CObject
{
	friend class CMgrText;

public:
	CTextB();
	virtual ~CTextB();
	
public:
	BOOL		IsValid(){ return m_bUse; };
	UINT		GetTextID(){ return m_nTextID; };

	virtual	BOOL	FlashText(DWORD nStyle);
	virtual	void	GenString();
	virtual	void	Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode) = 0;

	virtual	CString	GetInfo();
	CString	GetBarString(){return m_strBarString;};
	CString	GetString(){ return m_strText; };

	virtual    CTextB*  NewCopy() = 0;
	virtual	void	CopyExceptID(CTextB *pSrc);
	virtual	void	CopyAll(CTextB *pSrc);		//CopyExceptID 与 CopyAll 的区别
															//CopyExceptID中ID不参与复制，而CopyAll是整体复制且包括ID
	virtual	CString	GetFileString(LONGLONG nCur);
	//
	void		ReferenceAdd();
	void		ReferenceReduce();

	//扩展信息	
	virtual BOOL		GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo );
	virtual BOOL		SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo );
	virtual BOOL		SetTextData(CString& strTxt);
	virtual BOOL		TextReplace(CString& strTxt, CString& strAliases);
	virtual BOOL		GetTextByAliases(CString& strTxt, CString& strAliases);

public:
	int			m_nMarkTimes;	//打标次数
	BOOL		m_bHex;				//十六进制标识
	BOOL		m_bRand;
	int			m_nJumpDis;
	int			m_iDataset;
	
	int			m_nText;			//文本方式，如固定文本，日期文本，跳号文本
	CString	m_strDate;
	CString	m_strFix;

	CStringArray m_saSerial;	//跳号序列数据
	
	//跳号文本
	BOOL			m_bFill;			//是否补齐
	CString		m_strPrefix;		//补齐位数字符
	CString		m_strFromFile;	//跳号取自于文件之名字
	
	LONGLONG  m_nJumpStart;//跳号开始
	LONGLONG  m_nJumpEnd;  //跳号结束

	int			m_nJumpLen;		//跳号部分长度
	int			m_nJumpStep;		//跳号时步长
	int			m_nFromFile;		//是否跳号取自于文件

	CString	m_strLeft;			//跳号前置字符串
	CString	m_strRight;			//跳号后置字符串
	CString	m_strBinText;		//进制文本内容
	
	//由于条码文本的内容在选中文本时不能对其进行修改只能修改其文本参数
	//只有在选中条码时才能对本参数进行修改
	BOOL		m_bBarCode;		//是否是条码文本
	UINT		m_nBarType;		//条码类型
	
	BOOL		m_bOverMark;			//跳号结束
	BOOL		m_bOverMarkWarn;	//跳号至尾报警

	BOOL		m_bModel;
	CString	m_strModelName;
	CString	m_strSerialFileName;
	
	int			m_nCurFileSerial;
	DWORD	m_FilePosition;
	CString	m_strCurfileSerial;
	CString	m_strLocal;         //语言信息
	
protected:
	int			m_nRefeNum;
	BOOL		m_bUse;				//用于在保存时去除多的无用的文本信息
	
	UINT		m_nTextID;
	CString	m_strBarString;	//条码字串
	CString	m_strText;			//当前文本

	TEXTEXINFO	m_extInfo;
	CRITICAL_SECTION  m_csObj;

protected:
	CString	FormatString(LONGLONG nVaL);
	void	    SerializeExtInfoOld(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void	    SerializeExtInfo(FILE* fp,BOOL bFlag,BOOL bUnicode);
};

#endif // !defined(AFX_TEXT_H__445AF437_EBB6_49DC_9087_48900DC5F3C1__INCLUDED_)
