#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#include "StaffLine.h"

using namespace cv;
using namespace std;

class Stem
{
public:
	Stem();
	~Stem();

	void AddPixel(Point2i p);
	int GetNumPixels();
	Point2i GetPixel(int i);

	Point2i GetMinX();
	Point2i GetMaxX();

	bool MergeStem(Stem & stem);
	Stem FilterStem(int maxWidth, vector < vector <int> > & vBoard);

	int GetWidth();
	int GetHeight();

	void Translate(int x, int y);

	bool IsFarFromStaff(int staffSpaceHeight, vector <StaffLine> & s);

private:
	vector <Point2i> pixels;
	Point2i minX, maxX;
	int x0, y0, x1, y1;

	bool isAdjacent(Point2i a, Point2i b);
};

