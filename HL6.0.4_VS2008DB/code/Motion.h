#ifndef HL_MOTION_PARA_DEFINE_20101023_1202_XLS0_H
#define HL_MOTION_PARA_DEFINE_20101023_1202_XLS0_H


//�˶�������
enum{
	lineAxis,			//ƽ����
	rotateAxis,			//��ת��
	AxisEnd
};


//�˶�״̬����
typedef enum{
	moveStatusErr = -1, 	
	moveStatusMoving,		//�����˶�
	moveStatusFinish,		//�˶����
	moveStatusLimitF,		//������λ
	moveStatusLimitR,		//������λ
	moveStatusHome,			//����ԭ��
	moveStatusAlm,			//������Ϣ
}moveStatus;

//�˶���������,�ٶȵ�λȫ��Ϊ����
typedef struct BASE_PARA
{
	int		nWorkSpeed;		//�˶��ٶ�
	int     nEmptySpeed;		//���������ٶ�
	int		nWorkAcc;			//���ٶ� ����pulse/��
	int     nEmptyAcc;       //���ٶ� ����pulse/��
	int     nHomeAcc;			//���ٶ� ����pulse/��
	int		nHomeHighSpeed;		//�����ٶ�
	int		nHomeLowSpeed;		//�������ٶ�
}BASEPARA, *LPBASEPARA;


//���˶���ص�ȫ������
typedef struct	MOTION_PARA
{
	//Ӳ�����˶���һЩ��������
	int			nPPR;					//ÿת������

	double	dbPitch;			//�ݼ��
	int			nDelay;				//�˶���ɺ���ʱ
	int 		nAxisType;		    //�˶������� 0 ƽ���ᣬ 1 ��ת��
	int			nMotorDire;			//���ת������ 1���� -1 ����
	
	////�ٶȲ���,��һ�����:ֱ���˶�(��λ:mm)
	double	dbLineEmptySpeed;	//�ճ��ٶ�
	double	dbLineWorkSpeed;	//�����ٶ�
	double	dbHomeHighSpeed;	//������ٶȣ����ٶΣ�
	double	dbHomeLowSpeed;		//������ٶȣ����ٶΣ�
	
	////�ٶȲ���,�ڶ������,��ת�˶�(��λ:��)
	double	dbREmptySpeed;		//�ճ��ٶ�
	double	dbRWorkSpeed;		//�����ٶ�
	
	//�ٶȲ���,�Ӽ���ʱ��
	double	dbEmptyTAcc;		//�ճ̼���ʱ��
	double	dbWorkTAcc;			//��������ʱ��
	double	dbHomeTAcc;			//�������ʱ��
	
	BOOL    bHardOrgEnable;     //��еԭ��ʹ��
	BOOL    bAutoHome;			//�Ƿ��Զ�����
	BOOL    bEnabled;			//�Ƿ�ʹ��
}MOTIONPARA, *LPMOTIONPARA;



class CMotion: public CObject
{
public:
	CMotion();
	virtual ~CMotion();

	BOOL Home(int nAxis,BOOL bSoftHome);
	void GoSoftHome(BOOL bAuto = FALSE);

	void ResetPara(int nAxis = 0);//�������
	BOOL GetMotionPara(int nAxis,MOTIONPARA &para);
	BOOL SetMotionPara(int nAxis,const MOTIONPARA &para);
	BOOL GetBufferMovePara(int nWorkType,double dbValue,MOTOR_PARA &para);//�����˶���ʽ���˶�����Ϊ������������ֵ
	
	//����Ϊ����ָ����Ժ�������
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
	MOTIONPARA	m_para[4];		//ԭʼ�˶�����
	BASEPARA	m_basePara[4];	//����ת��������ղ���
	BOOL		m_bIsRun;		//�Ƿ���������,������
};

extern CMotion g_Motion; 

#endif //HL_MOTION_PARA_DEFINE_20101023_1202_XLS0_H