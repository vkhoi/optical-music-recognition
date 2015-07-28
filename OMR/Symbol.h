#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>

#include "StaffLine.h"

using namespace std;
using namespace cv;

class Symbol
{
public:
	Symbol();
	~Symbol();

	void AddPixel(Point2i p);
	int GetNumPixels();
	Point2i GetPixel(int i);
	vector <Point2i> GetAllPixels();

	bool IsSymbol(int staffLineHeight);
	bool IsNote(int staffLineHeight, vector < vector <int> > & vImage);
	bool IsWholeNote(int staffSpaceHeight, vector < vector <int> > & vImage);
	bool IsFarFromStaff(int staffSpaceHeight, int label, vector <StaffLine> & s);
	bool IsGClef(int staffSpaceHeight);
	bool IsAccidental(int staffSpaceHeight);
	bool IsRest(int staffSpaceHeight);

private:
	vector <Point2i> pixels;

	bool isInside(int x, int y, int rows, int cols);

	int x0, y0, x1, y1;
};

