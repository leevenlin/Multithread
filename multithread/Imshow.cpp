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


Mat ShowImg(SENDRow, SENDCol, CV_8UC3, Scalar(0,0,0));

void ShowMessege() {

	for (int i = 0; i < 20; i++)
	{
		char scstr[30];
		switch (Client[i].Data_ID)
		{
		case SelfCheck: //雷达自检
			
			selfcheck = 1;
			if (selfcheck)
			{
				char str2[10] = "...ing...";
				sprintf_s(scstr, "SelfCheck:%s", str2);

			}
			else sprintf_s(scstr, "SelfCheck:");
			putText(ShowImg, scstr, Point(10, 20), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case RadarCon: //雷达控制
			switch (control)
			{
			case CONTROL::PAUSE:
				sprintf_s(scstr, "RadarCon:...pause...");
				break;
			case CONTROL::SHORTP:
				sprintf_s(scstr, "RadarCon:...short...");
				break;
			case CONTROL::MIDP:
				sprintf_s(scstr, "RadarCon:...Mid...");
				break;
			default:
				sprintf_s(scstr, "RadarCon: no");
				break;
			}
			putText(ShowImg, scstr, Point(10, 40), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case RECord:  //记录
			switch (record)
			{
			case RECORD::START:
				sprintf_s(scstr, "Record:...START...");
				break;
			case RECORD::STOP:
				sprintf_s(scstr, "Record:...STOP...");
				break;
			default:
				sprintf_s(scstr, "Record: nan");
				break;
			}
			putText(ShowImg, scstr, Point(10, 60), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case ShowBack:
			sprintf_s(scstr, "ShowBack:...ing...");
			putText(ShowImg, scstr, Point(10, 80), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case STopOrRunShowBack:
			switch (playbackcontrol)
			{
			case PLAYBACK::KEEPON:
				sprintf_s(scstr, "ShowBack:...run...");
				break;
			case PLAYBACK::PAUSEPLAY:
				sprintf_s(scstr, "ShowBack:...pause...");
				break;
			case PLAYBACK::STOPPLAY:
				sprintf_s(scstr, "ShowBack:...stop...");
				break;
			default:
				sprintf_s(scstr, "ShowBack: nan");
				break;
			}
			putText(ShowImg, scstr, Point(10, 100), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;

		case Span: //量程
			range_NM;
			sprintf_s(scstr, "Range:...%d...",range_NM);
			putText(ShowImg, scstr, Point(10, 120), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case ShowSection:
			ShowSector.start;
			ShowSector.end;
			sprintf_s(scstr, "ShowSection: %d ~ %d", ShowSector.start, ShowSector.end);
			putText(ShowImg, scstr, Point(10, 140), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case ShowThresh:
			thresh;
			sprintf_s(scstr, "Thresh:...%d...", thresh);
			putText(ShowImg, scstr, Point(10, 160), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case GAIN_m:
			gain;
			sprintf_s(scstr, "Gain:  %.3f",gain);
			putText(ShowImg, scstr, Point(10, 180), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case CONTRAST://对比度
			contrast;
			sprintf_s(scstr, "Contrast:...%d...", contrast);
			putText(ShowImg, scstr, Point(10, 200), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case CntCircle: //累计圈数
			turns;
			sprintf_s(scstr, "CntCircle:...%d...",turns);
			putText(ShowImg, scstr, Point(10, 220), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case SampleRate:
			rate;
			sprintf_s(scstr, "SampleRate: %.3f", rate);
			putText(ShowImg, scstr, Point(10, 240), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case VoltageSelect: //电压档位
			gear;
			sprintf_s(scstr, "VoltageSelect: %d",gear);
			putText(ShowImg, scstr, Point(10, 260), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case SampleSpan://采样量程
			SamRange;
			sprintf_s(scstr, "SampleSpan:...%d...",SamRange);
			putText(ShowImg, scstr, Point(10, 280), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case SampleSetion: //采集扇区
			SamSector.start;
			SamSector.end;
			sprintf_s(scstr, "SampleSetion: %d ~ %d", SamSector.start, SamSector.end);
			putText(ShowImg, scstr, Point(10, 300), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case SaveDest://存储位置
			sprintf_s(scstr, "SaveDest:.......");
			putText(ShowImg, scstr, Point(10, 320), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case AUTO: 
			sprintf_s(scstr, "Auto:.......");
			putText(ShowImg, scstr, Point(10, 340), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case SemiAuto:
			sprintf_s(scstr, "SemiAuto:.......");
			putText(ShowImg, scstr, Point(10, 360), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		case NoAuto:
			sprintf_s(scstr, "NoAuto:.......");
			putText(ShowImg, scstr, Point(10, 380), FONT_HERSHEY_SIMPLEX,
				0.5, 255, 1.5, 8);
			break;
		default:
			break;
		} 
	}

}
void ImgShow()
{
	Mat img = imread("radar2.jpg");
	Mat grayimg;
	cvtColor(img, grayimg,CV_RGB2GRAY );
	cvtColor(ShowImg, ShowImg, CV_RGB2GRAY);
	resize(grayimg, ShowImg, ShowImg.size(), 0, 0, INTER_LINEAR);

	while (1)
	{
		/*ShowMessege();*/
		ShowMessege();

		imshow("1080x1080", ShowImg);
		imwrite("save.jpg", ShowImg);
		waitKey(0);
		Sleep(100);
	}

	


}

