#include "stdafx.h"
#include "conio.h"
#include "windows.h"

#include "function.h"
#include "SerialPort.h"

//sperrryÀ×´ï¿ØÖÆÖ¸Áî
unsigned char sperry_1[4] = { 0x00,0x10,0x65,0x75 };
unsigned char sperry_2[5] = { 0x00,0x20,0x62,0x76,0x78 };

extern CSerialPort port;
void Selfcheck(void)
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

void Range(void)
{

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