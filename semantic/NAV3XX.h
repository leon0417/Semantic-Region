#ifndef _NAV3XX_H_
#define _NAV3XX_H_

#include "SocketComm.h"
#include "afxstr.h"

#define DATA_LENGTH 1440

class LASERNAV350
{
public:
	LASERNAV350( CString IP, CString PORT, CWnd* pWnd ):m_strServer(IP), m_strPort(PORT), pLaserOwner(pWnd) 
	{
		m_strBuff.Empty();
	}
	~LASERNAV350() {}
	bool SocketClientInit();
	void LaserInit();
	bool GetInitMessage();
	void StringFormat(CString &str);
	void GetLaserData();
	bool DataIsFull();
	void WriteLaserData();
	void FormatLaserData();
	long ASC2HEX( CString str );
	void LASERDATA2FILE();
	void returnLaserData( double data[], int len );
private:
	CString m_strPort;
	CString m_strServer;
	CSocketComm m_SocketObject;
	CWnd* pLaserOwner;
	CString m_strBuff;
	long LongData[1440];
public:
	void pushInStrBuff( CString buff )
	{
		m_strBuff += buff;
	}
	long returnIthLaserData( int i )
	{
		return LongData[i];
	}
	void clearBuff()
	{
		m_strBuff.Empty();
	}
};

#endif