// optical_flow_demo.cpp : 定义控制台应用程序的入口点。
//

#include "include.h"
#include <iostream>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <string>
using namespace std;
using namespace cv;
#define DRAW_TRACK
#define MAX_CORNERS 1000
#define FLOW_DENSITY 5
#define SET_POINT_NUM 4
#define MIN_FLOW_TENSOR 1.0f
#define ARROW_LENGTH 50.0f
#define MOTION_POINT_BIAS 10.0f
#define MAX_TRACK_DIS 5.0f  // 1/5 of img width
Mat src;
Mat dst;
Mat track_line;
Mat mask;
int point_num;
Point track_priv_cur[2];
Point cur_pt[SET_POINT_NUM];
Point cur_pt_lu_rd[SET_POINT_NUM/2];
//视频opticalFlowFarnebackProcess类，继承自帧处理基类
class OF_Farneback : public FrameProcessor {
	Mat gray,gray_roi, mask_roi;  //当前灰度图
	Mat gray_prev, gray_prev_roi, output_roi;  //之前的灰度图

public:
	OF_Farneback() {}
	void process(Mat &frame, Mat &output) {
		//得到灰度图
		cvtColor(frame, gray, CV_BGR2GRAY);
		frame.copyTo(output);

		//第一帧
		if (gray_prev.empty()) {
			gray.copyTo(gray_prev);
		}
		line(output, cur_pt[0], cur_pt[1], CV_RGB(255, 0, 0), 1, 8);
		line(output, cur_pt[1], cur_pt[2], CV_RGB(255, 0, 0), 1, 8);
		line(output, cur_pt[2], cur_pt[3], CV_RGB(255, 0, 0), 1, 8);
		line(output, cur_pt[0], cur_pt[3], CV_RGB(255, 0, 0), 1, 8);

		mask_roi = mask(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
		gray_roi = gray(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
		gray_prev_roi = gray_prev(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
		output_roi = output(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
		bitwise_and(mask_roi, gray_roi, gray_roi);
		bitwise_and(mask_roi, gray_prev_roi, gray_prev_roi);
		

		//稠密光流
		Mat flow,flow_map[2], flowsum, flowsum_tmp;
		calcOpticalFlowFarneback(gray_prev_roi, gray_roi, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
		Mat flow_xy[2],absflow_xy[2];  // 声明Mat数组，共3个元素，存放分离后的通道

		split(flow, flow_xy);
		absflow_xy[0] = abs(flow_xy[0]);
		absflow_xy[1] = abs(flow_xy[1]);
		absflow_xy[0].convertTo(flow_map[0], CV_8U);
		absflow_xy[1].convertTo(flow_map[1], CV_8U);
		flowsum = flow_map[0] + flow_map[1];
		flowsum_tmp = flowsum - MIN_FLOW_TENSOR;
		//flowsum_tmp = flowsum - 1.0f;
		int flow_non_zero_cnt = countNonZero(flowsum_tmp);

		threshold(flowsum, flowsum, MIN_FLOW_TENSOR, 255, CV_THRESH_BINARY);
		//threshold(flowsum, flowsum, 1.0f, 255, CV_THRESH_BINARY);

		Moments moment = moments(flowsum, true);
		Scalar  flow_sum = sum(flow);
		float aa = flow_sum[0];
		float len_bias = sqrt(flow_sum[0] * flow_sum[0] + flow_sum[1] * flow_sum[1]) / ARROW_LENGTH;
		flow_sum[0] /= len_bias;
		flow_sum[1] /= len_bias;
		//cvtColor(flowsum, flowsum, COLOR_GRAY2RGB);
		//flowsum.copyTo(output_roi);

		for (size_t y = 0; y<gray_prev_roi.rows; y += FLOW_DENSITY) {
			for (size_t x = 0; x<gray_prev_roi.cols; x += FLOW_DENSITY) {
				Point2f fxy = flow.at<Point2f>(y, x);
				if(mask_roi.at<uchar>(y,x) !=0 && (abs(fxy.x)>= MIN_FLOW_TENSOR|| abs(fxy.y) >= MIN_FLOW_TENSOR))
					line(output, Point(x + cur_pt_lu_rd[0].x, y + cur_pt_lu_rd[0].y), Point(cvRound(x + fxy.x + cur_pt_lu_rd[0].x), cvRound(y + fxy.y + cur_pt_lu_rd[0].y)), CV_RGB(0, 255, 0), 1, 8);
			}
		}
		if (moment.m00 != 0&& flow_non_zero_cnt>= (cur_pt_lu_rd[0].x*cur_pt_lu_rd[0].y)/ MOTION_POINT_BIAS)//除数不能为0
		{
			int x = cvRound(moment.m10 / moment.m00);//计算重心横坐标
			int y = cvRound(moment.m01 / moment.m00);//计算重心纵坐标
			if (track_priv_cur[0].x == -1 && track_priv_cur[0].y == -1)
			{
				track_priv_cur[1] = Point(x + cur_pt_lu_rd[0].x, y + cur_pt_lu_rd[0].y);
				track_priv_cur[0] = track_priv_cur[1];
			}
				track_priv_cur[1] = Point(x + cur_pt_lu_rd[0].x, y + cur_pt_lu_rd[0].y);
			if (getDistance(track_priv_cur[0], track_priv_cur[1]) > gray_prev_roi.cols / MAX_TRACK_DIS)
				track_priv_cur[1] = track_priv_cur[0];

#ifdef DRAW_TRACK
			line(track_line, track_priv_cur[0], track_priv_cur[1], CV_RGB(155, 155, 0), 1, 8);
#endif
			circle(output, Point(x + cur_pt_lu_rd[0].x, y + cur_pt_lu_rd[0].y), 5, CV_RGB(255, 0, 0));
			arrowedLine(output, Point(x + cur_pt_lu_rd[0].x, y + cur_pt_lu_rd[0].y), Point(cvRound(x + flow_sum[0] + cur_pt_lu_rd[0].x),
				y + flow_sum[1] + cur_pt_lu_rd[0].y), CV_RGB(255, 0, 0), 2, 8);

			
			track_priv_cur[0] = track_priv_cur[1];

		}
#ifdef DRAW_TRACK
		output += track_line;
#endif
		Mat mask_roi_t;
		mask_roi_t = mask_roi- 230;
		cvtColor(mask_roi_t, mask_roi_t, COLOR_GRAY2RGB);
		mask_roi_t += output_roi;
		mask_roi_t.copyTo(output_roi);
		cv::swap(gray_prev, gray);

	}

};

void on_mouse(int event, int x, int y, int flags, void* ustc)
{
	if (event == CV_EVENT_LBUTTONUP)
	{
		dst.copyTo(src);
		cur_pt[int(point_num%SET_POINT_NUM)] = Point(x, y);
		
		for (int i = 0; i < SET_POINT_NUM; i++)
		{
			char temp_1[20];
			sprintf_s(temp_1, " %d,%d", cur_pt[i].x, cur_pt[i].y);
			putText(src, temp_1, cur_pt[i], FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 255));
			circle(src, cur_pt[i], 3, cvScalar(255, 0, 0), CV_FILLED, CV_AA, 0);
		}

		imshow("rlt", src);
		point_num++;
	}
}
void process()
{
	point_num = 0;
	Mat curr_frame;
	VideoProcessor processor;
	OF_Farneback of_fb;
	//打开输入视频
	//processor.setInput("F:\\video_dataset\\test1.avi");
	processor.setInput("E:\\SIMIT\\2020_new_project\\zsxl\\dataset\\fgq.avi");
	processor.setOutput("E:\\SIMIT\\2020_new_project\\zsxl\\dataset\\result\\OF_ZSXL_FGQ.avi");
	processor.displayOutput("rlt");
	//设置每一帧的延时
	processor.setDelay(1000. / processor.getFrameRate());
	//设置帧处理函数，可以任意
	processor.setFrameProcessor(&of_fb);

	processor.readNextFrame(src,false);
	src.copyTo(dst);
	mask = Mat::zeros(src.size(), CV_8UC1);
	track_line = Mat::zeros(src.size(), CV_8UC3);
	track_priv_cur[0] = Point(-1, -1);
	while (1)
	{
		int key = waitKey(10);
		setMouseCallback("rlt", on_mouse, 0);
		imshow("rlt", src);
		if (key == 13 || key == 'q')
		{
			//if (cur_pt[0].x > cur_pt[1].x)
			//	swap(cur_pt[0], cur_pt[1]);
			break;
		}
			
	}
	quadrilateralVertexSort(cur_pt);
	std::vector<cv::Point > contour;
	contour.reserve(4);
	contour.push_back(cur_pt[0]);
	contour.push_back(cur_pt[1]);
	contour.push_back(cur_pt[2]);
	contour.push_back(cur_pt[3]);
	std::vector<std::vector<cv::Point >> contours;
	contours.push_back(contour);
	cv::fillPoly(mask, contours, 255);

	int minx= cur_pt[0].x, miny= cur_pt[0].y, maxx=0, maxy=0;
	for (int i = 0; i < 4; i++)
	{
		minx = minx > cur_pt[i].x ? cur_pt[i].x : minx;
		miny = miny > cur_pt[i].y ? cur_pt[i].y : miny;
		maxx = maxx < cur_pt[i].x ? cur_pt[i].x : maxx;
		maxy = maxy < cur_pt[i].y ? cur_pt[i].y : maxy;
	}
	cur_pt_lu_rd[0].x = minx; cur_pt_lu_rd[1].x = maxx;
	cur_pt_lu_rd[0].y = miny; cur_pt_lu_rd[1].y = maxy;

	processor.run_syj();
}
