#ifndef _SEMANTIC_NAVIGATION_H_
#define _SEMANTIC_NAVIGATION_H_

#include "NAV3XX.h"
#include <vector>
#include "Wheels.h"

#define NUM_SEM_SYMBOLE 25
#define NUM_SEM_REGION  14
#define MAX_DIS_DIFF    20000
#define MIN_DIS_DIFF    20
#define MIN_DIS         900
#define INF             10000
#define ORIENTATION     4      //0: up, 1: right, 2: down, 3: left 
#define UP_ORIENTATION    180
#define RIGHT_ORIENTATION 270
#define DOWN_ORIENTATION  180
#define LEFT_ORIENTATION  90
#define ORIEN0          180
#define ONE_CIRCLE      360
#define REGION_NUM      14
#define ANGLE_VELOCITY  5
#define MIN_TURN_ANGLE  5
#define MIN_MOVE_DIS    1200
#define MOVE_VELOCITY   5

#define WM_USER_COMPASS_SEND_MESSAGE  0x0502

class SemNav
{
public:
	SemNav(LASERNAV350* laser, CWnd* pWnd, CWheel* wheels):NavLaser(laser), m_pWOwner(pWnd), Wheels(wheels), 
		bStartCompass(false), bWheelsReady(true), bNavFin(false)
	{
		
	};
	~SemNav() {};


private:

	LASERNAV350* NavLaser;

	double navData[NUM_SEM_SYMBOLE][DATA_LENGTH];//存放样本点激光数据
	double sampleLaserData[DATA_LENGTH];//采集当前激光数据
	int    sampleMatchingNum[NUM_SEM_SYMBOLE];//匹配的样本点序号
	int    maxRegionNum[NUM_SEM_REGION];
	bool   bStartCompass;
	double dCompassData;
	bool   bWheelsReady;
	bool   bNavFin;

	CWnd*   m_pWOwner;
	CWheel* Wheels;

public:

	void   ReadFromTXT( double data[NUM_SEM_SYMBOLE][DATA_LENGTH], int len, int name );
	void   InitNav();//导航初始化，读取导航点文本数据
	void   SampleLaserData();//采集当前位置激光数据
	bool   OneSampleMatching( double src[], double dst[], int len );//一个样本点匹配
	void   CertainSymMatching( int curReg );
	void   AllSampleMatching();
	void   returnSYMXY(int sym, int &x, int &y);
	void   returnREGXY(int reg, int &x, int &y);
	void   findReg();
	//void   findREGPath( int srcNum, int dstNum );
	void   findAllREGPath( int srcNum, int dst[], int cnt, int len, std::vector<int> &bestPath, std::vector<int> &bestOrientation );
	void   findREGPath( int srcNum, int dstNum, std::vector<std::vector<int>> &path );
	void   findNextReg( int srcNum, int dstNum, int reg[], const int Len );
	void   findOrientation( std::vector<std::vector<int>> path, std::vector<int> &bestPath, std::vector<int> &bestOrientation );
	int    findOneOrientation( int srcNum, int dstNum );//获得一次转移的方向，0: up, 1: right, 2: down, 3: left 
	int    findFirstOrientation( int reg[], const int len );
	bool   findSameRegPath( std::vector<int> path, int element );
	double OrientationSetZero( double angle );
	int    returnMinNum( double vec[], int len );
	void   returnREGNum( int srcNum, int reg[], int minDisREG[], const int len);
	int    returnDisSrc2Dst( int srcNum, int dstNum );
	void   returnRegSym( int curReg, std::vector<int> &sym );
	void   Navigation( int dst[], int len, int &regNum, int &dstNum );
	
	//static DWORD WINAPI ThreadCompass(LPVOID lpParameter);
	//void SampleMatching();

	LASERNAV350* returnLaser()
	{
		return NavLaser;
	}

	void returnCompassData( double compass )
	{
		dCompassData = compass - ORIEN0;
		if ( dCompassData < 0 )
		{
			dCompassData += ONE_CIRCLE;
		}
	}

	void setbWheelsReady()
	{
		bWheelsReady = true;
	}

	void clearbWheelsReady()
	{
		bWheelsReady = false;
	}

	bool wheelsReady()
	{
		return bWheelsReady;
	}

	void setFinishFlag()
	{
		bNavFin = true;
	}

	void clearFinishFlag()
	{
		bNavFin = false;
	}

	bool returnFinishFlag()
	{
		return bNavFin;
	}
};

#endif