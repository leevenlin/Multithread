
#include "stdafx.h"
#include "conio.h"
#include "windows.h"
#include "command.h"
#include "function.h"


char TCPRecv[TCP_SIZE] = { 0 };
char UDPRecv[UDP_SIZE] = { 0 };
time_to_char ClientTime = { 0 };
Position_to_char ClientPosition;
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
	{ 0x01,		 &selfcheck,	   Selfcheck    	,false ,	Selfcheck_Back ,		2},
	{ 0x02,		 &control,		   Control		    ,false ,	Control_Back ,			2},
	{ 0x03,		 &record,		   Record			,false ,	Record_Back  ,			2},
	{ 0x04,		 playbacktime,	   Playback			,false ,	Playback_Back  ,		9},
	{ 0x05,		 &playbackcontrol, PlaybackControl	,false ,	PlaybackControl_Back  , 2},
	{ 0x11,		 &range,	   	   Range			,false ,	Range_Back  ,			2},
	{ 0x12,		 show,			   Show				,false ,	ShowSector_Back  ,		5},
	{ 0x21,		 &thresh,		   Thresh			,false ,	Thresh_Back  ,			2},
	{ 0x22,		 &gain,			   Gain				,false ,	Gain_Back  ,			2},
	{ 0x23,		 &contrast,		   Contrast			,false ,	Contrast_Back  ,		2},
	{ 0x24,		 &turns,		   Turns			,false ,	Turns_Back  ,			2},
	{ 0x31,		 &rate,			   Rate				,false ,	Rate_Back  ,			2},
	{ 0x32,		 &gear,			   Gear				,false ,	Gear_Back  ,			2},
	{ 0x33,		 recordrange,  	   RecordRange		,false ,	RecordRange_Back  ,		3},
	{ 0x34,		 recordsector,	   RecordSector		,false ,	RecordSector_Back  ,	5},
	{ 0x35,		 saveaddr,		   SaveAddr			,false ,	SaveAddr_Back  ,		129},
	{ 0x41,		 &automode,		   Auto				,false ,	Mode_Back  ,			2},
	{ 0x42,		 semiauto,		   HalfAuto			,false ,	Mode_Back  ,			0},
	{ 0x43,		 handle,		   Handle			,false ,	Mode_Back  ,			0},

};

void GetData(char Recv[])
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
	Client[id_num].CallBack_Flag=true;
}

void UDPData(char Recv[])
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
	memcpy(&ClientPosition, &Recv[Msg_Hder_Len + MsgLen_Len + Time_len], sizeof(Position_to_char));


}

extern SOCKET sockclient;
void CallBack_Thread(void)
{
	SYSTEMTIME sys;
	char SendStr[150],*ReturnStr;

	TIME T;
	int_to_char D_len;
	int_to_char hder;
	hder.int_num = MSG_Hder;
	memcpy(SendStr, &hder.char_num, 4 * sizeof(uchar));
	while (1)
	{
		//设置时间信息,每一轮发送数据只取一次时间
		GetLocalTime(&sys); 
		T.year = (uchar)sys.wYear;
		T.month = sys.wMonth;
		T.day = sys.wDay;
		T.hour = sys.wHour;
		T.minute = sys.wMinute;
		T.second = sys.wSecond;
		T.m_sec = (ushort)sys.wMilliseconds;
		memcpy(&SendStr[Msg_Hder_Len + MsgLen_Len], &T, 8 * sizeof(uchar));
		//遍历发送
		for (int i = 0; i < sizeof(Client) / sizeof(Client[0])-2; i++)
		{

			if (Client[i].CallBack_Flag)
			{
				Client[i].CallBack_Flag = false;
				ReturnStr = Client[i].CallBack_Func();
				//设置数据长度
				D_len.int_num = Msg_Hder_Len + MsgLen_Len + Time_len + Client[i].datalen; 
				memcpy(&SendStr[Msg_Hder_Len], D_len.char_num, 4 * sizeof(uchar));
				//copy数据段
				memcpy(&SendStr[Msg_Hder_Len + MsgLen_Len + Time_len], ReturnStr, Client[i].datalen * sizeof(uchar));
				//数据发送
				send(sockclient, SendStr, D_len.int_num*sizeof(uchar), 0);
			}
		}
		//发送图像及信息

		Sleep(20);
	}
}
