// GrayMgr.h: interface for the CGrayMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAYMGR_H__10FDD111_883E_4AB7_8DEE_E0A3D7E22B6B__INCLUDED_)
#define AFX_GRAYMGR_H__10FDD111_883E_4AB7_8DEE_E0A3D7E22B6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MarkMgr.h"
#include "..\DllInterface\GrayApiDefine.h"

class CGrayMgr : public CMarkMgr
{
	class CNodeMgr : public CNodeV
	{
		public:
			CNodeMgr();
			~CNodeMgr();
			void SetBmpCfg(BMPSPEC* pCfg);
			void AddDataLen(int nDot);
			void AddDataBlock(DOT6* pData);
			virtual UINT Mark (HWND hWnd);
			
		private:
			BMPSPEC m_bmpSpec;
			CArray<DOT6*, DOT6*> m_arrpData;
			CArray<int, int> m_arrDataLen;
	};

public:
	CGrayMgr();
	virtual ~CGrayMgr();
	void Run();

	static BOOL CallFunction(UINT eFunc, LPVOID pPara); 
	
	static CNodeMgr* m_pNode;
};

#endif // !defined(AFX_GRAYMGR_H__10FDD111_883E_4AB7_8DEE_E0A3D7E22B6B__INCLUDED_)
