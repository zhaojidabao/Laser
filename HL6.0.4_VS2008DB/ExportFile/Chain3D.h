#pragma once

#include <afxtempl.h>

struct LABEL_DOT {
	double x;
	double y;
	double z;
};

typedef CArray<LABEL_DOT,LABEL_DOT>	LABEL_STROKE;//2D线条
typedef CArray<LABEL_STROKE*,LABEL_STROKE*>	LABEL_LIST;//线条链表

typedef struct	//3D线条数据
{	
	int			iLayer;
	LABEL_STROKE  dotArray;
}LABEL_3DSTROKE;
typedef CArray<LABEL_3DSTROKE*,LABEL_3DSTROKE*>	LABEL_3DLIST;//取贴图后的3D线条链表

typedef struct
{
	LABEL_LIST list;
	int			iLayer;
	BOOL		bSelected;
	int			nMarkNum;
}LABEL_NODE;
typedef CArray<LABEL_NODE*,LABEL_NODE*>LABEL_DATA;


struct POS_DOT {
	double x;
	double y;
	double z;
};

//台阶
typedef struct 
{
	CArray<POS_DOT, POS_DOT> pos;
	CArray<double, double> length;
	CArray<double, double> width;
}STEP, *PSTEP; 

//斜面
typedef struct 
{
	POS_DOT p0;
	POS_DOT p1;
	double	dbLong;
	int			nDirect;	//0:绕Y轴线 1:绕X轴线
}INCLINE, *PINCLINE;

//圆柱(半)
typedef struct 
{
	double dbRadius;
	double dbLong;
	double dbSR;	//起始角度
	double dbER;	//终止角度
}COLUMN, *PCOLUMN;

//球(半)
typedef struct 
{
	double dbRadius;
	BOOL	  bInside;
}SPHERE, *PSPHERE;

//圆锥
typedef struct
{
	double dbLong;
	double dbRadius0;
	double dbRadius1;
	double dbSR;	//起始角度
	double dbER;	//终止角度
	BOOL	 bInside;
	int		 nDirect;	//0:横放 1:竖放
}TAPER, *PTAPER;

//边框拉伸或旋转生成的模型
typedef struct
{
	int m_nBuildType;	// 0 边框拉伸 1 边框旋转
	LABEL_LIST m_arrEdge;
	LABEL_DOT  m_dotOrg;
	LABEL_DOT  m_dotVec;
}ManulModel, *PManulModel;

typedef struct tagTRANSFORM
{
	tagTRANSFORM(){
		memset(this, 0, sizeof(tagTRANSFORM));
	}
	BOOL		bRelative;
	double	dx; 
	double	dy; 
	double	dz; 
	double	dbAngle; 
	int			nDirect;   /*0:绕Z 1:X 2:Y*/
}TRANSFORM;

typedef	struct tagHL_3DPARA
{
	void Clear()
	{
		iModule=0;
		nShape=0;
		strFile.Empty();
		step.pos.RemoveAll();
		step.length.RemoveAll();
		step.width.RemoveAll();
		memset(&incline, 0, sizeof(INCLINE));
		memset(&column, 0, sizeof(COLUMN));
		memset(&sphere, 0, sizeof(SPHERE));
		memset(&taper, 0, sizeof(TAPER));
		///
		for(int i = 0; i<manulModel.m_arrEdge.GetCount(); i++)
		{
			if( manulModel.m_arrEdge[i] )
			{
				delete ((LABEL_STROKE*)manulModel.m_arrEdge[i]);
				manulModel.m_arrEdge[i] = NULL;
			}
		}
		manulModel.m_arrEdge.RemoveAll();

		manulModel.m_dotOrg.x = 0;
		manulModel.m_dotOrg.y = 0;
		manulModel.m_dotOrg.z = 0;

		manulModel.m_dotVec.x = 0;
		manulModel.m_dotVec.y = 0;
		manulModel.m_dotVec.z = 0;

	}

	tagHL_3DPARA(){
		Clear();
	}

	void Copy(const tagHL_3DPARA& src)
	{
		Clear();
		////////
		iModule	=	src.iModule;
		strFile	=	src.strFile;
		nShape	=	src.nShape;
		step.pos.Copy(src.step.pos);
		step.length.Copy(src.step.length);
		step.width.Copy(src.step.width);
		memcpy(&incline, &src.incline, sizeof(INCLINE));
		memcpy(&column, &src.column, sizeof(COLUMN));
		memcpy(&sphere, &src.sphere, sizeof(SPHERE));
		memcpy(&taper, &src.taper, sizeof(TAPER));

		//
		LABEL_STROKE* pStroke = NULL;
		manulModel.m_nBuildType = src.manulModel.m_nBuildType;
		for(int i = 0; i<src.manulModel.m_arrEdge.GetCount(); i++)
		{
			if( src.manulModel.m_arrEdge[i] )
			{
				pStroke = new LABEL_STROKE;
				pStroke->Append(*(src.manulModel.m_arrEdge[i]));
				manulModel.m_arrEdge.Add(pStroke);
			}
		}

		manulModel.m_dotOrg.x = src.manulModel.m_dotOrg.x;
		manulModel.m_dotOrg.y = src.manulModel.m_dotOrg.y;
		manulModel.m_dotOrg.z = src.manulModel.m_dotOrg.z;

		manulModel.m_dotVec.x = src.manulModel.m_dotVec.x;
		manulModel.m_dotVec.y = src.manulModel.m_dotVec.y;
		manulModel.m_dotVec.z = src.manulModel.m_dotVec.z;
	}

	int			iModule;
	CString	strFile;
	int			nShape;
	STEP		step;
	INCLINE	incline;
	COLUMN column;
	SPHERE	sphere;
	TAPER	taper;
	ManulModel manulModel;

}HL_3DPARA;

typedef UINT (WINAPI *T_HS_MarkStroke)(UINT nType, LPVOID pInfo, int nNum, UINT nLayerIndex);

class AFX_EXT_CLASS CChain3D
{
public:
	HL_3DPARA m_3dpara;

public:	
	CChain3D(void);
	~CChain3D(void);

	void	Init(double dbRange,	int nFine);
	BOOL InitOpenGL(HWND wndDraw);
	BOOL Build(BOOL bNew);
	void Serialize(FILE* fp,BOOL bSave);	
	BOOL LabelText(const LABEL_DATA* pData, double dbStep, LABEL_DOT dotBase,BOOL bForce=FALSE);

	// 计算平均法向量
	//BOOL LabelNormal(const LABEL_DATA* pData, double dbStep, double &dx,double& dy,double& dz);
	BOOL LabelNormal(double dbLeft,double dbRight,double dbBottom,double dbTop,double dbStep, double &dx,double& dy,double& dz);
	BOOL ConvertDataByNormal(LABEL_DATA* pData,double &dx,double& dy,double& dz,LABEL_DOT dotBase);
	BOOL   FindNormal(double x,double y,double &dx,double& dy,double& dz);
	// 求向量(x,y)与x轴夹角
	double GetVectorAngle(double x,double y);

	void	ClearLabel();
	int		GetObjCount();
	void	SetSelectedObjbeMarked(BOOL bFlag);

	void	Draw();
	void	Zoom(BOOL bIn);
	void	SightV(BOOL bUp);
	void	SightH(BOOL bLeft);
	void	Drag(int cx, int cy);
	void	RawView(int nDirect/*0:斜视 1:XY 2:YZ 3:XZ*/);
	void	GetViewPos(double& dx, double& dy, double& dz, double& dbAngle, double& dbToBaseZ, BOOL& bRotateXY);
	void	ClearToBaseZ();

	BOOL OutOfLimit();
	void	Mark(HWND hWnd, T_HS_MarkStroke pMarkApi,double dbZOffset);

	void	Transform(TRANSFORM& tsf);
	void	GetCenterPos(double& x, double& y, double &z);
	void	Get3DStrokeList(LABEL_3DLIST  *p3DList);
	void	Free3DStrokeList(LABEL_3DLIST *p3DList);

	void	Delete();
	void	Select(BOOL bNext);
	void	Convert(double x, double y);

	BOOL CanUndo();
	BOOL CanRedo();
	BOOL Undo();
	BOOL Redo();

	void Cal3DBox(double &x0,double &x1,double &y0,double &y1,double &z0,double &z1);

protected:
	void	UpdatePara();
	BOOL BuildAll();
	void	ClearObject();

	void	DrawCoord();
	void	DrawPlane();
	void	DrawZmap();
	void	DrawString3D(LPCSTR str, float pos[3], float color[4]);

	void	SortObjHori(double x,double y);
	CArray<LPVOID, LPVOID> m_arrObjSort;	
	double FindZ(double& x, double& y, const LABEL_DOT& dotBase);

private:
	int			m_iSel;
	int			m_nFine;
	BOOL		m_bSelMark;
	HWND	m_hWnd;
	HGLRC	m_hRC;

	LPVOID	m_pCmdPool;

	CArray<LPVOID, LPVOID> m_arrObject;	
	CArray<LPVOID, LPVOID> m_arrDataLabel;

	struct V3D
	{
		V3D(){
			memset(this,0,sizeof(V3D));
		}
		double dx;
		double dy;
		double dz;
	};
	V3D		m_range;
	V3D		eye;

	CSize		m_view;
	double	m_cxDrag;
	double	m_cyDrag;
	double	m_dbZoom;
};
