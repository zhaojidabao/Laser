// NodeCircle.h: interface for the CNodeCircle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECIRCLE_H__8DC0CF6E_7D26_421B_B120_4A1974C0C43E__INCLUDED_)
#define AFX_NODECIRCLE_H__8DC0CF6E_7D26_421B_B120_4A1974C0C43E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeV.h"
#include "Dot.h"
#include "Stroke.h"
#include "GArc.h"

class CNodeCircle : public CNodeV  
{
public:
	CNodeCircle();
	virtual ~CNodeCircle();

	//	
	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual void  Draw(XDC *pDC);	
	virtual void  Rotate(CDot dot,double angle,int nPlane);
	virtual void  Scale(CDot dot,double ratioX,double ratioY);
	virtual BOOL Move(double moveX,double moveY,double moveZ);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeCircle();
		return pNode;
	};
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
	virtual void LoadAssistantPoints(CArray<DOT,DOT> &pointArray);
	//
	CGArc    m_GArc;
	CStroke  m_stkCtrlDot;
	CArray<int, int> m_aryTouchSts;
	int m_nArcType;
	
	double m_dbPrecision;
	int m_nSampleCount;
};

#endif // !defined(AFX_NODECIRCLE_H__8DC0CF6E_7D26_421B_B120_4A1974C0C43E__INCLUDED_)
