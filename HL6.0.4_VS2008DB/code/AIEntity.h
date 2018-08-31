// Entity.h descripe point,line,bezier in ai file
#ifndef AFX_ENTITY_E2C7663F_DCFA_45bc_B0ED_473D61B4B716
#define AFX_ENTITY_E2C7663F_DCFA_45bc_B0ED_473D61B4B716

#include <windows.h>

// AI�е�ͼԪ
#define	AI_LINE		0		//	ֱ��
#define	AI_BEZIER	1		// bezier����
#define	AI_TEXT		2		// �ı�
#define AI_IMAGE	3		// λͼ

// �ļ����뷵��ֵ
#define DECODE_SUCCESS		0	// �ɹ�
#define DECODE_PARA			1	// �������
#define DECODE_FILEERROR	2	// �ļ���ʽ����
#define DECODE_VERSION		3	// �ļ��Ƿ�(��8.0�����°汾)
#define DECODE_ALLOCMEMORY	4	// �ڴ����ʧ��

class CCurve;
class CAIPath;
class CAILayer;

typedef CTypedPtrList<CObList,CCurve*>		AICurveList;
typedef CTypedPtrList<CObList,CAIPath*>		AIPathList;
typedef CTypedPtrList<CObList,CAILayer*>	AILayerList;

// �����������
struct DBPT
{
	double x;
	double y;
	DBPT()
	{
		x = 0;
		y = 0;
	}
	DBPT(double x,double y)
	{
		this->x = x;
		this->y = y;
	}
	void operator=(const DBPT& src)
	{
		x = src.x;
		y = src.y;
	}

	void operator*(double ratio)
	{
		x *= ratio;
		y *= ratio;
	}
};

// AI�ļ�ͼ�����
struct DBRect
{
	double left;
	double bottom;
	double right;
	double top;
	DBRect()
	{
		left = 0;
		bottom = 300;
		right = 300;
		top = 0;
	}
	void operator=(const DBRect& src)
	{
		left = src.left;
		bottom = src.bottom;
		right = src.right;
		top = src.top;
	}

	double Width()
	{
		return right - left;
	}

	double Height()
	{
		return top - bottom;
	}

	DBPT Center()
	{
		return DBPT(left + Width()/2,bottom + Height()/2);
	}
};


class  CCurve : public CObject 
{
public:
	CCurve(){	m_nCurveType = 0;}
	virtual ~CCurve(){ m_dtArray.RemoveAll(); };
	int GetType();
	virtual void CreateCurve(double dbPrecision = 0.1){};
	void GetArrayDot(CArray<DBPT,DBPT>& dtArray);

protected:
	int m_nCurveType;
	// �����ϸ�����������
	CArray<DBPT,DBPT> m_dtArray;	
};


// ����ֱ��
class  CCurve_Line : public CCurve
{
public:
	CCurve_Line();
	~CCurve_Line(){ m_dtArray.RemoveAll(); }
	CCurve_Line(DBPT& pt0,DBPT& pt1);
	CCurve_Line(double x0,double y0, double x1, double y1);
	void operator=(const CCurve_Line& src);
	void CreateCurve(double dbPrecision = 0.1);
public:
	DBPT m_pt0;
	DBPT m_pt1;
};

// ����Bezier����
class  CCurve_Bezier :  public CCurve
{
public:
	CCurve_Bezier();
	~CCurve_Bezier();
	CCurve_Bezier(DBPT* pt);
	void operator=(const CCurve_Bezier& src);
	void SetCtrlPoint(DBPT& pt,int index);
	void CreateCurve(double dbPrecision = 0.1);
	void ClearCtrlPoint();

protected:
	// ��Ҫ�㷨���������߸���������
	void CalCurvePoint(float t, DBPT& stPt);
	// ����Ϳ��Ƶ�����
	DBPT m_vecCtrlPt[4];	
};

// AI�е�·��,�������(CCurve:Line��Bezier),�൱��StrokeList
class  CAIPath : public CObject 
{
public:
	CAIPath()
	{ 
		m_bCLosed = FALSE; 
		m_bFilled = FALSE; 
		m_wireColor= m_fillColor =RGB(0,0,0); 
		m_dbStrokeWidth = 1;
		m_bWired = TRUE;
	};
	~CAIPath(void);
	int  GetCount();
	void Add(CCurve* pCurve);
	void GetArrayDot(CArray<DBPT,DBPT>& dtArray);
	void Delete();

public:
	AICurveList m_curveList;

	BOOL m_bCLosed;	// ���
	BOOL m_bFilled;	// ���
	BOOL m_bWired;	// �Ƿ��Ǳ߿�FALSEȥ�����ȥ������
	
	COLORREF m_wireColor;	// �߿���ɫ
	COLORREF m_fillColor;	// �����ɫ��m_bFilled=TRUE����Ч
	double m_dbStrokeWidth;  // �߿�
};

// AI��ͼ��,�������CAIPath
class  CAILayer : public CObject
{
public:
	CAILayer(){};
	CAILayer(const CString& strLayerName){ m_strLayerName = m_strLayerName; }
	~CAILayer(void);
	int GetCount();
	AIPathList &GetAIPathList();
	void Add(CAIPath* pPath);
	void Delete();

	CString m_strLayerName;

public:
	AIPathList m_pathList;
};


class  CAIFile 
{
public:
	CAIFile(){ };
	~CAIFile(void);
	int GetCount();
	void Add(CAILayer* pLayer);
	AILayerList &GetAILayerList();
	void Delete();
	DBRect  m_rcRange;

public:
	AILayerList m_layerLists;
};

#endif