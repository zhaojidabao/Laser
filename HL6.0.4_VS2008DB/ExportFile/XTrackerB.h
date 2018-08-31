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
		DotSelectList *m_pNext; //ǰһ��ָ��
		DotSelectList *m_pFront;//��һ��ָ��
		CDot *m_pLeft;          //����ָ��
		CDot *m_pRight;         //�ҽ��ָ��
		BOOL bLeft;             //�����Ƿ�Ϊ��
		BOOL bRight;            //�ҽ���Ƿ�Ϊ��
		BOOL bLeftDraw;         //�Ƿ�����߻���
		BOOL bRightDraw;        //�Ƿ����ұ߻���
		int  m_nNum;            //��ĺ���,���ڵ�Ķ�λ����Ŵ�0��ʼ
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
		void InverDrawDot(CWnd *pWnd);//����
		void AddSelectDot(CDot &dot,CDot &left,CDot &right,int nNum,BOOL bLeft,BOOL bRight)//����˫������
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

		void DelSelectDot()//ɾ��һ֧��
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

		void DataExChange()//��������Ҫ���������ݲŽ��н���
		{
			if(m_pTail==m_pHead)
			{
				return;
			}
			
			DotSelectList *pList=m_pHead;
			DotSelectList *pTail=pList->m_pNext;
			int i,j;
			while(pTail)//��ǰ����
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
			
			//�Ӻ���ǰ
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
		
		void DeleteDot(CDot &dot)//ɾ������ͬʱ�޸ĺ���
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
		CStrokeB *m_pAdd;//Stroke�ĵ�ַ,��̬�޸�
		DotHead *m_pNext;

		int m_nCount;
	};

public:
	XTrackerB();
	virtual ~XTrackerB();
		
public:
	// ����ѡ���б��еĵ�һ����
	int FindSelListFirstPt(CDot &dot);
	// ����ָ��Stroke���ɱ༭�б���
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
	
	//��ѡ�о������Ŀ��Ƶ�ʱ����¼�������ĵ�����λ��
	CDot m_dtQuadCenter;
	CRect m_rect;
	CRect m_rectOut;

};

#endif // !defined(AFX_XTRACKERB_H__5E712833_FD16_48EA_B5C2_04F6539B5E9A__INCLUDED_)
