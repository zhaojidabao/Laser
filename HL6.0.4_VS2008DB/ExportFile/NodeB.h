// NodeB.h: interface for the CNodeB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEB_H__D7901975_42A0_49E6_A5D6_E9004BB62885__INCLUDED_)
#define AFX_NODEB_H__D7901975_42A0_49E6_A5D6_E9004BB62885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataBase.h"
#include "Property.h"
#include "XDC.h"
#include "StrokeB.h"

class AFX_EXT_CLASS CNodeB : public CDataBase
{
public:
	CNodeB();
	virtual ~CNodeB();
	
public:
	virtual void SetSelect(BOOL bSelect){m_bSelect = bSelect;};
	virtual void InvertSelect();
	virtual BOOL IsSelect(){return m_bSelect;};
	virtual int  GetBaseType();
	virtual void DrawExtern(XDC *pDC);
	virtual void LockLayer(int nLayer);
	virtual void UnLockLayer(int nLayer);
	virtual BOOL GetNodeFirstDot(CDot &dt);
	virtual BOOL GetNodeLastDot(CDot &dt);
	virtual void Adjust(MSG &msg,const CPoint &ptOrg);
	virtual BOOL Move_B(double moveX,double moveY);
	virtual BOOL Move_B(double moveX,double moveY, double moveZ);
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill=0,COLORREF colorFill=0);
	virtual void GetLayColor(int &nLayer,int &nFillLayer);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	virtual void SetProperty(int mode,CProperty pty, XDC *pDc = NULL);
	virtual void Scan(StrokeList *pList);
	virtual void Scale_B(CDot &dot,double ratioX,double ratioY);
	virtual void Rotate_B(CDot &dot,double angle);
	virtual void Rotate_B(CDot &dot, int nPlane, double angle); //nPlane: 0,XY 1,ZX 2,YZ
	virtual BOOL Shear_B(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual double GetLength();
	
public:
	void   SetGroupFlag(BOOL bFlag){m_bGroup = bFlag;};
	int    GetObjID();
	void   SetObjID(int nObjID);
    int    GetObjType();
	void   SetObjType(int nObjType);
	int    GetType();
	void   SetMarkMode(int nMarkMode);
	BOOL   IsEmpty();
	void   SetFlag(BOOL bflag);
	void   FilterPoint(double dbPrecis, double dbAngle);
	CStrokeB* GetGoLineStroke();
	
public:
	StrokeList m_list;
	BOOL      m_bSelect;//选中标志 TRUE: 选中; FALSE: 不选中
	UINT      m_nXPos;  //0 head;1 left,2 right,3 tail，4parity bit
	int			m_nMarkMode;
	BOOL      m_bIsCurSortTree;
	BOOL      m_bGroup;//表示条码是否为组合对象。
	CProperty m_property;

protected:
	int       m_type;
	int       m_nObjID;
	int       m_nObjType;
};

#endif // !defined(AFX_NODEB_H__D7901975_42A0_49E6_A5D6_E9004BB62885__INCLUDED_)
