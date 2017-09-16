#include "rect2polarmap.h"

//******************************************************************************************
//*                                ֱ������ת�����귽��
//******************************************************************************************
const int Rect2PolarMap::DATA_STORE_RAM               = 0;
const int Rect2PolarMap::DATA_STORE_HD                = 1;
const std::string Rect2PolarMap::RECT2POLAR_FILE_NAME = "convData.dat";

Rect2PolarMap::~Rect2PolarMap()
{
	clearMap();
}

const Rect2PolarMap& Rect2PolarMap::getInstance(int angleSum, int dataPerSamp)
{
	if(angleSum==instance.getAngleSum() && dataPerSamp==instance.getDataPerSamp())
		return (Rect2PolarMap&)instance;

	instance.setMap(angleSum, dataPerSamp);
	return (Rect2PolarMap&)instance;
}

const Rect2PolarMap& Rect2PolarMap::getInstance()
{
	if(instance.getAngleSum()>0 && instance.getDataPerSamp()>0)
		return (Rect2PolarMap&)instance;
	
	return getInstance(4096, 8176);
}

void Rect2PolarMap::setMap(int angleSum, int dataPerSamp)
{
	clearMap();
	if (rect2ploarFileExists())//�ж�ֱ������ת������ת���ļ��Ƿ����
	{
		std::ifstream readFile(RECT2POLAR_FILE_NAME, std::ios::binary | std::ios::in);
		int angle_data[2];
		readFile.read((char*)angle_data, sizeof(angle_data)/sizeof(angle_data[0]));
		readFile.close();

		if ((angle_data[0] = angleSum) && (angle_data[1] = dataPerSamp))//�ж�ת���ļ��Ƿ��뵱ǰ����ƥ�䣬��ƥ�䣬�򷵻�
		{
			this->angleSum    = angleSum;
			this->dataPerSamp = dataPerSamp;
			return;
		}
	}

	//��ת���ļ���ƥ�䣬�����¼���ת������

	this->angleSum    = angleSum;
	this->dataPerSamp = dataPerSamp;

	const int mapWidth = dataPerSamp*2-1;
	Rect2PolarData *data = new Rect2PolarData[mapWidth+1];//��Ҫ����ÿ��head,��˳�ΪmapWidth+1

	int dist2 = (dataPerSamp-1)*(dataPerSamp-1);
	std::ofstream saveMap(RECT2POLAR_FILE_NAME, std::ios::binary | std::ios::out);

	int angle_data[] = {angleSum, dataPerSamp};
	saveMap.write((char*)angle_data, sizeof(angle_data)/sizeof(angle_data[0]));

	for (int row=0; row<mapWidth; ++row)
	{
		int colIdx = -1;
		for (int col=0; col<mapWidth; ++col)
		{
			if((row-dataPerSamp)*(row-dataPerSamp)+(col-dataPerSamp)*(col-dataPerSamp) <= dist2)
			{
				colIdx = col;
				break;
			}
		}
		if (colIdx == -1)
			continue;

		data[0].rowBegin = row;
		data[0].colBegin = colIdx;
		data[0].ratio    = -1;
		double theta     = 0;
		double dist      = 0;
		int dataCnt      = 1;//���ڴ洢data�ļ�����
		double aglIdx    = 0;
		double aglStep_1 = 1/(2*PI/4096);//�����ز�֮��Ƕȵĵ���
		for (int col=colIdx; col<mapWidth-colIdx+2; ++col, ++dataCnt)
		{
			if (row == dataPerSamp)
			{
				if (col<dataPerSamp)
					theta = 1.5*PI;
				else
					theta = 0.5*PI;
				dist = col-dataPerSamp>0 ? (col-dataPerSamp):(dataPerSamp-col);
			} 
			else
			{
				dist = std::sqrt((double)(row-dataPerSamp)*(row-dataPerSamp) + (col-dataPerSamp)*(col-dataPerSamp));
				theta = std::asin((col-dataPerSamp)/dist);
				if(row<dataPerSamp && col<dataPerSamp)
					theta += 2*PI;
				else if(row>=dataPerSamp && col>=dataPerSamp)
					theta = PI-theta;
				else if(row>=dataPerSamp && col<dataPerSamp)
					theta = PI-theta;
			}
 			aglIdx                 = theta*aglStep_1;
			data[dataCnt].ratio    = (float)(1 - (aglIdx - (int)aglIdx));
			data[dataCnt].rowBegin = (unsigned short)(aglIdx+0.5);
			data[dataCnt].colBegin = (unsigned short)(dist+0.5);
		}
		saveMap.write((char*)data, sizeof(Rect2PolarData)*(mapWidth-2*colIdx+3));
	}
	saveMap.close();
	
}

inline const int Rect2PolarMap::getAngleSum() const
{
	return angleSum;
}

inline const int Rect2PolarMap::getDataPerSamp() const
{
	return dataPerSamp;
}

Rect2PolarData** Rect2PolarMap::getMap()
{
	if(map == NULL)
	{
		const int mapWidth = 2*dataPerSamp-1;
		Rect2PolarData head;
		std::ifstream read(Rect2PolarMap::RECT2POLAR_FILE_NAME, std::ios::binary | std::ios::in);
		int angle_data[2];
		read.read((char*)angle_data, sizeof(angle_data)/sizeof(angle_data[0]));

		map = new Rect2PolarData*[mapWidth];
		for(int row=0; row<mapWidth; ++row)
		{
			read.read((char*)&head, sizeof(Rect2PolarData));
			int dataLen = mapWidth - 2*(head.colBegin-1);
			map[row] = new Rect2PolarData[dataLen + 1];//�豣��head����ÿ�г���ΪdataLen+1;
			map[row][0] = head;
			read.read((char*)(map[row] + 1), sizeof(Rect2PolarData)*dataLen);
		}
		read.close();
	}
	
	return map;
}

//private
Rect2PolarMap::Rect2PolarMap()
{
	angleSum    = 0;
	dataPerSamp = 0;
	map         = NULL;
}

void Rect2PolarMap::clearMap()
{
	if(map != NULL)
	{
		for (int cont=0; cont<this->dataPerSamp*2-1; ++cont)
		{
			delete[] map[cont];
			map[cont] = NULL;
		}
		delete[] map;
		map = NULL;
	}
}

bool Rect2PolarMap::rect2ploarFileExists() const
{
	std::fstream file;
	file.open(RECT2POLAR_FILE_NAME, std::ios::in);
	if (!file)
	{
		return false;
	}
	else
	{
		file.close();
		return true;
	}
}

Rect2PolarMap Rect2PolarMap::instance = Rect2PolarMap();