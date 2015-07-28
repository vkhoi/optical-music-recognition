#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

class StaffLine
{
public:
	StaffLine();
	~StaffLine();

	// add new pixel to the staff along with the pixel's color
	void Add(Point2i pixel, int color);

	// calculate percent of black pixels
	double GetPercentBlackPixels();

	// trim the white pixels from the end of the staff line
	void TrimEnd();

	// get pixels[i]
	Point2i GetPixel(int i);

	// get colors[i]
	int GetColor(int i);

	// get number of pixels
	int GetNumerPixels();

	double GetMeanX();
	int GetMinY();
	int GetMaxY();

private:
	vector <Point2i> pixels;
	vector <int> colors;

	// started = false if no black pixel has been added to the staff
	bool started = false;

	int nBlackPixels, nWhitePixels;

	double sumX, meanX;
	int minY, maxY;

	/*----------------------------------------------*/

};

