// multithread.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "command.h"
#include "Imshow.h"

#include "math.h"
#include "ADSample.h"
#include <queue>
#include "SerialPort.h"
using namespace std;

int TotalData = 512 << 20;
unsigned char *totalDataBuf = NULL;

HANDLE hMutex = NULL;//互斥量
					 //线程函数

bool Flag_pause = 1;  //雷达暂停标志位

DWORD WINAPI GatherThread(LPVOID lpParamter)
{
	//for (int i = 0; i < 10; i++)
	//{
	//	//请求一个互斥量锁
	//	WaitForSingleObject(hMutex, INFINITE);
	//	cout << "A Thread Fun Display!"<<endl ;
	//	//Sleep(1);
	//	//释放互斥量锁
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
	return 0L;//表示返回的是long型的0
}
DWORD WINAPI RecvThread(LPVOID lpParamter)
{
	//初始化socket库
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsa);
	//tcp客户端
	TCPClient();
	//UDPClient();
	//清理套接字资源
	WSACleanup();
	return 0L;//表示返回的是long型的0
}
DWORD WINAPI ShowThread(LPVOID lpParamter)
{
	ImgShow();
	return 0L;//表示返回的是long型的0
}

DWORD WINAPI SaveThread(LPVOID lpParamter)
{
	
	return 0L;//表示返回的是long型的0
}

CSerialPort port;
int main()
{
	totalDataBuf = (unsigned char*)malloc(sizeof(char)*TotalData);

	//打开串口

	port.InitPort(1, 76800, 'Y');
	//启动雷达

	//设置主线程优先级
	hMutex = CreateMutex(NULL, FALSE, "screen");
	HANDLE hPrimaryThread = GetCurrentThread();
	SetThreadPriority(hPrimaryThread, THREAD_PRIORITY_NORMAL);
	//创建采集线程
	HANDLE hThread = CreateThread(NULL, 0, GatherThread, NULL, CREATE_SUSPENDED, NULL);
	SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
	ResumeThread(hThread);
	//創建接收線程
	HANDLE hThread2 = CreateThread(NULL, 0, RecvThread, NULL, CREATE_SUSPENDED, NULL);
	SetThreadPriority(hThread2, THREAD_PRIORITY_NORMAL);
	ResumeThread(hThread2);
	//创建显示线程
	HANDLE hThread3 = CreateThread(NULL, 0, ShowThread, NULL, CREATE_SUSPENDED, NULL);
	SetThreadPriority(hThread3, THREAD_PRIORITY_NORMAL);
	ResumeThread(hThread3);
	//图像存储线程
	HANDLE hThread4 = CreateThread(NULL, 0, SaveThread, NULL, CREATE_SUSPENDED, NULL);
	SetThreadPriority(hThread4, THREAD_PRIORITY_NORMAL);
	ResumeThread(hThread4);
	//关闭线程
	CloseHandle(hThread);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);
	//主线程的执行路径
	//for (int i = 0; i < 10; i++)
	//{
	//	//请求获得一个互斥量锁
	//	//WaitForMultipleObjects
	//	WaitForSingleObject(hMutex, INFINITE);
	//	cout << "Main Thread Display!" << endl;
	//	//Sleep(100);
	//	//释放互斥量锁
	//	ReleaseMutex(hMutex);
	//}
	////主线程入口

	////////
	system("pause");
	return 0;
}
