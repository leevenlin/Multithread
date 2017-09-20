// multithread.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "command.h"
#include "Imshow.h"
#include <opencv2\opencv.hpp>

#include "math.h"
#include "ADSample.h"
#include <queue>
#include "SerialPort.h"

using namespace cv;
using namespace std;

int TotalData = 512 << 20;
unsigned char *totalDataBuf = NULL;

HANDLE hMutex = NULL;//������
					 //�̺߳���

bool Flag_pause = 1;  //�״���ͣ��־λ

DWORD WINAPI GatherThread(LPVOID lpParamter)
{
	//for (int i = 0; i < 10; i++)
	//{
	//	//����һ����������
	//	WaitForSingleObject(hMutex, INFINITE);
	//	cout << "A Thread Fun Display!"<<endl ;
	//	//Sleep(1);
	//	//�ͷŻ�������
	//	ReleaseMutex(hMutex);
	//	//cout << i << "liwenlin" << endl;
	//}
	unsigned int i = 0;
	int nRet = -1;
	unsigned int NumOfBoard = 1;//Specify the number of cards need to operate on.
	ST_CARDINFO stCardInfo[_QTFIRMWARE_NUM_OF_CARD];//card info for per each card on PC mainboard
													//Control multiple cards in sequence
	for (i = 0; i<NumOfBoard; i++)
	{
		QT_FifoMultipleSetup(&stCardInfo[i], i, pmt);
		while (Flag_pause) {};
		QT_FifoMultiple(&stCardInfo[i], i, pmt);
	}
	return 0L;//��ʾ���ص���long�͵�0
}
DWORD WINAPI RecvThread(LPVOID lpParamter)
{
	//��ʼ��socket��
	cout << "TCP" <<sizeof(OilArea2)<< endl;
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsa);
	//tcp�ͻ���
	TCPClient();
	//UDPClient();
	//�����׽�����Դ
	cout << "TCP end" << endl;
	WSACleanup();
	return 0L;//��ʾ���ص���long�͵�0
}
extern  Mat ShowImg;
DWORD WINAPI ShowThread(LPVOID lpParamter)
{
	cout << "imshow!" << endl;
	//Mat tmp_row = ShowImg.row(0).clone();
	//cout << sizeof(tmp_row) << endl;
	ImgShow();
	return 0L;//��ʾ���ص���long�͵�0
}

DWORD WINAPI SaveThread(LPVOID lpParamter)
{
	int i = 0;
	while (i<100000) {
		i++;
		//cout << i << endl;
		Sleep(100);
	};
	return 0L;//��ʾ���ص���long�͵�0
}

CSerialPort port;
int main()
{
	//totalDataBuf = (unsigned char*)malloc(sizeof(char)*TotalData);

	//�򿪴���

	//port.InitPort(1, 76800, 'Y');
	//�����״�

	//�������߳����ȼ�
	hMutex = CreateMutex(NULL, FALSE, _T("screen"));
	//HANDLE hPrimaryThread = GetCurrentThread();
	//SetThreadPriority(hPrimaryThread, THREAD_PRIORITY_NORMAL);
	//������ʾ�߳�
	HANDLE hThread3 = CreateThread(NULL, 0, ShowThread, NULL, 0, NULL);

	////�����ɼ��߳�
	//HANDLE hThread = CreateThread(NULL, 0, GatherThread, NULL, CREATE_SUSPENDED, NULL);
	//SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
	//ResumeThread(hThread);
	////�������վ���
	HANDLE hThread2 = CreateThread(NULL, 0, RecvThread, NULL, 0, NULL);
	//ͼ��洢�߳�
	HANDLE hThread4 = CreateThread(NULL, 0, SaveThread, NULL, 0, NULL);
	//�ر��߳�
//	CloseHandle(hThread);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);
	//���̵߳�ִ��·��
	for (int i = 0; i < 100000; i++)
	{
		//������һ����������
		//WaitForMultipleObjects
		WaitForSingleObject(hMutex, INFINITE);
		//cout << "Main Thread Display!" << endl;
		Sleep(100);
		//Sleep(100);
		//�ͷŻ�������
		ReleaseMutex(hMutex);
	}
	////���߳����
	
	////////
	system("pause");
	return 0;
}
