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
	static const int         DATA_STORE_RAM;//ת�����ݱ������ڴ���,�������ڴ������,�ٶȽϿ�
	static const int         DATA_STORE_HD;//ת�����ݱ�����Ӳ����,�������ڴ��С�����,��ȡ�ٶȽ���
	static const std::string RECT2POLAR_FILE_NAME;//ת�������ļ��洢����

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

	bool rect2ploarFileExists() const;//�ж�ֱ������ת�������ļ��Ƿ����

	static Rect2PolarMap instance;
	Rect2PolarData **map;
	int angleSum;
	int dataPerSamp;
	int storeFlg;//���ݴ洢��ʽ���
};



#endif //_RECT_2_PLOAR_MAP_H_