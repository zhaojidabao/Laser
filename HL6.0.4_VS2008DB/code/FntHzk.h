// FntHzk.h: interface for the CFntHzk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNTHZK_H__8096AEF0_EF47_4F8E_ABD9_304C971C269C__INCLUDED_)
#define AFX_FNTHZK_H__8096AEF0_EF47_4F8E_ABD9_304C971C269C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"
#include "Fnt.h"
#include "IndexFile.h"

#define	FONT_HZK_12		_T("12*12")
#define	FONT_HZK_14		_T("14*14")
#define	FONT_HZK_16		_T("16*16")
#define	FONT_ASC_5_7	_T("5*7")
#define	FONT_ASC_8_16	_T("8*16")

class CFntHzk  : public CFnt
{
	DECLARE_SERIAL(CFntHzk)

public:
	void SortDotList_V();
	//void Delete();
	CFntHzk();
	virtual ~CFntHzk();
	virtual void Serialize( CArchive& ar );
	virtual BOOL MakeStroke();
	CString	m_strHzkType;		
	CString	m_strAscType;		

private:
	BOOL OpenHzk();
	int	 PutHz(int x, int y, int z, char *p);
	int  GetBit(unsigned char c, int n);
	void AddtoDotList(double x, double y, BOOL bRollBack = TRUE);
	void SaveInitDotList(double x, double y);

	void ChangeWorkDirect(int nDir);

	int		m_Handle;
	int		m_HandleASC;
	BOOL	m_bFirst;
	CStroke *m_pStrokeCur;		//当前的stroke
	int		m_HZKtype;

	StrokeList	m_InitDotList;	//保存初始生成的点列表
	int			m_curWorkDir;	//当前工作台方向
	int			m_curFlyDir;	//当前飞行方向

	int			m_DotLeft, m_DotRight;
	int			m_DotTop;
	int			m_DotBottom;
	BOOL		m_bSaveFirst;

	//自造点阵字库
	CUserFontFile *m_pUserFile;
	USERFONTFILE* GetDataOfChar(WORD c);

};

#endif // !defined(AFX_FNTHZK_H__8096AEF0_EF47_4F8E_ABD9_304C971C269C__INCLUDED_)
