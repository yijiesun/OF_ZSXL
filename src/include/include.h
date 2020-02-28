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
	
	string Outputfile;//����ļ���

	int currentIndex;
	int digits;
	string extension;
	FrameProcessor *frameprocessor;
	void(*process)(Mat &, Mat &);//ͼ������ָ��
	bool callIt;
	string WindowNameInput;
	string WindowNameOutput;
	int delay;//��ʱ
	long fnumber;
	//��frameToStopֹͣ
	long frameToStop;
	bool stop;//��ͣ��־
	vector<string> images;//ͼ��������Ϊ������Ƶ��

public:
	
	vector<string>::const_iterator itImg;

	VideoProcessor() : callIt(true), delay(0), fnumber(0), stop(false), digits(0), frameToStop(-1) {}
	void setDownsamplingPercent(float per);
	void setFrameProcessor(void(*process)(Mat &, Mat &));//����ͼ������
	void  setFrameProcessor(FrameProcessor *frameprocessor);
	void displayInput(string wn);//����������Ƶ���Ŵ���
	void displayOutput(string wn);//���������Ƶ���Ŵ���
	void dontDisplay();//���ٴ���
	void run_syj();
	void stopIt() { stop = true; }//��ͣ����λ
	bool isStopped() { return stop; }//��ѯ��ͣ��־λ
	bool isOpened() { return  caputure.isOpened() || !images.empty(); }//������Ƶ�򿪱�־
	void setDelay(int d) { delay = d; }//������ʱ
	bool readNextFrame(Mat &frame, bool isFrameNumAdd = true);//��ȡ��һ֡
	void CallProcess() { callIt = true; }
	void  dontCallProcess() { callIt = false; }
	void stopAtFrameNo(long frame) { frameToStop = frame; }//����ֹͣ֡
	long getFrameNumber();// ��õ�ǰ֡��λ��
	Size getFrameSize(bool isOrg=true);//���֡��С
	double getFrameRate() { return caputure.get(CV_CAP_PROP_FPS); }//��ȡ֡��
	int getCodec();//��ñ�������

	//����Ƶ
	bool setInput(string filename);
	bool setInput(const vector<string> &imgs);
	bool setOutput(const string &filename, int codec = 0, double framerate = 0.0, bool isColor = true);
	//�����Ƶ֡��ͼƬfileme+currentIndex.ext,��filename001.jpg
	bool setOutput(const string &filename,const string &ext,int numberOfDigits = 3,int startIndex = 0);
	void writeNextFrame(Mat &frame);//д��һ֡

};


void RUN_FYGQ();

#endif