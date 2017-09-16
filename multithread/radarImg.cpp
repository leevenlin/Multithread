#include "radarImg.h"

#include <QDebug>

RadarImg::RadarImg()
{

}

RadarImg::RadarImg(const std::string fileName)
{
	this->readFile(fileName);
}

RadarImg::~RadarImg(){}

bool RadarImg::readFile(const std::string fileName)
{
	std::ifstream ifs(fileName, std::ios::binary | std::ios::in);

	unsigned char *head = new unsigned char[32];
	ifs.read((char*)head, 32);
	unsigned int dataLen = ((unsigned int)head[26]<<8) + (unsigned int)head[24];

	//std::cout<<dataLen<<std::endl;

	short *data = new short[dataLen*ANGLE_SUM];
	short *dataTmp = new short[dataLen];

	//读取第一个回波数据
	short agleIdx1st = ((unsigned short)head[10]<<4)+((unsigned short)head[8]>>4);
	ifs.read((char*)data, dataLen*sizeof(data[0]));

	int dataIdx = dataLen;//从第二个回波数据开始进入循环
	int cont = 1;
	short agleIdx = 0;
	short agleIdxLast = agleIdx1st;
	bool readGoOn = true;
	short *p;
	while(readGoOn)
	{
		ifs.read((char*)head, 32);
		agleIdx = (((unsigned short)head[10])<<4)+(((unsigned short)head[8])>>4);
		//std::cout<<(int)head[10]<<"\t"<<(int)head[8]<<std::endl;
		//判断数据是否连续,如果连续,则直接保存,否则进行插值或取平均运算
		if( (agleIdx-agleIdxLast)==1 || (agleIdx-agleIdxLast)==(1-4096) )
		{
			ifs.read((char*)(data+cont*dataLen), sizeof(data[0])*dataLen);
			++cont;
		}
		else
		{
			ifs.read((char*)dataTmp, sizeof(data[0])*dataLen);
			//若当前角度计数与上一个角度计数相同,则取平均数,否则进行插值填补空缺
			if (agleIdx == agleIdxLast)
			{
				p = data+(cont-1)*dataLen;
				meanArr(p, dataTmp, dataLen);
			}
			else
			{
				int dataLose = (agleIdx-agleIdxLast)>0 ? (agleIdx-agleIdxLast) : (agleIdx-agleIdxLast+ANGLE_SUM);
				p = data+(cont-1)*dataLen;
				int interpNum = dataLose;
				cont += dataLose;
				if(cont > ANGLE_SUM)
					interpNum -= (cont - ANGLE_SUM);
				interpArr(p, dataTmp, dataLen, dataLose, interpNum);
			}
		}
		agleIdxLast = agleIdx;
		//是否继续读取数据
		if (cont >= ANGLE_SUM)
		{
			readGoOn = false;
		}
	}
	//转换图象,以角度计数1开始,4096结束
	short *data2 = new short[dataLen*ANGLE_SUM];
	memcpy(data2, data+dataLen*(ANGLE_SUM-agleIdx1st), dataLen*(agleIdx1st)*sizeof(data[0]));
	memcpy(data2+dataLen*agleIdx1st, data, dataLen*(ANGLE_SUM-agleIdx1st)*sizeof(data[0]));
	cv::Mat(cont, dataLen, CV_16SC1, data2).convertTo(radarImgRcd, CV_32FC1, 1./(1<<16), 0.5);

	radarImgRcd.push_back(radarImgRcd.rowRange(0, 2));

	delete[] data2;    data2   = NULL;
	delete[] dataTmp;  dataTmp = NULL;
	delete[] data;     data    = NULL;
	delete[] head;     head    = NULL;
	ifs.close();
	return true;
}

int RadarImg::readFile(std::ifstream &ifs, cv::Mat lastImg, int lastAgl, FileList &list)
{
	unsigned char *head = new unsigned char[32];
	ifs.read((char*)head, 32);
	unsigned int dataLen = ((unsigned int)head[26]<<8) + (unsigned int)head[24];
	ifs.seekg(-32, std::ios::cur);//重置文件输入流指针到输入时的状态

	//std::cout<<dataLen<<std::endl;

	short *data = new short[dataLen*(ANGLE_SUM+1)];
	short *dataTmp = new short[dataLen];

	//读取第一个回波数据
	short agleIdx1st = lastAgl;
	int idx = lastImg.rows<ANGLE_SUM ? (lastImg.rows-1):(lastAgl-1);
	short *pLastImgLastRow = lastImg.ptr<short>(idx);
	memcpy(data, pLastImgLastRow, sizeof(short)*dataLen);

	//int dataIdx = dataLen;//从第二个回波数据开始进入循环
	int cont = 1;
	short agleIdx = 0;
	short agleIdxLast = agleIdx1st;
	bool readGoOn = true;
	short *p;

	int returnFlg = 0;

	while(readGoOn)
	{
		ifs.read((char*)head, 32);
		agleIdx = (((unsigned short)head[10])<<4)+(((unsigned short)head[8])>>4);
		//std::cout<<(int)head[10]<<"\t"<<(int)head[8]<<std::endl;
		//判断数据是否连续,如果连续,则直接保存,否则进行插值或取平均运算
		if( (agleIdx-agleIdxLast)==1 || (agleIdx-agleIdxLast)==(1-ANGLE_SUM) )
		{
			ifs.read((char*)(data+cont*dataLen), sizeof(data[0])*dataLen);
			++cont;
		}
		else
		{
			ifs.read((char*)dataTmp, sizeof(data[0])*dataLen);
			//若当前角度计数与上一个角度计数相同,则取平均数,否则进行插值填补空缺
			if (agleIdx == agleIdxLast)
			{
				p = data+(cont-1)*dataLen;
				meanArr(p, dataTmp, dataLen);
			}
			else
			{
				int dataLose = (agleIdx-agleIdxLast)>0 ? (agleIdx-agleIdxLast) : (agleIdx-agleIdxLast+ANGLE_SUM);
				p = data+(cont-1)*dataLen;
				int interpNum = dataLose;
				cont += dataLose;
				if(cont > ANGLE_SUM+1)
					interpNum -= (cont - (ANGLE_SUM+1));
				interpArr(p, dataTmp, dataLen, dataLose, interpNum);
			}
		}
		agleIdxLast = agleIdx;
		//是否继续读取数据
		if (cont > ANGLE_SUM)
		{
			readGoOn = false;
		}

		if (ifs.eof())
		{
			if (list.hasNext())
			{
				ifs.close();
				ifs.open(list.getNextFileName(), std::ios::binary | std::ios::in);
			}
			else
			{
				returnFlg = returnFlg | RadarImg::READ_FILE_NOMOREFILE;
				readGoOn = false;
			}
		}
	}
	//转换图象,以角度计数1开始,4096结束
	short *data2 = new short[dataLen*ANGLE_SUM];
	memcpy(data2, data+dataLen*(ANGLE_SUM-agleIdx1st+1), dataLen*(agleIdx1st)*sizeof(data[0]));
	memcpy(data2+dataLen*agleIdx1st, data+dataLen, dataLen*(ANGLE_SUM-agleIdx1st)*sizeof(data[0]));
	//cv::Mat(ANGLE_SUM, dataLen, CV_16SC1, data2).convertTo(radarImgRcd, CV_32FC1, 1./(1<<16), 0.5);
	radarImgRcd = cv::Mat(ANGLE_SUM, dataLen, CV_16SC1, data2).clone();

	radarImgRcd.push_back(radarImgRcd.rowRange(0, 2));

	delete[] data2;    data2   = NULL;
	delete[] dataTmp;  dataTmp = NULL;
	delete[] data;     data    = NULL;
	delete[] head;     head    = NULL;

	return returnFlg;
}

//cv::Mat& RadarImg::getRadarImg() const
//{
//	return (cv::Mat&) radarImg;
//}

cv::Mat& RadarImg::getRadarImgRcd() const
{
	//cv::Mat img = radarImgRcd;

	//std::vector<int> param;
	//param.push_back(CV_IMWRITE_PNG_COMPRESSION);
	//param.push_back(10);
	//cv::imwrite("radarImg_rect.png", img*256, param);

	//cv::namedWindow("demo", cv::WINDOW_NORMAL);
	//cv::imshow("demo", img);
	//cv::waitKey();
	//cv::destroyWindow("demo");
	return (cv::Mat&)radarImgRcd;
}

void RadarImg::convImg()
{
	//Rect2PolarData head;
	//std::ifstream read(Rect2PolarMap::RECT2POLAR_FILE_NAME, std::ios::binary | std::ios::in);
	//int mapWidth = this->radarImgRcd.cols*2-1;
	//Rect2PolarData *data = new Rect2PolarData[mapWidth];
	//int rows = mapWidth;
	//this->radarImg = cv::Mat(rows, mapWidth, this->radarImgRcd.type(), cv::Scalar(0));
	//for (int row=0; row<radarImg.rows; ++row)
	//{
	//	read.read((char*)&head, sizeof(Rect2PolarData));
	//	int dataLen = mapWidth - 2*(head.colBegin-1);
	//	//if(dataLen < 1 || dataLen>mapWidth)
	//	//{
	//	//	std::cout<<row<<"\t"<<dataLen<<std::endl;
	//	//	system("pause");
	//	//}
	//	
	//	read.read((char*)data, sizeof(Rect2PolarData)*dataLen);
	//	float *pRow = radarImg.ptr<float>(row);
	//	//qDebug()<<row;
	//	for (int col=0; col<dataLen; ++col)
	//	{
	//		//if(row == 8175)
	//			//qDebug()<<data[col].rowBegin<<"\t"<<data[col].colBegin<<"\t"<<data[col].ratio;
	//		pRow[head.colBegin+col-1] = radarImgRcd.at<float>(data[col].rowBegin, data[col].colBegin)*data[col].ratio
	//			+ radarImgRcd.at<float>(data[col].rowBegin+1, data[col].colBegin)*(1-data[col].ratio);
	//		//std::cout<<pRow[head.colBegin+col-1]<<std::endl;
	//	}
	//}

	//cv::Mat uint8Img;
	//radarImg.convertTo(uint8Img, CV_8UC1, 256);

	//cv::equalizeHist(uint8Img, uint8Img);


	////cv::namedWindow("demo", cv::WINDOW_NORMAL);
	////cv::imshow("demo", radarImg);
	////cv::waitKey();
	////cv::destroyWindow("demo");

	//std::vector<int> param;
	//param.push_back(CV_IMWRITE_PNG_COMPRESSION);
	//param.push_back(10);
	////cv::imwrite("radarImg_ploar.png", radarImg*256, param);
	////cv::imwrite("radarImg_ploar_equ.png", uint8Img, param);

	//cv::Mat medianFiltedImg;
	//cv::GaussianBlur(radarImg*256, medianFiltedImg, cv::Size(5, 5), 2, 2);
	//cv::imwrite("radarImg_ploar_gaussian.png", medianFiltedImg, param);

	//delete[] data;
	//data = NULL;
}

void RadarImg::convRect2Polar(const cv::Mat &rectImg, cv::Mat &dst)
{
	Rect2PolarData head;
	std::ifstream read(Rect2PolarMap::RECT2POLAR_FILE_NAME, std::ios::binary | std::ios::in);
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

	//cv::Mat uint8Img;
	//radarImg.convertTo(uint8Img, CV_8UC1, 256);

	//cv::equalizeHist(uint8Img, uint8Img);

	//std::vector<int> param;
	//param.push_back(CV_IMWRITE_PNG_COMPRESSION);
	//param.push_back(10);

	//cv::Mat medianFiltedImg;
	//cv::GaussianBlur(radarImg*256, medianFiltedImg, cv::Size(5, 5), 2, 2);
	//cv::imwrite("radarImg_ploar_gaussian.png", medianFiltedImg, param);

	delete[] data;  data = NULL;
}

void RadarImg::convRect2Polar_RAM(const cv::Mat &rectImg, cv::Mat &dst)
{
	Rect2PolarData *head;
	std::ifstream read(Rect2PolarMap::RECT2POLAR_FILE_NAME, std::ios::binary | std::ios::in);
	int mapWidth = rectImg.cols*2-1;
	long fileSize = read.seekg(0, std::ios_base::end).tellg();
	int dataNum = fileSize/sizeof(Rect2PolarData);
	Rect2PolarData *data = new Rect2PolarData[dataNum];
	read.seekg(0, std::ios::beg);
	read.read((char*)data, fileSize);
	int rows = mapWidth;

	dst = cv::Mat(rows, mapWidth, rectImg.type(), cv::Scalar(0));
	//cv::Mat dst = dst;
	Rect2PolarData *pdata = data;
	for (int row=0; row<dst.rows; ++row)
	{
		//qDebug()<<row;
		//read.read((char*)&head, sizeof(Rect2PolarData));
		head = pdata;
		++pdata;
		int dataLen = mapWidth - 2*(head->colBegin-1);

		//read.read((char*)data, sizeof(Rect2PolarData)*dataLen);
		short *pRow = dst.ptr<short>(row);

		for (int col=0; col<dataLen; ++col)
		{
			pRow[head->colBegin+col-1] = rectImg.at<short>(pdata[col].rowBegin, pdata[col].colBegin)*pdata[col].ratio
				+ rectImg.at<short>(pdata[col].rowBegin+1, pdata[col].colBegin)*(1-pdata[col].ratio);
		}
		pdata += dataLen;
	}
	delete[] data;
	data = NULL;
}

const int RadarImg::READ_FILE_SUCCESS    = 1;
const int RadarImg::READ_FILE_EOF        = 1<<1;
const int RadarImg::READ_FILE_NOMOREFILE = 1<<2;

//private方法
void RadarImg::meanArr(short* reslut, short* added, int arrLen)
{
	for (int cont=0; cont<arrLen; ++cont)
	{
		reslut[cont] = (short)(((float)(reslut[cont]+added[cont]))*0.5);
	}
}

void RadarImg::interpArr(short* a, short *b, int arrLen, int range, int interpNum)
{
	for (int cont=1; cont<=interpNum; ++cont)
	{
		int idx0 = cont*arrLen;
		double k = ((double)cont)/((double)range);
		for (int idx=0; idx<arrLen; ++idx)
		{
			a[idx0+idx] = a[idx] + (short)(((double)(b[idx]-a[idx]))*k);
		}
	}
	//memcpy(a+interpNum*arrLen, b, sizeof(a[0])*arrLen);
}



