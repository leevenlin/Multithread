#pragma once
#ifndef IMSHOW_H
#define IMSHOW_H


#define SelfCheck		0x01
#define RadarCon		0x02		
#define Record			0x03		 
#define ShowBack		0x04		
#define STopOrRunShowBack 0x05		 
	 
#define Span			0x11
#define ShowSection		0x12		 
#define ShowThresh		0x21		 
#define Gain			0x22 
#define Contrast		0x23		 
#define CntCircle		0x24		 
#define SampleRate		0x31		 
#define VoltageSelect	0x32		 
#define SampleSpan		0x33	 
#define SampleSetion	0x34		 
#define SaveDest	0x35	 
#define Auto		0x41	 
#define SemiAuto	0x42		 
#define NoAuto		0x43		 
void ImgShow();

#endif // !IMSHOW_H
