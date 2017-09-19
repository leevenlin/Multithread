#include "stdafx.h"
#include "conio.h"
#include "windows.h"

#include "function.h"
#include "SerialPort.h"
#include "command.h"	
#include <vector>

using namespace std;
//sperrry雷达控制指令
unsigned char sperry_1[4] = { 0x00,0x10,0x65,0x75 };
unsigned char sperry_2[5] = { 0x00,0x20,0x62,0x76,0x78 };

extern CSerialPort port;

extern uchar selfcheck ;
extern uchar control;
extern uchar shortp ;
extern uchar record ;
extern uchar playbacktime[8] ;
extern uchar playbackcontrol ;
extern uchar range ;
extern uchar show[4];
extern uchar thresh ;
extern uchar gain ;
extern uchar contrast;
extern uchar turns ;
extern uchar rate ;
extern uchar gear ;
extern uchar recordrange[2] ;
extern uchar recordsector[4];
extern uchar saveaddr[128] ;
extern uchar automode ;
extern uchar semiauto[257];
extern uchar handle[0xffff];




void Selfcheck(void)
{

}
void Control(void)
{
	switch (control)
	{
	case CONTROL::PAUSE:
		Pause();
		break;
	case CONTROL::SHORTP:
		ShortP();
		break;
	case CONTROL::MIDP:
		MidP();
		break;
	default:
		break;
	}
}

void StartRecord(void)
{

}

void StopRecord(void)
{

}

void ShortP(void)
{
	sperry_1[2] = 0x6e;
	sperry_1[3] = 0x7e;

	port.WriteData(sperry_1, 4);
	Sleep(100);
	port.WriteData( sperry_2, 5);
	Sleep(350);
}

void MidP(void)
{
	sperry_1[2] = 0x6d;
	sperry_1[3] = 0x7d;
	port.WriteData(sperry_1, 4);
	Sleep(100);
	port.WriteData(sperry_2, 5);
	Sleep(350);
}


void Pause(void)
{
	sperry_1[2] = sperry_1[2] - 0x08;
	sperry_1[3] = sperry_1[3] - 0x08;

	port.WriteData(sperry_1, 4);
	Sleep(100);
	port.WriteData(sperry_2, 5);
	Sleep(350);
}

void Record(void)
{
	switch (record)
	{
	case RECORD::START:
		StartRecord();
		break;
	case RECORD::STOP:
		StopRecord();
		break;
	default:
		break;
	}
}

TIME PBTime; ///回放时间
void Playback(void)
{
	memcpy(&PBTime, playbacktime, 4 * sizeof(uchar));
	PBTime.year;
	PBTime.month;
	PBTime.day;
}



void PlaybackPause(void)
{

}

void PlaybackStop(void)
{

}

void PlaybackKeepon(void)
{

}

void PlaybackControl(void)
{
	switch (playbackcontrol)
	{
	case PLAYBACK::KEEPON:
		PlaybackKeepon();
		break;
	case PLAYBACK::PAUSEPLAY:
		PlaybackPause();
		break;
	case PLAYBACK::STOPPLAY:
		PlaybackStop();
		break;
	default:
		break;
	}
}

float range_num[11] = { 0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6 };
float range_NM = 0;
void Range(void)
{
	range_NM  = range_num[range-1];

}

_2short ShowSector;
void Show(void)
{
	memcpy(&ShowSector, recordsector, 4 * sizeof(uchar));
	ShowSector.start;
	ShowSector.end;
}

void Thresh(void)
{


}

void Gain(void)
{

}

void Contrast(void)
{

}

void Turns(void)
{

}

void Rate(void)
{

}

void Gear(void)
{

}

ushort SamRange = 0;//采集量程
void RecordRange(void)
{
	memcpy(&SamRange, recordrange, 2 * sizeof(uchar));

}

_2short SamSector;
void RecordSector(void)
{
	memcpy(&SamSector, recordsector, 4 * sizeof(uchar));
	SamSector.start;
	SamSector.end;
}

void SaveAddr(void)
{

}

void Auto(void)
{

}

vector<uchar> TargetNum; //半自动接收数据
void HalfAuto(void)
{
	uchar N = semiauto[0];
	TargetNum.clear();
	for (int i = 0; i < N; i++)
		TargetNum.push_back(semiauto[i + 1]);
}

//RecvHand TmpPool;
vector<vector<ImgPt>> HandPts;
void Handle(void)
{
	uchar AreaNum = handle[0];
	if (AreaNum == 0) {
		HandPts.clear();
		return;
	}
	else
	{
		HandPts.clear(); //清空
		int Index = 1;
		while (!AreaNum) //解析区域
		{
			AreaNum--;
			ushort N;
			memcpy(&N, &handle[Index], 2 * sizeof(uchar));//提取N
			Index += 2; 

			int_to_char tmp_int;
			vector<ImgPt> Pts;
			Pts.clear();
			for (int i = 0; i < N; i++) {	//解析区域中的点
				memcpy(&tmp_int, &handle[Index], 4 * sizeof(uchar));
				Index += 4;
				Pts.push_back(tmp_int.pt); //点压栈
			}
			HandPts.push_back(Pts); //vector压栈
		}
	}
}