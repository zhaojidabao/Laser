// NodeEllipse.h: interface for the CNodeEllipse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEELLIPSE_H__7C4826C1_3F94_11D4_B789_FAB90A0E980F__INCLUDED_)
#define AFX_NODEELLIPSE_H__7C4826C1_3F94_11D4_B789_FAB90A0E980F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeV.h"
#include "Dot.h"

class CNodeEllipse : public CNodeV  
{
public:
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void SerializePoint(FILE *fp, BOOL bFlag,BOOL bUnicode);
	BOOL Create(CDot dt1,CDot dt2,DOT Center,MSG msg);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeEllipse();
		return pNode;
	};

	CNodeEllipse();
	virtual ~CNodeEllipse();
	
private:
	CDot			m_dotArray[13];
	CStroke*	m_pStrkTemp;
	double		m_dbARC;
	int				m_nCount;

public:
	virtual void   Rotate(CDot dot,double angle);
	void	CtrlPointToPoint();
	void	PointToCtrPoint();
	UINT	Mark(HWND hWnd);
	virtual UINT SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark);
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill = 0,COLORREF colorFill = 0);
	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
};

#endif // !defined(AFX_NODEELLIPSE_H__7C4826C1_3F94_11D4_B789_FAB90A0E980F__INCLUDED_)
