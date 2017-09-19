#ifndef COMMAND_H
#define COMMAND_H

#define TCP_SIZE 100
#define UDP_SIZE 100
#define MSG_Hder   0xFF7FFF7F
#define Msg_Hder_Len 4
#define MsgLen_Len   4
#define Time_len  8
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

struct ImgPt
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
//enum  num_or_str
//{
//	shownum = 1,
//	showstr = 0
//};
struct UserData
{
	uchar  Data_ID;
	uchar *Info;
	void(*Func)(void);

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
union time_to_char
{
	uchar byte[8];
	TIME time;
};


void TCPClient();


#endif // !COMMAND_H


