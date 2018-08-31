#pragma once
#include "AIDecodeDefine.h"
#include "StrokeB.h"
#include "cv.h"
#include "HighGuI.h"

// CAiFillMgr 命令目标
class CAiFillMgr : public CObject
{
public:
	CAiFillMgr();
	virtual ~CAiFillMgr();
	
	//
	BOOL DeCode( const CString& file, StrokeList &listLineData );

	//生成图像数据
	void CreateImageData( int nWidth, int nHeight );
	
	//销毁图像数据
	void ReleaseImageData();
	
	//解析AI，并在位图上绘图
	BOOL DrawPath( CAIStroke* pAiStroke, double dbLeft, double dbBottom );
	
	//获取点数组在位图上的坐标
	BOOL GetPointArray( CArray<PTOUT,PTOUT> &dtArray,  CvPoint *pPointData, double dbLeft, double dbBottom );
	
	//颜色转换
	CvScalar RgbToCvcolor( COLORREF color);

	//形态学腐蚀处理
	void ErodeFillPart( int nPenWidth );

	//区域叠加计数
	void AddGroupCount();

	//进行叠加运算，完成后加入最终的位图
	void AddBmp( int nType );

	//生成填充线条
	void CreateLineList();

	//删除填充线条
	void DeleteLineList();

	//X方向扫描
	void ScanX( int nRowPos, double dbY, StrokeList &listData );

	//Y方向扫描
	void ScanY( int nColPos, double dbX, StrokeList &listData );

	//
	CAITranslate m_aiTrnslate;

	//最终的效果图
	IplImage *m_pBmp;
	
	//原始空白图
	IplImage *m_pBmpOrg;
	
	//组合阶段性图
	IplImage *m_pBmpGroup;
	
	//标记图,用以限制图像处理的范围
	IplImage *m_pBmpMark;
	
	//数据矩阵，用以记录填充叠加属性
	IplImage *m_pBmpMat;
	
	//复合路径填充规则
	int m_nXR;
	
	//路径方向
	int m_nDirection;

	//
	int m_nBmpWidth;
	int m_nBmpHeight;
	double	m_dbHeight;
	double	m_dbWidth;
	double m_dbSpaceX;
	double m_dbSpaceY;
	int m_nScanType;
	double m_dbScanDis;
	BOOL m_bLoop;	//往复
	StrokeList m_listLineData;
};


