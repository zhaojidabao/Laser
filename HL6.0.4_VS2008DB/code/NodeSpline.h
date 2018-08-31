// NodeSpline.h: interface for the CNodeSpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODESPLINE_H__B486F4E0_C7EC_4E66_8EE0_490E71886DFB__INCLUDED_)
#define AFX_NODESPLINE_H__B486F4E0_C7EC_4E66_8EE0_490E71886DFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Dot.h"
#include "Stroke.h"
#include "GSpline.h"

class CNodeSpline : public CNodeV  
{
public:
	CNodeSpline();
	virtual ~CNodeSpline();
	
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeSpline();
		return pNode;
	};

	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual void  Draw(XDC *pDC);	
	virtual void  Rotate(CDot dot, double angle, int nPlane);
	virtual void  Scale(CDot dot,double ratioX,double ratioY);
	virtual BOOL  Move(double moveX,double moveY,double moveZ = 0);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	
	//
	void Serialize_DotList(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void AddDotToArray(CArray<CDot,CDot> &listDot, CDot dot);
	void CreateStroke();

	BOOL TrackEdit(CWnd* pWnd);
	BOOL CheckTouchStatus(CDot dot);
	double GetDotDistance(CDot dot1, CDot dot2);
	void SetEditPoint(CDot dot);
	void DrawCtrlPointStroke(XDC *pXDC, double dbMove);
	void PrecisionSet(double dbPrecision, int nSampleCount);

public:
	CGSpline m_GSpline;
	CStroke  m_stkCtrlDot;
	CArray<int, int> m_aryTouchSts;
	
	double m_dbPrecision;
	int      m_nSampleCount;
};

#endif // !defined(AFX_NODESPLINE_H__B486F4E0_C7EC_4E66_8EE0_490E71886DFB__INCLUDED_)
