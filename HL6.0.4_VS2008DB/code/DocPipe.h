#ifndef FENCER_2006_0627_1136
#define FENCER_2006_0627_1136

#include "Chain.h"
#include "Quad.h"
#include "mfccmd.h"
#include "Lay.h"

#define MULDOC_VERSION "5.3.5"

typedef struct Motor_Para
{
	int nAxisNo;    //轴号
	int nMoveModel; //运动模式： 0 绝对，1 相对
	int nMoveType;  //运动类型 0-工作,1-空程，2-归零
	double dbMoveValue; //运动量

	CStringA strIO;			//多文档打标、下载共用此值(I8-I4)	
	WORD uIOValid;		//I15-I4的有效值
	
	Motor_Para()
	{
		nAxisNo = 0;
		nMoveModel = 0;
		nMoveType = 0;
		dbMoveValue = 0.0;

		strIO = "0000";
		uIOValid = 0xF;
	};
	
	void operator = (const Motor_Para& src){
		nAxisNo = src.nAxisNo;
		nMoveModel = src.nMoveModel;
		nMoveType = src.nMoveType;
		dbMoveValue = src.dbMoveValue;

		strIO = src.strIO;
		uIOValid = src.uIOValid;
	};

	WORD StrIOToWORD ()
	{
		WORD wIO = 0;
		for ( int i = 0; i < strIO.GetLength(); ++i ) 
		{
			wIO <<=  1;
			if ( '1' == strIO[i])
			{
				wIO  |=  1;
			}
		}

		return wIO;
	}

	void WordToStrIO(WORD wIO, int nBit)
	{
		strIO.Empty();
		for (int i = 0; i < nBit; ++i) 
		{
			strIO.Insert(0, wIO & 0x1 ? "1" : "0" );
			wIO  >>=  1;
		}
	}
}MOTOR;


struct MULDOCFILE 
{
	~MULDOCFILE()
	{
		arName.RemoveAll();
		arOption.RemoveAll();
	}

	BOOL Serialize(const CString& strFileName, BOOL bLoad)
	{
		BOOL bRet = FALSE;
		CFile fp;
		if (bLoad) 
		{
			bRet = fp.Open(strFileName,CFile::modeRead );
		}
		else
		{
			bRet = fp.Open(strFileName,CFile::modeWrite|CFile::modeCreate );
		}
		
		if (!bRet) 
		{
			return FALSE;
		}
		
		int nDoc(0);
		MOTOR motor;
		CStringA strVer;
		CStringA str;
		CArchive ar(&fp,bLoad?CArchive::load:CArchive::store);
		if (bLoad) 
		{
			ar >> strVer;
			if (strVer>MULDOC_VERSION) 
			{
				ar.Close();
				fp.Close();
				return FALSE;		//不支持低版软件打开高版文件
			}
			
			ar >> nDoc;
			for(int i = 0;i<nDoc;i++)
			{
				ar >> str;
				arName.Add(str);
				
				if (strVer.CompareNoCase("5.3.5") == 0)
				{
					ar >> motor.nAxisNo;
					ar >> motor.nMoveModel;
					ar >> motor.nMoveType;
					ar >> motor.dbMoveValue;

					ar >> motor.strIO;
					ar >> motor.uIOValid;
				}
				else
				{	
					int nMove, nDirection, nDelay;
					ar >> nMove;
					ar >> nDirection;
					ar >> nDelay;

					if (strVer.CompareNoCase("2009.03.15") == 0)
					{
						ar >> motor.strIO;
					}
					else 
					{
						if (strVer  >=  ("5.3.4"))
						{
							ar >> motor.strIO;
							ar >> motor.uIOValid;
						}					
					}
				}
				
				arOption.Add(motor);
			}

			if (strVer.CompareNoCase("5.3.5")  !=  0)
			{
				double dbFreq;
				ar >> dbFreq;
			}
		}
		else
		{
			str = MULDOC_VERSION;
			ar  << str;
			ar  << arName.GetSize();
			
			for(int i = 0;i<arName.GetSize();i++)
			{
				ar << arName[i];
				ar << arOption[i].nAxisNo;
				ar << arOption[i].nMoveModel;
				ar << arOption[i].nMoveType;
				ar << arOption[i].dbMoveValue;
				ar << arOption[i].strIO;
				ar << arOption[i].uIOValid;
			}
		}
		
		ar.Close();
		fp.Close();
		
		return TRUE;
	}
	
	CArray<CStringA, CStringA> arName; //文档名
	CArray<MOTOR, MOTOR&> arOption;	//文档属性
};

#endif