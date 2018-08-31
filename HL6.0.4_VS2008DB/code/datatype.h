#ifndef DiffCorr_DEFINE
#define DiffCorr_DEFINE

#include "CaliLib.h"


typedef struct CALI_PARA{
	CALI_PARA(){
		Clear();
	}
	void Clear(){
		memset(this,0,sizeof(CALI_PARA));
	}
	
	void operator=(const CALI_PARA& src)
	{
		nCaliType=src.nCaliType;
		triPara=src.triPara;
		gridPara=src.gridPara;
		superPara=src.superPara;
		redPara=src.redPara;
		bEnableCoord=src.bEnableCoord;
		nCoord=src.nCoord;
		dbOffsetX=src.dbOffsetX;
		dbOffsetY=src.dbOffsetY;
		dbRotateAngle=src.dbRotateAngle;
	}
	
	UINT nCaliType;	
	//У������ #define CALITYPE_TRA 0
	//  #define CALITYPE_GRID 1 
	// #define CALITYPE_SUPER 2
	
	//��ͳУ����ʽ����
	TRADITIONPARA triPara;
	
	//����У������
	GRIDPARA	gridPara;
	
	//�߾�У������
	SUPERPARA superPara;
	
	//���У������
	REDPARA redPara;
	
	//�����û�����ϵ����
	BOOL bEnableCoord;	//�Ƿ������û�����ϵ 0-������ 1-����
	UINT nCoord;
	double dbOffsetX;
	double dbOffsetY;
	double dbRotateAngle;
}CALI_PARA;
#endif //DiffCorr_DEFINE