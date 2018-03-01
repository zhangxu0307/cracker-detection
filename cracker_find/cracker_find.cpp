// cracker_find.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <vector>
#include<time.h> 

using namespace cv;
using namespace std;

void location(Mat &srcImg,Mat &binImg)
{

    vector< vector<Point> > contours ;
    if(binImg.data)
    {
        findContours(binImg,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	}

    if(contours.size() > 1)
    {
        double maxArea = 0;
        vector <Point> maxContour;
        for(size_t i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if (area > maxArea)
            {
                maxArea = area;
                maxContour = contours[i];
            }
        }
        Rect maxRect;
        Mat ROI;
        if (maxContour.size()!=0)
        {
             maxRect = boundingRect(maxContour);          
        }

        rectangle(srcImg, maxRect, cv::Scalar(0,0,255));
    }
}


void preProcessing(Mat &srcImg,Mat &binImg,int elementSize = 7)
{
	Mat grayImg;
	cvtColor(srcImg,grayImg,CV_RGB2GRAY);
	int blockSize = 25;  
    int constValue = 35;    
    adaptiveThreshold(grayImg, binImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue); 
	//threshold(grayImg,binImg,150,255,THRESH_BINARY_INV);
	Mat element = getStructuringElement(MORPH_RECT, Size(elementSize, elementSize));
	dilate(binImg,binImg,element);
	medianBlur(binImg,binImg,9);
}

int _tmain(int argc, _TCHAR* argv[])
{

	clock_t start,finish;  
    double totaltime;  
	start=clock();

	Mat srcImg = imread("D:\\crack.jpg");
		
	Mat binImg;
	preProcessing(srcImg,binImg);
	imshow("binImg",binImg);
	location(srcImg,binImg);
	imshow("ansImg",srcImg);

	finish=clock();  
    totaltime=(double)(finish-start)/CLOCKS_PER_SEC;  
    cout<<"\n此程序的运行时间为"<<totaltime<<"秒！"<<endl;
	//system("pause");

	waitKey();

	return 0;
}

