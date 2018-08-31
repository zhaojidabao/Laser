// NodeCodebar.h: interface for the CNodeCodebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_)
#define AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Quad.h"
#include "XDC.h"
#include "NodeT.h"

#include <vector>
#include <complex>
#include <list>
using namespace std;

class CNodeCodebar : public CNodeT  
{
	DECLARE_SERIAL(CNodeCodebar);

public:
	CNodeCodebar();
	virtual ~CNodeCodebar();
	virtual void Draw(XDC *pDC);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeCodebar();
		return pNode;
	};

public:
	virtual BOOL PreprocessMark(DWORD nStyle);
	CQuad GetMarkRange();
	
	virtual BOOL Create(CRect &rect);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);

	virtual void Rotate(CDot dot,double angle,int nPlane = 0);
	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY){return TRUE;};
	virtual BOOL Move(double moveX,double moveY,double mvoeZ = 0);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual void Drag(CDot dot, double ratioX, double ratioY,double moveX,double moveY,double moveZ = 0);

	virtual BOOL CreateStrokeList();
	virtual CString GetParitybit(CString markcode){return "";};//得到校验位

	virtual void DrawExtern(XDC *pDC);
	virtual BOOL GetLockFlg();
	virtual void LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	
public:
	virtual void RefreshBarText( CWnd* pWnd, int nTextID );
	BOOL    GenBarCode(UINT nStyle);

	BARTYPE m_BarType;
	
	double	m_dbFillSpace;		//填充间隔
	BOOL		m_bRev;				//确定条码是否反填
    BOOL		m_bGoBack;		//块往复
    BOOL		m_bConnect;		//块连接
	double	m_dbRevWidth;	//反相条码时的宽度
	double	m_dbHeightRev;	//反相条码时的高度(一维)
	double	m_dbAngle;			//条码角度
	BOOL		m_bAutoFill;		//自动填充
	BOOL		m_bMatrix;			//条码点阵化
	BOOL		m_bParity;			//是否启用校验码

	DWORD	m_nSingleLineMarkCount;
	UINT		m_nNarrowLineNum;

	BYTE*	m_pByte;			//条码二进制信息
	UINT		m_nCol;				//条码行数
	UINT		m_nRow;			//条码列数
	
	double	m_dbDotRadius;	//点大小
	double	m_dbshrink;			//条码缩进量
	double	m_dbHeightBar;	//条码高度
	int			m_nOutline;			//PDF417条码行数
	int			m_nErrLev;			//纠错级别，用于QR,DATAMATRIX
	int			m_nHPoint;			//垂直方向点个数,用于QR,DATAMATRIX
	int			m_nWPoint;			//水平方向点个数,用于DATAMATRIX
	int			m_nSymbolNo;		//DATAMATRIX 编码
	int			m_nShapeNo;		//DATAMARTIX 形状
	int			m_nVersion;		//版本

	int			m_nCodeType;
	BOOL		m_bAutoSet;		//设置条码长宽属性;//true 自动设置;FALSE 用户设置
	int			m_nAlign;			//条码与字符号 1左对齐;2中间对齐;3右对齐
	double	m_narrowW;		//用于条码拉变时,记录当前条码窄条宽度

	// 镂空参数
	BOOL m_bHollow;
	double m_dbHollowWidth;
	double m_dbHollowHeight;

	UINT m_nCodePage;
	
	CDot		m_dotBase;			//条码基准点
	CArray<StrokeList*, StrokeList*> m_CodeBarList;
	
	double	m_dbRatioX;
	double	m_dbRatioY;
	CDot		m_dotDragPt;
	
	BOOL		m_bTextBar;        //是否存在关联文本对象
	
private:
	BOOL  CreateStrokeList3();
	BOOL  CreatePDFStrokeList();
	BOOL  CreateStrokeList2();
	BOOL  CreateStrokeList1();
	void	 InitCodebar();
	
	BOOL   AddToList();
	void	  FillStroke(list<CStroke*> * pList,CStroke *pStroke);
	
	BOOL   ClearBarList();
	BOOL   BetterOneCodeBar();
	BOOL   ConnectOneCodeBar();
	double Distance(CDot &sDot, CDot &eDot);
	BOOL   ProOneStroke(CDot &dt, CStroke *pStrk);
	BOOL   MinDsIsFirstDot(CDot &dt, CStroke *pStrk);

	double m_dbSpace;		//对于EAN8和EAN13码来说会存在长短之分
									//长比短的长多少即为此参数,目前末做序列化操作
									//现用常量来代替，沿用原来的值（0.6MM）
	double m_dbcodeRate;	//此值固定为2.75

	//
	BOOL MakeLineToList(StrokeList *pStrkList, double x, double y0, double y1);
	BOOL MakeLineList(double x, double y0, double y1, int nLineCnt, double dbGoStep);
	BOOL MakeLineList(int nType, double x, int nLineCnt, double dbGoStep, BOOL bLong = FALSE);
	/*nType 0:实际输出, 1:边框反向, 2:边框输出 */
};

#endif // !defined(AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_)

