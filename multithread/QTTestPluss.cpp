// QTTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "math.h"

//#ifdef __cplusplus
//	extern "C" {
//#endif

#include "ADSample.h"
#include <windows.h>
#include <queue>
#include <iostream>
using namespace std;
//#ifdef __cplusplus
//	}
//#endif


int Range2Byte(int x);

DWORD WINAPI Fun(LPVOID lpParamter)
{
	unsigned int i = 0;
	int nRet = -1;
	unsigned int NumOfBoard = 1;//Specify the number of cards need to operate on.
	ST_CARDINFO stCardInfo[_QTFIRMWARE_NUM_OF_CARD];//card info for per each card on PC mainboard
													//Control multiple cards in sequence
	for (i = 0; i<NumOfBoard; i++)
	{
		QT_FifoMultiple(&stCardInfo[i], i, pmt);
	}
	 return 0L;
 }

int main(void)
{
	int n,m;
	queue<int> q1;

	for (int i = 0; i<10; i++)

		q1.push(i);

	if (!q1.empty())

		cout << "dui lie  bu kong\n";

	n = q1.size();

	cout << n << endl;

	m = q1.back();

	cout << m << endl;


	struct paramater pmt;
	pmt.sampleRate = 1000;
	pmt.inputRange = 2;
	pmt.sampleRange = 2.5;
	pmt.angleStart = 1;
	pmt.angleEnd = 1;
	pmt.fileName = 1;
	pmt.fileLocation = 1;
	pmt.saveFile = true;

	HANDLE hThread = CreateThread(NULL, 0, Fun, NULL, 0, NULL);
	CloseHandle(hThread);
	//unsigned int i = 0;
	//int nRet = -1;
	//unsigned int NumOfBoard = 1;//Specify the number of cards need to operate on.
	//ST_CARDINFO stCardInfo[_QTFIRMWARE_NUM_OF_CARD];//card info for per each card on PC mainboard
	//												//Control multiple cards in sequence
	//for (i = 0; i<NumOfBoard; i++)
	//{
	//	QT_FifoMultiple(&stCardInfo[i], i, pmt);
	//}
	//return nRet;
	char *source = NULL;
	char *dent = NULL;
	source = (char*)malloc(100 * sizeof(char));
	dent = (char*)malloc(100 * sizeof(char));
	char s[] = "a,b,c";
	memcpy((void*)&dent[0], &s, 6 * sizeof(char));
	memcpy((void*)&source[3], &dent[2], 6 * sizeof(char));
	printf("%s", source);
	//*source[0] = "a";
}

int QT_FifoMultiple(ST_CARDINFO *pstCardInfo, unsigned int unBoardIndex, paramater pmt)
{
	//////////////////////////////////////////////////////////////////////////
	//----Acquisition parameters
	QTFM_ABA_MODE ABAMode = ABA_NORMAL_MODE;
	unsigned int ABADivider = 48;//In step of 16
	unsigned int NoChnInUse = 1;//Number of active channel in use
	//unsigned int SegmentLen = (8 << 10) - 16;//number of samples per segment
	//----------added by lorry
	unsigned int SegmentLenTmp = Range2Byte(pmt.sampleRange);
	unsigned int SegmentLen = SegmentLenTmp << 10 - 16;//number of samples per segment
	unsigned int DmaGranularityB = 4 << 20;//PCIe DMA granularity to get data. It shouldn't be greater than EACH_DMA_MAX_LEN
	unsigned int AccCnt = 0;//>=3;
	unsigned int MeanEn = 1;
	unsigned int MeanMode = 0;//0:16bit mode 1:32bit mode
	unsigned int PreTriggerLen = 0;//pre-trigger length in samples
	unsigned int SegmentCnt = 0;//should be 0 for non-limited trigger events
	unsigned int uncompressMode = 0;//bit[0]=1:使能数据压缩模块,0:禁止数据压缩功能
									//bit[1]=1:使能数据解压缩功能，0：禁止数据解压缩功能
	QTFM_COMMON_TRIGGER_MODE TrigMode = QTFM_COMMON_TRIGGER_MODE_EDGE;//QTFM_COMMON_TRIGGER_MODE_LEVEL
	unsigned int TimeStampEn = 0x3;//Enable/Disable Timestamp mode. The following list valid values:
								   //       0x0: Disable timestamp mode. Timestamp counter keep no change and values is param2 and param3 are ignored.
								   //       0x3: Enable GPS mode, internal clock is used to counting.Accumulative error could impact on precision of timestamp along the time.
								   //       0x13: Enable GPS mode, external 1pps clock is required to counting. This is a more accurate way vs. 0x3 mode.
	unsigned int TimeStampSegmentLen = 0;
	unsigned int TimeStampSel = 0x1;//时间戳数据选择。0：选择GPS时间戳数据；1：选择用户时间戳数据
	unsigned int GpsScond = 0;
	bool         SaveFile = pmt.saveFile;
	//------------added by lorry, Range 2
	QTFM_INPUT_RANGE   CurrInputRange = QTFM_INPUT_RANGE_2;//+full-scale in mV
	double              OffSet = 400000;//[-fullscale,+fullscale] in uV
	if (pmt.inputRange == 1)
	{
		QTFM_INPUT_RANGE   CurrInputRange = QTFM_INPUT_RANGE_1;//+full-scale in mV
	    double             OffSet         = 100000;//[-fullscale,+fullscale] in uV
	}
	else if (pmt.inputRange == 2)
	{
		QTFM_INPUT_RANGE   CurrInputRange = QTFM_INPUT_RANGE_2;//+full-scale in mV
		double              OffSet = 400000;//[-fullscale,+fullscale] in uV
	}
	else
	{
		QTFM_INPUT_RANGE   CurrInputRange = QTFM_INPUT_RANGE_3;//+full-scale in mV
		double              OffSet         = 500000;//[-fullscale,+fullscale] in uV
	}									
	//----------added by lorry
	if (pmt.angleStart == 360)
	{
		pmt.angleStart = 0;
	}
	unsigned int StartAngle = pmt.angleStart * 4096 / 360;
	unsigned int EndAngle = pmt.angleEnd * 4096 / 360;
	unsigned long long TotalData = (unsigned long long) 1 << 30; //1GB

																 //////////////////////////////////////////////////////////////////////////
																 //----Clock parameters
	unsigned int Fref = 100000000;//Reference clock frequency. 
	QTFM_COMMON_CLOCK_REFERENCE RefClkMode = QTFM_COMMON_CLOCK_REF_MODE_2;//Change to QTFM_COMMON_CLOCK_REF_MODE_1 if external reference clock is required.
	QTFM_COMMON_ADC_CLOCK ADCClkMode = QTFM_COMMON_ADC_CLOCK_MODE_0;//Change to QTFM_COMMON_ADC_CLOCK_MODE_1 if external sampling clock is required.

																	//////////////////////////////////////////////////////////////////////////

																	//Internal variables
	int nRet = -1;//Default return value
	char szFilePathName[MAX_PATH]/* = {0}*/;
	unsigned int ClockDivider = 0;
	unsigned int tmp;
	unsigned char *pszBuf = NULL;
	//added by lorry 
	unsigned char *totalDataBuf = NULL;
	unsigned int PerBufSize = 0;//should be aligned with pstCardInfo->dib.EachTransferLen_Tx
	long long RecvDataLen = 0;
	long long AvailableData = 0;//No. of available sample data bytes in buffer
	int BufOverflow = -1;//buffer overflow flag
	unsigned int NoBytesOverwritten = 0;//No. of overwritten sample data bytes
	FILE *fp = NULL;
	errno_t err;
	//////////////////////////////////////////////////////////////////////////
	//----Open card
	CHECK_RETURN_VALUE(QTOpenBoard(pstCardInfo, unBoardIndex));
	CHECK_RETURN_VALUE(QTResetBoard(pstCardInfo));
	//////////////////////////////////////////////////////////////////////////
	//Assign value to card structure member
	pstCardInfo->DAQParams.AccCnt = AccCnt;
	if (AccCnt && (!MeanEn || MeanMode))
		pstCardInfo->dib.MaxEachDmaLenB = SegmentLen * 4 - (SegmentLen * 4) % 128;//align with 128B
	else
		pstCardInfo->dib.MaxEachDmaLenB = SegmentLen * 2 - (SegmentLen * 2) % 128;//align with 128B
	pstCardInfo->dib.MaxEachDmaLenB = (DmaGranularityB>EACH_DMA_MAX_LEN) ? EACH_DMA_MAX_LEN : DmaGranularityB;
	pstCardInfo->dib.MaxEachDmaLenB = pstCardInfo->dib.MaxEachDmaLenB - pstCardInfo->dib.MaxEachDmaLenB % 128;//Align with 128 bytes
																											  //Take max sample rate as default. Users feel free to change it by uncomment this line then assign new value.
																											  //pstCardInfo->ClockInfo.SRate = 250000000;//Sample rate in Hz
																											  //////////////////////////////////////////////////////////////////////////
																											  //----Setup clock
	CHECK_RETURN_VALUE(QTClockSet(pstCardInfo, Fref, 1, (pstCardInfo->ClockInfo.SRate), QTFM_COMMON_CLOCK_VCO_MODE_0, RefClkMode, ADCClkMode, 1));
	//----Setup AFE
	if (pstCardInfo->ProdInfo.product_number != 0x1125)
	{
		CHECK_RETURN_VALUE(QTAdcModeSet(pstCardInfo, 0, (pstCardInfo->stAI.bForceIOdelay) ? 0 : (1 << 8), 0));
	}
	//Analog input enable. Run the command multiple times for other channels
	CHECK_RETURN_VALUE(QTInputChannelSet(pstCardInfo, CHANNEL_0, 0, 0, 0, 0, 1));//Channel 0
																				 //Select data format between offset binary code and two's complement
	CHECK_RETURN_VALUE(QTDataFormatSet(pstCardInfo, 1));//offset binary or two's complement
														//----Setup GPIO
	CHECK_RETURN_VALUE(QTGPIOSet(pstCardInfo, 0, 0, 0));//input and disable digital data acquisition by default
	CHECK_RETURN_VALUE(QTGPIODelaySet(pstCardInfo, -1, 4));
	CHECK_RETURN_VALUE(QTGPIORead(pstCardInfo, 0, &tmp));
	CHECK_RETURN_VALUE(QTDigInLatency(pstCardInfo, 35));//Align digital input with analog input.
														//----Setup Input range and offset
	if (pstCardInfo->ProdInfo.couple_type == 0xDC)
	{
		//----Set analog input range first then offset
		CHECK_RETURN_VALUE(QTChannelRangeSet(pstCardInfo, -1, CurrInputRange));
		//----Set analog offset
		CHECK_RETURN_VALUE(QTChannelOffsetSet(pstCardInfo, -1, OffSet));
	}
	//----Setup work mode, acquisition parameters
	CHECK_RETURN_VALUE(QTABAModeSet(pstCardInfo, ABAMode, ABADivider, NoChnInUse));
	CHECK_RETURN_VALUE(QTWorkModeSet(pstCardInfo, SegmentLen, PreTriggerLen, SegmentCnt, QTFM_COMMON_BOARD_WORK_MODE_FIFO_MULTIPLE_LOOPBUF | TrigMode, uncompressMode, 0));
	CHECK_RETURN_VALUE(QTTimestampSet(pstCardInfo, TimeStampEn, TimeStampSegmentLen, GpsScond, (pstCardInfo->ClockInfo.SRate), TimeStampSel));
	//----Setup trigger source and types
	//Internal periodic pulse for multiple trigger events
	//CHECK_RETURN_VALUE (QTPulseTriggerSet (pstCardInfo,QTFM_COMMON_TRIGGER_TYPE_RISING_EDGE,0x20,2*SegmentLen,1));
	CHECK_RETURN_VALUE(QTExtTriggerSet(pstCardInfo, QTFM_COMMON_TRIGGER_SOURCE_EXTERNAL_5, QTFM_COMMON_TRIGGER_TYPE_RISING_EDGE, 1));
	//CHECK_RETURN_VALUE (QTTriggerSourceMaskSet (pstCardInfo,QTFM_COMMON_TRIGGER_SOURCE_MASK_OR,37,0));//Align external trigger input with analog input

	//----Define the total amount of data and allocate memory as an intermediate buffer
	PerBufSize = 64 << 20;//100MB,should be aligned with pstCardInfo->dib.EachTransferLen_Tx
	pszBuf = (unsigned char*)malloc(sizeof(char)*PerBufSize);
	//----------added by lorry
	totalDataBuf = (unsigned char*)malloc(sizeof(char)*TotalData);
	if (!pszBuf)
	{
		printf_s("Buffer allocate error\n");
		return nRet;
	}
	//////////////////////////////////////////////////////////////////////////
	//Open file to save data

	CHECK_RETURN_VALUE(QTWriteRegister(pstCardInfo, 0x78200000, 0x10, 76800));//baud rate
	CHECK_RETURN_VALUE(QTWriteRegister(pstCardInfo, 0x78200000, 0x04, (StartAngle << 4) + (EndAngle << 20)));//angle
	CHECK_RETURN_VALUE(QTWriteRegister(pstCardInfo, 0x78200000, 0x08, (StartAngle << 0) + (EndAngle << 16)));
	CHECK_RETURN_VALUE(QTWriteRegister(pstCardInfo, 0x78200000, 0x0C, SegmentLen));

	//----Get PCIe DMA ready waiting for trigger events
	QTStart(pstCardInfo, QTFM_COMMON_TRANSMIT_DIRECTION_BRD2PC, 1, 0xffffffff, 0);
	CHECK_RETURN_VALUE(QTWriteRegister(pstCardInfo, 0x78200000, 0, 0xF03));
	//Dump register value
	//CHECK_RETURN_VALUE(QTDumpRegister(pstCardInfo,0x7ae00000,32));
	//----Receive sample data to buffer

	char filename[MAX_PATH];
	time_t timer;
	struct tm timeinfo;
	while (1)
	{
		timer = time(0);
		localtime_s(&timeinfo, &timer);
		sprintf_s(filename, MAX_PATH, "%d-%d-%d-%d-%d-%d-%d-%d-%d.dat", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, SegmentLen, StartAngle * 360 / 4096, EndAngle * 360 / 4096);

		err = fopen_s(&fp, filename, "wb");
		if (0 == err)
			printf("open file successfully\n");
		else
		{
			printf(" file open failed\n");
			return -1;
		}

		RecvDataLen = 0;
		while (RecvDataLen<TotalData)
		{
			QTBufStatusGet(pstCardInfo, &AvailableData, &BufOverflow, &NoBytesOverwritten);
			if (BufOverflow || NoBytesOverwritten) break;
			RecvDataLen += NoBytesOverwritten;
			if (AvailableData<PerBufSize) continue;
			QTBufSDataGet(pstCardInfo, &pszBuf[0], (unsigned int)PerBufSize);

			//-----------added by lorry
			memcpy((void*)&totalDataBuf[RecvDataLen], &pszBuf[0], PerBufSize * sizeof(unsigned char));
			//memcpy((void*)&data_stream_target[n_samples_collect - samples_to_collect],ADQ_GetPtrStream(adq_cu, adq_num),samples_in_buffer * sizeof(signed short));
			if (SaveFile)
				fwrite(pszBuf, sizeof(UCHAR), PerBufSize, fp);
			RecvDataLen += PerBufSize;
			printf_s("Received %d(MB),vfifo %d(MB)\n", RecvDataLen >> 20, AvailableData >> 20);
		}

		printf("Acquisition done...\n");
		if (BufOverflow)
		{
			printf("Samples data were lost! Either speed up buffer read rate or reduce sample rate\n");
		}
		if (NoBytesOverwritten)
		{
			printf("Samples data could be corrupted by overwritten!Either speed up buffer read rate or reduce sample rate\n");
		}

		//----close the file
		//if (SaveFile)
		//  fwrite(pszBuf,sizeof(UCHAR),PerBufSize,fp);
		fclose(fp);
		printf("close file successfully\n");
	}
	free(pszBuf);
	pszBuf = NULL;
	//----Stop acquisition and close card handle
	QTStart(pstCardInfo, QTFM_COMMON_TRANSMIT_DIRECTION_BRD2PC, 0, 0xffffffff, 0);
	CHECK_RETURN_VALUE(QTCloseBoard(pstCardInfo));
	nRet = RES_SUCCESS;
	return nRet;
}

int Range2Byte(int x)
{
	int y;
	double y1, y2, y3, y4;
	y4 = x * 1850 / 0.6;
	y1 = log10(y4);
	y2 = log10(2);
	y3 = y1 / y2;
	y = y3 + 1;
	return y;
}
