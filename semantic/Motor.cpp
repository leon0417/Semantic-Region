//////////////////////////////////////////////////////////////////////////
// by Wang Ke [7/11/2010 Administrator]
//----------------------------------------
#include "stdafx.h"
#include "Motor.h"

//----------------------------------------
//////////////////////////////////////////////////////////////////////////
bool CMotor::drive(CString &str)
{
	//--------------------------------------------
	const unsigned int len = 11;
	unsigned char buf[len];
	memset(buf, 0, len);
	//--------------------------------------------
	BYTE datalen = 0;
    int n        = 1;
	unsigned int ID = m_ID;
	//--------------------------------------------
	buf[0] = 0xaa;
	buf[1] = (unsigned char)ID;
	for ( int i = 2; i < len; ++i )
	{
		n = str.Find(' ');
		buf[i] = atoi( str.Mid( 0, n ) );
		str.Delete( 0, n + 1 );
		datalen++;
	}
	buf[10] = 0xff;
	//--------------------------------------------
	int m = CAN_Send->WriteToPort( buf, len );
	Sleep(20);
	//--------------------------------------------
	return true;
}

bool CMotor::drive_broadcast(CString &str)
{
	//--------------------------------------------
	const unsigned int len = 11;
	unsigned char buf[len];
	memset(buf, 0, len);
	//--------------------------------------------
	BYTE datalen = 0;
    int n        = 1;
	unsigned int ID = 0;
	//--------------------------------------------
	buf[0] = 0xaa;
	buf[1] = (unsigned char)ID;
	for ( int i = 2; i < len; ++i )
	{
		n = str.Find(' ');
		buf[i] = atoi( str.Mid( 0, n ) );
		str.Delete( 0, n + 1 );
		datalen++;
	}
	buf[10] = 0xff;
	//--------------------------------------------
	int m = CAN_Send->WriteToPort( buf, len );
	Sleep(100);
	//--------------------------------------------
	return true;
}
//原程序，驱动器更换前
//----------------------------------------
void CMotor::runByPos(double speed, double pos)
{
	//-----------------------------------
	BOOL         dir = (pos<0);	
	BYTE         pos1,pos2,pos3,pos4;
	BYTE         speed1;
	//-----------------------------------
	int          circle,pulse;
	unsigned int abspos;
	//-----------------------------------------------------------------
	if( pos_radian_LH - 0.5 && pos < pos_radian_LH + 0.5 )
	{
		//-------------------
		abspos = abs(int(pos/360.0*1024*dscale));
		circle = abspos/1024;
		pulse  = abspos%1024;
		//-------------------
		double   temp=speed/7020*70.0/360.0*60.0*dscale+0.5;
		speed1 = abs(int(temp));
		//-------------------
		pos1   = circle>>8;
		pos2   = circle;
		pos3   = pulse>>8;
		pos4   = pulse;
		//-------------------
		//	speed1=abs(int(speed));
		if(speed1==0)	speed1=1;
		//-----------------------------------------------------------------
		CString str;
		str.Format("%d %d %d %d %d %d %d ",COM_ABS_RUN,dir,speed1,pos1,pos2,pos3,pos4);	
		drive(str);
	}
	return;
	//	
}

//----------------------------------------
void CMotor::runByAddPos(double speed, double angle)
{
    //-----------------------------------------------------------------
	BOOL dir=(angle<0);
	//-----------------------------------------------------------------
	BYTE pos1,pos2,pos3,pos4;

	BYTE thespeed0, thespeed1;
	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	
	//-----------------------------------------------------------------
	if( m_ID == LEFT_WHEEL || m_ID == RIGHT_WHEEL )
	{
		//circle = abspos/1024;//2048;
		//pulse  = abspos%1024;//2048;
		//speed1 = abs(int(speed));
		thespeed0 = (int)abs(speed);
		thespeed1 = (int)abs(speed) >> 8;
	}
	else
	{
		//abspos = abs(int(pos/360.0*1024*dscale));

		thespeed0 = (int)abs(speed);
		thespeed1 = (int)abs(speed) >> 8;

		double position = angle * this->motor_line * this->dscale / 360 * PI;
		int pos = (int)position;

		BYTE pos0 = (int)pos;
		BYTE pos1 = (int)pos >> 8;
		BYTE pos2 = (int)pos >> 16;
		BYTE pos3 = (int)pos >> 24;
		//circle = abspos/1024;
		//pulse  = abspos%1024;
		//
		//double temp = speed;
		//
		//speed1 = abs(int(temp));
		//
		//pos1 = circle>>8;
		//pos2 = circle;
		//pos3 = pulse>>8;
		//pos4 = pulse;
		//speed1=abs(int(speed));
		//-----------------------------------------------------------------
		if((pos_radian>pos_dis_LH-0.5&&dir)||(pos_radian<pos_dis_LL+0.5&&!dir)||!bES)
		{
			CString str;
			//str.Format("%d %d %d %d %d %d %d ",COM_REL_RUN,dir,speed1,pos1,pos2,pos3,pos4);
			str.Format("%d %d %d %d %d %d %d %d ",COM_REL_DATA0, COM_REL_DATA1, thespeed0, thespeed1, pos0, pos1, pos2, pos3);
			drive(str);
			run();
		}
		//-----------------------------------------------------------------
	}
	return;
    //
}
//----------------------------------------
void CMotor::runByV(double speed)
{
	//
	BOOL dir=(speed<0);
	//
    //double thespeed1  = 0;//int(speed/(2*PI*R) * dscale * 60 / dmaxspeed * 70 + 0.5);
	//double thespeed2  = 5;

	//UINT16 thespeed = 200;

	BYTE thespeed0 = (int)speed;
	BYTE thespeed1 = (int)speed >> 8;

	//
	CString str;
	str.Format("%d %d %d %d %d %d %d %d ",COM_SPEED_MODEL_DATA0, COM_SPEED_MODEL_DATA1, thespeed0, thespeed1, 0, 0, 0, 0);		
	drive(str);		
	run();
    //
	return;
}
//----------------------------------------
void CMotor::setToZero(double speed)
{
	//
	BOOL dir=(speed<0);
	//
    //double thespeed1  = 0;//int(speed/(2*PI*R) * dscale * 60 / dmaxspeed * 70 + 0.5);
	//double thespeed2  = 5;

	UINT16 thespeed = (int)speed;

	BYTE thespeed0 = thespeed;
	BYTE thespeed1 = thespeed >> 8;
	
	//
	CString str;
	str.Format("%d %d %d %d %d %d %d %d ", COM_SET_ZERO_DATA0, COM_SET_ZERO_DATA1, thespeed0, thespeed1, 0, 0, 0, 0);		
	drive(str);		
	run();
    //
	return;
}
//----------------------------------------
void CMotor::zero(double Motion_Speed, double current)
{
	//-----------------------------
	CString str;
    //	double dspeed;
	int speed1,speed2,current_num,rotate_direction;
	current_num = int(current / 12.0*1024);
	//-----------------------------
	if(Motion_Speed>=0) 
		rotate_direction = 0; 
	else 
	{
		rotate_direction = 1;
		Motion_Speed     = -Motion_Speed;
	}
	//-----------------------------
	//	Motion_Speed=Motion_Speed
	speed1   = abs(int(Motion_Speed*70.0+0.5));
	speed2   = 0;
	BYTE       current1,current2;
	current1 = current_num >> 8;
	current2 = current_num;
	//-----------------------------
	str.Format("%d %d %d %d %d %d ",COM_CONSTRAINT_RUN,rotate_direction,speed1,speed2, current1,current2);
	//发送位移指令计数值
	SendCounter++;
	//
	drive(str);
	//
	return;
}
//----------------------------------------
void CMotor::getState(void)
{
	//
	CString str;
	str.Format("%d %d ",COM_REQSTATE_DATA0,COM_REQSTATE_DATA1);
	drive(str);
	//
	return;
}
//----------------------------------------
void CMotor::clearAbsPos(void)
{
	//
	CString str;
	str.Format("%d %d %d %d ", COM_CLRABSPOS_DATA0, COM_CLRABSPOS_DATA1, COM_CLRABSPOS_DATA2, COM_CLRABSPOS_DATA3);
	drive(str);
	pos_radian = 0;
	//
	return;
}
//----------------------------------------
void CMotor::clearAlarm(void)
{
	//
	CString str;
	str.Format("%d %d %d ",COM_CLRFAULT_DATA0,COM_CLRFAULT_DATA1,COM_CLRFAULT_DATA2);
    drive(str);
	//
	return;
}
//----------------------------------------
void CMotor::getAbsPos(void)
{
	//
	CString str;
	str.Format("%d %d ",COM_GETABSPOS_DATA0,COM_GETABSPOS_DATA1);
	drive(str);
	//
	return;
}
//----------------------------------------
void CMotor::getRelPos(void)
{
	//
	CString str;
	str.Format("%d %d ",COM_GETRELPOS_DATA0,COM_GETRELPOS_DATA1);
	drive(str);
    //
	return;
}
//----------------------------------------
void CMotor::setES(void)
{
	//
	CString str;
	str.Format("%d %d %d ",COM_EMG_DATA0,COM_EMG_DATA1,COM_EMG_DATA2);
	drive(str);
	//
	return;
}
//----------------------------------------
void CMotor::stop()
{
    CString str;
	str.Format("%d %d %d %d %d %d %d %d ",COM_DRVSTOP_DATA0, COM_DRVSTOP_DATA1, 0, 0, 0, 0, 0, 0);
	drive(str);
	run();
	return;
}


void CMotor::stop_broadcast()
{
    CString str;
	str.Format("%d %d %d %d %d %d %d %d ",COM_DRVSTOP_DATA0, COM_DRVSTOP_DATA1, 0, 0, 0, 0, 0, 0);
	drive(str);
	//run();
	return;
}

//驱动器减速停止
void CMotor::stop_reducer()
{
    CString str;
	str.Format("%d %d %d %d %d %d %d %d ",COM_DRVSTOPREDUCER_DATA0, COM_DRVSTOPREDUCER_DATA1, COM_DRVSTOPREDUCER_DATA2, COM_DRVSTOPREDUCER_DATA3, 0, 0, 0, 0);
	drive(str);
	//run();
	return;
}


//----------------------------------------
bool CMotor::start()
{
    //------------------------------------
	CString str;
	str.Format("%d %d %d %d %d %d %d %d ", COM_DRVSTART_DATA0, COM_DRVSTART_DATA1, COM_DRVSTART_DATA2, COM_DRVSTART_DATA3, 0, 0, 0, 0);
	if (!drive(str))
		return false;
	//------------------------------------
    return true;
}
//----------------------------------------
bool CMotor::run()
{
	//------------------------------------
	CString str;
	str.Format("%d %d %d %d %d %d %d %d ", COM_RUN_DATA0, COM_RUN_DATA1, 0, 0, 0, 0, 0, 0);

	if (!drive(str))
		return false;
	//------------------------------------
	return true;
}

bool CMotor::run_broadcast()
{
	//------------------------------------
	CString str;
	str.Format("%d %d %d %d %d %d %d %d ", COM_RUN_DATA0, COM_RUN_DATA1, 0, 0, 0, 0, 0, 0);

	if (!drive_broadcast(str))
		return false;
	//------------------------------------
	return true;
}
//----------------------------------------
void CMotor::turn(double angle,double _turnSpeed)//相对位置运行(new)
{
	BYTE thespeed0, thespeed1;
	//----------------------------------------
	pos_radian_des += angle;
	//----------------------------------------
	if (((pos_radian_LL + 0.5 >= pos_radian ) || ( pos_radian >= pos_radian_LH - 0.5)) && !bInitializing || abs(angle) < MOTOR_ARRIVED_THRESHOULD)
	{
		bArrived  = true;
		bPArrived = true; //没有封装同步获得电机状态的API函数，是一个失败
		//
		pos_radian_des = pos_radian;
		//
		setPAFlags();
		//
		return;
	}
	//----------------------------------------
    if ((pos_radian_LL + 0.5 <= pos_radian ) && ( pos_radian <= pos_radian_LH - 0.5) || bInitializing)
    {
		//---------------------------------------------------------------
		CString str;
		int dir = 0;    // 方向变量 
		//---------------------------------------------------------------
		if (angle < 0)
		  dir = 1;      // 反向运行
		//---------------------------------------------------------------
		//BYTE speed1 = int( _turnSpeed / 360.0 * dscale * 60 / dmaxspeed * 70 + 0.5 );
		//BYTE speed2 = 0;
		thespeed0 = (int)abs(_turnSpeed);
		thespeed1 = (int)abs(_turnSpeed) >> 8;

		double position = angle * this->motor_line * this->dscale / 360 * PI;
		int pos = (int)position;

		BYTE pos0 = (int)pos;
		BYTE pos1 = (int)pos >> 8;
		BYTE pos2 = (int)pos >> 16;
		BYTE pos3 = (int)pos >> 24;
		//---------------------------------------------------------------
		//long p     = long( abs(angle) / 360.0 * 1024.0 * dscale );
		//---------------------------------------------------------------
		//int  quan,pluse;
		//BYTE pos1,pos2,pos3,pos4;
		//----------------------------------------
		//quan  = p / 1024;
		//pluse = p % 1024;
		//----------------------------------------
		//pos1  = quan >> 8;
		//pos2  = quan;
		//pos3  = pluse >> 8;
		//pos4  = pluse;
        //----------------------------------------
		str.Format("%d %d %d %d %d %d %d %d ",COM_REL_DATA0, COM_REL_DATA1, thespeed0, thespeed1, pos0, pos1, pos2, pos3);
		//----------------------------------------
		//发送位移指令计数值
		SendCounter ++;
		//-----------------------------------------
		drive(str);
		//run();
    }
    //----------------------------------------
	return;
}
