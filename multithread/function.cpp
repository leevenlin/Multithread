#include "stdafx.h"
#include "conio.h"
#include "windows.h"
#include <opencv2\opencv.hpp>

#include "function.h"
#include "SerialPort.h"
#include "command.h"	
#include <vector>
#include "Imshow.h"
using namespace cv;
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
extern uchar contrast ;
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
float range_NM;
void Range(void)
{
	range_NM  = range_num[range-1];

}

_2short ShowSector; ///显示扇区

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

ushort SamRange;		//采集量程
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
		HandPts.clear();
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

//////////////////////////////////////////////////////数据发送/////////////////////////////

char TCPsend2[2];
char TCPsend3[3];
char TCPsend5[5];
char TCPsend9[9];

char* Selfcheck_Back(void)
{
	TCPsend2[0] = SelfCheck;
	//待定
	return TCPsend2;
}

char* Control_Back(void)
{
	TCPsend2[0] = RadarCon;
	TCPsend2[1] = control;
	return TCPsend2;
}

//void ShortP_Back(void)
//{
//	TCPsend9[0] = 
//}
//
//void MidP_Back(void)
//{
//
//}
//
//void Pause_Back(void)
//{
//
//}

char* Record_Back(void)
{
	TCPsend2[0] = RECord;
	TCPsend2[1] = record;
	return TCPsend2;
}

char* Playback_Back(void)
{
	TCPsend9[0] = ShowBack;
	time_to_char T;
	//获取时间
	//else
	memset(&T.byte, ERR, 8 * sizeof(uchar));
	memcpy(&TCPsend9[1], &T.time, 8 * sizeof(char));
	return TCPsend9;
}

char* PlaybackControl_Back(void)
{
	TCPsend2[0] = STopOrRunShowBack;
	TCPsend2[1] = playbackcontrol;
	return TCPsend2;
}

//void PlaybackStop_Back(void)
//{
//
//}

char* Range_Back(void)
{
	TCPsend2[0] = Span;
	TCPsend2[1] = range;
	return TCPsend2;
}

char* ShowSector_Back(void)
{
	TCPsend5[0] = Span;
	memcpy(&TCPsend5[1], show, 4 * sizeof(char));
	return TCPsend2;
}

char* Thresh_Back(void)
{
	TCPsend2[0] = ShowThresh;
	TCPsend2[1] = thresh;
	return TCPsend2;
}

char* Gain_Back(void)
{
	TCPsend2[0] = GAIN_m;
	TCPsend2[1] = gain;
	return TCPsend2;
}

char* Contrast_Back(void)
{
	TCPsend2[0] = CONTRAST;
	TCPsend2[1] = contrast;
	return TCPsend2;
}

char* Turns_Back(void)
{
	TCPsend2[0] = CntCircle;
	TCPsend2[1] = turns;
	return TCPsend2;
}

char* Rate_Back(void)
{
	TCPsend2[0] = SampleRate;
	TCPsend2[1] = rate;
	return TCPsend2;
}

char* Gear_Back(void)
{
	TCPsend2[0] = VoltageSelect;
	TCPsend2[1] = gear;
	return TCPsend2;
}

char* RecordRange_Back(void)
{
	TCPsend3[0] = SampleSpan;
	memcpy(&TCPsend3[1], recordrange, 2 * sizeof(uchar));
	return TCPsend3;
}

char* RecordSector_Back(void)
{
	TCPsend5[0] = SampleSetion;
	memcpy(&TCPsend5[1], recordsector, 4 * sizeof(char));
	return TCPsend5;
}

char* SaveAddr_Back(void)
{
	char TCPsend129[129];
	TCPsend129[0] = SaveDest;
	memcpy(&TCPsend129[1], saveaddr, 128 * sizeof(char));
	return TCPsend129;
}

char* Mode_Back(void)
{
	TCPsend2[0] = AUTO;
	TCPsend2[1] = AUTO;
	return TCPsend2;

}


extern  Mat ShowImg;
vector<OilCoverArea> Areas;
vector <ImgPt> ImgPts;
vector <EarthPt> EarthPts;
long int VectorPtsStart = 0; //多个Area中的点全部存放在ImgPts和EarthPts容器中
char OilMsg[0x7fffff];
OilArea1 Tuble1;
OilArea2 Tuble2;
ushort row;
ushort col;



char* OILMsg_Back(int &N)
{
	long int Index = 0;
	OilMsg[Index] = OILimg; Index += 1;
	OilMsg[Index] = Areas.size(); Index += 1;
	if (Areas.size() > 0) {
		for (int i = 0; i < Areas.size(); i++) {
			OilMsg[Index] = (uchar)i; Index += 1;//ID
			OilMsg[Index] = Areas[i].belief; Index += 1;//置信度
			memcpy(&OilMsg[Index], &(Areas[i].OrientOfCenter), 8 * sizeof(uchar)); Index += 8;//质心方位
			{
				Tuble1.DisOfCenter = Areas[i].DisOfCenter; 
				Tuble1.RowOfCenter = Areas[i].RowOfCenter;
				Tuble1.ColOfCenter = Areas[i].ColOfCenter;
				memcpy(&OilMsg[Index], &Tuble1, 6 * sizeof(uchar)); Index += 6;
			}
			{
				Tuble2.east_speed = Areas[i].east_speed;
				Tuble2.north_speed = Areas[i].north_speed;
				Tuble2.area = Areas[i].area;
				Tuble2.rateOfChange = Areas[i].rateOfChange;
				memcpy(&OilMsg[Index], &Tuble2, 32 * sizeof(uchar)); Index += 32;
			}
			unsigned int Num = Areas[i].num;
			memcpy(&OilMsg[Index], &Num, 4 * sizeof(uchar)); Index += 4;//点个数
			if (Num > 0) { //复制点数据
				memcpy(&OilMsg[Index], &ImgPts[VectorPtsStart], 4 * Num * sizeof(uchar)); Index += 4 * Num;
				memcpy(&OilMsg[Index], &EarthPts[VectorPtsStart], 16 * Num * sizeof(uchar)); Index += 16 * Num;
				VectorPtsStart += Num;//更新起点
			}
		}
	}
	row = ShowImg.rows;
	col = ShowImg.cols;
	if (Index + (long int)row*col < 0x7fffff) {///////判断内存是否足够
		for (int i = 0; i < row; i++) {
			Mat tmp_row = ShowImg.row(i).clone();
			memcpy(&OilMsg[Index], &tmp_row, sizeof(tmp_row)); Index += sizeof(tmp_row);
		}
	}
	else
	{
		row = 0; col = 0;
		memcpy(&OilMsg[Index], &row, 2 * sizeof(uchar)); Index += 2;//质心方位
		memcpy(&OilMsg[Index], &row, 2 * sizeof(uchar)); Index += 2;//质心方位
	}
	N = Index;
	return OilMsg;
}


