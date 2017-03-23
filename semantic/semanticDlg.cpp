
// semanticDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "semantic.h"
#include "semanticDlg.h"
#include "afxdialogex.h"
#include "Motor.h"
#include "CSerialPort.h"
#include <stdio.h>
#include <conio.h>
#include <vector>

//科大讯飞语音合成
#include "VoiceInteraction.h"

VoiceInteraction voice_speak;//定义对象

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


long data1[DATA_LENGTH];

int navRegData[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 11, 13};//0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 95, 8, 6, 2, 7, 1, 11, 0, 4, 13, 3, 9, 12, 10


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CsemanticDlg 对话框


CsemanticDlg::CsemanticDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsemanticDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pCAN_Send = NULL;
	pCAN_Recv = NULL;

	p_leftWheel = NULL;
	p_rightWheel = NULL;

	wheels = NULL;

	nav = NULL;
	

	//读罗盘数据控制命令字
	CompaControl[0] = 0x68;
	CompaControl[1] = 0x04;
	CompaControl[2] = 0;
	CompaControl[3] = 0x04;
	CompaControl[4] = 0x08;

	bCompassReady = false;

	bNavStart = false;

	m_orientation = 0;
}

void CsemanticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsemanticDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_MESSAGE(WM_USER_SOCKET_REV_MESSAGE, OnSocketCommunication)
	ON_MESSAGE(WM_USER_COMPASS_SEND_MESSAGE, OnCompassCommunication)
	ON_BN_CLICKED(IDC_READLASERDATA, &CsemanticDlg::OnBnClickedReadlaserdata)
	ON_BN_CLICKED(IDC_WRITELASERDATA, &CsemanticDlg::OnBnClickedWritelaserdata)
END_MESSAGE_MAP()


LONG CsemanticDlg::OnCompassCommunication(WPARAM ch, LPARAM num)
{
	pCompass->WriteToPort(CompaControl, 5);

	return 0;
}

// CsemanticDlg 消息处理程序
LONG CsemanticDlg::OnSocketCommunication(WPARAM ch, LPARAM num)
{
	CString m_str;
	
	m_str.Format( "%s", (LPBYTE)ch );

	if ( -1 != m_str.Find(0x02) && -1 != m_str.Find(0x03) )
		laser->pushInStrBuff( m_str.Mid(1, num - 2) );
		//nav->returnLaser()->pushInStrBuff( m_str.Mid(1, num - 2) );
	else if ( -1 != m_str.Find(0x02) )
		laser->pushInStrBuff( m_str.Mid(1, num - 1) );
		//nav->returnLaser()->pushInStrBuff( m_str.Mid(1, num - 1) );
	else
		laser->pushInStrBuff( m_str.Mid(0, num) );
		//nav->returnLaser()->pushInStrBuff( m_str.Mid(0, num) );

	return 0;
}



LONG CsemanticDlg::OnCommunication(WPARAM ch, LPARAM port)
{
	if (port <= 0 || port > 4)
		return -1;

	static int CAN_cnt = 0;
	static int Compass_cnt = 0;


	if ( 2 == port )
	{
		if ( 0xac == ch )
		{
			CAN_cnt = 0;
			memset( CAN_Buf, 0, 11 );
			CAN_Buf[CAN_cnt] = ch;
		}
		else
		{
			CAN_cnt++;
			CAN_Buf[CAN_cnt] = (unsigned char)ch;
		}		
	}


	if ( 4 == port )
	{
		if ( 0x68 == ch )
		{
			Compass_cnt = 0;
			memset( Compass_Buf, 0, 14 );
			Compass_Buf[Compass_cnt] = ch;
		}
		else
		{
			Compass_cnt++;
			Compass_Buf[Compass_cnt] = (unsigned char)ch;
		}	

		if ( 13 == Compass_cnt )
		{
			unsigned char  heading[3];
			heading[0] = Compass_Buf[10];
			heading[1] = Compass_Buf[11];
			heading[2] = Compass_Buf[12];

			m_orientation = rcmdBCD2Float(heading, 3, 2);//获得接收数据中的航向角

			bCompassReady = true;
		}
	}

	if ( 10 == CAN_cnt ) 
	{
		CMotor* _theMotor;
		switch (CAN_Buf[1])
		{
		case 0x0a:
			_theMotor   = p_leftWheel;
			break;
		case 0x14:
			_theMotor   = p_rightWheel;
			break;
		default:
			_theMotor   = NULL;
			break;
		}

		if ( NULL != _theMotor )
		{
			if ( 0xaa == CAN_Buf[3] && 0xaa == CAN_Buf[4] && CAN_Buf[5] & 0x01 )//
			{
				_theMotor->bArrived = true;
			}
			else if ( 0xEE == CAN_Buf[3] && 0xEE == CAN_Buf[4] && CAN_Buf[5] & 0xff )
			{
				_theMotor->bAlarm = true;
			}
		}
	}

	return 0;
}

BOOL CsemanticDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	pCAN_Send = new CSerialPort;
	pCAN_Send->InitPort(this, 1U, 115200U, 'N', 8U, 0U);
	pCAN_Send->StartMonitoring();

	pCAN_Recv = new CSerialPort;
	pCAN_Recv->InitPort(this, 2U, 115200U, 'N', 8U, 0U);
	pCAN_Recv->StartMonitoring();

	p_leftWheel  = new CMotor(LEFT_WHEEL, LeftWheel_dscale, LeftWheel_dmaxspeed, LeftWheel_speed, LeftWheel_pos_radian_LL, LeftWheel_pos_radian_LH, LeftWheel_Line, pCAN_Send, pCAN_Recv);
	p_rightWheel = new CMotor(RIGHT_WHEEL, RightWheel_dscale, RightWheel_dmaxspeed, RightWheel_speed, RightWheel_pos_radian_LL, RightWheel_pos_radian_LH, RightWheel_Line, pCAN_Send, pCAN_Recv);

	p_leftWheel->start();//启动所有电机驱动器
	p_rightWheel->start();

	wheels = new CWheel(p_leftWheel, p_rightWheel);

	pCompass = new CSerialPort;
	pCompass->InitPort(this, 4U, 9600U, 'N', 8U, 0U);
	pCompass->StartMonitoring();

	//pCompass->WriteToPort(CompaControl, 5);

	laser = new LASERNAV350(_T("192.168.1.120"), _T("2111"), this);
	laser->SocketClientInit();
	laser->LaserInit();
	
	Sleep(1000);

	if ( laser->GetInitMessage() )
	{
		MessageBoxA("Init Success!");
	}

	nav = new SemNav(laser, this, wheels);

	//laser->returnLaserData( data1, DATA_LENGTH );

	SetTimer(KEYBOARD_TIMER, 50, NULL);
	SetTimer(NAVLASER_TIMER, 50, NULL);
	SetTimer(COMPASS_TIMER,  100, NULL);
	SetTimer(WHEELS_TIMER,   50, NULL);
	

	voice_speak.Speak("hello world");//科大讯飞语音合成
	
	nav -> InitNav();
	//nav -> SampleLaserData();
	//nav -> SampleMatching();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsemanticDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsemanticDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsemanticDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsemanticDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static bool bGetLaserData = true, b1Panle = true, b2Panle = true, b3Panle = true, b4Panle = true, b5Panle = true, b6Panle = true;
	static bool b0 = true, b1 = true, b2 = true, b3 = true, b4 = true, b5 = true, b6 = true, b7 = true, b8 = true, b9 = true, b10 = true, b11 = true, b12 = true, b13 = true;
	static int  len = sizeof(navRegData) / sizeof(int);
	static int  cnt = 0, regNum, dstNum;
	switch(nIDEvent)
	{
	case KEYBOARD_TIMER:
		{
			//short key_down = 0;
			//key_down = GetKeyState(VK_MENU) & 0xff00;

			bool flag_forward = (GetKeyState(87) & 0xff00);		//键值W，前进
			bool flag_backward = (GetKeyState(83) & 0xff00);	//键值S，后退
			bool flag_turn_left = (GetKeyState(65) & 0xff00);	//键值A，左转
			bool flag_turn_right = (GetKeyState(68) & 0xff00);  //键值D，右转
			bool flag_stop = (GetKeyState(16) & 0xff00);        //键值Shift，停止
			bool flag_nav  = (GetKeyState(69) & 0xff00);        //键值E

			bool flag_num1 = (GetKeyState(49) & 0xff00);		//键值1
			bool flag_num2 = (GetKeyState(50) & 0xff00);		//键值2
			bool flag_num3 = (GetKeyState(51) & 0xff00);		//键值3
			bool flag_num4 = (GetKeyState(52) & 0xff00);		//键值4
			bool flag_num5 = (GetKeyState(53) & 0xff00);        //键值5
			bool flag_num6 = (GetKeyState(54) & 0xff00);		//键值6
			bool flag_num7 = (GetKeyState(55) & 0xff00);		//键值7
			bool flag_num8 = (GetKeyState(56) & 0xff00);		//键值8
			bool flag_num9 = (GetKeyState(57) & 0xff00);		//键值9
			bool flag_num0 = (GetKeyState(48) & 0xff00);        //键值0

			if(flag_forward)
			{
				//Sample();
				wheels->Forward( 60*20, 15 );
			}
			else if(flag_backward)
			{
				wheels->Backward( 60*20, 15 );
			}
			else if(flag_turn_left)
			{
				wheels->Swerve( -360 * 3, 15 );
			}
			else if(flag_turn_right)
			{
				wheels->Swerve( 360 * 3, 15 );
			}
			else if(flag_stop)
			{
				//wheels->BothWheelsStop();
				//p_leftWheel->stop();
				bNavStart = false;
				wheels->BothWheelsStopReducer();
			}
			else if ( flag_nav )
			{
				bNavStart = true;
			}

			//读取文本内容
			if(flag_num1)
			{
				ReadText(1);
			}
			if(flag_num2)
			{
				ReadText(2);
			}
			if(flag_num3)
			{
				ReadText(3);
			}
			if(flag_num4)
			{
				ReadText(4);
			}
			if(flag_num5)
			{
				ReadText(5);
			}
			if(flag_num6)
			{
				ReadText(6);
			}
			if(flag_num7)
			{
				ReadText(7);
			}
			if(flag_num8)
			{
				ReadText(8);
			}
			if(flag_num9)
			{
				ReadText(9);
			}
			if(flag_num0)
			{
				ReadText(0);
			}

			break;
		}
	case NAVLASER_TIMER:
		//ReadFromTXT(data1, DATA_LENGTH, 1);
		//int pos = 0;
		if ( bGetLaserData && nav->wheelsReady() )
		{
			nav->returnLaser()->GetLaserData();
			bGetLaserData = false;
		}
		//pos = m_strBuff.Find("5A0");
		//nav->SampleLaserData();
		//laser->returnLaserData( data1, DATA_LENGTH );
		//Sample();
		
		if ( nav->returnLaser()->DataIsFull() )
		{
			bGetLaserData = true;
			if ( bNavStart )
			{
				nav->Navigation( navRegData, len, regNum, dstNum );
				switch ( dstNum )
				{
				case 0:
					/*if ( b1Panle )//停下读展板
					{
						wheels->BothWheelsStopReducer();
						ReadText(0);						
						Sleep(1000);
						nav->setbWheelsReady();
						b1Panle = false;
					}
					*/
					if ( b0 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(0);
						b0 = false;
					}
					break;
				case 1:					
					
					if ( b1 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(1);
						b1 = false;
					}
					break;
				case 2:
					
					if ( b2 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(2);
						b2 = false;
					}
					break;
				case 3:
					
					if ( b3 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(3);
						b3 = false;
					}
					break;
				case 4:
					
					if ( b4 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(4);
						b4 = false;
					}
					break;
				case 5:
					
					if ( b5 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(5);
						b5 = false;
					}
					break;
				case 6:
					
					if ( b6 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(6);
						b6 = false;
					}
					break;
				case 7:
					
					if ( b7 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(7);
						b7 = false;
					}
					break;
				case 8:
					
					if ( b8 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(8);
						b8 = false;
					}
					break;
				case 9:
					
					if ( b9 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(9);
						b9 = false;
					}
					break;
				case 10:
					
					if ( b10 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(10);
						b10 = false;
					}
					break;
				case 11:
					
					if ( b11 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(11);
						b11 = false;
					}
					break;
				case 12:
					

					if ( b12 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(12);
						b12 = false;
					}
				case 13:
					
					if ( b13 )
					{
						wheels->BothWheelsStopReducer();
						ReadText(13);
						b13 = false;
					}
					break;
				default:
					break;
				}
				if ( nav->returnFinishFlag() )
				{
					//cnt++;
					//MessageBoxA("Set finish flag!");
					nav->clearFinishFlag();
					bNavStart = false;
				}
			}
		}
		break;
	case COMPASS_TIMER:
		pCompass->WriteToPort(CompaControl, 5);
		if ( bCompassReady )
		{
			if ( abs ( m_orientation ) > 400 )
				MessageBoxA( "CompassReady is dead!" );
			nav->returnCompassData( m_orientation );
			bCompassReady = false;
		}
		break;
	case WHEELS_TIMER:
		if ( wheels->WheelsArrived() ) 
			nav->setbWheelsReady();
		break;
	default:
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}

//读取文本内容并朗读
void CsemanticDlg::ReadText( int num )
{
	//科大讯飞语音合成
	CStdioFile myFile;
	CString str;

	CString name;
	CString name1 = "./展板翻译/text";
	CString name2;
	CString name3 = ".txt";

	char textNum[100];
	itoa( num, textNum, 10 );
	name2.Format("%s", textNum);

	name = name1 + name2 + name3;

	if ( myFile.Open( name, CFile::modeRead ) )
	{
		myFile.ReadString( str );
		myFile.Close();
	}
	else
	{
		MessageBoxA("Read text error!");
	}
	voice_speak.Speak(str);//科大f讯飞语音合成
}


float CsemanticDlg::rcmdBCD2Float(unsigned char * addr_buf, unsigned char integer_length, unsigned char decimal_length )
{
	unsigned char i,num_byte_integer,num_byte_decimal,flag;
	unsigned long temp1;
	float temp2;
	float result = 0;

		
	temp1 = 0;
	temp2 = 1;
	if( 1 == ( integer_length % 2 ) )
	{
		num_byte_integer = ( integer_length + 1 ) / 2;
		if( 0x10 == ( ( *addr_buf ) & 0xF0 ) )
		{
			flag = 1;
		}
		else
		{
			flag = 0;
		}		
	}
	else
	{
		num_byte_integer = integer_length / 2 + 1;
		if( 0x01 == ( ( *addr_buf ) & 0x0F ) )
		{
			flag = 1;
		}
		else
		{
			flag = 0;
		}
	}
	for( i = 0; i < num_byte_integer; i++ )
	{
		temp1 = temp1 << 8;
		temp1 = ( *( addr_buf + i ) ) + temp1;
	}
	for( i = 0; i < integer_length; i++ )
	{
		result = result + ( (float)( temp1 & 0x0000000F ) ) * temp2;
		temp2 = temp2 * 10;
		temp1 = temp1 >> 4;
	}
	
	temp1 = 0;
	temp2 = 1;
	if( 1 == ( decimal_length % 2 ) )
	{
		num_byte_decimal = ( decimal_length + 1 ) / 2;
	}
	else
	{
		num_byte_decimal = decimal_length / 2;
	}
	for( i = 0; i < num_byte_decimal; i++ )
	{
		temp1 = temp1 << 8;
		temp1 = ( *( addr_buf + i + num_byte_integer ) ) + temp1;
	}
	for( i = 0; i < decimal_length; i++ )
	{
		temp2 = temp2 / 10;
	}
	for( i = 0; i < decimal_length; i++ )
	{
		result = result + ( (float)( temp1 & 0x0000000F ) ) * temp2;
		temp2 = temp2 * 10;
		temp1 = temp1 >> 4;
	}
	

	if( 1 == flag )
	{
		result = 0 - result;
	}

	return( result );
}


void CsemanticDlg::OnBnClickedReadlaserdata()
{
	// TODO: 在此添加控件通知处理程序代码
	laser->GetLaserData();
	//Sample();
}


void CsemanticDlg::OnBnClickedWritelaserdata()
{
	// TODO: 在此添加控件通知处理程序代码
	char num[100];
	char buf[100];
	CString name1 = "d://data//corridor//";
	CString name3 = ".txt";
	CString name;
	if ( laser->DataIsFull() )
	{
		MessageBoxA("Data is Full");
		laser->FormatLaserData();
		GetDlgItem(IDC_EDITFILENAME)->GetWindowTextA((LPTSTR)num, 100);
		name = name1 + (CString)num + name3;

		FILE *pFile = fopen(name, "w");

		for ( int i = 0; i < DATA_LENGTH; ++i )
		{
			int a = laser->returnIthLaserData(i);
			_itoa(a, buf, 10);
			fwrite( buf, 1, strlen(buf), pFile );
			fwrite( "\n", 1, 1, pFile );
		}
		fclose(pFile);
		laser->clearBuff();
	}
}