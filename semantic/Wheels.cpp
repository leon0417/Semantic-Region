#include "stdafx.h"
#include "Wheels.h"

//------------------------------------------------------
//�������ܣ����û����˵���ת��ǶȺ��ٶȣ�˫�ֲ��٣�ԭ��ת�䣩
//���������angle��ת��ĽǶȣ���λ���ȣ��㣩���涨������תʱ�Ƕ�Ϊ��ֵ������תʱ�Ƕ�Ϊ��ֵ
//          turn_speed��ת��Ľ��ٶȣ���λ����/�루��/s��
//���ز�������
//------------------------------------------------------
void CWheel::Swerve(double angle,double turn_speed)
{
	double motor_speed = 0;//�����ת�٣�r/min
	double motor_angle = 0;//���ת���ĽǶȣ��ȣ��㣩
	double motor_angle_left = 0;//���ֵ��ת���ĽǶ�
	double motor_angle_right = 0;//���ֵ��ת���ĽǶ�

	motor_angle = WheelDistance * abs(angle) / (2 * R);
	motor_speed = WheelDistance * LeftWheel_dscale * turn_speed / (12 * R);//����������ת��
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

	//�������е��������
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
//�������ܣ����û�����ǰ���ٶȺ�λ��
//���������speed��ǰ���ٶȣ���λ��cm/s��
//          distance��ǰ��λ�ƣ���λ��cm��
//���ز�������
//------------------------------------------------------
void CWheel::Forward( double distance, double speed )
{
	double motor_speed = 0;//���ӵ���ٶȣ�r/min
	double motor_angle = 0;//���ӵ��ת���Ƕȣ��ȣ��㣩
	motor_speed = 60 * LeftWheel_dscale * speed/ (2 * PI * R);
	motor_angle = distance * 360 / (2 * PI *R);
	
	//�������е��������
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
//�������ܣ����û����˺����ٶȺ�λ��
//���������speed�������ٶȣ���λ��cm/s��
//          distance������λ�ƣ���λ��cm��
//���ز�������
//------------------------------------------------------
void CWheel::Backward( double distance, double speed )
{
	double motor_speed = 0;//���ӵ���ٶȣ�r/min
	double motor_angle = 0;//���ӵ��ת���Ƕȣ��ȣ��㣩
	motor_speed = 60 * LeftWheel_dscale * speed/ (2 * PI * R);
	motor_angle = distance * 360 / (2 * PI *R);
	
	//�������е��������
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

//����ֹͣ
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