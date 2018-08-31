// Layer.cpp: implementation of the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Layer.h"
#include "Property.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLayer,CObject,2000)

CLayer::CLayer()
{
	m_color	 =  RGB(0,0,0);
	m_nLayer =  0;
	m_bDefault = TRUE;
	m_bLockFlag = FALSE;

	m_para.emcc = g_DAT.m_stEMCCParaDft;
}


CLayer::CLayer(int nCardType)
{
	m_color	 =  RGB(0,0,0);
	m_nLayer =  0;
	m_bDefault = TRUE;
	m_bLockFlag = FALSE;

	TCHAR szPath[256];
	GetExepath(szPath,256);
	TCHAR szFileName[256];
	_stprintf_s(szFileName,256, _T("%s%s"), szPath, _T("default.yh"));
	
	BOOL bUnicode = CStdioFileEx::IsFileUnicode(szFileName);
	FILE *fp = NULL;
	if ( ( fp = _tfopen(szFileName,bUnicode ? _T("rb") : _T("rt") ) ) == NULL )
	{
		PARA para;
		m_para = para;//初始化
	}
	else
	{
		Serialize(fp, FALSE, nCardType,bUnicode);
		fclose(fp);
	}
}


CLayer::~CLayer()
{

}

void CLayer::operator = (const CLayer& src)
{
	m_nLayer		 =  src.m_nLayer;
	m_color			 =  src.m_color;
	m_bDefault		 =  src.m_bDefault;
	m_para			 =  src.m_para;
	m_bLockFlag	 =  src.m_bLockFlag;
}



CLayer::CLayer(const CLayer& src)
{
	m_nLayer		 =  src.m_nLayer;
	m_color			 =  src.m_color;
	m_bDefault		 =  src.m_bDefault;
	m_bLockFlag	 =  src.m_bLockFlag;
	m_para			 =  src.m_para;
}



void CLayer::Serialize(FILE* fp,BOOL bFlag, int nCardType,BOOL bUnicode)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp,bUnicode);
		
	if (bFlag)
	{
		//写入头信息
		fs <<  _T("B_Y");
		fs << 100;
		fs << m_nLayer;
		fs << 101;
		fs << m_color;
		fs << 102;
		fs << m_bDefault;
		fs << 136;
		fs << m_bLockFlag;
		if (EMCC == nCardType) 
		{
			WritedEmcc(fs);
		}
		
		//写入尾信息
		fs << _T("E_Y");
	}
	else
	{
		CString str;
		BOOL bNewPara = FALSE; //是否为新层参数,不是的话将激光关延时赋值给打标延时
		while( fs >> str )
		{
			//以后改进更好的方式，这种方式效率低下
			if (str == _T("B_Y"))
			{
				continue;
			}
			else if ( str == _T("100") )
			{
				fs >> m_nLayer;
			}
			else if ( str == _T("101") )
			{
				fs >> m_color;
			}
			else if ( str == _T("102") )
			{
				fs >> m_bDefault;
			}
			else if ( str == _T("136") )
			{
				fs >> m_bLockFlag;
			}
			else if ( str == _T("E_Y") )
			{
				if ( ! bNewPara )
					m_nLayer = -1;
				break;
			}
			else
			{
				if ( EMCC  !=  nCardType )
				{
					continue;
				}
				else
				{
					BOOL bRet = ReadEmcc(fs, str);
					if ( TRUE == bRet )
					{
						bNewPara = TRUE;
						continue;
					}
					else
					{
						fs >> str;
						continue;
					}
				}
			}
		}//end while
	}//end else
}



BOOL CLayer::ReadEmcc(CMySerialize &fs, CString &str)
{
	BOOL bRead = FALSE;
	CString strAlias = str.Left(3);

	PARA_CFG stParaCfg;
	stParaCfg.m_strAlias = strAlias;
	BOOL bCfg = g_DAT.GetLayerParaCfg(&stParaCfg);
	
	/////////////////
	if ( strAlias == _T("CNT") )
	{
		fs >> m_para.emcc.nCount;
		bRead = TRUE;

		if ( bCfg ) {
			m_para.emcc.nCount = m_para.emcc.nCount>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nCount;
			m_para.emcc.nCount = m_para.emcc.nCount<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nCount;
		}
	}
	else if ( strAlias == _T("ASV") )
	{
		fs >> m_para.emcc.dbMarkV;
		bRead = TRUE;

		if ( bCfg ) {
			m_para.emcc.dbMarkV = m_para.emcc.dbMarkV>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbMarkV;
			m_para.emcc.dbMarkV = m_para.emcc.dbMarkV<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbMarkV;
		}
	}
	else if ( strAlias == _T("ESV") )
	{
		fs >> m_para.emcc.dbJumpV;
		bRead = TRUE;
		
		if ( bCfg ) {
			m_para.emcc.dbJumpV = m_para.emcc.dbJumpV>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbJumpV;
			m_para.emcc.dbJumpV = m_para.emcc.dbJumpV<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbJumpV;
		}
	}
	else if ( strAlias == _T("QSF") )
	{
		fs  >>  m_para.emcc.dbQFreq;
		bRead = TRUE;

		if ( bCfg ) {
			m_para.emcc.dbQFreq = m_para.emcc.dbQFreq>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbQFreq;
			m_para.emcc.dbQFreq = m_para.emcc.dbQFreq<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbQFreq;
		}
	}
	else if ( strAlias == _T("QRT") )
	{
		fs  >>  m_para.emcc.dbQRls;
		bRead = TRUE;

		if ( bCfg ) {
			m_para.emcc.dbQRls = m_para.emcc.dbQRls>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbQRls;
			m_para.emcc.dbQRls = m_para.emcc.dbQRls<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbQRls;
		}
	}
	else if ( strAlias == _T("WAT") )
	{
		fs   >>   m_para.emcc.dbCurrent;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.dbCurrent = m_para.emcc.dbCurrent>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbCurrent;
			m_para.emcc.dbCurrent = m_para.emcc.dbCurrent<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbCurrent;
		}
	}
	else if ( strAlias == _T("FPT") )
	{
		fs  >>  m_para.emcc.nFpkTime;
		bRead = TRUE;
		
		if ( bCfg ) {
			m_para.emcc.nFpkTime = m_para.emcc.nFpkTime>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nFpkTime;
			m_para.emcc.nFpkTime = m_para.emcc.nFpkTime<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nFpkTime;
		}
	}
	else if ( strAlias == _T("FIP") )
	{
		fs  >>  m_para.emcc.dbFPKInitRate;
		bRead = TRUE;
		
		if ( bCfg ) {
			m_para.emcc.dbFPKInitRate = m_para.emcc.dbFPKInitRate>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbFPKInitRate;
			m_para.emcc.dbFPKInitRate = m_para.emcc.dbFPKInitRate<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbFPKInitRate;
		}
	}
	else if ( strAlias == _T("FAP") )
	{
		fs  >>  m_para.emcc.dbFPKLenRate;
		bRead = TRUE;

		if ( bCfg ) {
			m_para.emcc.dbFPKLenRate = m_para.emcc.dbFPKLenRate>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbFPKLenRate;
			m_para.emcc.dbFPKLenRate = m_para.emcc.dbFPKLenRate<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbFPKLenRate;
		}
	}
	else if ( strAlias == _T("FMP") )
	{
		fs  >>  m_para.emcc.dbFPKMaxPRate;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.dbFPKMaxPRate = m_para.emcc.dbFPKMaxPRate>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbFPKMaxPRate;
			m_para.emcc.dbFPKMaxPRate = m_para.emcc.dbFPKMaxPRate<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbFPKMaxPRate;
		}
	}
	else if ( strAlias == _T("FCR") )
	{
		fs  >>  m_para.emcc.dbFPKCurRate;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.dbFPKCurRate = m_para.emcc.dbFPKCurRate>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbFPKCurRate;
			m_para.emcc.dbFPKCurRate = m_para.emcc.dbFPKCurRate<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbFPKCurRate;
		}
	}
	else if ( strAlias == _T("MQR") )
	{
		fs >> m_para.emcc.nQuality;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.nQuality = m_para.emcc.nQuality>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nQuality;
			m_para.emcc.nQuality = m_para.emcc.nQuality<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nQuality;
		}
	}
	else if ( strAlias == _T("LDT") )
	{
		fs >> m_para.emcc.nLayerDelay;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.nLayerDelay = m_para.emcc.nLayerDelay>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nLayerDelay;
			m_para.emcc.nLayerDelay = m_para.emcc.nLayerDelay<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nLayerDelay;
		}
	}
	else if ( strAlias == _T("ODL") )
	{
		fs >> m_para.emcc.nLaserOnDelay;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.nLaserOnDelay = m_para.emcc.nLaserOnDelay>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nLaserOnDelay;
			m_para.emcc.nLaserOnDelay = m_para.emcc.nLaserOnDelay<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nLaserOnDelay;
		}
	}
	else if ( strAlias == _T("FDL") )
	{
		fs >> m_para.emcc.nLaserOffDelay;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.nLaserOffDelay = m_para.emcc.nLaserOffDelay>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nLaserOffDelay;
			m_para.emcc.nLaserOffDelay = m_para.emcc.nLaserOffDelay<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nLaserOffDelay;
		}
	}
	else if ( strAlias == _T("MLT") )
	{
		fs >> m_para.emcc.nMoveLineDelay;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.nMoveLineDelay = m_para.emcc.nMoveLineDelay>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nMoveLineDelay;
			m_para.emcc.nMoveLineDelay = m_para.emcc.nMoveLineDelay<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nMoveLineDelay;
		}
	}
	else if ( strAlias == _T("JDL") )
	{
		fs >> m_para.emcc.nJumpDelay;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.nJumpDelay = m_para.emcc.nJumpDelay>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nJumpDelay;
			m_para.emcc.nJumpDelay = m_para.emcc.nJumpDelay<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nJumpDelay;
		}
	}
	else if ( strAlias == _T("MDL") )
	{
		fs >> m_para.emcc.nRoundDelay;
		bRead = TRUE;	
		
		if ( bCfg ) {
			m_para.emcc.nRoundDelay = m_para.emcc.nRoundDelay>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nRoundDelay;
			m_para.emcc.nRoundDelay = m_para.emcc.nRoundDelay<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nRoundDelay;
		}
	}
	else if ( strAlias == _T("CUR") )
	{
		fs >> m_para.emcc.dbCurrentR;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.dbCurrentR = m_para.emcc.dbCurrentR>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.dbCurrentR;
			m_para.emcc.dbCurrentR = m_para.emcc.dbCurrentR<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.dbCurrentR;
		}
	}
	else if ( strAlias == _T("SMC") )
	{
		fs >> m_para.emcc.dbSimmerCur;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.dbSimmerCur = m_para.emcc.dbSimmerCur>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbSimmerCur;
			m_para.emcc.dbSimmerCur = m_para.emcc.dbSimmerCur<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbSimmerCur;
		}
	}
	else if ( strAlias == _T("WAF") || strAlias == _T("PLS") )
	{
		fs >> m_para.emcc.nWaveForm;
		bRead = TRUE;	

		if (bCfg) 
		{
			m_para.emcc.nWaveForm = m_para.emcc.nWaveForm>stParaCfg.m_dbMax ? (int)stParaCfg.m_dbMax : m_para.emcc.nWaveForm;
			m_para.emcc.nWaveForm = m_para.emcc.nWaveForm<stParaCfg.m_dbMin ? (int)stParaCfg.m_dbMin : m_para.emcc.nWaveForm;
		}
	}
	else if ( strAlias == _T("POW") )
	{
		fs >> m_para.emcc.dbPowerR;
		bRead = TRUE;	

		if ( bCfg ) {
			m_para.emcc.dbPowerR = m_para.emcc.dbPowerR>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbPowerR;
			m_para.emcc.dbPowerR = m_para.emcc.dbPowerR<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbPowerR;
		}
	}
	else if ( strAlias == _T("SMP") )
	{
		fs >> m_para.emcc.dbSimmerCur;
		bRead = TRUE;	

		if ( bCfg )
		{
			m_para.emcc.dbSimmerCur = m_para.emcc.dbSimmerCur>stParaCfg.m_dbMax ? stParaCfg.m_dbMax : m_para.emcc.dbSimmerCur;
			m_para.emcc.dbSimmerCur = m_para.emcc.dbSimmerCur<stParaCfg.m_dbMin ? stParaCfg.m_dbMin : m_para.emcc.dbSimmerCur;
		}
	}

	return bRead;
}


CProperty CLayer::GetProperty()
{
	CProperty pty;
	pty.m_color	 = m_color;
	pty.m_nLayer = m_nLayer;
	pty.m_bLockFlag = m_bLockFlag;

	return pty;
}

void CLayer::WritedEmcc(CMySerialize &fs)
{
	fs <<  _T("CNT");
	fs << m_para.emcc.nCount;
	
	fs << _T("ASV");
	fs << m_para.emcc.dbMarkV;
	
	fs << _T("ESV");
	fs << m_para.emcc.dbJumpV;
	
	fs << _T("QSF");
	fs << m_para.emcc.dbQFreq;
	
	fs << _T("QRT");
	fs << m_para.emcc.dbQRls;
	
	fs << _T("WAT");
	fs << m_para.emcc.dbCurrent;
	
	fs << _T("FPT");
	fs << m_para.emcc.nFpkTime;
	
	fs << _T("FIP");
	fs << m_para.emcc.dbFPKInitRate;
	
	fs << _T("FAP");
	fs << m_para.emcc.dbFPKLenRate;
	
	fs << _T("FMP");
	fs << m_para.emcc.dbFPKMaxPRate;
	
	fs << _T("FCR");
	fs << m_para.emcc.dbFPKCurRate;
	
	fs << _T("MQR");
	fs << m_para.emcc.nQuality;
	
	fs << _T("LDT");
	fs << m_para.emcc.nLayerDelay;
	
	fs << _T("ODL");
	fs << m_para.emcc.nLaserOnDelay;
	
	fs << _T("FDL");
	fs << m_para.emcc.nLaserOffDelay;
	
	fs << _T("MLT");
	fs << m_para.emcc.nMoveLineDelay;
	
	fs << _T("JDL");
	fs << m_para.emcc.nJumpDelay;
	
	fs << _T("MDL");
	fs << m_para.emcc.nRoundDelay;
	
	fs << _T("CUR");
	fs << m_para.emcc.dbCurrentR;
	
	fs << _T("SMC");
	fs << m_para.emcc.dbSimmerCur;
	
	if ( g_DAT.m_strEMCCLaser == _T("M_IPG") || g_DAT.m_strEMCCLaser == _T("M_IPG_GENERAL") )
	{
		fs << _T("PLS");
	}
	else
	{
		fs << _T("WAF");
	}
	
	fs << m_para.emcc.nWaveForm;
	
	fs << _T("POW");
	fs << m_para.emcc.dbPowerR;
	
	fs << _T("SMP");
	fs << m_para.emcc.dbSimmerCur;
}

void CLayer::UpdatePara()
{
}

void CLayer::SetColor(COLORREF color)
{
	m_color = color;
}
