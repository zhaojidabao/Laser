// Text.cpp : 实现文件
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

//Text序列化完全开放，定制可以根据需要对此部分进行调整
void	CText::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize fs(fp,bUnicode);
	
	if ( bFlag )
	{
		//写入头信息
		fs << _T("TXT_B");
		fs << 100;
		fs << m_bHex;				//是否是16进制显示
		fs << 101;
		fs << m_bRand;			//是否取随机数操作
		fs << 102;
		fs << m_nJumpDis;		//间隔设置
		fs << 103;
		fs << m_strText;			//文本内容
		fs << 104;
		fs << m_nText;			//文本方式:0固定文本1跳号文本3日期文本
		fs << 105;
		fs << m_strDate;			//日期格式
		fs << 106;
		CString strTmp = m_strFix;
		strTmp.Replace(_T("\r\n"),_T("\01"));
		fs << strTmp;				//固定部分文本
		fs << 107;
		fs << m_nMarkTimes;	//当前打标次数
		fs << 108;
		fs << _T("TEXT_FILE_BEGIN");	//取自于文件跳号时，将文件内容取出
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
		fs << m_strFromFile;		//取自于文件的文件名
		
		DWORD dwLoWord = ( m_nJumpStart & 0xFFFFFFFF ); 
		DWORD dwHiWord = ( m_nJumpStart & 0xFFFFFFFF00000000) >> 32; 
		fs  << 113;
		fs <<  dwLoWord;			//跳号开始点
		fs <<  dwHiWord;
		dwLoWord = ( m_nJumpEnd & 0xFFFFFFFF); 
		dwHiWord = ( m_nJumpEnd & 0xFFFFFFFF00000000 ) >> 32; 
		fs << 114;
		fs <<  dwLoWord;			//跳号结束点
		fs <<  dwHiWord;
		fs << 115;
		fs << m_nJumpLen;		//跳号长度
		fs << 116;
		fs << m_nJumpStep;		//步长
		fs << 117;
		fs << m_nFromFile;		//是否取自于文件
		fs << 118;
		fs << m_bFill;				//是否补齐位数
		fs << 119;
		fs << m_nTextID;
		fs << 120;
		fs << m_iDataset;		//条码字符集设置
		fs << 121;
		fs << m_strBinText;		//进制文本内容
		fs << 122;
		fs << m_bBarCode;		//是否是条码信息
		fs << 123;
		fs << m_nBarType;		//条码类型
		fs << 124;
		fs << m_bJumpLoop;
		
		//模板文本
		fs << 10000;
		fs << m_bModel;
		fs << 10002;                  
		fs << m_strSerialFileName;  //文件名
		fs << 10003;
		fs << m_nCurFileSerial;     //当前取自文件序号
		fs << 10004;
		fs << (int)m_FilePosition;  //文件指针位置
		fs << 10005;
		fs << m_strCurfileSerial;   //当前取自文件跳号内容
		fs << 10006;
		fs << m_strModelName;
		fs << 10007;
		fs << m_bOverMarkWarn;

		//语言信息
		fs << 10008;
		fs << m_strLocal;
		
		fs << 10010;
		SerializeExtInfo(fp, bFlag, bUnicode);
		
		//写入尾信息
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
					fs >> m_bHex;		//是否是16进制显示
					if ( m_bHex )
						m_strBinText = _T("0123456789ABCDEF");
					break;
				}
			case 101:
				fs >> m_bRand;		//是否取随机数操作
				break;
			case 102:
				fs >> m_nJumpDis;	//间隔设置
				break;
			case 103:
				fs >> m_strText;		//文本内容
				break;
			case 104:
				fs >> m_nText;		//文本方式:0固定文本1跳号文本3日期文本
				break;
			case 105:
				fs >> m_strDate;		//日期格式
				break;
			case 106:
				{
					fs >> str;			//固定部分文本
					str.Replace(_T("\01"),_T("\r\n"));
					m_strFix = str;
					break;
				}
			case 107:
				fs >> m_nMarkTimes;//当前打标次数
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
				fs >> m_strFromFile;		//取自于文件的文件名
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

			case 114: //跳号终点
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
				fs >> m_nJumpLen;			//跳号长度
				break;

			case 116:
				fs >> m_nJumpStep;			//步长
				break;

			case 117:
				fs >> m_nFromFile;			//是否取自于文件
				break;
			case 118:
				fs >> m_bFill;					//是否补齐位数
				break;
			case 119:
				fs >> m_nTextID;
				break;
			case 120:
				fs >> m_iDataset;			//条码字符集设置
				break;
			case 121:
				fs >> m_strBinText;			//进制文本内容
				break;
			case 122:
				fs >> m_bBarCode;			//是否是条码信息
				break;
			case 123:
				fs >> m_nBarType;
				break;
			case 124:
				fs >> m_bJumpLoop;
				break;
			case 10000:
				fs >> m_bModel;			//模板文本
				break;

			case 10001:
				{
					int  nModelNo;
					///////
					fs >> nModelNo;			//模板文本
					m_strModelName.Format(_T("%d"),nModelNo);
				}
				break;

			case 10002:
				fs >> m_strSerialFileName;
				break;

			case 10003:
				fs >> m_nCurFileSerial;     //当前取自文件序号
				break;

			case 10004:
				fs >> m_FilePosition;			//文件指针位置
				break;

			case 10005:
				fs >> m_strCurfileSerial;   //当前取自文件跳号内容
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

//创建一个当前文本的复本
CTextB* CText::NewCopy()
{
	CText *pText = new CText();
	if ( pText  !=  NULL )
	{
		pText->CopyAll(this);
	}
	////如有需要，此处添加定制代码
	//pText->m_bJumpLoop = m_bJumpLoop;

	return pText;
}


// 以下函数用于定制扩展用，如果需要，将相关代码激活即可应用
BOOL CText::FlashText(DWORD nStyle)
{
	////// 这里作循环跳号处理
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

//CopyAll是整体复制包括ID
void	CText::CopyAll( CTextB *pSrc )
{
	CText* pText = (CText*)pSrc;
	m_bJumpLoop = pText->m_bJumpLoop;
	CTextB::CopyAll(pSrc);
}

//CopyExceptID中ID不参与复制
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

	//此处添加定制代码
}

CString	CText::GetInfo()
{
	//此处添加定制代码

	return CTextB::GetInfo();
}


//CopyAll是整体复制包括ID
void	CText::CopyAll( CTextB *pSrc)
{
	CTextB::CopyAll(pSrc);
}

CString	CText::GetFileString(LONGLONG nCur)
{
	return CTextB::GetFileString(nCur);
}
*/

