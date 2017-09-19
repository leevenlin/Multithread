// socketclint.cpp : �������̨Ӧ�ó������ڵ㡣
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
extern TIME PBTime;		//�ط�ʱ��
extern uchar playbackcontrol;
extern float range_NM;	//��λ
extern _2short ShowSector;
extern uchar thresh;
extern uchar gain;
extern uchar contrast;
extern uchar turns;
extern uchar rate;
extern uchar gear;
extern ushort SamRange;		//�ɼ�����
extern _2short SamSector;	//��������
extern uchar saveaddr[128];
extern uchar automode;
extern vector<uchar> TargetNum;			//���Զ���������
extern vector<vector<ImgPt>> HandPts;	//�ֶ���������


Mat ShowImg(1080, 1080, CV_8UC3, Scalar(0, 0, 0));
void ShowMessege() {

	for (int i = 0; i < sizeof(&Client) / sizeof(Client[0]); i++)
	{
		switch (Client[i].Data_ID)
		{
		case SelfCheck: //�״��Լ�
			if(selfcheck)
			break;
		case RadarCon: //�״����
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
		case Record:  //��¼
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

		case Span: //����
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
		case Contrast://�Աȶ�
			contrast;
			break;
		case CntCircle: //�ۼ�Ȧ��
			turns;
			break;
		case SampleRate:
			rate;
			break;
		case VoltageSelect: //��ѹ��λ
			gear;
			break;
		case SampleSpan://��������
			SamRange;
			break;
		case SampleSetion: //�ɼ�����
			SamSector.start;
			SamSector.end;
			break;
		case SaveDest://�洢λ��
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

