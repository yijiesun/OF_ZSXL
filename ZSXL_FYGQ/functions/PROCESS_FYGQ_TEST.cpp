// optical_flow_demo.cpp : 定义控制台应用程序的入口点。
//

#include "include.h"
#include <iostream>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <string>
#include "putText.h"
using namespace std;
using namespace cv;
#define DRAW_TRACK
#define MAX_CORNERS 1000
#define FLOW_DENSITY 5
#define MIN_FLOW_TENSOR 2.0f
#define ARROW_LENGTH 50.0f
#define MOTION_POINT_PER 1.0f //计算质心光流矢量时的最少光流数量  img size/MOTION_POINT_BIAS
#define MAX_TRACK_DIS 5.0f  // 1/5 of img width
#define CONTINUES_FRAME_GOAL 5//A窗口连续多少帧出现向下的矢量，则检测B窗口

//视频opticalFlowFarnebackProcess类，继承自帧处理基类
class PROCESS_FYGQ : public FrameProcessor {
public:
	int status;//0-innit；1-检测A窗口；2-检测B窗口；3-成功；4-失败
	int arrow_down_cnt;//矢量向下帧数计
	Mat firstFrame;
	Mat firstFrameDraw;
	Mat track_line;
	Mat mask;
	int roi_area_size;
	int point_num;
	Point track_priv_cur[2];
    Point cur_pt[4];
	Point cur_pt_lu_rd[3];
	Point LU_POINT;
	Point RU_POINT;
	Point LC_POINT;
	Point RC_POINT;
	Mat gray,gray_roi, mask_roi;  //当前灰度图
	Mat gray_prev, gray_prev_roi, output_roi;  //之前的灰度图

	PROCESS_FYGQ() {}
	void process(Mat &frame, Mat &output) {
		//得到灰度图
		cvtColor(frame, gray, CV_BGR2GRAY);
		frame.copyTo(output);

		//第一帧
		if (gray_prev.empty()) {
			gray.copyTo(gray_prev);
		}
		line(output, cur_pt[0], cur_pt[1], CV_RGB(155, 155, 0), 2, 8);
		line(output, cur_pt[1], cur_pt[2], CV_RGB(155, 155, 0), 2, 8);
		line(output, cur_pt[2], cur_pt[3], CV_RGB(155, 155, 0), 2, 8);
		line(output, cur_pt[0], cur_pt[3], CV_RGB(155, 155, 0), 2, 8);

		line(output, LU_POINT, cur_pt[0], CV_RGB(255, 0, 0), 2, 8);
		line(output, cur_pt[0], cur_pt[3], CV_RGB(255, 0, 0), 2, 8);
		line(output, cur_pt[3], RU_POINT, CV_RGB(255, 0, 0), 2, 8);
		line(output, RU_POINT, LU_POINT, CV_RGB(255, 0, 0), 2, 8);

		line(output, cur_pt[0], LC_POINT, CV_RGB(0, 0, 255), 2, 8);
		line(output, LC_POINT, RC_POINT, CV_RGB(0, 0, 255), 2, 8);
		line(output, RC_POINT, cur_pt[3], CV_RGB(0, 0, 255), 2, 8);
		line(output, cur_pt[0], cur_pt[3], CV_RGB(0, 0, 255), 2, 8);

		if (status == 1)//检测A窗口
		{
			mask_roi = mask(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
			gray_roi = gray(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
			gray_prev_roi = gray_prev(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
			output_roi = output(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[1]));
		}
		else if (status == 2)
		{
			mask_roi = mask(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[2]));
			gray_roi = gray(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[2]));
			gray_prev_roi = gray_prev(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[2]));
			output_roi = output(Rect(cur_pt_lu_rd[0], cur_pt_lu_rd[2]));
		}

		bitwise_and(mask_roi, gray_roi, gray_roi);
		bitwise_and(mask_roi, gray_prev_roi, gray_prev_roi);
		

		//稠密光流
		Mat flow,flow_map[2], flowsum, flowsum_tmp;
		calcOpticalFlowFarneback(gray_prev_roi, gray_roi, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
		Mat flow_xy[2],absflow_xy[2];  // 声明Mat数组，共3个元素，存放分离后的通道

		split(flow, flow_xy);
		absflow_xy[0] = abs(flow_xy[0]);
		flow_xy[0] += 0.5;
		flow_xy[1] += 0.5;
		absflow_xy[1] = abs(flow_xy[1]);
		absflow_xy[0].convertTo(flow_map[0], CV_8UC1);
		absflow_xy[1].convertTo(flow_map[1], CV_8UC1);
		flowsum = flow_map[0] + flow_map[1];
		flowsum_tmp = flowsum -MIN_FLOW_TENSOR;
		//flowsum_tmp = flowsum - 1.0f;
		int flow_non_zero_cnt = countNonZero(flowsum_tmp);
		float flow_per = (float)flow_non_zero_cnt / roi_area_size*100.0f;
		cout <<"-- "<< flow_per << endl;
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
		int x = 0, y = 0;
		if (moment.m00 != 0&& flow_per >= MOTION_POINT_PER)
		{
			
			x = cvRound(moment.m10 / moment.m00);//计算重心横坐标
			y = cvRound(moment.m01 / moment.m00);//计算重心纵坐标
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
		if (status == 1)
		{
			if (flow_sum[1] > 0)
				arrow_down_cnt++;
			else
				arrow_down_cnt = 0;
			if (arrow_down_cnt > CONTINUES_FRAME_GOAL)
			{
				arrow_down_cnt = 0;
				status = 2;
			}
		}
		else if (status == 2)
		{
			if (flow_sum[1] > 0 && y>max(cur_pt[0].y, cur_pt[3].y))
				arrow_down_cnt++;
			else
				arrow_down_cnt = 0;
			if (arrow_down_cnt > CONTINUES_FRAME_GOAL)
			{
				arrow_down_cnt = 0;
				status = 3;
			}
		}

		
		if (status == 1)
		{
			string txt2 = "检测红色A窗口...";
			string txt20 = "运动物体占比 " + fp2string(flow_per);
			string txt21 = "运动物体质心坐标 "+ fp2string(x + cur_pt_lu_rd[0].x) + "," + fp2string(y + cur_pt_lu_rd[0].y);
			string txt22 = "光流矢量和单位向量 " + fp2string(flow_sum[0]) + "," + fp2string(flow_sum[1]);
			string txt23 = "向下光流矢量连续出现次数 " + fp2string(arrow_down_cnt);
			putTextZH(output, txt2.c_str(), Point(20, 120), CV_RGB(255, 0, 0), 30, "华文正楷");
			putTextZH(output, txt20.c_str(), Point(20, 160), CV_RGB(255, 0, 0), 20, "华文正楷");
			putTextZH(output, txt21.c_str(), Point(20, 190), CV_RGB(255, 0, 0), 20, "华文正楷");
			putTextZH(output, txt22.c_str(), Point(20, 210), CV_RGB(255, 0, 0), 20, "华文正楷");
			putTextZH(output, txt23.c_str(), Point(20, 230), CV_RGB(255, 0, 0), 20, "华文正楷");
			
		}
		else if (status == 2)
		{
			string txt2 = "红色A窗口检测成功";
			string txt3 = "检测蓝色B窗口...";
			string txt20 = "运动物体占比 " + fp2string(flow_per);
			string txt21 = "运动物体质心坐标 " + fp2string(x + cur_pt_lu_rd[0].x) + "," + fp2string(y + cur_pt_lu_rd[0].y);
			string txt22 = "光流矢量和单位向量 " + fp2string(flow_sum[0]) + "," + fp2string(flow_sum[1]);
			string txt23 = "向下光流矢量连续出现次数 " + fp2string(arrow_down_cnt);
			putTextZH(output, txt2.c_str(), Point(20, 120), CV_RGB(0, 255, 0), 30, "华文正楷");
			putTextZH(output, txt3.c_str(), Point(20, 160), CV_RGB(255, 0, 0), 30, "华文正楷");
			putTextZH(output, txt20.c_str(), Point(20, 200), CV_RGB(255, 0, 0), 20, "华文正楷");
			putTextZH(output, txt21.c_str(), Point(20, 230), CV_RGB(255, 0, 0), 20, "华文正楷");
			putTextZH(output, txt22.c_str(), Point(20, 260), CV_RGB(255, 0, 0), 20, "华文正楷");
			putTextZH(output, txt23.c_str(), Point(20, 290), CV_RGB(255, 0, 0), 20, "华文正楷");
		}
		else if (status == 3)
		{
			string txt2 = "红色A窗口检测成功";
			string txt3 = "蓝色B窗口检测成功";
			string txt4 = "成绩合格！";
			putTextZH(output, txt2.c_str(), Point(20, 120), CV_RGB(0, 255, 0), 30, "华文正楷");
			putTextZH(output, txt3.c_str(), Point(20, 160), CV_RGB(0, 255, 0), 30, "华文正楷");
			putTextZH(output, txt4.c_str(), Point(20, 200), CV_RGB(255, 0, 0), 60, "华文正楷");

		}
		string txt24 = "规则:\n1,在红色A窗口中检测到连续向下的光流矢量\n2,满足条件1后启动窗口B检测，\nB窗口检测到连续向下的光流矢量\n并且质心在B窗口中";
		string txtc = fp2string(max(cur_pt[0].x, cur_pt[3].x))+","+ fp2string(max(cur_pt[0].y, cur_pt[3].y));
		putTextZH(output, txt24.c_str(), Point(20, 400), CV_RGB(255, 0, 0), 15, "华文正楷");
		putTextZH(output, txtc.c_str(), Point(max(cur_pt[0].x, cur_pt[3].x), max(cur_pt[0].y, cur_pt[3].y)), CV_RGB(255, 255, 0), 15, "华文正楷");
		Mat mask_roi_t;
		mask_roi_t = mask_roi- 230;
		cvtColor(mask_roi_t, mask_roi_t, COLOR_GRAY2RGB);
		mask_roi_t += output_roi;
		mask_roi_t.copyTo(output_roi);
		cv::swap(gray_prev, gray);

	}

	//将opencv的mouse回调函数放在类里面
	static void onMouse(int event, int x, int y, int, void* userdata)
	{
		// Check for null pointer in userdata and handle the error
		PROCESS_FYGQ* temp = reinterpret_cast<PROCESS_FYGQ*>(userdata);
		temp->on_Mouse(event, x, y);
	}

	void on_Mouse(int event, int x, int y)
	{
		if (event == CV_EVENT_LBUTTONUP)
		{
			firstFrame.copyTo(firstFrameDraw);
			cur_pt[int(point_num%4)] = Point(x, y);

			for (int i = 0; i < 4; i++)
			{
				char temp_1[20];
				sprintf_s(temp_1, " %d,%d", cur_pt[i].x, cur_pt[i].y);
				putText(firstFrameDraw, temp_1, cur_pt[i], FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 255));
				circle(firstFrameDraw, cur_pt[i], 3, cvScalar(255, 0, 0), CV_FILLED, CV_AA, 0);
			}

			imshow("rlt", firstFrameDraw);
			point_num++;
		}
	}

	void setup(Mat &frame)
	{
		arrow_down_cnt = 0;
		status = 0;
		point_num = 0;
		frame.copyTo(firstFrame);

		mask = Mat::zeros(firstFrame.size(), CV_8UC1);
		track_line = Mat::zeros(firstFrame.size(), CV_8UC3);
		track_priv_cur[0] = Point(-1, -1);

		imshow("rlt", firstFrame);

		while (1)
		{
			int key = waitKey(10);
			setMouseCallback("rlt", onMouse, this);
			if (key == 13 || key == 'q')
			{
				break;
			}

		}

		cout << "ROI:" << cur_pt[3].x - cur_pt[0].x << endl;
		quadrilateralVertexSort(cur_pt);
		LC_POINT.x = min(cur_pt[0].x, cur_pt[1].x) + cvRound(abs(cur_pt[0].x - cur_pt[1].x) / 2.0);
		LC_POINT.y = min(cur_pt[0].y, cur_pt[1].y) + cvRound(abs(cur_pt[0].y - cur_pt[1].y) / 2.0);
		RC_POINT.x = min(cur_pt[2].x, cur_pt[3].x) + cvRound(abs(cur_pt[2].x - cur_pt[3].x) / 2.0);
		RC_POINT.y = min(cur_pt[2].y, cur_pt[3].y) + cvRound(abs(cur_pt[2].y - cur_pt[3].y) / 2.0);
		findExtendPointOfLine(LC_POINT, cur_pt[0], LU_POINT);
		findExtendPointOfLine(RC_POINT, cur_pt[3], RU_POINT);
		std::vector<cv::Point > contour;
		contour.reserve(4);
		contour.push_back(LU_POINT);
		contour.push_back(LC_POINT);
		contour.push_back(RC_POINT);
		contour.push_back(RU_POINT);
		std::vector<std::vector<cv::Point >> contours;
		contours.push_back(contour);
		cv::fillPoly(mask, contours, 255);

		roi_area_size = countNonZero(mask);

		cur_pt_lu_rd[0].x = min4(LU_POINT.x, cur_pt[0].x, cur_pt[3].x, RU_POINT.x);
		cur_pt_lu_rd[1].x = max4(LU_POINT.x, cur_pt[0].x, cur_pt[3].x, RU_POINT.x);
		cur_pt_lu_rd[0].y = min4(LU_POINT.y, cur_pt[0].y, cur_pt[3].y, RU_POINT.y);
		cur_pt_lu_rd[1].y = max4(LU_POINT.y, cur_pt[0].y, cur_pt[3].y, RU_POINT.y);

		cur_pt_lu_rd[2].x = max4(cur_pt_lu_rd[1].x, cur_pt[1].x, cur_pt[2].x, cur_pt[3].x);
		cur_pt_lu_rd[2].y = max4(cur_pt_lu_rd[1].y, cur_pt[1].y, cur_pt[2].y, cur_pt[3].y);
		status = 1;
	}


};


void RUN_FYGQ()
{
	VideoProcessor processor;
	PROCESS_FYGQ fygq_process;

	//打开输入视频
	//processor.setInput("F:\\video_dataset\\test1.avi");
	processor.setInput("E:\\SIMIT\\2020_new_project\\zsxl\\dataset\\fgq.avi");
	processor.setOutput("E:\\SIMIT\\2020_new_project\\zsxl\\dataset\\result\\OF_ZSXL_FGQ.mp4.avi");
	processor.displayOutput("rlt");
	//设置每一帧的延时
	processor.setDelay(1000. / processor.getFrameRate());
	//设置帧处理函数，可以任意
	processor.setFrameProcessor(&fygq_process);
	Mat frame;
	processor.readNextFrame(frame, false);
	fygq_process.setup(frame);
	processor.run_syj();
}