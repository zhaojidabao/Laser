#ifndef __GRAYAPIDEFINE_H_
#define __GRAYAPIDEFINE_H_


//平台发出的打标消息
#ifndef WM_MARK_END
	#define WM_MARK_END  (WM_USER+5)
#endif 
#define WM_ENV_ERROR (WM_USER+10)	//环境错误，需要退出
#define MARK_TEND	100
#define MARK_SEND	101
#define MARK_INFO	103

//Dll采用的数据定义，必须和平台的定义保持一致或兼容
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
	int      nDotSteps;	//单点跳转步数
	double dbStepLen;	//单步跳转长度(mm)
	double dbQFreq;		//打标Q频(KHZ)
}GRAY_BMPSPEC;


namespace GRAY_SET
{
	enum CALL_FUNC {READY, SET_CFG, SET_DATALEN, SET_DATA, MARK_START, MARK_STOP, QUIT};
	/*
		-----------------------
		CALL_FUNC	说明
		-----------------------
		READY:		启动打标服务
		SET_CFG:	打标参数设置
		SET_DATALEN:打标数据长度
		SET_DATA:	打标数据设置
		MARK_START:	开始打标
		MARK_STOP:	中止打标
		QUIT:		停止打标服务
	*/

	extern "C"
	{
		typedef BOOL (*CALLFUNCTION)(UINT eFunc, LPVOID pPara);
		/*
			说明：DLL回调平台功能函数，阻塞式

			参数定义：	eFunc 功能ID

						pPara 功能参数，调入时表示入参，返回时表示返回参数
						---------------------------------------------------------------------
						READY	  --	调入时：窗台句柄，平台以此句柄通知打标进度
										调出时：无
						SET_CFG	  --	调入时：GRAY_BMPSPEC，注意数据一致或兼容性
										调出时：无 
						SET_DATALEN--	调入时：长度,打标点个数
										调出时：无
						SET_DATA  --	调入时：GRAY_DOT6，全部要打标的数据，由dll分配释放
										调出时：无 
						MARK_START--	调入时：无
										调出时：无
						MARK_STOP --	调入时：无
										调出时：无 
						QUIT	  --	调入时：无
										调出时：无 

			返回值：TRUE 成功
					FALSE 失败
		*/

		typedef void (*GRAYSET)(CALLFUNCTION pCallBack);
		/*
			说明：DLL入口函数，弹出模态对话框，外部不能再进行其他卡服务
			参数定义：	pFunc 服务回调平台功能函数指针
		*/
	}
}

using namespace GRAY_SET;

#endif
