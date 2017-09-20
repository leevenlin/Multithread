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

struct ImgPt  //ͼ������
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
struct Position  //UDP���սṹ��
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
	ushort DisOfCenter;//���ľ���
	ushort RowOfCenter;
	ushort ColOfCenter;

};
struct OilArea2
{
	double east_speed;
	double north_speed;
	double area;
	double rateOfChange; //����仯��
};
	


	//unsigned int NumberOfPts; //����
//52��char
union time_to_char
{
	uchar byte[8];
	TIME time;
};

struct EarthPt  //��γ������
{
	double longitude;
	double latitude;
};
struct OilCoverArea  //��������ṹ��
{
	uchar belief;
	double OrientOfCenter;
	ushort DisOfCenter;//���ľ���
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


