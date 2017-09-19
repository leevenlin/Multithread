
#include "stdafx.h"
#include "conio.h"
#include "windows.h"
#include "command.h"
#include "function.h"


char TCPRecv[TCP_SIZE] = { 0 };
char UDPRecv[UDP_SIZE] = { 0 };
time_to_char ClientTime = { 0 };
enum  num_or_str tmp;

uchar selfcheck = 0;
uchar control=0;
uchar shortp = 0;
uchar record=0;
uchar playbacktime[8] = { 0 };
uchar playbackcontrol = 0;
uchar range = 0;
uchar show[4] = { 0 };
uchar thresh = 0;
uchar gain = 0;
uchar contrast = 0;
uchar turns = 0;
uchar rate = 0;
uchar gear = 0;
uchar recordrange[2] = { 0 };
uchar recordsector[4] = { 0 };
uchar saveaddr[128] = { 0 };
uchar automode = 0;
uchar semiauto[257] = { 0 };
uchar handle[0xffff] = { 0 };

UserData Client[]=
{//                                        
	{ 0x01,		 &selfcheck,	   Selfcheck    	},
	{ 0x02,		 &control,		   Control		    },
	{ 0x03,		 &record,		   Record			},
	{ 0x04,		 playbacktime,	   Playback			},
	{ 0x05,		 &playbackcontrol, PlaybackControl	},
	{ 0x11,		 &range,	   	   Range			},
	{ 0x12,		 show,			   Show				},
	{ 0x21,		 &thresh,		   Thresh			},
	{ 0x22,		 &gain,			   Gain				},
	{ 0x23,		 &contrast,		   Contrast			},
	{ 0x24,		 &turns,		   Turns			},
	{ 0x31,		 &rate,			   Rate				},
	{ 0x32,		 &gear,			   Gear				},
	{ 0x33,		 recordrange,  	   RecordRange		},
	{ 0x34,		 recordsector,	   RecordSector		},
	{ 0x35,		 saveaddr,		   SaveAddr			},
	{ 0x41,		 &automode,		   Auto				},
	{ 0x42,		 semiauto,		   HalfAuto			},
	{ 0x43,		 handle,		   Handle			},

};

void GetData(char Recv[TCP_SIZE])
{
	UserData temp;
	int_to_char lenth;
	int_to_char Msg_Hder;
	uchar id_num;
	memcpy(&Msg_Hder, &Recv[0], 4 * sizeof(uchar));  /////提取报文头
	if (Msg_Hder.int_num != MSG_Hder)
	{
		printf("Data wrong!");
		return;
	}
	memcpy(&lenth, &Recv[Msg_Hder_Len], sizeof(int_to_char)); //提取数据长度
	memcpy(&ClientTime, &Recv[Msg_Hder_Len + MsgLen_Len], sizeof(time_to_char));//提取时间
	temp.Data_ID = Recv[Msg_Hder_Len+ MsgLen_Len+Time_len];//提取ID号
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
	unsigned int len = Msg_Hder_Len + MsgLen_Len + Time_len;
	memcpy(Client[id_num].Info, &Recv[len + 1], (lenth.int_num-len-1)*sizeof(uchar)); //提取数据段，长度为报文总长-报文头（包括时间和数据长度）- ID号长度
	Client[id_num].Func();
}