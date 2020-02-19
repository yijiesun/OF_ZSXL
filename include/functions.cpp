#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<stdio.h>
#include <iostream>
#include <string>
#include<sstream>
using namespace cv;
using namespace std;
double getDistance(Point pointO, Point pointA)

{

	double distance;

	distance = powf((pointO.x - pointA.x), 2) + powf((pointO.y - pointA.y), 2);

	distance = sqrtf(distance);



	return distance;

}
//四边形四个顶点排序，输出数组0123分别为lu,ld,rd,ru
void quadrilateralVertexSort(Point *points)
{
	Point px[4];
	Point py[4];
	Point lu, ld, rd, ru;
	memcpy(px, points, 4 * sizeof(Point));
	memcpy(py, points, 4 * sizeof(Point));
	int i, j;
	for (i = 0; i<3; i++)
		for (j = 0; j<3 - i; j++) { 
			if (px[j].x > px[j + 1].x) {
				swap(px[j], px[j + 1]);
			}
		}
	for (i = 0; i<3; i++)
		for (j = 0; j<3 - i; j++) {
			if (py[j].y > py[j + 1].y) {
				swap(py[j], py[j + 1]);
			}
		}
	points[0] = (px[0].y < px[1].y) ? px[0]:px[1];//lu
	points[1] = (px[0].y < px[1].y) ? px[1] : px[0];//ld
	points[2] = (px[2].y < px[3].y) ? px[3] : px[2];//rd
	points[3] = (px[2].y < px[3].y) ? px[2] : px[3];//ru
}

//寻找p1点关于p2点对称的点dst
void findExtendPointOfLine(Point &p1,Point &p2,Point &dst)
{
	dst.x = 2 * p2.x - p1.x;
	dst.y = 2 * p2.y - p1.y;
}


string fp2string(float Num)
{
	ostringstream oss;
	oss << Num;
	string str(oss.str());
	return str;
}

int max4(int a,int b,int c,int d)
{
	int tmp = max(a, b);
	int tmp1 = max(c, d);
	return max(tmp, tmp1);
}

int min4(int a, int b, int c, int d)
{
	int tmp = min(a, b);
	int tmp1 = min(c, d);
	return min(tmp, tmp1);
}