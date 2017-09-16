//extern "C" {
#include "time.h"
#include "stdio.h"
#include "dos.h"
#include "QTfmCommon.h"
#include "QTDevice.h"
#include "QTApi.h"
#include "QT_IMPORT.h"
//}

//class QT
//{
//public:
//
//};
struct paramater {
	unsigned int sampleRate;
	unsigned int inputRange;
	float sampleRange;
	unsigned int angleStart;
	unsigned int angleEnd;
	char fileName;
	char fileLocation;
	bool saveFile;
}pmt;
extern "C" {
	int QT_FifoMultiple(ST_CARDINFO *pstCardInfo, unsigned int unBoardIndex, paramater pmt);
	int QT_FifoMultipleSetup(ST_CARDINFO *pstCardInfo, unsigned int unBoardIndex, paramater pmt);
}

#pragma once
