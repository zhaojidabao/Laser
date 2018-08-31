// Text.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Text.h"

// CText
CText::CText()
{
	m_bJumpLoop = FALSE;
}

CText::~CText()
{
}

//Text���л���ȫ���ţ����ƿ��Ը�����Ҫ�Դ˲��ֽ��е���
void	CText::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	
	if ( bFlag )
	{
		//д��ͷ��Ϣ
		fs << _T("TXT_B");
		fs << 100;
		fs << m_bHex;				//�Ƿ���16������ʾ
		fs << 101;
		fs << m_bRand;			//�Ƿ�ȡ���������
		fs << 102;
		fs << m_nJumpDis;		//�������
		fs << 103;
		fs << m_strText;			//�ı�����
		fs << 104;
		fs << m_nText;			//�ı���ʽ:0�̶��ı�1�����ı�3�����ı�
		fs << 105;
		fs << m_strDate;			//���ڸ�ʽ
		fs << 106;
		CString strTmp = m_strFix;
		strTmp.Replace(_T("\r\n"),_T("\01"));
		fs << strTmp;				//�̶������ı�
		fs << 107;
		fs << m_nMarkTimes;	//��ǰ������
		fs << 108;
		fs << _T("TEXT_FILE_BEGIN");	//ȡ�����ļ�����ʱ�����ļ�����ȡ��
		int n = m_saSerial.GetSize();
		for( int i = 0; i<n; i++ )
		{
			fs  <<  m_saSerial.GetAt(i);
		}
		fs << _T("TEXT_FILE_END");
		fs << 109;
		fs << m_strLeft;
		fs << 110;
		fs << m_strRight;
		fs << 111;
		fs << m_strPrefix;
		fs << 112;
		fs << m_strFromFile;		//ȡ�����ļ����ļ���
		
		DWORD dwLoWord = ( m_nJumpStart & 0xFFFFFFFF ); 
		DWORD dwHiWord = ( m_nJumpStart & 0xFFFFFFFF00000000) >> 32; 
		fs  << 113;
		fs <<  dwLoWord;			//���ſ�ʼ��
		fs <<  dwHiWord;
		dwLoWord = ( m_nJumpEnd & 0xFFFFFFFF); 
		dwHiWord = ( m_nJumpEnd & 0xFFFFFFFF00000000 ) >> 32; 
		fs << 114;
		fs <<  dwLoWord;			//���Ž�����
		fs <<  dwHiWord;
		fs << 115;
		fs << m_nJumpLen;		//���ų���
		fs << 116;
		fs << m_nJumpStep;		//����
		fs << 117;
		fs << m_nFromFile;		//�Ƿ�ȡ�����ļ�
		fs << 118;
		fs << m_bFill;				//�Ƿ���λ��
		fs << 119;
		fs << m_nTextID;
		fs << 120;
		fs << m_iDataset;		//�����ַ�������
		fs << 121;
		fs << m_strBinText;		//�����ı�����
		fs << 122;
		fs << m_bBarCode;		//�Ƿ���������Ϣ
		fs << 123;
		fs << m_nBarType;		//��������
		fs << 124;
		fs << m_bJumpLoop;
		
		//ģ���ı�
		fs << 10000;
		fs << m_bModel;
		fs << 10002;                  
		fs << m_strSerialFileName;  //�ļ���
		fs << 10003;
		fs << m_nCurFileSerial;     //��ǰȡ���ļ����
		fs << 10004;
		fs << (int)m_FilePosition;  //�ļ�ָ��λ��
		fs << 10005;
		fs << m_strCurfileSerial;   //��ǰȡ���ļ���������
		fs << 10006;
		fs << m_strModelName;
		fs << 10007;
		fs << m_bOverMarkWarn;

		//������Ϣ
		fs << 10008;
		fs << m_strLocal;
		
		fs << 10010;
		SerializeExtInfo(fp, bFlag, bUnicode);
		
		//д��β��Ϣ
		fs << _T("TXT_E");
	}
	else
	{
		CString str;
		while( fs >> str )
		{
			if ( str  == _T("TXT_E") )
				break;

			switch( _ttoi(str) ) 
			{
			case 100:
				{
					fs >> m_bHex;		//�Ƿ���16������ʾ
					if ( m_bHex )
						m_strBinText = _T("0123456789ABCDEF");
					break;
				}
			case 101:
				fs >> m_bRand;		//�Ƿ�ȡ���������
				break;
			case 102:
				fs >> m_nJumpDis;	//�������
				break;
			case 103:
				fs >> m_strText;		//�ı�����
				break;
			case 104:
				fs >> m_nText;		//�ı���ʽ:0�̶��ı�1�����ı�3�����ı�
				break;
			case 105:
				fs >> m_strDate;		//���ڸ�ʽ
				break;
			case 106:
				{
					fs >> str;			//�̶������ı�
					str.Replace(_T("\01"),_T("\r\n"));
					m_strFix = str;
					break;
				}
			case 107:
				fs >> m_nMarkTimes;//��ǰ������
				break;

			case 108:
				{
					BOOL bFlag = FALSE;
					m_saSerial.RemoveAll();

					while( fs >> str )
					{		
						if ( str == _T("TEXT_FILE_END") )
							break;

						if ( str == _T("TEXT_FILE_BEGIN"))
						{
							bFlag = TRUE;
							continue;
						}
						
						if ( bFlag )
						{
							m_saSerial.Add( str );
						}
					}
					break;
				}
			case 109:
				fs >> m_strLeft;
				break;

			case 110:
				fs >> m_strRight;
				break;

			case 111:
				fs >> m_strPrefix;
				break;

			case 112:
				fs >> m_strFromFile;		//ȡ�����ļ����ļ���
				break;

			case 113:
				if ( g_dwVersion  >=  53005 )
				{
					DWORD dwLoVal(0),dwHiVal(0);
					fs >> dwLoVal;
					fs >> dwHiVal;
					m_nJumpStart  = dwHiVal;
					m_nJumpStart <<=  32;	
					m_nJumpStart  |=  dwLoVal;
				}
				else
				{
					DWORD dwVal(0);
					fs >> dwVal;
					m_nJumpStart = dwVal;	
				}
				break;

			case 114: //�����յ�
				if ( g_dwVersion  >=  53005 )
				{
					DWORD dwLoVal(0),dwHiVal(0);
					fs >> dwLoVal;
					fs >> dwHiVal;
					m_nJumpEnd =  dwHiVal;
					m_nJumpEnd <<=  32;	
					m_nJumpEnd  |=  dwLoVal;
				}
				else
				{
					DWORD dwVal(0);
					fs >> dwVal;
					m_nJumpEnd = dwVal;	
				}
				break;

			case 115:
				fs >> m_nJumpLen;			//���ų���
				break;

			case 116:
				fs >> m_nJumpStep;			//����
				break;

			case 117:
				fs >> m_nFromFile;			//�Ƿ�ȡ�����ļ�
				break;
			case 118:
				fs >> m_bFill;					//�Ƿ���λ��
				break;
			case 119:
				fs >> m_nTextID;
				break;
			case 120:
				fs >> m_iDataset;			//�����ַ�������
				break;
			case 121:
				fs >> m_strBinText;			//�����ı�����
				break;
			case 122:
				fs >> m_bBarCode;			//�Ƿ���������Ϣ
				break;
			case 123:
				fs >> m_nBarType;
				break;
			case 124:
				fs >> m_bJumpLoop;
				break;
			case 10000:
				fs >> m_bModel;			//ģ���ı�
				break;

			case 10001:
				{
					int  nModelNo;
					///////
					fs >> nModelNo;			//ģ���ı�
					m_strModelName.Format(_T("%d"),nModelNo);
				}
				break;

			case 10002:
				fs >> m_strSerialFileName;
				break;

			case 10003:
				fs >> m_nCurFileSerial;     //��ǰȡ���ļ����
				break;

			case 10004:
				fs >> m_FilePosition;			//�ļ�ָ��λ��
				break;

			case 10005:
				fs >> m_strCurfileSerial;   //��ǰȡ���ļ���������
				break;
			case 10006:
				fs >> m_strModelName;
				break;

			case 10007:
				fs >> m_bOverMarkWarn;
				break;

			case 10008:
				fs >> m_strLocal;
				break;

			case 10009:
				{
					fs >> str;
					if ( str == _T("TXTEXT_B") )
					{
						SerializeExtInfoOld(fp, bFlag, bUnicode);
					}
				}
				break;

			case 10010:
				{
					fs >> str;
					if ( str == _T("TXTEXT_B") )
					{
						SerializeExtInfo(fp, bFlag, bUnicode);
					}
					break;
				}
				
			default:
				break;
			}
		}	
	}
}

//����һ����ǰ�ı��ĸ���
CTextB* CText::NewCopy()
{
	CText *pText = new CText();
	if ( pText  !=  NULL )
	{
		pText->CopyAll(this);
	}
	////������Ҫ���˴���Ӷ��ƴ���
	//pText->m_bJumpLoop = m_bJumpLoop;

	return pText;
}


// ���º������ڶ�����չ�ã������Ҫ������ش��뼤���Ӧ��
BOOL CText::FlashText(DWORD nStyle)
{
	////// ������ѭ�����Ŵ���
	if (  nStyle == 0x01  ) 
	{
		if ( m_nText == TEXT_SERIAL && m_bOverMark && m_bJumpLoop)
		{
			m_bOverMark = m_bOverMarkWarn = FALSE;
			m_nMarkTimes = -1;
		}
	}
	///////
	return CTextB::FlashText(nStyle);
}

//CopyAll�����帴�ư���ID
void	CText::CopyAll( CTextB *pSrc )
{
	CText* pText = (CText*)pSrc;
	m_bJumpLoop = pText->m_bJumpLoop;
	CTextB::CopyAll(pSrc);
}

//CopyExceptID��ID�����븴��
void CText::CopyExceptID(CTextB *pSrc)
{
	CText* pText = (CText*)pSrc;
	m_bJumpLoop = pText->m_bJumpLoop;
	CTextB::CopyExceptID(pSrc);
}
/*
void	CText::GenString()
{
	CTextB::GenString();

	//�˴���Ӷ��ƴ���
}

CString	CText::GetInfo()
{
	//�˴���Ӷ��ƴ���

	return CTextB::GetInfo();
}


//CopyAll�����帴�ư���ID
void	CText::CopyAll( CTextB *pSrc)
{
	CTextB::CopyAll(pSrc);
}

CString	CText::GetFileString(LONGLONG nCur)
{
	return CTextB::GetFileString(nCur);
}
*/

