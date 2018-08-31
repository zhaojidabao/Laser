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
	virtual	void	CopyAll(CTextB *pSrc);		//CopyExceptID �� CopyAll ������
															//CopyExceptID��ID�����븴�ƣ���CopyAll�����帴���Ұ���ID
	virtual	CString	GetFileString(LONGLONG nCur);
	//
	void		ReferenceAdd();
	void		ReferenceReduce();

	//��չ��Ϣ	
	virtual BOOL		GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo );
	virtual BOOL		SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo );
	virtual BOOL		SetTextData(CString& strTxt);
	virtual BOOL		TextReplace(CString& strTxt, CString& strAliases);
	virtual BOOL		GetTextByAliases(CString& strTxt, CString& strAliases);

public:
	int			m_nMarkTimes;	//������
	BOOL		m_bHex;				//ʮ�����Ʊ�ʶ
	BOOL		m_bRand;
	int			m_nJumpDis;
	int			m_iDataset;
	
	int			m_nText;			//�ı���ʽ����̶��ı��������ı��������ı�
	CString	m_strDate;
	CString	m_strFix;

	CStringArray m_saSerial;	//������������
	
	//�����ı�
	BOOL			m_bFill;			//�Ƿ���
	CString		m_strPrefix;		//����λ���ַ�
	CString		m_strFromFile;	//����ȡ�����ļ�֮����
	
	LONGLONG  m_nJumpStart;//���ſ�ʼ
	LONGLONG  m_nJumpEnd;  //���Ž���

	int			m_nJumpLen;		//���Ų��ֳ���
	int			m_nJumpStep;		//����ʱ����
	int			m_nFromFile;		//�Ƿ�����ȡ�����ļ�

	CString	m_strLeft;			//����ǰ���ַ���
	CString	m_strRight;			//���ź����ַ���
	CString	m_strBinText;		//�����ı�����
	
	//���������ı���������ѡ���ı�ʱ���ܶ�������޸�ֻ���޸����ı�����
	//ֻ����ѡ������ʱ���ܶԱ����������޸�
	BOOL		m_bBarCode;		//�Ƿ��������ı�
	UINT		m_nBarType;		//��������
	
	BOOL		m_bOverMark;			//���Ž���
	BOOL		m_bOverMarkWarn;	//������β����

	BOOL		m_bModel;
	CString	m_strModelName;
	CString	m_strSerialFileName;
	
	int			m_nCurFileSerial;
	DWORD	m_FilePosition;
	CString	m_strCurfileSerial;
	CString	m_strLocal;         //������Ϣ
	
protected:
	int			m_nRefeNum;
	BOOL		m_bUse;				//�����ڱ���ʱȥ��������õ��ı���Ϣ
	
	UINT		m_nTextID;
	CString	m_strBarString;	//�����ִ�
	CString	m_strText;			//��ǰ�ı�

	TEXTEXINFO	m_extInfo;
	CRITICAL_SECTION  m_csObj;

protected:
	CString	FormatString(LONGLONG nVaL);
	void	    SerializeExtInfoOld(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void	    SerializeExtInfo(FILE* fp,BOOL bFlag,BOOL bUnicode);
};

#endif // !defined(AFX_TEXT_H__445AF437_EBB6_49DC_9087_48900DC5F3C1__INCLUDED_)
