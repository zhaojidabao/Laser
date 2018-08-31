// NodeText.h: interface for the CNodeText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_)
#define AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeT.h"
#include "Dot.h"
#include "Fnt.h"
#include "Tree.h"
#include "Quad.h"
#include "NodeT.h"

#include <vector>
#include <complex>
#include <list>

class CNodeText : public CNodeT
{	
	DECLARE_SERIAL(CNodeText);

public:
	virtual void  AddToPlt(CNode *pNodePlt);
	virtual BOOL Click(CPoint &point,CWnd *pWnd);
	virtual void SetProperty(int mode,CProperty pty, XDC *pDc = NULL);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual BOOL Move(double moveX,double moveY,double moveZ = 0);
	virtual void	 Rotate(CDot dot, double angle, int nPlane = 0);
	virtual void	 Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY,double moveZ = 0);
	virtual BOOL PreprocessMark(DWORD nStyle);
	virtual UINT	 Mark(HWND hWnd);
	virtual UINT SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark = FALSE);
	virtual CQuad CalLimit();
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Draw(XDC *pDC);
	virtual BOOL CopyToMe(CNode *pSrc,DWORD dwStyle);//复制全部数据，如果dwStyle = TRUE重新建立ID
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	virtual BOOL Create(CRect &rect);
	virtual void Scan(StrokeList *pList);		
	virtual BOOL CanGroup();
	virtual CNode *CopyStrokeOnly();
	virtual CQuad  GetMarkRange();
	virtual void DrawExtern(XDC *pDC);
	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual void RefreshBarText(CWnd* pWnd,int nTextID);
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill);
	virtual void SetPathOptimizeState(BOOL bState);
	//bRef为TRUE，只取打标数据范围(不包括如圆弧排布中的圆弧),FALSE取包括圆弧的范围
	virtual  CQuad	 GetLimit(BOOL bRef = FALSE);
	//
	BOOL GetFrameDots(CArray<CDot,CDot>& frameDots);
	void FillStroke(list<CStroke*> * pList,CStroke *pStroke);
	
	CNodeText();	
	virtual ~CNodeText();
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeText();
		return pNode;
	};
	
public:
	virtual void SetStrokeList(StrokeList *pStrokeList);
	virtual void LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	virtual void LoadWheelList(int nType, StrokeList *pListStroke);
	virtual BOOL GetLockFlg();
	virtual void  Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);

	CDot		m_dotBase;
	CDot		m_dotLT;
	CDot		m_dotRB;

	//跳号文本
	int		m_nAlign;		//对齐方式
	int		m_nPath;		//排列方式，如弧形，直线
	double	m_dbDirection;	//直线方式下的方向
	double	m_dbRadius;		//弧形方式下的半径
	BOOL	m_bCCW;			//弧形方式下的时针方向
	double	m_dbStart;		//弧形方式下的开始角度
	BOOL	m_bInside;		//弧形方式下的字符朝内
	BOOL	m_bPrintInverse;//印章刻印
	
	BOOL		m_bCustomFormat;	//是否输出指定文本内容
	CString	m_strFormat;		//输出指定文本内容的格式
	CString	 m_strPrecisK;		//字体精度因子
	
	double	m_dbCharSpace;	//字间距
	double	m_dbWidthFactor;//宽度系数
	double	m_dbHeight;		//字高度
	double	m_dbLineSpace;	//行间距
	BOOL		m_bFixSpace;//等间距字符启用
	double	m_dbFixSpace;	//等间距字符

	// add by zhq,增加字体自动焊接和过滤交点选项
	BOOL		m_bWeld;
	BOOL		m_bInterFilter;
	double	m_dbInterSpace;

	// 自动加粗
	BOOL m_bBroaden;
	double m_dbLineWidth;
	double m_dbBroadenD;

	int			m_nFont;			//字体方式
	CString	m_strFontCHS;	//SHX中文字体
	CString	m_strFontEN;		//SHX英文字体
	CString	m_strFontNUM;	//SHX数字字体
	LOGFONT m_lf;				//TTF字体
	CString	m_strFontUser;	//自定义字体
	CString	m_strHzkType;		//当前选中的点阵字库类型
	CString	m_strAscType;	//当前选中的点阵英文字库
	
	CFnt   *	m_pFnt;
	//
	int			m_nBarType;;		//条码类型
	BOOL		m_bParity;			//条码是否加校验
	BOOL		m_bBarText;		//是否为条码文本
	CString	m_strBarString;	//条码内容
	int			m_nErrLev;			//纠错级别
	int			m_nSize;			//条码大小
	int			m_nCodeType;		//条码编码方式

	//
	double m_dbMatrixR;
	double m_dbMatrixFillD;
	BOOL  ProcessMatrixFont(StrokeList& listStroke);

	BOOL  CreateString(BOOL bNext);
	void	 CreateFnt();
	void	 CaseNum(CString &str);	//将所有的除数字以外的信息去除
	BOOL  GenBarString();				//格式文本
	
public:
	void	CreateFromBar(UINT nText);
	void	CreateFromBar(CUIntArray& ArrayTextID);
	void	CopyProperty(CNodeText* pSrc);
	
	BOOL	 GetNodeFirstDot(CDot &dt);
	BOOL	 GetNodeLastDot(CDot &dt);
	BOOL	 AdvanceSet(CRect &rect);

private:
	CQuadRangle m_quadChar;	

	void FormatString(CString& strText);
	void CalcRatio(CQuadRangle q1, CQuadRangle q2, double& rx, double& ry);
};

#endif // !defined(AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_)
