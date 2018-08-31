#ifndef __GEN_CODEBAR_2MATRIX_H_H__
#define __GEN_CODEBAR_2MATRIX_H_H__
/////////////////////////////////////////////////////////////
/*////////////////////////////////////////////////////////////
		GenCodebar2Matrix.dll ���ڶԳ���2ά�����ɽ��з�װ��
	�Ա�򻯵����ߡ�
	Designed by Vinca		-------2006/7/28
////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////
////////Լ�������нӿڲ������ڴ���ɺ��������߽��з������ͷ�
////////������������ֵ���壺
	0		����	�����ɹ�
	////
	-1		����	δ֪����
	-2		����	��ڲ���ָ��Ƿ�
	-3		����	�������Ͳ�֧��
	-4		����	��ڲ����Ƿ�
	-5		����	������ʼ��ʧ��
	-6		����	��������ʧ�ܣ�δ֪����
	-7		����	��������ʧ�ܣ���ڲ������ֲ�ƥ���������ɹ��򣬾���ο����ڲ�����ĳЩֵ����
	////
	1		����	��������ʧ�ܣ�Ԥ�����ڴ治��������ο����ڲ�����ĳЩֵ����
///////////////////////////////////////////////////////////*/
//�ṹ�嶨��
//QR������ڡ����ڲ�������
typedef struct IQR{
	////�������
	struct{
		ULONG _versionType : 5;//�汾���ͣ�1~~20
		ULONG _levelType : 3;//������1~~4
		ULONG _bConver : 1;//�Ƿ���(1��ʾ���0��ʾ������)
		ULONG _countsConver : 10;//����ʱ���ܱ�������С��Ԫ����
		ULONG _codetype : 3;//����ģʽ 0:Numeric 1:Alphabet-numeric 2:8-bit data 3:Kanji (shift-jis) 4:ECC Level
		ULONG _bMicro: 1; //Micro QR
		ULONG _reserved : 10;//����
	} _bits;
	BYTE* _lpszSrc;//������������
	long _sizeSrcBytes;//�������������ֽڳ���
	////���ֵ�����ɺ�������ֵ���壺
				//-7����ʾ����ʧ�ܣ������������ݳ��ȴ��ڵ�ǰ���õİ汾��������������������󳤶ȣ�
					//��ʱ��_sizeRetMaxSrcBytes��Ϊ�������������������ݵ��ֽڸ���
				//1��_lpszRetBuffer�ֽ�������������ʱ��_sizeRetBytesΪ���������ֽڴ�С
	long _sizeRetMaxSrcBytes;//�����ɺ�������0��-7ʱ����ʾ��汾���͡��������Ӧ���������������������ݵ��ֽڸ���
	long _sizeRetBytes;//�������ɵ������ֽڳ���
	long _sizeRetRowsOrCols;//���ؾ��������������(�С�����ͬ)
	long _sizeRetBuffer;//�������ֽڳ���
	BYTE* _lpszRetBuffer;//������
} IQR;

//DataMatrix������ڡ����ڲ�������
typedef struct IDATAMATRIX{
	////�������
	struct{
		ULONG _eccType : 3;//�������ͣ�Ĭ�ϣ�ECC--200(5)��ȡֵ��Χ��ECC--000(0)ֱ��ECC--200(5)
		ULONG _outStyle : 1;//�������ͣ�0��ʾ�����Σ�1��ʾ������
		ULONG _bUserDefined : 1;//�û��Զ��峤��ģ����(1ʱ��_widthNums��_heightNums��Ч)
		ULONG _bConver : 1;//�Ƿ���(1��ʾ���0��ʾ������)
		ULONG _countsConver : 10;//����ʱ���ܱ�������С��Ԫ����
		ULONG _reserved : 16;//����
	} _bits;
	long _widthNums;//�Զ���ģ����
	long _heightNums;//�Զ���ģ��߶�
	BYTE* _lpszSrc;//������������
	long _sizeSrcBytes;//�������������ֽڳ���
	////���ֵ�����ɺ�������ֵ���壺
				//-7����ʾ����ʧ�ܣ���ʱ��_sizeRetRows��_sizeRetColsΪ�Զ�������߶ȡ���ȱ����趨��ֵ
				//1��_lpszRetBuffer�ֽ�������������ʱ��_sizeRetBytesΪ���������ֽڴ�С
	long _sizeRetBytes;//�������ɵ������ֽڳ���
	long _sizeRetRows;//���ؾ��������
	long _sizeRetCols;//���ؾ��������
	long _sizeRetBuffer;//�������ֽڳ���
	BYTE* _lpszRetBuffer;//������
	long   _symbolNo;//���뷽ʽ
	long   _shapeNo;//��״
} IDATAMATRIX;

//PDF417������ڡ����ڲ�������
typedef struct IPDF417{
	////�������
	struct{
		ULONG _eccLevel : 4;//������(0~8)
		ULONG _nRows : 7;//417��������(0��3~~90)
		ULONG _nCols : 5;//417��������(0��1~30)
		ULONG _bConver : 1;//�Ƿ���(1��ʾ���0��ʾ������)
		ULONG _countsConver : 10;//����ʱ���ܱ�������С��Ԫ����
		ULONG _reserved : 5;//����
	} _bits;
	long _xScale;//Xģ���ȸ���
	long _yScale;//Yģ��߶ȸ���
	BYTE* _lpszSrc;//������������
	long _sizeSrcBytes;//�������������ֽڳ���
	////���ֵ�����ɺ�������ֵ���壺
	//1��_lpszRetBuffer�ֽ�������������ʱ��_sizeRetBytesΪ���������ֽڴ�С
	long _sizeRetBytes;//�������ɵ������ֽڳ���
	long _sizeRetRows;//���ؾ��������
	long _sizeRetCols;//���ؾ��������
	long _sizeRetBuffer;//�������ֽڳ���
	BYTE* _lpszRetBuffer;//������
} IPDF417;

//HANXIN������ڡ����ڲ�������
typedef struct IS_GEN_CODEBAR_HANXIN{
	////�������
	struct{
		ULONG _versionType : 5;	//�汾���ͣ�1~~20
		ULONG _levelType : 3;	//������1~~4
		ULONG _bConver : 1;		//�Ƿ���(1��ʾ���0��ʾ������)
		ULONG _countsConver : 10;//����ʱ���ܱ�������С��Ԫ����
		ULONG _codetype : 3;//����ģʽ 0:Numeric 1:Alphabet-numeric 2:8-bit data 3:Kanji (shift-jis) 4:ECC Level
		ULONG _bReverse:10; //����
	} _bits;
	BYTE* _lpszSrc;//������������
	long _sizeSrcBytes;//�������������ֽڳ���
	////���ֵ�����ɺ�������ֵ���壺
	//-7����ʾ����ʧ�ܣ������������ݳ��ȴ��ڵ�ǰ���õİ汾��������������������󳤶ȣ�
	//��ʱ��_sizeRetMaxSrcBytes��Ϊ�������������������ݵ��ֽڸ���
	//1��_lpszRetBuffer�ֽ�������������ʱ��_sizeRetBytesΪ���������ֽڴ�С
	long _sizeRetMaxSrcBytes;//�����ɺ�������0��-7ʱ����ʾ��汾���͡��������Ӧ���������������������ݵ��ֽڸ���
	long _sizeRetBytes;//�������ɵ������ֽڳ���
	long _sizeRetRowsOrCols;//���ؾ��������������(�С�����ͬ)
	long _sizeRetBuffer;//�������ֽڳ���
	BYTE* _lpszRetBuffer;//������
} IHANXIN;

//�����������ɺ����ӿ���ڡ����ڲ�������
typedef struct IMATRIXBAR{
	long _type;//1QR�룬2DataMatrix�룬3Pdf417��, 4HANXIN��
	union{
		IQR _qr;
		IDATAMATRIX _dataMatrix;
		IPDF417 _pdf417;
		IHANXIN _hanxin;
	} _u_value;
} IMATRIXBAR;

//////�����ӿ�����
class IGenCodebar2Matrix{
public:
	////��ȡ������Ϣ
	LPCSTR GetErrInfo(long errNo, long nResourceType){
		if ( !LoadLib() )
			return (LPCSTR) NULL;
		return (((LPCSTR (__stdcall*)(long,long)) m_pFunc[0])(errNo,nResourceType));
	}
	////��������
	long GenCodebar(LPVOID lpValueSrcRet){
		if ( !LoadLib() )
			return -1;
		return (((long (__stdcall*)(LPVOID)) m_pFunc[1])(lpValueSrcRet));
	}
	////
	IGenCodebar2Matrix(){
		Clear();
	}
	~IGenCodebar2Matrix(){
		ReleaseLib();
	}
	void ReleaseLib(){
		if ( m_hLib ) ::FreeLibrary(m_hLib);
		Clear();
	}

private:
	void Clear(){
		memset(this,0,sizeof(*this));
	}
	BOOL LoadLib(){
		if ( m_hLib )
			return TRUE;
		m_hLib = ::LoadLibrary(TEXT("GenCodebar2Matrix.dll"));
		if ( !m_hLib ){
			::OutputDebugString(TEXT("Can't find GenCodebar2Matrix.dll!\n"));
			return FALSE;
		}
		BOOL bOk = TRUE;
		for( int i = 0;i<2;i++ ){
			m_pFunc[i] = (DWORD) ::GetProcAddress((HMODULE) m_hLib,(LPCSTR) (MAKELONG(i+1,0)));
			if ( 0 == m_pFunc[i] ) bOk = FALSE;
		}
		if ( !bOk ){
			::OutputDebugString(TEXT("Incorrect version of GenCodebar2Matrix.dll!\n"));
			ReleaseLib();
			return FALSE;
		}
		return TRUE;
	}
	
	////////////////
	DWORD m_pFunc[2];
	HINSTANCE m_hLib;
};
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
#endif
