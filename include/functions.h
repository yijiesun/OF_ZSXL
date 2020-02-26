#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
#include <iostream>
#include <string>
#include<sstream>
using namespace std;
using namespace cv;

double getDistance(Point pointO, Point pointA);
void quadrilateralVertexSort(Point *points);
void findExtendPointOfLine(Point &p1, Point &p2, Point &dst);
string fp2string(float Num);
int max4(int a, int b, int c, int d);
int min4(int a, int b, int c, int d);