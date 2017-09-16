#pragma once
#ifndef _RADAR_IMG_H_
#define _RADAR_IMG_H_

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "rect2polarmap.h"
#include "fileList.h"


#ifndef NULL
#define NULL 0
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef PI
#define PI 3.141592653589793
#endif

//#define Rec2PolarDataFormat 1

//#if Rec2PolarDataFormat == 0
//struct Rect2PolarData
//{
//	unsigned short rowBegin;
//	unsigned short colBegin; 
//	float          ratio;
//	unsigned short end;
//};
//
//#else

//#endif

class RadarImg
{
public:
	RadarImg();
	RadarImg(const std::string fileName);
	~RadarImg();
	static const int ANGLE_SUM = 4096;

	bool readFile(const std::string fileName);
	int readFile(std::ifstream &ifs, cv::Mat lastImg, int lastAgl, FileList &list);
	//cv::Mat& getRadarImg() const;
	cv::Mat& getRadarImgRcd() const;

	void convImg();
	static void convRect2Polar(const cv::Mat &rectImg, cv::Mat &dst);
	static void convRect2Polar_RAM(const cv::Mat &rectImg, cv::Mat &dst);

	static const int READ_FILE_SUCCESS;//读取一圈雷达数据成功
	static const int READ_FILE_EOF;//读取雷达数据过程中文件结束，更新ifstream
	static const int READ_FILE_NOMOREFILE;//更新ifstream指针失败
	
private:
	//cv::Mat radarImg;
	cv::Mat radarImgRcd;

	void meanArr(short* a, short* b, int arrLen);
	void interpArr(short* a, short *b, int arrLen, int range, int interpNum);
};


#endif