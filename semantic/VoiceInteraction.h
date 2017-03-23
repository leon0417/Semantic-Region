// VoiceInteraction.h

#ifndef Voice_Interaction
#define Voice_Interaction

/*
* �����ϳɣ�Text To Speech��TTS�������ܹ��Զ�����������ʵʱת��Ϊ������
* ��Ȼ��������һ���ܹ����κ�ʱ�䡢�κεص㣬���κ����ṩ������Ϣ�����
* ��Ч����ֶΣ��ǳ�������Ϣʱ���������ݡ���̬���º͸��Ի���ѯ������
*/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <errno.h>

#include "msp_cmn.h"
#include "msp_errors.h"

#include "qtts.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

 class VoiceInteraction
{
	public:
		const char* login_params;//��¼����,appid��msc���,��������Ķ�
		const char* session_begin_params_tts;//�����ϳɲ���
		const char* session_begin_params_iat;//������д������������⹦�ܣ�
		const char* filename;//�ϳɵ������ļ�����

		VoiceInteraction()
		{
			login_params         = "appid = 562b90e0, work_dir = .";//��¼����,appid��msc���,��������Ķ�,appidΪ�ƴ�Ѷ������ƽ̨ע��ʱ���ɣ�һ���˺Ŷ�Ӧһ�����������˺ţ�ֻ����Ĵ˴���appid
			/*
			* voice_name:    �ϳɷ�����
			* text_encoding: �ϳ��ı������ʽ
			* sample_rate:   �ϳ���Ƶ������
			* speed:         �ϳ���Ƶ��Ӧ������
			* volume:        �ϳ���Ƶ������
			* pitch:         �ϳ���Ƶ������
			* rdn:           �ϳ���Ƶ���ַ�����ʽ
			* ��ϸ����˵������ġ�iFlytek MSC Reference Manual��
			*/
	    	//session_begin_params_tts = "voice_name = xiaoyan, text_encoding = gb2312, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";//�����ϳ�
		   	session_begin_params_tts = "voice_name = Catherine, text_encoding = gb2312, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";//�����ϳ�

			filename = "tts.wav"; //�ϳɵ������ļ�����
		}

		//�����ϳɣ��ı��ϳ�������ֻ�������ı�����
		void Speak(const char* text);
};


#endif
