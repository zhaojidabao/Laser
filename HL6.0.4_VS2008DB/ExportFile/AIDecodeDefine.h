#ifndef AFX_AI_DECODEDEFINE_5FCBBDC1_9B26_4014_9CA0_7A9DB590C213
#define AFX_AI_DECODEDEFINE_5FCBBDC1_9B26_4014_9CA0_7A9DB590C213

// 文件解码返回值
#define DECODE_SUCCESS		0	// 成功
#define DECODE_PARA			1	// 错误参数
#define DECODE_FILEERROR	2	// 文件格式错误
#define DECODE_VERSION		3	// 文件非法(非8.0及以下版本)
#define DECODE_ALLOCMEMORY	4	// 内存分配失败


///////////////////////////////////tagPTOUT////////////////////////////////
//								  输出点序列							///
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


// AI文件图形外框
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


//////////////////////////////////类CAIStroke//////////////////////////////
//									AI中的线条							///
///////////////////////////////////////////////////////////////////////////
class CAIStroke : public CObject
{
public:
	CArray<PTOUT,PTOUT> m_ptArray;
	
	BOOL m_bCLosed;	// 封闭
	BOOL m_bFilled;	// 填充
	BOOL m_bWired;	// 是否是边框，FALSE去掉填充去轮廓用
	
	COLORREF m_wireColor;	// 边框颜色
	COLORREF m_fillColor;	// 填充颜色，m_bFilled=TRUE是有效
	double m_dbStrokeWidth;  // 线宽
	
	int m_nID;
	BOOL m_bCompoundPathEnd;
	int m_nXR;			// 复合路径填充规则
	int m_nDirection;	// 路径方向
};


//////////////////////////////////类CAILayer///////////////////////////////
//								AI中的图层								///
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
	// 导出8.0及以下版本的AI文件
	// file: ai文件名
	// dbPrecision:Bezier曲线拟合精度
	// 导出后的数据在m_layerList中
	int Translate(LPWSTR szFile,DOUBLE dbPrecision);
	AILayer_LIST m_layerList;
	DBRect  m_rcRange;
protected:
	void Delete();
};

#endif