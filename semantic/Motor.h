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
#define LeftWheel_dscale           71.3//减速比
#define LeftWheel_dmaxspeed        7020
#define LeftWheel_speed            0.15//0.2m/s
#define LeftWheel_pos_radian_LH    65535
#define LeftWheel_pos_radian_LL    -65535
#define LeftWheel_Line             256//编码器线数
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
#define LeftElbow_pos_radian_LH    40 //度
#define LeftElbow_pos_radian_LL    0  //度
#define LeftElbow_Line             256
//-------------------------------------
#define RightElbow_dscale          172.4
#define RightElbow_dmaxspeed       7020
#define RightElbow_speed           30
#define RightElbow_pos_radian_LH   40 //度
#define RightElbow_pos_radian_LL   0  //度
#define RightElbow_Line            256
//-------------------------------------
#define LeftSholder_dscale         342//132.44
#define LeftSholder_dmaxspeed	   7020
#define LeftSholder_speed		   30
#define LeftSholder_pos_radian_LH  -40 //度
#define LeftSholder_pos_radian_LL  40  //度
#define LeftSholder_Line           256
//-------------------------------------
#define RightSholder_dscale		   342//132.44
#define RightSholder_dmaxspeed     7020
#define RightSholder_speed		   30 
#define RightSholder_pos_radian_LH 40  //度
#define RightSholder_pos_radian_LL -40 //度
#define RightSholder_Line          256
//-------------------------------------
#define Head_dscale			       78.54 //
#define Head_dmaxspeed			   7020   //
#define Head_speed				   10     // 10degree/s
#define Head_pos_radian_LH         80  //度    // 软限位
#define Head_pos_radian_LL         -80 //度  // 软限位
#define Head_Line                  256
//-------------------------------------
#define LeftArm_dscale             194.68
#define LeftArm_dmaxspeed          7020
#define LeftArm_speed              30
#define LeftArm_pos_radian_LH      80  //度
#define LeftArm_pos_radian_LL      -80 //度
#define LeftArm_Line               256
//-------------------------------------
#define RightArm_dscale            194.68
#define RightArm_dmaxspeed         7020
#define RightArm_speed             30
#define RightArm_pos_radian_LH     80  //度
#define RightArm_pos_radian_LL     -80 //度
#define RightArm_Line              256
//-------------------------------------
//#define R  0.06						//轮子半径6cm
#define R						   6 //轮子半径7.5cm
#define WheelDistance			   43.7 //轮距43.7cm
#define PI						   3.1415926
//-------------------------------------

// 电机ID
//----------------
typedef enum {
		LEFT_WHEEL = 10,   // 左轮0a
		RIGHT_WHEEL = 20,  // 右轮14
	
} MotoType;

#define MOTOR_NUMBER  7  //全部电机数量
#define INITAL_MOTOR_NUMBER  5  //需要归0的电机数量，轮子不需要归0
//
//-------------------------------------
//驱动器指令
//----------------
//获取相对位置信息
#define COM_GETRELPOS_DATA0   0x01 
#define COM_GETRELPOS_DATA1   0x40
//获取绝对位置信息
#define COM_GETABSPOS_DATA0   0x01
#define COM_GETABSPOS_DATA1   0x41
//状态查询
#define COM_REQSTATE_DATA0    0x01
#define COM_REQSTATE_DATA1    0x42
//驱动器停止(new)
#define COM_DRVSTOP_DATA0     0x00
#define COM_DRVSTOP_DATA1     0x90
//驱动器减速停止(new)
#define COM_DRVSTOPREDUCER_DATA0 0x00
#define COM_DRVSTOPREDUCER_DATA1 0x40
#define COM_DRVSTOPREDUCER_DATA2 0x02
#define COM_DRVSTOPREDUCER_DATA3 0x00

//驱动器启动(new)
//#define COM_DRVSTART_DATA0
//#define COM_DRVSTART_DATA1
#define COM_DRVSTART_DATA0    0x00
#define COM_DRVSTART_DATA1    0x40
#define COM_DRVSTART_DATA2    0x03
#define COM_DRVSTART_DATA3    0x00
//绝对位置清零(new)
#define COM_CLRABSPOS_DATA0   0x00
#define COM_CLRABSPOS_DATA1   0x40
#define COM_CLRABSPOS_DATA2   0x06
#define COM_CLRABSPOS_DATA3   0x00
//限位运行
#define COM_CONSTRAINT_RUN    0x06
//绝对位置运行
#define COM_ABS_RUN           0x07
//相对位置运行
#define COM_REL_RUN           0x08
//相对位置运行(new)
#define COM_REL_DATA0         0x00
#define COM_REL_DATA1         0x82
//电机运行(new)
#define COM_RUN_DATA0         0x00
#define COM_RUN_DATA1         0xa0
//急停
#define COM_EMG_DATA0         0x09
#define COM_EMG_DATA1         0x55
#define COM_EMG_DATA2         0x55
//故障清除
#define COM_CLRFAULT_DATA0    0x0B
#define COM_CLRFAULT_DATA1    0xCC
#define COM_CLRFAULT_DATA2    0xCC
//归0(new)
#define COM_SET_ZERO_DATA0    0x01
#define COM_SET_ZERO_DATA1    0x70
#define COM_SET_ZERO_SPEED    200
//速度模式(new)
#define COM_SPEED_MODEL_DATA0 0x00
#define COM_SPEED_MODEL_DATA1 0x70

//速度位置(new)
#define COM_ABS_SPEED_POS_DATA0   0x00
#define COM_ABS_SPEED_POS_DATA1   0x81   //-2000000000-2000000000


//超时
#define ACTION_TIME   500   // 舞蹈定时器时间(500ms)
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
	bool   drive(CString &str);						// 发送驱动指令
	bool   drive_broadcast(CString &str);
public:
	//--------------------------------------------------
	bool   start(void);                              //电机启动
	bool   run(void);                                //电机运行(new)
	bool   run_broadcast();                          //for wheels
	void   stop(void);                               //电机停止 
	void   stop_broadcast();                         //for wheels
	void   stop_reducer();	                         //驱动器减速停止

    void   setES(void);                              //设置急停
	void   zero(double Motion_Speed,double current); //电流归零
	void   runByV(double speed);                     // 速度运行
	void   runByAddPos(double speed,double  pos);    // 相对位置运行
	void   runByPos(double speed,double pos);        // 绝对位置运行
	//
	void   turn(double angle, double _turnSpeed = 20);
	//struct WheelTurn swerve(double angle,double turn_speed);
	
	//--------------------------------------------------
	void   clearAlarm(void);                         // 清除警报
	void   clearAbsPos(void);                        // 绝对位置清零
	//--------------------------------------------------
    void   getRelPos(void);                          // 获得相对位置
    void   getAbsPos(void);                          // 获取绝对位置
	void   getState(void);                           // 获取驱动器状态 
	void   setToZero(double speed);
	//--------------------------------------------------
	inline MotoType getID(void)       { return m_ID;}                                    // 获取电机ID
	inline void     clearArrived(void){ bArrived = false;bPArrived = false; }            // 清除到达标志
	inline bool     Arrived(void)     { return bArrived;} //{(bArrived || bPArrived);}   // 位置或归零到达
	inline bool     pArrived(void)    { return bPArrived;}                               // 位置到达
	inline bool     started(void)     { return bStarted;}
	inline bool     aAlarmed(void)    { return bAlarm;}
	inline BYTE     getError(void)    { return errorCode;}
	//--------------------------------------------------
private:
	//bool   drive(CString &str);						// 发送驱动指令
public:
	//-------------------------------------
	bool bzeroed;                                   // 归零完成标志位
	bool bzero;										// 是否归零 True 归零
	bool bzeroing;									// 是否归零中，true归零中
	long pos_p;										// 绝对位置 脉冲数
	//-------------------------------------
	union
	{
		double pos_dis;								// 绝对位置 距离 米
		double pos_radian;							// 绝对位置 角度 
	};
	//-------------------------------------
	bool   bArrived;								// 到位通知 TRUE 到位 
	bool   bStarted;								// 驱动器是否启动 
	bool   bPArrived;                               // 伪到达,通过查询计算获得;
	bool   bRuning;									// 电机是否运行
	bool   bAlarm;									// 电机是否故障
	BYTE   errorCode;								// 错误代码
	const  double dscale;							// 减速比
	const  double dmaxspeed;	 					// 最大速度
	const  unsigned int motor_line;
	unsigned int initTimeOut;
	unsigned int actTimeOut;
	//-------------------------------------
	union
	{
		double pos_dis_LH;							// 距离上限
		double pos_radian_LH;						// 角度上限
	};
	union
	{
		double pos_dis_LL;      					// 距离下限
		double pos_radian_LL;   					// 角度下限
	};
	union{
		double pos_dis_des;     					// 绝对位置 目标位置
		double pos_radian_des;  					// 绝对位置 目标角度
	};
	//-------------------------------------
	double speed;               //
	//-------------------------------------
	bool   bInitializing;       					// 正在初始化标志位
	bool   bmid;                					// 初始化后要到达指定位置标志位
	bool   bmiding;             					// 初始化后正在到达指定位置标志位
	bool   bES;                					    // 急停标志位
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
	void setPAFlags(void)//设置伪到达
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
	     return false;//如果发送计数器的数值大于等于长度，那么说明还没收到位置到达
	   else
         return _AFlags[SendCounter-1];
	   //
	}
	//--------------------------------------
	std::vector< bool > _AFlags;                   //到达标志
	int  SendCounter;                              //发送电机位移指令计数值
	int  ReceiveCounter;                           //接受电机位移指令计数值

    //--------------------------------------
};

//
//------------------------------------------------------------------------
#endif