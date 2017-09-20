#pragma once

#ifndef FUNCTION_H
#define FUNCTION_H

#include "command.h"

void Selfcheck(void);

void Control(void);

void ShortP(void);

void MidP(void);

void Pause(void);

void Record(void);

void Playback(void);

void PlaybackControl(void);

void PlaybackStop(void);

void Range(void);

void Show(void);

void Thresh(void);

void Gain(void);

void Contrast(void);

void Turns(void);

void Rate(void);

void Gear(void);

void RecordRange(void);

void RecordSector(void);

void SaveAddr(void);

void Auto(void);

void HalfAuto(void);

void Handle(void);

/////////·¢ËÍº¯Êý

char* Selfcheck_Back(void);

char* Control_Back(void);
//
//void ShortP_Back(void);
//
//void MidP_Back(void);
//
//void Pause_Back(void);

char* Record_Back(void);

char* Playback_Back(void);

char* PlaybackControl_Back(void);

//void PlaybackStop_Back(void);

char* Range_Back(void);

char* ShowSector_Back(void);

char* Thresh_Back(void);

char* Gain_Back(void);

char* Contrast_Back(void);

char* Turns_Back(void);

char* Rate_Back(void);

char* Gear_Back(void);

char* RecordRange_Back(void);

char* RecordSector_Back(void);

char* SaveAddr_Back(void);

char* Mode_Back(void);

char* OILMsg_Back(int &N);
#endif // !FUNCTION_H

