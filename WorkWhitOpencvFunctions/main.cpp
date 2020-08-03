#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include<vector>
#include<cmath>
#include<opencv2/opencv.hpp>
const double degreToRad = M_PI / 180;

using namespace std;
using namespace cv;
Mat RightRotate(Mat img)
{
	Mat res(img.cols, img.rows, img.type());
	for (auto i = 0; i < img.rows; i++)
	{
		for (auto j = 0; j < img.cols; j++)
		{
			res.at<Vec3b>(j, img.rows - i - 1) = img.at<Vec3b>(i, j);
		}
	}

	return res;
}


cv::Point rotatePoint(const cv::Point& p, double alpha)
{
	auto x1 = p.x * cos((-alpha) * degreToRad) - p.y * sin((-alpha) * degreToRad);
	auto y1 = p.x * sin((-alpha) * degreToRad) + p.y * cos((-alpha) * degreToRad);
	return cv::Point(x1, y1);
}


cv::Mat rotateImage(const cv::Mat& src, double alpha)
{
	cv::Mat dst(src.size(), src.type());
	dst.setTo(0);
	cv::Rect r(0, 0, src.cols, src.rows);
	cv::Point center(src.cols / 2, src.rows / 2);
	for (int i = 0; i < dst.cols; i++)
	{
		for (int j = 0; j < dst.rows; j++)
		{
			cv::Point p(i, j);
			cv::Point rotated = rotatePoint(p - center, alpha) + center;
			if (r.contains(rotated))
			{
				dst.at<Vec3b>(p) = src.at<cv::Vec3b>(rotated);
			}
		}
	}
	return dst;

}


int g_slider_position = 0;
int g_run = 1, g_dontset = 0; //start out in single step mode
cv::VideoCapture g_cap;

void onTrackbarSlide(int pos, void*) {

	g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);

	if (!g_dontset)
		g_run = 1;
	g_dontset = 0;

}


int main()
{


	//string path = "C:/New folder/lamborghini1.jpg";
	Mat img = cv::Mat(1000, 1000, CV_8UC3);
	img.setTo(0);
	int r = 40;

	int endy = 0;
	int starty = 100;
	int level1 = endy;
	int level0 = img.rows - starty;
	int h0 = 10;
	float scale = float(level0) / h0;
	int level0y = level0 + r + 2;
	int level1y = level1 - r - 2;
	cv::line(img, cv::Point(0, level0y), cv::Point(img.cols, level0y), cv::Vec3b(255, 255, 255), 4);
	cv::line(img, cv::Point(0, level1y), cv::Point(img.cols, level1y), cv::Vec3b(255, 255, 255), 4);

	float g = 10.0, v0 = -20.0, t = 0.0, y = 0.0;
	float a = -g;
	float m = 10;
	float energy = m * g * h0  + m * v0 * v0 / 2;
	float energyLooseCoef = 0.4;
	float energyLooseCoef1 = 0.2;
	float y0 = h0;
	int i = 43;
	int speed = 8;
	while (true)
	{
		float y = y0 + v0 * t + (a * t * t) / 2;
		if (y <= 0)
		{
			energy -= energy * energyLooseCoef;
			v0 = sqrt(energy * 2 / m);
			t = 0.0;
			y0 = 0.0;
		}
		//if (y >= 30)
		//{
		//	//energy -= energy * energyLooseCoef1;
		//	v0 = -sqrt((energy - m * g * y) * 2 / m);
		//	t = 0.0;
		//	y0 = 30;
		//	//y = 50;
		//}
		cout << "energy = " << energy << endl;
		cout << "y = " << y << endl;





		if (i >= img.cols - 40 - 2)
		{
			speed = -speed;
		}
		if (i <= 42)
		{
			speed = -speed;
		}
		i += speed;
		int coord = level0 - y * scale;
		Mat tmp = img.clone();
		cv::circle(tmp, Point(img.cols/2, coord), r, Vec3b(0, 255, 0), -1, 50);
		cv::imshow("tmp1", tmp);
		t += 0.01;
		cv::waitKey(10);
	}
	return 0;

}