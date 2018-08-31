// Layer.h: interface for the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_)
#define AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_

#include "Property.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CURVES_NUM_POINTS 17
#define MAX_QSOFT_CNT	  20

const int nCCurvesLen = sizeof(int) * CURVES_NUM_POINTS * 2;


typedef union LaserPara
{	
	LaserPara()
	{
		memset(this,0,sizeof(LaserPara));
	}
	
	void operator = ( const LaserPara & src )
	{	
		if ( &src == this )	
		{
			return;
		}
		memcpy(this, &src, sizeof(LaserPara));
	}

	EMCCPARA emcc;
	char     szBuff[1024];
}PARA;

class  CLayer : public CObject  
{	
	DECLARE_SERIAL(CLayer)
	
public:
	CLayer();
	CLayer(int nType);
	virtual ~CLayer();
	virtual void Serialize( FILE* fp,BOOL bFlag, int nCardType,BOOL bUnicode);
	CLayer(const CLayer& src);
	void operator = (const CLayer& src);
	
public:
	void UpdatePara();
	CProperty GetProperty();
	
	void SetColor(COLORREF color);
	
	BOOL ReadEmcc(CMySerialize &fs, CString &str);
	
	void WritedEmcc(CMySerialize &fs);
	
	int	 m_nLayer;
	COLORREF m_color;
	BOOL m_bDefault;
	BOOL m_bLockFlag;
	PARA m_para;
};

#endif // !defined(AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_)
