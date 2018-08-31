#ifndef ERROR_HEAD
#define ERROR_HEAD

//������붨��
#define FUN_OK 0
#define STRING_FUN_OK "Funtion Execute Ok"
#define BUF_LEN_ERR 1 //ȡ�������ʱ���峤�Ȳ���
#define STRING_BUF_LEN_ERR "Char Buffer length is not enough"

//������̴������
#define MARK_STOP 10 //��λ����Ѿ���ֹ���
#define STRING_MARK_STOP "Mark Task is stoped by user"
#define MARK_FIFO_BREAK 12	//����
#define STRING_MARK_FIFO_BREAK "data stream to CARD is broken off"
#define MARK_FIFO_FULL  13	//FIFO��
#define STRING_MARK_FIFO_FULL "FIFO is full"
#define LIST_DELAY_TIME_ERR 14 //LISTָ������ʱ���ݴ���
#define STRING_LIST_DELAY_TIME_ERR "List Dealy Time Error"
#define ALREADY_CMD_PAUSE 15 //�Ѿ���������ָ����ͣ״̬
#define STRING_ALREADY_CMD_PAUSE "Already Command Pause"
#define ALREADY_FIFO_PAUSE 16 //�Ѿ����ڶϵ���ͣ״̬
#define STRING_ALREADY_FIFO_PAUSE "Already FIFO PAUSE"


#define MARK_TASK_NO_START 20 //�������δ��ʼ
#define STRING_MARK_TASK_NO_START "Marking Task no Start"
#define MARK_NO_STRAT		21 //ARM7��δ��ʼִ��FIFO�������
#define STRING_MARK_NO_STRAT "CARD does'nt Start to Execute Marking Task"
#define UNSUPPERED_STROKE_TYPE 22 //��֧�ֵ���������
#define STRING_UNSUPPERED_STROKE_TYPE "UnSuppered stroke type"
#define MARK_DOT_NUM_ERR	23 //����������
#define STRING_MARK_DOT_NUM_ERR "Marking Dot Num. Error"
#define MARK_Vector_NUM_ERR 24 //������Ŀ����
#define STRING_MARK_Vector_NUM_ERR "Marking vector Num. Error"
#define MARK_BMP_NUM_ERR	25 //��BMP���ص��������
#define STRING_MARK_BMP_NUM_ERR "Marking BITMAP pixel Num. Error"
#define LAYER_NUM_OVER		26 //�������Ŀ�������ֵ
#define STRING_LAYER_NUM_OVER "Layer Num. is Over Allowed 64 layers"
//�����ܴ������
#define SOFT_ADJ_CURRENT_ERR 30 //�豸�������������ʧ��
#define STRING_SOFT_ADJ_CURRENT_ERR "Soft Adjust Current Test Error,Please Check Hardware!"
#define SOFT_ADJ_CURRENT_DISABLE 31 //�����ļ���δ�������������
#define STRING_SOFT_ADJ_CURRENT_DISABLE "Soft Adjust is disable by cofiguration file"
#define SAME_CURRENT 32			//�뿨��ǰ����ֵ��ͬ
#define STRING_SAME_CURRENT	"Same Current"

//����ʼ���������
#define INI_ERR_CARD_NOFIND  40 //��ʼ�����󣬿�δ�ҵ�
#define STRING_INI_ERR_CARD_NOFIND "Can't Find PCI3000 Maring Card, Please Check connetion!"
#define INI_ERR_CANNOT_RESET 41 //���ܳɹ���λ
#define STRING_INI_ERR_CANNOT_RESET "Can't Reset PCI3000 Card,Please Check CARD!"
#define INI_ERR_CARD_VER_ERR 42 //PCI3000���汾���뱾����Ҫ��İ汾�Ų�ͬ
#define STRING_INI_ERR_CARD_VER_ERR "Card Ver Error,Please Download the suitable Verion Card program(*.Hex)"
#define INI_ERR_CARD_TYPE	 43 //�ǹ����꿨
#define STRING_INI_ERR_CARD_TYPE "CARD program Error!"
#define UNOPEN_CARD 44		//����δ��
#define STRING_UNOPEN_CARD "Please First Open the Card before this operation"
#define CARDINDEX_OVER 45 //����������
#define STRING_CARDINDEX_OVER "CARD Index is over"
#define CARD_OPENED	46	//���Ѿ���
#define STRING_CARD_OPENED "CARD has opened"
#define OPEN_CARD_FAILED 47//����ʧ�ܣ���Ϊ�ڴ���䲻��
#define STRING_OPEN_CARD_FAILED "Open Card Failed because of not enough Memory"
//���������ô������
#define CARDPARA_FILE_CANNOT_OPEN 50 //�����������ļ����ܴ�
#define STRING_CARDPARA_FILE_CANNOT_OPEN "Can't Open the CARD configuration File,please check the path!"

//У����ش������
#define CALI_FILE_CANNOT_OPEN 60	//У���ļ����ܴ�
#define STRING_CALI_FILE_CANNOT_OPEN "Can't open the Calibration FILE,Please CHECK the file is exist"
#define UNSUPPERED_CALI_TYPE  61	//��֧�ֵ�У������
#define STRING_UNSUPPERED_CALI_TYPE "UnSupported calibration type"

//�����ӿڴ������
#define PARA_LENGTH_NOENOUGH	70 //�ַ����������Ȳ���
#define STRING_PARA_LENGTH_NOENOUGH "Parameter Length is not enough"
#define PARA_POLYGON_NUM_ERR	71 ////�������Ŀ����
#define STRING_PARA_POLYGON_NUM_ERR "Polygon list Num. Error"
#define PARA_CURRENT_GRAY_NUM_ERR 72 //�Ҷȵ���������
#define STRING_PARA_CURRENT_GRAY_NUM_ERR "Gray Current List Num. Error"

//���͹���֧�ִ���
#define UNSUPPORT_BMP_MARK		80 //�˻��Ͳ�֧�ִ�Ҷ�λͼ
#define STRING_UNSUPPORT_BMP_MARK "Can't support Gray BITMAP  Marking"

#endif