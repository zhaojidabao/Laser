#ifndef UserFont_H
#define UserFont_H
#include <afxtempl.h>
#define ASCIINUM 256

struct BaseDot
{
public:
	void operator = (BaseDot &bt)
	{
		x = bt.x;
		y = bt.y;
	};
	BaseDot()
	{
		x = y = 0;
	};
	double x;
	double y;
};

class QRect
{
public:
	QRect()
	{
		left  = right = 0;
		top = bottom  = 0;
	};
	BOOL IsEmpty()
	{
		if (left == right || top == bottom)
			return TRUE;
		return FALSE;
	};
	void operator = (QRect &rt)
	{
		left = rt.left;
		right = rt.right;
		top = rt.top;
		bottom = rt.bottom;
	};
	double left;
	double right;
	double top;
	double bottom;
};

class SK:public CObject
{
public:
	SK(){};
	~SK()
	{
		m_dotArray.RemoveAll();
	};
	void Add(BaseDot bt)
	{
		m_dotArray.Add(bt);
	};
	CArray<BaseDot,BaseDot>m_dotArray;
};

typedef CTypedPtrList<CObList,SK*>SKList;

class CBaseNode
{
public:
	CBaseNode(){};
	~CBaseNode()
	{
		SK *sk = NULL;
		POSITION pos = m_list.GetHeadPosition();
		while( pos )
		{
			sk =  m_list.GetNext(pos);
			if ( sk )
			{
				delete sk;
				sk = NULL;
			}
		}
		m_list.RemoveAll();
	};
	///////
	SKList m_list;
	QRect qt;
};

struct UserFONT
{
	CBaseNode bs;
	double dbLeftDis;//×ó¼ä¾à
	double dbRightDis;//ÓÒ¼ä¾à
	double dbHeight;//×Ö¸ß
	double dbWidth;//×Ö¿í
};

#endif
