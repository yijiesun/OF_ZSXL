#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <strstream>

using namespace std;
using namespace cv;



double getDistance(Point pointO, Point pointA);
void quadrilateralVertexSort(Point *points);
void findExtendPointOfLine(Point &p1, Point &p2, Point &dst);
string fp2string(float Num);
string int2string(int Num);
int max4(int a, int b, int c, int d);
int min4(int a, int b, int c, int d);
//h.m.s.ms
string getCurrentDate();
double what_time_is_it_now_s();
class DEBUG {
public:
	VideoWriter writer;
	string outVideoFile;
	FILE *fp;
	DEBUG()
	{
	};
	void setup(string logFile)
	{
		if ((fp = fopen(logFile.c_str(), "w")) == NULL) {
			puts("Fail to open log file!");
		}
	};
	void writeAndShowLog(string log)
	{
		string output = getCurrentDate();
		output += log;
		output += "\n";
		printf(output.c_str());
		fprintf(fp, "%s", output.c_str());
	};

	template<class src_type>
	void writeAndShowLog(string log,src_type src)
	{
		strstream ss;
		ss << src;
		string ret;
		ss >> ret;

		string output = getCurrentDate();
		output += log;
		output += ret;
		output += "\n";
		printf(output.c_str());
		fprintf(fp, "%s", output.c_str());

	};

	bool setVideoOutput(int codec, double framerate, bool isColor,Size size)
	{
		return writer.open(outVideoFile,
			codec,
			framerate,
			size,
			isColor);
	}

	void writeVidFrame(Mat &frame) {
		if(!outVideoFile.empty())
			writer.write(frame);
	}

	void close()
	{
		fclose(fp);
		if (!outVideoFile.empty()) writer.release();
	};
};