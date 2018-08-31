// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Quad.h"
#include "StrokeB.h"
#include "NodeB.h"
#include "QuadRangle.h"

class CQuad;
class CTree;

class AFX_EXT_CLASS CNode : public CNodeB  
{
public:
	CNode();
	virtual ~CNode();

public:
	StrokeList &GetList(){ return m_list; };
	void	 Serialize_list(FILE* fp,BOOL bFlag,BOOL bUnicode);

public:
	virtual void	  AddToPlt(CNode *pNodePlt);
	virtual BOOL  LayerSelect(int nLayer);
	virtual void	  SetStrokeList(StrokeList *pStrokeList);
	virtual void   GetStrokeList(StrokeList *pStrokeList);

	virtual BOOL   PreprocessMark(DWORD nStyle){return FALSE;};
	virtual BOOL   Track(CWnd* pWnd,CPoint ptOrg){return TRUE;};
	virtual BOOL   Create(CRect &rect){return TRUE;};
	virtual BOOL   CopyToMe(CNode *pSrc,DWORD dwStyle);//复制全部数据,dwStyle仅对条码和文本有效
	virtual void    Delete();
	virtual BOOL  CanUnGroup(){return FALSE;};
	virtual BOOL  CanGroup(){return TRUE;};
	virtual  void   Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode){};

	virtual CQuad CalLimit() = 0;
	virtual CQuad GetLimit(BOOL bRef = FALSE) = 0;
	virtual BOOL Include(CQuad quad) = 0;
	virtual BOOL GetLockFlg() = 0;
	virtual CStrokeB* CreateNewStroke() = 0;
	virtual CNode*	CreateNewNode() = 0;
	virtual void   InitQuadRange() = 0;
	virtual BOOL  IsOnDot(CPoint &point,CWnd *pWnd) = 0;
	virtual BOOL  Click(CPoint &point,CWnd *pWnd) = 0;
	virtual void   Draw(XDC *pDC) = 0;
	virtual void   AddRect(CWnd *pWnd) = 0;
	virtual void   DelRect(CWnd *pWnd) = 0;
	virtual CTree* UnGroup() = 0;
	virtual UINT    Mark(HWND hWnd) = 0;
    virtual UINT    SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark) = 0;

	virtual BOOL  Move( double moveX,double moveY,double moveZ = 0 );
	virtual BOOL  Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual void	  Scale(CDot dot,double ratioX,double ratioY);
	virtual void   Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY,double moveZ = 0);
	virtual void   Rotate(CDot dot, double angle, int nPlane=0 );

	virtual void  SetSelect(BOOL bSelect);
	//
	virtual void   JoinStroke(double dbMinDistance);
	virtual void   LoadAssistantPoints(CArray<DOT,DOT> &pointArray);
	virtual void   LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	virtual void   LoadWheelList(int nType, StrokeList *pListStroke);
	virtual void   SetPathOptimizeState(BOOL bState);	
	virtual  CQuadRangle GetQuadRangle();	
	
public:
	CNode* 	m_pLeft;
	CNode* 	m_pRight;
	BOOL		m_bPathOptimize;
	
protected:
	CQuadRangle m_quadRangle;	
	//////
	void	 AppendStrokeB(CStrokeB *pObjStroke, CStrokeB *pSortStroke);

};

