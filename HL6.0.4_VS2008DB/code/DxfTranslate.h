// DxfTranslate.h: interface for the CDxfTranslate class.
//
//////////////////////////////////////////////////////////////////////

//*******************************************************************//
//Class interface for user interface of decoder v1.0,designed by 
//PengZili,Feb. 2005.
//phone:0755-86161316;E-mail:pzili@21cn.com
//copyright reserved by HAN'S LASER co. ltd.(Shen Zhen)
//*******************************************************************//

#if !defined(AFX_DXFTRANSLATE_H__1049F708_A23C_4EF5_9959_10ED958D1122__INCLUDED_)
#define AFX_DXFTRANSLATE_H__1049F708_A23C_4EF5_9959_10ED958D1122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//++++++++++++++++++++tagPOINTOUT+++++++++++++++++++++++++++++++++++++++++++++++//
//						�����ı�ʾ											//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
typedef struct tagPOINTOUT
{
	DOUBLE x;
	DOUBLE y;
	DOUBLE z;
}POINTOUT;

//++++++++++++++++++++ ��CDxfStroke+++++++++++++++++++++++++++++++++++++++++++++++//
//						�������	   											  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class  CDxfStroke : public CObject
{
public:
	CArray<POINTOUT,POINTOUT>m_pointArray;
	CString m_nLayerName;
};


//++++++++++++++++++++ ��CDxfStroke3D+++++++++++++++++++++++++++++++++++++++++++++++//
//						�������	   											  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define OUT_3DOUTLINE	0					// ������
#define OUT_3DFACE		1					// ���3dface, polyline face
class  CDxfStroke3D : public CObject
{
public:
	CString m_nLayerName;
	CArray<POINTOUT,POINTOUT>m_pointArray;	//�������
	int m_nType;							// 0:��� 1:3dface(���ĸ������)
};

// 3DFace
class CDxfStroke3D_3DFace : public CDxfStroke3D
{
public:										
	int m_nInvisEdgeFlag;					// 1:��һ���߲��ɼ� 2:�ڶ����߲��ɼ� 4:�������߲��ɼ� 8:�������߲��ɼ�
};

//++++++++++++++++++++ ��CDxfArcCirOut+++++++++++++++++++++++++++++++++++++++++++++++//
//						���Բ����Բ	   											  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define CAOUT_BASE		0
#define CAOUT_LINE		1
#define CAOUT_CIRCLE	2
#define CAOUT_ARC		3

class CDxfArcCirOut : public CObject//bass class
{
public:
	CString m_nLayerName;				  //���
	int m_nObjType;						  //����
};

class CDxfArcCirOut_LINE: public CDxfArcCirOut//ֱ��
{
public:
	CArray<POINTOUT,POINTOUT>m_pointArray;//�������
};

class CDxfArcCirOut_ARC : public CDxfArcCirOut//Բ��
{
public:
	DOUBLE	m_dbRadius;	//�뾶
	POINTOUT m_centerP;	//Բ��
	POINTOUT m_startP;	//���
	POINTOUT m_endP;	//�յ�
	INT m_nDire;		//����:0-˳;1-��	
};

class CDxfArcCirOut_CIRCLE : public CDxfArcCirOut//Բ
{
public:
	DOUBLE	m_dbRadius;	//�뾶
	POINTOUT m_centerP;	//Բ��
};

//++++++++++++++++++++����ӿ�1: ��CDxfTranslate+++++++++++++++++++++++++++++++++++++++++++++++//
//      �ӿ�1�� Translate(CString flie,                //��������ļ�·��					//
//						  DOUBLE precision,			   //����ֵ	                            //
//						  CString strFontPath,		   //�ֿ�·��							//  
//						  )                                                                 //
//      �ӿ�2��CTypedPtrList<CObList,CDxfStroke *>m_DxfStrokeList ���������Stroke����      //
//      �ӿ�3��CTypedPtrList<CObList,CDxfArcCirOut *>m_DxfObjList ���������Stroke����       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
typedef CTypedPtrList<CObList,CDxfStroke *> DXFSTROKE_LIST;
typedef CTypedPtrList<CObList,CDxfArcCirOut *> DXFACOUT_LIST;
typedef CTypedPtrList<CObList,CDxfStroke3D *> DXFSTROKE_LIST3D; // �����߿��3dface��ģ��
class AFX_EXT_CLASS CDxfTranslate  
{
public:
	CDxfTranslate();
	virtual ~CDxfTranslate(); 

	BOOL Translate(CString flie,DOUBLE precision,CString strFontPath);

	// nOutType:1ֻ����� 2:ֻ��3DFace 3:������3DFace
	// ������������m_DxfStrokeList3D�У�������CDxfStroke3D�е�m_nType����
	BOOL Translate3D( CString flie,DOUBLE precision, CString strFontPath,int nOutType);

	DXFSTROKE_LIST m_DxfStrokeList;

	DXFACOUT_LIST m_DxfObjList;

	DXFSTROKE_LIST3D m_DxfStrokeList3D;
};

//++++++++++++++++++++����ӿ�2: TranslateDXF/DeleteObjStk+++++++++++++++++++++++++++++++++++++++++++++//
//	(Added by peng zili, 2006-08-21, phone: 86161387)
//	˵��: ����	TranslateDXF ����Dxf�ĵ�����, ȷ������ʹ��������(����)DeleteObjɾ������
//	1. TranslateDXF
//	flie -		[IN] ��������ļ���(·��)
//	precision -	[IN] ������������ֵ			
//	dxfObjOut - [OUT]�������(��������ʽ)
//	strFontPath-[IN] SHX�ֿ�·��(Ϊ�ղ�����ȷ����SHX����)
//	RET - TRUE: ����ɹ�; FALSE - ����ʧ��
//	2. DeleteObjStk
//	dxfObjDel - [IN] �������(��������ʽ), TranslateDXF�����(dxfObjOut)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
struct OBJ_OUTSTROKE : public CObject
{
	OBJ_OUTSTROKE()
	{
		m_bContinueStroke = TRUE;
	}
	DXFSTROKE_LIST lststkOut;	
	CString m_nLayerName;
	BOOL	m_bContinueStroke;
};

typedef CTypedPtrList<CObList, OBJ_OUTSTROKE*> DXFOBJOUTLIST_STROKE;

extern "C"
AFX_EXT_API BOOL TranslateDXF( CString flie, DOUBLE precision, DXFOBJOUTLIST_STROKE& dxfObjOut, CString strFontPath = _T("") );
extern "C"
AFX_EXT_API	void DeleteObjStk( const DXFOBJOUTLIST_STROKE& dxfObjDel );

//++++++++++++++++END ����ӿ�2+++++++++++++++++++++++++++++++++++

//++++++++++++++++++++����ӿ�3: TranslateDXF/DeleteOutObj+++++++++++++++++++++++++++++++++++++++++++++//
//	(Added by peng zili, 2007-04-10, phone: 86161387)
//	˵��: ����	TranslateDXFObj ����Dxf�ĵ�����, �Զ���ʽ��֯������, ȷ������ʹ��������(����)DeleteObjɾ������
//	1. TranslateDXFObj
//	flie -		[IN] ��������ļ���(·��)
//	precision -	[IN] ������������ֵ			
//	dxfObjOut - [OUT]�������(��������ʽ)
//	strFontPath-[IN] SHX�ֿ�·��(Ϊ�ղ�����ȷ����SHX����)
//	RET - TRUE: ����ɹ�; FALSE - ����ʧ��
//	2. DeleteOutObj
//	dxfObjDel - [IN] �������(��������ʽ), TranslateDXF�����(dxfObjOut)
//
//	NOTE: ��Ȳ�����󲻱������νṹ, ʹ��ʱ��Ҫ��ɢ
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
struct OBJ_OUTARC : public CObject
{
	DXFACOUT_LIST m_lstObjOutArc;
	CString m_strLayerName;
};
typedef CTypedPtrList<CObList, OBJ_OUTARC *> DXFOBJOUTARC_LIST;

extern "C"
AFX_EXT_API BOOL TranslateDXFObj( CString flie, DOUBLE precision, DXFOBJOUTARC_LIST& dxfObjOut, CString strFontPath = _T("") );
extern "C"
AFX_EXT_API	void DeleteOutObj( const DXFOBJOUTARC_LIST& dxfObjDel );

//++++++++++++++++END ����ӿ�3+++++++++++++++++++++++++++++++++++


#endif // !defined(AFX_DXFTRANSLATE_H__1049F708_A23C_4EF5_9959_10ED958D1122__INCLUDED_)
