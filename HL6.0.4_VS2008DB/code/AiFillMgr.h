#pragma once
#include "AIDecodeDefine.h"
#include "StrokeB.h"
#include "cv.h"
#include "HighGuI.h"

// CAiFillMgr ����Ŀ��
class CAiFillMgr : public CObject
{
public:
	CAiFillMgr();
	virtual ~CAiFillMgr();
	
	//
	BOOL DeCode( const CString& file, StrokeList &listLineData );

	//����ͼ������
	void CreateImageData( int nWidth, int nHeight );
	
	//����ͼ������
	void ReleaseImageData();
	
	//����AI������λͼ�ϻ�ͼ
	BOOL DrawPath( CAIStroke* pAiStroke, double dbLeft, double dbBottom );
	
	//��ȡ��������λͼ�ϵ�����
	BOOL GetPointArray( CArray<PTOUT,PTOUT> &dtArray,  CvPoint *pPointData, double dbLeft, double dbBottom );
	
	//��ɫת��
	CvScalar RgbToCvcolor( COLORREF color);

	//��̬ѧ��ʴ����
	void ErodeFillPart( int nPenWidth );

	//������Ӽ���
	void AddGroupCount();

	//���е������㣬��ɺ�������յ�λͼ
	void AddBmp( int nType );

	//�����������
	void CreateLineList();

	//ɾ���������
	void DeleteLineList();

	//X����ɨ��
	void ScanX( int nRowPos, double dbY, StrokeList &listData );

	//Y����ɨ��
	void ScanY( int nColPos, double dbX, StrokeList &listData );

	//
	CAITranslate m_aiTrnslate;

	//���յ�Ч��ͼ
	IplImage *m_pBmp;
	
	//ԭʼ�հ�ͼ
	IplImage *m_pBmpOrg;
	
	//��Ͻ׶���ͼ
	IplImage *m_pBmpGroup;
	
	//���ͼ,��������ͼ����ķ�Χ
	IplImage *m_pBmpMark;
	
	//���ݾ������Լ�¼����������
	IplImage *m_pBmpMat;
	
	//����·��������
	int m_nXR;
	
	//·������
	int m_nDirection;

	//
	int m_nBmpWidth;
	int m_nBmpHeight;
	double	m_dbHeight;
	double	m_dbWidth;
	double m_dbSpaceX;
	double m_dbSpaceY;
	int m_nScanType;
	double m_dbScanDis;
	BOOL m_bLoop;	//����
	StrokeList m_listLineData;
};


