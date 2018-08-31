#ifndef __GRAYAPIDEFINE_H_
#define __GRAYAPIDEFINE_H_


//ƽ̨�����Ĵ����Ϣ
#ifndef WM_MARK_END
	#define WM_MARK_END  (WM_USER+5)
#endif 
#define WM_ENV_ERROR (WM_USER+10)	//����������Ҫ�˳�
#define MARK_TEND	100
#define MARK_SEND	101
#define MARK_INFO	103

//Dll���õ����ݶ��壬�����ƽ̨�Ķ��屣��һ�»����
typedef struct GRAY_DOT6
{
	double x;
	double y;
	float  z;
	float  i;
	float  b1;
	float  b2;
	
	GRAY_DOT6()
	{
		::memset(this, 0, sizeof(GRAY_DOT6));
	};
	
	void operator = ( const GRAY_DOT6 & src )
	{
		if( &src == this )	
		{
			return;
		}
		::memcpy(this, &src, sizeof(GRAY_DOT6));
	}
} GRAY_DOT6;

typedef struct GRAY_BMPSPEC
{
	WORD   nVer;
	/////
	int      nDotSteps;	//������ת����
	double dbStepLen;	//������ת����(mm)
	double dbQFreq;		//���QƵ(KHZ)
}GRAY_BMPSPEC;


namespace GRAY_SET
{
	enum CALL_FUNC {READY, SET_CFG, SET_DATALEN, SET_DATA, MARK_START, MARK_STOP, QUIT};
	/*
		-----------------------
		CALL_FUNC	˵��
		-----------------------
		READY:		����������
		SET_CFG:	����������
		SET_DATALEN:������ݳ���
		SET_DATA:	�����������
		MARK_START:	��ʼ���
		MARK_STOP:	��ֹ���
		QUIT:		ֹͣ������
	*/

	extern "C"
	{
		typedef BOOL (*CALLFUNCTION)(UINT eFunc, LPVOID pPara);
		/*
			˵����DLL�ص�ƽ̨���ܺ���������ʽ

			�������壺	eFunc ����ID

						pPara ���ܲ���������ʱ��ʾ��Σ�����ʱ��ʾ���ز���
						---------------------------------------------------------------------
						READY	  --	����ʱ����̨�����ƽ̨�Դ˾��֪ͨ������
										����ʱ����
						SET_CFG	  --	����ʱ��GRAY_BMPSPEC��ע������һ�»������
										����ʱ���� 
						SET_DATALEN--	����ʱ������,�������
										����ʱ����
						SET_DATA  --	����ʱ��GRAY_DOT6��ȫ��Ҫ�������ݣ���dll�����ͷ�
										����ʱ���� 
						MARK_START--	����ʱ����
										����ʱ����
						MARK_STOP --	����ʱ����
										����ʱ���� 
						QUIT	  --	����ʱ����
										����ʱ���� 

			����ֵ��TRUE �ɹ�
					FALSE ʧ��
		*/

		typedef void (*GRAYSET)(CALLFUNCTION pCallBack);
		/*
			˵����DLL��ں���������ģ̬�Ի����ⲿ�����ٽ�������������
			�������壺	pFunc ����ص�ƽ̨���ܺ���ָ��
		*/
	}
}

using namespace GRAY_SET;

#endif
