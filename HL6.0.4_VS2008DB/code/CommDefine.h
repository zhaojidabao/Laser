#ifndef __24DA3DE5_CBCC_492C_81B1_F65E74483348_
#define __24DA3DE5_CBCC_492C_81B1_F65E74483348_

#include "..\inc\baseStruct.h"
#include "..\inc\baseInterface.h"

#define LAYERCOUNT		256

//�����̣߳���ʼ�����
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

//��ͼģʽ
#define DRAWMODEL_NONE					0	  //�Ŵ���С��
#define DRAWMODEL_DRAG					1     //�ڵ�༭
#define DRAWMODEL_HAND					2     //�ֻ�ģʽ
#define DRAWMODEL_SPLINE					3     //��������
#define DRAWMODEL_ARC_3PT				4     //����Բ��
#define DRAWMODEL_ARC_CENTER			5     //(Բ��+����)Բ��
#define DRAWMODEL_CIRCLE					6     //Բ
#define DRAWMODEL_RECTANGLE			7     //����
#define DRAWMODEL_ELLIPSE					8     //��Բ
#define DRAWMODEL_POLYGON				9     //�����
#define DRAWMODEL_GRID						10    //����
#define DRAWMODEL_TEXT					11    //�ı�
#define DRAWMODEL_CODEBAR				12    //����
#define DRAWMODEL_DOT						13    //��
#define DRAWMODEL_SORT_OBJECT_LIST      16    //�ֶ�����
#define DRAWMODEL_ASORT_OBJECT_LIST     17    //�Զ�����
#define DRAWMODEL_OBJ_EDIT              18    //����༭
#define DRAWMODEL_CLICK					20    //ѡ����ѡ
#define DRAWMODEL_CLICKFRAME			21    //�߿�ʽ����
#define DRAWMODEL_DISTORTION			22    //��ת/�б�


//����խ�����
#define basenar								0.191
#define UPDATE_TRACKER_VIEW				1
#define UPDATE_TRACKER_PROPERTY		2
#define	UPDATE_LOGIC						3	
#define UPDATE_DEVICE						4
#define	UPDATE_QUAD							5

#define OFFSET_TEXTID(nBASE,nTextID)	( nBASE + nTextID )

//������䷽ʽ
#define FILL_HOR			0	
#define FILL_VER			1

//����������ʽ
#define POLYGON_LINE		0
#define POLYGON_STAR	1
#define POLYGON_CROSS	2

//�����ı���ʽ
#define TEXT_FIX			0
#define TEXT_SERIAL		1
#define TEXT_DATE			2

//�����Ų���ʽ
#define PATH_LINE			0
#define PATH_ROUND		1

//������뷽ʽ
#define ALIGN_LEFT			0
#define ALIGN_RIGHT		1
#define ALIGN_MID			2

//��������
#define FONT_TTF			0
#define FONT_SHX			1
#define FONT_USER			2
#define FONT_MATRIX		3
//
// ���뷽ʽ
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


//������
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

//������
enum HL_ERRORCODE
{
	HLOK    = 0,
	HLERROR = 1,
	HLEND
};

//���ݵ�ṹ��
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
	
	int		nDotSteps;	//������ת����
	double	dbStepLen;	//������ת����(mm)
	double	dbQFreq;	//���QƵ(KHZ)
}BMPSPEC;

typedef	struct tagLISTPAUSEPARA
{
	UINT	nMark;
	UINT	nSts;
	BOOL bLevel;
	BOOL bPCOnly;
} LISTPAUSEPARA;

// ��̬�˶�������
struct SMoveMarkParam
{
	UINT m_nMode;		// 0ƽ���ʷִ��1��ת���
	double m_dbDivDx;	// x����Ч�ƶ���Χ
	double m_dbDivDy;	// y����Ч�ƶ���Χ

	double m_dbWheelDiameter;	// ��ת��깤��ֱ��
	double m_dbDivAngle;			// ���Ƕ�
	double m_dbExcursionX;		// xƫ��
	double m_dbExcursionY;		// yƫ��
	double m_dbPrecisionK;			// �������������� (ƽ̨��)
	int	   m_nEncodePPR;			// ������PPR (��ת)
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

//�������༭����ò���
typedef struct tagSysCfg
{	
	tagSysCfg()
	{	
		memset(this, 0, sizeof(tagSysCfg));
	};
	
	double	m_dbDotMoveStepLen;	//�㾫ȷ�ƶ�����
    double	m_dbDotFilterPrecis;		//�˵㾫ȷ
    double	m_dbDotFilterAngle;		//�˵�Ƕ�
	double	m_dbMicro;					//΢��
	double	m_dbClick;					//ʰȡ����
	double	m_dbOneToTwo;			//�ϵ㾫��
	int			m_nUndoCount;			//�ع�����
	double	m_dbExtendSpace;		//�������
	double	m_dbKillAngleSpace;		//����-���㽻���֮��ļ��
	double	m_dbGraphImportPrics;	//ͼ�ε��뾫��
	double	m_dbDrawEllipSet;		//��Բ����
	int			m_nGridCol;					//������
    int			m_nGridRow;				//������
	int			m_nPolygon;				//������
    int			m_nPolyNum;				//������
	int			m_nPolyCross;				//������
    int			m_nPolyStar;				//������
	int			m_nLayerCnt;				//������
    int			m_nSafeDoorRemark;	//��ȫ�űպϼ������
	double	m_dbPreViewZoomScale;//Ԥ����껬������
	TCHAR	m_szCardName[20];
	
	double	m_dbDiameter;				//Բ��ֱ��
	double	m_dbDivAngle;				//�зֽǶ�
	int			m_nFillNum;					//һ�δ������ߴ���

	// ��̬���
	SMoveMarkParam  m_sMoveMarkParam;
	
	BOOL		m_bCloseRedIfMark;			//���ʱ�Ƿ�غ��
	double	m_dbJoinDistance;			//���ߺϲ�ʱ�ľ�������
	int			m_nRedPreviewLayerNo;	//���Ԥ��ʱ��Ӧ�Ĳ��
	double   m_dbZOffset;					//Z Offset
	double   m_db3dModelPrecision;		//3dģ�;���
	double   m_db3dTrancePrecision;		//3d������ͼ����
	double   m_dbPrecision;					//Բ������
	double   m_dbDotmatrixCirPrec;
	BOOL	 m_n3DOptimize;
	int			m_nSampleCount;				//Բ���ֶ���

	BOOL		m_bFillOptimize;				//�Ƿ���������Ż�
	double	m_dbInterSpace;				// �����˵㳤��
	double  m_dbLineWidth;
	double  m_dbBroadenD;
	int			m_nCatchPrecision;			// ����㾫��,ȡֵ��Χֻ����1-5mm,����ԽС�����Խ��ȷ
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
	CDot    m_SDt;       // ��ʾ��Χ���
	CDot    m_EDt;       // ��ʾ��Χ�յ�
} ADRAWNODE;

#endif //__24DA3DE5_CBCC_492C_81B1_F65E74483348_