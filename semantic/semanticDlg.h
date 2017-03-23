
// semanticDlg.h : 头文件
//

#pragma once
#include "Motor.h"
#include "NAV3XX.h"
#include "Wheels.h"
#include "semantic navigation.h"

#define KEYBOARD_TIMER  10
#define NAVLASER_TIMER  11
#define COMPASS_TIMER   12
#define WHEELS_TIMER    13

// CsemanticDlg 对话框
class CsemanticDlg : public CDialogEx
{
// 构造
public:
	CsemanticDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SEMANTIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LONG    OnCommunication(UINT, LONG);
	afx_msg LONG    OnSocketCommunication(WPARAM ch, LPARAM num);
	afx_msg LONG    OnCompassCommunication(WPARAM ch, LPARAM num);
	DECLARE_MESSAGE_MAP()

private:
	CSerialPort* pCAN_Send;
	CSerialPort* pCAN_Recv;

	CSerialPort* pCompass;

	unsigned char Compass_Buf[14];
	unsigned char CAN_Buf[11];

	CMotor* p_leftWheel;
	CMotor* p_rightWheel;

	CWheel* wheels;

	double  m_orientation;

	char CompaControl[5];  //罗盘控制命令字 写一次读一次角度
	

	LASERNAV350* laser;

	SemNav* nav;

	bool bCompassReady;
	bool bNavStart;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	float rcmdBCD2Float(unsigned char * addr_buf, unsigned char integer_length, unsigned char decimal_length );//三维罗盘接收数据格式转换，得到角度
	void  ReadText( int num );//读取文本内容并朗读

	afx_msg void OnBnClickedReadlaserdata();
	afx_msg void OnBnClickedWritelaserdata();
};
