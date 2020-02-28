#ifndef INCLUDE_H
#define INCLUDE_H
#include <stdio.h>
#ifdef win32
#include <tchar.h>
#endif
#include "opencv2/opencv.hpp"
#include <sstream>
#include <iomanip>
#include "functions.h"

using namespace std;
using namespace cv;

class FrameProcessor;
class FrameProcessor {
public:
	virtual void process(Mat &input, Mat &ouput) = 0;
};

class VideoProcessor {
private:
	float frame_downsampling_percent;
	VideoCapture caputure;
	VideoWriter writer;
	
	string Outputfile;//输出文件名

	int currentIndex;
	int digits;
	string extension;
	FrameProcessor *frameprocessor;
	void(*process)(Mat &, Mat &);//图像处理函数指针
	bool callIt;
	string WindowNameInput;
	string WindowNameOutput;
	int delay;//延时
	long fnumber;
	//第frameToStop停止
	long frameToStop;
	bool stop;//暂停标志
	vector<string> images;//图像序列作为输入视频流

public:
	
	vector<string>::const_iterator itImg;

	VideoProcessor() : callIt(true), delay(0), fnumber(0), stop(false), digits(0), frameToStop(-1) {}
	void setDownsamplingPercent(float per);
	void setFrameProcessor(void(*process)(Mat &, Mat &));//设置图像处理函数
	void  setFrameProcessor(FrameProcessor *frameprocessor);
	void displayInput(string wn);//设置输入视频播放窗口
	void displayOutput(string wn);//设置输出视频播放窗口
	void dontDisplay();//销毁窗口
	void run_syj();
	void stopIt() { stop = true; }//暂停键置位
	bool isStopped() { return stop; }//查询暂停标志位
	bool isOpened() { return  caputure.isOpened() || !images.empty(); }//返回视频打开标志
	void setDelay(int d) { delay = d; }//设置延时
	bool readNextFrame(Mat &frame, bool isFrameNumAdd = true);//读取下一帧
	void CallProcess() { callIt = true; }
	void  dontCallProcess() { callIt = false; }
	void stopAtFrameNo(long frame) { frameToStop = frame; }//设置停止帧
	long getFrameNumber();// 获得当前帧的位置
	Size getFrameSize(bool isOrg=true);//获得帧大小
	double getFrameRate() { return caputure.get(CV_CAP_PROP_FPS); }//获取帧率
	int getCodec();//获得编码类型

	//打开视频
	bool setInput(string filename);
	bool setInput(const vector<string> &imgs);
	bool setOutput(const string &filename, int codec = 0, double framerate = 0.0, bool isColor = true);
	//输出视频帧到图片fileme+currentIndex.ext,如filename001.jpg
	bool setOutput(const string &filename,const string &ext,int numberOfDigits = 3,int startIndex = 0);
	void writeNextFrame(Mat &frame);//写下一帧

};


void RUN_FYGQ();

#endif