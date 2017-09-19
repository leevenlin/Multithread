#include "stdafx.h"
#include "conio.h"
#include "windows.h"

#include "function.h"
#include "SerialPort.h"
#include "command.h"	
//sperrryÀ×´ï¿ØÖÆÖ¸Áî
unsigned char sperry_1[4] = { 0x00,0x10,0x65,0x75 };
unsigned char sperry_2[5] = { 0x00,0x20,0x62,0x76,0x78 };

extern CSerialPort port;
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

void Playback(void)
{

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


void Range(void)
{
	float range_num[11] = { 0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6 };
	float range_NM = range_num[range-1];
}

void Show(void)
{

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

void RecordRange(void)
{

}

void RecordSector(void)
{

}

void SaveAddr(void)
{

}

void Auto(void)
{

}

void HalfAuto(void)
{

}

void Handle(void)
{


}