#ifndef HL_MOTION_PARA_DEFINE_20101023_1202_XLS0_H
#define HL_MOTION_PARA_DEFINE_20101023_1202_XLS0_H


//运动轴类型
enum{
	lineAxis,			//平移轴
	rotateAxis,			//旋转轴
	AxisEnd
};


//运动状态定义
typedef enum{
	moveStatusErr = -1, 	
	moveStatusMoving,		//正在运动
	moveStatusFinish,		//运动完成
	moveStatusLimitF,		//正向限位
	moveStatusLimitR,		//负向限位
	moveStatusHome,			//处于原点
	moveStatusAlm,			//警报信息
}moveStatus;

//运动基本参数,速度单位全部为脉冲
typedef struct BASE_PARA
{
	int		nWorkSpeed;		//运动速度
	int     nEmptySpeed;		//不带负载速度
	int		nWorkAcc;			//加速度 递增pulse/秒
	int     nEmptyAcc;       //加速度 递增pulse/秒
	int     nHomeAcc;			//加速度 递增pulse/秒
	int		nHomeHighSpeed;		//回零速度
	int		nHomeLowSpeed;		//回零起动速度
}BASEPARA, *LPBASEPARA;


//和运动相关的全部参数
typedef struct	MOTION_PARA
{
	//硬件及运动的一些基本参数
	int			nPPR;					//每转脉冲数

	double	dbPitch;			//螺间距
	int			nDelay;				//运动完成后延时
	int 		nAxisType;		    //运动轴类型 0 平移轴， 1 旋转轴
	int			nMotorDire;			//电机转动方向 1正向， -1 反向
	
	////速度参数,第一种情况:直线运动(单位:mm)
	double	dbLineEmptySpeed;	//空程速度
	double	dbLineWorkSpeed;	//工作速度
	double	dbHomeHighSpeed;	//回零点速度（高速段）
	double	dbHomeLowSpeed;		//回零点速度（低速段）
	
	////速度参数,第二种情况,旋转运动(单位:度)
	double	dbREmptySpeed;		//空程速度
	double	dbRWorkSpeed;		//工作速度
	
	//速度参数,加减速时间
	double	dbEmptyTAcc;		//空程加速时间
	double	dbWorkTAcc;			//工作加速时间
	double	dbHomeTAcc;			//回零减速时间
	
	BOOL    bHardOrgEnable;     //机械原点使能
	BOOL    bAutoHome;			//是否自动回零
	BOOL    bEnabled;			//是否使能
}MOTIONPARA, *LPMOTIONPARA;



class CMotion: public CObject
{
public:
	CMotion();
	virtual ~CMotion();

	BOOL Home(int nAxis,BOOL bSoftHome);
	void GoSoftHome(BOOL bAuto = FALSE);

	void ResetPara(int nAxis = 0);//重设参数
	BOOL GetMotionPara(int nAxis,MOTIONPARA &para);
	BOOL SetMotionPara(int nAxis,const MOTIONPARA &para);
	BOOL GetBufferMovePara(int nWorkType,double dbValue,MOTOR_PARA &para);//传入运动方式、运动量，为缓冲区参数赋值
	
	//以下为立即指令，待以后扩充用
	BOOL AMove(int nAxis,double dbWorkPos,int nType);
	BOOL RMove(int nAxis,double dbRelPos,int nType);
	void StopRun();
	BOOL IsMotionStop(){ return m_bIsRun; };
	int	 GetAxisType(int nAxisNo);
	int  GetAxisPulse(int nAxis,double dbOrg);
	double PosTranslate( int nAxis, LONGLONG nPos);

protected:
	void ConvertToPulse(int nAxis,double dbOrg, double& dbPulse);
	void ResetBasePara(int nAxis);
	
private:
	MOTIONPARA	m_para[4];		//原始运动参数
	BASEPARA	m_basePara[4];	//经过转换后的最终参数
	BOOL		m_bIsRun;		//是否正在运行,扩充用
};

extern CMotion g_Motion; 

#endif //HL_MOTION_PARA_DEFINE_20101023_1202_XLS0_H