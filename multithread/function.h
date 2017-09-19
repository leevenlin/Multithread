#pragma once

#ifndef FUNCTION_H
#define FUNCTION_H

#include "command.h"
void Selfcheck(void);

extern uchar control;
void Control(void);

void ShortP(void);

void MidP(void);

void Pause(void);

extern uchar record;
void Record(void);

void Playback(void);

extern uchar playbackcontrol;
void PlaybackControl(void);

void PlaybackStop(void);

extern uchar range;
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

#endif // !FUNCTION_H

