
#include "stdafx.h"
#include "conio.h"
#include "windows.h"
#include "command.h"
#include "function.h"


char TCPRecv[TCP_SIZE] = { 0 };
char UDPRecv[UDP_SIZE] = { 0 };
char ClientTime[8] = { 0 };

union int_to_char
{
	int int_num;
	char char_num[4];
};
typedef struct
{
	char  Data_ID;
	char *Info;
	void(*Func)(void);				

}UserData;
char playbacktime[8] = { 0 };
char range = 0;
char show[4] = { 0 };
char thresh = 0;
char gain = 0;
char contrast = 0;
char turns = 0;
char rate = 0;
char gear = 0;
char recordrange[2] = { 0 };
char recordsector[4] = { 0 };
char saveaddr[100] = { 0 };
char halfauto = 0;
char handle[1000] = { 0 };

UserData Client[]=
{
	{ 0x01,		 NULL,			 Selfcheck },
	{ 0x02,		 NULL,			 ShortP },
	{ 0x03,		 NULL,			 MidP },
	{ 0x04,		 NULL,			 Pause },
	{ 0x05,		 NULL,			 Record },
	{ 0x06,		 playbacktime,	 Playback },
	{ 0x07,		 NULL,			 PlaybackPause },
	{ 0x08,		 NULL,			 PlaybackStop },
	{ 0x11,		 &range,		 Range },
	{ 0x12,		 show,			 Show },
	{ 0x21,		 &thresh,		 Thresh },
	{ 0x22,		 &gain,			 Gain },
	{ 0x23,		 &contrast,		 Contrast },
	{ 0x24,		 &turns,		 Turns },
	{ 0x31,		 &rate,			 Rate },
	{ 0x32,		 &gear,			 Gear },
	{ 0x33,		 recordrange,  	 RecordRange },
	{ 0x34,		 recordsector,	 RecordSector },
	{ 0x35,		 saveaddr,		 SaveAddr },
	{ 0x41,		 NULL,			 Auto },
	{ 0x42,		 &halfauto,		 HalfAuto },
	{ 0x43,		 handle,		 Handle },

};

void GetData(char Recv[TCP_SIZE])
{
	UserData temp;
	int_to_char lenth;
	char id_num;
	if (Recv[0] != 0xff || Recv[1] != 0xff)
	{
		printf("Data wrong!");
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		lenth.char_num[i] = Recv[i + 2];
	}
	for (int i = 0; i < 8; i++)
	{
		ClientTime[i] = Recv[i + 6];
	}
	temp.Data_ID = Recv[14];
	for (int i = 0; i < sizeof(Client) / sizeof(Client[0]); i++)
	{
		if (Client[i].Data_ID == temp.Data_ID)
		{
			id_num = i;
			break;
		}
		else if (i == sizeof(Client) / sizeof(Client[0])-1)
		{
			printf("ID wrong!");
			return;
		}

	}
	memcpy(Client[id_num].Info, &Recv[15], lenth.int_num);
	Client[id_num].Func();
}