// VoiceInteraction.h

#ifndef Voice_Interaction
#define Voice_Interaction

/*
* 语音合成（Text To Speech，TTS）技术能够自动将任意文字实时转换为连续的
* 自然语音，是一种能够在任何时间、任何地点，向任何人提供语音信息服务的
* 高效便捷手段，非常符合信息时代海量数据、动态更新和个性化查询的需求。
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
		const char* login_params;//登录参数,appid与msc库绑定,请勿随意改动
		const char* session_begin_params_tts;//语音合成参数
		const char* session_begin_params_iat;//语音听写（具有语义理解功能）
		const char* filename;//合成的语音文件名称

		VoiceInteraction()
		{
			login_params         = "appid = 562b90e0, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动,appid为科大讯飞语音平台注册时生成，一个账号对应一个，若更换账号，只需更改此处的appid
			/*
			* voice_name:    合成发音人
			* text_encoding: 合成文本编码格式
			* sample_rate:   合成音频采样率
			* speed:         合成音频对应的语速
			* volume:        合成音频的音量
			* pitch:         合成音频的音调
			* rdn:           合成音频数字发音方式
			* 详细参数说明请参阅《iFlytek MSC Reference Manual》
			*/
	    	//session_begin_params_tts = "voice_name = xiaoyan, text_encoding = gb2312, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";//语音合成
		   	session_begin_params_tts = "voice_name = Catherine, text_encoding = gb2312, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";//语音合成

			filename = "tts.wav"; //合成的语音文件名称
		}

		//语音合成，文本合成语音，只需设置文本即可
		void Speak(const char* text);
};


#endif
