#ifndef WHEELS_H 
#define WHEELS_H

#include "Motor.h"

class CWheel
{
public:
	CWheel(CMotor* p_leftWheel = NULL, CMotor* p_rightWheel = NULL):pLeftWheel(p_leftWheel), pRightWheel(p_rightWheel)
	{
	};

	void   Swerve( double angle, double turn_speed );//转弯
	void   Forward( double distance,double speed );//前进
	void   Backward( double distance, double speed );//后退
	void   BothWheelsStop();
	void   BothWheelsRun();
	void   BothWheelsStopReducer();//减速停止
	bool   WheelsArrived();
private:
	CMotor* pLeftWheel;
	CMotor* pRightWheel;
};

#endif