#pragma once

#include "mfccmd.h"
#include "HLDoc.h"

class CModifyZCmd :	public CCommand
{
public:
	CModifyZCmd(XTracker *pTrack,double dbX, double dbY, double dbZ);
	virtual ~CModifyZCmd();

	virtual BOOL Do();
	virtual BOOL Undo();

protected:
	XTracker* m_pTracker;
	double	m_dbMoveX;
	double	m_dbMoveY;
	double	m_dbMoveZ;
};
