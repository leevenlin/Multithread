#ifndef COMMAND_H
#define COMMAND_H

#define TCP_SIZE 100
#define UDP_SIZE 100
#define MSG_Hder   0xFF7FFF7F
#define Msg_Hder_Len 4
#define MsgLen_Len   4
#define Time_len  8

extern char TCPRecv[TCP_SIZE];
extern char UDPRecv[UDP_SIZE];

typedef unsigned char  uchar;

union int_to_char
{
	unsigned int int_num;
	uchar char_num[4];
};
//enum  num_or_str
//{
//	shownum = 1,
//	showstr = 0
//};
struct UserData
{
	char  Data_ID;
	char *Info;
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


