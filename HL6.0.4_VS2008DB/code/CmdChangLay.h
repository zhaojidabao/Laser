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

	// ѡ��״̬
	BOOL*	m_pFlag;

	// ��ͼ�����
	int m_nLayer;
	int m_nFillLayer;
	int m_nFillScalLayer;

	// ��ͼ�����
	int* m_pLayer;
	int* m_pFillLayer;
	int* m_pFillScalLayer;
};

#endif // !defined(AFX_CMDCHANGLAY_H__D6326A24_FA9C_11D8_ABC5_000AE64C79F3__INCLUDED_)
