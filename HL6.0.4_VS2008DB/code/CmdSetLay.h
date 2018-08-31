// CmdSetLay.h: interface for the CCmdSetLay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSETLAY_H__F9C2F981_87FC_11D4_A484_00105A615525__INCLUDED_)
#define AFX_CMDSETLAY_H__F9C2F981_87FC_11D4_A484_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdSetLay : public CCommand  
{
public:
	CCmdSetLay(CHLDoc *pDoc,int mode,CLayer &ly);
	virtual ~CCmdSetLay();
	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc * m_pDoc;
	CLayer	m_layerOld;
	CLayer	m_layerNew;

	// 选中状态
	BOOL*	m_pFlag;

	// 旧图层参数
	int* m_pLayer;
	int* m_pFillLayer;
	int* m_pFillScalLayer;

	BOOL	m_bFirst;
	BOOL	m_bFlag;
	int		m_nMode;
};

#endif // !defined(AFX_CMDSETLAY_H__F9C2F981_87FC_11D4_A484_00105A615525__INCLUDED_)
