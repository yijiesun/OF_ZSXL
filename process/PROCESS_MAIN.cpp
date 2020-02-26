#include <stdio.h>
#include <tchar.h>
#include "opencv2/opencv.hpp"
#include <sstream>
#include <iomanip>
#include "include.h"

void VideoProcessor::setFrameProcessor(void(*process)(Mat &, Mat &)) {
	frameprocessor = 0;
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
	//���Ѵ򿪣��ͷ����´�
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
	//�����ļ���
	Outputfile = filename;
	//�����չ��
	extension.clear();
	//����֡��
	if (framerate == 0.0) {
		framerate = getFrameRate();
	}
	//��ȡ����ԭ��Ƶ�ı��뷽ʽ
	char c[4];
	if (codec == 0) {
		codec = getCodec(c);
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

bool VideoProcessor::setOutput(const string &filename,//·��
	const string &ext,//��չ��
	int numberOfDigits,//����λ��
	int startIndex) {//��ʼ����
	if (numberOfDigits<0)
		return false;
	Outputfile = filename;
	extension = ext;
	digits = numberOfDigits;
	currentIndex = startIndex;
	return true;

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

Size VideoProcessor::getFrameSize() {
	if (images.size() == 0) {
		// ����Ƶ�����֡��С
		int w = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_WIDTH));
		int h = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_HEIGHT));
		return Size(w, h);
	}
	else {
		//��ͼ����֡��С
		cv::Mat tmp = cv::imread(images[0]);
		return (tmp.data) ? (tmp.size()) : (Size(0, 0));
	}
}

bool VideoProcessor::readNextFrame(Mat &frame, bool isFrameNumAdd) {
	if (images.size() == 0)
		return caputure.read(frame);
	else {
		if (itImg != images.end()) {
			frame = imread(*itImg);
			if (isFrameNumAdd)
				itImg++;
			return frame.data ? 1 : 0;
		}
		else
			return false;
	}
}

int VideoProcessor::getCodec(char codec[4]) {
	if (images.size() != 0)
		return -1;
	union { // ���ݽṹ4-char
		int value;
		char code[4];
	} returned;
	//��ñ���ֵ
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
	//�����չ����Ϊ�գ�д��ͼƬ�ļ���
	if (extension.length()) {
		stringstream ss;
		ss << Outputfile << setfill('0') << setw(digits) << currentIndex++ << extension;
		imwrite(ss.str(), frame);
	}
	//��֮��д����Ƶ�ļ���
	else {
		writer.write(frame);
	}
}

void VideoProcessor::run_syj() {
	Mat frame;
	Mat output;
	if (!isOpened())
		return;
	stop = false;
	while (!isStopped()) {
		//��ȡ��һ֡
		if (!readNextFrame(frame))
			break;
		if (!readNextFrame(frame))
			break;
		if (WindowNameInput.length() != 0)
			imshow(WindowNameInput, frame);
		//�����֡
		if (callIt) {
			if (process)
				process(frame, output);
			else if (frameprocessor)
			{
				cout << getFrameNumber();
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
		//������ͣ��������������
		if (delay >= 0 && waitKey(delay) >= 0)
			waitKey(0);
		//����ָ����ͣ�����˳�
		if (frameToStop >= 0 && getFrameNumber() == frameToStop)
			stopIt();
	}

}