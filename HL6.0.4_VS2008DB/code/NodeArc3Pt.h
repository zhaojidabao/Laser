// NodeArc3Pt.h: interface for the CNodeArc3Pt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEARC3PT_H__04F90BF8_4AE3_4B5E_8BA6_42C2852E8744__INCLUDED_)
#define AFX_NODEARC3PT_H__04F90BF8_4AE3_4B5E_8BA6_42C2852E8744__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Dot.h"
#include "Stroke.h"
#include "GArc.h"

class CNodeArc3Pt : public CNodeV  
{
public:
	CNodeArc3Pt();
	CNodeArc3Pt(int nArcType);
	virtual ~CNodeArc3Pt();
	
	//
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeArc3Pt();
		return pNode;
	};	

	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual void  Draw(XDC *pDC);	
	virtual void  Rotate(CDot dot, double angle, int nPlane = 0);
	virtual void  Scale(CDot dot,double ratioX,double ratioY);
	virtual BOOL Move(double moveX,double moveY, double moveZ = 0);
	virtual void   ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	virtual BOOL CopyToMe(CNode *pSrc,DWORD dwStyle);
	
	//
	void Serialize_DotList(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void AddDotToArray(CArray<CDot,CDot> &listDot, CDot dot);
	void CreateStroke();
	
	BOOL TrackEdit(CWnd* pWnd);
	BOOL CheckTouchStatus(CDot dot);
	double GetDotDistance(CDot dot1, CDot dot2);
	void SetEditPoint(CDot dot);
	void DrawCtrlPointStroke(XDC *pXDC, double dbMove);
	void DrawPrevStroke(CDot dot1, CDot dot2, CDot dot3, XDC *pXDC);
	void PrecisionSet(double dbPrecision, int nSampleCount);
	
	//
	CGArc	m_GArc;
	CStroke	m_stkCtrlDot;
	CArray<int, int> m_aryTouchSts;

	int			m_nArcType;
	double	m_dbPrecision;
	int			m_nSampleCount;
};

#endif // !defined(AFX_NODEARC3PT_H__04F90BF8_4AE3_4B5E_8BA6_42C2852E8744__INCLUDED_)
