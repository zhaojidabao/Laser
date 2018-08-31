#ifndef		__INNERMSG_H_98520F42_6FD0_424F_B982_87F79749E32F
#define		__INNERMSG_H_98520F42_6FD0_424F_B982_87F79749E32F

#include "baseHatch.h"

#define EPSILON  (0.000001)
#define SAMEVALUE(a,b)	( fabs(a-b) < EPSILON )
#define DIFFERVALUE(a,b)	( fabs(a-b) > EPSILON )

//以下是本软件所需的宏定义
#define PAI				3.141592653589793		//圆周率
#define CR				((char)0x0D+(char)0x0A)	//回车
#define CENTER		0				//实际坐标中心点

//以下两个宏纯属用于坐标变换
#define LCENTER			16384			//逻辑中心点
#define DPMM				50				//逻辑坐标中的每毫米显示的点数

#define PTY_INIT			0	//初始化
#define PTY_LAYER		1	//层颜色改变
#define PTY_PARA		2	//层打标参数改变
#define PTY_OUTLINE	3	//移动边框
#define PTY_DEL			4	//删除层
#define PTY_FILL			5	//移动填充体
#define PTY_ADD			6	//增加新层
#define PTY_CHANGE	7	//改变当前层
#define PTY_FILL_COUNTOUR		8	//移动填充体

//定义颜色
#define  BLACK 		RGB(0,0,0)
#define  RED   		RGB(255,0,0)
#define	GREEN 		RGB(0,255,0)
#define	BLUE  		RGB(0,0,255)
#define	YELLOW		RGB(255,255,0)
#define	MAGENTA	RGB(255,0,255)
#define	YANGREEN	RGB(0,255,255)
#define	GRAY	 	    RGB(127,127,127)

//定义基本点位置
#define BASE_TL			0
#define BASE_TM			1
#define BASE_TR			2
#define BASE_ML			3
#define BASE_MM			4
#define BASE_MR			5
#define BASE_BL			6
#define BASE_BM			7
#define BASE_BR			8

//拾取点的位置
#define	PT					0
#define	TRACKLEFT		5	
#define	TRACKRIGHT	7
#define	UPPER			4
#define	LOWER			6
#define	UPPER_LEFT	0
#define	UPPER_RIGHT	1
#define	LOWER_LEFT	3
#define	LOWER_RIGHT	2
#define TRACKCENTER	9	//中心点

// 定义节点类型
enum ENU_OBJTYPE
{	
	NODE_BASE			= 0,		// 未知节点
	NODE_LINE			= 1,		// 曲线 
	NODE_RECT			= 2,		// 矩形
	NODE_HLTEXT		= 3,		// 文本
	NODE_SERIAL		= 4,		// 跳号
	NODE_BMP			= 5,		// BMP
	NODE_ELLIPSE		= 6,		// 圆
	NODE_PLT			= 7,		// PLT
	NODE_GRID			= 8,		// 网格
	NODE_POLYGON	= 9,		// 多边形
	NODE_DXF			= 10,		// DXF
	NODE_CODEBAR	= 11,		// 二维条码
	NODE_COMBIN     = 12,		// 组合对象
	/*打标对象*/
	NODE_DELAY		= 13,		//延时
	NODE_INPUT		= 14,		//输入
	NODE_OUTPUT		= 15,		//输出
	//
	NODE_SPLINE      = 16,		//样条曲线
	NODE_ARC			= 17,		//三点方式圆弧
	NODE_CIRCLE      = 18,		//圆
	NODE_DOT			= 19,		//点
	NODE_UNION       = 20,		//群组对象
	NODE_MOTORMOVE	= 21, //运动控制对象
	NODE_MOTORPOS		= 22, //运动轴位置设定
	NODE_CONDITIONS	= 23, //条件判断对象
	NODE_CONDITIONE	= 24, //条件判断对象
	NODE_NC				= 25,
	NODE_BUFFERLASER = 26, //缓冲区激光控制对象
	NODE_CCD				= 27, //CCD对象
	NODE_POSCALI		= 28,	//位置校正对象
	NODE_AI					= 29,
	NODE_3DMODEL		= 30,
	NODE_3DFRAME		= 31,
	NODE_DST			= 32,
	NODE_END
};

const USHORT OBJ_MAX_MARKCOUNT = 6;

typedef struct tagWToC
{	
	double   dbW;
	double   dbC;
	//////////
	tagWToC()
	{
		::memset(this,0,sizeof(tagWToC));
	};
	void operator=( const tagWToC & src )
	{
		if( &src == this )	
			return;
		//////////
		dbW   = src.dbW;
		dbC   = src.dbC;
	}
} WTOC;

//斜线填充参数
struct GPara
{
	double DisLoop;
};

//轮廓填充参数
struct CPara
{
	double Dis;	
	int iLoopNum;
};

//缩进填充参数
struct RPara
{
	//基本填充参数
	double	Dis;          //填充间距
	double	OriAngle;     //填充角度
	BOOL	bFillToAndFro;//往复填充
	
	//多次填充
	int		nFillNum;     //填充次数
	double	ProAngle;     //累进角度
	double	AddDis;       //累进间距	
	
	//优化设置	
	double	dbStartOffset;//起始偏移
	double	dbEndOffset;  //末尾偏移
	double	dbLineReduce; //线缩进量
	BOOL	bEqualDis;    //等间距

	//轮廓环绕设置
	double	Reduce;       //缩进边距
	int		iLoopNum;     //缩扩圈数
	BOOL	bInner;       //内向
	BOOL	bOuter;       //外向
	
	//
	BOOL	bSaveCon;     //保留缩进
	BOOL	bSaveFLine;   //保留填充线
	BOOL	bConFirst;    //先生成轮廓

	double dbLineReduceEnd;	// 线尾点缩进量
};


//特殊打标处理
typedef struct tagSubObjMarkSet
{	
	tagSubObjMarkSet()
	{
		bIsMarkFram   = FALSE;
		bIsMarkFill   = FALSE;
        bIsFillFirst  = FALSE;
		nLayer        = 0;
		nSubCnt       = 1;
		bIsRevMark    = FALSE;
	};
	
	void operator = ( const tagSubObjMarkSet & src )
	{
		if ( &src == this )
			return;
		
		bIsMarkFram= src.bIsMarkFram;
		bIsMarkFill	= src.bIsMarkFill;
        bIsFillFirst	= src.bIsFillFirst;
		nLayer       = src.nLayer;
		nSubCnt		= src.nSubCnt;
		bIsRevMark = src.bIsRevMark;
	}
	
public:
	int 		nSubCnt;
	int			nLayer;
	BOOL		bIsMarkFram;
	BOOL 	bIsMarkFill;
	BOOL 	bIsFillFirst;
	BOOL 	bIsRevMark;
} SUBOBJMARKSET;



typedef struct tagObjMarkSet
{	
	tagObjMarkSet()
	{
		nMarkCount = 0;
	};
	
	void operator = ( const tagObjMarkSet & src )
	{
		if( &src == this )	
			return;
		//////////////
		nMarkCount   = src.nMarkCount;
		memcpy(stSub, src.stSub, sizeof(SUBOBJMARKSET) * OBJ_MAX_MARKCOUNT);
		if (nMarkCount > OBJ_MAX_MARKCOUNT) 
		{
			nMarkCount = OBJ_MAX_MARKCOUNT; // 最多只能打六遍
		}
	}
	
	int   nMarkCount;
	SUBOBJMARKSET stSub[OBJ_MAX_MARKCOUNT];
}OBJMARKSET;


//
#define USER_RIGHT_ADMIN			0x0001  //0:管理员     1
#define USER_RIGHT_NORMALMARK 0x0002  //1:通用打标   2
#define USER_RIGHT_MULMARK		0x0004  //2:多文档打标 4
#define USER_RIGHT_CREATE			0x0008  //3:创建对象   8
#define USER_RIGHT_EDIT				0x0010  //4:对象编辑   16
#define USER_RIGHT_SYSTEMSET	0x0020  //5:系统设置   32
#define USER_RIGHT_CALI				0x0040  //6:校正设置   64
#define USER_RIGHT_SAVE			0x0080  //7:文件保存   128
#define USER_RIGHT_OPEN			0x0100  //8:打开文档   256
#define USER_RIGHT_LAYERSET		0x0200  //9:层参数设置 512
#define USER_RIGHT_ADVANCE		0x0400  //10:高级参数  1024

typedef struct tagUserInfo
{
	tagUserInfo()
	{
		memset(this,0,sizeof(tagUserInfo));
	};

	void operator = ( const tagUserInfo & src )
	{
		if( &src == this )	
			return;
		
		dwRight1   = src.dwRight1;
		dwRight2   = src.dwRight2;
		nFlag = src.nFlag;
		bIsDefaultUser = src.bIsDefaultUser;
		memcpy(&szUserName, &(src.szUserName), 64*sizeof(TCHAR));
		memcpy(&szPassWord, &(src.szPassWord), 64*sizeof(TCHAR));
	}
	
	TCHAR    szUserName[64];
	TCHAR    szPassWord[64];
	BOOL     bIsDefaultUser;
	int			nFlag;
	
	// Bit 0:管理员;   1:通用打标; 2:多文档打标; 3:创建对象; 4:对象编辑
	//     5:系统设置; 6:校正设置; 7:文件保存;   8:打开文档; 9:层参数设置
	//    10:高级参数
	DWORD   dwRight1;
	DWORD   dwRight2;
} USERINFO;


typedef struct tagRightInfo
{
	tagRightInfo()
	{
		memset(this,0,sizeof(tagRightInfo));
	};
	
public:
	int		nVersion;
	int     nUserCount;		
}RIGHTINFO;


#endif //__INNERMSG_H_98520F42_6FD0_424F_B982_87F79749E32F