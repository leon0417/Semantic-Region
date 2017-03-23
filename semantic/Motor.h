//////////////////////////////////////////////////////////////////////////
#ifndef MOTOR_H 
#define MOTOR_H
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
// by Wang Ke based on previous robot example [7/11/2010 Administrator]
//------------------------------------------------------------------------
//#include "ControlCAN.h"
#include <vector>
#include "CSerialPort.h"
//-------------------------------------
#define LeftWheel_dscale           71.3//���ٱ�
#define LeftWheel_dmaxspeed        7020
#define LeftWheel_speed            0.15//0.2m/s
#define LeftWheel_pos_radian_LH    65535
#define LeftWheel_pos_radian_LL    -65535
#define LeftWheel_Line             256//����������
//-------------------------------------
#define RightWheel_dscale          71.3
#define RightWheel_dmaxspeed       7020
#define RightWheel_speed           0.15//0.2m/s
#define RightWheel_pos_radian_LH   65535
#define RightWheel_pos_radian_LL   -65535
#define RightWheel_Line            256
//-------------------------------------`
#define LeftElbow_dscale           172.4
#define LeftElbow_dmaxspeed        7020
#define LeftElbow_speed            30
#define LeftElbow_pos_radian_LH    40 //��
#define LeftElbow_pos_radian_LL    0  //��
#define LeftElbow_Line             256
//-------------------------------------
#define RightElbow_dscale          172.4
#define RightElbow_dmaxspeed       7020
#define RightElbow_speed           30
#define RightElbow_pos_radian_LH   40 //��
#define RightElbow_pos_radian_LL   0  //��
#define RightElbow_Line            256
//-------------------------------------
#define LeftSholder_dscale         342//132.44
#define LeftSholder_dmaxspeed	   7020
#define LeftSholder_speed		   30
#define LeftSholder_pos_radian_LH  -40 //��
#define LeftSholder_pos_radian_LL  40  //��
#define LeftSholder_Line           256
//-------------------------------------
#define RightSholder_dscale		   342//132.44
#define RightSholder_dmaxspeed     7020
#define RightSholder_speed		   30 
#define RightSholder_pos_radian_LH 40  //��
#define RightSholder_pos_radian_LL -40 //��
#define RightSholder_Line          256
//-------------------------------------
#define Head_dscale			       78.54 //
#define Head_dmaxspeed			   7020   //
#define Head_speed				   10     // 10degree/s
#define Head_pos_radian_LH         80  //��    // ����λ
#define Head_pos_radian_LL         -80 //��  // ����λ
#define Head_Line                  256
//-------------------------------------
#define LeftArm_dscale             194.68
#define LeftArm_dmaxspeed          7020
#define LeftArm_speed              30
#define LeftArm_pos_radian_LH      80  //��
#define LeftArm_pos_radian_LL      -80 //��
#define LeftArm_Line               256
//-------------------------------------
#define RightArm_dscale            194.68
#define RightArm_dmaxspeed         7020
#define RightArm_speed             30
#define RightArm_pos_radian_LH     80  //��
#define RightArm_pos_radian_LL     -80 //��
#define RightArm_Line              256
//-------------------------------------
//#define R  0.06						//���Ӱ뾶6cm
#define R						   6 //���Ӱ뾶7.5cm
#define WheelDistance			   43.7 //�־�43.7cm
#define PI						   3.1415926
//-------------------------------------

// ���ID
//----------------
typedef enum {
		LEFT_WHEEL = 10,   // ����0a
		RIGHT_WHEEL = 20,  // ����14
	
} MotoType;

#define MOTOR_NUMBER  7  //ȫ���������
#define INITAL_MOTOR_NUMBER  5  //��Ҫ��0�ĵ�����������Ӳ���Ҫ��0
//
//-------------------------------------
//������ָ��
//----------------
//��ȡ���λ����Ϣ
#define COM_GETRELPOS_DATA0   0x01 
#define COM_GETRELPOS_DATA1   0x40
//��ȡ����λ����Ϣ
#define COM_GETABSPOS_DATA0   0x01
#define COM_GETABSPOS_DATA1   0x41
//״̬��ѯ
#define COM_REQSTATE_DATA0    0x01
#define COM_REQSTATE_DATA1    0x42
//������ֹͣ(new)
#define COM_DRVSTOP_DATA0     0x00
#define COM_DRVSTOP_DATA1     0x90
//����������ֹͣ(new)
#define COM_DRVSTOPREDUCER_DATA0 0x00
#define COM_DRVSTOPREDUCER_DATA1 0x40
#define COM_DRVSTOPREDUCER_DATA2 0x02
#define COM_DRVSTOPREDUCER_DATA3 0x00

//����������(new)
//#define COM_DRVSTART_DATA0
//#define COM_DRVSTART_DATA1
#define COM_DRVSTART_DATA0    0x00
#define COM_DRVSTART_DATA1    0x40
#define COM_DRVSTART_DATA2    0x03
#define COM_DRVSTART_DATA3    0x00
//����λ������(new)
#define COM_CLRABSPOS_DATA0   0x00
#define COM_CLRABSPOS_DATA1   0x40
#define COM_CLRABSPOS_DATA2   0x06
#define COM_CLRABSPOS_DATA3   0x00
//��λ����
#define COM_CONSTRAINT_RUN    0x06
//����λ������
#define COM_ABS_RUN           0x07
//���λ������
#define COM_REL_RUN           0x08
//���λ������(new)
#define COM_REL_DATA0         0x00
#define COM_REL_DATA1         0x82
//�������(new)
#define COM_RUN_DATA0         0x00
#define COM_RUN_DATA1         0xa0
//��ͣ
#define COM_EMG_DATA0         0x09
#define COM_EMG_DATA1         0x55
#define COM_EMG_DATA2         0x55
//�������
#define COM_CLRFAULT_DATA0    0x0B
#define COM_CLRFAULT_DATA1    0xCC
#define COM_CLRFAULT_DATA2    0xCC
//��0(new)
#define COM_SET_ZERO_DATA0    0x01
#define COM_SET_ZERO_DATA1    0x70
#define COM_SET_ZERO_SPEED    200
//�ٶ�ģʽ(new)
#define COM_SPEED_MODEL_DATA0 0x00
#define COM_SPEED_MODEL_DATA1 0x70

//�ٶ�λ��(new)
#define COM_ABS_SPEED_POS_DATA0   0x00
#define COM_ABS_SPEED_POS_DATA1   0x81   //-2000000000-2000000000


//��ʱ
#define ACTION_TIME   500   // �赸��ʱ��ʱ��(500ms)
#define INIT_ACTION_TIMEOUT_THREHOLD     30 * 1000 / ACTION_TIME  //(ACTION_TIME)(30s)
#define DANCE_TIMEOUT_THREHOLD      5 * 1000 / ACTION_TIME   //(5s)

//-------------------------------------
#define MOTOR_ARRIVED_THRESHOULD 1
//-------------------------------------
class CMotor
{
public:
	//
	CMotor(MotoType thisID, double scale, double maxspeed,
           double _speed,  double _pos_radian_LL = 0.0, double _pos_radian_LH = 0.0, unsigned int _motor_line = 0, CSerialPort* pCAN_Send = NULL, CSerialPort* pCAN_Recv = NULL, 
		   int devType = 4, DWORD deVind = 0, int canNum = 0)
		   :m_ID(thisID), dscale(scale), dmaxspeed(maxspeed), speed(_speed),
		   pos_radian_LH(_pos_radian_LH), pos_radian_LL(_pos_radian_LL), motor_line(_motor_line), 
		   m_devType(devType), m_deVind(deVind), m_canNum(canNum), 
		   bzero(false),   bzeroing(false), pos_p(0.0), CAN_Send(pCAN_Send), CAN_Recv(pCAN_Recv), 
		   pos_dis(0.0),pos_radian(0.0),pos_radian_des(0.0),pos_dis_des(0.0),
		   bArrived(false),bStarted(false), bRuning(false), bAlarm(false), errorCode(0),bzeroed(false),
		   bmid(false),    bmiding(false),  bES(false),     bEStemp(false),bInitializing(false),
		   SendCounter(0),ReceiveCounter(0), initTimeOut(0), actTimeOut(0)
	{

	};
	//
	~CMotor(){};
	//
	bool   drive(CString &str);						// ��������ָ��
	bool   drive_broadcast(CString &str);
public:
	//--------------------------------------------------
	bool   start(void);                              //�������
	bool   run(void);                                //�������(new)
	bool   run_broadcast();                          //for wheels
	void   stop(void);                               //���ֹͣ 
	void   stop_broadcast();                         //for wheels
	void   stop_reducer();	                         //����������ֹͣ

    void   setES(void);                              //���ü�ͣ
	void   zero(double Motion_Speed,double current); //��������
	void   runByV(double speed);                     // �ٶ�����
	void   runByAddPos(double speed,double  pos);    // ���λ������
	void   runByPos(double speed,double pos);        // ����λ������
	//
	void   turn(double angle, double _turnSpeed = 20);
	//struct WheelTurn swerve(double angle,double turn_speed);
	
	//--------------------------------------------------
	void   clearAlarm(void);                         // �������
	void   clearAbsPos(void);                        // ����λ������
	//--------------------------------------------------
    void   getRelPos(void);                          // ������λ��
    void   getAbsPos(void);                          // ��ȡ����λ��
	void   getState(void);                           // ��ȡ������״̬ 
	void   setToZero(double speed);
	//--------------------------------------------------
	inline MotoType getID(void)       { return m_ID;}                                    // ��ȡ���ID
	inline void     clearArrived(void){ bArrived = false;bPArrived = false; }            // ��������־
	inline bool     Arrived(void)     { return bArrived;} //{(bArrived || bPArrived);}   // λ�û���㵽��
	inline bool     pArrived(void)    { return bPArrived;}                               // λ�õ���
	inline bool     started(void)     { return bStarted;}
	inline bool     aAlarmed(void)    { return bAlarm;}
	inline BYTE     getError(void)    { return errorCode;}
	//--------------------------------------------------
private:
	//bool   drive(CString &str);						// ��������ָ��
public:
	//-------------------------------------
	bool bzeroed;                                   // ������ɱ�־λ
	bool bzero;										// �Ƿ���� True ����
	bool bzeroing;									// �Ƿ�����У�true������
	long pos_p;										// ����λ�� ������
	//-------------------------------------
	union
	{
		double pos_dis;								// ����λ�� ���� ��
		double pos_radian;							// ����λ�� �Ƕ� 
	};
	//-------------------------------------
	bool   bArrived;								// ��λ֪ͨ TRUE ��λ 
	bool   bStarted;								// �������Ƿ����� 
	bool   bPArrived;                               // α����,ͨ����ѯ������;
	bool   bRuning;									// ����Ƿ�����
	bool   bAlarm;									// ����Ƿ����
	BYTE   errorCode;								// �������
	const  double dscale;							// ���ٱ�
	const  double dmaxspeed;	 					// ����ٶ�
	const  unsigned int motor_line;
	unsigned int initTimeOut;
	unsigned int actTimeOut;
	//-------------------------------------
	union
	{
		double pos_dis_LH;							// ��������
		double pos_radian_LH;						// �Ƕ�����
	};
	union
	{
		double pos_dis_LL;      					// ��������
		double pos_radian_LL;   					// �Ƕ�����
	};
	union{
		double pos_dis_des;     					// ����λ�� Ŀ��λ��
		double pos_radian_des;  					// ����λ�� Ŀ��Ƕ�
	};
	//-------------------------------------
	double speed;               //
	//-------------------------------------
	bool   bInitializing;       					// ���ڳ�ʼ����־λ
	bool   bmid;                					// ��ʼ����Ҫ����ָ��λ�ñ�־λ
	bool   bmiding;             					// ��ʼ�������ڵ���ָ��λ�ñ�־λ
	bool   bES;                					    // ��ͣ��־λ
	bool   bEStemp;             //
	//-------------------------------------
private:
	//-------------------------------------
	MotoType    m_ID;
         int    m_devType;
	   DWORD    m_deVind;
	     int    m_canNum;
    CSerialPort*    CAN_Send;
	CSerialPort*    CAN_Recv;
	//-------------------------------------
public:
	//-------------------------------------
	void setPAFlags(void)//����α����
	{
       //
       SendCounter ++;
	   //
	   _AFlags.push_back(true);
	   //
       ReceiveCounter++;
	   //
	   return;
	}
	//-------------------------------------
	void clearAFlags(void)
	{
		if (_AFlags.size() > 0 )
			_AFlags.clear();
		//----------------
		ReceiveCounter = 0;
		SendCounter    = 0;
		//
		return;
	}
	//--------------------------------------
	void setAFlagTrue(void)
	{
       ReceiveCounter ++;
	   //----------------
	   _AFlags.push_back(true);
	   //----------------
	   return;
	}
	//--------------------------------------
	bool getAFlag(void)
	{
	   //
	   if (SendCounter > _AFlags.size())
	     return false;//������ͼ���������ֵ���ڵ��ڳ��ȣ���ô˵����û�յ�λ�õ���
	   else
         return _AFlags[SendCounter-1];
	   //
	}
	//--------------------------------------
	std::vector< bool > _AFlags;                   //�����־
	int  SendCounter;                              //���͵��λ��ָ�����ֵ
	int  ReceiveCounter;                           //���ܵ��λ��ָ�����ֵ

    //--------------------------------------
};

//
//------------------------------------------------------------------------
#endif