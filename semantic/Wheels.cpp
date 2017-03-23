#include "stdafx.h"
#include "Wheels.h"

//------------------------------------------------------
//函数功能：设置机器人底盘转弯角度和速度（双轮差速，原地转弯）
//输入参数：angle：转弯的角度，单位：度（°）；规定：向左转时角度为负值，向右转时角度为正值
//          turn_speed：转弯的角速度，单位：度/秒（°/s）
//返回参数：无
//------------------------------------------------------
void CWheel::Swerve(double angle,double turn_speed)
{
	double motor_speed = 0;//电机的转速，r/min
	double motor_angle = 0;//电机转动的角度，度（°）
	double motor_angle_left = 0;//左轮电机转动的角度
	double motor_angle_right = 0;//右轮电机转动的角度

	motor_angle = WheelDistance * abs(angle) / (2 * R);
	motor_speed = WheelDistance * LeftWheel_dscale * turn_speed / (12 * R);//计算出电机的转速
	if(angle > 0)
	{
		motor_angle_left = - motor_angle;
		motor_angle_right = - motor_angle;
	}
	else
	{
		motor_angle_left = motor_angle;
		motor_angle_right = motor_angle;
	}

	//启动所有电机驱动器
	pLeftWheel->start();
	pRightWheel->start();

	pLeftWheel->turn(motor_angle_left,motor_speed);
	//pLeftWheel->run();

	pRightWheel->turn(motor_angle_right,motor_speed);
	//pRightWheel->run();
	BothWheelsRun();
	return;
}

//------------------------------------------------------
//函数功能：设置机器人前进速度和位移
//输入参数：speed：前进速度，单位：cm/s；
//          distance：前进位移，单位：cm；
//返回参数：无
//------------------------------------------------------
void CWheel::Forward( double distance, double speed )
{
	double motor_speed = 0;//轮子电机速度，r/min
	double motor_angle = 0;//轮子电机转动角度，度（°）
	motor_speed = 60 * LeftWheel_dscale * speed/ (2 * PI * R);
	motor_angle = distance * 360 / (2 * PI *R);
	
	//启动所有电机驱动器
	pLeftWheel->start();
	pRightWheel->start();

	pLeftWheel->turn(-motor_angle, motor_speed);
	//pLeftWheel->run();

	pRightWheel->turn(motor_angle, motor_speed);
	//pRightWheel->run();
	BothWheelsRun();
	return;
}

//------------------------------------------------------
//函数功能：设置机器人后退速度和位移
//输入参数：speed：后退速度，单位：cm/s；
//          distance：后退位移，单位：cm；
//返回参数：无
//------------------------------------------------------
void CWheel::Backward( double distance, double speed )
{
	double motor_speed = 0;//轮子电机速度，r/min
	double motor_angle = 0;//轮子电机转动角度，度（°）
	motor_speed = 60 * LeftWheel_dscale * speed/ (2 * PI * R);
	motor_angle = distance * 360 / (2 * PI *R);
	
	//启动所有电机驱动器
	pLeftWheel->start();
	pRightWheel->start();

	pLeftWheel->turn(motor_angle, motor_speed);
	//pLeftWheel->run();

	pRightWheel->turn(-motor_angle, motor_speed);	
	//pRightWheel->run();
	BothWheelsRun();
	return;
}


void CWheel::BothWheelsRun()
{
	pRightWheel->run_broadcast();
}

void CWheel::BothWheelsStop()
{
	pLeftWheel->stop_broadcast();
	pRightWheel->stop_broadcast();
	BothWheelsRun();
}

//减速停止
void CWheel::BothWheelsStopReducer()
{
	pLeftWheel->stop_reducer();
	pRightWheel->stop_reducer();
	BothWheelsRun();
}


bool CWheel::WheelsArrived()
{
	if ( true == pLeftWheel->bArrived || true == pRightWheel->bArrived )
	{
		pLeftWheel->bArrived = false;
		pRightWheel->bArrived = false;
		return true;
	}
	else
		return false;
}