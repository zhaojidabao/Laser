// StrokeB.h: interface for the CStrokeB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STROKEB_H__66C34014_40DD_40D9_B446_4BD0DD2BF8AD__INCLUDED_)
#define AFX_STROKEB_H__66C34014_40DD_40D9_B446_4BD0DD2BF8AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Dot.h"
#include "QuadB.h"
#include "XDC.h"
#include "Property.h"

class   CStrokeB;
typedef CTypedPtrList<CObList,CStrokeB *> StrokeList;
typedef CArray<CDot,CDot> DotList;

class HotDot
{
public:
	int  m_iStrokeIndex;    //所属线条索引
	CDot m_CrossDot;	    //交点信息
	BOOL m_bOutLine;		//是否为外轮廓
	void operator =(const HotDot& src)
	{
		m_iStrokeIndex=src.m_iStrokeIndex;
		m_CrossDot=src.m_CrossDot;
		m_bOutLine=src.m_bOutLine;
	}
};

//DOTEX结构体主要用于进行缩过处理
typedef struct
{
	double x;
	double y;
	UINT nFlag;		//做为标志做用
}DOTEX;

typedef CArray<DOTEX,DOTEX> DotExList;

class AFX_EXT_CLASS CStrokeB : public CObject  
{
public:
	CStrokeB();
	virtual ~CStrokeB();
	CStrokeB(CProperty pty);	
	
public:
	void SetProperty(int mode,CProperty &pty);
	void SetLayColor(int nLayer,COLORREF color);
	long IsInDots();
	long GetOptLayer();
	void TranslateDot(double &x,double &y,double a,double x0,double y0);
	void SetFlag(int bflag);
	void SetParent(LPVOID *pParent);

	void SetStyle(int nStyle);
	int  GetStyle();
	BOOL IsFillStyle();

	void SetLockFlag(BOOL bFlag);
	BOOL GetLockFlag();
	double Distance(CDot &sDot, CDot &eDot);
	UINT CalCrossEx(CDot aDot,CDot bDot,CDot cDot,CDot dDot,CDot& dotCross,
			CDot& dotCross2, UINT& nL1Dir,UINT& nL2Dir);
	//计算矢量a->b的角度(-pi,pi]
	double CalLineAngle(CDot aDot,CDot bDot);
	double GetAngle(CDot &p1, CDot &p2, CDot &p3);

	BOOL CalCross(CDot aDot,CDot bDot,CDot cDot,CDot dDot,CDot& dotCross,BOOL& bValid);
	double CalDistance(CDot dot1,CDot dot2);
	BOOL CalLine(CDot aDot,CDot bDot,double& k,double& b,UINT& nLType);
	void TransCoord(double x0,double y0,double angle,BOOL bRev,double& x,double &y);
	void DrawBmp(CDC *pDC);
	
	int  GetMarkUnit();
	void TrueToLog();
	CDot GetAt(int index);

	CDot CalCrossPoint(CDot dotS,CDot dotE,double scan);
	void ScanFill(DotList* listInfo, double scan,double str,double end);
	
	void Add(CDot dot);
	void Scale(CDot dot,double ratioX,double ratioY);
	void Rotate(CDot dot,double angle);
	void Rotate(CDot dot, int nPlane,double angle);
	void Move(double moveX,double moveY);
	void Move(double moveX,double moveY, double moveZ);
	void Effect(double r,double start,BOOL bCCW,BOOL bInside,CQuadB quad);
	void Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual void Serialize(FILE* fp, BOOL bFlag, BOOL bUnicode);

	int	 GetSize();
	void AddHead(CDot dot);
	void DrawExtern(XDC *pDC);

public:
	BOOL ISClockWise(); 
	void Excursion(CStrokeB* pStroke,double dbOffset);
	BOOL IsRegion();
	void MoveDot(int nNum,double MoveX,double MoveY);
	CDot GetDot(CDot &dot1,CDot &dot2,double dbPoint);
	void RemoveDot(int nIndex);
	BOOL IsEmpty();
	BOOL DotInQuad(CQuadB &quad);
	void ReplaceDot();
	BOOL HasDot(CQuadB &quad);
	void LogToTrue();
	void SetDot(unsigned int index,CDot dot);
	void Append(const CStrokeB &stroke);
	void RemoveAllPoint();
	void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScale100);
	LPVOID GetParent(){return m_pParent;};
	BOOL DotIsInStroke(CDot &dot);
	void SetDotToStart(CDot &dt);
	void FilterPoint(double dbPresic, double dbAngle);
	void Reverse();
	void SetStrokeReverse(CDot &dt);
	BOOL KillAngle(int nNum, double dbSpace);
	double GetLength();
	virtual CStrokeB* Copy();
	
public:
	BOOL CalArcJoint(CDot aDot,CDot bDot,CDot cDot,double dbPrecision,CStrokeB* pRtnStroke);
	void DotToDotEx(CDot dot,DOTEX& dt,UINT nFlag=0){dt.x = dot.x;dt.y = dot.y;dt.nFlag = nFlag;};
	void DotExToDot(DOTEX dot,CDot& dt){dt.x = dot.x;dt.y = dot.y;};
	
public:
	CProperty			m_property;
	CArray<CDot,CDot>	m_dotArray;
	BOOL		m_bSelect;
	
	BOOL	m_bLockFlag; // 锁定标记 TRUE：锁定(不能加载到XTracker中), FALSE ：非锁定
	BOOL	m_bTag;      // 选定标记 TRUE：已经选定(可编辑)， FALSE:没有选定(不可编辑)
	double	m_dbR;
	LPVOID* m_pParent;
	BOOL	m_bPathFlag;

private:
	int		m_nStyle;    // 操作标记  0-普通Stroke，1-填充线, 2-缩进轮廓
};

#endif // !defined(AFX_STROKEB_H__66C34014_40DD_40D9_B446_4BD0DD2BF8AD__INCLUDED_)
