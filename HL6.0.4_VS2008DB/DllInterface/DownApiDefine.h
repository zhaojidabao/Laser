#ifndef __DOWNAPIDEFINE_H_
#define __DOWNAPIDEFINE_H_


/******************************************************************************/
	#define  DOWN_VERSION "5.3.4"		//5.3.4为初始版本,以后顺次5.3.5 ...
/******************************************************************************/


#ifndef WM_MARK_END
#define WM_MARK_END  (WM_USER+5)
#endif


#ifndef WM_ENV_ERROR
#define WM_ENV_ERROR (WM_USER+10)	//环境错误，需要退出
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
		
		char szFile[MAX_PATH] ;		//文档名
		WORD uIOValid;				//IO有效值 (I15-I4)
		WORD uIOMdt;				//IO托管位 (I15-I4)
		UINT nRet;					//下载返回：0 无错误，
									//			1 文件传输失败，
									//			2 文件太大，超过flash范围
									//			3 FLash写入错误
									//			4 文档数据未准备好
									//			5 文档数据超出打标范围
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
			// ... 待扩展
		}
	}

	char szVersion[20];			//交互数据的版本号
	int	nBlock;					//数据块个数
	HD_DATA_BLOCK* pBlock;		//数据块指针
};

enum CALL_FUNC 
{
	READY,					//界面初始化完毕，获取信息
	MARK_START,				//开始打标
	MARK_STOP,				//中止打标 
	DOWN_MUL,				//多文档下载
	QUIT,					//界面退出
	GET_MDTIO ,				//获取I15-I4的信号值
	SAVE_DOC,				//保存当前文档的设置
	SAVE_MULDOC				//保存当前设置方案为多文档
};

extern "C"
{
	typedef BOOL (*CALLFUNCTION)(CALL_FUNC eFunc, LPVOID pPara);
	/*
		说明：库服务回调平台功能函数，阻塞式

		参数定义：	eFunc 功能ID
					pPara 功能参数，调入时表示入参，返回时表示返回参数

					READY	  --	调入时：窗台句柄，外部以此通知控制进程
									调出时：HD_DATA_HEAD指针
					MARK_START--	调入时：HD_DATA_HEAD指针
									调出时：无 
					MARK_STOP --	调入时：无
									调出时：无 
					DOWN_MUL	--	调入时：HD_DATA_HEAD指针
									调出时：HD_DATA_HEAD指针
					QUIT	  --	调入时：无
									调出时：无 
					GET_MDTIO  --	调入时：无
									调出时：I15-14的信号值
					SAVE_DOC	--	调入时：HD_DATA_HEAD指针
									调出时：无
					SAVE_MULDOC--	调入时：无
									调出时：无 

		返回值：TRUE 成功
				FALSE 失败
	*/

	typedef void (*DOWNMODALDLG)(CALLFUNCTION pFunc);
	/*
		说明：启动服务入口函数，弹出模态对话框，外部不能再进行其他卡服务
		参数：pFunc 服务回调平台功能函数指针
	*/
}

#endif
