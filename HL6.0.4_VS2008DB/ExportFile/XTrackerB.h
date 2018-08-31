// XTrackerB.h: interface for the XTrackerB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTRACKERB_H__5E712833_FD16_48EA_B5C2_04F6539B5E9A__INCLUDED_)
#define AFX_XTRACKERB_H__5E712833_FD16_48EA_B5C2_04F6539B5E9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\ExportFile\Dot.h"
#include "..\ExportFile\StrokeB.h"

class AFX_EXT_CLASS XTrackerB  
{
public:
	class DotSelectList  
	{
	public:
		DotSelectList()
		{
			m_pNext=m_pFront=NULL;
			m_Node.x=m_Node.y=0;
			m_nNum=-1;
			m_pLeft=m_pRight=NULL;
			bLeft=bRight=TRUE;
			bLeftDraw=bRightDraw=FALSE;
		}
		~DotSelectList()
		{
		};
		
	public:
		CDot m_Node;
		DotSelectList *m_pNext; //前一个指针
		DotSelectList *m_pFront;//后一个指针
		CDot *m_pLeft;          //左结点指针
		CDot *m_pRight;         //右结点指针
		BOOL bLeft;             //左结点是否为空
		BOOL bRight;            //右结点是否为空
		BOOL bLeftDraw;         //是否向左边画线
		BOOL bRightDraw;        //是否向右边画线
		int  m_nNum;            //点的号码,用于点的定位，编号从0开始
	};
	
	class DotHead
	{
	public:
		DotHead()
		{
			m_pAdd   = NULL;
			m_pHead  = NULL;
			m_pNext  = NULL;
			m_pTail  = NULL;
			m_nCount = 0;
		}
		~DotHead()
		{
		}

	public:
		void InverDrawDot(CWnd *pWnd);//反画
		void AddSelectDot(CDot &dot,CDot &left,CDot &right,int nNum,BOOL bLeft,BOOL bRight)//构作双向链表
		{
			DotSelectList *pAdd = new DotSelectList;
			pAdd->m_Node        = dot;
			
			pAdd->m_pLeft       = new CDot;
			pAdd->m_pRight      = new CDot;
			*(pAdd->m_pLeft)    = left;
			*(pAdd->m_pRight)   = right;

			pAdd->bLeftDraw     = bLeft;
			pAdd->bRightDraw    = bRight;
			pAdd->m_nNum        = nNum;
			
			if(m_pHead==NULL)
			{
				m_pHead=pAdd;
				m_pTail=pAdd;
			}
			else
			{
				m_pTail->m_pNext=pAdd;
				pAdd->m_pFront=m_pTail;
				m_pTail=pAdd;
			}
		}

		void DelSelectDot()//删除一支点
		{
			DotSelectList *plist=m_pHead;
			while(plist)
			{
				m_pHead=m_pHead->m_pNext;
				if(plist->bLeft)
				{
					delete plist->m_pLeft;
					plist->bLeft=FALSE;
				}
				if(plist->bRight)
				{
					delete plist->m_pRight;
					plist->bRight=FALSE;
				}
				delete plist;
				plist=m_pHead;
			}
			m_pTail=NULL;
		}

		void DataExChange()//排序，至少要有两个数据才进行交换
		{
			if(m_pTail==m_pHead)
			{
				return;
			}
			
			DotSelectList *pList=m_pHead;
			DotSelectList *pTail=pList->m_pNext;
			int i,j;
			while(pTail)//从前往后
			{
				i=pList->m_nNum;
				j=pTail->m_nNum;
				if(i+1==j)
				{
					delete pList->m_pRight;
					pList->bRight=FALSE;
					pList->m_pRight=&pTail->m_Node;
				}
				pList=pList->m_pNext;
				pTail=pTail->m_pNext;
			}
			pTail=pList;
			i=pList->m_nNum;
			j=m_pHead->m_nNum;
			if((i+1)%m_nCount==j)
			{
				delete pList->m_pRight;
				pList->bRight=FALSE;
				pList->m_pRight=&m_pHead->m_Node;
			}
			
			//从后往前
			pList=pTail->m_pFront;
			while(pList)
			{
				i=pTail->m_nNum;
				j=pList->m_nNum;
				if(i-1==j)
				{
					delete pTail->m_pLeft;
					pTail->bLeft=FALSE;
					pTail->m_pLeft=&pList->m_Node;
				}
				pTail=pTail->m_pFront;
				pList=pList->m_pFront;
			}

			i=pTail->m_nNum;
			j=m_pTail->m_nNum;
			if((j+1)%m_nCount==i)
			{
				delete pTail->m_pLeft;
				pTail->bLeft=FALSE;
				pTail->m_pLeft=&m_pTail->m_Node;
			}
		}
		
		void DeleteDot(CDot &dot)//删除结点的同时修改号码
		{
			if(m_pHead==NULL)	
				return;
			DotSelectList *plist=m_pHead;
			while(plist)
			{
				if(plist->m_Node==dot)
					break;
				plist=plist->m_pNext;
			}
			if(plist==m_pHead)
			{
				m_pHead=plist->m_pNext;
			}
			if(plist==m_pTail)
			{
				m_pTail=plist->m_pFront;
			}
			if(plist->m_pFront)
				plist->m_pFront->m_pNext=plist->m_pNext;
			if(plist->m_pNext)
				plist->m_pNext->m_pFront=plist->m_pFront;
			if(plist->bLeft)
				delete plist->m_pLeft;
			if(plist->bRight)
				delete plist->m_pRight;
			delete plist;
			plist=m_pHead;
			while(plist)
			{
				plist->m_nNum--;
				plist=plist->m_pNext;
			}
		}
		
		void operator =(DotHead &head)
		{
			m_pAdd=head.m_pAdd;
			m_pHead=head.m_pHead;
			m_pNext=head.m_pNext;
		}

		DotSelectList *m_pHead;
		DotSelectList *m_pTail;
		CStrokeB *m_pAdd;//Stroke的地址,动态修改
		DotHead *m_pNext;

		int m_nCount;
	};

public:
	XTrackerB();
	virtual ~XTrackerB();
		
public:
	// 查找选中列表中的第一个点
	int FindSelListFirstPt(CDot &dot);
	// 增加指定Stroke到可编辑列表中
	void AddStrokeToEditList(CStrokeB *pStroke);

	BOOL Track(CPoint &point,CWnd *pWnd);
	void LastBrush(CWnd *pWnd,CPoint &ptf,CPoint &ptl);
	void DrawRect(CWnd *pWnd,CPoint &ptf,CPoint &ptl,CPoint &point);
	BOOL IsFillStyle(CStrokeB *pStroke);
	BOOL IsDiffStroke();
	int  HeadOrTail(DotHead *pHead);
	BOOL DotListSub(DotHead **plist,int &nCount);
	void DataEXChange();
	double Distance(CDot &sDot, CDot &eDot);
	void ReplaceDot();
	void AddDotHeadB(CStrokeB *pStroke,CQuadB &quad);
	void ClearDot();
	BOOL ListEmpty();
	void AddDotToHead(CStrokeB *pStroke,CDot &dotIndex, double dbdata);
	BOOL HitSelectDotB(CDot &dotTest, double dbdata);
	
public:
	DotHead *m_pDotHead;
	DotHead *m_pDotTail;
	CPoint m_ptMeasureLine;
	CPoint m_ptPrev;
	CPoint m_ptNow;
	double m_HITBORDER;
	CPoint m_ptDown;
	
	//当选中矩形中心控制点时，记录矩形中心的物理位置
	CDot m_dtQuadCenter;
	CRect m_rect;
	CRect m_rectOut;

};

#endif // !defined(AFX_XTRACKERB_H__5E712833_FD16_48EA_B5C2_04F6539B5E9A__INCLUDED_)
