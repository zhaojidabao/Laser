#ifndef __24DA3DE5_CBCC_492C_81B1_F65E74483348_
#define __24DA3DE5_CBCC_492C_81B1_F65E74483348_

#include "..\inc\baseStruct.h"
#include "..\inc\baseInterface.h"

#define LAYERCOUNT		256

//定义线程，开始与结束
#define WM_MARK_END					( WM_USER+5 )
#define WM_MARK_START				( WM_USER+6 )
#define WM_MARK_STOP				( WM_USER+7 )
#define IDC_STATUS_BAR_BUTTON	( WM_USER+8 )
#define IDC_STATUS_PROGRESSCtrl ( WM_USER+9 )
#define WM_MARK_PAUSE				( WM_USER+10 )
#define WM_SELMARK_END			( WM_USER+11 )
#define WM_FASTMARK_END			( WM_USER+12 )
#define WM_DISPLAY_INFOR			( WM_USER+13 )
#define WM_OUTPUT_MARK_MSG		( WM_USER+14 )
#define WM_DISPLAY_CARD_STATUS	( WM_USER+15 )
#define ID_SORT_OBJECT_LIST		( WM_USER+20 )
#define ID_ASORT_OBJECT_LIST		( WM_USER+21 )
#define ID_G_OBJECT_LIST			( WM_USER+22 )
#define ID_SEL_OBJECT_LIST			( WM_USER+23 )
#define WM_FASTMARK_PRO			( WM_USER+24 )
#define WM_PREVIEW_END			( WM_USER+50 )

//
#define WM_SETDLG_TYPE			( WM_USER+200 )

//绘图模式
#define DRAWMODEL_NONE					0	  //放大、缩小用
#define DRAWMODEL_DRAG					1     //节点编辑
#define DRAWMODEL_HAND					2     //手绘模式
#define DRAWMODEL_SPLINE					3     //样条曲线
#define DRAWMODEL_ARC_3PT				4     //三点圆弧
#define DRAWMODEL_ARC_CENTER			5     //(圆心+两点)圆弧
#define DRAWMODEL_CIRCLE					6     //圆
#define DRAWMODEL_RECTANGLE			7     //矩形
#define DRAWMODEL_ELLIPSE					8     //椭圆
#define DRAWMODEL_POLYGON				9     //多边形
#define DRAWMODEL_GRID						10    //网格
#define DRAWMODEL_TEXT					11    //文本
#define DRAWMODEL_CODEBAR				12    //条码
#define DRAWMODEL_DOT						13    //点
#define DRAWMODEL_SORT_OBJECT_LIST      16    //手动排序
#define DRAWMODEL_ASORT_OBJECT_LIST     17    //自动排序
#define DRAWMODEL_OBJ_EDIT              18    //对象编辑
#define DRAWMODEL_CLICK					20    //选择挑选
#define DRAWMODEL_CLICKFRAME			21    //边框方式缩放
#define DRAWMODEL_DISTORTION			22    //旋转/切变


//条码窄条宽度
#define basenar								0.191
#define UPDATE_TRACKER_VIEW				1
#define UPDATE_TRACKER_PROPERTY		2
#define	UPDATE_LOGIC						3	
#define UPDATE_DEVICE						4
#define	UPDATE_QUAD							5

#define OFFSET_TEXTID(nBASE,nTextID)	( nBASE + nTextID )

//定义填充方式
#define FILL_HOR			0	
#define FILL_VER			1

//定义多边形样式
#define POLYGON_LINE		0
#define POLYGON_STAR	1
#define POLYGON_CROSS	2

//定义文本方式
#define TEXT_FIX			0
#define TEXT_SERIAL		1
#define TEXT_DATE			2

//定义排布方式
#define PATH_LINE			0
#define PATH_ROUND		1

//定义对齐方式
#define ALIGN_LEFT			0
#define ALIGN_RIGHT		1
#define ALIGN_MID			2

//定义字体
#define FONT_TTF			0
#define FONT_SHX			1
#define FONT_USER			2
#define FONT_MATRIX		3
//
// 对齐方式
enum ENU_ALIGNTYPE
{
	CmdEqualWidth,
	CmdEqualHeight,
	CmdEqualWh,
	CmdDistanceX,
	CmdDistanceY,
	CmdAlignLeft,
	CmdAlignRight,
	CmdAlignTop,
	CmdAlignBottom,
	CmdAlignX,
	CmdAlignY
};

enum ENU_HLMARKMODE
{
	ENU_MARKNORMALL,
	ENU_MARKSPECPARASET,
	ENU_MARKSEL
};


//卡类型
enum CARD_TYPE
{
	NOCARD  = -1,
	PCI3000 = 0,
	DCP1000 = 1,
	PM5000  = 2,
	EMCC    = 3
};

enum ENU_HLDATAMODE
{
	ENU_MARK = 0,
	ENU_RED,  
	ENU_FRAME, 
	ENU_DOWNSINGLE,
	ENU_DOWNMULDOC
};

//错误码
enum HL_ERRORCODE
{
	HLOK    = 0,
	HLERROR = 1,
	HLEND
};

//数据点结构体
typedef struct DOT6
{
	double x;
	double y;
	float  z;
	float  i;
	float  b1;
	float  b2;
	
	DOT6()
	{
		::memset(this, 0, sizeof(DOT6));
	};
	
	void operator = ( const DOT6 & src )
	{
		if ( &src == this )	
		{
			return;
		}
		::memcpy(this, &src, sizeof(DOT6));
	}
} DOT6;

typedef struct BMPSPEC
{
	WORD   nVer;
	
	int		nDotSteps;	//单点跳转步数
	double	dbStepLen;	//单步跳转长度(mm)
	double	dbQFreq;	//打标Q频(KHZ)
}BMPSPEC;

typedef	struct tagLISTPAUSEPARA
{
	UINT	nMark;
	UINT	nSts;
	BOOL bLevel;
	BOOL bPCOnly;
} LISTPAUSEPARA;

// 动态运动打标参数
struct SMoveMarkParam
{
	UINT m_nMode;		// 0平面剖分打标1旋转打标
	double m_dbDivDx;	// x振镜有效移动范围
	double m_dbDivDy;	// y振镜有效移动范围

	double m_dbWheelDiameter;	// 旋转打标工件直径
	double m_dbDivAngle;			// 打标角度
	double m_dbExcursionX;		// x偏移
	double m_dbExcursionY;		// y偏移
	double m_dbPrecisionK;			// 编码器反馈精度 (平台用)
	int	   m_nEncodePPR;			// 编码器PPR (旋转)
	int	   m_nMotorDelay;	
	SMoveMarkParam()
	{
		m_nMode = 0;
		m_dbDivDx = m_dbDivDy = 10;

		m_dbWheelDiameter = 80;
		m_dbDivAngle = 1;
		m_dbPrecisionK = 100;
		m_nEncodePPR = 2000;
		m_dbExcursionX = m_dbExcursionY = 0;
		m_nMotorDelay = 100;
	}
};

//仅包含编辑软件用参数
typedef struct tagSysCfg
{	
	tagSysCfg()
	{	
		memset(this, 0, sizeof(tagSysCfg));
	};
	
	double	m_dbDotMoveStepLen;	//点精确移动步长
    double	m_dbDotFilterPrecis;		//滤点精确
    double	m_dbDotFilterAngle;		//滤点角度
	double	m_dbMicro;					//微调
	double	m_dbClick;					//拾取精度
	double	m_dbOneToTwo;			//断点精度
	int			m_nUndoCount;			//回滚次数
	double	m_dbExtendSpace;		//缩扩间距
	double	m_dbKillAngleSpace;		//导角-定点交叉点之间的间距
	double	m_dbGraphImportPrics;	//图形导入精度
	double	m_dbDrawEllipSet;		//画圆精度
	int			m_nGridCol;					//网格行
    int			m_nGridRow;				//网格列
	int			m_nPolygon;				//网格行
    int			m_nPolyNum;				//网格列
	int			m_nPolyCross;				//网格行
    int			m_nPolyStar;				//网格列
	int			m_nLayerCnt;				//层数量
    int			m_nSafeDoorRemark;	//安全门闭合继续打标
	double	m_dbPreViewZoomScale;//预览鼠标滑动比例
	TCHAR	m_szCardName[20];
	
	double	m_dbDiameter;				//圆周直径
	double	m_dbDivAngle;				//切分角度
	int			m_nFillNum;					//一次打标填充线次数

	// 动态打标
	SMoveMarkParam  m_sMoveMarkParam;
	
	BOOL		m_bCloseRedIfMark;			//打标时是否关红光
	double	m_dbJoinDistance;			//曲线合并时的距离限制
	int			m_nRedPreviewLayerNo;	//红光预览时对应的层号
	double   m_dbZOffset;					//Z Offset
	double   m_db3dModelPrecision;		//3d模型精度
	double   m_db3dTrancePrecision;		//3d数据贴图精度
	double   m_dbPrecision;					//圆弧精度
	double   m_dbDotmatrixCirPrec;
	BOOL	 m_n3DOptimize;
	int			m_nSampleCount;				//圆弧分段数

	BOOL		m_bFillOptimize;				//是否对填充进行优化
	double	m_dbInterSpace;				// 交叉滤点长度
	double  m_dbLineWidth;
	double  m_dbBroadenD;
	int			m_nCatchPrecision;			// 捕获点精度,取值范围只能是1-5mm,精度越小捕获的越精确
} SYSCFG;


typedef struct objPos
{
	double x;
	double y;
	double z;
	CDot   Lf;
	CDot   Rf;
	
	objPos()
	{
		memset(this, 0, sizeof(objPos));
	};
	
	void operator = ( const objPos & src )
	{
		if ( &src == this )	
			return;
		
		memcpy(this, &src, sizeof(objPos));
	}
	
} OBJPOS;


typedef struct ArrayMark
{
	QUADSG qd;
	CDot   Lf;
	CDot   Rf;
	double rota;
	
	ArrayMark()
	{
		memset(this, 0, sizeof(ArrayMark));
	};
	
	void operator = ( const ArrayMark & src )
	{
		if ( &src == this )	
			return;

		memcpy(this, &src, sizeof(ArrayMark));
	}
}ARRAYMARK;



typedef struct tagADrawNode
{
	tagADrawNode()
	{
	}
	
	~tagADrawNode()
	{
	}
	
	void operator = ( const tagADrawNode & src )
	{
		if ( &src == this )	
			return;
		///////////
		m_SDt   = src.m_SDt;
		m_EDt   = src.m_EDt;
	}

public:	
	CDot    m_SDt;       // 标示范围起点
	CDot    m_EDt;       // 标示范围终点
} ADRAWNODE;

#endif //__24DA3DE5_CBCC_492C_81B1_F65E74483348_