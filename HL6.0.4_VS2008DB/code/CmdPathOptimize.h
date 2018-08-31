// CmdPathOptimize.h: interface for the CCmdPathOptimize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDPATHOPTIMIZE_H__E7B40A82_E6EB_4ADF_9E49_BDB3F2B593A9__INCLUDED_)
#define AFX_CMDPATHOPTIMIZE_H__E7B40A82_E6EB_4ADF_9E49_BDB3F2B593A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HLDoc.h"
#include "mfccmd.h"

class CCmdPathOptimize : public CCommand  
{
public:
	CCmdPathOptimize(CHLDoc *pDoc);
	virtual ~CCmdPathOptimize();

	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	CHLDoc * m_pDoc;
};

#endif // !defined(AFX_CMDPATHOPTIMIZE_H__E7B40A82_E6EB_4ADF_9E49_BDB3F2B593A9__INCLUDED_)
