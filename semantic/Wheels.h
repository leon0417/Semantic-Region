#ifndef WHEELS_H 
#define WHEELS_H

#include "Motor.h"

class CWheel
{
public:
	CWheel(CMotor* p_leftWheel = NULL, CMotor* p_rightWheel = NULL):pLeftWheel(p_leftWheel), pRightWheel(p_rightWheel)
	{
	};

	void   Swerve( double angle, double turn_speed );//ת��
	void   Forward( double distance,double speed );//ǰ��
	void   Backward( double distance, double speed );//����
	void   BothWheelsStop();
	void   BothWheelsRun();
	void   BothWheelsStopReducer();//����ֹͣ
	bool   WheelsArrived();
private:
	CMotor* pLeftWheel;
	CMotor* pRightWheel;
};

#endif