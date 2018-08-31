// Lay.cpp: implementation of the CLay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLay,CObject,2000)

CLay::CLay()
{
	m_nCur = 0;
	m_nNext = 0;
}

CLay::~CLay()
{

}
CLay::CLay(const CLay& src)
{  	
	m_list.RemoveAll();

	int count = src.m_list.GetSize();
	for(int i = 0;i<count;i++)
	{
		CLayer ly;
		ly = src.m_list[i];
		m_list.Add(ly);
	}
	m_nCur = src.m_nCur;
	m_nNext = src.m_nNext;
}

void CLay::operator = (const CLay& src)
{
	m_list.RemoveAll();

	int count = src.m_list.GetSize();
	for(int i = 0;i<count;i++)
	{
		CLayer ly;
		ly = src.m_list[i];
		m_list.Add(ly);
	}
	m_nCur = src.m_nCur;
	m_nNext = src.m_nNext;

}

void CLay::Add(CLayer &ly)
{
	m_list.Add(ly);
	m_nNext++;
}

void CLay::SetCur(int cur)
{
	m_nCur = cur;
}

void CLay::AddNew()
{
	CLayer ly;
	ly.m_nLayer = m_nNext;
	ly.m_para = m_list[0].m_para;

	Add(ly);
}

void CLay::Del(int nLayer)
{
    int count = m_list.GetSize();
	for( int i = 0; i < count; i++ )
	{
		if (m_list.GetAt(i).m_nLayer == nLayer)
		{
			m_list.RemoveAt(i);
			int n = m_list.GetSize();
			if ( m_nCur > ( n - 1 ) )
				m_nCur = n-1;

			break;
		}
	}
}

int CLay::GetSize()
{
	return m_list.GetSize();
}

void CLay::Set(int mode, CLayer &ly)
{
	//改变层定义
	int count = m_list.GetSize();
	for(int i = 0;i<count;i++)
	{
		if (m_list[i].m_nLayer == ly.m_nLayer)
		{
			m_list[i] = ly;
			break;
		}
	}

	if ( mode = PTY_PARA )
	{
		//打标参数改变时，第0层打标参数对其它层的影响
		if ( ly.m_nLayer == 0 )
		{
			int count = m_list.GetSize();
			for ( int i = 1; i < count;i++ )
			{
				CLayer &tmp = m_list[i];
				if ( tmp.m_bDefault )
				{
					tmp.m_para = ly.m_para;
				}
			}
		}
	}
}


int CLay::GetCur()
{
	return m_nCur;
}

void CLay::Serialize(FILE* fp,BOOL bFlag, int nCardType,BOOL bUnicode)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp,bUnicode);
	
	if ( bFlag )
	{
		//写入头信息
		fs << _T("L_B"); 
		
		int nCount = m_list.GetSize();
		for(int i = 0; i < nCount; i++)
		{
			CLayer ly;
			ly = m_list.GetAt(i);
			ly.Serialize(fp,bFlag, nCardType,bUnicode);
		}
		
		//写入尾信息
		fs << _T("L_E");
	}
	else
	{
		CString str;
		while( fs >> str )
		{
			str.TrimRight();
			if ( str == _T("B_Y") )
			{
				CLayer ly;
				ly.Serialize(fp,bFlag, nCardType,bUnicode);

				if ( ly.m_nLayer  >=  0 )
					Add(ly);
			}
			if ( str == _T("L_E"))
				break;
		}
	}
}

CLayer CLay::GetCurLayer()
{
	int nCount = m_list.GetSize();
	if ( m_nCur  >=  nCount ) 
	{
		m_nCur = 0;
	}

	return m_list[m_nCur];
}

CLayer CLay::GetLayer(int index)
{
	int n = m_list.GetSize();
	if ( (index  >=  n) || (index  <=  0) )
	{
		index = 0;
	}

	return m_list[index];
}

void CLay::SetLayer(CLayer &layer)
{
	if ( layer.m_nLayer  >=  m_list.GetSize() )
		return;
	
	int nIndex       = layer.m_nLayer;
	m_list[nIndex] = layer;
}

void CLay::SetLayerColor(int index, COLORREF color)
{
	if ( index  >=  m_list.GetSize() )
		index = 0;
	
	m_list[index].m_color = color;
}

CLayer CLay::GetLayer(COLORREF color)
{
	int count = m_list.GetSize();
	for( int i = 0; i < count; i++ )
	{
		if ( m_list[i].m_color == color )
		{
			return m_list[i];
		}
	}

	return m_list[0];
}

CLayer CLay::GetLayer(CProperty pty)
{
	int count = m_list.GetSize();
	for ( int i = 0; i < count; i++ )
	{
		if ( m_list[i].m_nLayer == pty.m_nLayer )
		{
			return m_list[i];
		}
	}
	return m_list[0];
}

int CLay::GetLayerNum(CProperty pty)
{
	int count = m_list.GetSize();
	for ( int i = 0; i < count; i++ )
	{
		if ( m_list[i].m_nLayer == pty.m_nLayer )
		{
			return i;
		}
	}
	return 0;
}

int CLay::GetRepeat(int nLay)
{
	int  repeat = 0;
	int  nCount = m_list.GetSize();
	for( int i = 0; i<nCount; i++ )
	{
		if ( m_list.GetAt(i).m_nLayer == nLay )
		{
			repeat = m_list.GetAt(i).m_para.emcc.nCount;
			break;
		}
	}

	return repeat;
}


PARA CLay::GetPara(int nLay)
{
	int count = m_list.GetSize();
	for( int i = 0; i < count; i++ )
	{
		CLayer layer = m_list.GetAt(i);
		if ( layer.m_nLayer == nLay )
		{
			return  layer.m_para;
		}
	}

	return m_list[0].m_para;
}

BOOL CLay::GetLockFlg(int index)
{
	if ( index  >=  m_list.GetSize() )
		index = 0;

    return m_list[index].m_bLockFlag;
}

void CLay::SetLockFlg(int index, BOOL bLockFlg)
{
	if ( index >= m_list.GetSize() )
		index = 0;

    m_list[index].m_bLockFlag = bLockFlg;
}