#ifndef AFX_AI_DECODEDEFINE_5FCBBDC1_9B26_4014_9CA0_7A9DB590C213
#define AFX_AI_DECODEDEFINE_5FCBBDC1_9B26_4014_9CA0_7A9DB590C213

// �ļ����뷵��ֵ
#define DECODE_SUCCESS		0	// �ɹ�
#define DECODE_PARA			1	// �������
#define DECODE_FILEERROR	2	// �ļ���ʽ����
#define DECODE_VERSION		3	// �ļ��Ƿ�(��8.0�����°汾)
#define DECODE_ALLOCMEMORY	4	// �ڴ����ʧ��


///////////////////////////////////tagPTOUT////////////////////////////////
//								  ���������							///
///////////////////////////////////////////////////////////////////////////
typedef struct tagPTOUT
{
	DOUBLE x;
	DOUBLE y;

	tagPTOUT()
	{
		x = 0;
		y = 0;
	}
	tagPTOUT(double x,double y)
	{
		this->x = x;
		this->y = y;
	}
	void operator=(const tagPTOUT& src)
	{
		x = src.x;
		y = src.y;
	}
}PTOUT;


// AI�ļ�ͼ�����
struct DBRect
{
	double left;
	double bottom;
	double right;
	double top;
	DBRect()
	{
		left = 100000000;
		bottom = 100000000;
		right = -100000000;
		top = -100000000;
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

	PTOUT Center()
	{
		return PTOUT(left + Width()/2,bottom + Height()/2);
	}
};


//////////////////////////////////��CAIStroke//////////////////////////////
//									AI�е�����							///
///////////////////////////////////////////////////////////////////////////
class CAIStroke : public CObject
{
public:
	CArray<PTOUT,PTOUT> m_ptArray;
	
	BOOL m_bCLosed;	// ���
	BOOL m_bFilled;	// ���
	BOOL m_bWired;	// �Ƿ��Ǳ߿�FALSEȥ�����ȥ������
	
	COLORREF m_wireColor;	// �߿���ɫ
	COLORREF m_fillColor;	// �����ɫ��m_bFilled=TRUE����Ч
	double m_dbStrokeWidth;  // �߿�
	
	int m_nID;
	BOOL m_bCompoundPathEnd;
	int m_nXR;			// ����·��������
	int m_nDirection;	// ·������
};


//////////////////////////////////��CAILayer///////////////////////////////
//								AI�е�ͼ��								///
///////////////////////////////////////////////////////////////////////////
typedef CTypedPtrList<CObList, CAIStroke*> AISTROKE_LIST;
class  CAILayerOut : public CObject
{
public:
	CAILayerOut();
	virtual ~CAILayerOut();
	AISTROKE_LIST m_strokeList;
	CString m_strLayerName;
};

typedef CTypedPtrList<CObList, CAILayerOut*> AILayer_LIST;
class AFX_EXT_CLASS CAITranslate
{
public:
	CAITranslate();
	~CAITranslate();
	// ����8.0�����°汾��AI�ļ�
	// file: ai�ļ���
	// dbPrecision:Bezier������Ͼ���
	// �������������m_layerList��
	int Translate(LPWSTR szFile,DOUBLE dbPrecision);
	AILayer_LIST m_layerList;
	DBRect  m_rcRange;
protected:
	void Delete();
};

#endif