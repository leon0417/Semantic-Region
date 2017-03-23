#include "stdafx.h"
#include "NAV3XX.h"

bool LASERNAV350::SocketClientInit()
{
	WSADATA	WSAData = { 0 };
	if ( 0 != WSAStartup( WSA_VERSION, &WSAData ) )
	{
		// Tell the user that we could not find a usable
		// WinSock DLL.
		if ( LOBYTE( WSAData.wVersion ) != LOBYTE(WSA_VERSION) ||
			 HIBYTE( WSAData.wVersion ) != HIBYTE(WSA_VERSION) )
			 ::MessageBox(NULL, _T("Incorrect version of WS2_32.dll found"), _T("Error"), MB_OK);

		WSACleanup( );
		return FALSE;
	}
	m_SocketObject.setWndtoComm(pLaserOwner);
	bool bSuccess = false;
	m_SocketObject.SetServerState( false );
	bSuccess = m_SocketObject.ConnectTo( m_strServer, m_strPort, AF_INET, SOCK_STREAM); // TCP
	m_SocketObject.WatchComm();

	return TRUE;
}

void LASERNAV350::StringFormat(CString &str)
{
	str.Insert(str.GetLength(), 0x03);
	str.Insert(0, 0x02);
}

void LASERNAV350::LaserInit()
{
	CString cstr1 = "sMN SetAccessMode 3 F4724744";
	CString cstr2 = "sMN mNEVAChangeState 1";
	CString cstr3 = "sWN NPOSPoseDataFormat 1 0";
	CString cstr4 = "sWN NAVScanDataFormat 1 0";
	CString cstr5 = "sMN mNEVAChangeState 4";

	int cnt = 100000;

	StringFormat(cstr1);
	StringFormat(cstr2);
	StringFormat(cstr3);
	StringFormat(cstr4);
	StringFormat(cstr5);

	m_SocketObject.WriteComm((LPBYTE)(LPCTSTR)cstr1, cstr1.GetLength(), 0);
	//Sleep(100);
	m_SocketObject.WriteComm((LPBYTE)(LPCTSTR)cstr2, cstr2.GetLength(), 0);
	//while ( -1 != m_strBuff.Find("sAN SetAccessMode") && cnt-- )  break;
	//Sleep(100);
	m_SocketObject.WriteComm((LPBYTE)(LPCTSTR)cstr3, cstr3.GetLength(), 0);
	//Sleep(100);
	m_SocketObject.WriteComm((LPBYTE)(LPCTSTR)cstr4, cstr4.GetLength(), 0);
	//Sleep(100);
	m_SocketObject.WriteComm((LPBYTE)(LPCTSTR)cstr5, cstr5.GetLength(), 0);
	//Sleep(100);
}

bool LASERNAV350::GetInitMessage()
{
	if( -1 == m_strBuff.Find("sAN mNEVAChangeState") )
		return 0;
	else
	{
		m_strBuff.Empty();
		return 1;
	}
}

void LASERNAV350::GetLaserData()
{
	CString cstr = "sMN mNPOSGetData 1 1";
	StringFormat(cstr);

	m_SocketObject.WriteComm((LPBYTE)(LPCTSTR)cstr, cstr.GetLength(), 0);

	//FILE *pFile = fopen("d://1.txt", "w");
	
	//fwrite( m_strBuff, 1, m_strBuff.GetLength(), pFile );
	//fclose(pFile);
}

bool LASERNAV350::DataIsFull()
{
	if ( -1 != m_strBuff.Find(0x03) && -1 != m_strBuff.Find("5A0") )
		return 1;
	else
		return 0;
}

void LASERNAV350::WriteLaserData()
{
	FILE *pFile = fopen("d://1.txt", "w");
	
	fwrite( m_strBuff, 1, m_strBuff.GetLength(), pFile );
	fclose(pFile);
}

long LASERNAV350::ASC2HEX( CString str )
{
	long tmp = 0;

	for ( int i = 0; i < str.GetLength(); ++i )
	{	
        if( str[i] >= '0' && str.GetAt(i) <= '9' )
            tmp = tmp * 16 + str.GetAt(i) - '0';
        else if( str[i] <= 'f' && str.GetAt(i) >= 'a' )
            tmp = tmp * 16 + str.GetAt(i) - 'a' + 10;
        else if(str[i] <= 'F' && str.GetAt(i) >= 'A')
            tmp = tmp * 16 + str.GetAt(i) - 'A' + 10;
	}
	return tmp;
}

void LASERNAV350::FormatLaserData()
{
	int pos1 = m_strBuff.Find("5A0") + 4, pos2 = 0;
	m_strBuff = m_strBuff.Right(m_strBuff.GetLength() - pos1);
	//WriteLaserData();
	
	for ( int i = 0; i < DATA_LENGTH; ++i )
	{
		pos1 = 0;
		pos2 = m_strBuff.Find(" ");
		CString a = m_strBuff.Mid( pos1, pos2 );
		LongData[i] = ASC2HEX( m_strBuff.Mid( pos1, pos2 ) );
		pos1 = pos2 + 1;
		m_strBuff = m_strBuff.Right(m_strBuff.GetLength() - pos1);
	}
}

void LASERNAV350::LASERDATA2FILE()
{
	CString num;
	
	FILE *pFile = fopen("d://1.txt", "w");
	
	fwrite( m_strBuff, 1, m_strBuff.GetLength(), pFile );
	fclose(pFile);
}


void LASERNAV350::returnLaserData( double data[], int len )
{
	int pos1 = m_strBuff.Find("5A0") + 4, pos2;
	m_strBuff = m_strBuff.Right(m_strBuff.GetLength() - pos1);

	for ( int i = 0; i < len; ++i )
	{
		pos1 = 0;
		pos2 = m_strBuff.Find(" ");
		CString a = m_strBuff.Mid( pos1, pos2 );
		data[i] = ASC2HEX( m_strBuff.Mid( pos1, pos2 ) );
		pos1 = pos2 + 1;
		m_strBuff = m_strBuff.Right(m_strBuff.GetLength() - pos1);
	}
	clearBuff();

}