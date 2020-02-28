#ifndef PUTTEXT_H_
#define PUTTEXT_H_
#ifdef win32
#include <string>
#include <opencv2/opencv.hpp>
#include <windows.h>
	using namespace cv;
void GetStringSize(HDC hDC, const char* str, int* w, int* h);
void putTextZH(Mat &dst, const char* str, Point org, Scalar color, int fontSize,
	const char *fn = "Arial", bool italic = false, bool underline = false);
#endif 
#endif// PUTTEXT_H_