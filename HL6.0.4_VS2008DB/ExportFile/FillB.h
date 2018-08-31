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
#define FILL_MORD		8		//���˳���ȴ�������ٴ��߿�
#define FILL_PTP		16
#define FILL_CIRCLE		32
#define FILL_BETTER		64

class AFX_EXT_CLASS CFillB
{	
public:
	CFillB();
	virtual ~CFillB();

public:
	StrokeList* m_plist;		//ֻ�Ǳ���ָ������ݣ���������ʵ��
	
	double m_dbSpace;
	double m_dbBorderSpace;
	double m_dbPtp;				// ��������
    double m_dbCircle;			// ���߼��
    double m_dbHeadKillLen;		// ��������
    double m_dbTailKillLen;		// β������
	UINT m_nStyle;
	double m_dbAngle;			//���Ƕ�
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
