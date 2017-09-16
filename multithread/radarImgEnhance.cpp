#include "radarImgEnhance.h"
#include <QDebug>
#include <math.h>

//******************************************************************************************
//*                                图像增强方法
//******************************************************************************************
RadarImgEnhance::RadarImgEnhance(/*FileList fileList*/)
{
	this->setMaxEnhanceNum(50);
	this->enhanceCont   = 0;

	this->threshold = 0;
	this->gain      = 1;

	this->fileList = new FileList("*.dat");
	this->saveImgPath = "";
	this->saveImgFlg  = false;
}

RadarImgEnhance::~RadarImgEnhance()
{
	if(fileList != NULL)
	{
		delete fileList;
		fileList = NULL;
	}
}

void RadarImgEnhance::setThreshold(int threshold)
{
	this->threshold = threshold*2.5;
}

void RadarImgEnhance::setGain(int gain)
{
	this->gain = (double)gain/33.333;
}

void RadarImgEnhance::setMaxEnhanceNum(int num)
{
	this->maxEnhanceNum = num;
	this->enhanceCont   = 0;

	while (!matList.isEmpty())
	{
		matList.removeLast();
	}
}

int RadarImgEnhance::getMaxEnhanceNum() const
{
	return this->maxEnhanceNum;
}

int RadarImgEnhance::getRunState()
{
	return this->runFlg;
}

int RadarImgEnhance::getEnhanceCont() const
{
	return this->enhanceCont;
}

void RadarImgEnhance::unPause()
{
	this->runFlg = START_ENHANCE;

	mutexPause.unlock();
}

void RadarImgEnhance::pause()
{
	this->runFlg = STOP_ENHANCE;

	mutexPause.lock();
}

void RadarImgEnhance::run()
{
	unsigned char *head = new unsigned char[32];
	std::string firstFileName = fileList->getNextFileName();//读入第一个文件的文件名
	std::ifstream ifs(firstFileName, std::ios::binary | std::ios::in);
	ifs.read((char*)head, 32);
	unsigned int dataLen = ((unsigned int)head[26]<<8) + (unsigned int)head[24];

	short *data = new short[dataLen*RadarImg::ANGLE_SUM];

	//读取第一个回波数据
	short agleIdx1st = ((unsigned short)head[10]<<4)+((unsigned short)head[8]>>4);
	ifs.read((char*)data, dataLen*sizeof(data[0]));
	ifs.seekg(0, std::ios_base::beg);//重置文件流指针到文件起始位置

	this->firstFileAglStart = (int)agleIdx1st;
	img_rect_rcd = cv::Mat(1, dataLen, CV_16SC1, data);

	enhanceCont = 0;

	QString sdateTime = QString(firstFileName.data()).replace('/','\\');
	sdateTime = sdateTime.right(sdateTime.length() - sdateTime.lastIndexOf('\\')-1);

	int idx = 0;
	for (int i=0; i<6; ++i)
	{
		idx = sdateTime.indexOf("-", idx+1);
	}
	int timeStep_ms = 1250;//采样速度为48转/min，两帧时间间隔为1250ms
	QDateTime dateTime = QDateTime::fromString(sdateTime.left(idx), "yyyy-M-d-h-m-s").addMSecs(-timeStep_ms);

	SaveImgThread saveImgThread[10];
	while(true)
	{
		mutexPause.lock();

		dateTime = dateTime.addMSecs(timeStep_ms);

		RadarImg rImg = RadarImg();

		int readFlg = rImg.readFile(ifs, img_rect_rcd, firstFileAglStart, *fileList);
		if (readFlg && RadarImg::READ_FILE_NOMOREFILE)
			break;

		++enhanceCont;

		img_rect_last = rImg.getRadarImgRcd();
		img_rect_rcd  = img_rect_last.row(img_rect_last.rows-1).clone();
		enhanceImg();
		conv2gray();
		
		RadarImgEnhance::convRect2Polar_RAM(img_rect_gray, img_polar_gray);
		//conv2gray();

		QString name = dateTime.toString("yyyy-MM-dd-hh-mm-ss-zzz")+QString(".png");
		sendShowImgSignal(name);

		if (saveImgFlg)
		{
			//寻找空闲线程
			while (true)
			{
				bool flg = false;//标记是否存在空闲线程， 若找到，则为true，跳出循环
				for(int cont=0; cont<sizeof(saveImgThread)/sizeof(SaveImgThread); ++cont)
				{
					if (!saveImgThread[cont].isRunning())
					{
						saveImgThread[cont].writeImg(img_polar_gray, (saveImgPath+name).toStdString()).start();
						flg = true;
						break;
					}
				}
				if (flg)
					break;
			}
		}

		mutexPause.unlock();
	}

	//等待所有写图像线程结束
	for(int cont=0; cont<sizeof(saveImgThread)/sizeof(SaveImgThread); ++cont)
		saveImgThread[cont].wait();

	delete[] data;
	delete[] head;
}





void RadarImgEnhance::enhanceImg()
{
	//std::string name = QString::number(enhanceCont).toStdString();
	matList.append(img_rect_last);

	if (enhanceCont < 2)
	{
		img_rect = img_rect_last;
	}
	else if(enhanceCont <= maxEnhanceNum)
	{
		img_rect = img_rect*((double)(enhanceCont-1)/enhanceCont) + img_rect_last*(1./enhanceCont);
	}
	else
	{
		enhanceCont = maxEnhanceNum;
		cv::Mat firstImg = matList.takeFirst()*(1./enhanceCont);
		img_rect = img_rect - firstImg + img_rect_last*(1./enhanceCont);
		//img_rect = img_rect*((double)(enhanceCont-1)/enhanceCont) + img_rect_last*(1./enhanceCont);
	}

	cv::medianBlur(img_rect, img_rect, 5);
}

cv::Mat & RadarImgEnhance::conv2gray()
{
	cv::Mat tmp = (img_rect + ((1 << 15) - 256 * threshold)) * (gain / (1 << 8));
	//const  CvArr* s = (CvArr*)&tmp;
	//IplImage *g_pCannyImg = cvCreateImage(cvGetSize(s), IPL_DEPTH_8U, 1);
	//cvCanny(s, g_pCannyImg, 30, 80, 3);
	//cv::Mat tmp1 = cv::cvarrToMat(g_pCannyImg);
	tmp.convertTo(img_rect_gray, CV_8UC1);
	return img_rect_gray;
}

inline void RadarImgEnhance::sendShowImgSignal(QString imgName)
{
	emit imgUpdated(imgName);
}

bool RadarImgEnhance::readFiles()
{
	return fileList->addFiles();
}

bool RadarImgEnhance::hasFiles()
{
	return !fileList->empty();
}

void RadarImgEnhance::saveImg(bool flg)
{
	this->saveImgFlg = flg;
}

void RadarImgEnhance::setSaveImgPath(const QString &path)
{
	this->saveImgPath = path;
}

QStringList RadarImgEnhance::getFileList() const
{
	return fileList->getFileList();
}

//cv::Mat& RadarImgEnhance::getImg_polar() const
//{
//	return (cv::Mat&)img_polar;
//}

cv::Mat& RadarImgEnhance::getImg_polar_gray() const
{
	return (cv::Mat&)img_polar_gray;
}

//直角坐标转极坐标方法
void RadarImgEnhance::convRect2Polar(const cv::Mat &rectImg, cv::Mat &dst)
{
	Rect2PolarData head;
	std::ifstream read(Rect2PolarMap::RECT2POLAR_FILE_NAME, std::ios::binary | std::ios::in);

	int angle_data[2];
	read.read((char*)angle_data, sizeof(angle_data)/sizeof(angle_data[0]));

	int mapWidth = rectImg.cols*2-1;
	Rect2PolarData *data = new Rect2PolarData[mapWidth];
	int rows = mapWidth;

	dst = cv::Mat(rows, mapWidth, CV_16UC1, cv::Scalar(0));
	for (int row=0; row<dst.rows; ++row)
	{
		read.read((char*)&head, sizeof(Rect2PolarData));
		int dataLen = mapWidth - 2*(head.colBegin-1);

		read.read((char*)data, sizeof(Rect2PolarData)*dataLen);
		unsigned short *pRow = dst.ptr<unsigned short>(row);

		for (int col=0; col<dataLen; ++col)
		{
			pRow[head.colBegin+col-1] = rectImg.at<short>(data[col].rowBegin, data[col].colBegin)*data[col].ratio
				+ rectImg.at<short>(data[col].rowBegin+1, data[col].colBegin)*(1-data[col].ratio) + (1<<15);
		}
	}

	delete[] data;  data = NULL;
}

void RadarImgEnhance::convRect2Polar_RAM(const cv::Mat &rectImg, cv::Mat &dst)
{
	const int mapWidth = rectImg.cols*2-1;
	Rect2PolarMap r2pMap = Rect2PolarMap::getInstance(RadarImg::ANGLE_SUM, rectImg.cols);
	Rect2PolarData** map = r2pMap.getMap();

	const int rows = mapWidth;
	dst = cv::Mat(rows, mapWidth, rectImg.type(), cv::Scalar(0));

	Rec2PolarThread r2pThread[4];
	int thrDivLen[5] = {0, mapWidth*0.3, mapWidth*0.5, mapWidth*0.7, mapWidth-1};
	for (int thrIdx=0; thrIdx<sizeof(r2pThread)/sizeof(Rec2PolarThread); ++thrIdx)
	{
		r2pThread[thrIdx].convert(rectImg, dst, map, thrDivLen[thrIdx], thrDivLen[thrIdx+1]).start();
	}
	for(int cont=0; cont<sizeof(r2pThread)/sizeof(Rec2PolarThread); ++cont)
		r2pThread[cont].wait();
}

const int RadarImgEnhance::START_ENHANCE = 1;
const int RadarImgEnhance::STOP_ENHANCE = 0;
const int RadarImgEnhance::RUNNING = 1<<1;

const int ENHANCE_LINEAR = 1;
const int ENHANCE_EXP = 2;
const int ENHANCE_LOG = 3;
const int ENHANCE_SIGMOID = 4;


void enhance_linear(cv::Mat &mat)
{

}
void enhance_exp(cv::Mat &mat)
{

}
void enhance_log(cv::Mat &mat)
{

}
void enhance_sigmoid(cv::Mat &mat)
{

}


/////////////////////////////////SaveImgThread//////////////////////////////////
SaveImgThread::SaveImgThread()
{

}

SaveImgThread& SaveImgThread::writeImg(const cv::Mat &img, const std::string &fileName)
{
	this->img = img.clone();
	this->fileName = std::string(fileName);
	return (SaveImgThread&)*this;
}

void SaveImgThread::run()
{
	std::vector<int> param;
	param.push_back(CV_IMWRITE_PNG_COMPRESSION);
	param.push_back(10);
	cv::imwrite(fileName, img, param);
}

/////////////////////////////Rec2PolarThread////////////////////////////////////
Rec2PolarThread::Rec2PolarThread()
{
	
}

Rec2PolarThread& Rec2PolarThread::convert(const cv::Mat &rectImg, cv::Mat &dst, Rect2PolarData **map, int rowStart, int rowEnd)
{
	this->rectImg = rectImg;
	this->dst = dst;
	this->map = map;
	this->rowStart = rowStart;
	this->rowEnd = rowEnd;
	return (Rec2PolarThread&)*this;
}

void Rec2PolarThread::run()
{
	const int mapWidth = rectImg.cols*2-1;

	for (int row=rowStart; row<rowEnd; ++row)
	{
		Rect2PolarData &head = map[row][0];
		int dataLen = mapWidth - 2*(head.colBegin-1);

		unsigned char *pRow = dst.ptr<unsigned char>(row);
		Rect2PolarData *data = &map[row][0] + 1;

		for (int col=0; col<dataLen; ++col)
		{
			pRow[head.colBegin+col-1] = rectImg.at<unsigned char>(data[col].rowBegin, data[col].colBegin)*data[col].ratio
				+ rectImg.at<unsigned char>(data[col].rowBegin+1, data[col].colBegin)*(1-data[col].ratio) + (1<<15);
		}
	}
}