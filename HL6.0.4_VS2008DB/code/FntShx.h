// FntShx.h: interface for the CFntShx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNTSHX_H__9A216A01_FEAF_11D4_A486_00105A615525__INCLUDED_)
#define AFX_FNTSHX_H__9A216A01_FEAF_11D4_A486_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Fnt.h"

const int cad_vec[][2] = {
	{2,0},{2,1},{2,2},{1,2},{0,2},{-1,2},
	{-2,2},{-2,1},{-2,0},{-2,-1},{-2,-2},
	{-1,-2},{0,-2},{1,-2},{2,-2},{2,-1},
};	//every dirction of acad shx code

class CFntShx : public CFnt  
{
	struct ENSHX_HEAD
	{
		char c1[24];
		unsigned __int16 begin_ch;
		unsigned __int16 end_ch;
		unsigned __int16 total_ch;
	};
	
	struct ENSHX_INDEX
	{
		unsigned __int16 ch_asc;
		unsigned __int16 ch_bytes;
	};
	
	struct FONT_INFO
	{
		unsigned __int8 ascent;
		unsigned __int8 descent;
		unsigned __int16 style;
	};
	
	struct BIGSHX_HEAD
	{
		unsigned __int16 number_index;
		unsigned __int16 number_esc;
	};
	
	struct BIGSHX_INDEX
	{
		unsigned __int16 code;	//汉字内码
		unsigned __int16 len;	//长度
		__int32 pos;			//位置
	};
	
	struct UNIFONT_HEAD
	{
		char c1[25];		
	};
	
	struct UNIFONT_FONTNUM
	{
		unsigned __int32 num_unifont;
	};

	struct UNIFONT_FONTINDEXLEN
	{
		unsigned __int16 len_unifont;
	};

	struct UNIFONT_CODE
	{
		unsigned __int16 code;
	};
	
	struct UNIFONT_CODELEN
	{
		unsigned __int16 codelen;
	};
	//add end

	DECLARE_SERIAL(CFntShx)
	
public:
	CFntShx();
	virtual ~CFntShx();
	virtual void Serialize( CArchive& ar );
	
public:
	CString m_strChineseFont;	//中文字体
	CString m_strEnglishFont;	//英文字体
	CString m_strNumberFont;	//数字字体
	virtual BOOL MakeStroke();
	BOOL MakeFixStroke();		//固定间距
	BOOL MakeOldStroke();		//老方式，不等间距
	
	void processUnifont(unsigned __int8 letter);
	BOOL m_bEnglIshIsUnifont;
	BOOL m_bNumIsUnifont;
	
	int m_nIndexNum;
	
	UNIFONT_HEAD m_UnifontHead;
	UNIFONT_FONTNUM m_UnifontNum;
	UNIFONT_FONTINDEXLEN m_UnifontIndexLen;

	UNIFONT_CODE m_UnifontCode;
	UNIFONT_CODELEN m_UnifontCodelen;

protected:
	double m_dbUnitEnglish;
	double m_dbUnitNumber;
	double m_dbUnitEnshx;
	double m_dbUnitBigshx;

	CStroke *m_pStrokeCur;		//当前的stroke

	CDot m_dotCur;			//笔当前停留位置
	CDot m_dotPrev;			//笔上一位置
	ENSHX_HEAD m_EnshxHeadEnglish;
	ENSHX_HEAD m_EnshxHeadNumber;
	ENSHX_HEAD m_EnshxHead;

	ENSHX_INDEX * m_pIndexEnglish;
	ENSHX_INDEX * m_pIndexNumber;
	ENSHX_INDEX * m_pIndexEnshx;

	BIGSHX_HEAD m_BigshxHead;
	DWORD m_posBigshxIndexStart;

	CFile *m_pFileChinese;
	CFile *m_pFileNumber;
	CFile *m_pFileEnglish;
	CFile *m_pFile;
	CFile *m_pFileNum;

	void process(unsigned __int8 letter);	//英文或数字的处理
	void process(unsigned __int16 letter);	//中文或大字符的处理

	void CreateArc(CDot dotCenter,double r,double angleStart,double angle); // 第四个参数表示多少度
	void CreateLine(CDot dot1,CDot dot2);
	void CreateLine();
	__int8 decode(unsigned __int8 code);

	double	m_ratio;
	BOOL	m_bDraw;
	BOOL	m_bVert;
	BOOL m_bFirst;

	//定义堆栈
	double m_stack[10][2];
	int m_ps;
};

#endif // !defined(AFX_FNTSHX_H__9A216A01_FEAF_11D4_A486_00105A615525__INCLUDED_)
