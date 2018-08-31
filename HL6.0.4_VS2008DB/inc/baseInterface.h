#ifndef __HL_79D5442D_3EBB_46B7_89F6_2086B40DC5FC_H
#define __HL_79D5442D_3EBB_46B7_89F6_2086B40DC5FC_H

#include <afxtempl.h>
#include "baseHatch.h"

//层参数结构体定义
typedef struct LAYER_PARA
{
	int			nCount;				//打标次数
	double	dbMarkV;			//矢量打标速度(mm/s)
	double	dbJumpV;			//矢量空跳速度(mm/s)
	double	dbQFreq;			//Q频(KHz)
	double	dbQRls;				//Q释放(us)
	double	dbCurrent;			//电流(A)
	int			nFpkTime;			//首脉冲抑制时间	
	double	dbFPKInitRate;		//首点比例(0.0~1.0)	
	double	dbFPKLenRate;		//抑制长度系数(0.0~0.999)
	double	dbFPKMaxPRate;	//峰值能量比例(0.0~1.0)
	double	dbFPKCurRate;		//首脉冲电流系数(0.0~1.0)
	int			nQuality;			//打标质量系数
	int			nLayerDelay;		//层延时
	int			nLaserOnDelay;	//激光开延时(us)
	int			nLaserOffDelay;	//激光关延时(us)
	int			nMoveLineDelay;	//走笔延时(us)
	int			nJumpDelay;		//跳转延时(us)
	int			nRoundDelay;		//拐弯延时(us)
	double	dbCurrentR;			//电流(%)
	double	dbSimmerCur;		//维持电流(%)
	int			nWaveForm;		//波形
	double	dbPowerR;			//功率(%)
	
	void Assign(const LAYER_PARA& src)
	{
		memcpy(this, &src, sizeof(LAYER_PARA));
	}
	
	BOOL Equal(const LAYER_PARA & src)
	{
		if( &src == this )	
		{
			return TRUE;
		}
		
		return (
			nCount == src.nCount   &&
			dbMarkV == src.dbMarkV &&
			dbJumpV == src.dbJumpV &&
			dbQFreq  == src.dbQFreq  &&
			dbQRls == src.dbQRls &&
			dbCurrent == src.dbCurrent &&
			nFpkTime == src.nFpkTime &&
			dbFPKInitRate == src.dbFPKInitRate &&
			dbFPKLenRate == src.dbFPKLenRate &&
			dbFPKMaxPRate == src.dbFPKMaxPRate &&
			dbFPKCurRate == src.dbFPKCurRate &&
			nQuality == src.nQuality &&
			nLayerDelay == src.nLayerDelay &&
			nLaserOnDelay == src.nLaserOnDelay &&
			nLaserOffDelay == src.nLaserOffDelay &&
			nMoveLineDelay == src.nMoveLineDelay &&
			nJumpDelay == src.nJumpDelay &&
			nRoundDelay == src.nRoundDelay &&
			dbCurrentR == src.dbCurrentR &&
			dbSimmerCur == src.dbSimmerCur &&
			nWaveForm == src.nWaveForm && 
			dbPowerR  == src.dbPowerR 
			);
	}
} LAYERPARA,*LP_LAYERPARA;

//电机运动参数结构体定义
typedef struct MOTOR_PARA
{
	WORD   nVer;			//版本号,预留0
	//////////////////
	WORD   nAxis;			//轴号
	WORD   nMoveMode;		//运动模式:0-绝对运动,1-相对运动
	LONG   nMovePos;		//运动量或位置
	UINT   nMoveSpeed;		//运动速度(脉冲/秒)
	UINT   nMoveAcc;		//加速度(递增脉冲/秒)
	UINT   nDelay;			//运动后延时
}*LP_MOTOR_PARA;

//字体参数结构体定义
typedef struct  FONT_PARA
{
	int     nFontType;			//字体类型 0-TTF，1-SHX，2-HANS，3-DOT
    BYTE    nCharSet;			//字符集,Unicode编码时用
	char   szFontName[100];		//TTF、HANS字体用
	char   szShxCN [100];		//SHX、DOT字中文字体用
	char   szShxEN[100];		//SHX、DOT字英文字体用
	char   szShxNum [100];
} FONTPARA,*LP_FONTPARA;

typedef LAYERPARA  EMCCPARA;

//对外接口点数据结构
class CInterDot
{
public:
	CInterDot()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	CInterDot(double x1,double y1)
	{
		x = x1;
		y = y1;
	}

	CInterDot(double x1,double y1,double z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}

	CInterDot & operator=(const CInterDot &dot)
	{
		if ( this == &dot )
			return *this;
		else
		{
			x = dot.x;
			y = dot.y;
			z = dot.z;
		}
		return *this;
	}

public:
	double  x;	//x
	double  y;	//y
	double  z;	//z (3D预留)
};

//对外接口线条数据类
class CInterStroke: public CObject
{
public:
	CInterStroke()
	{
		m_nLayer = 0;
		m_dotArray.RemoveAll();
	}
	
	virtual ~CInterStroke (){
		m_dotArray.RemoveAll();
	}
	
	void AddDot(CInterDot& dot){
		m_dotArray.Add(dot);
	}

	void SetLayer( int nLayer ){
		m_nLayer = nLayer;
	}

	CInterStroke & operator=(const CInterStroke &stroke)
	{
		if ( this == &stroke )
			return *this;
		else
		{
			m_nLayer = stroke.m_nLayer;
			m_dotArray.RemoveAll();
			m_dotArray.Append(stroke.m_dotArray);
		}
		return *this;
	}
	
public:
	CArray<CInterDot,CInterDot> m_dotArray;
	int   m_nLayer;
};

//Text对象扩展属性
typedef struct  TEXT_EXINFO
{
	BOOL		bVar;              //是否为可变对象	
	TCHAR	szVarName[100];    //可变文本别名

	int		nRow;
	int		nCol;
	/////////////////
	TCHAR   szExtInfo[100];
	TCHAR	szInfo[10][100];
	int		    nValue[10];
	double	dbValue[10];
	DWORD	dwValue[10];

	TEXT_EXINFO()
	{
		InitData();
	}

	void InitData()
	{
		bVar = FALSE;
		memset( szVarName, 0, 100*sizeof(TCHAR) );		

		nRow = -1;
		nCol = -1;

		memset( szExtInfo, 0, 100*sizeof(TCHAR) );

		for(int i=0; i<10; i++)
		{
			memset( szInfo[i], 0, 100*sizeof(TCHAR) );
			nValue[i] = 0;
			dbValue[i] = 0.0;
			dwValue[i] = 0;
		}
	}

	void operator=(const TEXT_EXINFO &src)
	{
		if ( this == &src )
			return;

		bVar = src.bVar;
		_tcscpy(szVarName,src.szVarName);

		nRow = src.nRow;
		nCol = src.nCol;

		_tcscpy(szExtInfo, szExtInfo);

		for(int i=0; i<10; i++)
		{
			_tcscpy(szInfo[i], src.szInfo[i]);
			nValue[i] = src.nValue[i];
			dbValue[i] = src.dbValue[i];
			dwValue[i] = src.dwValue[i];
		}		
	}

} TEXTEXINFO,*LP_TEXTEXINFO;

//DOC扩展属性
typedef struct  DOC_EXINFO
{
	TCHAR   szExtInfo[10][100];
	int		    nReserved[10];
	double	dbReserved[10];
	DWORD	dwReserved[10];

	DOC_EXINFO()
	{
		InitData();
	}

	void InitData()
	{
		memset(this,0,sizeof(DOC_EXINFO));
	}

	void operator=(const DOC_EXINFO &src)
	{
		if ( this == &src )
			return;
		
		memcpy(this,&src,sizeof(DOC_EXINFO));
	}
} DOCEXINFO,*LP_DOCEXINFO;

//对外接口线条数据链表
typedef CTypedPtrList<CObList, CInterStroke *> InterStrokeList;



typedef struct BUF_CAMERA_PARA
{
	WORD  nVer;				//版本号,0
	///////
	UINT		nProgID;			//程序号
	UINT		nNGRepNum;	//重试次数
	UINT		nOverTime;		//超时时间(ms)
	UINT		nErrorMode;	//错误处理
	UINT		nBaudRate;		//波特率
	BOOL		bScanJump;	//振镜强制跳转
	double   dbX;				//振镜X位置(mm)
	double   dbY;				//振镜Y位置(mm)
	UINT		nDelay;			//跳转延时(ms)
}*LP_CAMERA_PARA;



















#endif ///__HL_79D5442D_3EBB_46B7_89F6_2086B40DC5FC_H