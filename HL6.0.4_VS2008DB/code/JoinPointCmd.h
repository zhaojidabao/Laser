// JoinPointCmd.h: interface for the JoinPointCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINPOINTCMD_H__926BEDA5_9CF1_40A5_9FF8_E760815BFD0D__INCLUDED_)
#define AFX_JOINPOINTCMD_H__926BEDA5_9CF1_40A5_9FF8_E760815BFD0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdBaseSave.h"

class CJoinPointCmd : public CCmdBaseSave
{
public:
	CJoinPointCmd(CHLDoc *pDoc,CWnd *pWnd);
	virtual ~CJoinPointCmd();
};

#endif // !defined(AFX_JOINPOINTCMD_H__926BEDA5_9CF1_40A5_9FF8_E760815BFD0D__INCLUDED_)
