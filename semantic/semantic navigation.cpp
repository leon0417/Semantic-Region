#include "stdafx.h"
#include "semantic navigation.h"
//#include <vector>
#include <utility>

#define REG_ROW 7
#define REG_COL 3
#define SYM_ROW 8
#define SYM_COL 4


int region[REG_ROW][REG_COL] = {{INF,  INF,  0},
								{INF,  INF,  1},
								{INF,  INF,  2},
								{INF,  13,   3},
								{12,   11,   4},
								{9,    10,   5},
								{8,    7,    6}};

int orientation[NUM_SEM_REGION][ORIENTATION];


int symbol[SYM_ROW][SYM_COL] = {{INF,  INF,  24,   0},
								{INF,  INF,  22,   1},
								{INF,  INF,  21,   2},
								{INF,  23,   20,   3},
								{17,   18,   19,   4},
								{16,   15,   14,   5},
								{11,   12,   13,   6},
								{10,   9,    8,    7}};



//DWORD WINAPI SemNav::ThreadCompass(LPVOID lpParameter)
//{
//	if ( bStartCompass )
//	{
//		::SendMessage(m_pWOwner->m_hWnd, WM_USER_COMPASS_SEND_MESSAGE, NULL, NULL);
//		Sleep(50);
//
//	}
//}



void SemNav::findReg()
{
	std::vector<std::pair<int, int>> vec;
	std::pair<int, int> p;
	int x = 0, y = 0;

	for( int i = 0; i < NUM_SEM_SYMBOLE; ++i )
	{
		if ( 1 == sampleMatchingNum[i] )
		{
			returnSYMXY( i, x, y );

			if ( x - 1 < REG_ROW && x - 1 >= 0 && y - 1 < REG_COL && y - 1 >= 0 )
			{
				p = std::make_pair( x - 1, y - 1 );
				vec.push_back( p );
			}				
			else
			{
				p = std::make_pair( 0, 0 );
				vec.push_back( p );
			}


			if ( x - 1 < REG_ROW && x - 1 >= 0 && y < REG_COL && y >= 0 )
			{
				p = std::make_pair( x - 1, y );
				vec.push_back( p );
			}
			else
			{
				p = std::make_pair( 0, 0 );
				vec.push_back( p );
			}

			if ( x < REG_ROW && x >= 0 && y - 1 < REG_COL && y - 1 >= 0 )  
			{
				p = std::make_pair( x, y - 1 );
				vec.push_back( p );
			}
			else
			{
				p = std::make_pair( 0, 0 );
				vec.push_back( p );
			}

			if ( x < REG_ROW && x >= 0 && y < REG_COL && y >= 0 )  
			{
				p = std::make_pair( x, y );
				vec.push_back( p );
			}
			else
			{
				p = std::make_pair( 0, 0 );
				vec.push_back( p );
			}
		}
	}

	std::vector<std::pair<int, int>> vec_reg_cnt;//first region, second count
	int cnt = 0;

	for ( std::vector<std::pair<int, int>>::iterator it = vec.begin(); it != vec.end(); ++it )
	{
		if ( ( 0 != (*it).first || 0 != (*it).second ) && region[(*it).first][(*it).second] != INF )
		{
			x = (*it).first;
			y = (*it).second;
		}
		else
			continue;

		for ( std::vector<std::pair<int, int>>::iterator iter = vec.begin(); iter != vec.end(); ++iter )
		{
			if ( x == (*iter).first && y == (*iter).second )
			{
				*iter = std::make_pair( 0, 0 );
				cnt++;
			}
		}

		vec_reg_cnt.push_back( std::make_pair( region[x][y], cnt ) );
		cnt = 0;
	}

	int max = 0;
	for ( std::vector<std::pair<int, int>>::iterator iter = vec_reg_cnt.begin(); iter != vec_reg_cnt.end(); ++iter )
	{
		if ( max < (*iter).second )
		{
			max = (*iter).second;
		}
	}

	memset( maxRegionNum, 0, NUM_SEM_REGION * sizeof( int ) );

	for ( std::vector<std::pair<int, int>>::iterator iter = vec_reg_cnt.begin(); iter != vec_reg_cnt.end(); ++iter )
	{
		if ( max == (*iter).second )
		{
			maxRegionNum[(*iter).first] = 1;
		}
		else
		{
			maxRegionNum[(*iter).first] = 0;
		}
	}
}

void SemNav::returnSYMXY(int sym, int &x, int &y)
{
	for ( int i = 0; i < SYM_ROW; ++i )
	{
		for ( int j = 0; j < SYM_COL; ++j )
		{
			if (sym == symbol[i][j])
			{
				x = i;
				y = j;
			}
		}
	}
}

void SemNav::returnREGXY(int reg, int &x, int &y)
{
	for ( int i = 0; i < REG_ROW; ++i )
	{
		for ( int j = 0; j < REG_COL; ++j )
		{
			if ( reg == region[i][j] )
			{
				x = i;
				y = j;
			}
		}
	}
}

void SemNav::ReadFromTXT( double data[NUM_SEM_SYMBOLE][DATA_LENGTH], int len, int name )
{
	CString filepath;

	CString name1 = "E://hgl//semantic//data//", name2, name3 = ".txt";

	char buf[10];

	itoa( name, buf, 10 );

	name2.Format(_T("%s"), buf);

	filepath = name1 + name2 + name3;

	CStdioFile m_file(filepath,CFile::modeRead); 
	CString contents;
	int cnt = 0;

	while (m_file.ReadString(contents))  
    {  
        double temp = 0.0;  
        data[name][cnt] = (double)atoi(contents);
        cnt++;
    }
	m_file.Close();
}

void SemNav::InitNav() 
{
	for( int i = 0; i < NUM_SEM_SYMBOLE; ++i )
	{
		ReadFromTXT( navData, DATA_LENGTH, i );
	}

	memset( sampleMatchingNum, 0, NUM_SEM_SYMBOLE * sizeof(int) );
	memset( maxRegionNum, 0, NUM_SEM_REGION * sizeof(int) );

	//HANDLE hThread = CreateThread(NULL, 0, ThreadCompass, NULL, 0, NULL);
	//CloseHandle(hThread);
}

//采集当前激光数据
void SemNav::SampleLaserData()
{	
	NavLaser->returnLaserData( sampleLaserData, DATA_LENGTH );
}


void SemNav::returnRegSym( int curReg, std::vector<int> &sym )
{
	int cur_x, cur_y;

	returnREGXY( curReg, cur_x, cur_y );

	sym.push_back( symbol[cur_x][cur_y] );
	sym.push_back( symbol[cur_x][cur_y + 1] );
	sym.push_back( symbol[cur_x + 1][cur_y] );
	sym.push_back( symbol[cur_x + 1][cur_y + 1] );

	if ( cur_x - 1 >= 0 && region[cur_x - 1][cur_y] != INF )
	{
		sym.push_back( symbol[cur_x - 1][cur_y] );
		sym.push_back( symbol[cur_x - 1][cur_y + 1] );
	}
	

	if ( cur_y + 1 < REG_COL && region[cur_x][cur_y + 1] != INF )
	{		
		sym.push_back( symbol[cur_x][cur_y + 2] );
		sym.push_back( symbol[cur_x + 1][cur_y + 2] );
	}
	

	if ( cur_x + 1 < REG_ROW && region[cur_x + 1][cur_y] != INF )
	{
		sym.push_back( symbol[cur_x + 2][cur_y] );
		sym.push_back( symbol[cur_x + 2][cur_y + 1] );
	}
	

	if ( cur_y - 1 >= 0 && region[cur_x][cur_y - 1] != INF )
	{		
		sym.push_back( symbol[cur_x][cur_y - 1] );
		sym.push_back( symbol[cur_x + 1][cur_y - 1] );
	}
}


void SemNav::CertainSymMatching( int curReg )
{
	std::vector<int> sym;

	returnRegSym( curReg, sym );

	memset( sampleMatchingNum, 0, sizeof(int) * NUM_SEM_SYMBOLE );

	for ( std::vector<int>::iterator iter = sym.begin(); iter != sym.end(); ++iter )
	{
		if ( OneSampleMatching( (double*)navData[(*iter)], (double*)sampleLaserData, DATA_LENGTH ) )
		{
			sampleMatchingNum[(*iter)] = 1;
		}
		else
		{
			sampleMatchingNum[(*iter)] = 0;
		}
	}
}


void SemNav::AllSampleMatching()
{
	for ( int i = 0; i < NUM_SEM_SYMBOLE; ++i )
	{
		if ( OneSampleMatching( (double*)navData + i * DATA_LENGTH, (double*)sampleLaserData, DATA_LENGTH ) )
			sampleMatchingNum[i] = 1;
		else
			sampleMatchingNum[i] = 0;
	}
}

bool SemNav::OneSampleMatching( double src[], double dst[], int len )
{
	const int window = 10; 
	int cnt = 0;
	double temp = 0;
	double meanVal[DATA_LENGTH / window] = {0};

	for( int i = 0; i < len / window; ++i )
	{
		for( int j = 0; j < len; j++ )
		{
			if ( src[j] < MAX_DIS_DIFF && src[j] > MIN_DIS_DIFF && 
				dst[(j - i * window + len) % len] < MAX_DIS_DIFF && dst[(j - i * window + len) % len] > MIN_DIS_DIFF )
			{
				temp = abs( src[j] - dst[(j - i * window + len) % len] );
				meanVal[i] += temp;
				cnt++;
			}
		}
		meanVal[i] = meanVal[i] / cnt;
		cnt = 0;
	}

	for ( int i = 0; i < len / window; ++i )
	{
		if ( meanVal[i] < MIN_DIS )
		{
			return true;
		}
	}
	return false;
}


int SemNav::returnDisSrc2Dst( int srcNum, int dstNum )
{
	int src_x = 0, src_y = 0, dst_x = 0, dst_y = 0;

	returnREGXY( srcNum, src_x, src_y );
	returnREGXY( dstNum, dst_x, dst_y );

	return abs( src_x - dst_x ) + abs( src_y - dst_y );
}


void SemNav::findAllREGPath( int srcNum, int dst[], int cnt, int len, 
	std::vector<int> &bestPath, 
	std::vector<int> &bestOrientation )//起始位置，全部目标位置，第cnt个目标位置，目标位置个数，保存最优路径，保存最优路径对应旋转
{
	bool bFirst = true;
	std::vector<std::vector<int>> tmpPath;
	std::vector<int> tmpBestPath;
	std::vector<int> tmpBestOrien;
	for ( int i = cnt; i < len; ++i )
	{
		if ( bFirst )
		{
			bFirst = false;
			findREGPath( srcNum, dst[i], tmpPath );
			findOrientation( tmpPath, tmpBestPath, tmpBestOrien );
			for( std::vector<int>::iterator iter = tmpBestPath.begin(); iter != tmpBestPath.end(); ++iter )
			{
				bestPath.push_back( (*iter) );
			}
			for ( std::vector<int>::iterator iter = tmpBestOrien.begin(); iter != tmpBestOrien.end(); ++iter )
			{
				bestOrientation.push_back( (*iter) );
			}
		}
		else
		{
			findREGPath( dst[i - 1], dst[i], tmpPath );
			findOrientation( tmpPath, tmpBestPath, tmpBestOrien );
			for( std::vector<int>::iterator iter = tmpBestPath.begin() + 1; iter != tmpBestPath.end(); ++iter )
			{
				bestPath.push_back( (*iter) );
			}
			for ( std::vector<int>::iterator iter = tmpBestOrien.begin(); iter != tmpBestOrien.end(); ++iter )
			{
				bestOrientation.push_back( (*iter) );
			}
		}
		tmpPath.clear();
		tmpBestPath.clear();
		tmpBestOrien.clear();
	}
}

bool SemNav::findSameRegPath( std::vector<int> path, int element )
{
	for ( std::vector<int>::iterator iter = path.begin(); iter != path.end(); ++iter )
	{
		if ( (*iter) == element )
			return true;
	}
	return false;
}

void SemNav::findREGPath( int srcNum, int dstNum, std::vector<std::vector<int>> &path )
{
	//std::vector<std::pair<int, double>> path;
	//std::vector<std::vector<int>> path;
	int copyRegion[REG_ROW][REG_COL];
	memcpy( &copyRegion, &region, REG_ROW * REG_COL * sizeof( int ) );
	std::vector<int> first;

	first.push_back(srcNum);
	path.push_back(first);
	//std::vector<std::vector<int>>::iterator iter = path.begin();
//	int src_x = 0, src_y = 0, dst_x = 0, dst_y = 0;	
	int reg[ORIENTATION] = {0};
	int minDisREG[ORIENTATION] = {INF, INF, INF, INF};
//	int orien;
//
	int dis = returnDisSrc2Dst( srcNum, dstNum );
//
//	returnREGXY( srcNum, src_x, src_y );
//	returnREGXY( dstNum, dst_x, dst_y );
//	findNextReg( srcNum, dstNum, reg, ORIENTATION );
////	orien = findFirstOrientation( reg, ORIENTATION );
//	returnREGNum( srcNum, reg, minDisREG, ORIENTATION );
//	
//
//	int cnt = 0;
	while( dis > 0 )
	{
		int len = path.size();
		for ( int i = 0; i < len; ++i )
		{
			int End = path[i][path[i].size() - 1];
			if ( End != dstNum )
			{
				findNextReg( End, dstNum, reg, ORIENTATION );
				returnREGNum( End, reg, minDisREG, ORIENTATION );

				for ( int j = 0; j < ORIENTATION; ++j )
				{
					if ( minDisREG[j] < INF )
					{
						if ( !findSameRegPath( path[i], minDisREG[j] ) )
						{
							std::vector<int> tmp(path[i]);
							tmp.push_back(minDisREG[j]);
							path.push_back(tmp);
						}
						else
						{
							int x, y;
							returnREGXY( minDisREG[j], x, y );
							region[x][y] = INF;
						}
					}
				}
			}
		}

		int maxLen = path[path.size() - 1].size();

		for( std::vector<std::vector<int>>::iterator iter = path.begin(); iter != path.end(); )
		{
			if ( (*iter).size() < maxLen )   
				iter = path.erase(iter);
			else
				++iter;
		}

		//int newSrc = path[path.size() - 1][path[path.size() - 1].size() - 1];
		//dis = returnDisSrc2Dst( newSrc, dstNum );
		for ( std::vector<std::vector<int>>::iterator iter = path.begin(); iter != path.end(); ++iter )
		{
			if ( dstNum == (*iter)[(*iter).size() - 1] )
			{
				dis = 0;
				break;
			}
		}
	}

	for( std::vector<std::vector<int>>::iterator iter = path.begin(); iter != path.end(); )
	{
		if ( dstNum != (*iter)[(*iter).size() - 1] )
			iter = path.erase(iter);
		else
			++iter;
	}

	memcpy( &region, &copyRegion, REG_ROW * REG_COL * sizeof( int ) );
}

void SemNav::findOrientation( std::vector<std::vector<int>> path, std::vector<int> &bestPath, std::vector<int> &bestOrientation )
{
	std::vector<std::vector<int>> orientation( path.size(), std::vector<int>( REGION_NUM, INF ));
	std::vector<int> turnNum( path.size(), INF );
	std::vector<int> tmpOrientation;

	int lenRow = path.size();
	int lenCol = path[0].size();
	int bestPathNum = INF;
//	int oneOri = INF;
	int cnt = 0;

	for ( int i = 0; i < lenRow; ++i )
	{
		for ( int j = 0; j < lenCol - 1; ++j )
		{
			orientation[i][ path[i][j] ] = findOneOrientation( path[i][j], path[i][j + 1] );
		}
	}

	for ( int i = 0; i < lenRow; ++i )
	{
		for ( int j = 0; j < lenCol - 1; ++j )
		{
			if ( orientation[i][ path[i][j] ] < INF && orientation[i][ path[i][j + 1] ] < INF )
			{
				if ( orientation[i][ path[i][j] ] != orientation[i][ path[i][j + 1] ] )
				{
					cnt++;
				}
			}
		}

		turnNum[i] = cnt;
		cnt = 0;
	}

	int minNum = INF;

	for ( int i = 0; i < turnNum.size(); ++i )
	{
		if ( turnNum[i] < minNum )
		{
			minNum = turnNum[i];
			bestPathNum = i;
		}
	}

	for ( std::vector<int>::iterator iter = path[bestPathNum].begin(); iter != path[bestPathNum].end(); ++iter )
	{
		bestPath.push_back( (*iter) );
	}

	for ( std::vector<int>::iterator iter = orientation[bestPathNum].begin(); iter != orientation[bestPathNum].end(); ++iter )
	{
		//bestOrientation.push_back( (*iter) );
		tmpOrientation.push_back( (*iter) );
	}


	for ( std::vector<int>::iterator iter = bestPath.begin(); iter != bestPath.end() - 1; ++iter )
	{
		bestOrientation.push_back( tmpOrientation[(*iter)] );
	}
}


void SemNav::Navigation( int dst[], int len, int &regNum, int &dstNum )
{
	static bool bFirst = true;
	static int src, times = 0, cnt = 0, tmp_src;
	static std::vector<std::vector<int>> path;
	static std::vector<int> bestPath;
	static std::vector<int> bestOrien;
	char buf[100];

	if ( bFirst )
	{
		bFirst = false;

		SampleLaserData();

		AllSampleMatching();

		findReg();

		for ( int i = 0; i < NUM_SEM_REGION; ++i )
		{
			if ( 1 == maxRegionNum[i] )
			{
				src = i;
				break;
			}
		}

		findAllREGPath( src, dst, cnt, len, bestPath, bestOrien );
		regNum = src;
		tmp_src = src;
		CString name = "d://data//corridor//path.txt";
		CTime time = CTime::GetCurrentTime();
		CString m_strTime = time.Format( "%Y-%m-%d %H:%M:%S" );
		FILE *pFile = fopen(name, "a+");

		char *p = (LPSTR)(LPCTSTR)m_strTime;			
		fwrite( p, 1, m_strTime.GetLength(), pFile );
		fwrite( ":  ", 1, 3, pFile );

		_itoa(src, buf, 10);
		fwrite( buf, 1, strlen(buf), pFile );
		fwrite( "\n", 1, 1, pFile );

		fclose(pFile);
		//findREGPath( src, dst[cnt], path );
		//findOrientation( path, bestPath, bestOrien );
	}
	else
	{
		SampleLaserData();
		//AllSampleMatching();
		CertainSymMatching( src );
		regNum = src;
		if ( tmp_src != src )
		{
			tmp_src = src;
			CString name = "d://data//corridor//path.txt";
			CTime time = CTime::GetCurrentTime();
			CString m_strTime = time.Format( "%Y-%m-%d %H:%M:%S" );
			FILE *pFile = fopen(name, "a+");

			char *p = (LPSTR)(LPCTSTR)m_strTime;			
			fwrite( p, 1, m_strTime.GetLength(), pFile );
			fwrite( ":  ", 1, 3, pFile );

			_itoa(src, buf, 10);
			fwrite( buf, 1, strlen(buf), pFile );
			fwrite( "\n", 1, 1, pFile );

			fclose(pFile);
		}
		findReg();

		if ( 1 == maxRegionNum[bestPath[times]]  )
		{
			double turnAngle = bestOrien[times] * ONE_CIRCLE / 4 - dCompassData;

			if( turnAngle > DOWN_ORIENTATION )
			{
				turnAngle -= ONE_CIRCLE;
			}

			if( turnAngle < -DOWN_ORIENTATION )
			{
				turnAngle += ONE_CIRCLE;				
			}

 			if ( abs( turnAngle ) > MIN_TURN_ANGLE && wheelsReady() )
			{
				clearbWheelsReady();
				Wheels->Swerve( turnAngle, ANGLE_VELOCITY ); 
			}
			else
			{
				if ( wheelsReady() )   Wheels->Forward( MIN_MOVE_DIS, MOVE_VELOCITY );
			}
		}
		else if ( times + 1 < bestPath.size() && 1 == maxRegionNum[bestPath[times + 1]] && 0 == maxRegionNum[bestPath[times]] )
		{
			times += 1;
			src = bestPath[times];
		}
		else
		{
			Wheels->BothWheelsStopReducer();
			//src = 
			bFirst = true;
			times = 0;
			setbWheelsReady();
			path.clear();
			bestPath.clear();
			bestOrien.clear();
		}

		if ( src == dst[cnt] )
		{
			dstNum = dst[cnt];
			cnt++;
			if ( cnt > len - 1 )
			{
				Wheels->BothWheelsStopReducer();
				setFinishFlag();
				bFirst = true;
				times = 0;
				cnt = 0;
				setbWheelsReady();
				path.clear();
				bestPath.clear();
				bestOrien.clear();
			}
		}
	}
}



//求出源区域相邻四个区域中距离目标区域最小距离的区域编号
void SemNav::returnREGNum( int srcNum, int reg[], int minDisREG[], const int len)
{
	int src_x = 0, src_y = 0;

	returnREGXY( srcNum, src_x, src_y );
	//minDisREG[ORIENTATION] = {INF, INF, INF, INF};
	//memset( minDisREG, INF, sizeof(int) * ORIENTATION );
	minDisREG[0] = INF;
	minDisREG[1] = INF;
	minDisREG[2] = INF;
	minDisREG[3] = INF;
	for ( int i = 0; i < len; ++i )
	{
		if ( 1 == reg[i] )
		{
			switch ( i )
			{
				case 0:
					{
						if ( src_x - 1 >= 0 )
						{
							minDisREG[0] = region[src_x - 1][src_y];
						}
						break;
					}
				case 1:
					{
						if ( src_y + 1 < REG_COL )
						{
							minDisREG[1] = region[src_x][src_y + 1];
						}
						break;
					}
				case 2:
					{
						if ( src_x + 1 < REG_ROW )
						{
							minDisREG[2] = region[src_x + 1][src_y];
						}
						break;
					}
				case 3:
					{ 
						if ( src_y - 1 >= 0 )
						{
							minDisREG[3] = region[src_x][src_y - 1];
						}
						break;
					}
				default:
					break;
			}
		}
	}
}


double SemNav::OrientationSetZero( double angle )//角度
{
	angle -= ORIEN0;

	if ( angle < 0 ) angle += ONE_CIRCLE;

	return angle;
}


int SemNav::findFirstOrientation( int reg[], const int len )
{
	double angle = OrientationSetZero( dCompassData );
	double orien[ORIENTATION];

	orien[0] = abs(angle - 0);
	orien[1] = abs(angle - 90);
	orien[2] = abs(angle - 180);
	orien[3] = abs(angle - 270);

	return returnMinNum( orien, ORIENTATION );	
}

int SemNav::returnMinNum( double vec[], int len )
{
	double min = 1000000;
	for ( int i = 0; i < len; ++i )
	{
		if ( vec[i] < min ) min = vec[i];
	}

	for ( int i = 0; i < len; ++i )
	{
		if ( vec[i] == min ) return i;
	}
}

void SemNav::findNextReg( int srcNum, int dstNum, int reg[], const int Len )
{
	int src_x = 0, src_y = 0, dst_x = 0, dst_y = 0;
	
	returnREGXY( srcNum, src_x, src_y );
	returnREGXY( dstNum, dst_x, dst_y );
	
	int dis[ORIENTATION] = {INF, INF, INF, INF};

	if ( src_x - 1 >= 0 && region[src_x - 1][src_y] != INF )
	{
		dis[0] = abs( src_x - 1 - dst_x ) + abs( src_y - dst_y );
	}
	

	if ( src_y + 1 < REG_COL && region[src_x][src_y + 1] != INF )
	{
		dis[1] = abs( src_x - dst_x ) + abs( src_y + 1 - dst_y );
	}
	

	if ( src_x + 1 < REG_ROW && region[src_x + 1][src_y] != INF )
	{
		dis[2] = abs( src_x + 1 - dst_x ) + abs( src_y - dst_y );
	}
	

	if ( src_y - 1 >= 0 && region[src_x][src_y - 1] != INF )
	{
		dis[3] = abs( src_x - dst_x ) + abs( src_y - 1 - dst_y );
	}
	

	int min = INF;

	for ( int i = 0; i < Len; ++i )
	{
		if ( dis[i] < min )
		{
			min = dis[i];
		}
	}

	for ( int i = 0; i < 4; ++i )
	{
		if ( min == dis[i] )
		{
			reg[i] = 1;
		}
		else
			reg[i] = 0;
	}
	
	
}

int SemNav::findOneOrientation( int srcNum, int dstNum )
{
	
	int src_x = 0, src_y = 0, dst_x = 0, dst_y = 0;
	int orientation = INF;
	
	returnREGXY( srcNum, src_x, src_y );
	returnREGXY( dstNum, dst_x, dst_y );

	if ( src_x == dst_x || src_y == dst_y )//srcNum与dstNum相邻性判断
	{
		if ( src_x > dst_x )
		{
			orientation = 0;
		}
		if ( src_y < dst_y )
		{
			orientation = 1;
		}
		if ( src_x < dst_x )
		{
			orientation = 2;
		}
		if ( src_y > dst_y )
		{
			orientation = 3;
		}
	}
	return orientation;
}
