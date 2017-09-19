// socketclint.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include "Imshow.h"
#include <Windows.h>
#include <iostream>
#include "command.h"

using namespace std;
using namespace cv;


extern float range_num[11];
extern UserData Client[];

extern uchar selfcheck;
extern uchar control;
extern uchar shortp;
extern uchar record;
extern TIME PBTime;		//回放时间
extern uchar playbackcontrol;
extern float range_NM;	//档位
extern _2short ShowSector;
extern uchar thresh;
extern uchar gain;
extern uchar contrast;
extern uchar turns;
extern uchar rate;
extern uchar gear;
extern ushort SamRange;		//采集量程
extern _2short SamSector;	//采样扇区
extern uchar saveaddr[128];
extern uchar automode;
extern vector<uchar> TargetNum;			//半自动接收数据
extern vector<vector<ImgPt>> HandPts;	//手动接受数据


Mat ShowImg(1080, 1080, CV_8UC3, Scalar(0, 0, 0));
void ShowMessege() {

	for (int i = 0; i < sizeof(&Client) / sizeof(Client[0]); i++)
	{
		switch (Client[i].Data_ID)
		{
		case SelfCheck: //雷达自检
			if(selfcheck)
			break;
		case RadarCon: //雷达控制
			switch (control)
			{
			case CONTROL::PAUSE:
			
				break;
			case CONTROL::SHORTP:
				
				break;
			case CONTROL::MIDP:
			
				break;
			default:
				break;
			}
			break;
		case Record:  //记录
			switch (record)
			{
			case RECORD::START:
				
				break;
			case RECORD::STOP:
				
				break;
			default:
				break;
			}
			break;
		case ShowBack:
			break;
		case STopOrRunShowBack:
			switch (playbackcontrol)
			{
			case PLAYBACK::KEEPON:
				
				break;
			case PLAYBACK::PAUSEPLAY:
			
				break;
			case PLAYBACK::STOPPLAY:
				
				break;
			default:
				break;
			}
			break;

		case Span: //量程
			range_NM;
			break;
		case ShowSection:
			ShowSector.start;
			ShowSector.end;
			break;
		case ShowThresh:
			thresh;
			break;
		case Gain:
			gain;
			break;
		case Contrast://对比度
			contrast;
			break;
		case CntCircle: //累计圈数
			turns;
			break;
		case SampleRate:
			rate;
			break;
		case VoltageSelect: //电压档位
			gear;
			break;
		case SampleSpan://采样量程
			SamRange;
			break;
		case SampleSetion: //采集扇区
			SamSector.start;
			SamSector.end;
			break;
		case SaveDest://存储位置
			break;
		case Auto: 
			break;
		case SemiAuto:
			break;
		case NoAuto:
			break;
		default:
			break;
		} 
	}

}
void ImgShow()
{
	Mat img = imread("radar2.jpg");
	
	resize(img, ShowImg, ShowImg.size(), 0, 0, INTER_LINEAR);
	while (true)
	{
		
		imshow("1080x1080", ShowImg);
		waitKey(0);
		Sleep(100);
	}

	


}

