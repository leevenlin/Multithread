#pragma once
#ifndef _RADAR_IMG_ENHANCE_H_
#define _RADAR_IMG_ENHANCE_H_

#include "radarImg.h"
#include <QThread>
#include "fileList.h"
#include "rect2polarmap.h"

#include <QDate>
#include <QTime>
#include <QMutex>
#include <QLinkedList>
#include <cmath>
#include <math.h>
class RadarImgEnhance : public QThread
{
	Q_OBJECT
public:
	RadarImgEnhance(/*FileList fileList*/);
	~RadarImgEnhance();
	void setThreshold(int threshold);
	void setGain(int gain);
	void setMaxEnhanceNum(int num);
	int  getMaxEnhanceNum() const;
	int  getRunState();
	//void resetEnhanceCont();
	int  getEnhanceCont() const;
	void unPause();
	void pause();

	void enhanceImg();
	cv::Mat& conv2gray();
	inline void sendShowImgSignal(QString imgName);

	bool readFiles();
	bool hasFiles();

	void saveImg(bool flg);
	void setSaveImgPath(const QString &path);

	

	QStringList getFileList() const;

	cv::Mat& getImg_polar_gray() const;

	//直角坐标转极坐标方法，数表从硬盘中分次读取加载
	static void convRect2Polar(const cv::Mat &rectImg, cv::Mat &dst);

	//直角坐标转极坐标方法，数表一次性读入，保存在内存中
	static void convRect2Polar_RAM(const cv::Mat &rectImg, cv::Mat &dst);

	static const int START_ENHANCE;
	static const int STOP_ENHANCE;
	static const int RUNNING;

	static const int ENHANCE_LINEAR;
	static const int ENHANCE_EXP;
	static const int ENHANCE_LOG;
	static const int ENHANCE_SIGMOID;

//protected:
	void run();



signals:
	void imgUpdated(QString imgName);

private:
	void enhance_linear(cv::Mat &mat);
	void enhance_exp(cv::Mat &mat);
	void enhance_log(cv::Mat &mat);
	void enhance_sigmoid(cv::Mat &mat);

	int      firstFileAglStart;//读取第一个文件的第一个回波的角度
	double   threshold;//阈值，0~100
	double   gain;//增益，1~100
	cv::Mat  img_rect;//直角坐标系下增强的图像
	cv::Mat  img_rect_rcd;//用于记录上一个图像最后一行，便于插值
	cv::Mat  img_rect_last;
	cv::Mat  img_rect_gray;
	int      enhanceCont;//计数器，用于计量叠加次数
	int      maxEnhanceNum;//叠加数目最大值
	int      runFlg;//设定标志位
	FileList *fileList;//存储打开的文件集合
	cv::Mat  img_polar_gray;
	bool     saveImgFlg;
	QString  saveImgPath;

	QMutex   mutexPause;
	IplImage *g_pCannyImg;
	const  CvArr* s;
	QLinkedList<cv::Mat> matList;
};

class SaveImgThread : public QThread
{
	Q_OBJECT
public:
	SaveImgThread();
	SaveImgThread& writeImg(const cv::Mat &img, const std::string &fileName);
protected:
	void run();
private:
	cv::Mat img;
	std::string fileName;
};

class Rec2PolarThread : public QThread
{
	Q_OBJECT
public:
	Rec2PolarThread();
	Rec2PolarThread& convert(const cv::Mat &rectImg, cv::Mat &dst, Rect2PolarData **map, int rowStart, int rowEnd);
	void run();
private:
	int mapWidth;
	cv::Mat rectImg;
	cv::Mat dst;
	Rect2PolarData **map;
	int rowStart;
	int rowEnd;
};

#endif

