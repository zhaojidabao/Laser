// RedMarkThread.h: interface for the RedMarkThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REDMARKTHREAD_H__7934462C_1C1B_4D0D_BA49_5437A14DCCD1__INCLUDED_)
#define AFX_REDMARKTHREAD_H__7934462C_1C1B_4D0D_BA49_5437A14DCCD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HLView.h"

extern CRITICAL_SECTION g_LockReadMark;
class RmLock
{
public:
	RmLock(){EnterCriticalSection(&g_LockReadMark);};
	~RmLock(){LeaveCriticalSection(&g_LockReadMark);};
};


class CRedMarkThread  
{
public:
	CRedMarkThread(); 
	virtual ~CRedMarkThread(); 

	UINT StartPreview(BOOL bIsRed = TRUE, BOOL bIsArray = FALSE);	//�����߿�Ԥ��
	UINT StopPreview();		//ֹͣ�߿�Ԥ�� //�ر��߳�
	void SetQuadArray(CArray<SELQUAD ,SELQUAD> &QuadList); //�ڴ˼���ͬ��
	void SetMarkStrk(CArray<ARRAYMARK, ARRAYMARK> &ArrayMark); //�ڴ˼���ͬ��

	DWORD GetMarkData(BOOL bFirstTouch = FALSE);
	DWORD GetArrayMarkData();

	static UINT PreviewThread(LPVOID pParam);				// Ԥ������߳�
    static UINT ArrayMarkThread(LPVOID pParam);			// ����Ԥ������߳�

private:
	CArray<SELQUAD ,SELQUAD> m_QuadList; //Ҫ�߿�Ԥ��������	
	CArray<SELQUAD ,SELQUAD> m_QuadMarkArray; //��������еı߿�Ԥ������
	HANDLE m_hEventRedPreview; //�߿�Ԥ��֪ͨ
	HANDLE m_hEventThreadExit; //�߿�Ԥ���߳��˳�
	HANDLE m_hEventUpdateArray; //�����Ҫ���������
	HANDLE m_hRedThread;	//���Ԥ���߳̾��

	BOOL   m_bIsRed;
	BOOL   m_bIsArray;

	CArray<ARRAYMARK, ARRAYMARK> m_PosArray;
	CArray<ARRAYMARK, ARRAYMARK> m_MarkArray;
	
};

#endif // !defined(AFX_REDMARKTHREAD_H__7934462C_1C1B_4D0D_BA49_5437A14DCCD1__INCLUDED_)
