// socketclint.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include "Imshow.h"
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;
void ImgShow()
{
	Mat img = imread("radar2.jpg");
	Mat ShowImg(1080,1080,CV_8UC3,Scalar(0,0,0));
	resize(img, ShowImg, ShowImg.size(), 0, 0, INTER_LINEAR);
	while (true)
	{
		imshow("src", img);
		imshow("1080x1080", ShowImg);
		waitKey(0);
		Sleep(100);
	}

	


}