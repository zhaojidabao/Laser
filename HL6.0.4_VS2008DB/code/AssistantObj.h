#ifndef __ASSISTANTOBJ_AB35950D_7150_4484_8F9F_2C8879CD433B_H
#define __ASSISTANTOBJ_AB35950D_7150_4484_8F9F_2C8879CD433B_H

/**********************************************************************************************
模块名				:  作图铺助点
文件名				:  AssistantObj.h
相关文件			:  AssistantObj.cpp
文件实现功能		:  提供作图铺助点(线条端点，线条中心，切线, 圆心)
说明:
	搜索精度说明:
	1								2				
	-	-	-				-	-	-	-	-
	-	|	-				-	-	|	-	-
	-	-	-				-	|	|	|	-
							-	-	|	-	-
							-	-	-	-	-
-----------------------------------------------------------------------------------------------
备注           : -
------------------------------------------------------------------------------------------------
修改记录 : 
日  期						版本				 修改人				修改内容 
2012/10/27				1.0				朱海清				 创建
************************************************************************************************/
#define Round(x)		( ((x)>0.0)?((int)(x+0.5)):((int)(x-0.5)) )

enum ASSPOINT_TYPE
{
	ASSPOINT_TYPE_ENDPOINT = 0,			// 线端点
	ASSPOINT_TYPE_LINECENTER,			// 线中心点
	ASSPOINT_TYPE_CIRCLECENTER,		// 圆心
	ASSPOINT_TYPE_CUTPOINT				// 切点
};

typedef struct AssPoint
{
	double x;				// 物理坐标x
	double y;				// 物理坐标y
	int		nType;	// 铺助点类型,取值参照ASSPOINT_TYPE
}ASSPoint;

typedef struct AnchorPoint
{
	int x;						// 逻辑坐标x
	int y;						// 逻辑坐标y
	int nType;				// 铺助点类型
	BOOL bEnable;		// 当前锚点生效，用于擦除用,取值参照ASSPOINT_TYPE
}ANCHORPOINT;

//对像使用方法,声明对象-》SetAssistantPara(必须)-》LoadAssistantPoints加载铺助点
class CAssistantObj 
{
public:
	CAssistantObj();
	~CAssistantObj();
	void	  SetPoint(double x, double y, int nType);
	void    SetPointsFromStroke( CStroke* pStroke);
	BOOL LookUp(double x, double y, double& dbx, double& dby);
	BOOL	SetAssistantPara(UINT nGridWidth, UINT nGridHeight, int nPrecision);
	BOOL	LoadAssistantPoints(const CArray<DOT,DOT> &pointArray);
	void DrawAnchor(CDC* pDC);

private:
	BOOL InitAssistant();			// 分配格子内存
	void	FreeAssistant();			// 释放空间

public:
	unsigned int	**m_pGridData;

	CPen pen;  
	AnchorPoint	m_anchorPoint;

private:
	UINT m_nGridWidth;	// 铺助网格的宽度
	UINT m_nGridHeight;	// 铺助网格的高度
	int m_nPrecision;	// 铺助精度，搜周边的m_nPrecision*m_nPrecision格子只能取1,2,3,4,5mm
};

#endif