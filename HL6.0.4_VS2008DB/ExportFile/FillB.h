// FillB.h: interface for the CFillB class.

//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILLB_H__1CBC1553_E86A_43B7_B960_ECA5770B6159__INCLUDED_)
#define AFX_FILLB_H__1CBC1553_E86A_43B7_B960_ECA5770B6159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrokeB.h"
#include "Property.h"


#define FILL_BACK		1
#define FILL_CROSS		2
#define FILL_EVEN		4
#define FILL_MORD		8		//打标顺序，先打填充线再打标边框
#define FILL_PTP		16
#define FILL_CIRCLE		32
#define FILL_BETTER		64

class AFX_EXT_CLASS CFillB
{	
public:
	CFillB();
	virtual ~CFillB();

public:
	StrokeList* m_plist;		//只是保存指针的数据，并不产生实体
	
	double m_dbSpace;
	double m_dbBorderSpace;
	double m_dbPtp;				// 点间距设置
    double m_dbCircle;			// 绕线间距
    double m_dbHeadKillLen;		// 首缩进量
    double m_dbTailKillLen;		// 尾缩进量
	UINT m_nStyle;
	double m_dbAngle;			//填充角度
	CProperty m_property;
	
	//
	NEW_FILL_DATA m_newFillData;
	
public:
	void Proc(StrokeList* plistData,StrokeList& listFillData);
	BOOL IsValide(double x1,double x2,double &ox1,double &ox2);
	void sort_bubble(double *plist, int size,BOOL bFlag);
	void CreateDotArray(DotList* pList,DotList& dotArray);
	void ScanFill(DotList* listInfo,double scan,double str,double end);
	void Rotate(StrokeList* plist,CDot dot, double angle);
	void Sortlist(DotList& dotArray,BOOL bFlag);
	void DeleteTmpList(StrokeList *list);
	void BetterProc(CArray<StrokeList*, StrokeList*> &FillList,
		            CArray<StrokeList*, StrokeList*> &RetList);
	double Distance(CDot &sDot, CDot &eDot);
	BOOL   DeletStrokeList(StrokeList* pList);
	void NobetterPro(CArray<StrokeList*, StrokeList*> &FillList, StrokeList& listFillData);
	BOOL StrokeProc(CStrokeB *pStrk, CArray<StrokeList*, StrokeList*> &RetList);
	void ListSortProc(CArray<StrokeList*, StrokeList*> &FillList, 
		CArray<StrokeList*, StrokeList*> &RetList);
	void ReverStrkList(StrokeList *pStrkList);
	void ReleaseFillList(CArray<StrokeList*, StrokeList*> &FillList);
	void AddToOneList(CArray<StrokeList*, StrokeList*> &FillList, CArray<StrokeList*, StrokeList*> &RetList);
	BOOL ZoomStroke(CStrokeB* pStroke,BOOL bBackFillDirect);
	void PtpProc(StrokeList* pList);
};

#endif // !defined(AFX_FILLB_H__1CBC1553_E86A_43B7_B960_ECA5770B6159__INCLUDED_)
