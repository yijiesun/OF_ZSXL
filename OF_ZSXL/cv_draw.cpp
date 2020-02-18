/**
@ 1、读取读片
@ 2、显示图片，延迟卡死
@ 3、一直进行鼠标操作，进行绘图，显示图片
*/
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
#include <string>
using namespace cv;

//Mat src;
//Mat dst;

//void on_mouse(Mat &frame, int event, int x, int y, int flags, void* ustc)
//{
//	static Point pre_pt;
//	static Point cur_pt;
//	char temp_1[20];
//	// 如果要在图片的任意位置作为起始点，这两步就不需要了
//	//pre_pt=Point(-1,-1);
//	//cur_pt=Point(-1,-1);
//	if (event == CV_EVENT_LBUTTONDOWN)
//	{
//		dst.copyTo(src);
//		pre_pt = Point(x, y);
//		//sprintf(temp_1,"x:%d,y:%d",x,y);
//		//xiaolei=Rect(x1,y1,0,0);
//		//putText(src,temp_1,Point(x,y),FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255));
//		circle(src, pre_pt, 0.5, cvScalar(255, 0, 0), CV_FILLED, CV_AA, 0);
//		imshow("src", src);
//	}
//	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
//	{
//		dst.copyTo(src);
//		cur_pt = Point(x, y);
//		sprintf(temp_1, "x:%d,y:%d", x, y);
//		//xiaolei=Rect(x1,y1,0,0);
//		putText(src, temp_1, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
//		line(src, pre_pt, cur_pt, cvScalar(0, 255, 0), 1, CV_AA, 0);
//		imshow("src", src);
//	}
//	else if (event == CV_EVENT_LBUTTONUP)
//	{
//		dst.copyTo(src);
//		cur_pt = Point(x, y);
//		sprintf(temp_1, "x:%d,y:%d", x, y);
//		//xiaolei=Rect(x1,y1,0,0);
//		putText(src, temp_1, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 255));
//		circle(src, cur_pt, 3, cvScalar(255, 0, 0), CV_FILLED, CV_AA, 0);
//		line(src, pre_pt, cur_pt, cvScalar(0, 255, 0), 1, CV_AA, 0);
//		imshow("src", src);
//	}
//}

//int main()
//{
//	//注意：这一步必须要有，不然进行不了鼠标操作
//	namedWindow("src", WINDOW_AUTOSIZE);//WINDOW_AUTOSIZE:系统默认,显示自适应
//	src = imread("E:\\SIMIT\\2020_new_project\\zsxl\\dataset\\1.jpg", 1);//1:为原图颜色,0:为灰度图，黑白颜色
//	src.copyTo(dst);
//
//	setMouseCallback("src", on_mouse, 0);
//
//	imshow("src", src);
//	waitKey(0);
//
//	return 0;
//}