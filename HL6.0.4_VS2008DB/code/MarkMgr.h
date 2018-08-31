// MarkMgr.h: interface for the CMarkMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARKMGR_H__785718E1_79B0_44C5_9918_F78FB6E8B536__INCLUDED_)
#define AFX_MARKMGR_H__785718E1_79B0_44C5_9918_F78FB6E8B536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HLDoc.h"

#define MARK_TEND	100
#define MARK_SEND	101
#define MARK_PREE	102
#define MARK_INFO	103

extern HANDLE hMark;
extern HANDLE hStopWork;
extern HANDLE hMarkSts;
extern HANDLE hThreadRun;
extern HANDLE hStopAfterTouch;

class CMarkMgr
{
public:
	CMarkMgr();
	virtual ~CMarkMgr();

public:
	BOOL	MarkIsOver();
	void	SetLoopMarkInfo(BOOL bLoopMark, int nIntervalTime);
	void	SetLay(const CLay &lay);
	BOOL	GetRedPreview();
	void	StopMark();
	UINT	ReMark();
	void	PauseMark();
	void	StartMark();
	DWORD	GetSingleMarkTime();
	BOOL	SetChain(CChain* pChain);
	void	StopThread();
	void	RunThread(LPVOID lPara);

	static UINT Touch(LPVOID lPara);			//��̤����
	static UINT PreProcMark(LPVOID lPara);	//����Ԥ������
	static UINT Mark(LPVOID lPara);			    //��괦��
	static void SetMarkType(DWORD nType);
	
	static CChain3D* m_pChain3d;
	static CChain*	m_pChain;
	static DWORD	m_dwSingle;		//�������ʱ��
	static DWORD	m_dwMarkCount;	//��ʶ�ܵ�Ԫ��
	static DWORD	m_dwCurPos;		//��ʶ��ǰ���λ�ã�����һ�������γɰٷֱ�

	static BOOL		m_bLoopMark;
	static int		m_nIntervalTime;
	static BOOL    m_bPreProc;

protected:
	static DWORD	m_nMarkType;	//��귽ʽ
	
	
};

#endif // !defined(AFX_MARKMGR_H__785718E1_79B0_44C5_9918_F78FB6E8B536__INCLUDED_)
