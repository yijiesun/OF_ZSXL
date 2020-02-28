#include <stdio.h>
#ifdef win32
#include <tchar.h>
#endif
#include "opencv2/opencv.hpp"
#include <sstream>
#include <iomanip>
#include "include.h"

void VideoProcessor::setFrameProcessor(void(*process)(Mat &, Mat &)) {
	frameprocessor = 0;
	frame_downsampling_percent = 1;
	this->process = process;
	CallProcess();
}

void  VideoProcessor::setFrameProcessor(FrameProcessor *frameprocessor) {
	process = 0;
	this->frameprocessor = frameprocessor;
	CallProcess();
}

bool VideoProcessor::setInput(string filename) {
	fnumber = 0;
	//若已打开，释放重新打开
	caputure.release();
	return caputure.open(filename);
}

bool VideoProcessor::setInput(const vector<string> &imgs) {
	fnumber = 0;
	caputure.release();
	images = imgs;
	itImg = images.begin();
	return true;
}

bool VideoProcessor::setOutput(const string &filename, int codec, double framerate, bool isColor) {
	//设置文件名
	Outputfile = filename;
	//清空扩展名
	extension.clear();
	//设置帧率
	if (framerate == 0.0) {
		framerate = getFrameRate();
	}
	//获取输入原视频的编码方式

	if (codec == 0) {
		codec = getCodec();
	}
	return writer.open(Outputfile,
		codec,
		framerate,
		getFrameSize(),
		isColor);
	//return writer.open(Outputfile,
	//	CV_FOURCC('m', 'p', '4', 'v'),
	//	framerate,
	//	getFrameSize(),
	//	isColor);
}

bool VideoProcessor::setOutput(const string &filename,//路径
	const string &ext,//扩展名
	int numberOfDigits,//数字位数
	int startIndex) {//起始索引
	if (numberOfDigits<0)
		return false;
	Outputfile = filename;
	extension = ext;
	digits = numberOfDigits;
	currentIndex = startIndex;
	return true;

}

void VideoProcessor::setDownsamplingPercent(float per)
{
	frame_downsampling_percent = per;
}

void VideoProcessor::displayInput(string wn) {
	WindowNameInput = wn;
	namedWindow(WindowNameInput, WINDOW_AUTOSIZE);
	//namedWindow(WindowNameInput);
}

void VideoProcessor::displayOutput(string wn) {
	WindowNameOutput = wn;
	namedWindow(WindowNameOutput);
}

void VideoProcessor::dontDisplay() {
	destroyWindow(WindowNameInput);
	destroyWindow(WindowNameOutput);
	WindowNameInput.clear();
	WindowNameOutput.clear();
}

long VideoProcessor::getFrameNumber() {
	long fnumber = static_cast<long>(caputure.get((CV_CAP_PROP_POS_FRAMES)));
	return fnumber;
}

Size VideoProcessor::getFrameSize(bool isOrg) {

	// 从视频流获得帧大小
	int w = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_WIDTH));
	int h = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_HEIGHT));
	if (!isOrg&&frame_downsampling_percent != 1)
	{
		w = static_cast<int>(w/ frame_downsampling_percent+0.5);
		h = static_cast<int>(h / frame_downsampling_percent + 0.5);
	}
	return Size(w, h);

}

bool VideoProcessor::readNextFrame(Mat &frame, bool isFrameNumAdd) {

	bool ret = caputure.read(frame);
	if(!ret)
		return ret;
	if (frame_downsampling_percent != 1)
		resize(frame, frame, getFrameSize(false));
	return ret;

}

int VideoProcessor::getCodec() {
	char codec[4];
	if (images.size() != 0)
		return -1;
	union { // 数据结构4-char
		int value;
		char code[4];
	} returned;
	//获得编码值
	returned.value = static_cast<int>(
		caputure.get(CV_CAP_PROP_FOURCC));
	// get the 4 characters
	codec[0] = returned.code[0];
	codec[1] = returned.code[1];
	codec[2] = returned.code[2];
	codec[3] = returned.code[3];
	return returned.value;
}

void VideoProcessor::writeNextFrame(Mat &frame) {
	if(!Outputfile.empty())
		writer.write(frame);
	
}

void VideoProcessor::run_syj() {
	Mat frame;
	Mat output;
	if (!isOpened())
		return;
	stop = false;
	while (!isStopped()) {
		//读取下一帧
		if (!readNextFrame(frame))
			break;
		if (WindowNameInput.length() != 0)
			imshow(WindowNameInput, frame);
		//处理该帧
		if (callIt) {
			if (process)
				process(frame, output);
			else if (frameprocessor)
			{
				frameprocessor->process(frame, output);
			}

		}
		else {
			output = frame;
		}
		if (Outputfile.length()) {
			//cvtColor(output, output, CV_GRAY2BGR);
			writeNextFrame(output);
		}
		if (WindowNameOutput.length() != 0)
			imshow(WindowNameOutput, output);
		//按键暂停，继续按键继续
		// if (delay >= 0 && waitKey(delay) >= 0)
		waitKey(10);
		//到达指定暂停键，退出
		if (frameToStop >= 0 && getFrameNumber() == frameToStop)
			stopIt();
	}
	if (!Outputfile.empty()) writer.release();
	caputure.release();
}