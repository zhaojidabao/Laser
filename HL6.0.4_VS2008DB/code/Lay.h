// Lay.h: interface for the CLay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAY_H__F86EEAE1_863E_11D4_A484_00105A615525__INCLUDED_)
#define AFX_LAY_H__F86EEAE1_863E_11D4_A484_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Layer.h"

class CLay: public CObject  //Lay>Layer
{
	DECLARE_SERIAL(CLay)
  
public:
	CLayer GetLayer(COLORREF color);

	int GetRepeat(int nLay);
	CLayer GetCurLayer();

	CLayer GetLayer(int index);
	void   SetLayer(CLayer &layer);

	CLayer GetLayer(CProperty pty);
	int   GetSize();
	int   GetCur();
	void SetCur(int cur);

	void SetLayerColor(int index, COLORREF color);

	//Updated by AndyF.Shelby on 2 Mar. 2006 for SleepMode
	CLayer& GetDefaultLayer();

	void Set(int mode,CLayer &ly);
	void Del(int nLayer);
	void AddNew();
	void Add(CLayer &ly);
	void Serialize(FILE* fp,BOOL bFlag, int nCardType,BOOL bUnicode);

	BOOL GetLockFlg(int index);
	void SetLockFlg(int index, BOOL bLockFlg);

	CLay();
	virtual ~CLay();
	void operator = (const CLay& src);
	CLay(const CLay& src);

protected:
	CArray<CLayer,CLayer>m_list;		//���б�
	int m_nNext;						//����ʱ����һ���
	int m_nCur;							//��ǰ��

	//cyadd
public:
	int GetLayerNum(CProperty pty);
	PARA GetPara(int nLay);
};

#endif // !defined(AFX_LAY_H__F86EEAE1_863E_11D4_A484_00105A615525__INCLUDED_)
