// Tree.h: interface for the CTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__F21BEEC1_E087_11D3_845D_444553540001__INCLUDED_)
#define AFX_TREE_H__F21BEEC1_E087_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Node.h"
#include "Quad.h"	// Added by ClassView
#include "Stroke.h"
#include "Property.h"
#include "Fill.h"
#include "XDC.h"
#include "TreeB.h"
#include "QuadRangle.h"	// Added by ClassView
#include "Plane.h"
#include "NodeT.h"


class CTree : public CTreeB  
{
	DECLARE_SERIAL(CTree)

public:
	//////////////////////////////////////////////
	//构造及析构
	CTree();
	CTree(CNode *pNode);
	virtual ~CTree();
	
	//增加节点
	void Add(CNode *pNode);
	BOOL CopyToMe(CTree *pSrc, DWORD dwStyle);
	//设置打标参数
	void SetProperty(int mode,CProperty pty);
	//拉变
	void Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY,double moveZ);
	//打标前预处理
	BOOL PreprocessMark(DWORD nStyle);
	//打标
	UINT Mark(HWND hWnd);
	//删除
	void Delete();
	//旋转
	void Rotate(CDot dot, double angle, int nPlane);
	void RotateFill(CDot dot, double angle, int nPlane);
	//切变
	void Shear(CDot dot, double dbAngleX, double dbAngleY);
	//移动
	void Move(double moveX,double moveY, double moveZ);
	//缩放
	void Scale(CDot dot,double ratioX,double ratioY);
	//计算有效范围及获取
	CQuad CalLimit();
	CQuad GetLimit();

	//绘图显示
	void	Draw(XDC *pDC);

	//序列化处理
	void	Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void	Save(CNode *pNode,FILE* fp,BOOL bLeft,BOOL bUnicode);
	CNode * Load(FILE* fp,BOOL bUnicode);

	//是否包含于指定的范围内
	BOOL Include(CQuad quad);
	//点取
	BOOL Click(CPoint &point,CWnd *pWnd);

	//挑选操作
	void	SetSelect(BOOL bFlag = TRUE);
	BOOL IsSelect();
	void	InvertSelect();

	//组操作
	CTree* 	UnGroup();
	BOOL		IsGroup();
	void		Group(CTree *pTree);
	BOOL		CanGroup();

	//填充
	void Fill();
	void TransFillLine(StrokeList& listFill,BOOL bDraw = FALSE);
	void Scan(CNode *pNode,StrokeList* listTemp);
	void GetFillStyle(FILLDATA& fill);
	void SetFillStyle(FILLDATA& fill);
	void DeleteFill();

	//取根节点
	CNode *GetRoot(){return m_pRoot;};
	int  GetMarkUnit(){return m_nMarkUnit;};

	void AddRect(CWnd *pWnd);
	BOOL GetLockFlg();
	BOOL IsBmpNode();

protected:
	CNode*  m_pRoot;
	CQuad	m_quadLimit;

	CNode*  CopyFrom(CNode *pNode,DWORD dwStyle);//复制
	void	Shear(CNode *pNode,CDot dot, double dbAngleX, double dbAngleY);
	void	Scale(CNode *pNode,CDot dot,double ratioX,double ratioY);
	void	Move(CNode *pNode,double moveX,double moveY, double moveZ);
	void	Rotate(CNode *pNode,CDot dot, double angle, int nPlane);	
	void   Drag(CNode *pNode,CDot dot,double ratioX,double ratioY,double moveX,double moveY, double moveZ);

	void   CalLimit(CNode *pNode);
	void   AddRect(CNode *pNode,CWnd *pWnd);

	BOOL IsBmpNode(CNode *pNode);
	BOOL GetLockFlg(CNode* pNode);
	BOOL PreprocessMark(CNode *pNode,DWORD nStyle);

	void   SetProperty(CNode *pNode,int mode,CProperty pty);
	UINT  Mark(CNode *pNode,HWND hWnd);
	BOOL Click(CNode *pNode,CPoint &point,CWnd *pWnd);
	BOOL Include(CNode *pNode,CQuad quad);
	void   Delete(CNode *pNode);
	void	InvertSelect(CNode *pNode);
	void	SetSelect(CNode *pNode,BOOL bFlag = TRUE);
	BOOL IsSelect(CNode *pNode);
	void	Draw(CNode *pNode,XDC *pDC);
	
public:
	double m_dbFillAngle;    //填充线角度
	double m_dbAngle1_Old;
	double m_dbAngle2_Old;
	double m_dbDistance_Old;
	double m_dbSpace1_Old;
	double m_dbSpace2_Old;
	int		  m_nStyle_Old;
	double m_dbCircleLen_Old;
	double m_dbHeadKillLen_Old;
	double m_dbTailKillLen_Old;
	//
	void SaveOldFill();
	void LoadOldFill();
	void Serialize_FillPara(FILE* fp,BOOL bFlag,BOOL bUnicode);

public:
	WORD    m_nGroup;	//组编号,用于旧版本兼容
	int		 GetType();
	StrokeList m_listDraw; //用于填充

	UINT MarkNormal(HWND hWnd);
	UINT MarkSpecParaSet(HWND hWnd);

	UINT SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub);
	UINT SelMark(CNode *pNode, int nLayer, HWND hWnd, HANDLE hStop, BOOL bIsRevMark = FALSE);
	UINT SelMarkFill(HWND hWnd, int nLayer, HANDLE hStop, BOOL bIsRevMark = FALSE);

public:
	void    RefreshBarText(CNode* pNode,CWnd* pWnd, int nTextID);
	virtual void AddToPlt(CNode *pNode,CNode *pNodeplt);

	CTree* ConvertPLT();
	CTree* ConvertFillToPLT();

	UINT CanConvert();
	void	AddFillToStrokeList(StrokeList *pStrokeList);
	void	CaptureObject(CQuad &quad,StrokeList *pStrokeList);
	void	SetStrokeList(CNode *pNode,StrokeList *pStrokeList,BOOL bFlag = FALSE);
	void	SetStrokeList(StrokeList *pStrokeList,BOOL bFlag = FALSE);
	BOOL IsEmpty(CNode *pNode);
	BOOL IsEmpty();
	void	DelRect(CNode *pNode,CWnd *pWnd);
	void	DelRect(CWnd *pWnd);
	BOOL PtInTree(CDot &dot);
	BOOL PtInTree(CPoint &point,CWnd *pWnd);
	BOOL SelectBarCode(CUIntArray& arrayTextID);
	BOOL CheckBarCode(CNode*pNode,CUIntArray& arrayTextID,CUIntArray &arrayValidID);
	void	GetMarkRange(CNode *pNode, CQuad &quad);
	CQuad GetMarkRange();
	void	GetStrokeList(CNode *pNode,StrokeList *pStrokeList);
	void	GetStrokeList(StrokeList *pStrokeList);

	void	SetLayColor(int nLayer,int nFillLayer, int FillScalLayer);
	void	SetLayColor(CNode *pNode,int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill);

	void	GetLayColor(int &nLayer,int &nFillLayer, int &FillScalLayer);
	void	GetLayColor(CNode *pNode,int &nLayer,int &nFillLayer);
	
	void	SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor);
	
	CQuad GetReFurbishQuad();

	void SetGroup(BOOL bGroup);
	void SetGroup(CNode *pNode,BOOL bGroup);

	BOOL GetFirstMarkCurrent(double & dbCurrent);
	BOOL ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	void ExportPlt(CNode *pNode,CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);

	BOOL AddFillToPlt(CNode *pNodeplt);

	void FilterPoint(double dbPrecis, double dbAngle);
	void FilterPoint(CNode *pNode, double dbPrecis, double dbAngle);

	BOOL GetFirstDot(CDot &dt);
	BOOL GetTreeFirstdot(CNode *pNode, CDot &dt);

	BOOL GetLastDot(CDot &dt);
	BOOL GetNodeLastDot(CNode *pNode, CDot &dt);

	void SetSortStatus(BOOL bIsSort = FALSE);
	void SetSortStatus(CNode *pNode, BOOL bIsSort);

	CTree*  ExtendObj(double dbSpace, BOOL bOpt = TRUE);

	CStroke* GetGoLineStroke();
	CStroke* GetGoLineStroke(CNode *pNode);

	void SetGroupNum(int nNum);
	int  GetGroupNum();

	BOOL ComToOneNode(CNode *pNode);

	double GetLength(CNode* pNode);
	double GetLength();

	BOOL IsHaveTxtOrCodeBar();
	BOOL IsHaveTxtOrCodeBar(CNode *pNode);

	void DrawExtern(CNode *pNode, XDC *pDC);
	void DrawExtern(XDC *pDC);

	CQuad& RetLimit();
	
	void JoinStroke(double dbMinDistance);
	void JoinStroke(CNode *pNode,double dbMinDistance);
	UINT MarkFill(HWND hWnd, BOOL bInverse);

	BOOL SetLayerSelect(int nItem,CNode *pNode,BOOL bFlag);
	BOOL SetLayerSelect(int nItem,BOOL bFlag);

	// add by zhq
	void LoadAssistantPoints(CArray<DOT,DOT> &pointArray);
	void LoadAssistantPoints(CNode *pNode, CArray<DOT,DOT> &pointArray);

	void LoadStrokeList(StrokeList *pStrokeList,BOOL bSelect,int nLayNo);
	void LoadStrokeList(CNode *pNode, StrokeList *pStrokeList,BOOL bSelect,int nLayNo);
	void LoadFillStrokeList(StrokeList *pStrokeList,BOOL bSelect,int nLayNo);

	void LoadWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType);
	void LoadWheelList(CNode *pNode, int nType, StrokeList *pListStroke);
	void LoadFillWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType);

	//
	BOOL m_bPathOptimize;
	void  SetPathOptimizeState(BOOL bState);
	void  SetPathOptimizeState(CNode *pNode,BOOL bState);
	BOOL HaveFunctionNode();
	BOOL HaveFunctionNode(CNode *pNode);

	void ChangeText(CNode *pNode, CString strTxt);
	void  SetTextData(CNode *pNode, CString& strTxt);
	void  GetTextValue(CNode* pNode,CString& strTxt);
	BOOL HaveNode(int nType);
	BOOL HaveNode(CNode *pNode, int nType);
	void  InitQuadRange(CNode *pNode);
	
	void SetFillProperty();
	UINT MarkFillList(HWND hWnd);
	UINT MarkList(HWND hWnd, const StrokeList &list);

	//
	void LoadFillLine(StrokeList *pStrokeList );
	void LoadFillContour(StrokeList *pStrokeList );
	
	BOOL GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);
	BOOL SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);
	
	void ResetTextID( int nMinID, int nOpType );
	void ResetTextID( CNode *pNode, int nMinID,int nOpType );
	//
	CNodeT* TransNode(CNode *pNode);
	BOOL      IsEnable3DEdit();
	CMgrText*  GetCurMgrText();
	//
};

#endif // !defined(AFX_TREE_H__F21BEEC1_E087_11D3_845D_444553540001__INCLUDED_)






















