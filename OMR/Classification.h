#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <dirent.h>
#include <string>

#include "Utilities.h"

using namespace std;
using namespace cv;

class Classification
{
public:
	Classification(string trainDataDir);
	~Classification();

	pair <int, int> PredictImage(Mat img);
	pair <int, int> PredictFeature(Mat feature);

private:
	string trainDataDir;
	vector < vector <string> > fileNames;
	vector < vector <Mat> > allImages;
	vector <CvSVM> svms;

	void train();
	void getFileNames();
	vector <int> getFeature(Mat & img);
};

