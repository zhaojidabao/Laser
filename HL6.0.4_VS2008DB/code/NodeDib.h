// NodeDib.h: interface for the CNodeDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEDIB_H__3083ECE1_1673_11D5_8738_80564FC10108__INCLUDED_)
#define AFX_NODEDIB_H__3083ECE1_1673_11D5_8738_80564FC10108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Node.h"
#include "Quad.h"	// Added by ClassView


typedef struct MARK_BMP
{
	UINT nType;				//1-二值图 2-灰度图
	double* pdbX;
	double* pdbY;
	union{
		struct{
			BYTE* pbGray;
			double dbSpace;
			int nAtu_time; //衰减时间
			double dbAtu_tag; //衰减目标
			double dbPlus_cof; //增益系数
		}g;					//灰度处理用
		struct{
			int nDelay;
		}d;					//二值化处理用
	};
}MARK_BMP;


class CNodeDib : public CNodeV  
{
public:    
	CNodeDib();
	CNodeDib(CString filename);
	virtual ~CNodeDib();
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeDib();
		return pNode;
	};

public:
	virtual void AddToPlt(CNode *pNodePlt);
	virtual UINT Mark(HWND hWnd);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual BOOL Move(double moveX,double moveY,double moveZ = 0);
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Draw(XDC *pDC);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual BOOL Create(CRect &rect);
	virtual void SetProperty(int mode,CProperty pty, XDC *pDc = NULL);
	virtual void Rotate(CDot dot, double angle, int nPlane = 0);
	virtual BOOL CanGroup();
	virtual void Scan(StrokeList *pList){return;};
	virtual void Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY);
	virtual BOOL  Shear(double dbBaseX, double dbBaseY, double angleX, double angleY){return TRUE;};

	virtual CQuad CalLimit();
	virtual void Delete();
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual BOOL Click(CPoint &point,CWnd *pWnd);

public:
	BOOL DataChange();	

	double m_dbSpaceX;	//X轴向点间距
	double m_dbSpaceY;	//Y轴向点间距

	int		m_nWidth;
	int     m_nHeight;
	int		m_bOppsitionCol;		//1反色 0正常
	double	m_dbMoveX;		//move bitmap
	double	m_dbMoveY;		//move bitmap
	CString  m_strBmpName;
	double	m_dbDotTime;//打位图时每个像素的间隔值
	BOOL		m_bLoop;		//是否是往复打标
	int			m_nDibType;	//0:灰度，1：二值
	double	m_dbLoopCompate;
	int			m_nAccTime;
	double	m_dbHeight;
	double	m_dbWidth;
	double	m_dbBrightness; //-1~1 CXImage库内为 -255 to 255
	double	m_dbContrast;	//-1~1 CXImage库内为 -100 to 100
	// IPG位图打时标时按二值方式
	int m_nMinFDL;			// 最小关延时
	int m_nMaxFDL;			// 最大关延时
	BYTE* m_pImageSrc;
	
private:
	BOOL DeCode(CString lpszPathName);
	CBitmap m_bmp;	
	double m_dbAngle;
	int m_iDispWidth;
	int m_iDispHeight;
	BOOL m_bXFlip;
	BOOL m_bYFlip;
	XDC *m_pDC;
	HBITMAP GetRotatedBitmapNT( HBITMAP hBitmap, double radians, COLORREF clrBack );
	void CalDibRgn(CQuad& quad,CPoint* pCPoint,int iCnt);
	void UpdateName();

	int Pai2Deg(double f);
	double Deg2Pai(int a);
	void ProcMinusRatioX();
	void ProcMinusRatioY();
	double m_dbSpaceXR; //修正后点间距 for LD
	double m_dbSpaceYR; //

public:
	BOOL DataProc();
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill = 0,COLORREF colorFill = 0);
	virtual void LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	virtual void LoadWheelList(int nType, StrokeList *pListStroke);
};

#endif // !defined(AFX_NODEDIB_H__3083ECE1_1673_11D5_8738_80564FC10108__INCLUDED_)
