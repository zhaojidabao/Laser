// NodeCodebar.h: interface for the CNodeCodebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_)
#define AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Quad.h"
#include "XDC.h"
#include "NodeT.h"

#include <vector>
#include <complex>
#include <list>
using namespace std;

class CNodeCodebar : public CNodeT  
{
	DECLARE_SERIAL(CNodeCodebar);

public:
	CNodeCodebar();
	virtual ~CNodeCodebar();
	virtual void Draw(XDC *pDC);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeCodebar();
		return pNode;
	};

public:
	virtual BOOL PreprocessMark(DWORD nStyle);
	CQuad GetMarkRange();
	
	virtual BOOL Create(CRect &rect);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);

	virtual void Rotate(CDot dot,double angle,int nPlane = 0);
	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY){return TRUE;};
	virtual BOOL Move(double moveX,double moveY,double mvoeZ = 0);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual void Drag(CDot dot, double ratioX, double ratioY,double moveX,double moveY,double moveZ = 0);

	virtual BOOL CreateStrokeList();
	virtual CString GetParitybit(CString markcode){return "";};//�õ�У��λ

	virtual void DrawExtern(XDC *pDC);
	virtual BOOL GetLockFlg();
	virtual void LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	virtual BOOL  CopyToMe(CNode *pSrc,DWORD dwStyle);
	
public:
	virtual void RefreshBarText( CWnd* pWnd, int nTextID );
	BOOL    GenBarCode(UINT nStyle);

	BARTYPE m_BarType;
	
	double	m_dbFillSpace;		//�����
	BOOL		m_bRev;				//ȷ�������Ƿ���
    BOOL		m_bGoBack;		//������
    BOOL		m_bConnect;		//������
	double	m_dbRevWidth;	//��������ʱ�Ŀ��
	double	m_dbHeightRev;	//��������ʱ�ĸ߶�(һά)
	double	m_dbAngle;			//����Ƕ�
	BOOL		m_bAutoFill;		//�Զ����
	BOOL		m_bMatrix;			//�������
	BOOL		m_bParity;			//�Ƿ�����У����

	DWORD	m_nSingleLineMarkCount;
	UINT		m_nNarrowLineNum;

	BYTE*	m_pByte;			//�����������Ϣ
	UINT		m_nCol;				//��������
	UINT		m_nRow;			//��������
	
	double	m_dbDotRadius;	//���С
	double	m_dbshrink;			//����������
	double	m_dbHeightBar;	//����߶�
	int			m_nOutline;			//PDF417��������
	int			m_nErrLev;			//����������QR,DATAMATRIX
	int			m_nHPoint;			//��ֱ��������,����QR,DATAMATRIX
	int			m_nWPoint;			//ˮƽ��������,����DATAMATRIX
	int			m_nSymbolNo;		//DATAMATRIX ����
	int			m_nShapeNo;		//DATAMARTIX ��״
	int			m_nVersion;		//�汾

	int			m_nCodeType;
	BOOL		m_bAutoSet;		//�������볤������;//true �Զ�����;FALSE �û�����
	int			m_nAlign;			//�������ַ��� 1�����;2�м����;3�Ҷ���
	double	m_narrowW;		//������������ʱ,��¼��ǰ����խ�����

	// �οղ���
	BOOL m_bHollow;
	double m_dbHollowWidth;
	double m_dbHollowHeight;

	UINT m_nCodePage;
	
	CDot		m_dotBase;			//�����׼��
	CArray<StrokeList*, StrokeList*> m_CodeBarList;
	
	double	m_dbRatioX;
	double	m_dbRatioY;
	CDot		m_dotDragPt;
	
	BOOL		m_bTextBar;        //�Ƿ���ڹ����ı�����
	
private:
	BOOL  CreateStrokeList3();
	BOOL  CreatePDFStrokeList();
	BOOL  CreateStrokeList2();
	BOOL  CreateStrokeList1();
	void	 InitCodebar();
	
	BOOL   AddToList();
	void	  FillStroke(list<CStroke*> * pList,CStroke *pStroke);
	
	BOOL   ClearBarList();
	BOOL   BetterOneCodeBar();
	BOOL   ConnectOneCodeBar();
	double Distance(CDot &sDot, CDot &eDot);
	BOOL   ProOneStroke(CDot &dt, CStroke *pStrk);
	BOOL   MinDsIsFirstDot(CDot &dt, CStroke *pStrk);

	double m_dbSpace;		//����EAN8��EAN13����˵����ڳ���֮��
									//���ȶ̵ĳ����ټ�Ϊ�˲���,Ŀǰĩ�����л�����
									//���ó��������棬����ԭ����ֵ��0.6MM��
	double m_dbcodeRate;	//��ֵ�̶�Ϊ2.75

	//
	BOOL MakeLineToList(StrokeList *pStrkList, double x, double y0, double y1);
	BOOL MakeLineList(double x, double y0, double y1, int nLineCnt, double dbGoStep);
	BOOL MakeLineList(int nType, double x, int nLineCnt, double dbGoStep, BOOL bLong = FALSE);
	/*nType 0:ʵ�����, 1:�߿���, 2:�߿���� */
};

#endif // !defined(AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_)

