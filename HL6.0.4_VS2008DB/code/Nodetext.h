// NodeText.h: interface for the CNodeText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_)
#define AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeT.h"
#include "Dot.h"
#include "Fnt.h"
#include "Tree.h"
#include "Quad.h"
#include "NodeT.h"

#include <vector>
#include <complex>
#include <list>

class CNodeText : public CNodeT
{	
	DECLARE_SERIAL(CNodeText);

public:
	virtual void  AddToPlt(CNode *pNodePlt);
	virtual BOOL Click(CPoint &point,CWnd *pWnd);
	virtual void SetProperty(int mode,CProperty pty, XDC *pDc = NULL);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual BOOL Move(double moveX,double moveY,double moveZ = 0);
	virtual void	 Rotate(CDot dot, double angle, int nPlane = 0);
	virtual void	 Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY,double moveZ = 0);
	virtual BOOL PreprocessMark(DWORD nStyle);
	virtual UINT	 Mark(HWND hWnd);
	virtual UINT SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark = FALSE);
	virtual CQuad CalLimit();
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Draw(XDC *pDC);
	virtual BOOL CopyToMe(CNode *pSrc,DWORD dwStyle);//����ȫ�����ݣ����dwStyle = TRUE���½���ID
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	virtual BOOL Create(CRect &rect);
	virtual void Scan(StrokeList *pList);		
	virtual BOOL CanGroup();
	virtual CNode *CopyStrokeOnly();
	virtual CQuad  GetMarkRange();
	virtual void DrawExtern(XDC *pDC);
	virtual BOOL Shear(double dbBaseX, double dbBaseY, double angleX, double angleY);
	virtual void RefreshBarText(CWnd* pWnd,int nTextID);
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill);
	virtual void SetPathOptimizeState(BOOL bState);
	//bRefΪTRUE��ֻȡ������ݷ�Χ(��������Բ���Ų��е�Բ��),FALSEȡ����Բ���ķ�Χ
	virtual  CQuad	 GetLimit(BOOL bRef = FALSE);
	//
	BOOL GetFrameDots(CArray<CDot,CDot>& frameDots);
	void FillStroke(list<CStroke*> * pList,CStroke *pStroke);
	
	CNodeText();	
	virtual ~CNodeText();
	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeText();
		return pNode;
	};
	
public:
	virtual void SetStrokeList(StrokeList *pStrokeList);
	virtual void LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo);
	virtual void LoadWheelList(int nType, StrokeList *pListStroke);
	virtual BOOL GetLockFlg();
	virtual void  Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);

	CDot		m_dotBase;
	CDot		m_dotLT;
	CDot		m_dotRB;

	//�����ı�
	int		m_nAlign;		//���뷽ʽ
	int		m_nPath;		//���з�ʽ���绡�Σ�ֱ��
	double	m_dbDirection;	//ֱ�߷�ʽ�µķ���
	double	m_dbRadius;		//���η�ʽ�µİ뾶
	BOOL	m_bCCW;			//���η�ʽ�µ�ʱ�뷽��
	double	m_dbStart;		//���η�ʽ�µĿ�ʼ�Ƕ�
	BOOL	m_bInside;		//���η�ʽ�µ��ַ�����
	BOOL	m_bPrintInverse;//ӡ�¿�ӡ
	
	BOOL		m_bCustomFormat;	//�Ƿ����ָ���ı�����
	CString	m_strFormat;		//���ָ���ı����ݵĸ�ʽ
	CString	 m_strPrecisK;		//���徫������
	
	double	m_dbCharSpace;	//�ּ��
	double	m_dbWidthFactor;//���ϵ��
	double	m_dbHeight;		//�ָ߶�
	double	m_dbLineSpace;	//�м��
	BOOL		m_bFixSpace;//�ȼ���ַ�����
	double	m_dbFixSpace;	//�ȼ���ַ�

	// add by zhq,���������Զ����Ӻ͹��˽���ѡ��
	BOOL		m_bWeld;
	BOOL		m_bInterFilter;
	double	m_dbInterSpace;

	// �Զ��Ӵ�
	BOOL m_bBroaden;
	double m_dbLineWidth;
	double m_dbBroadenD;

	int			m_nFont;			//���巽ʽ
	CString	m_strFontCHS;	//SHX��������
	CString	m_strFontEN;		//SHXӢ������
	CString	m_strFontNUM;	//SHX��������
	LOGFONT m_lf;				//TTF����
	CString	m_strFontUser;	//�Զ�������
	CString	m_strHzkType;		//��ǰѡ�еĵ����ֿ�����
	CString	m_strAscType;	//��ǰѡ�еĵ���Ӣ���ֿ�
	
	CFnt   *	m_pFnt;
	//
	int			m_nBarType;;		//��������
	BOOL		m_bParity;			//�����Ƿ��У��
	BOOL		m_bBarText;		//�Ƿ�Ϊ�����ı�
	CString	m_strBarString;	//��������
	int			m_nErrLev;			//������
	int			m_nSize;			//�����С
	int			m_nCodeType;		//������뷽ʽ

	//
	double m_dbMatrixR;
	double m_dbMatrixFillD;
	BOOL  ProcessMatrixFont(StrokeList& listStroke);

	BOOL  CreateString(BOOL bNext);
	void	 CreateFnt();
	void	 CaseNum(CString &str);	//�����еĳ������������Ϣȥ��
	BOOL  GenBarString();				//��ʽ�ı�
	
public:
	void	CreateFromBar(UINT nText);
	void	CreateFromBar(CUIntArray& ArrayTextID);
	void	CopyProperty(CNodeText* pSrc);
	
	BOOL	 GetNodeFirstDot(CDot &dt);
	BOOL	 GetNodeLastDot(CDot &dt);
	BOOL	 AdvanceSet(CRect &rect);

private:
	CQuadRangle m_quadChar;	

	void FormatString(CString& strText);
	void CalcRatio(CQuadRangle q1, CQuadRangle q2, double& rx, double& ry);
};

#endif // !defined(AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_)
