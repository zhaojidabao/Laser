// CmdChangLay.h: interface for the CCmdChangLay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDCHANGLAY_H__D6326A24_FA9C_11D8_ABC5_000AE64C79F3__INCLUDED_)
#define AFX_CMDCHANGLAY_H__D6326A24_FA9C_11D8_ABC5_000AE64C79F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdChangLay : public CCommand  
{
public:
	CCmdChangLay(CHLDoc *pDoc,int nLayer,int nFillLayer,int nFillScalLayer);
	virtual ~CCmdChangLay();
	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	BOOL m_bFirst;
	CHLDoc * m_pDoc;

	// 选中状态
	BOOL*	m_pFlag;

	// 新图层参数
	int m_nLayer;
	int m_nFillLayer;
	int m_nFillScalLayer;

	// 旧图层参数
	int* m_pLayer;
	int* m_pFillLayer;
	int* m_pFillScalLayer;
};

#endif // !defined(AFX_CMDCHANGLAY_H__D6326A24_FA9C_11D8_ABC5_000AE64C79F3__INCLUDED_)
