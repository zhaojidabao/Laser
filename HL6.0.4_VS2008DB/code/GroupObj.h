// GroupObj.h: interface for the CGroupObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPOBJ_H__84771DF0_22D0_44EC_81B5_1D6E9DDB904D__INCLUDED_)
#define AFX_GROUPOBJ_H__84771DF0_22D0_44EC_81B5_1D6E9DDB904D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tree.h"
#include "Nodeplt.h"
#include "GroupB.h"
#include "Chain3D.h"

class CGroupObj : public CGroupB
{
public:
	CGroupObj();
	virtual ~CGroupObj();
	
	OBJMARKSET m_MarkSet;
	BOOL			m_bMainSelect;

	CQuad GetLimit();
	BOOL Mark(HWND hWnd);
	BOOL CopyToMe( CGroupObj *pSrc, DWORD dwStyle,BOOL bGenFill = TRUE );
	void InsertHead(CTree *pTree);
	void Rotate(CDot &dot, double angle, int nPlane);
	void DeleteAll();
	void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void SetSelect(BOOL bFlag = TRUE);
	BOOL GetSelect(){return m_bSelect;};
	void Scale(CDot dot,double ratioX,double ratioY);
	void Drag(CDot dot, double ratioX, double ratioY, double moveX, double moveY, double moveZ);
	void Move(double moveX,double moveY,double moveZ);
	void Draw(XDC *pDC);
	BOOL Add(CTree *pTree);
	BOOL IsSelect();
	BOOL Include(CQuad quad);
	void InvertSelect();
	void SetStrokeList(StrokeList *pStrokeList);
	void Shear(CDot dot, double dbAngleX, double dbAngleY);
	BOOL GetLockFlg();
	void UnionGroup(CGroupObj *pGroupObj);
	void UnUnionGroup( CTypedPtrList<CObList,CGroupB *> &listGroupObj);
	void Group(CGroupObj *pGroupObj);
	void UnGroup(CTypedPtrList<CObList,CGroupB *> &listGroupObj);
	BOOL CheckBarCode(CUIntArray &arrayTextID,CUIntArray &arrayValidID);
	void SelectBarCode(CUIntArray& arrayTextID);
	void SetStrokeList(StrokeList *pStrokeList, BOOL bFlag);
	void SetFillStyle(FILLDATA& fill);
	void GetFillStyle(FILLDATA& fill);
	BOOL IsMainSelect();
	void SetMainSelect(BOOL bSelect);
	void SetProperty(int mode,CProperty pty);
	BOOL PreprocessMark(DWORD nStyle);
	CNode * GetNode();
	void GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray);
	void CalLimit(CDot dot,BOOL bAll,double dbXScale,double dbYScale );
	CQuadRangle GetQuadRangle();
	//预览边框
	BOOL GetFrameDots(CArray<CDot,CDot>& frameDots);

	void SetLayColor(int nLayer, int nFillLayer, int nFillScalLayer);
	void SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor);
	void GetLayColor(int &nLayer,int &nFillLayer, int &nFillScalLayer);

	CQuad GetReFurbishQuad();
	void GetSelectedStrokeList(StrokeList *pStrokeList);
	CQuad GetMarkRange();
	BOOL GetFirstMarkCurrent(double & dbCurrent);
	BOOL ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	void AddRect(CWnd *pWnd);
	BOOL MouseOnObject(CPoint &point,CWnd *pWnd);
	void CaptureObject(CQuad &quad,XTracker *pTrack);
	BOOL Click(CPoint &point,CWnd *pWnd);

	// add by zhq
	void LoadAssistantPoints(CArray<DOT,DOT> &pointArray);
	void LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	void LoadWheelList(CArray<StrokeList*, StrokeList*> &arrayList, int nType);
	int   GetType();
	void RefreshBarText(CWnd* pWnd,UINT nTextID);
	void SetPathOptimizeState(BOOL bState);
	BOOL GetSelObjLockFlg();
	void SetLayerSelect(int nItem,BOOL bFlag);
	void DrawExtern(XDC *pDC);
	int   GetSelObjLayerNo();
	CGroupObj * ExtendObj(double dbSpace, BOOL bOpt);
	void UpdateObjMarkSet(OBJMARKSET &ObjMarkSet);
	BOOL IsBmpNode();
	BOOL IsHaveTextOrCodeBar();
	double GetLength();
	void SetGroupNum(int nNum);
	int   GetGroupNum();
	BOOL ComToOneGroupObj(CNode *pNode);
	BOOL GetFirstDot(CDot &dt);
	BOOL GetLastDot(CDot &dt);
	void FilterPoint(double dbPrecis, double dbAngle);
	void JoinStroke(double dbMinDistance);
	void  Convert();
	UINT CanConvert();
	CGroupObj * ConvertPLT();
	void  LoadTreeList(TREELIST &listTree);
	void  GetStrokeList(StrokeList *pStrokeList);
	BOOL IsHaveTxtOrCodeBar();
	void  SetSortStatus(BOOL bIsSort);
	BOOL HaveFunctionNode();
	UINT  GetTreeCount(){return m_list.GetCount();};
	
	virtual void GetTextValue(CString& strTxt);
	void   SetTextData(CString &strTxt);
	BOOL  GetRulerPos(CDot &dot, int nAlignType);
	
	void LabelTo3D(LABEL_DATA &data);
	void Create3dDataFromHS( StrokeList *pListStroke, LABEL_NODE *pLabelNode );
	void InitQuadRange();
	
	BOOL GetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);
	BOOL SetTextExtInfo( LP_TEXTEXINFO lpTextExtInfo);

	void   ResetTextID( int nMinID, int nOpType);
	BOOL IsEnable3DEdit();

private:
	TREELIST	m_list;			//Tree链表

protected:
	BOOL			m_bSelect;		//是否被选中
	int				m_nGroupNum;
};

#endif // !defined(AFX_GROUPOBJ_H__84771DF0_22D0_44EC_81B5_1D6E9DDB904D__INCLUDED_)
