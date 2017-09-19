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


float range_num[11] = { 0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6 };
extern UserData Client[];
void ShowMessege() {

	for (int i = 0; i < sizeof(&Client) / sizeof(Client[0]); i++)
	{
		switch (Client[i].Data_ID)
		{
		case SelfCheck:
			if (!Client[i].Info)
			{
	
			}
			break;
		case RadarCon:
			if (!Client[i].Info)
			{
				char data = *(Client[i].Info);
				switch (data)
				{
				case 0x00://暂停
					break;
				case 0x01://短脉冲
					break;
				case 0x02://中脉冲
					break;
				default:
					break;
				}
			}break;
		case Record:
			if (!Client[i].Info)
			{
				char data = *(Client[i].Info);
				switch (data)
				{
				case 0x00://停止
					break;
				case 0x01://记录
					break;
				default:
					break;
				}
			}break;
		case ShowBack:
			if (!Client[i].Info)
			{
				time_to_char Time;
				memcpy(&Time, Client[i].Info, sizeof(time_to_char));//提取时间

			}break;
		case STopOrRunShowBack:
			if (!Client[i].Info)
			{
				char data = *(Client[i].Info);
				switch (data)
				{
				case 0x00://暂停
					break;
				case 0x01://继续
					break;
				case 0x10://停止
					break;
				default:
					break;
				}
			}break;

		case Span:
			if (!Client[i].Info)
			{
				char range = *(Client[i].Info);
				float span =  range_num[range-1];
			}break;
		case ShowSection:
			if (!Client[i].Info)
			{
				int_to_char _4byte;
				memcpy(&_4byte, Client[i].Info, sizeof(int_to_char));
				_4byte.B_and_E.start;
				_4byte.B_and_E.end;
			}break;
		case ShowThresh:
			if (!Client[i].Info)
			{
				uchar data = *(Client[i].Info);

			}break;
		case Gain:
			if (!Client[i].Info)
			{
				uchar data = *(Client[i].Info);
			}break;
		case Contrast:
			if (!Client[i].Info)
			{
				uchar data = *(Client[i].Info);
			}break;
		case CntCircle:
			if (!Client[i].Info)
			{
				uchar data = *(Client[i].Info);
			}break;
		case SampleRate:
			if (!Client[i].Info)
			{
				uchar data = *(Client[i].Info);
			}break;
		case VoltageSelect:
			if (!Client[i].Info)
			{
				uchar data = *(Client[i].Info);
			}break;
		case SampleSpan:
			if (!Client[i].Info)
			{
				unsigned short span;
				memcpy(&span, Client[i].Info, sizeof(unsigned short));

			}break;
		case SampleSetion:
			if (!Client[i].Info)
			{
				int_to_char _4byte;
				memcpy(&_4byte, Client[i].Info, sizeof(int_to_char));
				_4byte.B_and_E.start;
				_4byte.B_and_E.end;
			}break;
		case SaveDest:
			if (!Client[i].Info)
			{

			}break;
		case Auto:
			if (!Client[i].Info)
			{

			}break;
		case SemiAuto:
			if (!Client[i].Info)
			{

			}break;
		case NoAuto:
			if (!Client[i].Info)
			{

			}break;
		default:
			break;
		} 
	}

}
void ImgShow()
{
	Mat img = imread("radar2.jpg");
	Mat ShowImg(1080,1080,CV_8UC3,Scalar(0,0,0));
	resize(img, ShowImg, ShowImg.size(), 0, 0, INTER_LINEAR);
	while (true)
	{
		
		imshow("1080x1080", ShowImg);
		waitKey(0);
		Sleep(100);
	}

	


}

