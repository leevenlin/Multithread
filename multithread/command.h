#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include <vector>

#define TCP_SIZE 0xffff
#define UDP_SIZE 0xffff
#define MSG_Hder   0xFF7FFF7F
#define Msg_Hder_Len 4
#define MsgLen_Len   4
#define Time_len  8
#define TIME_ERR 0xFF FF FF FF FF FF FF FF 
#define ERR 0xFF
typedef unsigned char  uchar;
typedef unsigned short  ushort;

extern char TCPRecv[TCP_SIZE];
extern char UDPRecv[UDP_SIZE];


typedef enum
{
	PAUSE=0x00,
	SHORTP=0x01,
	MIDP=0x02,
}CONTROL;

typedef enum
{
	START = 0,
	STOP = 1,
}RECORD;

typedef enum
{
	PAUSEPLAY = 0,
	KEEPON = 1,
	STOPPLAY = 0X10,
}PLAYBACK;

struct _2short
{
	unsigned short start;
	unsigned short end;
};

struct ImgPt  //图像坐标
{
	ushort row;
	ushort col;
};

union int_to_char
{
	unsigned int int_num;
	uchar char_num[4];
	_2short B_and_E;
	ImgPt pt;
};
union short_to_char
{
	uchar char_num[2];
	ushort data_short;
};

struct UserData
{
	uchar  Data_ID;
	uchar *Info;
	void(*Func)(void);
	bool CallBack_Flag;
	char* (*CallBack_Func)(void);
	uchar datalen;
};
struct TIME
{
	uchar year;
	uchar month;
	uchar day;
	uchar hour;
	uchar minute;
	uchar second;
	unsigned short m_sec;
};
struct Position  //UDP接收结构体
{
	double longitude;
	double latitude;
	double height;
	double east_speed;
	double north_speed;
	double sky_speed;
	double direction;
};

union Position_to_char
{
	uchar byte[64];
	Position posiiton;
};


struct OilArea1
{
	ushort DisOfCenter;//质心距离
	ushort RowOfCenter;
	ushort ColOfCenter;

};
struct OilArea2
{
	double east_speed;
	double north_speed;
	double area;
	double rateOfChange; //面积变化率
};
	


	//unsigned int NumberOfPts; //点数
//52个char
union time_to_char
{
	uchar byte[8];
	TIME time;
};

struct EarthPt  //经纬度坐标
{
	double longitude;
	double latitude;
};
struct OilCoverArea  //溢油区域结构体
{
	uchar belief;
	double OrientOfCenter;
	ushort DisOfCenter;//质心距离
	ushort RowOfCenter;
	ushort ColOfCenter;
	double east_speed;
	double north_speed;
	double area;
	double rateOfChange; 
	long int num;
};


void TCPClient();
void UDPData(char Recv[]);


#endif // !COMMAND_H


