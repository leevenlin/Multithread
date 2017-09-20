// socketclint.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include "conio.h"
#include "windows.h"
#include "winsock.h"
#include "command.h"
#pragma comment(lib,"ws2_32.lib")

extern void GetData(char Recv[]);

SOCKET sockclient;
void TCPClient()
{
	//创建socket
	sockclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == sockclient)
	{ 
		return;
	}
	//连接服务器，建立服务器端套接字地址
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5678);
	//对于inet_addr()函数，它是把“xxx.xxx.xxx.xxx”形式表示的IPV4地址，转换为IN_ADDR结构体能够
	//接收的形式（unsigned long型，因为IN_ADDR结构体中的负责接收的S_addr成员变量的类型是unsigned long型）
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本机ip

	//向服务器发出连接请求，当然我们也可以通过connet函数的返回值判断到底有无连接成功。
	int iRetVal = connect(sockclient,(struct sockaddr*)&addr,sizeof(addr));
	printf("server connect error!");
	if(SOCKET_ERROR == iRetVal)
	{
		printf("服务器连接失败！");
		closesocket(sockclient);
		return;
	}
	printf("服务器连接成功！\n");
	//数据收发
	CHAR szSend[100] = "hello server";   //客户端  先发后收
	send(sockclient,szSend,sizeof(szSend),0);  //发送函数，可以通过返回值判断发送成功与否。
	printf("%d\n", sizeof(szSend));
	//接收服务器回传的数据
	while (1)
	{
		recv(sockclient, TCPRecv, TCP_SIZE, 0); //接收函数
		GetData(TCPRecv);
		printf("%s\n", TCPRecv);//表示以字符串的格式输出服务器端发送的内容。
		Sleep(100);
	}
	//  getch();//暂停一下
	//关闭socket
	closesocket(sockclient);
}
void UDPClient()
{
	//创建SOCKET ，ip协议族，数据报方式，udp协议。
	SOCKET sockclient = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(INVALID_SOCKET == sockclient)
	{
		return ;
	}
	//数据收发，服务器端套接字地址
	struct sockaddr_in svraddr = {0};
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(5780);
	svraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//指定服务器端的IP与端口。
	CHAR szSend[100] = "hello udp server";
	/*此函数先是客户端的套接字，然后是服务器端地址*/
	//简单理解为:从函数前面的客户端套接字的发送数据缓存区中将数发送给服务器端地址
	sendto(sockclient,szSend,100,0,(struct sockaddr*)&svraddr,sizeof(svraddr));//发送时构造ip地址和端口。

	//注：该程序也可以接收服务器端回传的数据。
	//简单理解为：从函数后面的服务器端地址中取数到客户端套接字的接收缓冲区szRecv中
	int len = sizeof(svraddr);
	while (1)
	{
		recvfrom(sockclient, UDPRecv, UDP_SIZE, 0, (struct sockaddr*)&svraddr, &len);
		UDPData(UDPRecv);
		printf("%s \n", UDPRecv);
		Sleep(100);
	}
	//关闭socket
	closesocket(sockclient);
}

//int main(int argc, _TCHAR* argv[])
//{
//	//初始化socket库
//	WSADATA wsa = {0};
//	WSAStartup(MAKEWORD(2,2),&wsa);
//	//tcp客户端
//	TCPClient();
//	//UDPClient();
//	//清理套接字资源
//	WSACleanup();
//	getch();
//
//
//	return 0;
//}


