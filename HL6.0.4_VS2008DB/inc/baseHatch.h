
#pragma once

#define FILLMINDIS  0.00001

typedef struct baseHatch
{
	baseHatch()
	{
		::memset(this, 0, sizeof(baseHatch));
		dbSpace = 0.01;
		nLoopNum = 1;
		dbLoopDist = 0.01;
		nFillNum = 1;
	}
	
public:
	BOOL	bEnable;		//填充使能
	BOOL	bEquaDist;		//等间距使能
	BOOL	bKeepAngle;		//填充线绕轮廓旋转
	int		nFillStyle;		//填充类型
	double	dbSpace;		//填充间距
	double	dbAngle;		//填充角度
	double	dbBeamComp;		//光栅大小
	double	dbLineReduce;   //线首点缩进量
	double	dbStartOffset;  //起始缩进量
	double	dbEndOffset;    //末尾缩进量
	double	dbMinJump;      //最小跳转间距
	
	BOOL	bLoop;			//缩扩使能
	int		nLoopNum;		//缩扩圈数
	double	dbLoopDist;		//缩扩间距
	BOOL	bRemoveLine;	//缩扩后去掉填充线
	BOOL	bRemoveOrg;		//缩扩后去掉原始轮廓
	
	int    nFillNum;		//填充次数
	double ProAngle;		//累进角度
	double AddDis;			//累进间距
	BOOL   bSaveCountour;	//填充后保留外框
	double dbLineReduceEnd;	// 线尾点缩进量
}BaseHatch;

typedef struct HATCH_PARA 
{
	HATCH_PARA()
	{
		::memset(this, 0, sizeof(HATCH_PARA));
		
		newFileData1.dbSpace = 0.01;
		newFileData1.dbLoopDist = 0.01;
		newFileData1.nLoopNum = 1;
		newFileData1.dbAngle  = 0;
		newFileData1.nFillNum = 1;
		
		newFileData2.dbSpace = 0.01;
		newFileData2.dbLoopDist = 0.01;
		newFileData2.nLoopNum = 1;
		newFileData2.dbAngle = 90;
		newFileData2.nFillNum = 1;
		
		bFillFirst = TRUE;
	}
	
public:
	BaseHatch  newFileData1;
	BaseHatch  newFileData2;
	BOOL 	   bFillFirst;          //先打填充
	BOOL       bFillAlign;          //打标时填充线对齐

}HATCHPARA,*LP_HATCHPARA;

typedef BaseHatch  NEW_FILL_DATA;
typedef HATCHPARA  FILLDATA;