#ifndef __DOWNAPIDEFINE_H_
#define __DOWNAPIDEFINE_H_


/******************************************************************************/
	#define  DOWN_VERSION "5.3.4"		//5.3.4Ϊ��ʼ�汾,�Ժ�˳��5.3.5 ...
/******************************************************************************/


#ifndef WM_MARK_END
#define WM_MARK_END  (WM_USER+5)
#endif


#ifndef WM_ENV_ERROR
#define WM_ENV_ERROR (WM_USER+10)	//����������Ҫ�˳�
#endif

#define MARK_TEND	100
#define MARK_SEND	101
#define MARK_INFO	103


struct HD_DATA_BLOCK				
{
	struct _5_3_4 
	{
		_5_3_4(){
			memset(this, 0, sizeof(_5_3_4));
		}
		
		char szFile[MAX_PATH] ;		//�ĵ���
		WORD uIOValid;				//IO��Чֵ (I15-I4)
		WORD uIOMdt;				//IO�й�λ (I15-I4)
		UINT nRet;					//���ط��أ�0 �޴���
									//			1 �ļ�����ʧ�ܣ�
									//			2 �ļ�̫�󣬳���flash��Χ
									//			3 FLashд�����
									//			4 �ĵ�����δ׼����
									//			5 �ĵ����ݳ�����귶Χ
	}m5_3_4;
};

struct HD_DATA_HEAD 
{
	HD_DATA_HEAD()
	{
		CStringA str(DOWN_VERSION);
		memset(szVersion, 0, 20);
		memcpy(szVersion, LPCSTR(str), str.GetLength());

		nBlock=0;
		pBlock=NULL;
	}

	~HD_DATA_HEAD(){
		nBlock=0;
		if (pBlock) {
			delete []pBlock;
			pBlock=NULL;
		}
	}

	void operator << (const HD_DATA_HEAD& src)
	{
		nBlock = src.nBlock;
		if (pBlock) {
			delete []pBlock;
			pBlock=NULL;
		}
		if (0 == nBlock) {
			return;
		}
		pBlock = new HD_DATA_BLOCK[nBlock];
		
		CStringA strVLow = szVersion;
		if (strVLow > CStringA(src.szVersion)) {
			strVLow = CStringA(src.szVersion);
		}
		for (int iBlock = 0; iBlock<src.nBlock; ++iBlock)
		{
			if ("5.3.4" == strVLow) {
				(*(pBlock+iBlock)).m5_3_4 = (*(src.pBlock+iBlock)).m5_3_4;
			}
			// ... ����չ
		}
	}

	char szVersion[20];			//�������ݵİ汾��
	int	nBlock;					//���ݿ����
	HD_DATA_BLOCK* pBlock;		//���ݿ�ָ��
};

enum CALL_FUNC 
{
	READY,					//�����ʼ����ϣ���ȡ��Ϣ
	MARK_START,				//��ʼ���
	MARK_STOP,				//��ֹ��� 
	DOWN_MUL,				//���ĵ�����
	QUIT,					//�����˳�
	GET_MDTIO ,				//��ȡI15-I4���ź�ֵ
	SAVE_DOC,				//���浱ǰ�ĵ�������
	SAVE_MULDOC				//���浱ǰ���÷���Ϊ���ĵ�
};

extern "C"
{
	typedef BOOL (*CALLFUNCTION)(CALL_FUNC eFunc, LPVOID pPara);
	/*
		˵���������ص�ƽ̨���ܺ���������ʽ

		�������壺	eFunc ����ID
					pPara ���ܲ���������ʱ��ʾ��Σ�����ʱ��ʾ���ز���

					READY	  --	����ʱ����̨������ⲿ�Դ�֪ͨ���ƽ���
									����ʱ��HD_DATA_HEADָ��
					MARK_START--	����ʱ��HD_DATA_HEADָ��
									����ʱ���� 
					MARK_STOP --	����ʱ����
									����ʱ���� 
					DOWN_MUL	--	����ʱ��HD_DATA_HEADָ��
									����ʱ��HD_DATA_HEADָ��
					QUIT	  --	����ʱ����
									����ʱ���� 
					GET_MDTIO  --	����ʱ����
									����ʱ��I15-14���ź�ֵ
					SAVE_DOC	--	����ʱ��HD_DATA_HEADָ��
									����ʱ����
					SAVE_MULDOC--	����ʱ����
									����ʱ���� 

		����ֵ��TRUE �ɹ�
				FALSE ʧ��
	*/

	typedef void (*DOWNMODALDLG)(CALLFUNCTION pFunc);
	/*
		˵��������������ں���������ģ̬�Ի����ⲿ�����ٽ�������������
		������pFunc ����ص�ƽ̨���ܺ���ָ��
	*/
}

#endif
