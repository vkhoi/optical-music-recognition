#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

class Beam
{
public:
	Beam();
	~Beam();

	void AddPixel(Point2i p);
	int GetNumPixels();
	Point2i GetPixel(int i);

	void Translate(int x, int y);

private:
	vector <Point2i> pixels;
};

