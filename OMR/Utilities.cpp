#include "Utilities.h"

#define oo 100000
#define WHITE 255
#define BLACK 0

Utilities::Utilities()
{
}


Utilities::~Utilities()
{
}

string Utilities::ToString(int x)
{
	if (x == 0) return "0";
	string res = "";
	while (x > 0)
	{
		res = char(x % 10 + 48) + res;
		x /= 10;
	}
	return res;
}

string Utilities::Padding(string s, int x)
{
	int n = s.length();
	for (int i = 0; i < x - n; i++)
		s = "0" + s;
	return s;
}

Mat Utilities::GetImage(vector <Point2i> & pixels)
{
	int minX = oo, maxX= -1, minY = oo, maxY = -1;
	for (int i = 0; i < pixels.size(); i++)
	{
		minX = min(minX, pixels[i].x);
		maxX = max(maxX, pixels[i].x);
		minY = min(minY, pixels[i].y);
		maxY = max(maxY, pixels[i].y);
	}
	int height = maxX - minX + 1, width = maxY - minY + 1;
	Mat result(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			result.at<uchar>(i, j) = WHITE;
	for (int i = 0; i < pixels.size(); i++)
	{
		result.at<uchar>(pixels[i].x - minX, pixels[i].y - minY) = BLACK;
	}
	return result;
}

Mat Utilities::ToFeature(Mat img)
{
	int nRows = img.rows, nCols = img.cols;
	Mat result(nRows * nCols, 1, CV_32FC1);
	int n = 0;
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
			result.at<float>(n++, 0) = img.at<uchar>(i, j);
	return result;
}
