#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <utility>

using namespace std;
using namespace cv;

class ImagePreprocess
{
public:
	ImagePreprocess();
	~ImagePreprocess();

	// binarize image using Otsu method
	static void Otsu(vector < vector <int> > & vImage);

	// return first: staff line height; second: staff space height
	static pair<int, int> GetStaffHeight(vector < vector <int> > & vImage);
};

