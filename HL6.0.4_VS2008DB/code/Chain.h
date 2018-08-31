// Chain.h: interface for the CChain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAIN_H__93671F63_E3AE_11D3_A07E_00105A615525__INCLUDED_)
#define AFX_CHAIN_H__93671F63_E3AE_11D3_A07E_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Quad.h"	// Added by ClassView
#include "Tree.h"
#include "Node.h"	// Added by ClassView
#include "XDC.h"
#include "XTracker.h"
#include "ChainBH.h"
#include "GroupObj.h"
#include "NodeMotorMove.h"
#include "HLDoc.h"

class CChain : public CChainBH
{
	DECLARE_DYNAMIC( CChain )  

public:
	CChain(); 
	virtual ~CChain();
	void operator = (const CChain& src);

public:
	static int selectNull;
	static int selectSingle;
	static int selectMul;
	static int selectGroup;
	static int selectUnion;
	
public:
	int		GetCount();
	void	Array(CArray<OBJPOS, OBJPOS> &dtArray);
	void	CalLimit(CDot dot,BOOL bAll = FALSE,double dbXScale = 1.0,double dbYScale = 1.0);
	CQuad SetSelectFlag(BOOL *pFlag);
	void	GetSelectFlag(BOOL *pFlag);
	void	GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray);
	int		GetMenuIndex();
	CNode * GetNode();
	void	SetProperty(int mode,CProperty pty);
	void	Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY, double moveZ);
	//填充
	void	GetFillStyle(FILLDATA& fill);
	void	SetFillStyle(FILLDATA&	fill,XTracker *pTrack);
	void	SelectDraw(CGroupObj *pGroupObj,XTracker *pTrack);
	//剪贴板使用
	void	Clipboard(FILE* fp,BOOL bFlag);
	void	AddRect(CQuad &quad,CWnd *pWnd);
	/*
		针对PreprocessMark进行改造。
		使之真正成为数据预处理之用而不是简单的用于得到个数用
		所以加入风格参数，定义如下：
		0x00	-	老用法，只是得到当前个数，这种用法只用于在不需要处理的地方，如首个
		0x01	-	对跳号数据进行实际的数据预处理
		0x05  -  预处理检查
	*/
	BOOL PreprocessMark(DWORD nStyle = 0x00);
	BOOL Mark(HWND hWnd);
	void Copy();
	void Rotate(CDot &dot, double angle, int nPlane);
	void Shear(CDot dot, double dbAngleX, double dbAngleY);

	CQuad GetLimit();
	CQuad GetAllLimit();
	CQuadRangle GetQuadRangle();
	BOOL GetFrameDots(CArray<CDot,CDot>& frameDots);
	CQuad GetReFurbishQuad();

	void	UnGroup();
	int		GetStatus();		//返回当前选择集状态
	void Group();
	void UnionGroup();
	void UnUnionGroup();
	void Delete();
	void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void SerializeOld(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void SetSelect(BOOL bFlag = TRUE);

	CQuad Include(CQuad quad,BOOL bMulSelect);	//根据指定的框选范围进行点取识别

	void Scale(CDot dot,double ratioX,double ratioY);
	void Move(double moveX,double moveY,double moveZ);
	void Draw(XDC *pDC);
	void Add(CGroupObj *pGroupObj);
	void SetSelectedObjbeMarked(BOOL bFlag){m_bSelMark  = bFlag;};
	
	void SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor);
	void SetAllSelTreeDoubleOutParaAdd(double dbLeftXAdd,double dbLeftYAdd,double dbRightXAdd,double dbRightYAdd);
	void GetAllSelTreeProp(DOUBLEOUTPARA &DbPara, CQuad &QD);
	void GetSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns);
	void SetAllSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns);
	
	void SetDrawFill(CArray<SELQUAD ,SELQUAD> &QuadList, int nIndex);
	void GetQuadListInQuad(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &TrakerQD);
	BOOL IsInSelQuadList(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &QD);
	void SetSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd );
	void SetUnSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd );
	
	void OnMAlign(CArray<SELQUAD,SELQUAD> &quadList, int nType);
	void AjustTreeForQuad(CArray<SELQUAD,SELQUAD> &quadList);
	void RefreshSelectQuadList(CArray<SELQUAD,SELQUAD> &quadList);
	double GetTotalLength(); //激光总路程
	
	void ClearALLMainSelect();
	BOOL HasObjForProcess();

private:
	CQuad	m_quadLimit;
	CRITICAL_SECTION  m_lockData;

public:
	void	Convert(BOOL bPopMsg = TRUE);
	void	ChangeQuad(CPoint &point,XTracker *pTrack,CWnd *pWnd, CArray<SELQUAD ,SELQUAD> &QuadList);
	void	ReLoadStrokeList(StrokeList *pStrokeList,BOOL bFlag = FALSE);
	BOOL HitOtherObject(CPoint &point,CWnd *pWnd);
	BOOL HaveSelectObject();
	BOOL DragSelectGroupObj(CDot &dot,double ratioX,double ratioY,double moveX,double moveY,double moveZ);
	void	CaptureObject(CQuad &quad, XTracker *pTrack, BOOL bUniSel = FALSE);
	BOOL SetStrokeList(StrokeList *pStrokeList,CPoint &point,CWnd *pWnd);
	BOOL ObjectSelected(CPoint &point,CWnd *pWnd);
	UINT	PreTrack();
	CQuad GetMarkRange();
	void	GetSelectedStrokeList(StrokeList *pStrokeList);
	void	GetLayColor(int &nLayer,int &nFillLayer, int &nFillScalLayer);
	void	GetAllGroupLayColor(int* pLayer,int* pFillLayer, int* pFillScalLayer);
	void	SetAllGroupLayColor(int* pLayer,int* pFillLayer, int* pFillScalLayer);
	void	SetLayColor(int nLayer,int nFillLayer = -1, int nFillScalLayer = -1);
	BOOL GetMarkSel(){return m_bSelMark;};
	BOOL GetFirstMarkCurrent(double & dbCurrent);
	BOOL ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);

	void FilterPoint(double dbPrecis, double dbAngle);
	void ExtendObj(double dbSpace,BOOL bIsKillOldObj, XTracker *pTrack, BOOL bOpt = TRUE);

	BOOL InitSort(CArray<SORTNODE,SORTNODE> &Sort, CWnd *pWnd);
	BOOL InitASort(CArray<ASNODE,ASNODE> &Sort);
	BOOL InitSelSort(CArray<GNODE,GNODE> &Sort);

	void SetGroupNum(int nNum);
	void ConverStrokeCmd();
	void SelGroupByNum(int nNum, XTracker *pTrack,CWnd *pWnd);
	void DelObj(DWORD dwGroupObj);
	void DelAllObjInList();
	void SetGroupObjSel(CGroupObj *pGroupObj, XTracker* pTracker);

	void ObjListLoad();
	void ObjListUnLoad();

	BOOL IsHaveTextOrCodeBar();
	void ConvertToOneGroupObjDelete();
	void UpdateObjMarkSet(OBJMARKSET &ObjMarkSet);
	void GetObjMarkSet(OBJMARKSET &ObjMarkSet);
	void SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub);
	BOOL FastMark(HWND hWnd);
	void StoreChain(const CChain& chain);
	void ReStoreChain(const CChain& chain);

	// 选中打标对象数据
	void ReStoreSelChain(const CChain& chain);
	//
	BOOL TextReplace(CString& strTxt, CString& strModelName);
	void JoinStroke(BOOL bPopMsg,double dbMinDist);
	void Serialize_OptimizeList(FILE* fp,BOOL bFlag);
	void SetLayerSelect(int nItem,BOOL bFlag);
	void LoadAssistantPoints(CArray<DOT,DOT> &pointArray);
	void LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	void LoadWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType);
	BOOL MarkPathOptimizeByLay(HWND hWnd);
	BOOL MarkPathOptimizeByStroke(HWND hWnd);
	BOOL FastMarkPathOptimizeByLay(HWND hWnd);
	BOOL FastMarkPathOptimizeByStroke(HWND hWnd);
	void SetPathOptimizeState(BOOL bState);
	
public:
	int		GetSelObjLayerNo();
	BOOL HaveFunctionNode();
	BOOL HandleAutoSegment(double dbRadius);
	BOOL GetSelObjLockFlg();
	void RefreshBarText(CWnd* pWnd,UINT nTextID);
	void DeleteAll();
	void AdjustObjectOrder(int nFrom, int nTo);	//nFrom 原来的下标号， nTo 调整后的序号下标
	CGroupObj * GetMainSelGroupObj();
	
	//
	int CreateWheelStroke( int nProcType,double dbRadius,double dbDivAngle,int nFillNu = 1);
	void AddArrayList(CArray<StrokeList*, StrokeList*> &arrayList);
	void SortWheelObj();
	void ProWheelObj(double dbStartPos, double dbRadius);
	void InitQuadRange();
	BOOL GetTextExtInfo( CString strObjName, LP_TEXTEXINFO lpTextExtInfo);
	BOOL SetTextExtInfo( CString strObjName, LP_TEXTEXINFO lpTextExtInfo);

	//
	void ResetMgrText();
	void ResetTextID( int nMinID ,int nOpType = 0);

	BOOL IsEnable3DEdit();
	//
	StrokeList m_listStroke;
	BOOL m_bPathOptimize;
	int		m_nPathOptimizeType;
	BOOL m_bWheelProStop;
};

#endif // !defined(AFX_CHAIN_H__93671F63_E3AE_11D3_A07E_00105A615525__INCLUDED_)
