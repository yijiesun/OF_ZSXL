/**
@ 1����ȡ��Ƭ
@ 2����ʾͼƬ���ӳٿ���
@ 3��һֱ���������������л�ͼ����ʾͼƬ
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
//	// ���Ҫ��ͼƬ������λ����Ϊ��ʼ�㣬�������Ͳ���Ҫ��
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
//	//ע�⣺��һ������Ҫ�У���Ȼ���в���������
//	namedWindow("src", WINDOW_AUTOSIZE);//WINDOW_AUTOSIZE:ϵͳĬ��,��ʾ����Ӧ
//	src = imread("E:\\SIMIT\\2020_new_project\\zsxl\\dataset\\1.jpg", 1);//1:Ϊԭͼ��ɫ,0:Ϊ�Ҷ�ͼ���ڰ���ɫ
//	src.copyTo(dst);
//
//	setMouseCallback("src", on_mouse, 0);
//
//	imshow("src", src);
//	waitKey(0);
//
//	return 0;
//}