#pragma once
#ifndef _RECT_2_PLOAR_MAP_H_
#define _RECT_2_PLOAR_MAP_H_

#ifndef NULL
#define NULL 0
#endif

//#ifndef true
//#define true 1
//#endif
//
//#ifndef false
//#define false 0
//#endif

#ifndef PI
#define PI 3.141592653589793
#endif

#include <fstream>
#include <string>

struct Rect2PolarData
{
	unsigned short rowBegin;
	unsigned short colBegin; 
	float          ratio;
};

class Rect2PolarMap
{
public:
	static const int         DATA_STORE_RAM;//转换数据保存在内存中,适用于内存大的情况,速度较快
	static const int         DATA_STORE_HD;//转换数据保存在硬盘中,适用于内存较小的情况,读取速度较慢
	static const std::string RECT2POLAR_FILE_NAME;//转换数据文件存储名称

	~Rect2PolarMap();
	static const Rect2PolarMap& getInstance(int angleSum, int dataPerSamp);
	static const Rect2PolarMap& getInstance();
	void setMap(int angleSum, int dataPerSamp);
	inline const int getAngleSum() const;
	inline const int getDataPerSamp() const;
	Rect2PolarData** getMap();

private:
	Rect2PolarMap();

	void clearMap();

	bool rect2ploarFileExists() const;//判断直角坐标转极坐标文件是否存在

	static Rect2PolarMap instance;
	Rect2PolarData **map;
	int angleSum;
	int dataPerSamp;
	int storeFlg;//数据存储方式标记
};



#endif //_RECT_2_PLOAR_MAP_H_