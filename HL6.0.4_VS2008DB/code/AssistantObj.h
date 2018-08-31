#ifndef __ASSISTANTOBJ_AB35950D_7150_4484_8F9F_2C8879CD433B_H
#define __ASSISTANTOBJ_AB35950D_7150_4484_8F9F_2C8879CD433B_H

/**********************************************************************************************
ģ����				:  ��ͼ������
�ļ���				:  AssistantObj.h
����ļ�			:  AssistantObj.cpp
�ļ�ʵ�ֹ���		:  �ṩ��ͼ������(�����˵㣬�������ģ�����, Բ��)
˵��:
	��������˵��:
	1								2				
	-	-	-				-	-	-	-	-
	-	|	-				-	-	|	-	-
	-	-	-				-	|	|	|	-
							-	-	|	-	-
							-	-	-	-	-
-----------------------------------------------------------------------------------------------
��ע           : -
------------------------------------------------------------------------------------------------
�޸ļ�¼ : 
��  ��						�汾				 �޸���				�޸����� 
2012/10/27				1.0				�캣��				 ����
************************************************************************************************/
#define Round(x)		( ((x)>0.0)?((int)(x+0.5)):((int)(x-0.5)) )

enum ASSPOINT_TYPE
{
	ASSPOINT_TYPE_ENDPOINT = 0,			// �߶˵�
	ASSPOINT_TYPE_LINECENTER,			// �����ĵ�
	ASSPOINT_TYPE_CIRCLECENTER,		// Բ��
	ASSPOINT_TYPE_CUTPOINT				// �е�
};

typedef struct AssPoint
{
	double x;				// ��������x
	double y;				// ��������y
	int		nType;	// ����������,ȡֵ����ASSPOINT_TYPE
}ASSPoint;

typedef struct AnchorPoint
{
	int x;						// �߼�����x
	int y;						// �߼�����y
	int nType;				// ����������
	BOOL bEnable;		// ��ǰê����Ч�����ڲ�����,ȡֵ����ASSPOINT_TYPE
}ANCHORPOINT;

//����ʹ�÷���,��������-��SetAssistantPara(����)-��LoadAssistantPoints����������
class CAssistantObj 
{
public:
	CAssistantObj();
	~CAssistantObj();
	void	  SetPoint(double x, double y, int nType);
	void    SetPointsFromStroke( CStroke* pStroke);
	BOOL LookUp(double x, double y, double& dbx, double& dby);
	BOOL	SetAssistantPara(UINT nGridWidth, UINT nGridHeight, int nPrecision);
	BOOL	LoadAssistantPoints(const CArray<DOT,DOT> &pointArray);
	void DrawAnchor(CDC* pDC);

private:
	BOOL InitAssistant();			// ��������ڴ�
	void	FreeAssistant();			// �ͷſռ�

public:
	unsigned int	**m_pGridData;

	CPen pen;  
	AnchorPoint	m_anchorPoint;

private:
	UINT m_nGridWidth;	// ��������Ŀ��
	UINT m_nGridHeight;	// ��������ĸ߶�
	int m_nPrecision;	// �������ȣ����ܱߵ�m_nPrecision*m_nPrecision����ֻ��ȡ1,2,3,4,5mm
};

#endif