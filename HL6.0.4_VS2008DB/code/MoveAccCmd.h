// MoveAccCmd.h: interface for the MoveAccCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEACCCMD_H__C6164DCC_DF34_4AA9_810D_D37A0EE27EEA__INCLUDED_)
#define AFX_MOVEACCCMD_H__C6164DCC_DF34_4AA9_810D_D37A0EE27EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CMoveAccCmd : public CCommand  
{
public:
	CMoveAccCmd(XTracker *pTracker,double dbMoveX,double dbMoveY);
	virtual ~CMoveAccCmd();

	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	XTracker *m_pTracker;
	double   m_dbMoveX;
	double   m_dbMoveY;
};

#endif // !defined(AFX_MOVEACCCMD_H__C6164DCC_DF34_4AA9_810D_D37A0EE27EEA__INCLUDED_)
