#pragma once

#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class Utilities
{
public:
	Utilities();
	~Utilities();

	static string ToString(int x);
	static string Padding(string s, int pad);
	static Mat GetImage(vector <Point2i> & pixels);
	static Mat ToFeature(Mat img);
};

