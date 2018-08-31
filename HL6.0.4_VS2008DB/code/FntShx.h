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
		unsigned __int16 code;	//��������
		unsigned __int16 len;	//����
		__int32 pos;			//λ��
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
	CString m_strChineseFont;	//��������
	CString m_strEnglishFont;	//Ӣ������
	CString m_strNumberFont;	//��������
	virtual BOOL MakeStroke();
	BOOL MakeFixStroke();		//�̶����
	BOOL MakeOldStroke();		//�Ϸ�ʽ�����ȼ��
	
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

	CStroke *m_pStrokeCur;		//��ǰ��stroke

	CDot m_dotCur;			//�ʵ�ǰͣ��λ��
	CDot m_dotPrev;			//����һλ��
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

	void process(unsigned __int8 letter);	//Ӣ�Ļ����ֵĴ���
	void process(unsigned __int16 letter);	//���Ļ���ַ��Ĵ���

	void CreateArc(CDot dotCenter,double r,double angleStart,double angle); // ���ĸ�������ʾ���ٶ�
	void CreateLine(CDot dot1,CDot dot2);
	void CreateLine();
	__int8 decode(unsigned __int8 code);

	double	m_ratio;
	BOOL	m_bDraw;
	BOOL	m_bVert;
	BOOL m_bFirst;

	//�����ջ
	double m_stack[10][2];
	int m_ps;
};

#endif // !defined(AFX_FNTSHX_H__9A216A01_FEAF_11D4_A486_00105A615525__INCLUDED_)
